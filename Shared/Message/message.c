
/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include "message.h"

/*****************************************************************************/
/* DEFINES                                                                   */
/*****************************************************************************/

#define LOG_MODULE "Mess"
#define LOG_LEVEL LOG_CONF_LEVEL_MESSAGE

#define SIZE_OF_UART_BUFFER sizeof(struct message)
#define SIZE_OF_MESSAGE_DATA_BUFFER (sizeof(struct message))

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
static uint8_t data_in_buffer[SIZE_OF_MESSAGE_DATA_BUFFER];
static struct message_handler_channel channel;

/*****************************************************************************/
/* PRIVATE FUNCTIONS                                                         */
/*****************************************************************************/

static int serial_input_byte(unsigned char c)
{
    if (!circullar_buffer_put(&buffer, &c)) {
        LOG_ERR("UART buffer overrun\n");
    }

    return true;
}

static bool_t has_any_bytes_to_receive(void)
{
    return circullar_buffer_has_any(&buffer);
}

static uint8_t receive_byte(void)
{
    uint8_t data = 0;
    (void)circullar_buffer_get(&buffer, &data);
    return data;
}

bool_t send_single_byte(const uint8_t byte)
{
    putchar(byte);
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

    message_handler_init_channel(
	    &channel,
	    MESSAGE_ADDRESS_MOTE,
	    send_single_byte,
	    has_any_bytes_to_receive,
	    receive_byte,
        data_in_buffer,
        sizeof(data_in_buffer));

    LOG_DBG("Initialised\n");
}

void message_process(void)
{
    message_handler_process(&channel);
}

bool_t message_get(struct message *received)
{
    return message_handler_receive(&channel, received);
}

void message_send_message(struct message *message)
{
    message_handler_send(&channel, message);
}

void message_send(
    struct message *message,
    const uint8_t destination,
    const uint16_t type,
    const uint8_t length)
{
    message->source = MESSAGE_ADDRESS_MOTE;
    message->type.type_value = type;
	message->destination = destination;
	message->length = length;
    message_handler_send(&channel, message);
}
