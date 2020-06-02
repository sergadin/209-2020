import socket
import struct

int_struct = struct.Struct('<i')
intsize = 4
input_codenames = ['ERRC_OK','ERRC_BADISTREAM','ERRC_BADDATA','ERRC_SHUTDOWN',
             'ERRC_UNKNOWN']

output_codenames = {'Q_STANDARD':0, 'Q_SHUTDOWN':1, 'Q_CLEAR':2}

def SendStandardCommand(sock):
    print('Enter n, m', end=':')
    n,m = map(int,input().split())
    while n < 0 or m < 0:
        print('bad size. try typing it again.')
        print('Enter n, m', end=':')
        n,m = map(int,input().split())       
        
    print('Enter matrix (n rows, m columns)', end=':')
    mat = []
    for i in range(n):
        row = list(map(int, input().split()))
        while(len(row) != m):
            print('bad row size. try typing it again.')
            row = list(map(int, input().split()))    
        mat.append(row)
#    print('you entered matrix:')
#    print(mat)
#    print('sendind additional data...',end='')
    SendInt(sock, intsize*(m*n+3))
    SendInt(sock, output_codenames['Q_STANDARD'])
#    print('sent.')
    SendMatrix(sock, n, m, mat)
    print('data sent.')
          
def SendShutdownCommand(sock):
    SendInt(sock, intsize)
    SendInt(sock, output_codenames['Q_SHUTDOWN'])
    
def SendClearCommand(sock):
    SendInt(sock, intsize)
    SendInt(sock, output_codenames['Q_CLEAR'])

    
SendUniversalCommand = {
    'send_matrix': SendStandardCommand,
    'shutdown': SendShutdownCommand,
    'cleardbs': SendClearCommand
}

def SendMatrix(sock, n, m, mat):
#    print('sending matrix {}x{}...'.format(n,m),end='')
#    print('(mat={})'.format(mat))
    SendInt(sock, n)
    SendInt(sock, m)
    for i in range(n):
        for j in range(m):
            SendInt(sock, mat[i][j])
#    print('sent.')

def SendInt(sock, var):
#    print('sending int {}'.format(var))
    sock.sendall(int_struct.pack(var))

def GetInt(sock):
    return int_struct.unpack(
        sock.recv(intsize, socket.MSG_WAITALL)
        )[0]

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
        print('Enter your command (variants: "send_matrix", "shutdown", "cleardbs", "exit")', end=':')
        s = input()
        print('entered command "{}"'.format(s))
        if s not in SendUniversalCommand:
            if s == 'exit':
                print('Goodbye')
                break
            print('Unknown command; try again')
            continue
        
        SendUniversalCommand[s](sock)
        
        print('waiting for answer...')
        future_len = GetInt(sock)
  #      print('future_len={}'.format(future_len))
        err_code = GetInt(sock)

        if input_codenames[err_code] == 'ERRC_OK':
            matrices_n = GetInt(sock)
            if matrices_n != 0:
                print('recieved {} matrices, namely:'.format(matrices_n))
                for i in range(matrices_n):
                    mat = GetMatrix(sock)
    #                print('recieved matrix:')
                    print(*mat, sep='\n')
            else:
                print("everything's ok")
        elif input_codenames[err_code] == 'ERRC_SHUTDOWN':
            print('server is shutting down. Goodnight')
            break
        else:
            print("not everything's ok: err_code = {}".format(input_codenames[err_code]))
            error_message = GetStr(sock, future_len - intsize)
            print('message: "{}"'.format(error_message))
    except Exception as e:
        print("Unknown exception! Details:", e)
        print("Client stopped")
        break

sock.close()
