/// \file test_box.cpp
/// \brief Test box implementation.

//
// Created by gregorian-rayne on 13/01/2026.
//

#include <gtest/gtest.h>
#include "pnf/pnf.hpp"

using namespace pnf;

TEST(BoxTest, Creation) {
    const Box box(100.0, BoxType::X);
    EXPECT_DOUBLE_EQ(box.price(), 100.0);
    EXPECT_EQ(box.type(), BoxType::X);
    EXPECT_TRUE(box.marker().empty());
}

TEST(BoxTest, CreationWithMarker) {
    const Box box(50.0, BoxType::O, "A");
    EXPECT_DOUBLE_EQ(box.price(), 50.0);
    EXPECT_EQ(box.type(), BoxType::O);
    EXPECT_EQ(box.marker(), "A");
}

TEST(BoxTest, SetMarker) {
    Box box(100.0, BoxType::X);
    box.set_marker("5");
    EXPECT_EQ(box.marker(), "5");
}

TEST(BoxTest, SetType) {
    Box box(100.0, BoxType::X);
    box.set_type(BoxType::O);
    EXPECT_EQ(box.type(), BoxType::O);
}

TEST(BoxTest, ToString) {
    const Box box(100.0, BoxType::X);
    const std::string str = box.to_string();
    EXPECT_FALSE(str.empty());
}