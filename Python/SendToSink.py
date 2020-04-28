import sys
from Message import MessageType
from Message import Messages
from Message.MessageHandler import MessageHandler
from Message.Message import Message


def main(argv):
    port = argv[0]

    # Setup what's needed
    handler = MessageHandler(port, show_input=True)

    message = Messages.SendToSinkRequest(length=256*256)
    reply = handler.send_and_wait(message, MessageType.MESSAGE_TYPE_SEND_TO_SINK_REPLY)
    print(reply)

if __name__ == '__main__':
    main(sys.argv[1:])