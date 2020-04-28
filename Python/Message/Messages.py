from Message import MessageType
from Message import MessageAddress
from Message.MessageData import MessageDataBuilder
from Message.MessageData import MessageDataReader
from Message.Message import Message


class TestRequest(Message):
    def __init__(
            self,
            source=MessageAddress.MESSAGE_ADDRESS_PC,
            destination=MessageAddress.MESSAGE_ADDRESS_MOTE,
            data=[]):
        super(TestRequest, self).__init__(
            message_type=MessageType.MESSAGE_TYPE_TEST_REQUEST,
            source=source,
            destination=destination,
            data=data)


class TestResponse(Message):
    def __init__(
            self,
            source=MessageAddress.MESSAGE_ADDRESS_PC,
            destination=MessageAddress.MESSAGE_ADDRESS_MOTE,
            data=[]):
        super(TestResponse, self).__init__(
            message_type=MessageType.MESSAGE_TYPE_TEST_RESPONSE,
            source=source,
            destination=destination,
            data=data)


class WriteRequest(Message):
    def __init__(
            self,
            source=MessageAddress.MESSAGE_ADDRESS_PC,
            destination=MessageAddress.MESSAGE_ADDRESS_MOTE,
            data=None,
            address=0,
            write_data=None):
        if data is None:
            builder = MessageDataBuilder()

            self.address = address
            builder.add_uint32_t(self.address)

            self.length = len(write_data)
            builder.add_uint16_t(self.length)

            self.write_data = write_data
            builder.add_uint8_t_array(self.write_data)

            message_data = builder.get_data()
        else:
            reader = MessageDataReader(data)

            self.address = reader.get_uint32_t()
            self.length = reader.get_uint16_t()
            self.write_data = reader.get_uint8_t_array(self.length)

            message_data = data
        super(WriteRequest, self).__init__(
            message_type=MessageType.MESSAGE_TYPE_WRITE_REQUEST,
            source=source,
            destination=destination,
            data=message_data,
            timeout=15,
            attempts=2)


class WriteResponse(Message):
    def __init__(
            self,
            source=MessageAddress.MESSAGE_ADDRESS_PC,
            destination=MessageAddress.MESSAGE_ADDRESS_MOTE,
            data=None,
            success=True):
        if data is None:
            builder = MessageDataBuilder()

            self.success = success
            builder.add_bool_t(self.success)

            message_data = builder.get_data()
        else:
            reader = MessageDataReader(data)

            self.success = reader.get_bool_t()

            message_data = data
        super(WriteResponse, self).__init__(
            message_type=MessageType.MESSAGE_TYPE_WRITE_RESPONSE,
            source=source,
            destination=destination,
            data=message_data)


class ReadRequest(Message):
    def __init__(
            self,
            source=MessageAddress.MESSAGE_ADDRESS_PC,
            destination=MessageAddress.MESSAGE_ADDRESS_MOTE,
            data=None,
            address=0,
            length=0):
        if data is None:
            builder = MessageDataBuilder()

            self.address = address
            builder.add_uint32_t(self.address)

            self.length = length
            builder.add_uint16_t(length)

            message_data = builder.get_data()
        else:
            reader = MessageDataReader(data)

            self.address = reader.get_uint32_t()
            self.length = reader.get_uint16_t()

            message_data = data
        super(ReadRequest, self).__init__(
            message_type=MessageType.MESSAGE_TYPE_READ_REQUEST,
            source=source,
            destination=destination,
            data=message_data)


class ReadResponse(Message):
    def __init__(
            self,
            source=MessageAddress.MESSAGE_ADDRESS_PC,
            destination=MessageAddress.MESSAGE_ADDRESS_MOTE,
            data=None,
            length=0,
            read_data=[]):
        if data is None:
            builder = MessageDataBuilder()

            self.length = length
            builder.add_uint16_t(length)

            self.read_data = read_data
            builder.add_uint8_t_array(self.read_data)

            message_data = builder.get_data()
        else:
            reader = MessageDataReader(data)

            self.length = reader.get_uint16_t()
            self.read_data = reader.get_uint8_t_array(self.length)

            message_data = data
        super(ReadResponse, self).__init__(
            message_type=MessageType.MESSAGE_TYPE_READ_RESPONSE,
            source=source,
            destination=destination,
            data=message_data)


class FormatRequest(Message):
    def __init__(
            self,
            source=MessageAddress.MESSAGE_ADDRESS_PC,
            destination=MessageAddress.MESSAGE_ADDRESS_MOTE,
            data=[]):
        super(FormatRequest, self).__init__(
            message_type=MessageType.MESSAGE_TYPE_FORMAT_REQUEST,
            source=source,
            destination=destination,
            data=data,
            timeout=20,
            attempts=2)


class FormatResponse(Message):
    def __init__(
            self,
            source=MessageAddress.MESSAGE_ADDRESS_PC,
            destination=MessageAddress.MESSAGE_ADDRESS_MOTE,
            data=[],
            success=True):
        if data is None:
            builder = MessageDataBuilder()

            self.success = success
            builder.add_bool_t(success)

            message_data = builder.get_data()
        else:
            reader = MessageDataReader(data)

            self.success = reader.get_bool_t()

            message_data = data
        super(FormatResponse, self).__init__(
            message_type=MessageType.MESSAGE_TYPE_FORMAT_RESPONSE,
            source=source,
            destination=destination,
            data=message_data)


class SendToSinkRequest(Message):
    def __init__(
            self,
            source=MessageAddress.MESSAGE_ADDRESS_PC,
            destination=MessageAddress.MESSAGE_ADDRESS_MOTE,
            data=None,
            length=0):
        if data is None:
            builder = MessageDataBuilder()

            self.length = length
            builder.add_uint32_t(length)

            message_data = builder.get_data()
        else:
            reader = MessageDataReader(data)

            self.length = reader.get_uint32_t()

            message_data = data
        super(SendToSinkRequest, self).__init__(
            message_type=MessageType.MESSAGE_TYPE_SEND_TO_SINK_REQUEST,
            source=source,
            destination=destination,
            data=message_data,
            attempts=2,
            timeout=60)


class SendToSinkReply(Message):
    def __init__(
            self,
            source=MessageAddress.MESSAGE_ADDRESS_PC,
            destination=MessageAddress.MESSAGE_ADDRESS_MOTE,
            data=None,
            success=0):
        if data is None:
            builder = MessageDataBuilder()

            self.success = success
            builder.add_bool_t(success)

            message_data = builder.get_data()
        else:
            reader = MessageDataReader(data)

            self.success = reader.get_bool_t()

            message_data = data
        super(SendToSinkReply, self).__init__(
            message_type=MessageType.MESSAGE_TYPE_SEND_TO_SINK_REPLY,
            source=source,
            destination=destination,
            data=message_data)