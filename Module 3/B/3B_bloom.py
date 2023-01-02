"""
Фильтр Блума
Реализуйте фильтр Блума, позволяющий дать быстрый, но вероятностный ответ, присутствует ли объект в коллекции.
Реализация самой структуры данных должна быть инкапуслирована, т.е. не зависеть от форматов входных/выходных данных и
непосредственно ввода/вывода.
Реализация битового массива также должна быть инкапсулирована. Массив битов должен быть эффективно расположен в памяти.
Параметрами структуры данных являются n - приблизительное количество элементов (целое), P - вероятность
ложноположительного ответа.
Размер структуры, m, вычисляется как -n log2 P / ln 2, а количество хэш-функций - как -log2 P. Оба значения округляются
до ближайшего целого.
В качестве семейства функций используйте семейство хэш-функций вида
hi(x) = (((i + 1)*x + pi+1) mod M) mod m,
где - x - ключ, i - номер хэш-функции, i∈[0; k-1], pj - j-тое по счету простое число, а M - 31ое число Мерсенна.

Формат входных данных
На стандартном потоке ввода задаётся последовательность команд. Пустые строки игнорируются.
Первая строка содержит команду вида set n P.
Каждая последующая строка содержит ровно одну команду: add K, search K или print, где K - неотрицательное число (64
бита вам хватит), ключ.

Формат результата
Команда set инициализирует структуру и выводит вычисленные параметры в формате "m k".
Команда add добавляет в структуру ключ K.
Команда search выводит либо "1", если элемент возможно присутствует в структуре, либо "0", если он там отсутствует.
Команда print выводит внутреннее состояние структуры - последовательность из 0 и 1, не разделенную пробелами.
В любой непонятной ситуации результатом работы любой команды будет "error".
Результат работы программы выводится в стандартный поток вывода.
"""
from math import log, e, ceil
import sys


class BitArray:
    def __init__(self, n):
        """
        Конструктор класса, создает массив
        :param n: размер массива
        """
        self.__size = n
        self.__data = bytearray(ceil(self.__size / 8))

    def size(self):
        """
        Метод получения размера массива
        :return: размер массиве
        """
        return self.__size

    def __getitem__(self, item):
        """
        Оператор доступа по индексу
        :param item: индекс
        :return: значение, записанное по индексу item
        """
        return 1 if self.__data[item // 8] & 1 << (item % 8) else 0

    def __setitem__(self, key, value):
        """
        Оператор изменения по индексу
        :param key: индекс
        :param value: добавляемое значение
        :return: None
        """
        if not self[key]:
            self.__data[key // 8] += value << key % 8

    def __str__(self):
        """
        Метод получения строкового представления массива
        :return: строка из 0 и 1, представляющая массив
        """
        output = ''
        for i in range(self.__size):
            output += '1' if self[i] else '0'
        return output

    def __repr__(self):
        """
        Метод получения строкового представления массива
        :return: строка из 0 и 1, представляющая массив
        """
        return self.__str__()


class BloomFilter:
    def __init__(self, n, p):
        """
        Конструктор класса, определяет размер структуры, битовый массив, число хэш-функций
        :param n: приблизительное число элементов
        :param p: вероятность ложноположительного ответа
        """
        if n <= 0 or p < 0 or p > 1:
            raise ValueError('invalid parameters')

        self.__m = round(-n * log(p, 2) / log(2, e))
        self.__k = round(-log(p, 2))

        if self.__k < 1:
            raise ValueError('invalid parameters')

        self.__array = BitArray(self.__m)
        self.__primes = self.__get_prime_array(self.__k)

    @staticmethod
    def __get_prime_array(n):
        """
        Метод составления массив простых чисел
        :param n: размер массива
        :return: массив
        """
        result = [2]
        curr = 1
        number = 2
        while curr != n:
            for i in range(curr):
                if not number % result[i]:
                    number += 1
                    break
                elif i == curr - 1:
                    result.append(number)
                    curr += 1
        return result

    def __h(self, i, x):
        """
        Метод вычисления хэш-функции
        :param i: номер функции
        :param x: ключ
        :return: значение х-ф
        """
        return ((i + 1) * x + self.__primes[i]) % 2147483647 % self.__m

    def add(self, key):
        """
        Метод добавления ключа в структуру
        :param key: ключ
        :return: None
        """
        for i in range(self.__k):
            self.__array[self.__h(i, key)] = 1

    def search(self, key):
        """
        Метод поиска ключа в структуре
        :param key: ключ
        :return: 1/0 в зависимости от результата поиска
        """
        for i in range(self.__k):
            if not self.__array[self.__h(i, key)]:
                return False
        return True

    def get_m(self):
        """
        Геттер размера структуры
        :return: размер структуры
        """
        return self.__m

    def get_k(self):
        """
        Геттер числа хэш-функций
        :return: число хэш-функций
        """
        return self.__k

    def __str__(self):
        """
        Метод получения строкового представления внутреннего состояния структуры
        :return: стр. представления внутр. состояния
        """
        return str(self.__array)


def handler(input_data):
    """
    Функция обработки входных команд
    :param input_data:
    :return: None
    """

    def error():
        print('error')

    input_data = [line.split() for line in input_data.split('\n') if line]
    if not input_data:
        return

    i = -1
    bf = None
    for command in input_data:
        i += 1
        if command[0] == 'set' and len(command) == 3:
            try:
                bf = BloomFilter(int(command[1]), float(command[2]))
                print(f'{bf.get_m()} {bf.get_k()}')
                break
            except ValueError:
                error()
                continue
        error()

    if bf is None:
        return

    for command in input_data[i + 1:]:
        if len(command) > 2:
            error()
            continue

        if command[0] == 'add':
            if len(command) != 2:
                error()
                continue
            bf.add(int(command[1]))

        elif command[0] == 'search':
            if len(command) != 2:
                error()
                continue
            print(int(bf.search(int(command[1]))))

        elif command[0] == 'print':
            if len(command) != 1:
                error()
                continue
            print(bf)

        else:
            error()

if __name__ == '__main__':
    handler(sys.stdin.read())
