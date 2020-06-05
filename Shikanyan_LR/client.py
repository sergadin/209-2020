  # Socket client example in python

import socket
import sys
import struct

host = '127.0.0.1'
port = 5555  # web

#select subject=Math end print end reselect group=320-* end print end reselect teacher=Shikanyan end print end
def getStrFromServer(sock,a):
    data = sock.recv(a)
    #data.decode('utf-8')
    return data.decode('utf-8')
def getIntFromServer(sock):
    unpacker = struct.Struct('i')
    data = sock.recv(unpacker.size,socket.MSG_WAITALL)
    int_val = unpacker.unpack(data)
    return int_val[0]

# create socket
print('# Creating socket')
try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error:
    print('Failed to create socket')
    sys.exit()

print('# Getting remote IP address')
try:
    remote_ip = socket.gethostbyname( host )
except socket.gaierror:
    print('Hostname could not be resolved. Exiting')
    sys.exit()

# Connect to remote server
print('# Connecting to server, ' + host + ' (' + remote_ip + ')')
s.connect((remote_ip , port))

while 1:
    print('Введите команду:')
    request = input()
    l = len(request)
    packer = struct.Struct('i')
    packed_data = packer.pack(l)
    try:
        s.sendall(packed_data)
        s.sendall(request.encode('utf-8'))
    except socket.error:
        print ('Send failed')
        sys.exit()
    if (request == "stop"):
        break
    a = getIntFromServer(s)
    print("Сервер ответил:")
    for i in range(a):
        b = getIntFromServer(s)
        for j in range(b):
            c = getIntFromServer(s)
            print(getStrFromServer(s, c+1))
s.close()
