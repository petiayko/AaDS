#ifndef KNAPSACK_KNAPSACK_HPP
#define KNAPSACK_KNAPSACK_HPP

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

    for (size_t i = 0; i < items.size(); ++i) {
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

#endif //KNAPSACK_KNAPSACK_HPP
