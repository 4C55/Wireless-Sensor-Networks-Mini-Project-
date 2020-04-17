#ifndef CONNECTION_H_
#define CONNECTION_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include "Types/types.h"
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

bool_t connection_has_finished_sending(void);

#ifdef	__cplusplus
}
#endif

#endif /* CONNECTION_H_ */