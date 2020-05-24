import socket
import pandas
from io import StringIO


class DataBaseClient:
    def __init__(self, addr='localhost', port=8080, timeout=1):
        self.port = port
        self.addr = addr
        self.timeout = timeout
        self.id = self.raw_query('new_user')

    def __del__(self):
        self.raw_query(f'close_db\n{self.id}', False)

    def set_timeout(self, timeout):
        self.timeout = timeout

    def raw_query(self, request, timeout=None):
        with socket.socket() as s:
            s.connect((self.addr, self.port))
            s.send(request.encode())
            data = b''
            s.settimeout(self.timeout if timeout is None else timeout)
            while True:
                try:
                    data_part = s.recv(256)
                    data += data_part
                    if len(data_part) < 256:
                        break
                except:
                    break
            return data.decode()

    def check(self, request):
        ws_pos = request.find(' ')
        req_type = request[:ws_pos if ws_pos > 0 else len(request)]
        if req_type in ['help', 'add', 'delete']:
            return True
        return request.lower().rstrip().endswith('end')

    def query(self, request, timeout=None):
        if self.check(request):
            return self.raw_query(f'request\n{self.id}\n{request}', timeout)
        else:
            print('Request should ends with "end"')
            return ''


inp = input('Type server addr and port separated by comma.\nFor default ("localhost", 8080) just press Enter\n')
addr, port = 'localhost', 8080
if inp.strip() != '':
    addr, port = inp.split(',')
cli = DataBaseClient(addr=addr, port=int(port), timeout=1)
inp = input()
while True:
    data = cli.query(inp.strip())
    if data != '':
        if data[:data.find('\n')].lower() == 'help:':
            print(data)
        elif data[:data.find('\n')].lower() == 'error:':
            print(data)
        else:
            df = pandas.read_csv(StringIO(data))
            print(df)
            print('For full view type: full')
            inp = input()
            if inp[:4].lower() == 'full':
                with pandas.option_context('display.max_rows', None, 'display.max_columns', None):
                    print(df)
            else:
                continue
    inp = input()
    if inp[:4].lower() == 'exit':
        break
