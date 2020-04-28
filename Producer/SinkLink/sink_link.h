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

#define SIZE_OF_SINGLE_TRANSMITION 104 //104

/*****************************************************************************/
/* PUBLIC ENUMERATIONS                                                       */
/*****************************************************************************/

enum sink_link_state
{
    CONNECTION_STATE_IDLE,
    CONNECTION_STATE_SENDING,
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

void sink_link_init(void);

void sink_link_start_sending(uint8_t *buffer, const uint32_t length);

enum sink_link_state sink_link_continue_sending(void);

#ifdef	__cplusplus
}
#endif

#endif /* CONNECTION_H_ */