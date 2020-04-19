import MessageType
import Messages
from MessageHandler import MessageHandler
from Message import Message

def main():
    handler = MessageHandler('COM8')
    message = Messages.WriteRequest(address=10000000)
    reply = handler.send(message, MessageType.MESSAGE_TYPE_WRITE_RESPONSE)
    print(reply)
    handler.dispose()


if __name__ == '__main__':
    main()