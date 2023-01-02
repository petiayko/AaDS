#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>


template<class K = int64_t, class V = std::string>
class MinHeap {
public:
    struct Node {
        K key;
        V value;

        explicit Node(K k = K(), V v = V()) noexcept: key(k), value(v) {}

        [[nodiscard]] std::string to_string() const noexcept {
            /// метод преобразования узла в строку с возможностью указать индекс
            return std::to_string(key) + ' ' + static_cast<std::string>(value);
        }
    };

    void add(const K &key, const V &value) {
        /// метод добавления пары ключ-значение
        /// если ключ уже добавлен в кучу, будет вызвано исключение
        if (index_table.count(key) != 0) {
            throw std::logic_error{"This key have already added"};
        }
        tape.emplace_back(key, value);
        index_table.emplace(key, tape.size() - 1);
        _heapify(tape.size() - 1);
    }

    size_t index(const K &key) const noexcept {
        /// метод получения индекса по ключу
        /// если ключа нет в куче, будет возвращено -1
        auto node = index_table.find(key);
        if (node == index_table.end()) {
            return -1;
        }
        return node->second;
    }

    Node &at(const size_t ind) {
        /// метод, осуществляющий доступ по индексу
        /// если индекса нет, будет вызвано исключение
        if (ind >= tape.size()) {
            throw std::out_of_range{"Index is out of heap"};
        }
        return tape[ind];
    }

    Node extract() {
        /// метод извлечения корня кучи (удаление и возвращение функцией)
        /// если куча пустая, будет вызвано исключение
        if (empty()) {
            throw std::logic_error{"Cannot extract from empty heap"};
        }
        auto top = tape.front();
        remove(top.key);
        return top;
    }

    void remove(const K &key) {
        /// метод удаления узла по ключу
        /// если ключа нет в куче, будет вызвано исключение
        auto node = index_table.find(key);
        if (node == index_table.end()) {
            throw std::logic_error{"Cannot remove from empty heap"};
        }
        auto ind = node->second;
        index_table[tape.back().key] = ind;
        index_table.erase(node);
        _remove(ind);
    }

    [[nodiscard]] inline Node max() const {
        /// метод поиска максимума в куче
        /// возвращает макс. элемент
        /// если куча пустая, будет вызвано исключение
        if (tape.empty()) {
            throw std::logic_error{"Cannot find max element in empty heap"};
        }
        return tape[std::distance(tape.begin(),
                                  std::max_element(tape.begin(), tape.end(), [](const Node &n1, const Node &n2) {
                                      return n1.key < n2.key;
                                  }))];
    }

    [[nodiscard]] inline bool empty() const noexcept {
        /// метод проверки, пустая ли куча
        /// возвращает true, если пустая, false - иначе
        return tape.empty();
    }

    template<class Key, class Value>
    friend std::ostream &operator<<(std::ostream &, const MinHeap<Key, Value> &) noexcept;

private:
    std::vector<Node> tape;
    std::unordered_map<K, size_t> index_table;  // ключ, индекс в tape

    [[nodiscard]] static inline size_t _left(size_t i) noexcept {
        /// статический метод получения индекса левого ребенка
        return 2 * i + 1;
    }

    [[nodiscard]] static inline size_t _parent(size_t i) noexcept {
        /// статический метод получения индекса родителя
        return (i - 1) / 2;
    }

    void _heapify(size_t ind) noexcept {
        /// метод heapify для перестройки кучи
        if (!ind) {
            return;
        }
        auto parent = _parent(ind);
        while (ind && tape[ind].key < tape[parent].key) {
            std::swap(index_table.at(tape[ind].key), index_table.at(tape[parent].key));
            std::swap(tape[ind], tape[parent]);
            ind = parent;
            parent = _parent(parent);
        }
    }

    void _remove(size_t ind) noexcept {
        /// метод удаления по индексу
        std::swap(tape[ind], tape.back());
        tape.pop_back();
        size_t parent;
        if (ind != tape.size() - 1) {
            parent = _parent(ind);
            if (!ind || tape[parent].key < tape[ind].key) {
                auto left = _left(ind);
                auto right = left + 1;
                while (left < tape.size()) {
                    if (right < tape.size()) {
                        if (tape[ind].key < tape[left].key && tape[ind].key < tape[right].key) {
                            break;
                        }
                        if (tape[left].key < tape[right].key) {
                            std::swap(index_table.at(tape[ind].key), index_table.at(tape[left].key));
                            std::swap(tape[left], tape[ind]);
                            ind = left;
                        } else {
                            std::swap(index_table.at(tape[ind].key), index_table.at(tape[right].key));
                            std::swap(tape[right], tape[ind]);
                            ind = right;
                        }
                    } else {
                        if (tape[ind].key < tape[left].key) {
                            break;
                        }
                        std::swap(index_table.at(tape[ind].key), index_table.at(tape[left].key));
                        std::swap(tape[left], tape[ind]);
                        ind = left;
                    }
                    left = _left(ind);
                    right = left + 1;
                }
                return;
            }
            _heapify(ind);
        }
    }
};

template<class Key, class Value>
std::ostream &operator<<(std::ostream &out, const MinHeap<Key, Value> &heap) noexcept {
    /// оператор печати кучи в соответствии с заданными требованиями
    if (heap.empty()) {
        return out << '_';
    }
    auto empty_nodes = [](size_t n) {
        std::string res;
        if (n) {
            res.reserve(2 * n);
            for (; n; --n) {
                res += " _";
            }
        }
        return res;
    };
    out << "[" << heap.tape[0].to_string() << "]";
    size_t layer_size = 1;
    for (size_t i = 1; i < heap.tape.size(); ++i) {
        if (i == 2 * layer_size - 1) {
            out << '\n';
            layer_size *= 2;
        } else {
            out << ' ';
        }
        out << '[' << heap.tape[i].to_string() << ' ' << heap.tape[(i - 1) / 2].key << ']';
    }
    return out << empty_nodes(2 * layer_size - heap.tape.size() - 1);
}


void parser(const std::string &command, std::string &name, std::string &key, std::string &value, std::string &dump) {
    /// функция разбития входной строки на имя команды, аргументы (ключ и значение) и остальные символы
    name.clear();
    key.clear();
    value.clear();
    dump.clear();
    std::istringstream stream(command);
    stream >> name >> key >> value >> dump;
}

template<class I, class O>
void handler(I &stream_in, O &stream_out) {
    MinHeap<> mhp;

    std::string command;
    std::string name;
    std::basic_string<char> key;
    std::string value;
    std::string dump;

    size_t index;
    MinHeap<>::Node get_node_res;

    while (getline(stream_in, command)) {
        if (command.empty()) {
            continue;
        }
        parser(command, name, key, value, dump);
        if (key.empty()) {
            if (name == "min") {
                try {
                    get_node_res = mhp.at(0);
                    stream_out << get_node_res.key << " 0 " << get_node_res.value << '\n';
                } catch (std::logic_error &) {
                    stream_out << "error\n";
                }
            } else if (name == "max") {
                try {
                    get_node_res = mhp.max();
                    stream_out << get_node_res.key << ' ' << mhp.index(get_node_res.key) << ' ' << get_node_res.value
                               << '\n';
                } catch (std::logic_error &) {
                    stream_out << "error\n";
                }
            } else if (name == "extract") {
                try {
                    get_node_res = mhp.extract();
                    stream_out << get_node_res.to_string() << '\n';
                } catch (std::logic_error &) {
                    stream_out << "error\n";
                }
            } else if (name == "print") {
                stream_out << mhp << '\n';
            } else {
                stream_out << "error\n";
            }
        } else if (dump.empty()) {
            if (name == "search") {
                if (!value.empty()) {
                    stream_out << "error\n";
                    continue;
                }
                index = mhp.index(std::stoll(key));
                if (index != -1) {
                    stream_out << "1 " << index << " " << mhp.at(index).value << '\n';
                    continue;
                }
                stream_out << "0\n";
            } else if (name == "delete") {
                if (!value.empty()) {
                    stream_out << "error\n";
                    continue;
                }
                try {
                    mhp.remove(std::stoll(key));
                } catch (std::logic_error &) {
                    stream_out << "error\n";
                }
            } else if (name == "add") {
                try {
                    mhp.add(std::stoll(key), value);
                } catch (std::logic_error &) {
                    stream_out << "error\n";
                }
            } else if (name == "set") {
                try {
                    mhp.at(mhp.index(std::stoll(key))).value = value;
                } catch (std::out_of_range &) {
                    stream_out << "error\n";
                }
            } else {
                stream_out << "error\n";
            }
        } else {
            stream_out << "error\n";
        }
    }
}

int main() {
    handler<std::istream, std::ostream>(std::cin, std::cout);
    return 0;
}
