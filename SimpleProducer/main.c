/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include "file.h"
#include "types.h"
#include "contiki.h"
#include "leds.h"
#include "sink_link.h"
#include "compression.h"
#include "dev/button-sensor.h"

/*****************************************************************************/
/* DEFINES                                                                   */
/*****************************************************************************/

#define SIZE_OF_IMAGE_BUFFER (4 * 1024)
#define COMPRESSION_TYPE COMPRESSION_TYPE_SCALE

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
static uint8_t image_buffer[SIZE_OF_IMAGE_BUFFER];

/*File upload variables*/
static uint32_t total_to_send;
static uint32_t total_sent = 0;
static uint32_t total_count = 0;
static uint32_t compressed_length;

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

PROCESS(producer_process, "SimpleProducer");
AUTOSTART_PROCESSES(&producer_process);

PROCESS_THREAD(producer_process, ev, data)
{
  PROCESS_BEGIN();
  
  SENSORS_ACTIVATE(button_sensor);
  file_init();
  sink_link_init();
  P2DIR |= ((1 << 3) | (1 << 6));
  P2OUT &= ~((1 << 3) | (1 << 6));

  while(1) {
    PROCESS_PAUSE();
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);

    P2OUT |= (1 << 3);
    total_to_send = 65536;
    total_sent = 0;
    total_count = 0;

    /*Send start code*/
    start_sending_start_code(COMPRESSION_TYPE);
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
      compressed_length = apply_compression(
        image_buffer,
        total_count,
        COMPRESSION_TYPE
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
    P2OUT &= ~(1 << 3);
  }

  PROCESS_END();
}