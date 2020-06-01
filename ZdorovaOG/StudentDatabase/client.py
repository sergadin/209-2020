import socket
import pandas
from io import StringIO


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


class DataBaseClient:
    def __init__(self, addr='localhost', port=8989, timeout=10, buffsize=4096):
        self.port = port
        self.addr = addr
        self.timeout = timeout
        self.buffsize = buffsize
        self.sock = socket.socket()
        self.connected = False
        print('Try to connect')
        self.sock.connect((self.addr, self.port))
        is_pinged = self.query('Xo4y_3a4eT', 2, socket.MSG_WAITALL, True)
        if is_pinged != '+':
            raise Exception('Server abort connection. Relaunch program')
        print('Session started')
        self.connected = True

    def __del__(self):
        if self.connected:
            self.query('disconnect')
        self.sock.close()

    def check(self, request):
        req_type = get_word(request).lower()
        if req_type in ['help', 'disconnect', 'delete', 'exit', 'save']:
            return True
        return compare_lowered_postfix(request.rstrip(), 'end')

    def query(self, request, buffsize=None, flag=0, skip_check=False):
        if skip_check or self.check(request):
            self.sock.send((request + ' \7').encode())
            data = ''
            while True:
                data_part = self.sock.recv(self.buffsize if buffsize is None else buffsize, flag)
                data_part = data_part.decode()
                data += data_part
                if data_part[-1] == '\7':
                    break
            return data[:-1]
        else:
            print('Request should ends with "end"')
            return ''


addr, port = 'localhost', 8989
use_previous_inp = False

inp = input('Type server addr and port separated by comma.\nFor default ("localhost", 8989) just press Enter\n')
if inp.strip() != '':
    addr, port = inp.split(',')

cli = DataBaseClient(addr=addr, port=int(port))

while True:
    if not use_previous_inp:
        inp = input()
    use_previous_inp = False

    data = cli.query(inp.strip())
    if data != '':
        if compare_lowered_prefix(data, 'help:'):
            print(data)
        elif compare_lowered_prefix(data, 'error:'):
            print(data)
        elif data == 'disconnected':
            cli.connected = False
            break
        elif data == "Done":
            print(data)
        else:
            df = pandas.read_csv(StringIO(data))
            print(df)
            print('For full view type: full')
            inp = input()
            if compare_lowered_prefix(inp, 'full'):
                with pandas.option_context('display.max_rows', None, 'display.max_columns', None):
                    print(df)
            else:
                use_previous_inp = True
                continue
