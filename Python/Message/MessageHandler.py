import serial
import Message
from datetime import datetime
import MessageHandlerState
import MessageType
import MessageAddress
import Messages

class MessageHandler:
    def __init__(
            self,
            port):
        self.port = serial.Serial()
        self.port.baudrate = 115200
        self.port.port = port
        self.port.timeout = 0.01
        self.port.open()

        self.parsing_state = MessageHandlerState.MESSAGE_HANDLER_RECEIVING_START
        self.padding = False
        self.message_type = 0
        self.source = 0
        self.destination = 0
        self.length = 0
        self.data = []

    def is_padding(self, byte):
        if self.padding:
            if byte ==  Message.MESSAGE_START:
                self.padding = False
                return False
            else:
                self.reset_message_parsing_state()
                return True
        elif byte == Message.MESSAGE_START:
            self.padding = True
            return True

    def reset_message_parsing_state(self):
        self.parsing_state = MessageHandlerState.MESSAGE_HANDLER_RECEIVING_START
        self.padding = False
        self.message_type = 0
        self.source = 0
        self.destination = 0
        self.length = 0
        self.data = []

    def get_current_crc(self):
        crc = 0xAA
        crc = crc ^ (self.message_type & 0x00FF)
        crc = crc ^ (self.message_type & 0xFF00)
        crc = crc ^ self.source
        crc = crc ^ self.destination
        crc = crc ^ self.length
        for data in self.data:
            crc = crc ^ data
        return crc

    def parse_byte(self, byte):
        byte = int.from_bytes(byte, byteorder='little')

        # MESSAGE_HANDLER_RECEIVING_START
        if self.parsing_state == MessageHandlerState.MESSAGE_HANDLER_RECEIVING_START:
            if byte == Message.MESSAGE_START:
                self.parsing_state = MessageHandlerState.MESSAGE_HANDLER_RECEIVING_TYPE_1
                return None
        # MESSAGE_HANDLER_RECEIVING_TYPE_1
        elif self.parsing_state == MessageHandlerState.MESSAGE_HANDLER_RECEIVING_TYPE_1:
            if self.is_padding(byte):
                return None
            self.message_type = byte
            self.parsing_state = MessageHandlerState.MESSAGE_HANDLER_RECEIVING_TYPE_2
            return None
        # MESSAGE_HANDLER_RECEIVING_TYPE_2
        elif self.parsing_state == MessageHandlerState.MESSAGE_HANDLER_RECEIVING_TYPE_2:
            if self.is_padding(byte):
                return None
            self.message_type = (byte << 8) | self.message_type
            self.parsing_state = MessageHandlerState.MESSAGE_HANDLER_RECEIVING_SOURCE
            return None
        # MESSAGE_HANDLER_RECEIVING_SOURCE
        elif self.parsing_state == MessageHandlerState.MESSAGE_HANDLER_RECEIVING_SOURCE:
            if self.is_padding(byte):
                return None
            self.source = byte
            self.parsing_state = MessageHandlerState.MESSAGE_HANDLER_RECEIVING_DESTINATION
            return None
        # MESSAGE_HANDLER_RECEIVING_DESTINATION
        elif self.parsing_state == MessageHandlerState.MESSAGE_HANDLER_RECEIVING_DESTINATION:
            if self.is_padding(byte):
                return None
            self.destination = byte
            self.parsing_state = MessageHandlerState.MESSAGE_HANDLER_RECEIVING_LENGTH
            return None
        # MESSAGE_HANDLER_RECEIVING_LENGTH
        elif self.parsing_state == MessageHandlerState.MESSAGE_HANDLER_RECEIVING_LENGTH:
            if self.is_padding(byte):
                return None

            self.length = byte

            if self.length == 0:
                self.parsing_state = MessageHandlerState.MESSAGE_HANDLER_RECEIVING_CRC
            elif self.length <= Message.SIZE_OF_MESSAGE_DATA:
                self.parsing_state = MessageHandlerState.MESSAGE_HANDLER_RECEIVING_DATA
            return None
        # MESSAGE_HANDLER_RECEIVING_DATA
        elif self.parsing_state == MessageHandlerState.MESSAGE_HANDLER_RECEIVING_DATA:
            if self.is_padding(byte):
                return None
            self.data.append(byte)
            if len(self.data) == self.length:
                self.parsing_state = MessageHandlerState.MESSAGE_HANDLER_RECEIVING_CRC
            return None
        # MESSAGE_HANDLER_RECEIVING_CRC
        elif self.parsing_state == MessageHandlerState.MESSAGE_HANDLER_RECEIVING_CRC:
            if self.is_padding(byte):
                return None
            if byte == self.get_current_crc():
                self.parsing_state = MessageHandlerState.MESSAGE_HANDLER_RECEIVING_END
            else:
                self.reset_message_parsing_state()
            return None
        # MESSAGE_HANDLER_RECEIVING_END
        elif self.parsing_state == MessageHandlerState.MESSAGE_HANDLER_RECEIVING_END:
            output = None
            if byte == Message.MESSAGE_END:
                output = self.handle_valid_message()

            self.reset_message_parsing_state()
            return output

    def handle_valid_message(self):
        if self.message_type == MessageType.MESSAGE_TYPE_TEST_REQUEST:
            return Messages.TestRequest(
                source=self.source,
                destination=self.destination,
                data=self.data)
        elif self.message_type == MessageType.MESSAGE_TYPE_TEST_RESPONSE:
            return Messages.TestResponse(
                source=self.source,
                destination=self.destination,
                data=self.data)
        elif self.message_type == MessageType.MESSAGE_TYPE_WRITE_RESPONSE:
            return Messages.WriteResponse(
                source=self.source,
                destination=self.destination,
                data=self.data)
        else:
            return None


    def send(self,
             message,
             expected_response,
             expected_source=MessageAddress.MESSAGE_ADDRESS_MOTE,
             expected_destination=MessageAddress.MESSAGE_ADDRESS_PC,
             timeout_s=3):
        self.reset_message_parsing_state()
        self.port.write(message.get_bytes())
        self.port.flush()

        start = datetime.now()
        response = None
        while (datetime.now() - start).total_seconds() < timeout_s:
            received_byte = self.port.read(size=1)
            if len(received_byte) == 1:
                response = self.parse_byte(received_byte)
                if response is not None:
                    break

        if (
                response is not None and
                response.message_type == expected_response and
                response.source == expected_source and
                response.destination == expected_destination):
            return response
        else:
            return None




    def dispose(self):
        self.port.close()
