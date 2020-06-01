import socket
import struct
import time
 
def getStrFromServer(sock,a):
    f = str(a)+'s'
    packer = struct.Struct(f)
    data = sock.recv(a)
    return data.decode('utf-8')
def getIntFromServer(sock):
    unpacker = struct.Struct('i')
    data = sock.recv(unpacker.size,socket.MSG_WAITALL)
    int_val = unpacker.unpack(data)
    return int_val[0]
def sendStrToServer(sock,z):
    k = len(z)
    f = str(k)+'s'
    packer = struct.Struct('i')
    packed_data = packer.pack(k)
    sock.sendall(packed_data)
    packer = struct.Struct(f)
    packed_data = packer.pack(z.encode('utf-8'))
    sock.sendall(packed_data)
def mesegeFromServer(sock):
    a=getIntFromServer(sock)
    if(a>1):
        print("\033[34m mesege from server:")
        print("\033[30m")
        for i in range(a):
            time.sleep(0.01)
            b=getIntFromServer(sock);
            print(getStrFromServer(sock,b))
    else:
        b=getIntFromServer(sock);
        print("\033[34m mesege from server>","\033[30m", getStrFromServer(sock,b))
   
 
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('192.168.100.4', 5555) #37.45.88.83:5555
sock.connect(server_address)
print("\033[32m Welcome to our app!")
print("\033[32m write your request",end='>')
while(1):
    print("\033[30m")
    z = input()
    sendStrToServer(sock,z)
    if z=="stop end":
        break
    if z=="exit1979 end":
        break
    mesegeFromServer(sock)
    print("\033[32m write your request",end='>')
sock.close()
