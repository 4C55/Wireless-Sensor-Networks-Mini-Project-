#include "file.h"
#include "types.h"
#include "contiki.h"
#include "message.h"
#include "leds.h"
#include "Connection/connection.h"

#define SIZE_OF_IMAGE_BUFFER (1 * 1024)

/*Message variables*/
static struct message message;
static uint8_t reply_destination;
static uint8_t image_buffer[SIZE_OF_IMAGE_BUFFER];

/*File upload variables*/
static uint32_t total_to_send;
static uint32_t total_sent = 0;
static uint32_t total_count = 0;
static uint32_t to_send;
static uint32_t sent = 0;
static uint32_t count = 0;

PROCESS(producer_process, "ImageUpload");
AUTOSTART_PROCESSES(&producer_process);

PROCESS_THREAD(producer_process, ev, data)
{
  PROCESS_BEGIN();

  file_init();
  message_init(&message);
  connection_init();
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
      total_to_send = message.data.send_to_sink_req.length;
      total_sent = 0;
      total_count = 0;
      
      while (total_sent < total_to_send) {
        printf("total_sent: %ld; total_to_send: %ld\n", total_sent, total_to_send);
        
        if (total_to_send - total_sent >= SIZE_OF_IMAGE_BUFFER) {
          printf("WHY\n");
          total_count = SIZE_OF_IMAGE_BUFFER;
        } else {
          printf("AND WHY\n");
          total_count = total_to_send - total_sent;
        }

        printf("%ld\n", total_count);

        (void)file_read(total_sent, image_buffer, total_count);
        to_send = total_count;
        sent = 0;
        count = 0;

        while (sent < to_send) {
          if (to_send - sent > SIZE_OF_SINGLE_TRANSMITION) {
            count = SIZE_OF_SINGLE_TRANSMITION;
          } else {
            count = to_send - sent;
          } 

          connection_start_sending(image_buffer + sent, count);
          PROCESS_PAUSE();
          while (connection_get_state() == CONNECTION_STATE_SENDING) {
            PROCESS_PAUSE();
          }

          if (connection_get_state() == CONNECTION_STATE_SENT) {
            sent = sent + count;
          }
        }

        total_sent = total_sent + total_count;
      }

      message.data.send_to_sink_rep.success = true;
      message_send(
          &message,
          reply_destination,
          MESSAGE_TYPE_SEND_TO_SINK_REPLY,
          sizeof(message.data.send_to_sink_rep)); 
    }
  }

  PROCESS_END();
}

/*
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

      while (connection_get_state() == CONNECTION_STATE_SENDING) {
        PROCESS_PAUSE();
      }

      if (connection_get_state() == CONNECTION_STATE_FAILED) {
        //Repeat
        j = j - 100;
      }
    }
  }

  while(1) {
      PROCESS_PAUSE();
  }

  PROCESS_END();
}
*/
