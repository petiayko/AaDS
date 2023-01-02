import unittest
import sys
import io

from contextlib import redirect_stdout

from Module2.D.two_D_prefix import handler

INPUT_TEMP = 'input/input{}.txt'  # путь до входных данных
ANSWER_TEMP = 'answer/answer{}.txt'  # путь до ответов

FIRST_NUM = 1  # номер первого теста
LAST_NUM = 12  # номер последнего теста
SKIP_NUM = []  # номера тестов для пропуска


class PrefixTestClass(unittest.TestCase):
    def setUp(self):
        self.__first = FIRST_NUM
        self.__last = LAST_NUM
        self.__skip = SKIP_NUM
        self.__input_temp = INPUT_TEMP
        self.__answer_temp = ANSWER_TEMP
        self.__number = -1

    @staticmethod
    def __compare(output, answer):
        return set(output.split()) == set(answer.split())

    def __execute(self, number):
        input_file = open(self.__input_temp.format(number), 'r')
        sys.stdin = input_file
        stdout = io.StringIO()

        with redirect_stdout(stdout):
            handler([line for line in sys.stdin.read().split('\n') if line])
        answer_file = open(self.__answer_temp.format(number), 'r')
        self.assertTrue(self.__compare(stdout.getvalue().strip('\n'), answer_file.read().strip('\n')))

        input_file.close()
        answer_file.close()

    def test(self):
        for i in range(self.__first, self.__last + 1):
            self.__number = i
            if i in self.__skip:
                print(f'{i} skipped')
                continue
            try:
                self.__execute(i)
            except FileNotFoundError:
                print(f'Test number {i} was not found')

    def tearDown(self):
        if self.__number != self.__last:
            print(f'Test failed on {self.__number}')


if __name__ == '__main__':
    unittest.main()
