#ifndef CONNECTION_H_
#define CONNECTION_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include "types.h"
#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "sys/log.h"

/*****************************************************************************/
/* PUBLIC DEFINES                                                            */
/*****************************************************************************/

/*****************************************************************************/
/* PUBLIC ENUMERATIONS                                                       */
/*****************************************************************************/

enum connection_state
{
    CONNECTION_STATE_IDLE,
    CONNECTION_STATE_SENDING,
    CONNECTION_STATE_SENT,
    CONNECTION_STATE_FAILED
};

/*****************************************************************************/
/* PUBLIC STRUCTS                                                            */
/*****************************************************************************/

/*****************************************************************************/
/* PUBLIC VARIABLES                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* PUBLIC FUNCTION DECLARATION                                               */
/*****************************************************************************/

void connection_init(void);

void connection_start_sending(uint8_t *buffer, const uint16_t length);

enum connection_state connection_get_state(void);

#ifdef	__cplusplus
}
#endif

#endif /* CONNECTION_H_ */