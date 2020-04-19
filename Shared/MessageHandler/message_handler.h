#ifndef MESSAGE_HANDLER_H_
#define MESSAGE_HANDLER_H_

//------------------------------------------------------INCLUDES------------------------------------------------------//

#include "types.h"
#include "circullar_buffer.h"

//-------------------------------------------------------DEFINES------------------------------------------------------//

#define SIZE_OF_MESSAGE_DATA 32
#define SIZE_OF_EMPTY_MESSAGE 6 //Start + Type + Length + check  sum + End
#define SIZE_OF_EMPTY_DATA 0
#define SIZE_OF_MESSAGE_TYPE 2

//--------------------------------------------------------ENUMS-------------------------------------------------------//

enum message_address {
	MESSAGE_ADDRESS_MOTE,
	MESSAGE_ADDRESS_PC
};

enum message_type_value {
	MESSAGE_TYPE_EMPTY = 0,
	MESSAGE_TYPE_TEST_REQUEST = 1,
	MESSAGE_TYPE_TEST_RESPONSE = 2,
	NUMBER_OF_MESSAGE_TYPES
};

enum message_handler_state {
	MESSAGE_HANDLER_RECEIVING_START,
	MESSAGE_HANDLER_RECEIVING_TYPE_1,
	MESSAGE_HANDLER_RECEIVING_TYPE_2,
	MESSAGE_HANDLER_RECEIVING_SOURCE,
	MESSAGE_HANDLER_RECEIVING_DESTINATION,
	MESSAGE_HANDLER_RECEIVING_LENGTH,
	MESSAGE_HANDLER_RECEIVING_DATA,
	MESSAGE_HANDLER_RECEIVING_CRC,
	MESSAGE_HANDLER_RECEIVING_END,
};

//-------------------------------------------------------STRUCTS------------------------------------------------------//

#pragma pack(push, 1)
union message_data {
	uint8_t raw_data[SIZE_OF_MESSAGE_DATA];
};

union message_type {
	uint8_t type_bytes[SIZE_OF_MESSAGE_TYPE];
	enum message_type_value type_value;
};

struct message {
	union message_type type;
	uint8_t source;
	uint8_t destination;
	uint8_t length;
	union message_data data;
};
#pragma pack(pop)

struct message_handler_channel {
    uint8_t address;
	bool_t (*send_byte_call)(const uint8_t value);
	bool_t (*has_any_bytes_to_receive_call)(void);
	uint8_t (*receive_byte_call)(void);
	struct message received_message;
	uint8_t size_of_received_data;
	enum message_handler_state state;
	bool_t padding;
    struct circullar_buffer incomming_messages;
    struct circullar_buffer outgoing_messages;
};

//---------------------------------------------------PUBLIC FUNCTIONS-------------------------------------------------//

void message_handler_init_channel(
	struct message_handler_channel *handler,
	uint8_t address,
	bool_t (*send_single_byte)(const uint8_t byte),
	bool_t (*has_any_bytes_to_receive)(void),
	uint8_t (*receive_byte)(void),
    uint8_t *data_in_buffer,
    uint8_t data_in_buffer_size,
    uint8_t *data_out_buffer,
    uint8_t data_out_buffer_size);

void message_handler_process(struct message_handler_channel *channel);

void message_handler_send(struct message_handler_channel *channel, struct message *message);

bool_t message_handler_receive(struct message_handler_channel *channel, struct message *message);

#endif /* MESSAGE_HANDLER_H_ */
