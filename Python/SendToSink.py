import sys
from Message import MessageType
from Message import Messages
from Message.MessageHandler import MessageHandler
from Message.Message import Message


def main(argv):
    port = argv[0]
    compression_type = int(argv[1])

    # Setup what's needed
    handler = MessageHandler(port, show_input=True)

    message = Messages.SendToSinkRequest(length=256*256, compression_type=compression_type)
    reply = handler.send_and_wait(message, MessageType.MESSAGE_TYPE_SEND_TO_SINK_REPLY)
    
    if reply is None:
        print('Failed')
    else:
        print('Sent %d' % (reply.sent_length, ), end=' ')
        
        if reply.sent_length == 0:
            print('Failed')
        else:
            print('Success')

    print('Done')

if __name__ == '__main__':
    main(sys.argv[1:])