import MessageType
import Messages
from MessageHandler import MessageHandler
from Message import Message

def main():
    handler = MessageHandler('COM8')

    # message = Messages.TestRequest()
    # reply = handler.send_and_wait(message, MessageType.MESSAGE_TYPE_TEST_RESPONSE)
    # print(reply)

    # data = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    # message = Messages.WriteRequest(
    #     address=0,
    #     write_data=data)
    # reply = handler.send_and_wait(message, MessageType.MESSAGE_TYPE_WRITE_RESPONSE)
    # print(reply)

    message = Messages.ReadRequest(
        address=5,
        length=5)
    reply = handler.send_and_wait(message, MessageType.MESSAGE_TYPE_READ_RESPONSE)
    print(reply)
    print(reply.read_data)


    handler.dispose()


if __name__ == '__main__':
    main()