import MessageType
import MessageAddress

SIZE_OF_MESSAGE_DATA = 32


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





