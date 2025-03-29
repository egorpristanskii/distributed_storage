#include "value.h"

#include <gtest/gtest.h>

TEST(TestValue, IntDataTypeName) {
    storage::IntData value{5};
    ASSERT_EQ(value.typeName(), "int");
}

TEST(TestValue, IntDataToString) {
    storage::IntData value{5};
    ASSERT_EQ(value.toString(), "5");
}

TEST(TestValue, IntDataCloneData) {
    storage::IntData value{5};
    auto clone_value = value.cloneData();
    ASSERT_EQ(clone_value->toString(), "5");
    ASSERT_EQ(clone_value->typeName(), "int");
}

TEST(TestValue, StringDataTypeName) {
    storage::StringData value{"testdata"};
    ASSERT_EQ(value.typeName(), "std::string");
}

TEST(TestValue, StringDataToString) {
    storage::StringData value{"testdata"};
    ASSERT_EQ(value.toString(), "testdata");
}

TEST(TestValue, StringDataCloneData) {
    storage::StringData value{"testdata"};
    auto clone_value = value.cloneData();
    ASSERT_EQ(clone_value->toString(), "testdata");
    ASSERT_EQ(clone_value->typeName(), "std::string");
}
