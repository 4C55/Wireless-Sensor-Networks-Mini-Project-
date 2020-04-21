import MessageType
import Messages
from MessageHandler import MessageHandler
from Message import Message

def main():
    handler = MessageHandler('COM8')

    data = []
    for i in range(240):
        data.append(i)

    # Format memory
    message = Messages.FormatRequest()
    reply = handler.send_and_wait(message, MessageType.MESSAGE_TYPE_FORMAT_RESPONSE)
    if reply is None or not reply.success:
        return

    # Upload
    for i in range(271):
        message = Messages.WriteRequest(
            address=len(data) * i,
            write_data=data)
        reply = handler.send_and_wait(message, MessageType.MESSAGE_TYPE_WRITE_RESPONSE)
        print(len(data) * i)
        if reply is None or not reply.success:
            return

    # Read
    for i in range(271):
        message = Messages.ReadRequest(
            address=len(data) * i,
            length=len(data))
        reply = handler.send_and_wait(message, MessageType.MESSAGE_TYPE_READ_RESPONSE)
        if reply is None:
            return
        if reply is not None:
            print(len(data) * i)

    handler.dispose()


if __name__ == '__main__':
    main()