
/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include "message.h"

/*****************************************************************************/
/* DEFINES                                                                   */
/*****************************************************************************/

#define LOG_MODULE "Mess"
#define LOG_LEVEL LOG_CONF_LEVEL_MESSAGE

#define SIZE_OF_UART_BUFFER 50

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

static struct circullar_buffer buffer;
static uint8_t data_buffer[SIZE_OF_UART_BUFFER];

/*****************************************************************************/
/* PRIVATE FUNCTIONS                                                         */
/*****************************************************************************/

static int serial_input_byte(unsigned char c)
{
    (void)circullar_buffer_put(&buffer, &c);
    return true;
}

/*****************************************************************************/
/* PUBLIC FUNCTIONS                                                          */
/*****************************************************************************/

void message_init(void)
{
    uart1_set_input(serial_input_byte);
    circullar_buffer_init(
	    &buffer,
        data_buffer,
	    SIZE_OF_UART_BUFFER,
	    1);

    LOG_DBG("Initialised");
}

void message_process(void)
{
    char data;

    while (circullar_buffer_get(&buffer, &data)) {
        printf("%c", data);
    }
}
