#include "file.h"
#include "types.h"
#include "contiki.h"

PROCESS(nullnet_example_process, "ImageUpload");
AUTOSTART_PROCESSES(&nullnet_example_process);

PROCESS_THREAD(nullnet_example_process, ev, data)
{
  static uint32_t i; 
  PROCESS_BEGIN();

  file_init();
  PROCESS_PAUSE();

  static uint8_t buffer[50];
                   
  static uint8_t data[] = "0123456789";//"abcdefghij";

  file_write(0, data, 10);
  PROCESS_PAUSE();
  file_write(10, data, 10);
  PROCESS_PAUSE();
  file_read(0, buffer, 20);
  PROCESS_PAUSE();

  for (i = 0; i < 20; i++) {
    printf("%c ", buffer[i]);
    PROCESS_PAUSE();
  }

  printf("\n");

  for (i = 0; i < 20; i++) {
    printf("%c ", buffer[i]);
    PROCESS_PAUSE();
  }

  printf("\n");

  while(1) {
      PROCESS_PAUSE();
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
