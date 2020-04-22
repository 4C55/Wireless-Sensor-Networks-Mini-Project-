#ifndef MESSAGE_HANDLER_H_
#define MESSAGE_HANDLER_H_

//------------------------------------------------------INCLUDES------------------------------------------------------//

#include "types.h"
#include "circullar_buffer.h"

//-------------------------------------------------------DEFINES------------------------------------------------------//

#define SIZE_OF_MESSAGE_DATA 249
#define SIZE_OF_EMPTY_MESSAGE 6 //Start + Type + Length + check  sum + End
#define SIZE_OF_EMPTY_DATA 0
#define SIZE_OF_MESSAGE_TYPE 2

//--------------------------------------------------------ENUMS-------------------------------------------------------//

enum message_address {
	MESSAGE_ADDRESS_MOTE = 0,
	MESSAGE_ADDRESS_PC = 1
};

enum message_type_value {
	MESSAGE_TYPE_EMPTY = 0,
	MESSAGE_TYPE_TEST_REQUEST = 1,
	MESSAGE_TYPE_TEST_RESPONSE = 2,
	MESSAGE_TYPE_WRITE_REQUEST = 3,
	MESSAGE_TYPE_WRITE_RESPONE = 4,
	MESSAGE_TYPE_READ_REQUEST = 5,
	MESSAGE_TYPE_READ_RESPONSE = 6,
	MESSAGE_TYPE_FORMAT_REQUEST = 7,
	MESSAGE_TYPE_FORMAT_RESPONSE = 8,
	MESSAGE_SEND_TO_SINK_REQUEST = 10,
	MESSAGE_SEND_TO_SINK_REPLY = 11,
	NUMBER_OF_MESSAGE_TYPES
};

enum message_handler_state {
	MESSAGE_HANDLER_RECEIVING_START = 0,
	MESSAGE_HANDLER_RECEIVING_TYPE_1 = 1,
	MESSAGE_HANDLER_RECEIVING_TYPE_2 = 2,
	MESSAGE_HANDLER_RECEIVING_SOURCE = 3,
	MESSAGE_HANDLER_RECEIVING_DESTINATION = 4,
	MESSAGE_HANDLER_RECEIVING_LENGTH = 5,
	MESSAGE_HANDLER_RECEIVING_DATA = 6,
	MESSAGE_HANDLER_RECEIVING_CRC = 7,
	MESSAGE_HANDLER_RECEIVING_END = 8,
};

//-------------------------------------------------------STRUCTS------------------------------------------------------//

#pragma pack(push, 1)
struct write_request
{
	uint32_t address;
	uint16_t length;
	uint8_t write_data[SIZE_OF_MESSAGE_DATA - 6];
};

struct read_request
{
	uint32_t address;
	uint16_t length;
};

struct read_reply
{
	uint16_t length;
	uint8_t read_data[SIZE_OF_MESSAGE_DATA - 2];
};

struct write_reply
{
	bool_t success;
};

struct format_reply
{
	bool_t success;
};

struct send_to_sink_request
{
	uint32_t length;
};

struct send_to_sink_reply
{
	bool_t success;
};

union message_data {
	uint8_t raw_data[SIZE_OF_MESSAGE_DATA];
	struct write_request write_req;
	struct write_reply write_rep;
	struct read_request read_req;
	struct read_reply read_rep;
	struct format_reply format_rep;
	struct send_to_sink_request send_to_sink_req;
	struct send_to_sink_reply send_to_sink_rep;
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
