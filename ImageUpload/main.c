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

  static struct message received;
  static struct message reply;
  
  while(1) {
    PROCESS_PAUSE();
    message_process();
    
    if (!message_get(&received)) {
      continue;
    }

    switch (received.type.type_value)
    {
      case MESSAGE_TYPE_TEST_REQUEST:
      {
        message_send(&reply, MESSAGE_TYPE_TEST_RESPONSE, 0);
        break;
      }
      case MESSAGE_TYPE_WRITE_REQUEST:
      {
        message_send(&reply, MESSAGE_TYPE_WRITE_RESPONE, 0);
        break;
      }
      default:
      {
        break;
      }
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
