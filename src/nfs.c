/**
 * collectd - src/nfs.c
 * Copyright (C) 2005,2006  Jason Pepas
 * Copyright (C) 2012,2013  Florian Forster
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; only version 2 of the License is applicable.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * Authors:
 *   Jason Pepas <cell at ices.utexas.edu>
 *   Florian octo Forster <octo at collectd.org>
 *   Cosmin Ioiart <cioiart at gmail.com>
 **/

#include "collectd.h"

#include "plugin.h"
#include "utils/common/common.h"

#if KERNEL_NETBSD
// clang-format off
/*
 * Explicit order is required or it will not compile, see GitHub issue #3333
 */
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/sysctl.h>
#include <nfs/rpcv2.h>
#include <nfs/nfsproto.h>
#include <nfs/nfs.h>
// clang-format on
#endif

#if HAVE_KSTAT_H
#include <kstat.h>
/* #endif  HAVE_KSTAT_H */
#elif HAVE_PERFSTAT
# include <libperfstat.h>
#endif /* HAVE_PERFSTAT */

static const char *config_keys[] = {"ReportV2", "ReportV3", "ReportV4"};
static int config_keys_num = STATIC_ARRAY_SIZE(config_keys);
static bool report_v2 = true;
static bool report_v3 = true;
static bool report_v4 = true;

/*
see /proc/net/rpc/nfs
see http://www.missioncriticallinux.com/orph/NFS-Statistics

net x x x x
rpc_stat.netcnt         Not used; always zero.
rpc_stat.netudpcnt      Not used; always zero.
rpc_stat.nettcpcnt      Not used; always zero.
rpc_stat.nettcpconn     Not used; always zero.

rpc x x x
rpc_stat.rpccnt             The number of RPC calls.
rpc_stat.rpcretrans         The number of retransmitted RPC calls.
rpc_stat.rpcauthrefresh     The number of credential refreshes.

proc2 x x x...
proc3 x x x...

Procedure   NFS Version NFS Version 3
Number      Procedures  Procedures

0           null        null
1           getattr     getattr
2           setattr     setattr
3           root        lookup
4           lookup      access
5           readlink    readlink
6           read        read
7           wrcache     write
8           write       create
9           create      mkdir
10          remove      symlink
11          rename      mknod
12          link        remove
13          symlink     rmdir
14          mkdir       rename
15          rmdir       link
16          readdir     readdir
17          fsstat      readdirplus
18                      fsstat
19                      fsinfo
20                      pathconf
21                      commit
*/

#if KERNEL_LINUX || HAVE_LIBKSTAT
static const char *nfs2_procedures_names[] = {
    "null", "getattr", "setattr", "root",   "lookup",  "readlink",
    "read", "wrcache", "write",   "create", "remove",  "rename",
    "link", "symlink", "mkdir",   "rmdir",  "readdir", "fsstat"};
static size_t nfs2_procedures_names_num =
    STATIC_ARRAY_SIZE(nfs2_procedures_names);
#endif

static const char *nfs3_procedures_names[] = {
    "null",   "getattr", "setattr",  "lookup", "access",  "readlink",
    "read",   "write",   "create",   "mkdir",  "symlink", "mknod",
    "remove", "rmdir",   "rename",   "link",   "readdir", "readdirplus",
    "fsstat", "fsinfo",  "pathconf", "commit"};
static size_t nfs3_procedures_names_num =
    STATIC_ARRAY_SIZE(nfs3_procedures_names);

#if HAVE_LIBKSTAT
static const char *nfs4_procedures_names[] = {"null",
                                              "compound",
                                              "reserved",
                                              "access",
                                              "close",
                                              "commit",
                                              "create",
                                              "delegpurge",
                                              "delegreturn",
                                              "getattr",
                                              "getfh",
                                              "link",
                                              "lock",
                                              "lockt",
                                              "locku",
                                              "lookup",
                                              "lookupp",
                                              "nverify",
                                              "open",
                                              "openattr",
                                              "open_confirm",
                                              "open_downgrade",
                                              "putfh",
                                              "putpubfh",
                                              "putrootfh",
                                              "read",
                                              "readdir",
                                              "readlink",
                                              "remove",
                                              "rename",
                                              "renew",
                                              "restorefh",
                                              "savefh",
                                              "secinfo",
                                              "setattr",
                                              "setclientid",
                                              "setclientid_confirm",
                                              "verify",
                                              "write"};
static size_t nfs4_procedures_names_num =
    STATIC_ARRAY_SIZE(nfs4_procedures_names);
#endif

#if KERNEL_LINUX
static const char *nfs4_server40_procedures_names[] = {"null",
                                                       "compound",
                                                       "reserved",
                                                       "access",
                                                       "close",
                                                       "commit",
                                                       "create",
                                                       "delegpurge",
                                                       "delegreturn",
                                                       "getattr",
                                                       "getfh",
                                                       "link",
                                                       "lock",
                                                       "lockt",
                                                       "locku",
                                                       "lookup",
                                                       "lookupp",
                                                       "nverify",
                                                       "open",
                                                       "openattr",
                                                       "open_confirm",
                                                       "open_downgrade",
                                                       "putfh",
                                                       "putpubfh",
                                                       "putrootfh",
                                                       "read",
                                                       "readdir",
                                                       "readlink",
                                                       "remove",
                                                       "rename",
                                                       "renew",
                                                       "restorefh",
                                                       "savefh",
                                                       "secinfo",
                                                       "setattr",
                                                       "setclientid",
                                                       "setcltid_confirm",
                                                       "verify",
                                                       "write",
                                                       "release_lockowner"};

static size_t nfs4_server40_procedures_names_num =
    STATIC_ARRAY_SIZE(nfs4_server40_procedures_names);

static const char *nfs4_server4x_procedures_names[] = {
    /* NFS 4.1 */
    "backchannel_ctl", "bind_conn_to_session", "exchange_id", "create_session",
    "destroy_session", "free_stateid", "get_dir_delegation", "getdeviceinfo",
    "getdevicelist", "layoutcommit", "layoutget", "layoutreturn",
    "secinfo_no_name", "sequence", "set_ssv", "test_stateid", "want_delegation",
    "destroy_clientid", "reclaim_complete",
    /* NFS 4.2 */
    "allocate",      /* 3.18 */
    "copy",          /* 3.18 */
    "copy_notify",   /* 3.18 */
    "deallocate",    /* 3.18 */
    "ioadvise",      /* 3.18 */
    "layouterror",   /* 3.18 */
    "layoutstats",   /* 3.18 */
    "offloadcancel", /* 3.18 */
    "offloadstatus", /* 3.18 */
    "readplus",      /* 3.18 */
    "seek",          /* 3.18 */
    "write_same",    /* 3.18 */
    "clone"          /* 4.5 */
};

#define NFS4_SERVER40_NUM_PROC                                                 \
  (STATIC_ARRAY_SIZE(nfs4_server40_procedures_names))

#define NFS4_SERVER4X_NUM_PROC                                                 \
  (STATIC_ARRAY_SIZE(nfs4_server40_procedures_names) +                         \
   STATIC_ARRAY_SIZE(nfs4_server4x_procedures_names))

#define NFS4_SERVER_MAX_PROC (NFS4_SERVER4X_NUM_PROC)

static const char *nfs4_client40_procedures_names[] = {
    "null",
    "read",
    "write",
    "commit",
    "open",
    "open_confirm",
    "open_noattr",
    "open_downgrade",
    "close",
    "setattr",
    "fsinfo",
    "renew",
    "setclientid",
    "setclientid_confirm",
    "lock",
    "lockt",
    "locku",
    "access",
    "getattr",
    "lookup",
    "lookupp",
    "remove",
    "rename",
    "link",
    "symlink",
    "create",
    "pathconf",
    "statfs",
    "readlink",
    "readdir",
    "server_caps",
    "delegreturn",
    "getacl",
    "setacl",
    "fs_locations",      /* |35| 2.6.18 */
    "release_lockowner", /* |42| 2.6.36 */
    "secinfo",           /* |46| 2.6.39 */
    "fsid_present"       /* |54| 3.13 */
};

static const char *nfs4_client4x_procedures_names[] = {
    /* NFS 4.1 */
    "exchange_id",          /* |40| 2.6.30 */
    "create_session",       /* |40| 2.6.30 */
    "destroy_session",      /* |40| 2.6.30 */
    "sequence",             /* |40| 2.6.30 */
    "get_lease_time",       /* |40| 2.6.30 */
    "reclaim_complete",     /* |41| 2.6.33 */
    "layoutget",            /* |44| 2.6.37 */
    "getdeviceinfo",        /* |44| 2.6.37 */
    "layoutcommit",         /* |46| 2.6.39 */
    "layoutreturn",         /* |47| 3.0 */
    "secinfo_no_name",      /* |51| 3.1 */
    "test_stateid",         /* |51| 3.1 */
    "free_stateid",         /* |51| 3.1 */
    "getdevicelist",        /* |51| 3.1 */
    "bind_conn_to_session", /* |53| 3.5 */
    "destroy_clientid",     /* |53| 3.5 */
    /* NFS 4.2 */
    "seek",        /* |55| 3.18 */
    "allocate",    /* |57| 3.19 */
    "deallocate",  /* |57| 3.19 */
    "layoutstats", /* |58| 4.2 */
    "clone",       /* |59| 4.4 */
    "copy"         /* |60| 4.7 */
};

#define NFS4_CLIENT40_NUM_PROC                                                 \
  (STATIC_ARRAY_SIZE(nfs4_client40_procedures_names))

#define NFS4_CLIENT4X_NUM_PROC                                                 \
  (STATIC_ARRAY_SIZE(nfs4_client40_procedures_names) +                         \
   STATIC_ARRAY_SIZE(nfs4_client4x_procedures_names))

#define NFS4_CLIENT_MAX_PROC (NFS4_CLIENT4X_NUM_PROC)

#endif

#if HAVE_PERFSTAT
static const char *nfs4_client_procedures_names[] =
{
	"null",
	"getattr",
	"setattr",
	"lookup",
	"access",
	"readlink",
	"read",
	"write",
	"create",
	"mkdir",
	"symlink",
	"mknod",
	"remove",
	"rmdir",
	"rename",
	"link",
	"readdir",
	"statfs",
	"finfo",
	"commit",
	"open",
	"open_confirm",
	"open_downgrade",
	"close",
	"lock",
	"unlock",
	"lock_test",
	"set_clientid",
	"renew",
	"client_confirm",
	"secinfo",
	"release_lock",
	"replicate",
	"pcl_stat",
	"acl_stat_l",
	"pcl_stat_l",
	"acl_read",
	"pcl_read",
	"acl_write",
	"pcl_write",
	"delegreturn"
};
static size_t nfs4_client_procedures_names_num = STATIC_ARRAY_SIZE (nfs4_client_procedures_names);

static const char *nfs4_server_procedures_names[] =
{
	"null",
	"compound",
	"access",
	"close",
	"commit",
	"create",
	"delegpurge",
	"delegreturn",
	"getattr",
	"getfh",
	"link",
	"lock",
	"lockt",
	"locku",
	"lookup",
	"lookupp",
	"nverify",
	"open",
	"openattr",
	"open_confirm",
	"open_downgrade",
	"putfh",
	"putpubfh",
	"putrootfh",
	"read",
	"readdir",
	"readlink",
	"remove",
	"rename",
	"renew",
	"restorefh",
	"savefh",
	"secinfo",
	"setattr",
	"set_clientid",
	"clientid_confirm",
	"verify",
	"write",
	"release_lock"
};
static size_t nfs4_server_procedures_names_num = STATIC_ARRAY_SIZE (nfs4_server_procedures_names);

#endif

#if HAVE_LIBKSTAT
extern kstat_ctl_t *kc;
static kstat_t *nfs2_ksp_client;
static kstat_t *nfs2_ksp_server;
static kstat_t *nfs3_ksp_client;
static kstat_t *nfs3_ksp_server;
static kstat_t *nfs4_ksp_client;
static kstat_t *nfs4_ksp_server;
#endif

static int nfs_config(const char *key, const char *value) {
  if (strcasecmp(key, "ReportV2") == 0)
    report_v2 = IS_TRUE(value);
  else if (strcasecmp(key, "ReportV3") == 0)
    report_v3 = IS_TRUE(value);
  else if (strcasecmp(key, "ReportV4") == 0)
    report_v4 = IS_TRUE(value);
  else
    return -1;

  return 0;
}

#if KERNEL_LINUX || KERNEL_NETBSD
static int nfs_init(void) { return 0; }
/* #endif KERNEL_LINUX */

#elif HAVE_LIBKSTAT
static int nfs_init(void) {
  nfs2_ksp_client = NULL;
  nfs2_ksp_server = NULL;
  nfs3_ksp_client = NULL;
  nfs3_ksp_server = NULL;
  nfs4_ksp_client = NULL;
  nfs4_ksp_server = NULL;

  if (kc == NULL)
    return -1;

  for (kstat_t *ksp_chain = kc->kc_chain; ksp_chain != NULL;
       ksp_chain = ksp_chain->ks_next) {
    if (strncmp(ksp_chain->ks_module, "nfs", 3) != 0)
      continue;
    else if (strncmp(ksp_chain->ks_name, "rfsproccnt_v2", 13) == 0)
      nfs2_ksp_server = ksp_chain;
    else if (strncmp(ksp_chain->ks_name, "rfsproccnt_v3", 13) == 0)
      nfs3_ksp_server = ksp_chain;
    else if (strncmp(ksp_chain->ks_name, "rfsproccnt_v4", 13) == 0)
      nfs4_ksp_server = ksp_chain;
    else if (strncmp(ksp_chain->ks_name, "rfsreqcnt_v2", 12) == 0)
      nfs2_ksp_client = ksp_chain;
    else if (strncmp(ksp_chain->ks_name, "rfsreqcnt_v3", 12) == 0)
      nfs3_ksp_client = ksp_chain;
    else if (strncmp(ksp_chain->ks_name, "rfsreqcnt_v4", 12) == 0)
      nfs4_ksp_client = ksp_chain;
  }

  return 0;
} /* int nfs_init */
/* #endif HAVE_LIBKSTAT */

#elif HAVE_PERFSTAT
static int nfs_init (void)
{
	return (0);
}
#endif /* HAVE_PERFSTAT */


static void nfs_procedures_submit(const char *plugin_instance,
                                  const char **type_instances, value_t *values,
                                  size_t values_num) {
  value_list_t vl = VALUE_LIST_INIT;

  vl.values_len = 1;
  sstrncpy(vl.plugin, "nfs", sizeof(vl.plugin));
  sstrncpy(vl.plugin_instance, plugin_instance, sizeof(vl.plugin_instance));
  sstrncpy(vl.type, "nfs_procedure", sizeof(vl.type));

  for (size_t i = 0; i < values_num; i++) {
    vl.values = values + i;
    sstrncpy(vl.type_instance, type_instances[i], sizeof(vl.type_instance));
    plugin_dispatch_values(&vl);
  }
} /* void nfs_procedures_submit */

#if KERNEL_LINUX
static void nfs_submit_fields(int nfs_version, const char *instance,
                              char **fields, size_t fields_num,
                              const char **proc_names) {
  char plugin_instance[DATA_MAX_NAME_LEN];
  value_t values[fields_num];

  snprintf(plugin_instance, sizeof(plugin_instance), "v%i%s", nfs_version,
           instance);

  for (size_t i = 0; i < fields_num; i++)
    (void)parse_value(fields[i], &values[i], DS_TYPE_DERIVE);

  nfs_procedures_submit(plugin_instance, proc_names, values, fields_num);
}

static int nfs_submit_fields_safe(int nfs_version, const char *instance,
                                  char **fields, size_t fields_num,
                                  const char **proc_names,
                                  size_t proc_names_num) {
  if (fields_num != proc_names_num) {
    WARNING("nfs plugin: Wrong number of fields for "
            "NFSv%i %s statistics. Expected %" PRIsz ", got %" PRIsz ".",
            nfs_version, instance, proc_names_num, fields_num);
    return EINVAL;
  }

  nfs_submit_fields(nfs_version, instance, fields, fields_num, proc_names);

  return 0;
}

static int nfs_submit_nfs4_server(const char *instance, char **fields,
                                  size_t fields_num) {
  static int suppress_warning;
  size_t proc4x_names_num;

  switch (fields_num) {
  case NFS4_SERVER40_NUM_PROC:
  case NFS4_SERVER40_NUM_PROC + 19: /* NFS 4.1 */
  case NFS4_SERVER40_NUM_PROC + 31: /* NFS 4.2 */
  case NFS4_SERVER40_NUM_PROC + 32: /* NFS 4.2 */
    break;
  default:
    if (!suppress_warning) {
      WARNING("nfs plugin: Unexpected number of fields for "
              "NFSv4 %s statistics: %" PRIsz ". ",
              instance, fields_num);
    }

    if (fields_num > NFS4_SERVER_MAX_PROC) {
      fields_num = NFS4_SERVER_MAX_PROC;
      suppress_warning = 1;
    } else {
      return EINVAL;
    }
  }

  nfs_submit_fields(4, instance, fields, nfs4_server40_procedures_names_num,
                    nfs4_server40_procedures_names);

  if (fields_num > nfs4_server40_procedures_names_num) {
    proc4x_names_num = fields_num - nfs4_server40_procedures_names_num;
    fields += nfs4_server40_procedures_names_num;

    nfs_submit_fields(4, instance, fields, proc4x_names_num,
                      nfs4_server4x_procedures_names);
  }

  return 0;
}

static int nfs_submit_nfs4_client(const char *instance, char **fields,
                                  size_t fields_num) {
  size_t proc40_names_num, proc4x_names_num;

  static int suppress_warning;

  switch (fields_num) {
  case 34:
  case 35:
  case 36:
  case 37:
  case 38:
    /* 4.0-only configuration */
    proc40_names_num = fields_num;
    break;
  case 40:
  case 41:
    proc40_names_num = 35;
    break;
  case 42:
  case 44:
    proc40_names_num = 36;
    break;
  case 46:
  case 47:
  case 51:
  case 53:
    proc40_names_num = 37;
    break;
  case 54:
  case 55:
  case 57:
  case 58:
  case 59:
  case 60:
    proc40_names_num = 38;
    break;
  default:
    if (!suppress_warning) {
      WARNING("nfs plugin: Unexpected number of fields for NFSv4 %s "
              "statistics: %" PRIsz ". ",
              instance, fields_num);
    }

    if (fields_num > 34) {
      /* safe fallback to basic nfs40 procedures */
      fields_num = 34;
      proc40_names_num = 34;

      suppress_warning = 1;
    } else {
      return EINVAL;
    }
  }

  nfs_submit_fields(4, instance, fields, proc40_names_num,
                    nfs4_client40_procedures_names);

  if (fields_num > proc40_names_num) {
    proc4x_names_num = fields_num - proc40_names_num;
    fields += proc40_names_num;

    nfs_submit_fields(4, instance, fields, proc4x_names_num,
                      nfs4_client4x_procedures_names);
  }

  return 0;
}

static void nfs_read_linux(FILE *fh, const char *inst) {
  char buffer[1024];

  // The stats line is prefixed with type and number of fields, thus plus 2
  char *fields[MAX(NFS4_SERVER_MAX_PROC, NFS4_CLIENT_MAX_PROC) + 2];
  int fields_num = 0;

  if (fh == NULL)
    return;

  while (fgets(buffer, sizeof(buffer), fh) != NULL) {
    fields_num = strsplit(buffer, fields, STATIC_ARRAY_SIZE(fields));

    if (fields_num < 3)
      continue;

    if (strcmp(fields[0], "proc2") == 0 && report_v2) {
      nfs_submit_fields_safe(/* version = */ 2, inst, fields + 2,
                             (size_t)(fields_num - 2), nfs2_procedures_names,
                             nfs2_procedures_names_num);
    } else if (strncmp(fields[0], "proc3", 5) == 0 && report_v3) {
      nfs_submit_fields_safe(/* version = */ 3, inst, fields + 2,
                             (size_t)(fields_num - 2), nfs3_procedures_names,
                             nfs3_procedures_names_num);
    } else if (strcmp(fields[0], "proc4ops") == 0 && report_v4) {
      if (inst[0] == 's')
        nfs_submit_nfs4_server(inst, fields + 2, (size_t)(fields_num - 2));
    } else if (strcmp(fields[0], "proc4") == 0 && report_v4) {
      if (inst[0] == 'c')
        nfs_submit_nfs4_client(inst, fields + 2, (size_t)(fields_num - 2));
    }
  } /* while (fgets) */
} /* void nfs_read_linux */
#endif /* KERNEL_LINUX */

#if HAVE_LIBKSTAT
static int nfs_read_kstat(kstat_t *ksp, int nfs_version, const char *inst,
                          char const **proc_names, size_t proc_names_num) {
  char plugin_instance[DATA_MAX_NAME_LEN];
  value_t values[proc_names_num];

  if (ksp == NULL)
    return EINVAL;

  snprintf(plugin_instance, sizeof(plugin_instance), "v%i%s", nfs_version,
           inst);

  kstat_read(kc, ksp, NULL);
  for (size_t i = 0; i < proc_names_num; i++) {
    /* The name passed to kstat_data_lookup() doesn't have the
     * "const" modifier, so we need to copy the name here. */
    char name[32];
    sstrncpy(name, proc_names[i], sizeof(name));

    values[i].counter = (derive_t)get_kstat_value(ksp, name);
  }

  nfs_procedures_submit(plugin_instance, proc_names, values, proc_names_num);
  return 0;
}
#endif

#if KERNEL_LINUX
static int nfs_read(void) {
  FILE *fh;

  if ((fh = fopen("/proc/net/rpc/nfs", "r")) != NULL) {
    nfs_read_linux(fh, "client");
    fclose(fh);
  }

  if ((fh = fopen("/proc/net/rpc/nfsd", "r")) != NULL) {
    nfs_read_linux(fh, "server");
    fclose(fh);
  }

  return 0;
}
/* #endif KERNEL_LINUX */

#elif KERNEL_NETBSD
static int nfs_read(void) {
  struct nfsstats ns;
  size_t size = sizeof(ns);
  int mib[] = {CTL_VFS, 2, NFS_NFSSTATS};
  value_t values[nfs3_procedures_names_num];
  int i;

  /* NetBSD reports v2 statistics mapped to v3 and doen't yet support v4 */
  if (report_v2) {
    WARNING("nfs plugin: NFSv2 statistics have been requested "
            "but they are mapped to NFSv3 statistics in the kernel on NetBSD.");
    return 0;
  }

  if (report_v4) {
    WARNING("nfs plugin: NFSv4 statistics have been requested "
            "but they are not yet supported on NetBSD.");
    return 0;
  }

  if (sysctl(mib, STATIC_ARRAY_SIZE(mib), &ns, &size, NULL, 0) != 0)
    return 1;

  for (i = 0; i < nfs3_procedures_names_num; i++)
    values[i].counter = (derive_t)ns.rpccnt[i];
  nfs_procedures_submit("v3client", nfs3_procedures_names, values,
                        nfs3_procedures_names_num);

  for (i = 0; i < nfs3_procedures_names_num; i++)
    values[i].counter = (derive_t)ns.srvrpccnt[i];
  nfs_procedures_submit("v3server", nfs3_procedures_names, values,
                        nfs3_procedures_names_num);

  return 0;
}
/* #endif KERNEL_NETBSD */

#elif HAVE_LIBKSTAT
static int nfs_read(void) {
  if (report_v2) {
    nfs_read_kstat(nfs2_ksp_client, /* version = */ 2, "client",
                   nfs2_procedures_names, nfs2_procedures_names_num);
    nfs_read_kstat(nfs2_ksp_server, /* version = */ 2, "server",
                   nfs2_procedures_names, nfs2_procedures_names_num);
  }
  if (report_v3) {
    nfs_read_kstat(nfs3_ksp_client, /* version = */ 3, "client",
                   nfs3_procedures_names, nfs3_procedures_names_num);
    nfs_read_kstat(nfs3_ksp_server, /* version = */ 3, "server",
                   nfs3_procedures_names, nfs3_procedures_names_num);
  }
  if (report_v4) {
    nfs_read_kstat(nfs4_ksp_client, /* version = */ 4, "client",
                   nfs4_procedures_names, nfs4_procedures_names_num);
    nfs_read_kstat(nfs4_ksp_server, /* version = */ 4, "server",
                   nfs4_procedures_names, nfs4_procedures_names_num);
  }

  return 0;
}
/* #endif HAVE_LIBKSTAT */

#elif HAVE_PERFSTAT
static int nfs_read (void)
{
	perfstat_protocol_t pinfo;
	perfstat_id_t protid;
	int status;

	strcpy(protid.name, "nfsv2");
	status = perfstat_protocol(&protid, &pinfo, sizeof(perfstat_protocol_t), 1);
	if (status < 0)
	{
		char errbuf[1024];
		WARNING ("nfs plugin: perfstat_protocol: %s",
			sstrerror (errno, errbuf, sizeof (errbuf)));
		return (-1);
	}

	if (status == 1 && strcmp(protid.name, "nfsv2")!=0)
	{
		value_t values[nfs2_procedures_names_num];

		values[0].derive = (derive_t) pinfo.u.nfsv2.client.null;
		values[1].derive = (derive_t) pinfo.u.nfsv2.client.getattr;
		values[2].derive = (derive_t) pinfo.u.nfsv2.client.setattr;
		values[3].derive = (derive_t) pinfo.u.nfsv2.client.root;
		values[4].derive = (derive_t) pinfo.u.nfsv2.client.lookup;
		values[5].derive = (derive_t) pinfo.u.nfsv2.client.readlink;
		values[6].derive = (derive_t) pinfo.u.nfsv2.client.read;
		values[7].derive = (derive_t) pinfo.u.nfsv2.client.writecache;
		values[8].derive = (derive_t) pinfo.u.nfsv2.client.write;
		values[9].derive = (derive_t) pinfo.u.nfsv2.client.create;
		values[10].derive = (derive_t) pinfo.u.nfsv2.client.remove;
		values[11].derive = (derive_t) pinfo.u.nfsv2.client.rename;
		values[12].derive = (derive_t) pinfo.u.nfsv2.client.link;
		values[13].derive = (derive_t) pinfo.u.nfsv2.client.symlink;
		values[14].derive = (derive_t) pinfo.u.nfsv2.client.mkdir;
		values[15].derive = (derive_t) pinfo.u.nfsv2.client.rmdir;
		values[16].derive = (derive_t) pinfo.u.nfsv2.client.readdir;
		values[17].derive = (derive_t) pinfo.u.nfsv2.client.statfs;

		nfs_procedures_submit ("v2client", nfs2_procedures_names,
			values, nfs2_procedures_names_num);

		values[0].derive = (derive_t) pinfo.u.nfsv2.server.null;
		values[1].derive = (derive_t) pinfo.u.nfsv2.server.getattr;
		values[2].derive = (derive_t) pinfo.u.nfsv2.server.setattr;
		values[3].derive = (derive_t) pinfo.u.nfsv2.server.root;
		values[4].derive = (derive_t) pinfo.u.nfsv2.server.lookup;
		values[5].derive = (derive_t) pinfo.u.nfsv2.server.readlink;
		values[6].derive = (derive_t) pinfo.u.nfsv2.server.read;
		values[7].derive = (derive_t) pinfo.u.nfsv2.server.writecache;
		values[8].derive = (derive_t) pinfo.u.nfsv2.server.write;
		values[9].derive = (derive_t) pinfo.u.nfsv2.server.create;
		values[10].derive = (derive_t) pinfo.u.nfsv2.server.remove;
		values[11].derive = (derive_t) pinfo.u.nfsv2.server.rename;
		values[12].derive = (derive_t) pinfo.u.nfsv2.server.link;
		values[13].derive = (derive_t) pinfo.u.nfsv2.server.symlink;
		values[14].derive = (derive_t) pinfo.u.nfsv2.server.mkdir;
		values[15].derive = (derive_t) pinfo.u.nfsv2.server.rmdir;
		values[16].derive = (derive_t) pinfo.u.nfsv2.server.readdir;
		values[17].derive = (derive_t) pinfo.u.nfsv2.server.statfs;

		nfs_procedures_submit ("v2server", nfs2_procedures_names,
			values, nfs2_procedures_names_num);
	}

	strcpy(protid.name, "nfsv3");
	status = perfstat_protocol(&protid, &pinfo, sizeof(perfstat_protocol_t), 1);
	if (status < 0)
	{
		char errbuf[1024];
		WARNING ("nfs plugin: perfstat_protocol: %s",
			sstrerror (errno, errbuf, sizeof (errbuf)));
		return (-1);
	}

	if (status == 1 && strcmp(protid.name, "nfsv3")!=0)
	{
		value_t values[nfs3_procedures_names_num];

		values[0].derive = (derive_t) pinfo.u.nfsv3.client.null;
		values[1].derive = (derive_t) pinfo.u.nfsv3.client.getattr;
		values[2].derive = (derive_t) pinfo.u.nfsv3.client.setattr;
		values[3].derive = (derive_t) pinfo.u.nfsv3.client.lookup;
		values[4].derive = (derive_t) pinfo.u.nfsv3.client.access;
		values[5].derive = (derive_t) pinfo.u.nfsv3.client.readlink;
		values[6].derive = (derive_t) pinfo.u.nfsv3.client.read;
		values[7].derive = (derive_t) pinfo.u.nfsv3.client.write;
		values[8].derive = (derive_t) pinfo.u.nfsv3.client.create;
		values[9].derive = (derive_t) pinfo.u.nfsv3.client.mkdir;
		values[10].derive = (derive_t) pinfo.u.nfsv3.client.symlink;
		values[11].derive = (derive_t) pinfo.u.nfsv3.client.mknod;
		values[12].derive = (derive_t) pinfo.u.nfsv3.client.remove;
		values[13].derive = (derive_t) pinfo.u.nfsv3.client.rmdir;
		values[14].derive = (derive_t) pinfo.u.nfsv3.client.rename;
		values[15].derive = (derive_t) pinfo.u.nfsv3.client.link;
		values[16].derive = (derive_t) pinfo.u.nfsv3.client.readdir;
		values[17].derive = (derive_t) pinfo.u.nfsv3.client.readdirplus;
		values[18].derive = (derive_t) pinfo.u.nfsv3.client.fsstat;
		values[19].derive = (derive_t) pinfo.u.nfsv3.client.fsinfo;
		values[20].derive = (derive_t) pinfo.u.nfsv3.client.pathconf;
		values[21].derive = (derive_t) pinfo.u.nfsv3.client.commit;

		nfs_procedures_submit ("v3client", nfs3_procedures_names,
			values, nfs3_procedures_names_num);

		values[0].derive = (derive_t) pinfo.u.nfsv3.server.null;
		values[1].derive = (derive_t) pinfo.u.nfsv3.server.getattr;
		values[2].derive = (derive_t) pinfo.u.nfsv3.server.setattr;
		values[3].derive = (derive_t) pinfo.u.nfsv3.server.lookup;
		values[4].derive = (derive_t) pinfo.u.nfsv3.server.access;
		values[5].derive = (derive_t) pinfo.u.nfsv3.server.readlink;
		values[6].derive = (derive_t) pinfo.u.nfsv3.server.read;
		values[7].derive = (derive_t) pinfo.u.nfsv3.server.write;
		values[8].derive = (derive_t) pinfo.u.nfsv3.server.create;
		values[9].derive = (derive_t) pinfo.u.nfsv3.server.mkdir;
		values[10].derive = (derive_t) pinfo.u.nfsv3.server.symlink;
		values[11].derive = (derive_t) pinfo.u.nfsv3.server.mknod;
		values[12].derive = (derive_t) pinfo.u.nfsv3.server.remove;
		values[13].derive = (derive_t) pinfo.u.nfsv3.server.rmdir;
		values[14].derive = (derive_t) pinfo.u.nfsv3.server.rename;
		values[15].derive = (derive_t) pinfo.u.nfsv3.server.link;
		values[16].derive = (derive_t) pinfo.u.nfsv3.server.readdir;
		values[17].derive = (derive_t) pinfo.u.nfsv3.server.readdirplus;
		values[18].derive = (derive_t) pinfo.u.nfsv3.server.fsstat;
		values[19].derive = (derive_t) pinfo.u.nfsv3.server.fsinfo;
		values[20].derive = (derive_t) pinfo.u.nfsv3.server.pathconf;
		values[21].derive = (derive_t) pinfo.u.nfsv3.server.commit;

		nfs_procedures_submit ("v3server", nfs3_procedures_names,
			values, nfs3_procedures_names_num);
	}

	strcpy(protid.name, "nfsv4");
	status = perfstat_protocol(&protid, &pinfo, sizeof(perfstat_protocol_t), 1);
	if (status < 0)
	{
		char errbuf[1024];
		WARNING ("nfs plugin: perfstat_protocol: %s",
			sstrerror (errno, errbuf, sizeof (errbuf)));
		return (-1);
	}

	if (status == 1 && strcmp(protid.name, "nfsv4")!=0)
	{
		value_t cvalues[nfs4_client_procedures_names_num];
		value_t svalues[nfs4_server_procedures_names_num];

		cvalues[0].derive = (derive_t) pinfo.u.nfsv4.client.null;
		cvalues[1].derive = (derive_t) pinfo.u.nfsv4.client.getattr;
		cvalues[2].derive = (derive_t) pinfo.u.nfsv4.client.setattr;
		cvalues[3].derive = (derive_t) pinfo.u.nfsv4.client.lookup;
		cvalues[4].derive = (derive_t) pinfo.u.nfsv4.client.access;
		cvalues[5].derive = (derive_t) pinfo.u.nfsv4.client.readlink;
		cvalues[6].derive = (derive_t) pinfo.u.nfsv4.client.read;
		cvalues[7].derive = (derive_t) pinfo.u.nfsv4.client.write;
		cvalues[8].derive = (derive_t) pinfo.u.nfsv4.client.create;
		cvalues[9].derive = (derive_t) pinfo.u.nfsv4.client.mkdir;
		cvalues[10].derive = (derive_t) pinfo.u.nfsv4.client.symlink;
		cvalues[11].derive = (derive_t) pinfo.u.nfsv4.client.mknod;
		cvalues[12].derive = (derive_t) pinfo.u.nfsv4.client.remove;
		cvalues[13].derive = (derive_t) pinfo.u.nfsv4.client.rmdir;
		cvalues[14].derive = (derive_t) pinfo.u.nfsv4.client.rename;
		cvalues[15].derive = (derive_t) pinfo.u.nfsv4.client.link;
		cvalues[16].derive = (derive_t) pinfo.u.nfsv4.client.readdir;
		cvalues[17].derive = (derive_t) pinfo.u.nfsv4.client.statfs;
		cvalues[18].derive = (derive_t) pinfo.u.nfsv4.client.finfo;
		cvalues[19].derive = (derive_t) pinfo.u.nfsv4.client.commit;
		cvalues[20].derive = (derive_t) pinfo.u.nfsv4.client.open;
		cvalues[21].derive = (derive_t) pinfo.u.nfsv4.client.open_confirm;
		cvalues[22].derive = (derive_t) pinfo.u.nfsv4.client.open_downgrade;
		cvalues[23].derive = (derive_t) pinfo.u.nfsv4.client.close;
		cvalues[24].derive = (derive_t) pinfo.u.nfsv4.client.lock;
		cvalues[25].derive = (derive_t) pinfo.u.nfsv4.client.unlock;
		cvalues[26].derive = (derive_t) pinfo.u.nfsv4.client.lock_test;
		cvalues[27].derive = (derive_t) pinfo.u.nfsv4.client.set_clientid;
		cvalues[28].derive = (derive_t) pinfo.u.nfsv4.client.renew;
		cvalues[29].derive = (derive_t) pinfo.u.nfsv4.client.client_confirm;
		cvalues[30].derive = (derive_t) pinfo.u.nfsv4.client.secinfo;
		cvalues[31].derive = (derive_t) pinfo.u.nfsv4.client.release_lock;
		cvalues[32].derive = (derive_t) pinfo.u.nfsv4.client.replicate;
		cvalues[33].derive = (derive_t) pinfo.u.nfsv4.client.pcl_stat;
		cvalues[34].derive = (derive_t) pinfo.u.nfsv4.client.acl_stat_l;
		cvalues[35].derive = (derive_t) pinfo.u.nfsv4.client.pcl_stat_l;
		cvalues[36].derive = (derive_t) pinfo.u.nfsv4.client.acl_read;
		cvalues[37].derive = (derive_t) pinfo.u.nfsv4.client.pcl_read;
		cvalues[38].derive = (derive_t) pinfo.u.nfsv4.client.acl_write;
		cvalues[39].derive = (derive_t) pinfo.u.nfsv4.client.pcl_write;
		cvalues[40].derive = (derive_t) pinfo.u.nfsv4.client.delegreturn;

		nfs_procedures_submit ("v4client", nfs4_client_procedures_names,
			cvalues, nfs4_client_procedures_names_num);

		svalues[0].derive = (derive_t) pinfo.u.nfsv4.server.null;
		svalues[1].derive = (derive_t) pinfo.u.nfsv4.server.compound;
		svalues[2].derive = (derive_t) pinfo.u.nfsv4.server.access;
		svalues[3].derive = (derive_t) pinfo.u.nfsv4.server.close;
		svalues[4].derive = (derive_t) pinfo.u.nfsv4.server.commit;
		svalues[5].derive = (derive_t) pinfo.u.nfsv4.server.create;
		svalues[6].derive = (derive_t) pinfo.u.nfsv4.server.delegpurge;
		svalues[7].derive = (derive_t) pinfo.u.nfsv4.server.delegreturn;
		svalues[8].derive = (derive_t) pinfo.u.nfsv4.server.getattr;
		svalues[9].derive = (derive_t) pinfo.u.nfsv4.server.getfh;
		svalues[10].derive = (derive_t) pinfo.u.nfsv4.server.link;
		svalues[11].derive = (derive_t) pinfo.u.nfsv4.server.lock;
		svalues[12].derive = (derive_t) pinfo.u.nfsv4.server.lockt;
		svalues[13].derive = (derive_t) pinfo.u.nfsv4.server.locku;
		svalues[14].derive = (derive_t) pinfo.u.nfsv4.server.lookup;
		svalues[15].derive = (derive_t) pinfo.u.nfsv4.server.lookupp;
		svalues[16].derive = (derive_t) pinfo.u.nfsv4.server.nverify;
		svalues[17].derive = (derive_t) pinfo.u.nfsv4.server.open;
		svalues[18].derive = (derive_t) pinfo.u.nfsv4.server.openattr;
		svalues[19].derive = (derive_t) pinfo.u.nfsv4.server.open_confirm;
		svalues[20].derive = (derive_t) pinfo.u.nfsv4.server.open_downgrade;
		svalues[21].derive = (derive_t) pinfo.u.nfsv4.server.putfh;
		svalues[22].derive = (derive_t) pinfo.u.nfsv4.server.putpubfh;
		svalues[23].derive = (derive_t) pinfo.u.nfsv4.server.putrootfh;
		svalues[24].derive = (derive_t) pinfo.u.nfsv4.server.read;
		svalues[25].derive = (derive_t) pinfo.u.nfsv4.server.readdir;
		svalues[26].derive = (derive_t) pinfo.u.nfsv4.server.readlink;
		svalues[27].derive = (derive_t) pinfo.u.nfsv4.server.remove;
		svalues[28].derive = (derive_t) pinfo.u.nfsv4.server.rename;
		svalues[29].derive = (derive_t) pinfo.u.nfsv4.server.renew;
		svalues[30].derive = (derive_t) pinfo.u.nfsv4.server.restorefh;
		svalues[31].derive = (derive_t) pinfo.u.nfsv4.server.savefh;
		svalues[32].derive = (derive_t) pinfo.u.nfsv4.server.secinfo;
		svalues[33].derive = (derive_t) pinfo.u.nfsv4.server.setattr;
		svalues[34].derive = (derive_t) pinfo.u.nfsv4.server.set_clientid;
		svalues[35].derive = (derive_t) pinfo.u.nfsv4.server.clientid_confirm;
		svalues[36].derive = (derive_t) pinfo.u.nfsv4.server.verify;
		svalues[37].derive = (derive_t) pinfo.u.nfsv4.server.write;
		svalues[38].derive = (derive_t) pinfo.u.nfsv4.server.release_lock;

		nfs_procedures_submit ("v4server", nfs4_server_procedures_names,
			svalues, nfs4_server_procedures_names_num);
	}
	return (0);
}
#endif /* HAVE_PERFSTAT */

void module_register(void) {
  plugin_register_config("nfs", nfs_config, config_keys, config_keys_num);
  plugin_register_init("nfs", nfs_init);
  plugin_register_read("nfs", nfs_read);
} /* void module_register */
