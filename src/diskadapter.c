/**
 * collectd - src/diskadapter.c
 * Copyright (C) 2005-2012  Florian octo Forster
 * Copyright (C) 2014  Manuel Sanmartín
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
# include <libperfstat.h>
#endif

#if HAVE_PERFSTAT
static perfstat_diskadapter_t *stat_diskadapter;
static int num_diskadapter;
static int pnum_diskadapter;
# if HAVE_PERFSTAT_VIRTUALDISKADAPTER
static perfstat_diskadapter_t *stat_virtualdiskadapter;
static int num_virtualdiskadapter;
static int pnum_virtualdiskadapter;
# endif /* HAVE_PERFSTAT_VIRTUALDISKADAPTER */
/* #endif HAVE_PERFSTAT */

#else
# error "No applicable input method."
#endif

static const char *config_keys[] =
{
	"DiskAdapter",
	"IgnoreSelected"
};
static int config_keys_num = STATIC_ARRAY_SIZE (config_keys);

static ignorelist_t *ignorelist = NULL;

static int diskadapter_config (const char *key, const char *value)
{
  if (ignorelist == NULL)
    ignorelist = ignorelist_create (/* invert = */ 1);
  if (ignorelist == NULL)
    return (1);

  if (strcasecmp ("DiskAdapter", key) == 0)
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
} /* int diskadapter_config */

static int diskadapter_init (void)
{
	return (0);
} /* int diskadapter_init */

static void diskadapter_submit (const char *plugin_instance,
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
	sstrncpy (vl.plugin, "diskadapter", sizeof (vl.plugin));
	sstrncpy (vl.plugin_instance, plugin_instance,
			sizeof (vl.plugin_instance));
	sstrncpy (vl.type, type, sizeof (vl.type));

	plugin_dispatch_values (&vl);
} /* void diskadapter_submit */

#if HAVE_PERFSTAT
static void diskadapter_submit_all (perfstat_diskadapter_t *stat, int size)
{
	derive_t read_bytes;
	derive_t write_bytes;
	derive_t read_ops;
	derive_t write_ops;
	char *diskadapter_name;
	int i;

	for (i = 0; i < size; i++)
	{
		diskadapter_name = stat[i].name;

		read_bytes = stat[i].rblks*512;
		write_bytes = stat[i].wblks*512;

		read_ops = stat[i].xrate;
		write_ops = stat[i].xfers - stat[i].xrate;

		if ((read_ops == 0) && (write_ops == 0))
		{
			DEBUG ("diskadapter plugin: ((read_ops == 0) && "
					"(write_ops == 0)); => Not writing.");
			continue;
		}

		if ((read_bytes != 0) || (write_bytes != 0))
			diskadapter_submit (diskadapter_name, "disk_octets",
					read_bytes, write_bytes);

		if ((read_ops != 0) || (write_ops != 0))
			diskadapter_submit (diskadapter_name, "disk_ops",
					read_ops, write_ops);
	}
}

#endif /* HAVE_PERFSTAT */

static int diskadapter_read (void)
{
#if HAVE_PERFSTAT
	perfstat_id_t first_id;
	int rnum_diskadapter;

	num_diskadapter = perfstat_diskadapter(NULL, NULL, sizeof(perfstat_diskadapter_t), 0);
	if (num_diskadapter < 0)
	{
		char errbuf[1024];
		WARNING ("diskadapter plugin: perfstat_diskadapter: %s",
				sstrerror (errno, errbuf, sizeof (errbuf)));
		return (-1);
	}

	if (num_diskadapter != pnum_diskadapter || stat_diskadapter==NULL) 
	{
		sfree(stat_diskadapter);
		stat_diskadapter = (perfstat_diskadapter_t *)calloc(num_diskadapter,
			sizeof(perfstat_diskadapter_t));
		if (stat_diskadapter == NULL) 
		{
			ERROR ("diskadapter plugin: calloc failed.");
			return (-1);
		}
	} 
	pnum_diskadapter = num_diskadapter;

	first_id.name[0]='\0';
	rnum_diskadapter = perfstat_diskadapter(&first_id, stat_diskadapter,
		sizeof(perfstat_diskadapter_t), num_diskadapter);
	if (rnum_diskadapter < 0)
	{
		char errbuf[1024];
		WARNING ("diskadapter plugin: perfstat_diskadapter: %s",
				sstrerror (errno, errbuf, sizeof (errbuf)));
		return (-1);
	}

	diskadapter_submit_all(stat_diskadapter, rnum_diskadapter);

# if HAVE_PERFSTAT_VIRTUALDISKADAPTER

	num_virtualdiskadapter = perfstat_virtualdiskadapter(NULL, NULL, sizeof(perfstat_diskadapter_t), 0);
	if (num_virtualdiskadapter < 0)
	{
		char errbuf[1024];
		WARNING ("diskadapter plugin: perfstat_virtualdiskadapter: %s",
				sstrerror (errno, errbuf, sizeof (errbuf)));
		return (-1);
	}

	if (num_virtualdiskadapter == 0)
		return (0);

	if (num_virtualdiskadapter != pnum_virtualdiskadapter || stat_virtualdiskadapter==NULL) 
	{
		sfree(stat_virtualdiskadapter);
		stat_virtualdiskadapter = (perfstat_diskadapter_t *)calloc(num_virtualdiskadapter,
			sizeof(perfstat_diskadapter_t));
		if (stat_virtualdiskadapter == NULL) 
		{
			ERROR ("diskadapter plugin: calloc failed.");
			return (-1);
		}
	} 
	pnum_virtualdiskadapter = num_virtualdiskadapter;

	first_id.name[0]='\0';
	rnum_diskadapter = perfstat_virtualdiskadapter(&first_id, stat_virtualdiskadapter,
		sizeof(perfstat_diskadapter_t), num_virtualdiskadapter);
	if (rnum_diskadapter < 0)
	{
		char errbuf[1024];
		WARNING ("diskadapter plugin: perfstat_virtualdiskadapter: %s",
				sstrerror (errno, errbuf, sizeof (errbuf)));
		return (-1);
	}

	diskadapter_submit_all(stat_virtualdiskadapter, rnum_diskadapter);

# endif /* HAVE_PERFSTAT_VIRTUALDISKADAPTER */

#endif /* HAVE_PERFSTAT */

	return (0);
} /* int disk_read */

void module_register (void)
{
  plugin_register_config ("diskadapter", diskadapter_config,
      config_keys, config_keys_num);
  plugin_register_init ("diskadapter", diskadapter_init);
  plugin_register_read ("diskadapter", diskadapter_read);
} /* void module_register */
