import socket

def matr_to_str(matr):
    arr = []
    arr.append(str(len(matr)))
    arr.append(str(len(matr[0])))
    for i in matr:
        for j in i:
            arr.append(str(j))
    return " ".join(arr)

def str_to_matr(s):
    s = s.split(" ")
    matr = []
    rows_c = int(s[0])
    cols_c = int(s[1])
    for i in range(rows_c):
        matr.append([])
    
    curr_row = -1
    for i in range(rows_c * cols_c):
        if i%cols_c == 0:
            curr_row += 1
        
        matr[curr_row].append(float(s[i+2]))
    return matr

def get_dim():
    dim = [0, 0]
    while dim[0]<1 or dim[1]<1:
            try:
                dim = list(map(int, input("Input matr dimension like N M: ").split(" ")))
                if len(dim) != 2:
                    dim = [0, 0]
                    print("Input correct integer values")
            except Exception:
                dim = [0, 0]
                print("Input correct integer values")
    return dim

def input_matr(dim):
    matr = []
    print("Input matr elems: ")
    for i in range(dim[0]):
        matr.append([])
        for j in range(dim[1]):
            correct = False
            while not correct:
                try:
                    matr[i].append(float(input("Input elem {} {}: ".format(i ,j))))
                    correct = True
                except Exception:
                    print("Input correct float values")
    return matr


try:
    work = True
    while work:
        dim1 = get_dim()
        matr1 = input_matr(dim1)
        
        dim2 = get_dim()
        matr2 = input_matr(dim2)
        
        correct = True
        if dim1[1] != dim2[0]:
            correct = False
        matr_s = matr_to_str(matr1) + " " + matr_to_str(matr2)
        
        if correct:
            sock = socket.socket()
            try:
                sock.connect(('localhost', 27015))
                sock.send(matr_s.encode())
                data = sock.recv(1024)
                sock.close()
                print("\nMult matrix:")
                print(str_to_matr(data.decode()))
            except ConnectionError:
                print("Server is unavailable now =(")
        else:
            print("Matrix dims wrong =(")
    
        print("Enter 'F' for exit or 'enter' for continue: ")
        if input() == 'F':
            work = False
    print("Bye!")
            
            
except KeyboardInterrupt:
    print("CUL")