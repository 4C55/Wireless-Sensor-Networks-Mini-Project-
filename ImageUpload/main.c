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

  static struct message message;
  static uint8_t reply_destination;

  file_init();
  message_init(&message);
  //leds_on(LEDS_GREEN);
  PROCESS_PAUSE();
  
  PROCESS_PAUSE();

  

  while(1) {
    PROCESS_PAUSE();
    message_process();
    PROCESS_PAUSE();
    
    if (!message_get()) {
      continue;
    }

    reply_destination = message.source;

    if (message.type.type_value == MESSAGE_TYPE_TEST_REQUEST) {
      message_send(
          &message,
          reply_destination,
          MESSAGE_TYPE_TEST_RESPONSE,
          0);
    } else if (message.type.type_value == MESSAGE_TYPE_WRITE_REQUEST) {
      uint32_t address = message.data.write_req.address;
      uint32_t length = message.data.write_req.length;
      uint8_t *data = message.data.write_req.write_data;
      message.data.write_rep.success = file_write(address, data, length);

      PROCESS_PAUSE();

      message_send(
          &message,
          reply_destination,
          MESSAGE_TYPE_WRITE_RESPONE,
          sizeof(message.data.write_rep));      
    } else if (message.type.type_value == MESSAGE_TYPE_READ_REQUEST) {
      uint32_t address = message.data.read_req.address;
      uint32_t length = message.data.read_req.length;

      uint8_t *data = message.data.read_rep.read_data;
      message.data.read_rep.length = length;
      bool_t success = file_read(address, data, length);
      if (!success) {
        break;
      }

      PROCESS_PAUSE();

      message_send(
          &message,
          reply_destination,
          MESSAGE_TYPE_READ_RESPONSE,
          sizeof(message.data.read_rep)); 
    } else if (message.type.type_value == MESSAGE_TYPE_FORMAT_REQUEST) {
      message.data.format_rep.success = file_format_memory();

      PROCESS_PAUSE();

      message_send(
          &message,
          reply_destination,
          MESSAGE_TYPE_FORMAT_RESPONSE,
          sizeof(message.data.format_rep)); 
    } else if (message.type.type_value == MESSAGE_TYPE_SEND_TO_SINK_REQUEST) {
      message.data.send_to_sink_rep.success = false;

      message_send(
          &message,
          reply_destination,
          MESSAGE_TYPE_SEND_TO_SINK_REPLY,
          sizeof(message.data.send_to_sink_rep)); 
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
