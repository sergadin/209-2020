import socket

stop_word = '__4to-to__'

inp = input('Enter server adress and port separated by comma\n')
addr, port = inp.split(',')

sock = socket.socket()
sock.connect((addr.strip(), int(port)))
print('Ready')
while True:
    inp = input()
    if inp.startswith('delete') and not inp.endswith('end'):
        print('Delete request should ends with \'end\' word')
        continue
    sock.send((inp + ' ').encode())
    if inp in ['disconnect', 'SHUTDOWN']:
        sock.close()
        break
    data = ''
    while True:
        data_part = sock.recv(256)
        data += data_part.decode()
        if data.endswith(stop_word):
            break
    print(data[:-len(stop_word)])
