from Message import MessageType
from Message import MessageAddress

SIZE_OF_MESSAGE_DATA = 249

class Message:
    def __init__(
            self,
            message_type=MessageType.MESSAGE_TYPE_EMPTY,
            source=MessageAddress.MESSAGE_ADDRESS_PC,
            destination=MessageAddress.MESSAGE_ADDRESS_MOTE,
            data=[],
            attempts=3,
            timeout=3):
        self.message_type = message_type
        self.source = source
        self.destination = destination
        self.data = data
        self.attempts = attempts
        self.timeout = timeout




