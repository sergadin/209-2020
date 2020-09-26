import socket
import struct

def input_matr(dim):
    matr = []
    print("Input matr elems: ")
    print(dim)
    for i in range(dim[0]*dim[1]):
        correct = False
        while not correct:
            try:
                matr.append(float(input("Input elem: ")))
                correct = True
            except Exception:
                print("Input correct float values")
    return matr

def make_stuct_by_matr(matr):
    packer = struct.Struct('<'+("f "*len(matr))[0:-1])
    packed_data = packer.pack(*matr)
    return packed_data

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(("localhost", 7373))
msg = input("Input command: ")
val = len(msg)

# Send command length
packer = struct.Struct("<I")
packed_data = packer.pack(val)
sock.send(packed_data)

# Send command
packer = struct.Struct(f"{val}s")
packed_data = packer.pack(msg.encode())
sock.send(packed_data)

if (msg == "put"):
    # Input matrix name
    matr_name = input(sock.recv(1024).decode())
    val = len(matr_name)

    # Send matrix name length
    packer = struct.Struct("<I")
    packed_data = packer.pack(val)
    sock.send(packed_data)

    #Send matrix name
    packer = struct.Struct(f"{val}s")
    packed_data = packer.pack(matr_name.encode())
    sock.send(packed_data)

    # Send matrix rows
    matr_rows = int(input(sock.recv(1024).decode()))
    val = matr_rows

    packer = struct.Struct("<I")
    packed_data = packer.pack(val)
    sock.send(packed_data)

    # Send matrix columns
    matr_columns = int(input(sock.recv(1024).decode()))
    val = matr_columns

    packer = struct.Struct("<I")
    packed_data = packer.pack(val)
    sock.send(packed_data)

    # Matr length
    matr_ = input_matr([matr_rows, matr_columns])
    matr_length = len(" ".join(map(str, matr_))) + len(matr_)*2



    packer = struct.Struct("<I")
    packed_data = packer.pack(matr_length)
    sock.send(packed_data)

    sock.send(make_stuct_by_matr(matr_))

if (msg == "send"):
    # Send vect dim
    matr_columns = int(input(sock.recv(1024).decode()))
    val = matr_columns

    packer = struct.Struct("<I")
    packed_data = packer.pack(val)
    sock.send(packed_data)

    #Send vect
    print(sock.recv(1024).decode())
    vect_ = input_matr([1, matr_columns])
    vect_length = len(" ".join(map(str, matr_))) + len(matr_)*2 -(len(matr_)-1)

    packer = struct.Struct("<I")
    packed_data = packer.pack(vect_length)
    sock.send(packed_data)

    sock.send(make_stuct_by_matr(vect_))

if (msg == "mult"):
    # Input matrix name
    matr_name = input(sock.recv(1024).decode())
    val = len(matr_name)

    # Send matrix name length
    packer = struct.Struct("<I")
    packed_data = packer.pack(val)
    sock.send(packed_data)

    #Send matrix name
    packer = struct.Struct(f"{val}s")
    packed_data = packer.pack(matr_name.encode())
    sock.send(packed_data)




sock.close()

