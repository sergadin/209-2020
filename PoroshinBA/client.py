import socket


def compare_lowered_prefix(src, prefix):
    if len(src) < len(prefix):
        return False
    return src[:len(prefix)].lower() == prefix.lower()


def compare_lowered_postfix(src, postfix):
    if len(src) < len(postfix):
        return False
    return src[-len(postfix):].lower() == postfix.lower()


def get_word(s):
    return s.split(maxsplit=1)[0]


def query(sock, request, buffsize=1024, flag=0): 
    sock.send((request + ' \7').encode())
    data = ''
    while True:
        data_part = sock.recv(buffsize, flag)
        data_part = data_part.decode()
        data += data_part
        if data_part[-1] == '\7':
            break
    return data[:-1]


addr, port = 'localhost', 55432
use_previous_inp = False

inp = input('Type server addr and port separated by comma.\nFor default ("localhost", 55432) just press Enter\n')
if inp.strip() != '':
    addr, port = inp.split(',')

s = socket.socket()
s.connect((addr, int(port)))
print(query(s, '+'))
while True:     
    inp = input()
    data = query(s, inp.strip())
    if inp in ['disconnect', 'exit']:
        s.close();
        break;
    if data != '':
        print(data)