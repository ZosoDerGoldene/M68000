//
// Created by zoso on 11/18/24.
//
#include "gtest/gtest.h"
#include "../../src/cpu/registers/status_register.h"

TEST(condition_code_helper, add_0_0_byte) {
    byte_t a = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, a), 0b100);
}

TEST(condition_code_helper, add_0_0_sbyte) {
    sbyte_t a = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, a), 0b100);
}

TEST(condition_code_helper, add_0_0_word) {
    word_t a = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, a), 0b100);
}

TEST(condition_code_helper, add_0_0_sword) {
    sword_t a = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, a), 0b100);
}


TEST(condition_code_helper, add_0_0_long) {
    long_t a = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, a), 0b100);
}

TEST(condition_code_helper, add_0_0_slong) {
    long_t a = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, a), 0b100);
}

TEST(condition_code_helper, add_to_0_overflow_byte) {
    byte_t a = 0x80;
    byte_t sum = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, sum), 0b10111);
}

TEST(condition_code_helper, add_to_0_byte) {
    byte_t a = 0xFE;
    byte_t b = 2;
    byte_t sum = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, b, sum), 0b10101);
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(b, a, sum), 0b10101);
}

TEST(condition_code_helper, add_to_0_overflow_sbyte) {
    sbyte_t a = -0x80;
    sbyte_t c = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, c), 0b10111);
}

TEST(condition_code_helper, add_to_0_sbyte) {
    sbyte_t a = -2;
    sbyte_t b = 2;
    sbyte_t sum = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, b, sum), 0b10101);
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(b, a, sum), 0b10101);
}

TEST(condition_code_helper, add_to_0_overflow_word) {
    word_t a = 0x8000;
    word_t sum = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, sum), 0b10111);
}

TEST(condition_code_helper, add_to_0_word) {
    word_t a = 0xFFFE;
    word_t b = 2;
    word_t sum = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, b, sum), 0b10101);
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(b, a, sum), 0b10101);
}

TEST(condition_code_helper, add_to_0_overflow_sword) {
    sword_t a = -0x8000;
    sword_t c = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, c), 0b10111);
}

TEST(condition_code_helper, add_to_0_sword) {
    sword_t a = -2;
    sword_t b = 2;
    sword_t sum = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, b, sum), 0b10101);
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(b, a, sum), 0b10101);
}

TEST(condition_code_helper, add_to_0_overflow_long) {
    long_t a = 0x80000000;
    long_t sum = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, sum), 0b10111);
}

TEST(condition_code_helper, add_to_0_long) {
    long_t a = 0xFFFFFFFE;
    long_t b = 2;
    long_t sum = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, b, sum), 0b10101);
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(b, a, sum), 0b10101);
}

TEST(condition_code_helper, add_to_0_overflow_slong) {
    slong_t a = -0x7FFFFFFF;
    slong_t c = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, c), 0b10111);
}

TEST(condition_code_helper, add_to_0_slong) {
    slong_t a = -2;
    slong_t b = 2;
    slong_t sum = 0;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, b, sum), 0b10101);
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(b, a, sum), 0b10101);
}

TEST(condition_code_helper, add_2_2_byte) {
    byte_t a = 2;
    byte_t sum = 4;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, sum), 0);
}

TEST(condition_code_helper, add_2_2_sbyte) {
    sbyte_t a = 2;
    sbyte_t sum = 4;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, sum), 0);
}

TEST(condition_code_helper, add_2_2_word) {
    word_t a = 2;
    word_t sum = 4;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, sum), 0);
}

TEST(condition_code_helper, add_2_2_sword) {
    sword_t a = 2;
    sword_t sum = 4;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, sum), 0);
}

TEST(condition_code_helper, add_2_2_long) {
    long_t a = 2;
    long_t sum = 4;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, sum), 0);
}

TEST(condition_code_helper, add_2_2_slong) {
    slong_t a = 2;
    slong_t sum = 4;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, a, sum), 0);
}

TEST(condition_code_helper, add_to_negative_byte) {
    byte_t a = 0x80;
    byte_t b = 0x10;
    byte_t sum = 0x90;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, b, sum), 0b1000);
}

TEST(condition_code_helper, add_to_negative_sbyte) {
    sbyte_t a = 0x7f;
    sbyte_t b = 0x10;
    sbyte_t sum = -0x13;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, b, sum), 0b01010);
}

TEST(condition_code_helper, add_to_negative_word) {
    word_t a = 0x8000;
    word_t b = 0x1000;
    word_t sum = a+b;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, b, sum), 0b1000);
}

TEST(condition_code_helper, add_to_negative_sword) {
    sword_t a = 0x7fff;
    sword_t b = 0x1000;
    sword_t sum = -0x1300;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, b, sum), 0b01010);
}

TEST(condition_code_helper, add_to_negative_long) {
    long_t a = 0x80000000;
    long_t b = 0x10000000;
    long_t sum = a+b;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, b, sum), 0b1000);
}

TEST(condition_code_helper, add_to_negative_slong) {
    slong_t a = 0x7fffffff;
    slong_t b = 0x10000000;
    slong_t sum = a+b;
    ASSERT_EQ(cpu::registers::condition_code_helper::get_flags(a, b, sum), 0b01010);
}
