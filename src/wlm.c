/**
 * collectd - src/wlm.c
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

#ifdef HAVE_WLM
#include <sys/wlm.h>
#else
# error "No applicable input method."
#endif

static struct wlm_info *wlminfo;
static int prev_wlmcnt;

static const char *config_keys[] =
{
  "SuperClassOnly",
  "SubClassOnly",
  "Class"
};
static int config_keys_num = STATIC_ARRAY_SIZE (config_keys);

static bool wlm_super_only = false;
static bool wlm_sub_only = false;
static char *wlm_name = NULL;

static int wlm_versflags;

static int wlm_config (const char *key, const char *value) /* {{{ */
{
	if (strcasecmp ("SuperClassOnly", key) == 0)
	{
		if (IS_TRUE (value))
			wlm_super_only = true;
		else
			wlm_super_only = false;
	}
	else if (strcasecmp ("SubClassOnly", key) == 0)
	{
		if (IS_TRUE (value))
			wlm_sub_only = true;
		else
			wlm_sub_only = false;
	}
	else if (strcasecmp ("Class", key) == 0)
	{
		sfree (wlm_name);
		wlm_name = sstrdup (value);
	}
	else
	{
		return (-1);
	}

	return (0);
} /* }}} int wlm_config */

static int wlm_init(void) /* {{{ */
{
	if (wlm_initialize(WLM_VERSION) != 0)
	{
		char errbuf[1024];
		WARNING ("wlm plugin: wlm_initialize: %s",
			sstrerror (errno, errbuf, sizeof (errbuf)));
		return (-1);
	}

	wlm_versflags = WLM_VERSION;

	if (wlm_super_only)
		wlm_versflags |= WLM_SUPER_ONLY;

	if (wlm_sub_only)
		wlm_versflags |= WLM_SUB_ONLY;

	return (0);
} /* }}} int wlm_init */

static void submit (const char *plugin_instace, /* {{{ */
    const char *type_instance, gauge_t value)
{
	value_t values[1];
	value_list_t vl = VALUE_LIST_INIT;

	values[0].gauge = value;

	vl.values = values;
	vl.values_len = STATIC_ARRAY_SIZE (values);

	sstrncpy (vl.host, hostname_g, sizeof (vl.host));
	sstrncpy (vl.plugin, "wlm", sizeof (vl.plugin));
	sstrncpy (vl.plugin_instance, plugin_instace , sizeof (vl.plugin_instance));
	sstrncpy (vl.type, "wlm", sizeof (vl.type));
	sstrncpy (vl.type_instance, type_instance, sizeof (vl.type_instance));

	plugin_dispatch_values (&vl);
} /* }}} void submit */

static int wlm_read (void) /* {{{ */
{
	struct wlm_args wlmargs;
	struct wlm_info *pwlminfo;
	int wlmcnt;
	int status;
	int i;

	memset(&wlmargs, 0, sizeof(wlmargs));
	wlmargs.versflags = wlm_versflags;

	if (wlm_name)
	{
		sstrncpy (wlmargs.cl_def.data.descr.name,
			wlm_name, sizeof(wlmargs.cl_def.data.descr.name));
	}
	wlmcnt = 0;

	/* Get the number of WLMs */
	status = wlm_get_info(/* struct wlm_args */ &wlmargs,
		/* struct wlm_info */ NULL,
		/* (out) */ &wlmcnt);
	if (status != 0)
	{
		char errbuf[1024];
		WARNING ("wlm plugin: wlm_get_info: %s",
			sstrerror (errno, errbuf, sizeof (errbuf)));
		return (-1);
	}

	if (wlmcnt > 0)
	{
		/*  If necessary, reallocate the "wlminfo" memory */
		if (prev_wlmcnt != wlmcnt || wlminfo == NULL)
		{
			struct wlm_info *tmp;

			tmp = realloc (wlminfo, wlmcnt * sizeof(struct wlm_info));
			if (tmp == NULL)
			{
				ERROR ("wlm plugin: realloc failed.");
				return (ENOMEM);
			}
                              wlminfo = tmp;
                    }
		prev_wlmcnt = wlmcnt;

		/* Get wlm_info of all WLMs */
		status = wlm_get_info(/* struct wlm_args */ &wlmargs,
			/* (out) struct wlm_info */ wlminfo,
			/* size */ &wlmcnt);
		if (status != 0)
                    {
			char errbuf[1024];
			WARNING ("wlm plugin: wlm_get_info: %s",
				sstrerror (errno, errbuf, sizeof (errbuf)));
			return (-1);
		}

		/* Iterate over all WLMs and dispatch information */
		for (i = 0, pwlminfo=wlminfo; i < wlmcnt; i++, pwlminfo++)
		{
			submit (pwlminfo->i_descr.name, "cpu",
			        (gauge_t) pwlminfo->i_regul[WLM_RES_CPU].consum/100.0);
			submit (pwlminfo->i_descr.name, "mem",
			        (gauge_t) pwlminfo->i_regul[WLM_RES_MEM].consum/100.0);
			submit (pwlminfo->i_descr.name, "bio",
			        (gauge_t) pwlminfo->i_regul[WLM_RES_BIO].consum/100.0);
		} /* for (i = 0 ... wlmcnt) */
	}

	return (0);
} /* }}} int wlm_read */

void module_register (void)
{
	plugin_register_config ("wlm", wlm_config,
		config_keys, config_keys_num);
	plugin_register_init ("wlm", wlm_init);
	plugin_register_read ("wlm", wlm_read);
} /* void module_register */

/* vim: set fdm=marker sw=4 ts=4 tw=78 et : */
