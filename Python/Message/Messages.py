import MessageType
import MessageAddress
from MessageData import MessageDataBuilder
from MessageData import MessageDataReader
from Message import Message


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
            address=0):
        if data is None:
            builder = MessageDataBuilder()

            self.address = address
            builder.add_uint32_t(self.address)

            message_data = builder.get_data()
        else:
            reader = MessageDataReader(data)

            self.address = reader.get_uint32_t()

            message_data = data
        super(WriteRequest, self).__init__(
            message_type=MessageType.MESSAGE_TYPE_WRITE_REQUEST,
            source=source,
            destination=destination,
            data=message_data)


class WriteResponse(Message):
    def __init__(
            self,
            source=MessageAddress.MESSAGE_ADDRESS_PC,
            destination=MessageAddress.MESSAGE_ADDRESS_MOTE,
            data=None):
        super(WriteResponse, self).__init__(
            message_type=MessageType.MESSAGE_TYPE_WRITE_RESPONSE,
            source=source,
            destination=destination,
            data=data)