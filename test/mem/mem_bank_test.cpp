//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by Benjamin Landorff on 5/6/21.
//
#include "gtest/gtest.h"
#include "../../src/memory/memory_bank.h"
[[maybe_unused]]
memory::chip_ram_bank bank2; // make sure all the static initialization required actually happens


    TEST(mem_bank_test, r_w_long_0) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<long_t>(40, 0x23456789);
        ASSERT_EQ(bank.read<long_t>(40), 0x23456789);
    }

    TEST(mem_bank_test, r_w_long_2) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<long_t>(42, 0x23456789);
        ASSERT_EQ(bank.read<long_t>(42), 0x23456789);
    }

    TEST(mem_bank_test, r_w_long_4) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<long_t>(44, 0x23456789);
        ASSERT_EQ(bank.read<long_t>(44), 0x23456789);
    }

    TEST(mem_bank_test, r_w_long_6) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<long_t>(46, 0x23456789);
        ASSERT_EQ(bank.read<long_t>(46), 0x23456789);
    }

    TEST(mem_bank_test, r_w_slong_0) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<slong_t>(40, 0x23456789);
        ASSERT_EQ(bank.read<long_t>(40), 0x23456789);
    }

    TEST(mem_bank_test, r_w_slong_2) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<slong_t>(42, 0x23456789);
        ASSERT_EQ(bank.read<long_t>(42), 0x23456789);
    }

    TEST(mem_bank_test, r_w_slong_4) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<slong_t>(44, 0x23456789);
        ASSERT_EQ(bank.read<long_t>(44), 0x23456789);
    }

    TEST(mem_bank_test, r_w_slong_6) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<slong_t>(46, 0x23456789);
        ASSERT_EQ(bank.read<long_t>(46), 0x23456789);
    }

    TEST(mem_bank_test, r_w_slong_0_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<slong_t>(40, -2);
        ASSERT_EQ(bank.read<long_t>(40), 0xfffffffe);
    }

    TEST(mem_bank_test, r_w_slong_2_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<slong_t>(42, -2);
        ASSERT_EQ(bank.read<long_t>(42), 0xfffffffe);
    }

    TEST(mem_bank_test, r_w_slong_4_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<slong_t>(44, -2);
        ASSERT_EQ(bank.read<long_t>(44), 0xfffffffe);
    }

    TEST(mem_bank_test, r_w_slong_6_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<slong_t>(46, -2);
        ASSERT_EQ(bank.read<long_t>(46), 0xfffffffe);
    }

    TEST(mem_bank_test, r_w_long_0_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<long_t>(40, 0xfffffffe);
        ASSERT_EQ(bank.read<slong_t>(40), -2);
    }

    TEST(mem_bank_test, r_w_long_2_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<long_t>(42, 0xfffffffe);
        ASSERT_EQ(bank.read<slong_t>(42), -2);
    }

    TEST(mem_bank_test, r_w_long_4_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<long_t>(44, 0xfffffffe);
        ASSERT_EQ(bank.read<slong_t>(44), -2);
    }

    TEST(mem_bank_test, r_w_long_6_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<long_t>(46, 0xfffffffe);
        ASSERT_EQ(bank.read<slong_t>(46), -2);
    }

    TEST(mem_bank_test, r_w_word_0) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<word_t>(40, 0x2345);
        ASSERT_EQ(bank.read<word_t>(40), 0x2345);
    }

    TEST(mem_bank_test, r_w_word_2) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<word_t>(42, 0x2345);
        ASSERT_EQ(bank.read<word_t>(42), 0x2345);
    }

    TEST(mem_bank_test, r_w_word_4) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<word_t>(44, 0x2345);
        ASSERT_EQ(bank.read<word_t>(44), 0x2345);
    }

    TEST(mem_bank_test, r_w_word_6) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<word_t>(46, 0x2345);
        ASSERT_EQ(bank.read<word_t>(46), 0x2345);
    }

    TEST(mem_bank_test, r_w_sword_0) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sword_t>(40, 0x2345);
        ASSERT_EQ(bank.read<word_t>(40), 0x2345);
    }

    TEST(mem_bank_test, r_w_sword_2) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sword_t>(42, 0x2345);
        ASSERT_EQ(bank.read<word_t>(42), 0x2345);
    }

    TEST(mem_bank_test, r_w_sword_4) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sword_t>(44, 0x2345);
        ASSERT_EQ(bank.read<word_t>(44), 0x2345);
    }

    TEST(mem_bank_test, r_w_sword_6) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sword_t>(46, 0x2345);
        ASSERT_EQ(bank.read<word_t>(46), 0x2345);
    }

    TEST(mem_bank_test, r_w_sword_0_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sword_t>(40, -2);
        ASSERT_EQ(bank.read<word_t>(40), 0xfffe);
    }

    TEST(mem_bank_test, r_w_sword_2_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sword_t>(42, -2);
        ASSERT_EQ(bank.read<word_t>(42), 0xfffe);
    }

    TEST(mem_bank_test, r_w_sword_4_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sword_t>(44, -2);
        ASSERT_EQ(bank.read<word_t>(44), 0xfffe);
    }

    TEST(mem_bank_test, r_w_sword_6_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sword_t>(46, -2);
        ASSERT_EQ(bank.read<word_t>(46), 0xfffe);
    }

    TEST(mem_bank_test, r_w_word_0_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<word_t>(40, 0xfffe);
        ASSERT_EQ(bank.read<sword_t>(40), -2);
    }

    TEST(mem_bank_test, r_w_word_2_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<word_t>(42, 0xfffe);
        ASSERT_EQ(bank.read<sword_t>(42), -2);
    }

    TEST(mem_bank_test, r_w_word_4_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<word_t>(44, 0xfffe);
        ASSERT_EQ(bank.read<sword_t>(44), -2);
    }

    TEST(mem_bank_test, r_w_word_6_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<word_t>(46, 0xfffe);
        ASSERT_EQ(bank.read<sword_t>(46), -2);
    }

    TEST(mem_bank_test, r_w_byte_0) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(40, 0x23);
        ASSERT_EQ(bank.read<byte_t>(40), 0x23);
    }

    TEST(mem_bank_test, r_w_byte_2) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(42, 0x23);
        ASSERT_EQ(bank.read<byte_t>(42), 0x23);
    }

    TEST(mem_bank_test, r_w_byte_4) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(44, 0x23);
        ASSERT_EQ(bank.read<byte_t>(44), 0x23);
    }

    TEST(mem_bank_test, r_w_byte_6) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(46, 0x23);
        ASSERT_EQ(bank.read<byte_t>(46), 0x23);
    }

    TEST(mem_bank_test, r_w_sbyte_0) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(40, 0x23);
        ASSERT_EQ(bank.read<byte_t>(40), 0x23);
    }

    TEST(mem_bank_test, r_w_sbyte_2) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(42, 0x23);
        ASSERT_EQ(bank.read<byte_t>(42), 0x23);
    }

    TEST(mem_bank_test, r_w_sbyte_4) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(44, 0x23);
        ASSERT_EQ(bank.read<byte_t>(44), 0x23);
    }

    TEST(mem_bank_test, r_w_sbyte_6) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(46, 0x23);
        ASSERT_EQ(bank.read<byte_t>(46), 0x23);
    }

    TEST(mem_bank_test, r_w_sbyte_0_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(40, -2);
        ASSERT_EQ(bank.read<byte_t>(40), 0xfe);
    }

    TEST(mem_bank_test, r_w_sbyte_2_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(42, -2);
        ASSERT_EQ(bank.read<byte_t>(42), 0xfe);
    }

    TEST(mem_bank_test, r_w_sbyte_4_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(44, -2);
        ASSERT_EQ(bank.read<byte_t>(44), 0xfe);
    }

    TEST(mem_bank_test, r_w_sbyte_6_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(46, -2);
        ASSERT_EQ(bank.read<byte_t>(46), 0xfe);
    }

    TEST(mem_bank_test, r_w_byte_0_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(40, 0xfe);
        ASSERT_EQ(bank.read<sbyte_t>(40), -2);
    }

    TEST(mem_bank_test, r_w_byte_2_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(42, 0xfe);
        ASSERT_EQ(bank.read<sbyte_t>(42), -2);
    }

    TEST(mem_bank_test, r_w_byte_4_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(44, 0xfe);
        ASSERT_EQ(bank.read<sbyte_t>(44), -2);
    }

    TEST(mem_bank_test, r_w_byte_6_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(46, 0xfe);
        ASSERT_EQ(bank.read<sbyte_t>(46), -2);
    }

    TEST(mem_bank_test, r_w_byte_1) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(41, 0x23);
        ASSERT_EQ(bank.read<byte_t>(41), 0x23);
    }

    TEST(mem_bank_test, r_w_byte_3) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(43, 0x23);
        ASSERT_EQ(bank.read<byte_t>(43), 0x23);
    }

    TEST(mem_bank_test, r_w_byte_5) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(45, 0x23);
        ASSERT_EQ(bank.read<byte_t>(45), 0x23);
    }

    TEST(mem_bank_test, r_w_byte_7) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(47, 0x23);
        ASSERT_EQ(bank.read<byte_t>(47), 0x23);
    }

    TEST(mem_bank_test, r_w_sbyte_1) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(41, 0x23);
        ASSERT_EQ(bank.read<byte_t>(41), 0x23);
    }

    TEST(mem_bank_test, r_w_sbyte_3) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(43, 0x23);
        ASSERT_EQ(bank.read<byte_t>(43), 0x23);
    }

    TEST(mem_bank_test, r_w_sbyte_5) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(45, 0x23);
        ASSERT_EQ(bank.read<byte_t>(45), 0x23);
    }

    TEST(mem_bank_test, r_w_sbyte_7) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(47, 0x23);
        ASSERT_EQ(bank.read<byte_t>(47), 0x23);
    }

    TEST(mem_bank_test, r_w_sbyte_1_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(41, -2);
        ASSERT_EQ(bank.read<byte_t>(41), 0xfe);
    }

    TEST(mem_bank_test, r_w_sbyte_3_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(43, -2);
        ASSERT_EQ(bank.read<byte_t>(43), 0xfe);
    }

    TEST(mem_bank_test, r_w_sbyte_5_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(45, -2);
        ASSERT_EQ(bank.read<byte_t>(45), 0xfe);
    }

    TEST(mem_bank_test, r_w_sbyte_7_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<sbyte_t>(47, -2);
        ASSERT_EQ(bank.read<byte_t>(47), 0xfe);
    }

    TEST(mem_bank_test, r_w_byte_1_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(41, 0xfe);
        ASSERT_EQ(bank.read<sbyte_t>(41), -2);
    }

    TEST(mem_bank_test, r_w_byte_3_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(43, 0xfe);
        ASSERT_EQ(bank.read<sbyte_t>(43), -2);
    }

    TEST(mem_bank_test, r_w_byte_5_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(45, 0xfe);
        ASSERT_EQ(bank.read<sbyte_t>(45), -2);
    }

    TEST(mem_bank_test, r_w_byte_7_neg) {
        char mem[65536];
        memory::bank bank(memory::memory_type::chip_ram, mem);
        bank.write<byte_t>(47, 0xfe);
        ASSERT_EQ(bank.read<sbyte_t>(47), -2);
    }
//
//    TEST(mem_bank_test, odd_long_write) {
//        MockExceptionHandler exh;
//        Memory memory(exh, MemoryLayout<MockExceptionHandler>::getA500Layout(exh));
//        memory.store<u32>(47, 0x0);
//        ASSERT_EQ(exh.has_address_error());
//    }
//
//    TEST(mem_bank_test, odd_word_write) {
//        MockExceptionHandler exh;
//        Memory memory(exh, MemoryLayout<MockExceptionHandler>::getA500Layout(exh));
//        memory.store<u16>(47, 0x0);
//        ASSERT_EQ(exh.has_address_error());
//    }