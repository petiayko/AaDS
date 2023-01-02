import sys


class Dequeue:
    def __init__(self, size):
        self.__set_size = size
        self.__dequeue = [None] * size
        self.__curr_size = 0
        self.__front = 0
        self.__back = 0

    def empty(self):
        return self.__curr_size == 0

    def full(self):
        return self.__curr_size == self.__set_size

    def pushb(self, val):
        if not self.full():
            self.__dequeue[self.__back] = val
            self.__back = (self.__back + 1) % self.__set_size
            self.__curr_size += 1
            return
        print('overflow')

    def pushf(self, val):
        if not self.full():
            self.__dequeue[self.__front - 1] = val
            self.__front = (self.__front - 1) % self.__set_size
            self.__curr_size += 1
            return
        print('overflow')

    def popb(self):
        if not self.empty():
            print(self.__dequeue[self.__back - 1])
            self.__dequeue[self.__back - 1] = None
            self.__back = (self.__back - 1) % self.__set_size
            self.__curr_size -= 1
            return
        print('underflow')

    def popf(self):
        if not self.empty():
            print(self.__dequeue[self.__front])
            self.__dequeue[self.__front] = None
            self.__front = (self.__front + 1) % self.__set_size
            self.__curr_size -= 1
            return
        print('underflow')

    def __repr__(self):
        if self.empty():
            return 'empty'
        if self.__front < self.__back:
            return ' '.join(str(val) for val in self.__dequeue[self.__front:self.__back])
        return ' '.join(str(val) for val in self.__dequeue[self.__front:] + self.__dequeue[:self.__back])


def error(*args, **kwargs):
    print('error')


def handler(input_data):
    input_data = [line for line in input_data.split('\n') if line]
    if not input_data:
        return

    i = -1
    size = -1
    for command in input_data:
        i += 1
        if 'set_size' in command:
            set_size = command.split(' ')
            if len(set_size) != 2 or set_size[0] != 'set_size' or not set_size[1].isdigit():
                error()
                continue
            else:
                size = int(set_size[1])
                break
        error()

    if size == -1:
        return

    dq = Dequeue(size)
    for command in input_data[i + 1:]:
        command = command.split(' ')
        if not command:
            continue

        if len(command) > 2:
            error()
            continue

        if command[0] in ['pushf', 'pushb']:
            if len(command) > 2 or len(command) == 1:
                error()
                continue

            method = getattr(dq, command[0], error)
            method(command[1])
        elif command[0] in ['popf', 'popb']:
            if len(command) > 1:
                error()
                continue

            method = getattr(dq, command[0], error)
            method()
        elif command[0] == 'print':
            if len(command) > 1:
                error()
                continue

            print(dq)
        else:
            error()


if __name__ == '__main__':
    handler(sys.stdin.read())
