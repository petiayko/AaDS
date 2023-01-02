#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

template<class K = int64_t, class V = std::string>
class SplayTree {
public:
    SplayTree() noexcept: root(nullptr) {}

    ~SplayTree() noexcept {
        if (!empty()) {
            clear();
        }
    }

    void add(const K &key, const V &value) {
        /*
         * метод добавления узла в дерево, если узла с таким ключом нет
         */
        if (empty()) {
            root = new Node{key, value};
            return;
        }
        auto search_result = _search(root, key);
        if (search_result.second) {
            root = _splay(search_result.first);
            throw std::logic_error{"Node with this key have already added"};
        }
        auto new_node = new Node{key, value, search_result.first};
        if (key < search_result.first->key) {
            search_result.first->left = new_node;
        } else {
            search_result.first->right = new_node;
        }
        root = _splay(new_node);
    }

    void set(const K &key, const V &new_value) {
        /*
         * метод изменения значения узла по ключу (если такой узел есть)
         */
        auto search_result = _search(root, key);
        root = _splay(search_result.first);
        if (!search_result.second) {
            throw std::logic_error{"Node with this key doesn't exist"};
        }
        if (search_result.first) {
            search_result.first->value = new_value;
        }
    }

    std::pair<bool, V> search(const K &key) noexcept {
        /*
         * метод поиска по заданному ключу
         * возвращает пару (флаг, значение), если узел найден, флаг равен true, значение - значению н. узла, иначе флаг
         * равен false, значение пустое
         */
        auto search_result = _search(root, key);
        root = _splay(search_result.first);
        if (search_result.second) {
            return std::make_pair(true, search_result.first->value);
        }
        return std::make_pair(false, V());
    }

    void remove(const K &key) {
        /*
         * метод удаления узла с заданным ключом (если такой узел есть в дереве)
         */
        auto search_result = _search(root, key);
        auto node = search_result.first;
        root = _splay(node);
        if (search_result.second) {
            if (node->left) {
                node->left->parent = nullptr;
            }
            if (node->right) {
                node->right->parent = nullptr;
            }
            if (!node->left) {
                root = node->right;
            } else if (!node->right) {
                root = node->left;
            } else {
                node->left = _splay(_max(node->left));
                node->left->right = node->right;
                if (node->right) {
                    node->right->parent = node->left;
                }
                root = node->left;
            }
            delete node;
            return;
        }
        throw std::logic_error{"Node with this key doesn't exist"};
    }

    std::pair<K, V> min() {
        /*
         * метод получения узла с минимальным ключом
         * если дерево не пустое, возвращает пару (ключ н. узла, значение н. узла), иначе вызывает исключение
         */
        if (!empty()) {
            auto min = _min(root);
            if (min) {
                root = _splay(min);
                return std::make_pair(min->key, min->value);
            }
        }
        throw std::logic_error{"Can`t find minimum element in empty tree"};
    }

    std::pair<K, V> max() {
        /*
         * метод получения узла с максимальным ключом
         * если дерево не пустое, возвращает пару (ключ н. узла, значение н. узла), иначе вызывает исключение
         */
        if (!empty()) {
            auto max = _max(root);
            if (max) {
                root = _splay(max);
                return std::make_pair(max->key, max->value);
            }
        }
        throw std::logic_error{"Can`t find maximum element in empty tree"};
    }

    void clear() noexcept {
        /*
         * метод очищения дерева
         */
        _clear(root);
        root = nullptr;
    }

    [[nodiscard]] inline bool empty() const noexcept {
        /*
         * метод проверки дерева на пустоту
         */
        return root == nullptr;
    }

    template<class Key, class Value>
    friend std::ostream &operator<<(std::ostream &, const SplayTree<Key, Value> &) noexcept;

private:
    /*
     * приватные методы реализованы как для обычного БДП, так как не всегда при их вызове нужно делать splay
     */
    struct Node {
        /*
         * структура узла дерева, его поля: левый ребенок, правый ребенок, родитель, ключ и значение
         */
        Node *left;
        Node *right;
        Node *parent;
        K key;
        V value;

        explicit Node(
                K k = 0,
                V v = 0,
                Node *p = nullptr) noexcept: left(nullptr), right(nullptr), parent(p), key(std::move(k)),
                                             value(std::move(v)) {}

        [[nodiscard]] std::string to_string() const noexcept {
            /*
             * метод перевода узла в строку, имеющую формат, заданный в задании
             */
            if (parent) {
                return "[" + std::to_string(key) + " " + static_cast<std::string>(value) + " " +
                       std::to_string(parent->key) + "]";
            }
            return "[" + std::to_string(key) + " " + static_cast<std::string>(value) + "]";
        }
    };

    Node *root;

    static void _zig(Node *x) noexcept {
        /*
         * алгоритм Zig для узла x
         */
        auto parent = x->parent;
        auto grandparent = parent->parent;
        if (grandparent) {
            if (grandparent->left == parent) {
                grandparent->left = x;
            } else {
                grandparent->right = x;
            }
        }
        if (parent->left == x) {
            auto right_tree = x->right;
            parent->left = right_tree;
            if (right_tree) {
                right_tree->parent = parent;
            }
            x->right = parent;
            x->parent = parent->parent;
            parent->parent = x;
        } else {
            auto left_tree = x->left;
            parent->right = left_tree;
            if (left_tree) {
                left_tree->parent = parent;
            }
            x->left = parent;
            x->parent = parent->parent;
            parent->parent = x;
        }
    }

    static void _zig_zig(Node *x) noexcept {
        /*
         * алгоритм ZigZig для узла x, реализованный через алгоритмы Zig
         */
        _zig(x->parent);
        _zig(x);
    }

    static void _zig_zag(Node *x) noexcept {
        /*
         * алгоритм ZigZag для узла x, реализованный через алгоритмы Zig
         */
        _zig(x);
        _zig(x);
    }

    static Node *_splay(Node *x) noexcept {
        /*
         * алгоритм splay для узла x
         * возвращает узел x, который стал корнем (если был определен)
         */
        if (!x || !x->parent) {
            return x;
        }
        Node *parent;
        Node *grandparent;
        while (x->parent) {
            parent = x->parent;
            grandparent = parent->parent;
            if (!grandparent) {
                _zig(x);
            } else if (x == parent->left && parent == grandparent->left ||
                       x == parent->right && parent == grandparent->right) {
                _zig_zig(x);
            } else {
                _zig_zag(x);
            }
        }
        return x;
    }

    static void _clear(Node *p) noexcept {
        /*
         * удаление дерева
         */
        if (p) {
            if (p->left) {
                _clear(p->left);
            }
            if (p->right) {
                _clear(p->right);
            }
            delete p;
        }
    }

    static std::pair<Node *, bool> _search(Node *top, const K &key) {
        /*
         * поиск узла с указанным ключом в дереве с корнем top
         * возвращает пару (узел, флаг) - если узел найден, возвращается он и true, иначе будет возвращен тот узел,
         * на котором стало понятно, что найденного в дереве нет, и false
         */
        if (!top) {
            return std::make_pair(nullptr, false);
        }
        while (key != top->key && top) {
            if (key > top->key) {
                if (!top->right) {
                    return std::make_pair(top, false);
                }
                top = top->right;
            } else if (key < top->key) {
                if (!top->left) {
                    return std::make_pair(top, false);
                }
                top = top->left;
            }
        }
        return std::make_pair(top, true);
    }

    static Node *_min(Node *x) {
        /*
         * поиск узла с мин. ключом в дереве с корнем x
         * возвращает найденный узел, если x определено, иначе - nullptr
         */
        if (!x) {
            return nullptr;
        }
        while (x->left) {
            x = x->left;
        }
        return x;
    }

    static Node *_max(Node *x) {
        /*
         * поиск узла с макс. ключом в дереве с корнем x
         * возвращает найденный узел, если x определено, иначе - nullptr
         */
        if (!x) {
            return nullptr;
        }
        while (x->right) {
            x = x->right;
        }
        return x;
    }
};

template<class Key, class Value>
std::ostream &operator<<(std::ostream &out, const SplayTree<Key, Value> &tree) noexcept {
    if (tree.empty()) {
        out << "_\n";
        return out;
    }

    using Node = typename SplayTree<Key, Value>::Node *;
    using node_info = std::pair<Node, size_t>;

    std::queue<node_info> curr_layer;
    std::queue<node_info> next_layer;
    curr_layer.push(std::make_pair(tree.root, int(tree.root == nullptr)));

    node_info node;
    size_t layer_size;
    std::string temp;
    do {
        layer_size = curr_layer.size();
        for (size_t i = 0; i < layer_size; ++i) {
            if (i) {
                out << ' ';
            }
            node = curr_layer.front();
            curr_layer.pop();
            if (node.first) {
                out << node.first->to_string();

                if (node.first->left) {
                    next_layer.push(std::make_pair(node.first->left, 0));
                } else if (next_layer.empty() || next_layer.back().first) {
                    next_layer.push(std::make_pair(nullptr, 1));
                } else {
                    ++next_layer.back().second;
                }

                if (node.first->right) {
                    next_layer.push(std::make_pair(node.first->right, 0));
                } else if (next_layer.empty() || next_layer.back().first) {
                    next_layer.push(std::make_pair(nullptr, 1));
                } else {
                    ++next_layer.back().second;
                }
            } else {
                temp.reserve(2 * node.second - 2);
                for (size_t j = node.second - 1; j; --j) {
                    temp.append(" _", 2);
                }
                out << '_' << temp;
                temp.clear();
                if (next_layer.empty() || next_layer.back().first) {
                    next_layer.push(std::make_pair(nullptr, 1));
                    next_layer.back().second = node.second * 2;
                } else {
                    next_layer.back().second += node.second * 2;
                }
            }
        }
        if (!next_layer.empty()) {
            out << '\n';
        }
        curr_layer = std::move(next_layer);
    } while (curr_layer.size() > 1);
    return out;
}

void parser(const std::string &&command, std::string &name, std::string &key, std::string &value, std::string &dump) {
    /*
     * функция разбития входной строки на имя команды, аргументы (ключ и значение) и остальные символы
     */
    name.clear();
    key.clear();
    value.clear();
    dump.clear();
    std::istringstream stream(command);
    stream >> name >> key >> value >> dump;
}

int main() {
    SplayTree<> spt;

    std::string command;
    std::string name;
    std::basic_string<char> key;
    std::string value;
    std::string dump;

    std::pair<bool, std::string> search_res;
    std::pair<int64_t, std::string> minmax_res;

    while (getline(std::cin, command)) {
        if (command.empty()) {
            continue;
        }
        parser(std::move(command), name, key, value, dump);
        if (key.empty()) {
            if (name == "min") {
                try {
                    minmax_res = spt.min();
                    std::cout << minmax_res.first << ' ' << minmax_res.second << '\n';
                } catch (std::logic_error &) {
                    std::cout << "error\n";
                }
            } else if (name == "max") {
                try {
                    minmax_res = spt.max();
                    std::cout << minmax_res.first << ' ' << minmax_res.second << '\n';
                } catch (std::logic_error &) {
                    std::cout << "error\n";
                }
            } else if (name == "print") {
                std::cout << spt;
            } else {
                std::cout << "error\n";
            }
        } else if (dump.empty()) {
            if (name == "search") {
                if (!value.empty()) {
                    std::cout << "error\n";
                    continue;
                }
                search_res = spt.search(std::stoll(key));
                if (search_res.first) {
                    std::cout << "1 " << search_res.second << '\n';
                    continue;
                }
                std::cout << "0\n";
            } else if (name == "delete") {
                if (!value.empty()) {
                    std::cout << "error\n";
                    continue;
                }
                try {
                    spt.remove(std::stoll(key));
                } catch (std::logic_error &) {
                    std::cout << "error\n";
                }
            } else if (name == "add") {
                try {
                    spt.add(std::stoll(key), value);
                } catch (std::logic_error &) {
                    std::cout << "error\n";
                }
            } else if (name == "set") {
                try {
                    spt.set(std::stoll(key), value);
                } catch (std::logic_error &) {
                    std::cout << "error\n";
                }
            } else {
                std::cout << "error\n";
            }
        } else {
            std::cout << "error\n";
        }
    }
    return 0;
}
