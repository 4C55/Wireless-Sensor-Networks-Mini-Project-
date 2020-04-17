
/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include "Connection/connection.h"

/*****************************************************************************/
/* DEFINES                                                                   */
/*****************************************************************************/

#define LOG_MODULE "Connection"
#define LOG_LEVEL LOG_CONF_LEVEL_CONNECTION

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

static bool_t sending;
static linkaddr_t sink_address =  {{ 0xf7, 0xc2, 0x6e, 0x14, 0x00, 0x74, 0x12, 0x00 }};

/*****************************************************************************/
/* PRIVATE FUNCTIONS                                                         */
/*****************************************************************************/

void send_callback(void *ptr, int status, int transmissions)
{
    sending = false;
    LOG_DBG("Send callback with flag %d\n", status);

  //MAC_TX_OK,

  /**< The MAC layer transmission could not be performed due to a
     collision. */
  //MAC_TX_COLLISION,

  /**< The MAC layer did not get an acknowledgement for the packet. */
  //MAC_TX_NOACK,

  /**< The MAC layer deferred the transmission for a later time. */
  //MAC_TX_DEFERRED,

  /**< The MAC layer transmission could not be performed because of an
     error. The upper layer may try again later. */
  //MAC_TX_ERR,

  /**< The MAC layer transmission could not be performed because of a
     fatal error. The upper layer does not need to try again, as the
     error will be fatal then as well. */
  //MAC_TX_ERR_FATAL,
}

/*
void input_callback(const void *data, uint16_t len,
  const linkaddr_t *src, const linkaddr_t *dest)
{
  if(len == sizeof(unsigned)) {
    unsigned count;
    memcpy(&count, data, sizeof(count));
    LOG_INFO("Received %u from ", count);
    LOG_INFO_LLADDR(src);
    LOG_INFO_("\n");
  }
}
*/

/*****************************************************************************/
/* PUBLIC FUNCTIONS                                                          */
/*****************************************************************************/

void connection_init(void)
{
   //nullnet_set_input_callback(input_callback);
}

void connection_start_sending(uint8_t *buffer, const uint16_t length)
{
    nullnet_buf = buffer;
    nullnet_len = length;
    sending = true;
    NETSTACK_NETWORK.output(&sink_address, send_callback);
}

bool_t connection_has_finished_sending(void)
{
    return !sending;
}
