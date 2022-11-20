/**
 * collectd - src/wpar.c
 * Copyright (C) 2010  Manuel Sanmartin
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
 *   Manuel Sanmartin <manuel.luis at gmail.com>
 **/

#include "collectd.h"
#include "utils/common/common.h"
#include "plugin.h"

#if !HAVE_PERFSTAT
# error "No applicable input method."
#endif

#include <sys/proc.h> /* AIX 5 */
#include <sys/protosw.h>
#include <libperfstat.h>

static int pagesize;
static int wpar_total_num;
static perfstat_wpar_total_t *wpar_total = NULL;
static unsigned long long timebase_saved;
static time_t time_saved;

typedef struct wpar_cpu {
  u_longlong_t user;
  u_longlong_t sys;
} wpar_cpu_t;

static wpar_cpu_t *prev_wcpu;
static wpar_cpu_t *cnt_wcpu;

static int wpar_init(void) /* {{{ */
{
  pagesize = getpagesize ();
  return (0);
} /* }}} int wpar_init */

static void memory_submit (const char *plugin_instance, /* {{{ */
  const char *type_instance, gauge_t value)
{
  value_t values[1];
  value_list_t vl = VALUE_LIST_INIT;

  values[0].gauge = value;

  vl.values = values;
  vl.values_len = 1;

  sstrncpy (vl.host, hostname_g, sizeof (vl.host));
  sstrncpy (vl.plugin, "wpar", sizeof (vl.plugin));
  sstrncpy (vl.plugin_instance, plugin_instance,
    sizeof (vl.plugin_instance));
  sstrncpy (vl.type, "memory", sizeof (vl.type));
  sstrncpy (vl.type_instance, type_instance,
    sizeof (vl.type_instance));

  plugin_dispatch_values (&vl);
} /* }}} void memory_submit */

static void cpu_submit (const char *plugin_instance, /* {{{ */
  const char *type_instance, derive_t value)
{
  value_t values[1];
  value_list_t vl = VALUE_LIST_INIT;

  values[0].derive = value;

  vl.values = values;
  vl.values_len = 1;

  sstrncpy (vl.host, hostname_g, sizeof (vl.host));
  sstrncpy (vl.plugin, "wpar", sizeof (vl.plugin));
  sstrncpy (vl.plugin_instance, plugin_instance,
    sizeof (vl.plugin_instance));
  sstrncpy (vl.type, "cpu", sizeof (vl.type));
  sstrncpy (vl.type_instance, type_instance,
    sizeof (vl.type_instance));

  plugin_dispatch_values (&vl);
} /* }}} void cpu_submit */

static void load_submit (const char *plugin_instance, /* {{{ */
  gauge_t snum, gauge_t mnum, gauge_t lnum)
{
  value_t values[3];
  value_list_t vl = VALUE_LIST_INIT;

  values[0].gauge = snum;
  values[1].gauge = mnum;
  values[2].gauge = lnum;

  vl.values = values;
  vl.values_len = STATIC_ARRAY_SIZE (values);

  sstrncpy (vl.host, hostname_g, sizeof (vl.host));
  sstrncpy (vl.plugin, "wpar", sizeof (vl.plugin));
  sstrncpy (vl.plugin_instance,
    plugin_instance, sizeof (vl.plugin_instance));
  sstrncpy (vl.type, "load", sizeof (vl.type));

  plugin_dispatch_values (&vl);
} /* }}} void load_submit */

static int wpar_read (void) /* {{{ */
{
  int i;
  int nwpar;
  perfstat_id_wpar_t id_wpar;
  perfstat_partition_total_t part;
  unsigned long long hardware_ticks;
  time_t time_now;
  time_t time_diff;

  /* Read the partition information to get the cpu time base */
  if (perfstat_partition_total(/* perfstat_id_t */ NULL,
      /* (out) perfstat_partition_total_t */ &part,
      /* sizeof_userbuff = */ sizeof(perfstat_partition_total_t),
      /* desired_number = */ 1) < 0)
  {
    char errbuf[1024];
    WARNING ("wpar plugin: perfstat_partition_total failed: %s",
        sstrerror (errno, errbuf, sizeof (errbuf)));
    return (-1);
  }

  if (timebase_saved > 0)
    hardware_ticks = part.timebase_last - timebase_saved;
  else
    hardware_ticks = 0;

  timebase_saved = part.timebase_last;

  time_now = time(NULL);
  time_diff = time_now - time_saved;
  time_saved = time_now;

  /* Read the number of partitions */
  nwpar = perfstat_wpar_total (/* id = */ NULL,
      /* (out) wpar_total */ NULL,
      /* size = */ sizeof (perfstat_wpar_total_t),
      /* nmemb = */ 0);
  if (nwpar < 0)
  {
    char errbuf[1024];
    WARNING ("wpar plugin: perfstat_wpar_total failed: %s",
        sstrerror (errno, errbuf, sizeof (errbuf)));
    return (-1);
  }
  else if (nwpar == 0)
  {
    /* Avoid "realloc returned NULL" messages */
    INFO ("wpar plugin: perfstat_wpar_total returned zero.");
    return (0);
  }

  /* If necessary, reallocate the "wpar_total" memory. */
  if ((wpar_total_num != nwpar) || (wpar_total == NULL))
  {
    perfstat_wpar_total_t *tmp;

    tmp = realloc (wpar_total, nwpar * sizeof (*wpar_total));
    if (tmp == NULL)
    {
      ERROR ("wpar plugin: realloc failed.");
      return (ENOMEM);
    }
    wpar_total = tmp;

    sfree(prev_wcpu);
    prev_wcpu = calloc (nwpar, sizeof (wpar_cpu_t));
    if (prev_wcpu == NULL)
    {
      ERROR ("wpar plugin: calloc failed.");
      return (ENOMEM);
    }

    sfree(cnt_wcpu);
    cnt_wcpu = calloc (nwpar, sizeof (wpar_cpu_t));
    if (cnt_wcpu == NULL)
    {
      ERROR ("wpar plugin: calloc failed.");
      return (ENOMEM);
    }
    /* Skip one loop in cpu collection to fill prev_wcpu */
    hardware_ticks = 0;
  }
  wpar_total_num = nwpar;

  memset (&id_wpar, 0, sizeof (id_wpar));
  id_wpar.spec = WPARNAME;

  /* Now actually query the data */
  nwpar = perfstat_wpar_total (/* id = */ &id_wpar,
      /* (out) */ wpar_total,
      /* size = */ sizeof(perfstat_wpar_total_t),
      /* nmemb = */ wpar_total_num);
  if (nwpar < 0)
  {
    char errbuf[1024];
    WARNING ("wpar plugin: perfstat_wpar_total failed: %s",
        sstrerror (errno, errbuf, sizeof (errbuf)));
    return (-1);
  }
  else if (nwpar > wpar_total_num)
  {
    INFO ("wpar plugin: Number of WPARs increased during allocation. "
        "Will ignore %i WPAR(s).", nwpar - wpar_total_num);
    nwpar = wpar_total_num;
  }

  /* Iterate over all WPARs and dispatch information */
  for (i = 0; i < nwpar; i++)
  {
    const char *wname = wpar_total[i].name;
    perfstat_memory_total_wpar_t wmemory;
    perfstat_cpu_total_wpar_t wcpu;
    float factor, snum, mnum, lnum;
    int status;

    /* Update the ID structure */
    memset (&id_wpar, 0, sizeof (id_wpar));
    id_wpar.spec = WPARID;
    id_wpar.u.wpar_id = wpar_total[i].wpar_id;

    /*
     * Memory
     */
    status = perfstat_memory_total_wpar(/* id = */ &id_wpar,
        /* (out) */ &wmemory,
        /* size = */ sizeof(wmemory), /* nmemb = */ 1);
    if (status < 0)
    {
      char errbuf[1024];
      WARNING ("wpar plugin: perfstat_memory_total_wpar(%s) failed: %s",
          wname, sstrerror (errno, errbuf, sizeof (errbuf)));
      continue;
    }
    memory_submit (wname, "used",   wmemory.real_inuse * pagesize);
    memory_submit (wname, "free",   wmemory.real_free * pagesize);
    memory_submit (wname, "cached", wmemory.numperm * pagesize);
    memory_submit (wname, "total",  wmemory.real_total * pagesize);

    /*
     * CPU and load
     */
    status = perfstat_cpu_total_wpar(/* id = */ &id_wpar,
        /* (out) */ &wcpu,
        /* size = */ sizeof(wcpu), /* nmemb = */ 1);
    if (status < 0)
    {
      char errbuf[1024];
      WARNING ("wpar plugin: perfstat_cpu_total_wpar(%s) failed: %s",
          wname, sstrerror (errno, errbuf, sizeof (errbuf)));
      continue;
    }

    factor = 1.0 / ((gauge_t) (1 << SBITS));
    snum = ((gauge_t) wcpu.loadavg[0]) * factor;
    mnum = ((gauge_t) wcpu.loadavg[1]) * factor;
    lnum = ((gauge_t) wcpu.loadavg[2]) * factor;

    load_submit (wname, snum, mnum, lnum);

    if (hardware_ticks > 0)
    {
      u_longlong_t diff_sys;
      u_longlong_t diff_user;
      /* Number of physical processors */
      int pncpus = (part.smt_thrds > 0 ) ?
        wcpu.ncpus / part.smt_thrds : wcpu.ncpus;

      diff_sys = (wcpu.psys - prev_wcpu[i].sys) * 100 * time_diff;
      cnt_wcpu[i].sys += (diff_sys / hardware_ticks) / pncpus;
      cpu_submit (wname, "system", cnt_wcpu[i].sys);

      diff_user = (wcpu.puser - prev_wcpu[i].user) * 100 * time_diff;
      cnt_wcpu[i].user += (diff_user / hardware_ticks) / pncpus;
      cpu_submit (wname, "user", cnt_wcpu[i].user);
    }
    else
    {
      cpu_submit (wname, "system", cnt_wcpu[i].sys);
      cpu_submit (wname, "user", cnt_wcpu[i].user);
    }

    prev_wcpu[i].sys = wcpu.psys;
    prev_wcpu[i].user = wcpu.puser;
  } /* for (i = 0 ... nwpar) */

  return (0);
} /* }}} int wpar_read */

void module_register (void)
{
  plugin_register_init ("wpar", wpar_init);
  plugin_register_read ("wpar", wpar_read);
}

/* vim: set sw=2 sts=2 et fdm=marker : */
