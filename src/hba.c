/**
 * collectd - src/hba.c
 * Copyright (C) 2012       Manuel Sanmartín
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
 *   Manuel Sanmartín <manuel.luis at gmail.com>
 **/

#include "collectd.h"
#include "utils/common/common.h"
#include "plugin.h"
#include "utils/ignorelist/ignorelist.h"

#if KERNEL_AIX
# include <sys/scsi.h>
# include <sys/scsi_buf.h>
# if HAVE_ODMI_H
#  include <odmi.h>
# endif
# if HAVE_SYS_CFGODM_H
#  include <sys/cfgodm.h>
# endif
#elif HAVE_LIBHBAAPI
# if HAVE_SYS_HBAAPI_H
#  include <sys/hbaapi.h>
# elif HAVE_HBAAPI_H
#  include <hbaapi.h>
# endif
#else /* HAVE_LIBHBAAPI */
# error "No applicable input method."
#endif

/* Define FC4 Type */
#ifndef FC_TYPE_FCP
# define FC_TYPE_FCP        0x08 /* SCSI FCP */
#endif

#if KERNEL_AIX
  struct hba_device {
    char *adapter;
    char *device;
  };

  static unsigned int cnt_read_loop;
  static struct hba_device *hba_list;
#endif

static const char *config_keys[] =
{
  "Hba",
  "IgnoreSelected"
};
static int config_keys_num = STATIC_ARRAY_SIZE (config_keys);

static ignorelist_t *ignorelist = NULL;

static int hba_config (const char *key, const char *value)
{
  if (ignorelist == NULL)
    ignorelist = ignorelist_create (/* invert = */ 1);
  if (ignorelist == NULL)
    return (1);

  if (strcasecmp ("HBA", key) == 0)
  {
    ignorelist_add (ignorelist, value);
  }
  else if (strcasecmp ("IgnoreSelected", key) == 0)
  {
    int invert = 1;
    if (IS_TRUE (value))
      invert = 0;
    ignorelist_set_invert (ignorelist, invert);
  }
  else
  {
    return (-1);
  }

  return (0);
} /* int hba_config */

static void hba_submit (const char *plugin_instance,
    const char *type, derive_t read, derive_t write)
{
  value_t values[2];
  value_list_t vl = VALUE_LIST_INIT;

  values[0].derive = read;
  values[1].derive = write;

  vl.values = values;
  vl.values_len = 2;
  sstrncpy (vl.host, hostname_g, sizeof (vl.host));
  sstrncpy (vl.plugin, "hba", sizeof (vl.plugin));
  sstrncpy (vl.plugin_instance, plugin_instance,
      sizeof (vl.plugin_instance));
  sstrncpy (vl.type, type, sizeof (vl.type));

  plugin_dispatch_values (&vl);
} /* void hba_submit */

#if HAVE_LIBHBAAPI && !KERNEL_AIX
static const char *hba_strerror(HBA_UINT32 code)
{
  switch(code)
  {
  case HBA_STATUS_OK:
    return "Function returned normally";
    break;
  case HBA_STATUS_ERROR:
    return "General Error return";
    break;
  case HBA_STATUS_ERROR_NOT_SUPPORTED:
    return "Function not supported";
    break;
  case HBA_STATUS_ERROR_INVALID_HANDLE:
    return "invalid handle";
    break;
  case HBA_STATUS_ERROR_ARG:
    return "Bad argument";
    break;
  case HBA_STATUS_ERROR_ILLEGAL_WWN:
    return "WWN not recognized";
    break;
  case HBA_STATUS_ERROR_ILLEGAL_INDEX:
    return "Index not recognized";
    break;
  case HBA_STATUS_ERROR_MORE_DATA:
    return "Larger buffer required";
    break;
  case HBA_STATUS_ERROR_STALE_DATA:
    return "Information has changed since last call to HBA_Refreshinformation";
    break;
  case HBA_STATUS_SCSI_CHECK_CONDITION:
    return "SCSI Check Condition";
    break;
  case HBA_STATUS_ERROR_BUSY:
    return "HBA busy or reserved, retry may be effective";
    break;
  case HBA_STATUS_ERROR_TRY_AGAIN:
    return "Request timedout, retry may be effective";
    break;
  case HBA_STATUS_ERROR_UNAVAILABLE:
    return "Referenced HBA has been removed or deactivated";
    break;
  case HBA_STATUS_ERROR_ELS_REJECT:
    return "The requested ELS was rejected by the local HBA";
    break;
  case HBA_STATUS_ERROR_INVALID_LUN:
    return "The specified LUN is not provided  the specified HBA";
    break;
  case HBA_STATUS_ERROR_INCOMPATIBLE:
    return "An incompatibility has been detected";
    break;
  case HBA_STATUS_ERROR_AMBIGUOUS_WWN:
    return "Multiple adapters have a matching WWN";
    break;
  case HBA_STATUS_ERROR_LOCAL_BUS:
    return "A persistent binding request included a bad local SCSI bus number";
    break;
  case HBA_STATUS_ERROR_LOCAL_TARGET:
    return "A persistent binding request included "
      "a bad local SCSI target number";
    break;
  case HBA_STATUS_ERROR_LOCAL_LUN:
    return "A persistent binding request included "
      "a bad local SCSI logical unit number";
    break;
  case HBA_STATUS_ERROR_LOCAL_SCSIID_BOUND:
    return "A persistent binding set request included "
      "a local SCSI ID that was already bound";
    break;
  case HBA_STATUS_ERROR_TARGET_FCID:
    return "A persistent binding request included a "
      "bad or unlocatable FCP Target FCID";
    break;
  case HBA_STATUS_ERROR_TARGET_NODE_WWN:
    return "A persistent binding request included a bad FCP Target Node WWN";
    break;
  case HBA_STATUS_ERROR_TARGET_PORT_WWN:
    return "A persistent binding request included a bad FCP Target Port WWN";
    break;
  case HBA_STATUS_ERROR_TARGET_LUN:
    return "A persistent binding request included "
      "an FCP Logical Unit Number not defined by the identified Target";
    break;
  case HBA_STATUS_ERROR_TARGET_LUID:
    return "A persistent binding request included an undefined "
      "or otherwise inaccessible Logical Unit Unique Identifier";
    break;
  case HBA_STATUS_ERROR_NO_SUCH_BINDING:
    return "A persistent binding remove request included a "
      "binding which did not match a binding established by the specified port";
    break;
  case HBA_STATUS_ERROR_NOT_A_TARGET:
    return "A SCSI command was requested to an Nx_Port"
      " that was not a SCSI Target Port";
    break;
  case HBA_STATUS_ERROR_UNSUPPORTED_FC4:
    return "A request was made concerning an unsupported FC-4 protocol";
    break;
  case HBA_STATUS_ERROR_INCAPABLE:
    return "A request was made to enable unimplemented capabilities for a port";
    break;
  case HBA_STATUS_ERROR_TARGET_BUSY:
    return "A SCSI function was requested at a time when issuing the "
      "requested command would cause a a SCSI overlapped command "
      "condition (see SAM-3)";
    break;
#ifdef HBA_STATUS_ERROR_NOT_LOADED
  case HBA_STATUS_ERROR_NOT_LOADED:
    return "A call to HBA_FreeLibrary when no library was loaded";
    break;
#endif
#ifdef HBA_STATUS_ERROR_ALREADY_LOADED
  case HBA_STATUS_ERROR_ALREADY_LOADED:
    return "A call to HBA_LoadLibrary when library was already loaded";
    break;
#endif
#ifdef HBA_STATUS_ERROR_ILLEGAL_FCID
  case HBA_STATUS_ERROR_ILLEGAL_FCID:
    return "The Address Id specified to HBA_SendRNIDV2 is illegal";
    break;
#endif
  default:
    return "Unknown HBA error code.";
  }
} /* const char *hba_strerror */
#endif

#ifdef KERNEL_AIX


struct hba_device *hba_odm_list(char *criteria)
{
  struct CuDv *cudv;
  struct listinfo info;
  struct hba_device *hba_list = NULL;
  int i;

  if (odm_initialize() < 0)
  {
    char *errmsg;

    if (odm_err_msg (odmerrno, &errmsg)  < 0)
      ERROR ("hba plugin: Could not initialize the ODM database: %d",
        odmerrno);
    else
      ERROR ("hba plugin: Could not initialize the ODM database: %s",
        errmsg);
    return NULL;
  }

  cudv = (struct CuDv *) odm_get_list(CuDv_CLASS, criteria, &info, 256, 1);
  if (*(int *)cudv == -1 || !cudv)
  {
    odm_terminate();
    return NULL;
  }

  hba_list = calloc (info.num+1, sizeof(struct hba_device));
  if (hba_list == NULL)
  {
    ERROR ("hba plugin: malloc failed");
    odm_terminate();
    return NULL;
  }

  for (i=0; i<info.num; i++) {
    hba_list[i].adapter = sstrdup(cudv[i].parent);
    hba_list[i].device =  sstrdup(cudv[i].name);
  }

  odm_free_list(cudv, &info);

  if (odm_terminate() < 0)
  {
    char *errmsg;

    if (odm_err_msg (odmerrno, &errmsg)  < 0)
      WARNING ("hba plugin: Could not terminate using the ODM database: %d",
        odmerrno);
    else
      WARNING ("hba plugin: Could not terminate using the ODM database: %s",
        errmsg);
  }

  return hba_list;
}

int hba_get_stats (char *adapter, char *device)
{
  int fd;
  int ret;
  struct scsi_chba scsi_chba;

  fd = open (device, O_RDONLY);
  if (fd < 0)
  {
    char errbuf[1024];
    ERROR ("hba plugin: open %s failed: %s",
      device, sstrerror (errno, errbuf, sizeof (errbuf)));
    close (fd);
    return (-1);
  }

  memset (&scsi_chba, 0, sizeof(struct scsi_chba));
  scsi_chba.flags = 0;
  scsi_chba.cmd = FC_SCSI_ADAP_STAT;

  ret = ioctl(fd, SCIOLCHBA, &scsi_chba);
  if (ret < 0)
  {
    char errbuf[1024];
    ERROR ("hba plugin: hba_get_stats ioctl %s failed: %s",
      device, sstrerror (errno, errbuf, sizeof (errbuf)));
    close (fd);
    return (-1);
  }

  hba_submit(adapter, "hba_frames",
    scsi_chba.un.adap_stat.RxFrames, scsi_chba.un.adap_stat.TxFrames);

  memset (&scsi_chba, 0, sizeof(struct scsi_chba));
  scsi_chba.flags = 0;
  scsi_chba.cmd = FC_TRAFFIC_STAT;
  scsi_chba.un.traffic_info.trans_type = FC_SCSI_FCP_TYPE;

  ret = ioctl(fd, SCIOLCHBA, &scsi_chba);
  if (ret < 0)
  {
    char errbuf[1024];
    ERROR ("hba plugin: hba_get_stats ioctl %s failed: %s",
      device, sstrerror (errno, errbuf, sizeof (errbuf)));
    close (fd);
    return (-1);
  }

  hba_submit(adapter, "hba_ops",
    scsi_chba.un.traffic_info.traffic_stat.inp_reqs,
    scsi_chba.un.traffic_info.traffic_stat.out_reqs);

  hba_submit(adapter, "hba_octects",
    scsi_chba.un.traffic_info.traffic_stat.inp_bytes,
    scsi_chba.un.traffic_info.traffic_stat.out_bytes);

  close(fd);
  return (0);
}
#endif

static int hba_read (void)
{
#if KERNEL_AIX
  int ret = 0;
  int i;

  if (!(cnt_read_loop%30))
  {
    if (hba_list != NULL)
    {
      for (i=0;hba_list[i].adapter!=NULL;i++)
      {
        sfree(hba_list[i].adapter);
        sfree(hba_list[i].device);
      }
      sfree(hba_list);
    }
    hba_list = hba_odm_list ("parent LIKE fcs* AND status=1");
    if (hba_list == NULL)
      return (0);
  }

  for (i=0;hba_list[i].adapter!=NULL;i++)
  {
    if (ignorelist_match (ignorelist, hba_list[i].adapter) == 0) 
    {
      char device[256];

      snprintf(device, sizeof(device),"/dev/%s", hba_list[i].device);
      ret += hba_get_stats (hba_list[i].adapter, device);
    }
  }

  cnt_read_loop++;

  if (ret<0)
    return (-1);

#elif HAVE_LIBHBAAPI
  HBA_HANDLE handle;
  HBA_STATUS status;
  HBA_UINT32 adapter;
  HBA_UINT32 nadapters;
  HBA_PORTSTATISTICS port_statistics;
  HBA_FC4STATISTICS fc4_statistics;
  HBA_ADAPTERATTRIBUTES adapter_attributes;
  HBA_PORTATTRIBUTES port_attributes;
  char adapter_name[1028];
  int port;

  nadapters = HBA_GetNumberOfAdapters();

  for (adapter=0 ; adapter < nadapters ; adapter++)
  {
    status = HBA_GetAdapterName(adapter, adapter_name);
    if (status != HBA_STATUS_OK)
    {
      WARNING ("hba plugin: HBA_GetAdapterName: %s",
        hba_strerror(status));
      continue;
    }

    handle = HBA_OpenAdapter (adapter_name);
    if (handle == 0)
    {
      WARNING ("hba plugin: HBA_OpenAdapter(%s) = 0",
        adapter_name);
      continue;
    }

    status = HBA_GetAdapterAttributes (handle, &adapter_attributes);
    if (status != HBA_STATUS_OK)
    {
      WARNING ("hba plugin: HBA_GetAdapterAttributes %s: failed: %s",
        adapter_name, hba_strerror(status));
      HBA_CloseAdapter(handle);
      continue;
    }

    for (port=0; port < adapter_attributes.NumberOfPorts; port++)
    {
      status = HBA_GetAdapterPortAttributes (handle, port, &port_attributes);
      if (status != HBA_STATUS_OK)
      {
        WARNING ("hba plugin: HBA_GetAdapterAttributes %s port %d: failed: %s",
          adapter_name, port, hba_strerror(status));
        continue;
      }

      if (ignorelist_match (ignorelist, port_attributes.OSDeviceName) != 0)
        continue;

      status = HBA_GetPortStatistics(handle, port, &port_statistics);
      if (status != HBA_STATUS_OK)
      {
        WARNING ("hba plugin: HBA_GetPortStatistics %s: failed: %s",
          port_attributes.OSDeviceName, hba_strerror(status));
        continue;
      }
      hba_submit(port_attributes.OSDeviceName, "hba_frames",
        port_statistics.RxFrames, port_statistics.TxFrames);

      status = HBA_GetFC4Statistics(handle,
         port_attributes.PortWWN, FC_TYPE_FCP, &fc4_statistics);

      if (status != HBA_STATUS_OK)
      {
        WARNING ("hba plugin: HBA_GetFC4Statistics: %s: failed: %s",
          port_attributes.OSDeviceName, hba_strerror(status));
        continue;
      }
      hba_submit(port_attributes.OSDeviceName, "hba_ops",
        fc4_statistics.InputRequests, fc4_statistics.OutputRequests);
      hba_submit(port_attributes.OSDeviceName, "hba_octects",
        fc4_statistics.InputMegabytes*(1024*1024),
        fc4_statistics.OutputMegabytes*(1024*1024));
    }

    HBA_CloseAdapter(handle);
  }
#endif
  return (0);
} /* int hba_read */

static int hba_init (void)
{
#if KERNEL_AIX
  /* Nothing */
#elif HAVE_LIBHBAAPI
  if (HBA_LoadLibrary() != HBA_STATUS_OK) {
    WARNING ("hba plugin: HBA_LoadLibrary failed: %s",  hba_strerror(status));
    return -1;
  }
#endif
  return (0);
} /* int hba_init */

static int hba_shutdown (void)
{
  ignorelist_free(ignorelist);
#if KERNEL_AIX
  /* Nothing */
#elif HAVE_LIBHBAAPI
  if (HBA_FreeLibrary() != HBA_STATUS_OK) {
    WARNING ("hba plugin: HBA_FreeLibrary failed: %s",  hba_strerror(status));
    return -1;
  }
#endif
  return (0);
} /* int hba_shutdown */

void module_register (void)
{
  plugin_register_config ("hba", hba_config,
    config_keys, config_keys_num);
  plugin_register_init ("hba", hba_init);
  plugin_register_shutdown ("hba", hba_shutdown);
  plugin_register_read ("hba", hba_read);
} /* void module_register */

/* vim: set sw=2 sts=2 tw=78 et fdm=marker : */
