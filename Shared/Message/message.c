
/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include "message.h"

/*****************************************************************************/
/* DEFINES                                                                   */
/*****************************************************************************/

#define LOG_MODULE "Mess"
#define LOG_LEVEL LOG_CONF_LEVEL_MESSAGE

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

/*****************************************************************************/
/* PRIVATE FUNCTIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* PUBLIC FUNCTIONS                                                          */
/*****************************************************************************/

void message_init(void)
{
    serial_line_init();
    LOG_DBG("Initialised");
}

void message_handle_line(char *line)
{
    LOG_DBG("%s", line);
}