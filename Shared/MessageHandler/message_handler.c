//------------------------------------------------------INCLUDES------------------------------------------------------//

#include "message_handler.h"

//------------------------------------------------------DEFINES-------------------------------------------------------//

#define MESSAGE_START 0x02
#define MESSAGE_END 0x03

#define MESSAGE_BUFFER_SIZE 5

//------------------------------------------------------STRUCTS-------------------------------------------------------//

//-------------------------------------------------------ENUMS--------------------------------------------------------//

//-------------------------------------------------PRIVATE VARIABLES--------------------------------------------------//

//----------------------------------------------PRIVATE FUNCTION DECLERATIONS-----------------------------------------//

static void reset_message_channel(struct message_handler_channel *channel);
static uint8_t message_get_crc(const struct message *message);
static void massage_handler_put_byte(
	struct message_handler_channel *channel,
	const uint8_t byte);
static bool_t is_in_address_list(
	struct message_handler_channel *channel,
	const uint8_t source_address);

//---------------------------------------------------PRIVATE FUNCTIONS------------------------------------------------//

/**
 * @brief Resets the message handler.
 *
 * @param handler The handler to reset.
 */
static void reset_message_channel(struct message_handler_channel *handler)
{
	handler->size_of_received_data = 0;
	handler->state = MESSAGE_HANDLER_RECEIVING_START;
	handler->padding = false;
}

/**
 * @brief Gets the message CRC.
 *
 * @param message The message to get CRC of.
 * @return The CRC of the message.
 */
static uint8_t message_get_crc(const struct message *message)
{
	uint8_t i;
	uint8_t crc = 0xAA;
	crc ^= message->type.type_bytes[0];
	crc ^= message->type.type_bytes[1];
	crc ^= message->source;
	crc ^= message->destination;
	crc ^= message->length;

	for (i = 0; i < message->length; i++) {
		crc ^= message->data.raw_data[i];
	}

	return crc;
}

/**
 * @brief Checks if the message type is valid.
 *
 * @param type The type to check.
 * @return True if the message type is valid.
 */
static bool_t is_valid_type(const uint16_t type)
{
	return type < NUMBER_OF_MESSAGE_TYPES;
}

static bool_t is_in_address_list(
		struct message_handler_channel *handler,
		const uint8_t address)
{
    return handler->address == address;
}

/**
 * @brief Checks if the message handler is padding the current message bytes.
 *
 * @param handler The message handler to check.
 * @param byte The currently received message.
 * @return True if the message handler is padding the current message bytes, false otherwise.
 */
static bool_t is_padding(struct message_handler_channel *handler, const uint8_t byte)
{
	if (handler->padding) {
		if (byte == MESSAGE_START) {
			handler->padding = false;
			return false;
		} else {
			reset_message_channel(handler);
			return true;
		}
	} else if (byte == MESSAGE_START) {
		handler->padding = true;
		return true;
	}

	return false;
}

/**
 * @brief Sends the byte with padding if needed.
 *
 * @param handler The message handler to send the byte using.
 * @param byte The byte to send.
 */
static void send_with_padding(const struct message_handler_channel *handler, const uint8_t byte)
{
	if (byte == MESSAGE_START) {
		handler->send_byte_call(MESSAGE_START);
	}

	handler->send_byte_call(byte);
}

/**
 * @brief Puts the byte into the message handler.
 *
 * Message handler will attempt to assemble a new message from the received bytes.
 *
 * @param handler The message handler to put the byte into.
 * @param byte The byte to put into the message handler.
 */
static void massage_handler_put_byte(
	struct message_handler_channel *channel,
	const uint8_t byte)
{
	switch (channel->state)
	{
	case MESSAGE_HANDLER_RECEIVING_START: {
		if (byte == MESSAGE_START) {
			channel->state = MESSAGE_HANDLER_RECEIVING_TYPE_1;
		}

		break;
	}
	case MESSAGE_HANDLER_RECEIVING_TYPE_1: {
		if (is_padding(channel, byte)) {
			return;
		}

		channel->received_message->type.type_bytes[0] = byte;
		channel->state = MESSAGE_HANDLER_RECEIVING_TYPE_2;

		break;
	}
	case MESSAGE_HANDLER_RECEIVING_TYPE_2: {
		if (is_padding(channel, byte)) {
			return;
		}

		channel->received_message->type.type_bytes[1] = byte;

		if (is_valid_type(channel->received_message->type.type_value)) {
			channel->state = MESSAGE_HANDLER_RECEIVING_SOURCE;
			return;
		}

		reset_message_channel(channel);

		break;
	}
	case MESSAGE_HANDLER_RECEIVING_SOURCE: {
		if (is_padding(channel, byte)) {
			return;
		}

		channel->received_message->source = byte;

        channel->state = MESSAGE_HANDLER_RECEIVING_DESTINATION;
        return;
	}
	case MESSAGE_HANDLER_RECEIVING_DESTINATION: {
		if (is_padding(channel, byte)) {
			return;
		}

		channel->received_message->destination = byte;
            
		if (is_in_address_list(channel, channel->received_message->destination)) {
			channel->state = MESSAGE_HANDLER_RECEIVING_LENGTH;
			return;
		}

		reset_message_channel(channel);

		break;
	}
	case MESSAGE_HANDLER_RECEIVING_LENGTH: {
		if (is_padding(channel, byte)) {
			return;
		}

		if (byte == 0) {
			channel->received_message->length = 0;
			channel->state = MESSAGE_HANDLER_RECEIVING_CRC;
			return;
		} else if (byte <= SIZE_OF_MESSAGE_DATA) {
			channel->received_message->length = byte;
			channel->state = MESSAGE_HANDLER_RECEIVING_DATA;
			return;
		}

		reset_message_channel(channel);
		break;
	}
	case MESSAGE_HANDLER_RECEIVING_DATA: {
		if (is_padding(channel, byte)) {
			return;
		}

		channel->received_message->data.raw_data[channel->size_of_received_data] = byte;
		channel->size_of_received_data++;

		if (channel->size_of_received_data < channel->received_message->length) {
			return;
		} else {
			channel->state = MESSAGE_HANDLER_RECEIVING_CRC;
			return;
		}

		break;
	}
	case MESSAGE_HANDLER_RECEIVING_CRC: {
		if (is_padding(channel, byte)) {
			return;
		}

		uint8_t crc = message_get_crc(channel->received_message);
		if (crc == byte) {
			channel->state = MESSAGE_HANDLER_RECEIVING_END;
			return;
		}

		reset_message_channel(channel);
		break;
	}
	case MESSAGE_HANDLER_RECEIVING_END: {
		if (byte == MESSAGE_END) {
			channel->has_message = true;
		}

		reset_message_channel(channel);
		break;
	}
	default: {
		reset_message_channel(channel);
		break;
	}
	}
}


//---------------------------------------------------PUBLIC FUNCTIONS-------------------------------------------------//

void message_handler_init_channel(
	struct message_handler_channel *handler,
	uint8_t address,
	bool_t (*send_single_byte)(const uint8_t byte),
	bool_t (*has_any_bytes_to_receive)(void),
	uint8_t (*receive_byte)(void),
	struct message *message_buffer)
{
	handler->address = address;
	handler->send_byte_call = send_single_byte;
	handler->has_any_bytes_to_receive_call = has_any_bytes_to_receive;
	handler->receive_byte_call = receive_byte;
	handler->received_message = message_buffer;
	handler->has_message = false;
	reset_message_channel(handler);
}

void message_handler_process(struct message_handler_channel *channel)
{
	while (true) {
        if (channel->has_any_bytes_to_receive_call()){
			uint8_t received = channel->receive_byte_call();
			massage_handler_put_byte(channel, received);
		} else {
			break;
		}
    }
}

void message_handler_send(struct message_handler_channel *channel, struct message *message)
{
	uint8_t i;
	(void)channel->send_byte_call(MESSAGE_START);
	send_with_padding(channel, message->type.type_bytes[0]);
	send_with_padding(channel, message->type.type_bytes[1]);
	send_with_padding(channel, message->source);
	send_with_padding(channel, message->destination);
	send_with_padding(channel, message->length);

	for (i = 0; i < message->length; i++) {
		send_with_padding(channel, message->data.raw_data[i]);
	}

	send_with_padding(channel, message_get_crc(message));
	(void)channel->send_byte_call(MESSAGE_END);
}

bool_t message_handler_receive(struct message_handler_channel *channel)
{
	if (channel->has_message) {
		channel->has_message = false;
		return true;
	}

	return false;
}
