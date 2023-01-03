#include <fstream>

#include <gtest/gtest.h>

#include "splay_tree.hpp"

TEST(SplayTree_Test, Constructor) {
    SplayTree<> spt;
    EXPECT_TRUE(spt.empty());
    spt.add(1, "2");
    spt.add(2, "3");
    spt.add(3, "4");
    spt.add(4, "5");
    EXPECT_FALSE(spt.empty());

    spt.clear();
    EXPECT_TRUE(spt.empty());
}

TEST(SplayTree_Test, Add_Search) {
    SplayTree<> spt;
    spt.add(1, "a");
    spt.add(2, "b");
    spt.add(3, "c");
    spt.add(4, "d");
    spt.add(5, "e");
    spt.add(6, "f");
    spt.add(7, "g");
    spt.add(8, "h");

    EXPECT_EQ(spt.search(1), std::make_pair(true, std::string("a")));
    EXPECT_EQ(spt.search(2), std::make_pair(true, std::string("b")));
    EXPECT_EQ(spt.search(3), std::make_pair(true, std::string("c")));
    EXPECT_EQ(spt.search(4), std::make_pair(true, std::string("d")));
    EXPECT_EQ(spt.search(5), std::make_pair(true, std::string("e")));
    EXPECT_EQ(spt.search(6), std::make_pair(true, std::string("f")));
    EXPECT_EQ(spt.search(7), std::make_pair(true, std::string("g")));
    EXPECT_EQ(spt.search(8), std::make_pair(true, std::string("h")));

    EXPECT_EQ(spt.search(0), std::make_pair(false, std::string()));
    EXPECT_EQ(spt.search(9), std::make_pair(false, std::string()));
    EXPECT_EQ(spt.search(15), std::make_pair(false, std::string()));
    EXPECT_EQ(spt.search(-2538375), std::make_pair(false, std::string()));

    EXPECT_THROW(spt.add(1, "aa"), std::logic_error);
    EXPECT_THROW(spt.add(2, "bb"), std::logic_error);
    EXPECT_THROW(spt.add(3, "cc"), std::logic_error);
    EXPECT_THROW(spt.add(4, "dd"), std::logic_error);
    EXPECT_THROW(spt.add(5, "ee"), std::logic_error);
    EXPECT_THROW(spt.add(6, "ff"), std::logic_error);
    EXPECT_THROW(spt.add(7, "gg"), std::logic_error);
    EXPECT_THROW(spt.add(8, "hh"), std::logic_error);
}

TEST(SplayTree_Test, Set) {
    SplayTree<> spt;
    spt.add(1, "a");
    spt.add(2, "b");
    spt.add(3, "c");
    spt.add(4, "d");
    spt.add(5, "e");
    spt.add(6, "f");
    spt.add(7, "g");
    spt.add(8, "h");

    spt.set(1, "aaaa");
    spt.set(2, "bbbb");
    spt.set(3, "cccc");
    spt.set(4, "dddd");
    spt.set(5, "eeee");
    spt.set(6, "ffff");
    spt.set(7, "gggg");
    spt.set(8, "hhhh");

    EXPECT_EQ(spt.search(1), std::make_pair(true, std::string("aaaa")));
    EXPECT_EQ(spt.search(2), std::make_pair(true, std::string("bbbb")));
    EXPECT_EQ(spt.search(3), std::make_pair(true, std::string("cccc")));
    EXPECT_EQ(spt.search(4), std::make_pair(true, std::string("dddd")));
    EXPECT_EQ(spt.search(5), std::make_pair(true, std::string("eeee")));
    EXPECT_EQ(spt.search(6), std::make_pair(true, std::string("ffff")));
    EXPECT_EQ(spt.search(7), std::make_pair(true, std::string("gggg")));
    EXPECT_EQ(spt.search(8), std::make_pair(true, std::string("hhhh")));

    EXPECT_THROW(spt.set(11, "1"), std::logic_error);
    EXPECT_THROW(spt.set(22, "2"), std::logic_error);
    EXPECT_THROW(spt.set(33, "3"), std::logic_error);
    EXPECT_THROW(spt.set(44, "4"), std::logic_error);
    EXPECT_THROW(spt.set(55, "5"), std::logic_error);
    EXPECT_THROW(spt.set(66, "6"), std::logic_error);
    EXPECT_THROW(spt.set(77, "7"), std::logic_error);
    EXPECT_THROW(spt.set(88, "8"), std::logic_error);
}

TEST(SplayTree_Test, Remove) {
    SplayTree<> spt;
    spt.add(1, "a");
    spt.add(2, "b");
    spt.add(3, "c");
    spt.add(4, "d");
    spt.add(5, "e");
    spt.add(6, "f");
    spt.add(7, "g");
    spt.add(8, "h");

    EXPECT_THROW(spt.remove(10), std::logic_error);
    EXPECT_THROW(spt.remove(9), std::logic_error);
    EXPECT_THROW(spt.remove(0), std::logic_error);

    spt.remove(1);
    EXPECT_FALSE(spt.search(1).first);
    spt.remove(3);
    EXPECT_FALSE(spt.search(3).first);
    spt.remove(5);
    EXPECT_FALSE(spt.search(5).first);
    spt.remove(6);
    EXPECT_FALSE(spt.search(6).first);
    spt.remove(8);
    EXPECT_FALSE(spt.search(8).first);
    spt.remove(2);
    EXPECT_FALSE(spt.search(2).first);
    spt.remove(4);
    EXPECT_FALSE(spt.search(4).first);
    spt.remove(7);
    EXPECT_FALSE(spt.search(7).first);

    EXPECT_TRUE(spt.empty());
}

TEST(SplayTree_Test, Min_max) {
    SplayTree<> spt;
    EXPECT_THROW(spt.min(), std::logic_error);
    EXPECT_THROW(spt.max(), std::logic_error);

    spt.add(100, "value");
    EXPECT_EQ(spt.min(), spt.max());

    spt.add(237, "hic sapentia");
    spt.add(2646, "");
    spt.add(-32743, "");
    spt.add(4, "\n");
    spt.add(2167352, "laudate dominum");
    spt.add(6, "1580");
    spt.add(5, "omnes gentes allelua");
    spt.set(6, "1580+1830");
    spt.add(541, "hic sapentia, soror");

    EXPECT_EQ(spt.min(), std::make_pair(static_cast<int64_t>(-32743), std::string("")));
    EXPECT_EQ(spt.max(), std::make_pair(static_cast<int64_t>(2167352), std::string("laudate dominum")));

    spt.set(2167352, "sanctus deus");
    spt.set(-32743, "sanctus fortis");
    EXPECT_EQ(spt.min(), std::make_pair(static_cast<int64_t>(-32743), std::string("sanctus fortis")));
    EXPECT_EQ(spt.max(), std::make_pair(static_cast<int64_t>(2167352), std::string("sanctus deus")));

    spt.remove(2167352);
    spt.remove(-32743);
    EXPECT_EQ(spt.min(), std::make_pair(static_cast<int64_t>(4), std::string("\n")));
    EXPECT_EQ(spt.max(), std::make_pair(static_cast<int64_t>(2646), std::string("")));
}

TEST(SplayTree_Test, Handler) {
    std::stringstream out_stream;
    std::stringstream answer_stream;
    for (size_t i = 1; i < 17; ++i) {
        std::ifstream input_file("../tests/input/input" + std::to_string(i) + ".txt", std::ios::in);
        if (!input_file.is_open()) {
            std::cerr << "File input" << i << ".txt was not open" << std::endl;
            continue;
        }
        handler<std::stringstream, std::ifstream>(out_stream, input_file);

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
