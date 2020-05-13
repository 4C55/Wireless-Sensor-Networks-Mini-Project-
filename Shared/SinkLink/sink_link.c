
/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include "sink_link.h"

/*****************************************************************************/
/* DEFINES                                                                   */
/*****************************************************************************/

#define LOG_MODULE "Connection"
#define LOG_LEVEL LOG_CONF_LEVEL_CONNECTION

/*****************************************************************************/
/* PRIVATE ENUMERATIONS                                                      */
/*****************************************************************************/

enum packet_result
{
  PACKET_RESULT_NONE,
  PACKET_RESULT_START,
  PACKET_RESULT_OK,
  PACKET_RESULT_FAILED,
  PACKET_RESULT_IN_PROGRESS
};

/*****************************************************************************/
/* PRIVATE STRUCTS                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* PRIVATE FUNCTION DECLARATION                                              */
/*****************************************************************************/

/*****************************************************************************/
/* PRIVATE VARIABLES                                                         */
/*****************************************************************************/

static enum packet_result result;
static uint32_t sent;
static uint32_t to_send;
static linkaddr_t sink_address =  {{ 0xf7, 0xc2, 0x6e, 0x14, 0x00, 0x74, 0x12, 0x00 }};

/*****************************************************************************/
/* PRIVATE FUNCTIONS                                                         */
/*****************************************************************************/

static uint32_t get_next_packet_size(void)
{
  if (to_send - sent >= SIZE_OF_SINGLE_TRANSMITION) {
    return SIZE_OF_SINGLE_TRANSMITION;
  } else {
    return to_send - sent;
  }
}

static void send_callback(void *ptr, int status, int transmissions)
{
    if (status == MAC_TX_OK) {
      result = PACKET_RESULT_OK;
      LOG_DBG("Successfull transmition with flag %d\n", status);
    } else {
      result = PACKET_RESULT_FAILED;
      LOG_DBG("Failed transmition with flag %d\n", status);
    }
}

/*****************************************************************************/
/* PUBLIC FUNCTIONS                                                          */
/*****************************************************************************/

void sink_link_init(void)
{
   result = PACKET_RESULT_NONE;
}

void sink_link_start_sending(uint8_t *buffer, const uint32_t length)
{
    nullnet_buf = buffer;
    to_send = length;
    sent = 0;
    result = PACKET_RESULT_START;
}

enum sink_link_state sink_link_continue_sending(void)
{
    switch (result)
    {
      case PACKET_RESULT_NONE:
      {
        return CONNECTION_STATE_IDLE;
      }
      case PACKET_RESULT_START:
      {
        /*Start sending:*/
        NETSTACK_MAC.on();
        nullnet_len = get_next_packet_size();
        result = PACKET_RESULT_IN_PROGRESS;
        NETSTACK_NETWORK.output(&sink_address, send_callback);
        return CONNECTION_STATE_SENDING;
      }
      case PACKET_RESULT_IN_PROGRESS:
      {
        return CONNECTION_STATE_SENDING;
      }
      case PACKET_RESULT_OK:
      {
        /*Possibly finished sending a packet*/
        sent = sent + nullnet_len;
        nullnet_buf = nullnet_buf + nullnet_len;
        nullnet_len = get_next_packet_size();

        if (nullnet_len == 0) {
          /*Done sending all packets*/
          NETSTACK_MAC.off();
          result = PACKET_RESULT_NONE;
          return CONNECTION_STATE_IDLE;
        }

        result = PACKET_RESULT_IN_PROGRESS;
        NETSTACK_NETWORK.output(&sink_address, send_callback);
        return CONNECTION_STATE_SENDING;
      }
      case PACKET_RESULT_FAILED:
      {
        NETSTACK_NETWORK.output(&sink_address, send_callback);
        return CONNECTION_STATE_SENDING;
      }
      default:
      {
        result = PACKET_RESULT_NONE;
        return CONNECTION_STATE_IDLE;
      }
    }
}
