import MessageType
from MessageHandler import MessageHandler
from Message import Message

def main():
    handler = MessageHandler('COM8')
    message = Message(MessageType.MESSAGE_TYPE_TEST_REQUEST)
    handler.send(message)

    handler.dispose()


if __name__ == '__main__':
    main()