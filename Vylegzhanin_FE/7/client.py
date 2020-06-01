import socket
import struct

int_struct = struct.Struct('<I')
intsize = 4
input_codenames = ['ERRC_OK','ERRC_BADISTREAM','ERRC_BADDATA','ERRC_SHUTDOWN',
             'ERRC_UNKNOWN']

output_codenames = {'Q_STANDARD':0, 'Q_SHUTDOWN':1, 'Q_CLEAR':2}

def SendStandardCommand(sock):
    print('Enter n, m', end=':')
    n,m = map(int,input().split())
    if(n < 0 or m < 0):
        print('bad size. try again.')
        return
    print('Enter matrix (n rows, m columns)', end=':')
    mat = []
    for i in range(n):
        row = list(map(int, input().split()))
        if(len(row) != m):
            print('bad row size. try again.')
            return
        mat.append(row)
    print('you entered matrix:')
    print(mat)
    print('sendind additional data...',end='')
    SendInt(sock, intsize*(m*n+3))
    SendInt(sock, output_codenames['Q_STANDARD'])
    print('sent.')
    SendMatrix(sock, n, m, mat)
    
def SendShutdownCommand(sock):
    SendInt(sock, intsize)
    SendInt(sock, output_codenames['Q_SHUTDOWN'])
    
def SendClearCommand(sock):
    SendInt(sock, intsize)
    SnedInt(sock, output_codenames['Q_CLEAR'])

    
command_switcher = {
    'standard': SendStandardCommand,
    'shutdown': SendShutdownCommand,
    'cleardbs': SendClearCommand
}

def SendMatrix(sock, n, m, mat):
    print('sending matrix {}x{}...'.format(n,m),end='')
    SendInt(sock, n)
    SendInt(sock, m)
    for i in range(n):
        for j in range(m):
            SendInt(sock, mat[i][j])
    print('sent.')
    


def ComposeArrPacker(length, prefix, typical):
    s = prefix + (typical+ ' ')*length
    return struct.Struct(s[:-1])

def ComposeIntArrPacker(length):
    return GetArrPacker(length, '<', 'I')

def SendData(sock, data, packer):
    sock.sendall(packer.pack(*data))

def SendSingleVar(sock, var, packer):
    sock.sendall(packer.pack(var))

def SendInt(sock, var):
    sock.sendall(int_struct.pack(var))

def GetData(sock, length, unpacker):
    msg = sock.recv(length, socket.MSG_WAITALL)
    return unpacker.unpack(msg)

def GetSingleVar(sock, length, unpacker):
    return GetData(sock, length, unpacker)[0]

def GetInt(sock):
    return GetSingleVar(sock, intsize, int_struct)

def GetStr(sock, length):
    return sock.recv(length, socket.MSG_WAITALL).decode('ascii')

def GetMatrix(sock):
    n,m = GetInt(sock), GetInt(sock)
    mat = []
    for i in range(n):
        mat.append([GetInt(sock) for j in range(m)])
    return mat


# Создаем TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('localhost', 1237)
sock.connect(server_address)

while True:
    try:
        print('Enter your command (variants: "standard", "shutdown", "clear", "exit")', end=':')
        s = input()
        print('entered command "{}"'.format(s))
        if s not in command_switcher:
            if s == 'exit':
                print('Goodbye')
                break
            print('Unknown command; try again')
            continue
        command_switcher[s](sock)
        print('waiting for answer...')
        future_len = GetInt(sock)
        print('future_len={}'.format(future_len))
        err_code = GetInt(sock)
        print('err_code={}'.format(input_codenames[err_code]))
        if input_codenames[err_code] == 'ERRC_OK':
            matrices_n = GetInt(sock)
            print('matrices_n={}'.format(matrices_n))
            for i in range(matrices_n):
                mat = GetMatrix(sock)
                print('recieved matrix:')
                print(mat)
        else:
            print("it's not ok")
            error_message = GetStr(sock, future_len-intsize)
            print('message: "{}"'.format(error_message))
    except Exception as e:
        print('Exception!', e)
    

'''
values = (12,1,1,5)
packer = struct.Struct('<I I I I')

try:
    # Отправка
    SendData(sock, values, packer)

    # Получение длины
    future_len = GetInt(sock)
    print('future_len={}'.format(future_len))
    err_code = GetInt(sock)
    print('err_code={}'.format(codenames[err_code]))
    if codenames[err_code] == 'ERRC_OK':
        matrices_n = GetInt(sock)
        print('matrices_n={}'.format(matrices_n))
        for i in range(matrices_n):
            mat = GetMatrix(sock)
            print(mat)
    else:
        print("it's not ok")
        
except Exception as e:
    print('Exception!', e)
finally:
    # закрываем сокет
    sock.close()
'''
