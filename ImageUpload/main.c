#include "file.h"
#include "types.h"
#include "contiki.h"
#include "lena1.h"

PROCESS(nullnet_example_process, "ImageUpload");
AUTOSTART_PROCESSES(&nullnet_example_process);

PROCESS_THREAD(nullnet_example_process, ev, data)
{
  PROCESS_BEGIN();

  file_init();

  while(1) {
      PROCESS_PAUSE();
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
