import sys
import os
from pathlib import Path
from PIL import Image
import numpy as np
from Message.MessageHandler import MessageHandler
from Message import CompressionType
from Compression import RunlLength
from Compression import Scale

STATE_IDLE = 0
STATE_RECEIVING_IMAGE = 1


def save_info(file, received_len, reconstructed_len):
    with open(file, 'w+') as f:
        f.write('"SEP=;"\n')
        f.write('recived_len;reconstructed_len\n')
        f.write('%d;%d\n' % (received_len, received_len))


def save_received_bytes(file, received_bytes):
    with open(file, 'w+') as f:
        strings = [str(i) for i in received_bytes]
        f.write(', '.join(strings))
        f.write('\n')


def save_reconstructed_bytes(file, reconstructed_bytes, columns=256):
    index = 0
    total = len(reconstructed_bytes)
    with open(file, 'w+') as f:
        while index < total:
            if index + columns > total:
                next = total
            else:
                next = index + columns
            row = reconstructed_bytes[index:next]
            row = [str(i) for i in row]
            f.write(', '.join(row))
            f.write('\n')
            index = next


def save_reconstructed_image(file, reconstructed_bytes, columns=256):
    data = np.asarray(reconstructed_bytes)
    data = np.reshape(reconstructed_bytes, (int(len(reconstructed_bytes) / columns), columns))
    img = Image.fromarray(np.uint8(data), 'L')
    img.save(file)


def handle_image(parent_path, received_data, compression_type):
    if compression_type == CompressionType.COMPRESSION_TYPE_NONE:
        type = 'COMPRESSION_TYPE_NONE'
        reconstructed_data = received_data
    elif compression_type == CompressionType.COMPRESSION_TYPE_SCALE:
        type = 'COMPRESSION_TYPE_SCALE'
        reconstructed_data = Scale.reconstruct(received_data, 256)
    elif compression_type == CompressionType.COMPRESSION_TYPE_RUN_LENGTH:
        type = 'COMPRESSION_TYPE_RUN_LENGTH'
        reconstructed_data = RunlLength.reconstruct(received_data)
    else:
        print('Unknown compression type!')
        return

    path = os.path.join(parent_path, type)
    Path(path).mkdir(parents=True, exist_ok=True)
    received_bytes_file = os.path.join(path, 'received.txt')
    reconstructed_bytes_file = os.path.join(path, 'reconstructed.txt')
    info_file = os.path.join(path, 'info.csv')
    image_file = os.path.join(path, 'reconstructed.png')

    save_info(info_file, len(received_data), len(reconstructed_data))
    save_received_bytes(received_bytes_file, received_data)
    save_reconstructed_bytes(reconstructed_bytes_file, reconstructed_data)
    save_reconstructed_image(image_file, reconstructed_data)
    print('%s; Received size: %d; Reconstructed size %d' % (type, len(received_data), len(reconstructed_data)))


def main(argv):
    path = argv[0]
    port = argv[1]

    # Setup what's needed
    handler = MessageHandler(port)
    state = STATE_IDLE
    print('Sink started')


    received_data = []
    compression_type = CompressionType.COMPRESSION_TYPE_NONE
    while True:
        received_data.append(handler.receive_byte())

        if state == STATE_IDLE:
            if len(received_data) >= 3:
                if received_data[0] == 0xff and received_data[1] == 0xff:
                    compression_type = received_data[2]
                    received_data = []
                    state = STATE_RECEIVING_IMAGE
                    print('Receiving image...', end=' ', flush=True)
        if state == STATE_RECEIVING_IMAGE:
            if len(received_data) > 2:
                if received_data[-1] == 0xff and received_data[-2] == 0xff and received_data[-3] == 0xff:
                    received_data.pop(-1)
                    received_data.pop(-1)
                    received_data.pop(-1)
                    print('Image received:', end=' ', flush=True)
                    handle_image(path, received_data, compression_type)
                    state = STATE_IDLE
                    received_data = []


if __name__ == '__main__':
    main(sys.argv[1:])