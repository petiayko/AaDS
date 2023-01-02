import sys


def calc_unban_time(attempts_number, interval, start_time, max_block, cur_time, attempts):
    """
    n - число попыток, введенных с клавиатуры
    Временная сложность O(n*log(n))        из-за сортировки записей
    Расход памяти O(n)                     расходуется на хранение записей
    """
    tbu = 0  # time before unban
    last_interval = max_block
    counter = 0

    attempts.sort()
    i_first = 0
    for i in range(len(attempts)):
        if cur_time - attempts[i] > 2 * last_interval:
            continue

        if not counter:
            i_first = i
        if attempts[i] <= attempts[i_first] + interval:
            counter += 1
        else:
            i_first += 1
            continue

        if counter == attempts_number:
            tbu = attempts[i] + start_time
            start_time *= 2
            if start_time > max_block:
                start_time = max_block
            counter = 0

    if tbu > cur_time:
        return tbu
    return 0


def handler(input_data):
    """
    Функция обработки ввода, вызова функции решения задачи
    :param input_data: вход
    :return: None
    """
    input_data = [line for line in input_data.split('\n') if line]
    if not input_data:
        return

    config = map(int, input_data[0].split())
    rest_time = calc_unban_time(*config, list(map(int, input_data[1:])))
    if rest_time > 0:
        print(rest_time)
        return
    print('ok')


if __name__ == '__main__':
    handler(sys.stdin.read())
