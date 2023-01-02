import sys


def get_vulnerable_path(data, lib):
    if lib in used:
        output.add(' '.join(list(data.keys())[::-1]))
    if lib in libs_map:
        for parent in libs_map[lib]:
            if parent in data:
                continue
            data[parent] = None
            get_vulnerable_path(data, parent)
            data.pop(parent)


if __name__ == '__main__':
    input_data = [line.split() for line in sys.stdin.read().split('\n')]
    if len(input_data) < 3:
        exit(0)
    vulnerable = input_data.pop(0)
    if not vulnerable or vulnerable == ['']:
        exit(0)
    used = set(input_data.pop(0))

    libs_map = dict()
    for line in input_data:
        for input_lib in line[1:]:
            if input_lib in libs_map:
                libs_map[input_lib].append(line[0])
                continue
            libs_map[input_lib] = [line[0]]

    output = set()
    for v_lib in vulnerable:
        get_vulnerable_path(data={v_lib: None}, lib=v_lib)

    if output:
        print('\n'.join(output))
