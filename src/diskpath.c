/**
 * collectd - src/diskpath.c
 * Copyright (C) 2005-2012  Florian octo Forster
 * Copyright (C) 2009-2014  Manuel Sanmartín
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
 *   Florian octo Forster <octo at collectd.org>
 *   Manuel Sanmartín <manuel.luis at gmail.com>
 **/

#include "collectd.h"
#include "utils/common/common.h"
#include "plugin.h"
#include "utils/ignorelist/ignorelist.h"

#if HAVE_PERFSTAT
# include <sys/protosw.h>
# include <libperfstat.h>
/* XINTFRAC was defined in libperfstat.h somewhere between AIX 5.3 and 6.1 */
# ifndef XINTFRAC
#  include <sys/systemcfg.h>
#  define XINTFRAC ((double)(_system_configuration.Xint) / \
                   (double)(_system_configuration.Xfrac))
# endif
# ifndef HTIC2NANOSEC
#  define HTIC2NANOSEC(x)  (((double)(x) * XINTFRAC))
# endif
#endif

#if HAVE_PERFSTAT
static perfstat_diskpath_t *stat_diskpath;
static int numdiskpath;
static int pnumdiskpath;

typedef struct diskpathstats
{
	char *name;

	/* This overflows in roughly 1361 years */
	unsigned int poll_count;

	derive_t read_ops;
	derive_t write_ops;
	derive_t read_time;
	derive_t write_time;

	derive_t avg_read_time;
	derive_t avg_write_time;

	struct diskpathstats *next;
} diskpathstats_t;

static diskpathstats_t *diskpathlist;
/* #endif HAVE_PERFSTAT */

#else
# error "No applicable input method."
#endif

static const char *config_keys[] =
{
	"DiskPath",
	"IgnoreSelected"
};
static int config_keys_num = STATIC_ARRAY_SIZE (config_keys);

static ignorelist_t *ignorelist = NULL;

static int diskpath_config (const char *key, const char *value)
{
  if (ignorelist == NULL)
    ignorelist = ignorelist_create (/* invert = */ 1);
  if (ignorelist == NULL)
    return (1);

  if (strcasecmp ("DiskPath", key) == 0)
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
} /* int diskpath_config */

static int diskpath_init (void)
{
	return (0);
} /* int diskpath_init */

static void diskpath_submit (const char *plugin_instance,
		const char *type,
		derive_t read, derive_t write)
{
	value_t values[2];
	value_list_t vl = VALUE_LIST_INIT;

	/* Both `ignorelist' and `plugin_instance' may be NULL. */
	if (ignorelist_match (ignorelist, plugin_instance) != 0)
	  return;

	values[0].derive = read;
	values[1].derive = write;

	vl.values = values;
	vl.values_len = 2;
	sstrncpy (vl.host, hostname_g, sizeof (vl.host));
	sstrncpy (vl.plugin, "diskpath", sizeof (vl.plugin));
	sstrncpy (vl.plugin_instance, plugin_instance,
			sizeof (vl.plugin_instance));
	sstrncpy (vl.type, type, sizeof (vl.type));

	plugin_dispatch_values (&vl);
} /* void diskpath_submit */

#if HAVE_PERFSTAT
static counter_t diskpath_calc_time_incr (counter_t delta_time, counter_t delta_ops)
{
	double interval = CDTIME_T_TO_DOUBLE (plugin_get_interval ());
	double avg_time = ((double) delta_time) / ((double) delta_ops);
	double avg_time_incr = interval * avg_time;

	return ((counter_t) (avg_time_incr + .5));
}
#endif /* HAVE_PERFSTAT */

static int diskpath_read (void)
{
#if HAVE_PERFSTAT
	derive_t read_bytes;
	derive_t write_bytes;
	derive_t read_time;
	derive_t write_time;
	derive_t read_ops;
	derive_t write_ops;
	perfstat_id_t firstpath;
	diskpathstats_t *dsp, *pre_dsp;
	int rnumdiskpath;
	int i;

	numdiskpath = perfstat_diskpath(NULL, NULL, sizeof(perfstat_diskpath_t), 0);
	if (numdiskpath < 0)
	{
		char errbuf[1024];
		WARNING ("diskpath plugin: perfstat_diskpath: %s",
				sstrerror (errno, errbuf, sizeof (errbuf)));
		return (-1);
	}

	if (numdiskpath != pnumdiskpath || stat_diskpath==NULL)
	{
		sfree(stat_diskpath);
		stat_diskpath = (perfstat_diskpath_t *)calloc(numdiskpath,
			sizeof(perfstat_diskpath_t));
		if (stat_diskpath == NULL)
		{
			ERROR ("diskpath plugin: calloc failed.");
			return (-1);
		}
	}
	pnumdiskpath = numdiskpath;

	firstpath.name[0]='\0';
	rnumdiskpath = perfstat_diskpath(&firstpath, stat_diskpath,
		sizeof(perfstat_diskpath_t), numdiskpath);
	if (rnumdiskpath < 0)
	{
		char errbuf[1024];
		WARNING ("diskpath plugin: perfstat_diskpath: %s",
				sstrerror (errno, errbuf, sizeof (errbuf)));
		return (-1);
	}

	for (i = 0; i < rnumdiskpath; i++)
	{
		derive_t diff_read_ops;
		derive_t diff_write_ops;
		derive_t diff_read_time;
		derive_t diff_write_time;
		char *diskpath_name;

		diskpath_name = stat_diskpath[i].name;

		for (dsp = diskpathlist, pre_dsp = diskpathlist; dsp != NULL;
			pre_dsp = dsp, dsp = dsp->next)
		{
			if (strcmp (diskpath_name, dsp->name) == 0)
				break;
		}

		if (dsp == NULL)
		{
			dsp = (diskpathstats_t *) calloc (1, sizeof (diskpathstats_t));
			if (dsp == NULL)
				continue;

			if ((dsp->name = strdup (diskpath_name)) == NULL)
			{
				free (dsp);
				continue;
			}

			if (pre_dsp == NULL)
				diskpathlist = dsp;
			else
				pre_dsp->next = dsp;
		}

		read_bytes = stat_diskpath[i].rblks*512;
		write_bytes = stat_diskpath[i].wblks*512;

		read_ops = stat_diskpath[i].xrate;
		write_ops = stat_diskpath[i].xfers - stat_diskpath[i].xrate;

		read_time = HTIC2NANOSEC(stat_diskpath[i].rserv) / 1000000.0;
		write_time = HTIC2NANOSEC(stat_diskpath[i].wserv) / 1000000.0;

		diff_read_ops = read_ops - dsp->read_ops;
		diff_write_ops = write_ops - dsp->write_ops;

		diff_read_time = read_time - dsp->read_time;
		diff_write_time = write_time - dsp->write_time;

		if (diff_read_ops != 0)
			dsp->avg_read_time += diskpath_calc_time_incr (
				diff_read_time, diff_read_ops);
		if (diff_write_ops != 0)
			dsp->avg_write_time += diskpath_calc_time_incr (
				diff_write_time, diff_write_ops);

		dsp->read_ops = read_ops;
		dsp->read_time = read_time;

		dsp->write_ops = write_ops;
		dsp->write_time = write_time;

		dsp->poll_count++;
		if (dsp->poll_count <= 2)
		{
			DEBUG ("diskpath plugin: (dsp->poll_count = %i) <= "
					"(min_poll_count = 2); => Not writing.",
					dsp->poll_count);
			continue;
		}

		if ((read_ops == 0) && (write_ops == 0))
		{
			DEBUG ("diskpath plugin: ((read_ops == 0) && "
					"(write_ops == 0)); => Not writing.");
			continue;
		}

		if ((read_bytes != 0) || (write_bytes != 0))
			diskpath_submit (diskpath_name, "disk_octets",
					read_bytes, write_bytes);

		if ((read_ops != 0) || (write_ops != 0))
			diskpath_submit (diskpath_name, "disk_ops",
					read_ops, write_ops);

		if ((dsp->avg_read_time != 0) || (dsp->avg_write_time != 0))
			diskpath_submit (diskpath_name, "disk_time",
					dsp->avg_read_time, dsp->avg_write_time);

	}
#endif /* HAVE_PERFSTAT */

	return (0);
} /* int disk_read */

void module_register (void)
{
  plugin_register_config ("diskpath", diskpath_config,
      config_keys, config_keys_num);
  plugin_register_init ("diskpath", diskpath_init);
  plugin_register_read ("diskpath", diskpath_read);
} /* void module_register */
