#include "file.h"
#include "types.h"
#include "contiki.h"
#include "message.h"
#include "leds.h"

PROCESS(nullnet_example_process, "ImageUpload");
AUTOSTART_PROCESSES(&nullnet_example_process);

PROCESS_THREAD(nullnet_example_process, ev, data)
{
  PROCESS_BEGIN();

  file_init();
  message_init();

  //leds_on(LEDS_GREEN);
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
        reply.data.write_rep.success = file_write(
          received.data.write_req.address,
          received.data.write_req.write_data,
          received.data.write_req.length);
        
        message_send(
          &reply,
          MESSAGE_TYPE_WRITE_RESPONE,
          sizeof(reply.data.write_rep));
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
