#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"

#define UDP_LOCAL_PORT	5678
#define UDP_REMOTE_PORT	8765

static struct simple_udp_connection udp_conn;

PROCESS(sink_process, "UDP server");
AUTOSTART_PROCESSES(&sink_process);

static void udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  uint16_t i;

  for (i = 0; i < datalen; i++) {
    putchar(data[i]);
  }
}

PROCESS_THREAD(sink_process, ev, data)
{
  PROCESS_BEGIN();

  /* Initialize DAG root */
  NETSTACK_ROUTING.root_start();

  /* Initialize UDP connection */
  simple_udp_register(
    &udp_conn,
    UDP_LOCAL_PORT,
    NULL,
    UDP_REMOTE_PORT,
    udp_rx_callback);

  printf("Sink running\n");

  while (1) {
    PROCESS_PAUSE();
  }

  PROCESS_END();
}