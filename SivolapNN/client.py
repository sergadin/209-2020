import socket

stop_word = '__4to-to__'

def chunkstring(string, length):
    return (string[0+i:length+i] for i in range(0, len(string), length))

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
    for substr in chunkstring(inp+' ', 256):
        sock.send(substr.encode())
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

