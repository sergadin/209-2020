import socket

TCP_IP = '192.168.0.108'
TCP_PORT = 64298
BUFFER_SIZE = 2048

def input_vector(dim):
    vector = []
    print("input vector elements:")
    print(dim)
    for i in range(dim[0]):
        correct = False
        while not correct:
            try:
                vector.append(float(input("Input element:")))
                correct = True
            except Exception:
                print("Vvedite chto-to normalnoe")
    return vector


connection_socket = socket.socket(socket.AF_INET , socket.SOCK_STREAM)
connection_socket.connect((TCP_IP , TCP_PORT))
while 1 > 0 :
    data = connection_socket.recv(BUFFER_SIZE)
    print ("received data:" , data)
    Magiya = [int(x) for x in data[0:(len(data)-2)].split()]
    print(Magiya)
    temp = 0
    i = 0
    while i < 37 :
        temp = temp + Magiya[i]
        i = i + 1
    print(temp)
    if temp == Magiya[37] :
        MESSAGE = str(3.145)
    else :
        MESSAGE = "-1"
    connection_socket.send(MESSAGE.encode())

connection_socket.close()
