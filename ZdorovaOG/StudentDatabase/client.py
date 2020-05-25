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
    def __init__(self, addr='localhost', port=8080, timeout=10, buffsize=256):
        self.port = port
        self.addr = addr
        self.timeout = timeout
        self.buffsize = buffsize
        self.id = self.raw_query('new_user')

    def __del__(self):
        self.raw_query(f'close_db\n{self.id}')

    def read(self, s):
        try:
            data = s.recv(self.buffsize, 0)
            return data
        except (ConnectionResetError, ConnectionAbortedError, BlockingIOError, AttributeError, OSError) as ex:
            print(ex)
            return b''

    def raw_query(self, request):
        with socket.socket() as s:
            s.connect((self.addr, self.port))
            s.send(request.encode())
            data = b''
            s.setblocking(0)
            s.settimeout(self.timeout)
            while True:
                data_part = self.read(s)
                if data_part == b'':
                    break
                data += data_part
            return data.decode()

    def check(self, request):
        req_type = get_word(request)
        if req_type in ['help', 'add', 'delete']:
            return True
        return compare_lowered_postfix(request.rstrip(), 'end')

    def query(self, request):
        if self.check(request):
            return self.raw_query(f'request\n{self.id}\n{request}')
        else:
            print('Request should ends with "end"')
            return ''


addr, port = 'localhost', 8080
use_previous_inp = False

inp = input('Type server addr and port separated by comma.\nFor default ("localhost", 8080) just press Enter\n')
if inp.strip() != '':
    addr, port = inp.split(',')

cli = DataBaseClient(addr=addr, port=int(port))

while True:
    if not use_previous_inp:
        inp = input()
    use_previous_inp = False

    if compare_lowered_prefix(inp, 'exit'):
        break

    data = cli.query(inp.strip())
    if data != '':
        if compare_lowered_prefix(data, 'help:'):
            print(data)
        elif compare_lowered_prefix(data, 'error:'):
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
