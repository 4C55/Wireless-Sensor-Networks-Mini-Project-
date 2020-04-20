import MessageType
import Messages
from MessageHandler import MessageHandler
from Message import Message

def main():
    handler = MessageHandler('COM8')
    data = []
    data.append(170)

    message = Messages.WriteRequest(
        address=0,
        length=1,
        write_data=data)
    reply = handler.send(message, MessageType.MESSAGE_TYPE_WRITE_RESPONSE)
    print(reply.success)
    handler.dispose()


if __name__ == '__main__':
    main()