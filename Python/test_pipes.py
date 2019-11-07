import os

fifo_name = 'fifo'

def main():
    data = b'blob\n\x00 123'
    try:
        os.mkfifo(fifo_name)
    except FileExistsError:
        pass
    with open(fifo_name, 'wb') as f:
        # b for binary mode
        f.write('{}\n'.format(len(data)).encode())
        f.write(data)

if __name__ == '__main__':
    main()