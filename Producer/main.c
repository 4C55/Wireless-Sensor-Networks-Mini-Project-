#include "File/file.h"
#include "Connection/connection.h"
#include "types.h"
#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"

PROCESS(nullnet_example_process, "Nullnet");
AUTOSTART_PROCESSES(&nullnet_example_process);

PROCESS_THREAD(nullnet_example_process, ev, data)
{
  static uint8_t buffer[2000];
  static uint32_t i = 0;
  static uint32_t j = 0;
  PROCESS_BEGIN();

  file_init();
  connection_init();
  PROCESS_PAUSE();

  for (i = 0; i < 66000; i = i + 2000) {
    file_read(i, buffer, 2000);
    PROCESS_PAUSE();

    for (j = 0; j < 2000; j = j + 100) {
      connection_start_sending(buffer + j, 100);

      while (!connection_has_finished_sending()) {
        PROCESS_PAUSE();
      }
    }
  }

  while(1) {
      PROCESS_PAUSE();
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
