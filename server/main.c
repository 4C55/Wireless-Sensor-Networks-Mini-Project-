#include <stdio.h>
#include <stdlib.h>
#include "clock.h"
#include "contiki.h"
#include "leds.h"
#include "sys/log.h"
#include "dev/button-sensor.h"

#define LOG_MODULE "Test"
#define LOG_LEVEL LOG_LEVEL_INFO

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND);
  SENSORS_ACTIVATE(button_sensor);

  while (1)
  {
    LOG_INFO("Server started\n");

    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
  
    leds_on(LEDS_RED);
    etimer_restart(&timer);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    leds_on(LEDS_GREEN);
    etimer_restart(&timer);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));    

    leds_on(LEDS_YELLOW);
    etimer_restart(&timer);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    leds_off(LEDS_RED | LEDS_GREEN | LEDS_YELLOW);
    etimer_restart(&timer);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  }

  PROCESS_END();
}