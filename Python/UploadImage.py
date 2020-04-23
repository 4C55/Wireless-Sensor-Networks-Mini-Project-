import sys
from Message import MessageType
from Message import Messages
from Message.MessageHandler import MessageHandler
from Message.Message import Message

MAX_NUMBER_OF_BYTES_TO_SEND = 240

def main(argv):
    file = argv[0]
    port = argv[1]

    # Setup what's needed
    handler = MessageHandler(port, show_input=True)
    file = open(file)
    image = file.read()
    file.close()

    # Extract bytes
    image = image.replace('\n', ',').strip(',').split(',')
    image = [int(i) for i in image]
    total_bytes = len(image)
    print("Found %d bytes to upload" % total_bytes)

    # Format memory
    print('Formatting memory...', end=' ')
    message = Messages.FormatRequest()
    reply = handler.send_and_wait(message, MessageType.MESSAGE_TYPE_FORMAT_RESPONSE)
    if reply is None or not reply.success:
        print("Failed!")
        return
    else:
        print('Success!')

    # Upload image
    print('Writing image file...', end=' ')
    bytes_sent = 0
    printed = []
    while bytes_sent < total_bytes:
        if total_bytes - bytes_sent >= MAX_NUMBER_OF_BYTES_TO_SEND:
            data = image[bytes_sent: bytes_sent + MAX_NUMBER_OF_BYTES_TO_SEND]
        else:
            data = image[bytes_sent:]
        message = Messages.WriteRequest(
            address=bytes_sent,
            write_data=data)
        reply = handler.send_and_wait(message, MessageType.MESSAGE_TYPE_WRITE_RESPONSE)
        if reply is None or not reply.success:
            print("Failed!")
            return
        bytes_sent = bytes_sent + len(data)
        completed = int((bytes_sent / total_bytes) * 100)
        if completed % 10 == 0 and completed not in printed:
            print('%d%%' % (completed,), end=' ')
            printed.append(completed)
    print("Success!")

    # Check image
    print('Verifying image file...', end=' ')
    bytes_read = 0
    printed = []
    while bytes_read < total_bytes:
        if total_bytes - bytes_read >= MAX_NUMBER_OF_BYTES_TO_SEND:
            bytes_to_read = MAX_NUMBER_OF_BYTES_TO_SEND
        else:
            bytes_to_read = total_bytes - bytes_read
        message = Messages.ReadRequest(
            address=bytes_read,
            length=bytes_to_read)
        reply = handler.send_and_wait(message, MessageType.MESSAGE_TYPE_READ_RESPONSE)
        if reply is None or reply.length == 0:
            print('Failed!')
            return

        received = reply.read_data
        expected = image[bytes_read: bytes_read + len(received)]
        for i in range(len(received)):
            if expected[i] != received[i]:
                print('Memory did not match. Failed!')
                return
        bytes_read = bytes_read + len(received)

        completed = int((bytes_read / total_bytes) * 100)
        if completed % 10 == 0 and completed not in printed:
            print('%d%%' % (completed, ), end=' ')
            printed.append(completed)

    print('Success!')
    print('Done')


if __name__ == '__main__':
    main(sys.argv[1:])