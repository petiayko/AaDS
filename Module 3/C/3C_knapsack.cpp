/// Рюкзак
/// Решите задачу о рюкзаке приближенно. Алгоритм должен быть инкапсулирован.
///
/// Формат входных данных
/// Данные подаются на стандартный поток ввода. Пустые строки игнорируются.
/// Первая строка содержит дробное число от 0 до 1 — коэффициент приближения. 0 — решение должно совпасть с оптимальным,
/// 0.1 — отличается не более чем на 10% и т.д.
/// Вторая строка содержит целое неотрицательное число — максимальную массу предметов, которую выдержит рюкзак.
/// Каждая последующая содержит два целых неотрицательных числа: массу предмета и его стоимость.
///
/// Формат результата
/// Первая строка содержит два числа: суммарную массу предметов и их суммарную стоимость.
/// В последующих строках записаны номера предметов, которые были помещены в рюкзак. Порядок не важен.
/// Результат работы программы выводится в стандартный поток вывода.

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>


class BitArray {
private:
    bool *data_ = nullptr;
    size_t size_ = 0u;

public:
    BitArray() {
        /// Конструктор класса
        size_ = 0u;
        data_ = nullptr;
    }

    explicit BitArray(size_t n) noexcept {
        /// Конструктор класса
        data_ = new bool[n]{false};
        size_ = n;
    }

    BitArray(const BitArray &ba) noexcept {
        /// Конструктор копирования
        data_ = new bool[ba.size_];
        for (size_t i = 0; i < ba.size_; ++i) {
            data_[i] = ba.data_[i];
        }
        size_ = ba.size_;
    }

    ~BitArray() {
        /// Деструктор класса
        delete[] data_;
        size_ = 0u;
    }

    BitArray &operator=(const BitArray &ba) noexcept {
        /// Оператор присваивания
        if (data_ != ba.data_) {
            delete[] data_;
            data_ = new bool[ba.size_];
            size_ = ba.size_;
            for (auto i = 0; i < ba.size_; ++i) {
                data_[i] = ba.data_[i];
            }
        }
        return *this;
    }

    bool operator[](size_t i) const {
        /// Оператор доступа по индексу
        if (i >= size_ || !data_) {
            throw std::out_of_range{"index is out of range"};
        }
        return *(data_ + i);
    }

    bool &operator[](size_t i) {
        /// Оператор доступа по индексу
        if (i >= size_ || !data_) {
            throw std::out_of_range{"index is out of range"};
        }
        return data_[i];
    }

    [[nodiscard]] std::string to_string() {
        if (!data_) {
            return "";
        }
        std::string output;
        for (auto i = 0; i < size_; ++i) {
            if (!data_[i]) {
                output += '0';
                continue;
            }
            output += '1';
        }
        return output;
    }
};

std::tuple<size_t, size_t, std::unordered_set<size_t>> knapsack_solve(const float eps, size_t w_max,
                                                                      const std::vector<std::pair<size_t, size_t>> &items) {
    /// Функция решения задачи (динамически по стоимостям)
    ///
    /// Вход:
    /// eps - коэффициент приближения
    /// w_max - вместимость рюкзака
    /// items - вектор пар <вес, стоимость>, соответствующих каждому предмету
    ///
    /// Выход:
    /// кортеж из:
    ///         полученного веса для приближенной задачи
    ///         собранной стоимости для исходной задачи
    ///         множества индексов выбранных предметов

    struct Item {
        size_t weight = 0u;
        size_t cost = 0u;
        size_t real_ind = 0u;

        explicit Item(size_t w = 0u, size_t c = 0u, size_t r_i = 0u) noexcept: weight(w), cost(c), real_ind(r_i) {}
    };

    // отсеивание бесполезных или слишком тяжелых предметов; получение максимума весов
    std::vector<Item> filtered_items;
    size_t costs_max = 0;
    std::unordered_map<size_t, size_t> indexes_map;

    for (auto i = 0; i < items.size(); ++i) {
        if (items[i].first > w_max || !items[i].second) {
            continue;
        }
        filtered_items.emplace_back(items[i].first, items[i].second, i + 1);
        if (items[i].second > costs_max) {
            costs_max = items[i].second;
        }
    }
    if (filtered_items.empty() || !costs_max) {
        return std::make_tuple(0, 0, std::unordered_set<size_t>());
    }

    // масштабирование
    if (eps != 0) {
        float coefficient = static_cast<float>(filtered_items.size()) / (eps * static_cast<float>(costs_max));
        std::transform(filtered_items.begin(), filtered_items.end(), filtered_items.begin(), [coefficient](auto &item) {
            item.cost = std::floor(item.cost * coefficient);
            return item;
        });
    }
    size_t costs_sum = std::accumulate(filtered_items.begin(), filtered_items.end(), 0,
                                       [](auto a, auto b) { return a + b.cost; });

    // создание таблицы меморизации и решение задачи
    std::vector<size_t> table_weights(costs_sum + 1, w_max + 1);
    std::unordered_map<size_t, std::unordered_set<size_t>> table_indexes;
    table_weights.front() = 0;
    size_t j_res = 0;

    for (const auto &filtered_item: filtered_items) {
        if (!filtered_item.cost) {
            continue;
        }
        for (auto j = costs_sum; j >= filtered_item.cost; --j) {
            if (table_weights[j] <= table_weights[j - filtered_item.cost] + filtered_item.weight) {
                continue;
            }
            table_weights[j] = table_weights[j - filtered_item.cost];
            table_weights[j] += filtered_item.weight;

            if (table_indexes.find(j - filtered_item.cost) == table_indexes.end()) {
                table_indexes[j] = std::unordered_set<size_t>();
            } else {
                table_indexes[j] = table_indexes[j - filtered_item.cost];
            }
            table_indexes[j].insert(filtered_item.real_ind);
            if (j > j_res) {
                j_res = j;
            }
        }
    }

    size_t cost = 0;
    for (const auto &i: table_indexes[j_res]) {
        cost += items[i - 1].second;
    }

    return std::make_tuple(table_weights[j_res], cost, table_indexes[j_res]);
}

template<class O, class I>
void handler(O &stream_out, I &stream_in) {
    /// Функция обработки ввода, вызова функции решения задачи
    ///
    /// Вход:
    /// stream_out - поток вывода
    /// stream_in - поток ввода
    ///
    /// Выход:
    /// void

    std::string input_line;

    float eps = -1;
    while (std::getline(stream_in, input_line)) {
        if (input_line.empty()) {
            continue;
        }
        eps = std::stof(input_line);
        break;
    }

    if (eps == -1) {
        return;
    }

    size_t w_max;
    while (std::getline(stream_in, input_line)) {
        if (input_line.empty()) {
            continue;
        }
        w_max = std::stoi(input_line);
        break;
    }

    size_t weight;
    size_t cost;
    std::vector<std::pair<size_t, size_t>> items;

    while (std::getline(stream_in, input_line)) {
        if (input_line.empty()) {
            continue;
        }
        std::istringstream stream(input_line);
        stream >> weight >> cost;
        items.emplace_back(weight, cost);
    }

    auto [res_w, res_c, indexes] = knapsack_solve(eps, w_max, items);
    stream_out << res_w << ' ' << res_c << std::endl;
    for (const auto &ind: indexes) {
        stream_out << ind << std::endl;
    }
}

int main() {
    handler<std::ostream, std::istream>(std::cout, std::cin);
    return 0;
}
