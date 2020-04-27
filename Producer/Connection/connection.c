
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

static enum connection_state state;
static linkaddr_t sink_address =  {{ 0xf7, 0xc2, 0x6e, 0x14, 0x00, 0x74, 0x12, 0x00 }};

/*****************************************************************************/
/* PRIVATE FUNCTIONS                                                         */
/*****************************************************************************/

void send_callback(void *ptr, int status, int transmissions)
{
    if (status == MAC_TX_OK) {
      state = CONNECTION_STATE_SENT;
      LOG_DBG("Successfull transmition with flag %d\n", status);
    } else {
      state = CONNECTION_STATE_FAILED;
      LOG_DBG("Failed transmition with flag %d\n", status);
    }
}

/*****************************************************************************/
/* PUBLIC FUNCTIONS                                                          */
/*****************************************************************************/

void connection_init(void)
{
   state = CONNECTION_STATE_IDLE;
}

void connection_start_sending(uint8_t *buffer, const uint16_t length)
{
    nullnet_buf = buffer;
    nullnet_len = length;
    state = CONNECTION_STATE_SENDING;
    NETSTACK_NETWORK.output(&sink_address, send_callback);
}

enum connection_state connection_get_state(void)
{
    return state;
}
