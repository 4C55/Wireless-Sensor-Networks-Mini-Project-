#ifndef MESSAGE_H_
#define MESSAGE_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include "types.h"
#include "sys/log.h"
#include "dev/uart1.h"
#include "circullar_buffer.h"
#include "message_handler.h"

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

void message_init(struct message *message_buffer);

void message_process(void);

bool_t message_get(void);

void message_send_message(struct message *message);

void message_send(
    struct message *message,
    const uint8_t destination,
    const uint16_t type,
    const uint8_t length);

#ifdef	__cplusplus
}
#endif

#endif /* MESSAGE_H_ */