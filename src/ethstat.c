/**
 * collectd - src/ethstat.c
 * Copyright (C) 2011       Cyril Feraudet
 * Copyright (C) 2012       Florian "octo" Forster
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
 *   Cyril Feraudet <cyril at feraudet.com>
 *   Florian "octo" Forster <octo@collectd.org>
 **/

#include "collectd.h"

#include "plugin.h"
#include "utils/avltree/avltree.h"
#include "utils/common/common.h"
#include "utils_complain.h"

#include <stddef.h>

#if HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#if HAVE_NET_IF_H
#include <net/if.h>
#endif
#if HAVE_LINUX_SOCKIOS_H
#include <linux/sockios.h>
#endif
#if HAVE_LINUX_ETHTOOL_H
#include <linux/ethtool.h>
#endif

#if HAVE_PERFSTAT
# include <libperfstat.h>
#endif

#if HAVE_PERFSTAT
struct netadapter_field {
  char *field;
  size_t offset;
};
typedef struct netadapter_field netadapter_field_t;

static netadapter_field_t netadapter_fields[] = {
  { "tx_packets", offsetof(perfstat_netadapter_t, tx_packets) },
  { "tx_bytes", offsetof(perfstat_netadapter_t, tx_bytes) },
  { "tx_interrupts", offsetof(perfstat_netadapter_t, tx_interrupts) },
  { "tx_errors", offsetof(perfstat_netadapter_t, tx_errors) },
  { "tx_packets_dropped", offsetof(perfstat_netadapter_t, tx_packets_dropped) },
  { "tx_queue_size", offsetof(perfstat_netadapter_t, tx_queue_size) },
  { "tx_queue_len", offsetof(perfstat_netadapter_t, tx_queue_len) },
  { "tx_queue_overflow", offsetof(perfstat_netadapter_t, tx_queue_overflow) },
  { "tx_broadcast_packets", offsetof(perfstat_netadapter_t, tx_broadcast_packets) },
  { "tx_multicast_packets", offsetof(perfstat_netadapter_t, tx_multicast_packets) },
  { "tx_carrier_sense", offsetof(perfstat_netadapter_t, tx_carrier_sense) },
  { "tx_DMA_underrun", offsetof(perfstat_netadapter_t, tx_DMA_underrun) },
  { "tx_lost_CTS_errors", offsetof(perfstat_netadapter_t, tx_lost_CTS_errors) },
  { "tx_max_collision_errors", offsetof(perfstat_netadapter_t, tx_max_collision_errors) },
  { "tx_late_collision_errors", offsetof(perfstat_netadapter_t, tx_late_collision_errors) },
  { "tx_deferred", offsetof(perfstat_netadapter_t, tx_deferred) },
  { "tx_timeout_errors", offsetof(perfstat_netadapter_t, tx_timeout_errors) },
  { "tx_single_collision_count", offsetof(perfstat_netadapter_t, tx_single_collision_count) },
  { "tx_multiple_collision_count", offsetof(perfstat_netadapter_t, tx_multiple_collision_count) },
  { "rx_packets", offsetof(perfstat_netadapter_t, rx_packets) },
  { "rx_bytes", offsetof(perfstat_netadapter_t, rx_bytes) },
  { "rx_interrupts", offsetof(perfstat_netadapter_t, rx_interrupts) },
  { "rx_errors", offsetof(perfstat_netadapter_t, rx_errors) },
  { "rx_packets_dropped", offsetof(perfstat_netadapter_t, rx_packets_dropped) },
  { "rx_bad_packets", offsetof(perfstat_netadapter_t, rx_bad_packets) },
  { "rx_multicast_packets", offsetof(perfstat_netadapter_t, rx_multicast_packets) },
  { "rx_broadcast_packets", offsetof(perfstat_netadapter_t, rx_broadcast_packets) },
  { "rx_CRC_errors", offsetof(perfstat_netadapter_t, rx_CRC_errors) },
  { "rx_DMA_overrun", offsetof(perfstat_netadapter_t, rx_DMA_overrun) },
  { "rx_alignment_errors", offsetof(perfstat_netadapter_t, rx_alignment_errors) },
  { "rx_noresource_errors", offsetof(perfstat_netadapter_t, rx_noresource_errors) },
  { "rx_collision_errors", offsetof(perfstat_netadapter_t, rx_collision_errors) },
  { "rx_packet_tooshort_errors", offsetof(perfstat_netadapter_t, rx_packet_tooshort_errors) },
  { "rx_packet_toolong_errors", offsetof(perfstat_netadapter_t, rx_packet_toolong_errors) },
  { "rx_packets_discardedbyadapter", offsetof(perfstat_netadapter_t, rx_packets_discardedbyadapter) }
};

static int netadapter_fields_size = STATIC_ARRAY_SIZE(netadapter_fields);
#endif /* HAVE_PERFSTAT */

struct value_map_s {
  char type[DATA_MAX_NAME_LEN];
  char type_instance[DATA_MAX_NAME_LEN];
};
typedef struct value_map_s value_map_t;

static char **interfaces;
static size_t interfaces_num;

static c_avl_tree_t *value_map;

static bool collect_mapped_only;

static int ethstat_add_interface(const oconfig_item_t *ci) /* {{{ */
{
  char **tmp;
  int status;

  tmp = realloc(interfaces, sizeof(*interfaces) * (interfaces_num + 1));
  if (tmp == NULL)
    return -1;
  interfaces = tmp;
  interfaces[interfaces_num] = NULL;

  status = cf_util_get_string(ci, interfaces + interfaces_num);
  if (status != 0)
    return status;

  interfaces_num++;
  INFO("ethstat plugin: Registered interface %s",
       interfaces[interfaces_num - 1]);

  return 0;
} /* }}} int ethstat_add_interface */

static int ethstat_add_map(const oconfig_item_t *ci) /* {{{ */
{
  value_map_t *map;
  int status;
  char *key;

  if ((ci->values_num < 2) || (ci->values_num > 3) ||
      (ci->values[0].type != OCONFIG_TYPE_STRING) ||
      (ci->values[1].type != OCONFIG_TYPE_STRING) ||
      ((ci->values_num == 3) && (ci->values[2].type != OCONFIG_TYPE_STRING))) {
    ERROR("ethstat plugin: The %s option requires "
          "two or three string arguments.",
          ci->key);
    return -1;
  }

  key = strdup(ci->values[0].value.string);
  if (key == NULL) {
    ERROR("ethstat plugin: strdup(3) failed.");
    return ENOMEM;
  }

  map = calloc(1, sizeof(*map));
  if (map == NULL) {
    sfree(key);
    ERROR("ethstat plugin: calloc failed.");
    return ENOMEM;
  }

  sstrncpy(map->type, ci->values[1].value.string, sizeof(map->type));
  if (ci->values_num == 3)
    sstrncpy(map->type_instance, ci->values[2].value.string,
             sizeof(map->type_instance));

  if (value_map == NULL) {
    value_map = c_avl_create((int (*)(const void *, const void *))strcmp);
    if (value_map == NULL) {
      sfree(map);
      sfree(key);
      ERROR("ethstat plugin: c_avl_create() failed.");
      return -1;
    }
  }

  status = c_avl_insert(value_map,
                        /* key = */ key,
                        /* value = */ map);
  if (status != 0) {
    if (status > 0)
      ERROR("ethstat plugin: Multiple mappings for \"%s\".", key);
    else
      ERROR("ethstat plugin: c_avl_insert(\"%s\") failed.", key);

    sfree(map);
    sfree(key);
    return -1;
  }

  return 0;
} /* }}} int ethstat_add_map */

static int ethstat_config(oconfig_item_t *ci) /* {{{ */
{
  for (int i = 0; i < ci->children_num; i++) {
    oconfig_item_t *child = ci->children + i;

    if (strcasecmp("Interface", child->key) == 0)
      ethstat_add_interface(child);
    else if (strcasecmp("Map", child->key) == 0)
      ethstat_add_map(child);
    else if (strcasecmp("MappedOnly", child->key) == 0)
      (void)cf_util_get_boolean(child, &collect_mapped_only);
    else
      WARNING("ethstat plugin: The config option \"%s\" is unknown.",
              child->key);
  }

  return 0;
} /* }}} */

static void ethstat_submit_value(const char *device, const char *type_instance,
                                 derive_t value) {
  static c_complain_t complain_no_map = C_COMPLAIN_INIT_STATIC;

  value_list_t vl = VALUE_LIST_INIT;
  value_map_t *map = NULL;

  if (value_map != NULL)
    c_avl_get(value_map, type_instance, (void *)&map);

  /* If the "MappedOnly" option is specified, ignore unmapped values. */
  if (collect_mapped_only && (map == NULL)) {
    if (value_map == NULL)
      c_complain(
          LOG_WARNING, &complain_no_map,
          "ethstat plugin: The \"MappedOnly\" option has been set to true, "
          "but no mapping has been configured. All values will be ignored!");
    return;
  }

  vl.values = &(value_t){.derive = value};
  vl.values_len = 1;

  sstrncpy(vl.plugin, "ethstat", sizeof(vl.plugin));
  sstrncpy(vl.plugin_instance, device, sizeof(vl.plugin_instance));
  if (map != NULL) {
    sstrncpy(vl.type, map->type, sizeof(vl.type));
    sstrncpy(vl.type_instance, map->type_instance, sizeof(vl.type_instance));
  } else {
    sstrncpy(vl.type, "derive", sizeof(vl.type));
    sstrncpy(vl.type_instance, type_instance, sizeof(vl.type_instance));
  }

  plugin_dispatch_values(&vl);
}

#ifdef KERNEL_LINUX
static int ethstat_read_interface (char *device) /* {{{ ethstat_read_interface */
{
  int fd;
  struct ethtool_gstrings *strings;
  struct ethtool_stats *stats;
  size_t n_stats;
  size_t strings_size;
  size_t stats_size;
  int status;

  fd = socket(AF_INET, SOCK_DGRAM, /* protocol = */ 0);
  if (fd < 0) {
    ERROR("ethstat plugin: Failed to open control socket: %s", STRERRNO);
    return 1;
  }

  struct ethtool_drvinfo drvinfo = {.cmd = ETHTOOL_GDRVINFO};

  struct ifreq req = {.ifr_data = (void *)&drvinfo};

  sstrncpy(req.ifr_name, device, sizeof(req.ifr_name));

  status = ioctl(fd, SIOCETHTOOL, &req);
  if (status < 0) {
    close(fd);
    ERROR("ethstat plugin: Failed to get driver information "
          "from %s: %s",
          device, STRERRNO);
    return -1;
  }

  n_stats = (size_t)drvinfo.n_stats;
  if (n_stats < 1) {
    close(fd);
    ERROR("ethstat plugin: No stats available for %s", device);
    return -1;
  }

  strings_size = sizeof(struct ethtool_gstrings) + (n_stats * ETH_GSTRING_LEN);
  stats_size = sizeof(struct ethtool_stats) + (n_stats * sizeof(uint64_t));

  strings = malloc(strings_size);
  stats = malloc(stats_size);
  if ((strings == NULL) || (stats == NULL)) {
    close(fd);
    sfree(strings);
    sfree(stats);
    ERROR("ethstat plugin: malloc failed.");
    return -1;
  }

  strings->cmd = ETHTOOL_GSTRINGS;
  strings->string_set = ETH_SS_STATS;
  strings->len = n_stats;
  req.ifr_data = (void *)strings;
  status = ioctl(fd, SIOCETHTOOL, &req);
  if (status < 0) {
    close(fd);
    free(strings);
    free(stats);
    ERROR("ethstat plugin: Cannot get strings from %s: %s", device, STRERRNO);
    return -1;
  }

  stats->cmd = ETHTOOL_GSTATS;
  stats->n_stats = n_stats;
  req.ifr_data = (void *)stats;
  status = ioctl(fd, SIOCETHTOOL, &req);
  if (status < 0) {
    close(fd);
    free(strings);
    free(stats);
    ERROR("ethstat plugin: Reading statistics from %s failed: %s", device,
          STRERRNO);
    return -1;
  }

  for (size_t i = 0; i < n_stats; i++) {
    char *stat_name;

    stat_name = (void *)&strings->data[i * ETH_GSTRING_LEN];
    /* Remove leading spaces in key name */
    while (isspace((int)*stat_name))
      stat_name++;

    DEBUG("ethstat plugin: device = \"%s\": %s = %" PRIu64, device, stat_name,
          (uint64_t)stats->data[i]);
    ethstat_submit_value(device, stat_name, (derive_t)stats->data[i]);
  }

  close(fd);
  sfree(strings);
  sfree(stats);

  return 0;
} /* }}} ethstat_read_interface */
/* #endif KERNEL_LINUX */
#elif HAVE_PERFSTAT
static int ethstat_read_interface (char *device) /* {{{ ethstat_read_interface */
{
  int ret, i;
  perfstat_id_t id;
  perfstat_netadapter_t stat;

  sstrncpy(id.name , device, IDENTIFIER_LENGTH);

  ret = perfstat_netadapter(&id, &stat, sizeof(perfstat_netadapter_t), 1);
  if (ret < 0)
  {
    char errbuf[1024];
    WARNING ("ethstat plugin: perfstat_netadapter (adapter=%s): %s",
      device, sstrerror (errno, errbuf, sizeof (errbuf)));
    return (-1);
  }

  if (ret != 1)
  {
    WARNING ("ethstat plugin: perfstat_netadapter (adapter=%s): not found",
      device);
    return (-1);
  }

  for (i=0; i < netadapter_fields_size; i++) 
  {
    u_longlong_t counter;

    counter = *(u_longlong_t *)((char *)&stat + netadapter_fields[i].offset);
    ethstat_submit_value(device, netadapter_fields[i].field, counter);
  }

  return (0);
} /* }}} ethstat_read_interface */
#endif


static int ethstat_read(void) {
  for (size_t i = 0; i < interfaces_num; i++)
    ethstat_read_interface(interfaces[i]);

  return 0;
}

static int ethstat_shutdown(void) {
  void *key = NULL;
  void *value = NULL;

  if (value_map == NULL)
    return 0;

  while (c_avl_pick(value_map, &key, &value) == 0) {
    sfree(key);
    sfree(value);
  }

  c_avl_destroy(value_map);
  value_map = NULL;

  return 0;
}

void module_register(void) {
  plugin_register_complex_config("ethstat", ethstat_config);
  plugin_register_read("ethstat", ethstat_read);
  plugin_register_shutdown("ethstat", ethstat_shutdown);
}
