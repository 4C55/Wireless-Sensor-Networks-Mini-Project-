#include "file.h"
#include "types.h"
#include "contiki.h"
#include "message.h"

PROCESS(nullnet_example_process, "ImageUpload");
AUTOSTART_PROCESSES(&nullnet_example_process);

PROCESS_THREAD(nullnet_example_process, ev, data)
{
  PROCESS_BEGIN();

  file_init();
  message_init();
  PROCESS_PAUSE();
  
  PROCESS_PAUSE();

  while(1) {
    PROCESS_PAUSE();
    message_process();
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
