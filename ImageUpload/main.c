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
  static uint8_t reply_type = MESSAGE_TYPE_EMPTY;
  static uint8_t reply_length;

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
        reply_type = MESSAGE_TYPE_TEST_RESPONSE;
        reply_length = 0;
        break;
      }
      case MESSAGE_TYPE_WRITE_REQUEST:
      {
        reply.data.write_rep.success = file_write(
          received.data.write_req.address,
          received.data.write_req.write_data,
          received.data.write_req.length);

        reply_type = MESSAGE_TYPE_WRITE_RESPONE;
        reply_length = sizeof(reply.data.write_rep);
        break;
      }
      case MESSAGE_TYPE_READ_REQUEST:
      {
        reply.data.read_rep.length = received.data.read_req.length;
        bool_t success = file_read(
          received.data.read_req.address,
          reply.data.read_rep.read_data,
          received.data.read_req.length);

        if (!success) {
          break;
        }
        
        reply_type = MESSAGE_TYPE_READ_RESPONSE;
        reply_length = sizeof(reply.data.read_rep);
        break;
      }
      case MESSAGE_TYPE_FORMAT_REQUEST:
      {
        reply.data.format_rep.success = file_format_memory();
        
        reply_type = MESSAGE_TYPE_FORMAT_RESPONSE;
        reply_length = sizeof(reply.data.format_rep);
        break;
      }
      default:
      {
        break;
      }
    }

    PROCESS_PAUSE();
    
    if (reply_type == MESSAGE_TYPE_EMPTY) {
      continue;
    }

    message_send(
          &reply,
          reply_type,
          reply_length);
    reply_type = MESSAGE_TYPE_EMPTY;
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
