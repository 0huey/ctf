import os
import sys
import select
import pty
from subprocess import Popen
from pexpect.fdpexpect import fdspawn

def Main():
    command = '/vagrant/Little Tommy/little_tommy'

    # open pseudo-terminal to interact with subprocess
    master_fd, slave_fd = pty.openpty()

    # use os.setsid() make it run in a new process group, or job control will not be enabled
    p = Popen(command,
              preexec_fn=os.setsid,
              stdin=slave_fd,
              stdout=slave_fd,
              stderr=slave_fd,
              universal_newlines=True)


    child = fdspawn(master_fd, encoding='ascii')
    input_handler = InputHandler()

    while child.isalive():
        index = child.expect(['Please enter an operation number:', "Thank you, your account number"])
        print("new output: " + child.before)
        if index == 0:
            next_input = input_handler.newop()

        if index == 1:
            input_handler.account_created(child.before)

        if next_input != None:
            os.write(master_fd, next_input)
            next_input = None

class InputHandler:
    def __init__(self):
        self.account_num = 0

    def newop(self):
        if self.account_num == 0:
            return self.new_account()

    def new_account(self):
        cmd = "1\nAAAA\nBBBB\n"
        return cmd.encode('ascii')

    def account_created(self, output):
        for line in output.split('\n'):
            if "Thank you, your account number" in line:
                words = line.split()
                num_index = words.index('number') + 1
                account = words[num_index]
                account.rstrip('.')
                self.account_num = account






if __name__ == '__main__':
    Main()


    #r, w, e = select.select([master_fd], [], [])
    # --#if sys.stdin in r:
        # --#d = os.read(sys.stdin.fileno(), 10240)
        # --#os.write(master_fd, d)
    # --#if master_fd in r:
        # --#o = os.read(master_fd, 10240)
        # --#if o:
            # --#os.write(sys.stdout.fileno(), o)
            # --#os.write(master_fd, b'5\n')
