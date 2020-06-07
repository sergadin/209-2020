import socket
import struct

stop_word = '__4to-to__'

inp = input('Enter server adress and port separated by comma\n')
addr, port = inp.split(',')

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((addr.strip(), int(port)))
sock.recv(1, socket.MSG_WAITALL)
print('Ready')
while True:
    inp = input()
    if inp.startswith('delete') and not inp.endswith('end'):
        print('Delete request should ends with \'end\' word')
        continue
    sock.send(struct.pack('!I', len(inp)))
    sended = 0
    while sended < len(inp):
        sended += sock.send(inp[sended:].encode())
        
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


