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

packer = struct.Struct("<I")
packed_data = packer.pack(val)
sock.send(packed_data)

packer = struct.Struct(f"{val}s")
packed_data = packer.pack(msg.encode())
sock.send(packed_data)

    matr_name = input(sock.recv(1024).decode())
    val = len(matr_name)

    packer = struct.Struct("<I")
    packed_data = packer.pack(val)
    sock.send(packed_data)

    packer = struct.Struct(f"{val}s")
    packed_data = packer.pack(matr_name.encode())
    sock.send(packed_data)

    matr_rows = int(input(sock.recv(1024).decode()))
    val = matr_rows

    packer = struct.Struct("<I")
    packed_data = packer.pack(val)
    sock.send(packed_data)
   
    matr_columns = int(input(sock.recv(1024).decode()))
    val = matr_columns

    packer = struct.Struct("<I")
    packed_data = packer.pack(val)
    sock.send(packed_data)
    
    matr_ = input_matr([matr_rows, matr_columns])
    matr_length = len(" ".join(map(str, matr_))) + len(matr_)*2


    packer = struct.Struct("<I")
    packed_data = packer.pack(matr_length)
    sock.send(packed_data)

    sock.send(make_stuct_by_matr(matr_))
