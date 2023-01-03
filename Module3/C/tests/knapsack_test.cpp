#include <fstream>

#include <gtest/gtest.h>

#include "knapsack.hpp"

std::unordered_set<std::string> split(const std::string &s) {
    std::stringstream stream(s);
    std::istream_iterator<std::string> begin(stream);
    std::istream_iterator<std::string> end;
    return {begin, end};
}

bool compare(const std::string &a, const std::string &b) {
    if (!a.empty() && !b.empty()) {
        return true;
    }
    if (a.empty() ^ b.empty() || a.substr(0, a.find('\n')) != b.substr(0, b.find('\n'))) {
        return false;
    }
    return split(a.substr(a.find('\n') + 1, a.size())) == split(b.substr(b.find('\n') + 1, b.size()));
}

TEST(Knapsack_Test, Handler) {
    std::stringstream out_stream;
    std::stringstream answer_stream;
    for (size_t i = 1; i < 29; ++i) {
        std::ifstream input_file("../tests/input/" + std::to_string(i) + ".txt", std::ios::in);
        if (!input_file.is_open()) {
            std::cerr << "File " << i << ".txt was not open" << std::endl;
            continue;
        }
        handler<std::stringstream, std::ifstream>(out_stream, input_file);

        std::ofstream answer_file("../tests/answer/" + std::to_string(i) + ".txt", std::ios::in);
        if (!answer_file.is_open()) {
            std::cerr << "File " << i << ".txt was not open" << std::endl;
            continue;
        }
        answer_stream << answer_file.rdbuf();

        EXPECT_TRUE(compare(out_stream.str(), answer_stream.str()));

        out_stream.clear();
        answer_file.clear();
        answer_stream.clear();
        input_file.close();
    }
}
