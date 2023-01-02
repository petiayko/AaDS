import sys


def get_ints(s):
    if s.isdigit():
        return int(s)

    val = ''
    summa = 0
    for i in range(len(s) - 1):
        if (s[i] == '-' and '-' not in val and not val) or s[i].isdigit():
            val += s[i]
        elif val and val != '-':
            summa += int(val)
            val = ''
        else:
            continue
    if val.isdigit():
        summa += int(val)
    if s[-1].isdigit():
        val += s[-1]
        summa += int(val)
    return summa


def handler():
    result = 0
    for v in sys.stdin.read().split():
        result += get_ints(v)
    print(result)


if __name__ == '__main__':
    handler()
