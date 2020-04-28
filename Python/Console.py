import sys
from Message import MessageType
from Message import Messages
from Message.MessageHandler import MessageHandler
from Message.Message import Message


def main(argv):
    port = argv[0]

    # Setup what's needed
    handler = MessageHandler(port, show_input=True)

    print('Console started')

    while True:
        handler.receive_byte()


if __name__ == '__main__':
    main(sys.argv[1:])