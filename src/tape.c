/**
 * collectd - src/tape.c
 * Copyright (C) 2005,2006  Scott Garrett
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
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
 *   Scott Garrett <sgarrett at technomancer.com>
 **/

#include "collectd.h"

#include "plugin.h"
#include "utils/common/common.h"

#if HAVE_LIBKSTAT
/* none */
/* #endif HAVE_LIBKSTAT */
#elif HAVE_PERFSTAT
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
/* #endif HAVE_PERFSTAT */

#else
#error "No applicable input method."
#endif

#if HAVE_KSTAT_H
#include <kstat.h>
#endif

#if HAVE_LIBKSTAT
#define MAX_NUMTAPE 256
extern kstat_ctl_t *kc;
static kstat_t *ksp[MAX_NUMTAPE];
static int numtape;
/* #endif HAVE_LIBKSTAT */
#elif HAVE_PERFSTAT
static perfstat_tape_t *stat_tape;
static int numtape;
static int pnumtape;

typedef struct tapestats
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

  struct tapestats *next;
} tapestats_t;

static tapestats_t *tapelist;
#endif /* HAVE_PERFSTAT */

static int tape_init(void) {
  kstat_t *ksp_chain;

  numtape = 0;

  if (kc == NULL)
    return -1;

  for (numtape = 0, ksp_chain = kc->kc_chain;
       (numtape < MAX_NUMTAPE) && (ksp_chain != NULL);
       ksp_chain = ksp_chain->ks_next) {
    if (strncmp(ksp_chain->ks_class, "tape", 4))
      continue;
    if (ksp_chain->ks_type != KSTAT_TYPE_IO)
      continue;
    ksp[numtape++] = ksp_chain;
  }
/* #endif HAVE_LIBKSTAT */
#elif HAVE_PERFSTAT
#endif /* HAVE_PERFSTAT */
  return 0;
} /* int tape_init */

#if HAVE_PERFSTAT
static counter_t tape_calc_time_incr (counter_t delta_time, counter_t delta_ops)
{
  double interval = CDTIME_T_TO_DOUBLE (plugin_get_interval ());
  double avg_time = ((double) delta_time) / ((double) delta_ops);
  double avg_time_incr = interval * avg_time;

  return ((counter_t) (avg_time_incr + .5));
}
#endif

static void tape_submit(const char *plugin_instance, const char *type,
                        derive_t read, derive_t write) {
  value_list_t vl = VALUE_LIST_INIT;
  value_t values[] = {
      {.derive = read},
      {.derive = write},
  };

  vl.values = values;
  vl.values_len = STATIC_ARRAY_SIZE(values);
  sstrncpy(vl.plugin, "tape", sizeof(vl.plugin));
  sstrncpy(vl.plugin_instance, plugin_instance, sizeof(vl.plugin_instance));
  sstrncpy(vl.type, type, sizeof(vl.type));

  plugin_dispatch_values(&vl);
} /* void tape_submit */

static int tape_read(void) {
#ifdef HAVE_LIBKSTAT
#if HAVE_KSTAT_IO_T_WRITES && HAVE_KSTAT_IO_T_NWRITES && HAVE_KSTAT_IO_T_WTIME
#define KIO_ROCTETS reads
#define KIO_WOCTETS writes
#define KIO_ROPS nreads
#define KIO_WOPS nwrites
#define KIO_RTIME rtime
#define KIO_WTIME wtime
#elif HAVE_KSTAT_IO_T_NWRITTEN && HAVE_KSTAT_IO_T_WRITES &&                    \
    HAVE_KSTAT_IO_T_WTIME
#define KIO_ROCTETS nread
#define KIO_WOCTETS nwritten
#define KIO_ROPS reads
#define KIO_WOPS writes
#define KIO_RTIME rtime
#define KIO_WTIME wtime
#else
#error "kstat_io_t does not have the required members"
#endif
  static kstat_io_t kio;

  if (kc == NULL)
    return -1;

  if (numtape <= 0)
    return -1;

  for (int i = 0; i < numtape; i++) {
    if (kstat_read(kc, ksp[i], &kio) == -1)
      continue;

    if (strncmp(ksp[i]->ks_class, "tape", 4) == 0) {
      tape_submit(ksp[i]->ks_name, "tape_octets", kio.KIO_ROCTETS,
                  kio.KIO_WOCTETS);
      tape_submit(ksp[i]->ks_name, "tape_ops", kio.KIO_ROPS, kio.KIO_WOPS);
      /* FIXME: Convert this to microseconds if necessary */
      tape_submit(ksp[i]->ks_name, "tape_time", kio.KIO_RTIME, kio.KIO_WTIME);
    }
  }
/* #endif HAVE_LIBKSTAT */
#elif HAVE_PERFSTAT
       int rnumtape;
       int i;
       perfstat_id_t first;
       derive_t read_bytes;
       derive_t write_bytes;
       derive_t read_time;
       derive_t write_time;
       derive_t read_ops;
       derive_t write_ops;
       tapestats_t *ts, *pre_ts;

       /* check how many perfstat_tape_t structures are available */
       numtape =  perfstat_tape(NULL, NULL, sizeof(perfstat_tape_t), 0);
       if (numtape < 0)
       {
               char errbuf[1024];
               WARNING ("tape plugin: perfstat_tape: %s",
                       sstrerror (errno, errbuf, sizeof (errbuf)));
               return (-1);
       }

       if (numtape == 0)
               return (0);

       if (numtape != pnumtape || stat_tape == NULL)
       {
               sfree(stat_tape);
               stat_tape = (perfstat_tape_t *)calloc(numtape,
                       sizeof(perfstat_tape_t));
               if (stat_tape == NULL) {
                       ERROR ("tape plugin: malloc failed.");
                       return (-1);
               }
       }
       pnumtape = numtape;

       first.name[0]='\0';
       rnumtape = perfstat_tape(&first, stat_tape, sizeof(perfstat_tape_t),
               numtape);
       if (rnumtape < 0)
       {
               char errbuf[1024];
               WARNING ("tape plugin: perfstat_tape: %s",
                       sstrerror (errno, errbuf, sizeof (errbuf)));
               return (-1);
       }
       for (i=0; i < rnumtape ; i++)
       {
               derive_t diff_read_ops;
               derive_t diff_write_ops;
               derive_t diff_read_time;
               derive_t diff_write_time;
               char *tape_name;

               tape_name = stat_tape[i].name;

               for (ts = tapelist, pre_ts = tapelist; ts != NULL;
                       pre_ts = ts, ts = ts->next)
               {
                       if (strcmp (tape_name, ts->name) == 0)
                               break;
               }

               if (ts == NULL)
               {
                       ts = (tapestats_t *) calloc (1, sizeof (tapestats_t));
                       if (ts == NULL)
                               continue;

                       if ((ts->name = strdup (tape_name)) == NULL)
                       {
                               free (ts);
                               continue;
                       }

                       if (pre_ts == NULL)
                               tapelist = ts;
                       else
                               pre_ts->next = ts;
               }

               read_bytes = stat_tape[i].rblks*stat_tape[i].bsize;
               write_bytes = stat_tape[i].wblks*stat_tape[i].bsize;

               read_ops = stat_tape[i].rxfers;
               write_ops = stat_tape[i].xfers - stat_tape[i].rxfers;

               read_time = HTIC2NANOSEC(stat_tape[i].rserv) / 1000000.0;
               write_time = HTIC2NANOSEC(stat_tape[i].wserv) / 1000000.0;

               diff_read_ops = read_ops - ts->read_ops;
               diff_write_ops = write_ops - ts->write_ops;

               diff_read_time = read_time - ts->read_time;
               diff_write_time = write_time - ts->write_time;

               if (diff_read_ops != 0)
                       ts->avg_read_time += tape_calc_time_incr (
                               diff_read_time, diff_read_ops);
               if (diff_write_ops != 0)
                       ts->avg_write_time += tape_calc_time_incr (
                               diff_write_time, diff_write_ops);

               ts->read_ops = read_ops;
               ts->read_time = read_time;

               ts->write_ops = write_ops;
               ts->write_time = write_time;

               ts->poll_count++;
               if (ts->poll_count <= 2)
               {
                       DEBUG ("tape plugin: (ts->poll_count = %i) <= "
                                       "(min_poll_count = 2); => Not writing.",
                                       ts->poll_count);
                       continue;
               }

               if ((read_ops == 0) && (write_ops == 0))
               {
                       DEBUG ("tape plugin: ((read_ops == 0) && "
                                       "(write_ops == 0)); => Not writing.");
                       continue;
               }

               if ((read_bytes != 0) || (write_bytes != 0))
                       tape_submit (tape_name, "tape_octets",
                                       read_bytes, write_bytes);

               if ((read_ops != 0) || (write_ops != 0))
                       tape_submit (tape_name, "tape_ops",
                                       read_ops, write_ops);

               if ((ts->avg_read_time != 0) || (ts->avg_write_time != 0))
                       tape_submit (tape_name, "tape_time",
                                       ts->avg_read_time, ts->avg_write_time);
       }
#endif /* HAVE_PERFSTAT */
  return 0;
}

void module_register(void) {
  plugin_register_init("tape", tape_init);
  plugin_register_read("tape", tape_read);
}
