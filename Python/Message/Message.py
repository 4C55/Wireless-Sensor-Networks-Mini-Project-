import MessageType
import MessageAddress

SIZE_OF_MESSAGE_DATA = 32
MESSAGE_START = 0x02
MESSAGE_END = 0x03


def add_message_byte(bytes, new_byte):
    bytes.append(new_byte)


def add_message_byte_with_padding(bytes, new_byte):
    if new_byte == MESSAGE_START:
        bytes.append(MESSAGE_START)
    bytes.append(new_byte)


class Message:
    def __init__(
            self,
            message_type=MessageType.MESSAGE_TYPE_EMPTY,
            source=MessageAddress.MESSAGE_ADDRESS_PC,
            destination=MessageAddress.MESSAGE_ADDRESS_MOTE,
            data=[]):
        self.message_type = message_type
        self.source = source
        self.destination = destination
        self.data = data

    def get_bytes(self):
        output = []
        add_message_byte(output, MESSAGE_START)
        add_message_byte_with_padding(output, self.message_type & 0xff)
        add_message_byte_with_padding(output, (self.message_type >> 8) & 0xff)
        add_message_byte_with_padding(output, self.source)
        add_message_byte_with_padding(output, self.destination)
        add_message_byte_with_padding(output, len(self.data))
        for data in self.data:
            add_message_byte_with_padding(output, data)

        crc = 0xAA
        for i in range(1, len(output)):
            crc = crc ^ output[i]
        add_message_byte_with_padding(output, crc)
        add_message_byte(output, MESSAGE_END)
        return output




