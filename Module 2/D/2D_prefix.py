"""
Автокоррекция
Реализуйте программу, которая предлагает варианты замены слова, в котором допущена одна ошибка.
Для решения этой задачи реализуйте сжатое префиксное дерево.
Регистр букв для программы коррекции не имеет значения (слова в словаре хранятся в нижнем регистре).
Варианты ошибок - как в алгоритме Дамерау-Левенштейна: вставка лишнего символа, удаление символа, замена символа или
транспозиция соседних символов.
Реализация алгоритма должна быть инкапсулирована. В комментариях напишите сложность ключевых алгоритмов с пояснением.
Обход детей узла можно и нужно реализовать в среднем за время, линейно зависящее от длины подходящего префикса.
Соответственно, проверка наличия слова в префиксном дереве — это в среднем линейная операция, зависящая только от
длины слова.

Формат входных данных
Данные подаются на стандартный поток ввода. Пустые строки игнорируются.
Первая строка содержит число N - количество слов в словаре.
Последующие N строк содержат слова из словаря, по одному в строке.
Остальные строки - слова, которые надо проверять.

Формат результата
Каждая строка выхода содержит предложение для исправления слова. Слова обрабатываются в порядке их появления.
Если слово не содержит ошибок, то выводится "%слово% - ok".
Если слово содержит одну ошибку, то выводится "%слово% -> %слово_в_словаре%". Если вариантов несколько, то они
сортируются лексикографически и разделяются запятой с пробелом.
Если слово содержит более одной ошибки, то выводится "%слово% -?"
Результат работы программы выводится в стандартный поток вывода.
"""

import sys


def get_common_prefix(s1, s2):
    """
    Функция получения наибольшего общего префикса двух входных строк
    :param s1: первая входная строка
    :param s2: вторая входная строка
    :return: НОП
    """
    # Сложность функции O(min(n, m)), где n, m - длины входных строк
    i = 0
    while i < min(len(s2), len(s1)) and s1[i] == s2[i]:
        i += 1
    return s1[:i]


def damerau_levenshtein_distance(s1, s2):
    """
    Функция определения расстояния Дамерау-Левенштейна между входными строками.
    :param s1: первая входная строка
    :param s2: вторая входная строка
    :return: расстояние Д-Л между строками
    """
    # Сложность O(n * m), где n, m - длины входных слов
    table = {**{(i, -1): i + 1 for i in range(-1, len(s1) + 1)}, **{(-1, j): j + 1 for j in range(-1, len(s2) + 1)}}
    for i in range(len(s1)):
        for j in range(len(s2)):
            table[(i, j)] = min(
                table[(i - 1, j)] + 1,
                table[(i, j - 1)] + 1,
                table[(i - 1, j - 1)] + int(s1[i] != s2[j]),
            )
            if i and j and s1[i] == s2[j - 1] and s1[i - 1] == s2[j]:
                table[(i, j)] = min(table[(i, j)], table[i - 2, j - 2] + 1)
    return table[len(s1) - 1, len(s2) - 1]


class Dictionary:
    def __init__(self, words=None):
        self.__prefix_tree = PrefixTree()
        if words is not None:
            self.add_words(words)

    def add_word(self, word):
        """
        Метод добавления слова в словарь.
        :param word: добавляемое слово
        :return:
        """
        self.__prefix_tree.add(word)

    def add_words(self, words):
        """
        Метод добавления коллекции слов в словарь.
        :param words: коллекция добавляемых слов
        :return:
        """
        for word in words:
            self.add_word(word)

    def find(self, word):
        """
        Метод поиска слова в словаре.
        :param word:
        :return: True или False в зависимости от результата поиска
        """
        return self.__prefix_tree.find(word)

    def find_similar_words(self, word):
        """
        Метод поиска похожих слов в словаре.
        :param word:
        :return:
        """
        return self.__prefix_tree.find_similar_words(word)


class PrefixTree:
    def __init__(self, value=None, children=None, is_leaf=False):
        self.__value = value  # значение
        if children is None:
            children = {}
        self.__children = children  # словарь детей
        self.__is_leaf = is_leaf  # булево поле: True, если в этом узле кончается слово

    def __repr__(self):
        return f'"{self.__value}" is{"" if self.__is_leaf else "n`t"} leaf with {len(self.__children)} ' \
               f'children: [{", ".join([ch for ch in self.__children])}]'

    def __hash__(self):
        return hash(self.__value)

    def __eq__(self, other):
        return self.__value == other.__value

    def __ne__(self, other):
        return not self == other

    def __find_similar_words(self, word, collected=''):
        """
        Метод нечеткого поиска строки word в дереве (расстояние Д-Л между искомым и найденными словами не должны
        превышать 1). Собираются все слова от корня дерева до листьев. Поиск в конкретном поддереве будет остановлен,
        если собранная строка+(значение корня поддерева) дает слово, заведомо расположенное на расстоянии > 1 от
        искомого. Собранное слово будет добавлено в коллекцию "похожих" на искомое, если в вершине, где is_leaf = True,
        и расстояние между словами < 2.
        :param word: искомое слово
        :param collected: собранная в процессе поиска строка
        :return: множество всех подобранных слов
        """
        # Сложность О(m * n ^ 2), где n - длина искомого слова, m - мощность алфавита
        similar_words = set()
        if self.__is_leaf and damerau_levenshtein_distance(word, collected) < 2:
            similar_words.add(collected)
        for (letter, node) in self.__children.items():
            need_go_deeper = False
            for i in range(-1, 2):
                if damerau_levenshtein_distance(word[:len(collected + node.__value) + i], collected + node.__value) < 2:
                    need_go_deeper = True
                    break
            if not need_go_deeper:
                continue
            similar_words |= node.__find_similar_words(word=word, collected=collected + node.__value)
        return similar_words

    def add(self, word):
        """
        Метод добавления слова в дерево. Ищем среди прямых потомков узел на нужную букву, если в нем значение, имеющее
        непустой префикс с добавляемым значением, оставляем в узле только префикс, к его детям добавляем два новых
        узла: от word и от того, каким этот узел был.
        :param word: добавляемое слово
        :return:
        """
        # Сложность выполнения O(n), где n - длина добавляемого слов
        if not word:
            return
        if self.__value is None:
            self.__value = word
            self.__children = {}
            self.__is_leaf = True
            return
        if word == self.__value:
            return
        if word[0] not in self.__children:
            if self.__value in word:
                if not word.index(self.__value):
                    if word[len(self.__value)] in self.__children:
                        self.__children[word[len(self.__value)]].add(word[len(self.__value):])
                    else:
                        self.__children[word[len(self.__value)]] = PrefixTree(word[len(self.__value):], {}, True)
                    return
            common_prefix = get_common_prefix(self.__value, word)
            new_node = PrefixTree(self.__value[len(common_prefix):], self.__children, self.__is_leaf)
            self.__value = common_prefix
            if common_prefix == word:
                self.__children = {new_node.__value[0]: new_node}
            else:
                self.__children = {word[len(common_prefix)]: PrefixTree(word[len(common_prefix):], {}, True),
                                   new_node.__value[0]: new_node}
                self.__is_leaf = False
            return
        self.__children[word[0]].add(word)

    def find(self, word):
        """
        Метод проверки, записано ли слово в дереве. Перебирая символы слова ищем детей текущего узла на i-ю букву, если
        таких нет, поиск окончен - слово отсутствует, иначе переходим в найденный узел. Проверяем, является ли вершина,
        на которой слово кончилось, концом какого-то слова (проверяем поле is_leaf)
        :param word: искомое слово
        :return: True или False в зависимости от результата поиска
        """
        # Сложность O(n), где n - длина искомого слова
        if self.__value is None:
            return False
        curr_node = self
        while word:
            if curr_node.__value not in word:
                return False
            if word.index(curr_node.__value):
                return False
            word = word[len(curr_node.__value):]
            if not word:
                return curr_node.__is_leaf
            if word[0] not in curr_node.__children:
                return False
            curr_node = curr_node.__children[word[0]]
        return curr_node.__is_leaf

    def find_similar_words(self, word):
        """
        Публичный метод нечеткого поиска строки в дереве
        :param word:
        :return: множество найденных слов
        """
        # Сложность О(m * n ^ 2), где n - длина искомого слова, m - мощность алфавита
        if self.__value is None:
            return set()
        if not self.has_children():
            return {self.__value} if damerau_levenshtein_distance(word, self.__value) <= 1 else set()
        if self.find(word):
            return {word}
        return self.__find_similar_words(word)

    def has_children(self):
        """
        Метод определения, есть ли у self дети
        :return: True или False
        """
        # Сложность O(1)
        return True if self.__children else False


def handler(input_data):
    """
    Функция обработки входных данных
    :param input_data: входные данные
    :return: None
    """
    if not input_data:
        return
    thesaurus_size = int(input_data[0])
    thesaurus = Dictionary([word.lower() for word in input_data[1:thesaurus_size + 1] if word])

    for word in input_data[thesaurus_size + 1:]:
        if not word:
            continue
        word_lower = word.lower()
        similar_words = thesaurus.find_similar_words(word_lower)
        if not similar_words:
            print(f'{word} -?')
            continue
        if word_lower in similar_words:
            print(f'{word} - ok')
            continue
        similar_words = list(similar_words)
        similar_words.sort()
        print(f'{word} -> {", ".join(similar_words)}')


if __name__ == '__main__':
    handler([line for line in sys.stdin.read().split('\n') if line])
