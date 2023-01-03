#include <fstream>

#include <gtest/gtest.h>

#include "minheap.hpp"

using Node = typename MinHeap<int64_t, std::string>::Node;

bool operator==(const Node& a, const Node& b) {
    return a.key == b.key && a.value == b.value;
}

TEST(MinHeap_Test, Constructor) {
    MinHeap<> mhp;
    EXPECT_TRUE(mhp.empty());
    mhp.add(0, "abc");
    mhp.add(1, "abc");
    mhp.add(2, "abc");
    mhp.add(3, "abc");
    mhp.add(4, "abc");
    EXPECT_FALSE(mhp.empty());
}

TEST(MinHeap_Test, Add_Index) {
    MinHeap<> mhp;
    mhp.add(1650, "q");
    mhp.add(-441, "w");
    mhp.add(0, "e");
    mhp.add(3523424, "r");
    mhp.add(-158884, "t");
    mhp.add(5, "y");
    mhp.add(6324, "u");

    EXPECT_EQ(mhp.index(-158884), 0);
    EXPECT_EQ(mhp.index(-441), 1);
    EXPECT_EQ(mhp.index(0), 2);
    EXPECT_EQ(mhp.index(3523424), 3);
    EXPECT_EQ(mhp.index(1650), 4);
    EXPECT_EQ(mhp.index(5), 5);
    EXPECT_EQ(mhp.index(6324), 6);

    EXPECT_EQ(mhp.index(-1), -1);
    EXPECT_EQ(mhp.index(9), -1);
    EXPECT_EQ(mhp.index(15), -1);
    EXPECT_EQ(mhp.index(-2538375), -1);

    EXPECT_EQ(mhp.at(0), Node(-158884, "t"));
    EXPECT_EQ(mhp.at(1), Node(-441, "w"));
    EXPECT_EQ(mhp.at(2), Node(0, "e"));
    EXPECT_EQ(mhp.at(3), Node(3523424, "r"));
    EXPECT_EQ(mhp.at(4), Node(1650, "q"));
    EXPECT_EQ(mhp.at(5), Node(5, "y"));
    EXPECT_EQ(mhp.at(6), Node(6324, "u"));

    EXPECT_THROW(mhp.at(7), std::out_of_range);
}

TEST(MinHeap_Test, Set) {
    MinHeap<> mhp;
    mhp.add(98465, "kpg");
    mhp.add(1251225, "chvo");
    mhp.add(-135235, "zas");
    mhp.add(-191994044, "");

    mhp.at(0) = Node(0, "zero");
    mhp.at(1) = Node(1, "one");
    mhp.at(2) = Node(2, "two");
    mhp.at(3) = Node(3, "three");

    EXPECT_EQ(mhp.at(0), Node(0, "zero"));
    EXPECT_EQ(mhp.at(1), Node(1, "one"));
    EXPECT_EQ(mhp.at(2), Node(2, "two"));
    EXPECT_EQ(mhp.at(3), Node(3, "three"));
}

TEST(MinHeap_Test, Remove) {
    MinHeap<> mhp;
    mhp.add(98465, "zas");
    mhp.add(1251225, "");
    mhp.add(-135235, "chvo");
    mhp.add(-191994044, "kpg");

    EXPECT_THROW(mhp.remove(0), std::logic_error);
    EXPECT_THROW(mhp.remove(98466), std::logic_error);
    EXPECT_THROW(mhp.remove(1245), std::logic_error);

    mhp.remove(98465);
    EXPECT_EQ(mhp.index(98465), -1);
    mhp.remove(-135235);
    EXPECT_EQ(mhp.index(-135235), -1);
    mhp.remove(1251225);
    EXPECT_EQ(mhp.index(1251225), -1);
    mhp.remove(-191994044);
    EXPECT_EQ(mhp.index(-191994044), -1);

    EXPECT_TRUE(mhp.empty());
}

TEST(MinHeap_Test, Max_extract) {
    MinHeap<> mhp;
    EXPECT_THROW(mhp.max(), std::logic_error);

    mhp.add(-15312352, "TEST(MinHeap_Test, Handler) {");
    EXPECT_EQ(mhp.max(), Node(-15312352, "TEST(MinHeap_Test, Handler) {"));

    mhp.add(28135, "index_table.erase(node);");
    mhp.add(-7856485, "_remove(ind);");
    mhp.add(-146, "}");
    mhp.add(-1116, "\n\t\b\a\r");
    mhp.add(57853423, "");

    EXPECT_EQ(mhp.max(), Node(57853423, ""));

    EXPECT_EQ(mhp.extract(), Node(-15312352, "TEST(MinHeap_Test, Handler) {"));
    EXPECT_EQ(mhp.extract(), Node(-7856485, "_remove(ind);"));
    EXPECT_EQ(mhp.extract(), Node(-1116, "\n\t\b\a\r"));
    EXPECT_EQ(mhp.extract(), Node(-146, "}"));
    EXPECT_EQ(mhp.extract(), Node(28135, "index_table.erase(node);"));
    EXPECT_EQ(mhp.extract(), Node(57853423, ""));
    EXPECT_THROW(mhp.extract(), std::logic_error);
    EXPECT_TRUE(mhp.empty());
}

TEST(MinHeap_Test, Handler) {
    std::stringstream out_stream;
    std::stringstream answer_stream;
    for (size_t i = 1; i < 20; ++i) {
        std::ifstream input_file("../tests/input/input" + std::to_string(i) + ".txt", std::ios::in);
        if (!input_file.is_open()) {
            std::cerr << "File input" << i << ".txt was not open" << std::endl;
            continue;
        }
        handler<std::ifstream, std::stringstream>(input_file, out_stream);

        std::ofstream answer_file("../tests/answer/answer" + std::to_string(i) + ".txt", std::ios::in);
        if (!answer_file.is_open()) {
            std::cerr << "File answer" << i << ".txt was not open" << std::endl;
            continue;
        }
        answer_stream << answer_file.rdbuf();

        EXPECT_EQ(out_stream.str(), answer_stream.str());

        out_stream.clear();
        answer_file.clear();
        answer_stream.clear();
        input_file.close();
    }
}
