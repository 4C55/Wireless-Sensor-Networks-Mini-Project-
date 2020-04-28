#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "dev/leds.h"
#include <string.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(nullnet_example_process, "Consumer");
AUTOSTART_PROCESSES(&nullnet_example_process);

/*---------------------------------------------------------------------------*/
void input_callback(
  const void *data,
  uint16_t len,
  const linkaddr_t *src,
  const linkaddr_t *dest)
{
  int i = 0;
  for (i = 0; i < len; i++) {
    putchar(((uint8_t *)data)[i]);
  }
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(nullnet_example_process, ev, data)
{
  static struct etimer periodic_timer;

  PROCESS_BEGIN();

  nullnet_set_input_callback(input_callback);

  etimer_set(&periodic_timer, 1 * CLOCK_SECOND);
  while(1) {
    leds_toggle(LEDS_GREEN);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}