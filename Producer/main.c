/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include "file.h"
#include "types.h"
#include "contiki.h"
#include "leds.h"
#include "sink_link.h"
#include "pc_link.h"
#include "compression.h"


/*****************************************************************************/
/* DEFINES                                                                   */
/*****************************************************************************/

#define SIZE_OF_IMAGE_BUFFER (4 * 1024)

/*****************************************************************************/
/* PRIVATE ENUMERATIONS                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* PRIVATE STRUCTS                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* PRIVATE FUNCTION DECLARATION                                              */
/*****************************************************************************/

/*****************************************************************************/
/* PRIVATE VARIABLES                                                         */
/*****************************************************************************/

/*Message variables*/
static struct message message;
static uint8_t reply_destination;
static uint8_t image_buffer[SIZE_OF_IMAGE_BUFFER];

/*File upload variables*/
static uint32_t total_to_send;
static uint32_t total_sent = 0;
static uint32_t total_count = 0;

/*****************************************************************************/
/* PRIVATE FUNCTIONS                                                         */
/*****************************************************************************/

static uint32_t get_next_buffer_lenght(void)
{
  if (total_to_send - total_sent >= SIZE_OF_IMAGE_BUFFER) {
    return SIZE_OF_IMAGE_BUFFER;
  } else {
    return total_to_send - total_sent;
  }
}

static void start_sending_start_code(enum compression_type compression)
{
  image_buffer[0] = 0xff;
  image_buffer[1] = 0xff;
  image_buffer[2] = compression;
  sink_link_start_sending(image_buffer, 3);
}

static void start_sending_end_code()
{
  image_buffer[0] = 0xff;
  image_buffer[1] = 0xff;
  image_buffer[2] = 0xff;
  sink_link_start_sending(image_buffer, 3);
}

static uint32_t apply_compression(
  uint8_t buffer[],
  const uint32_t original_length,
  enum compression_type type)
{
  switch (type)
  {
    case COMPRESSION_TYPE_RUN_LENGTH:
    {
      return compression_runlength(buffer, original_length);
    }
    case COMPRESSION_TYPE_SCALE:
    {
      return compression_scale(buffer, original_length, 256);
    }
    default:
    {
      return original_length;
    }
  }
}

/*****************************************************************************/
/* PROCESS FUNCTIONS                                                         */
/*****************************************************************************/

PROCESS(producer_process, "Producer");
AUTOSTART_PROCESSES(&producer_process);

PROCESS_THREAD(producer_process, ev, data)
{
  PROCESS_BEGIN();

  file_init();
  sink_link_init();
  pc_link_init(&message);
  P2DIR |= ((1 << 3) | (1 << 6));
  P2OUT &= ~((1 << 3) | (1 << 6));
  
  PROCESS_PAUSE();


  while(1) {
    PROCESS_PAUSE();
    pc_link_process();
    PROCESS_PAUSE();
    
    if (!pc_link_get()) {
      continue;
    }

    reply_destination = message.source;

    if (message.type.type_value == MESSAGE_TYPE_TEST_REQUEST) {
      pc_link_send(
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

      pc_link_send(
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

      pc_link_send(
          &message,
          reply_destination,
          MESSAGE_TYPE_READ_RESPONSE,
          sizeof(message.data.read_rep)); 
    } else if (message.type.type_value == MESSAGE_TYPE_FORMAT_REQUEST) {
      message.data.format_rep.success = file_format_memory();

      PROCESS_PAUSE();

      pc_link_send(
          &message,
          reply_destination,
          MESSAGE_TYPE_FORMAT_RESPONSE,
          sizeof(message.data.format_rep)); 
    } else if (message.type.type_value == MESSAGE_TYPE_SEND_TO_SINK_REQUEST) {
      P2OUT |= (1 << 3);
      total_to_send = message.data.send_to_sink_req.length;
      total_sent = 0;
      total_count = 0;

      /*Send start code*/
      start_sending_start_code(message.data.send_to_sink_req.compression_type);
      while (sink_link_continue_sending() == CONNECTION_STATE_SENDING) {
        PROCESS_PAUSE();
      }     

      while (total_sent < total_to_send) {       
        total_count = get_next_buffer_lenght();

        /*Read chunk of the image*/
        /*TBD - watchout for read failures*/
        (void)file_read(total_sent, image_buffer, total_count);
        PROCESS_PAUSE();

        /*Compress the chunk*/
        P2OUT |= (1 << 6);
        uint32_t compressed_length = apply_compression(
          image_buffer,
          total_count,
          message.data.send_to_sink_req.compression_type
        );
        P2OUT &= ~(1 << 6);

        /*Send compressed chunk*/
        /*TBD - watch out for transmission failures*/
        sink_link_start_sending(image_buffer, compressed_length);
        while (sink_link_continue_sending() == CONNECTION_STATE_SENDING) {
          PROCESS_PAUSE();
        }

        total_sent = total_sent + total_count;        
      }

      /*Send end code*/
      start_sending_end_code();
      while (sink_link_continue_sending() == CONNECTION_STATE_SENDING) {
        PROCESS_PAUSE();
      }

      message.data.send_to_sink_rep.sent_length = total_sent;
      pc_link_send(
          &message,
          reply_destination,
          MESSAGE_TYPE_SEND_TO_SINK_REPLY,
          sizeof(message.data.send_to_sink_rep));

      P2OUT &= ~(1 << 3);
    }
  }

  PROCESS_END();
}