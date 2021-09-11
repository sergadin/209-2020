import socket
import struct
import sys

int_struct = struct.Struct('<i')
intsize = 4
input_codenames = ['ERRC_OK','ERRC_BADISTREAM','ERRC_BADDATA','ERRC_SHUTDOWN',
             'ERRC_UNKNOWN']

output_codenames = {'Q_STANDARD':0, 'Q_SHUTDOWN':1, 'Q_CLEAR':2}

HELP_MESSAGE = '\nPossible commands:\n\
"send_matrix" - sends matrix A ([mxn]) to server;\n\
if there is at least one matrix B ([kxm]), then\n\
it returns list of all products B*A;\n\
else A is added to database.\n\n\
"shutdown" - closes server and this client.\n\n\
"cleardbs" - clears the database completely.\n\n\
"exit" - closes this client.\n\n\
"help" - shows this message.\n'


#
def ReadInts(num=1, caption='', err_msg="something's wrong; try again."):
    success = False
    res = []
    while not success:
        print(caption, end='')
        inp = map(int,input().split())
        try:
            res = list(inp)
            if len(res) != num:
                raise Exception('wrong len')
            else:
                success = True
        except Exception as e:
            print(err_msg)
        finally:
            if success:
                return res
    

def SendStandardCommand(sock):
    n, m = 0, 0
    while n <= 0 or m <= 0:
        print('(n, m should be positive)')
        n, m = ReadInts(2, "Enter n, m:", "It's not two integers. Try again.")
   
    print('Enter matrix (n rows, m columns):')
    mat = []
    for i in range(n):
        row = ReadInts(m,
                       'Enter row of {} integers ({}/{}):'.format(m, i+1, n),
                       'These are not {} integers; try again.'.format(m))    
        mat.append(row)

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

try:
    sock.connect(server_address)
except ConnectionRefusedError as cre:
    print('Server is now off.')
    sys.exit()

while True:
    try:
        print('Enter your command (variants: "send_matrix", "shutdown", "cleardbs", "exit", "help")', end=':')
        s = input()
        if s not in SendUniversalCommand:
            if s == 'exit':
                print('Goodbye.')
                break
            if s == 'help':
                print(HELP_MESSAGE)
                continue
            print('Unknown command. Try again.')
            continue
        
        SendUniversalCommand[s](sock)
        
        print('waiting for answer...')
        future_len = GetInt(sock)
        err_code = GetInt(sock)

        if input_codenames[err_code] == 'ERRC_OK':
            matrices_n = GetInt(sock)
            if matrices_n != 0:
                print('recieved {} matrices, namely:'.format(matrices_n))
                for i in range(matrices_n):
                    mat = GetMatrix(sock)
                    strend = '.' if i == matrices_n-1 else ',';

                    print(*mat, sep='\n',end=strend+'\n\n')
            else:
                print("command successfully executed")
        elif input_codenames[err_code] == 'ERRC_SHUTDOWN':
            print('Server is shut down.')
            print('Closing client.')
            break
        else:
            print("not everything's ok: err_code = {}".format(input_codenames[err_code]))
            error_message = GetStr(sock, future_len - intsize)
            print('message: "{}"'.format(error_message))
    except IOError as ioe:
        print("Server is closed by someone else.")
        break
    except Exception as e:
        print("Client stopped because of unknown exception. Details:", e)
        break

sock.close()
