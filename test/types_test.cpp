//
// Created by zoso on 11/18/24.
//

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Tests
#include "gtest/gtest.h"
#include "../src/types.h"

//BOOST_AUTO_TEST_SUITE(basic_types)

    TEST(basic_types, sign_extend_positive_byte) {
        byte_t byte = 0x1;
        ASSERT_EQ(sign_extend<byte_t>(byte), 0x1);
    }

    TEST(basic_types, sign_extend_negative_byte) {
        byte_t byte = 0xff;
        ASSERT_EQ(sign_extend<byte_t>(byte), 0xffffffff);
    }

    TEST(basic_types, sign_extend_positive_sbyte) {
        sbyte_t byte = 0x1;
        ASSERT_EQ(sign_extend<sbyte_t>(byte), 0x1);
    }

    TEST(basic_types, sign_extend_negative_sbyte) {
        sbyte_t byte = -1;
        ASSERT_EQ(sign_extend<sbyte_t>(byte), 0xffffffff);
    }

    TEST(basic_types, sign_extend_positive_word) {
        word_t word = 0x1;
        ASSERT_EQ(sign_extend<word_t>(word), 0x1);
    }

    TEST(basic_types, sign_extend_negative_word) {
        word_t word = 0xffff;
        ASSERT_EQ(sign_extend<word_t>(word), 0xffffffff);
    }

    TEST(basic_types, sign_extend_positive_sword) {
        sword_t word = 0x1;
        ASSERT_EQ(sign_extend<sword_t>(word), 0x1);
    }

    TEST(basic_types, sign_extend_negative_sword) {
        sword_t word = -1;
        ASSERT_EQ(sign_extend<sword_t>(word), 0xffffffff);
    }
