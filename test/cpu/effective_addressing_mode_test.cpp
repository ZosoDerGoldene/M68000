//
// Created by zoso on 11/18/24.
//

#include "gtest/gtest.h"
#include "../../src/cpu/ea/effective_address.h"
#include "../../src/memory/layout.h"

struct ea_mode : public ::testing::Test {
    ea_mode() : _cpu(cpu::cpu(_mem)), _layout(8, 0, 0), _ctx(_cpu, _mem, 0) {
        _mem.reserve(_layout);
    }
protected:
    memory::memory _mem;
    cpu::cpu _cpu;
    memory::amiga_500::a500_layout _layout;
    cpu::execution_context _ctx;
};

struct ea_mode_68020 : public ::testing::Test {
    ea_mode_68020() : _cpu(cpu::cpu(_mem)), _layout(8, 0, 0), _ctx(_cpu, _mem, 0) {
        _mem.reserve(_layout);
    }
protected:
    memory::memory _mem;
    cpu::cpu _cpu;
    memory::amiga_500::a500_layout _layout;
    cpu::execution_context _ctx;
};

TEST_F(ea_mode, d4) {
    cpu::ea::dn d4(4);
    d4.template write<long_t>(_ctx, 0x12345678);
    ASSERT_EQ(d4.read<byte_t>(_ctx), 0x78);
    ASSERT_EQ(d4.read<word_t>(_ctx), 0x5678);
    ASSERT_EQ(d4.read<long_t>(_ctx), 0x12345678);
}

TEST_F(ea_mode, a4) {
    cpu::ea::an a4(4);
    a4.template write<long_t>(_ctx, 0x12345678);
    // byte_t is not possible as we compile with -Wall -Werror
    ASSERT_EQ(a4.read<word_t>(_ctx), 0x5678);
    ASSERT_EQ(a4.read<long_t>(_ctx), 0x12345678);
}

#ifndef NDEBUG
TEST_F(ea_mode, a4_ind) {
    _cpu.a(4) = 0x100;
    _mem.write<long_t>(0x100, 0x12345678);
    cpu::ea::an_ind a4(4);
    ASSERT_EQ(a4.read<byte_t>(_ctx), 0x12);
    ASSERT_EQ(a4.read<word_t>(_ctx), 0x1234);
    ASSERT_EQ(a4.read<long_t>(_ctx), 0x12345678);
}

TEST_F(ea_mode, a4_ind_post_incr) {
    _cpu.a(4) = 0x100;
    _mem.write<long_t>(0x100, 0x12345678);
    cpu::ea::an_ind_post_incr a4(4);
    ASSERT_EQ(a4.read<long_t>(_ctx), 0x12345678);
    ASSERT_EQ(_cpu.a(4).l(), 0x104);
    _cpu.a(4) = 0x100;
    ASSERT_EQ(a4.read<word_t>(_ctx), 0x1234);
    ASSERT_EQ(_cpu.a(4).l(), 0x102);
    _cpu.a(4) = 0x100;
    ASSERT_EQ(a4.read<byte_t>(_ctx), 0x12);
    ASSERT_EQ(_cpu.a(4).l(), 0x101);
}

TEST_F(ea_mode, a4_ind_pre_decr) {
    _cpu.a(4) = 0x104;
    _mem.write<long_t>(0x100, 0x12345678);
    cpu::ea::an_ind_pre_decr a4(4);
    ASSERT_EQ(a4.read<long_t>(_ctx), 0x12345678);
    ASSERT_EQ(_cpu.a(4).l(), 0x100);
    _cpu.a(4) = 0x102;
    ASSERT_EQ(a4.read<word_t>(_ctx), 0x1234);
    ASSERT_EQ(_cpu.a(4).l(), 0x100);
    _cpu.a(4) = 0x101;
    ASSERT_EQ(a4.read<byte_t>(_ctx), 0x12);
    ASSERT_EQ(_cpu.a(4).l(), 0x100);
}

TEST_F(ea_mode, a7_ind_post_incr) {
    _cpu.a(7) = 0x100;
    _mem.write<long_t>(0x100, 0x12345678);
    cpu::ea::an_ind_post_incr a7(7);
    ASSERT_EQ(a7.read<long_t>(_ctx), 0x12345678);
    ASSERT_EQ(_cpu.a(7).l(), 0x104);
    _cpu.a(7) = 0x100;
    ASSERT_EQ(a7.read<word_t>(_ctx), 0x1234);
    ASSERT_EQ(_cpu.a(7).l(), 0x102);
    _cpu.a(7) = 0x100;
    ASSERT_EQ(a7.read<byte_t>(_ctx), 0x12);
    ASSERT_EQ(_cpu.a(7).l(), 0x102);
}

TEST_F(ea_mode, a7_ind_pre_decr) {
    _cpu.a(7) = 0x104;
    _mem.write<long_t>(0x100, 0x12345678);
    cpu::ea::an_ind_pre_decr a7(7);
    ASSERT_EQ(a7.read<long_t>(_ctx), 0x12345678);
    ASSERT_EQ(_cpu.a(7).l(), 0x100);
    _cpu.a(7) = 0x102;
    ASSERT_EQ(a7.read<word_t>(_ctx), 0x1234);
    ASSERT_EQ(_cpu.a(7).l(), 0x100);
    _cpu.a(7) = 0x102;
    ASSERT_EQ(a7.read<byte_t>(_ctx), 0x12);
    ASSERT_EQ(_cpu.a(7).l(), 0x100);
}

TEST_F(ea_mode, imm) {
    _mem.write<long_t>(0x102, 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    cpu::ea::imm imm;
    ASSERT_EQ(imm.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(imm.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(imm.read<byte_t>(_ctx), 0x34);
}

TEST_F(ea_mode, abs_short) {
    _mem.write<word_t>(0x102, 0x200);
    _mem.write<long_t>(0x200, 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    cpu::ea::abs_short abs;
    ASSERT_EQ(abs.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(abs.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(abs.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode, abs_long) {
    _mem.write<long_t>(0x102, 0x200);
    _mem.write<long_t>(0x200, 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    cpu::ea::abs_long abs;
    ASSERT_EQ(abs.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(abs.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(abs.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, an_ind_disp) {
    _mem.write<long_t>(0x300, 0x12345678);
    _mem.write<word_t>(0x102, 0x100);
    _cpu.a(4) = 0x200;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    cpu::ea::an_ind_disp an_ind_disp(4);
    ASSERT_EQ(an_ind_disp.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(an_ind_disp.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(an_ind_disp.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, an_ind_neg_disp) {
    _mem.write<long_t>(0x200, 0x12345678);
    _mem.write<sword_t>(0x102, -0x100);
    _cpu.a(4) = 0x300;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    cpu::ea::an_ind_disp an_ind_disp(4);
    ASSERT_EQ(an_ind_disp.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(an_ind_disp.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(an_ind_disp.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, pc_ind_disp) {
    _mem.write<long_t>(0x300, 0x12345678);
    _mem.write<word_t>(0x102, 0x200);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    cpu::ea::pc_ind_disp pc_ind_disp;
    ASSERT_EQ(pc_ind_disp.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_disp.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_disp.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, pc_ind_neg_disp) {
    _mem.write<long_t>(0x200, 0x12345678);
    _mem.write<sword_t>(0x302, -0x100);
    _cpu.pc().set_pc(0x300);
    _cpu.pc().prefetch();
    cpu::ea::pc_ind_disp pc_ind_disp;
    ASSERT_EQ(pc_ind_disp.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x300);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_disp.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x300);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_disp.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, a4_ind_idx_a3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (1 << 15) | (3 << 12) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, a4_ind_idx_a3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (1 << 15) | (3 << 12) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, a4_ind_idx_d3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (3 << 12) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode, a4_ind_idx_d3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (3 << 12) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, a4_ind_idx_neg_a3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (1 << 15) | (3 << 12) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, a4_ind_idx_neg_a3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (1 << 15) | (3 << 12) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, a4_ind_idx_neg_d3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (3 << 12) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode, a4_ind_idx_neg_d3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (3 << 12) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, a4_ind_idx_a3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (1 << 15) | (3 << 12) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, a4_ind_idx_a3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (1 << 15) | (3 << 12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, a4_ind_idx_d3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (3 << 12) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, a4_ind_idx_d3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (3 << 12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode, a4_ind_idx_neg_a3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (1 << 15) | (3 << 12) | (-0x20 & 0xff));
    _cpu.a(4) = 0x400;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, a4_ind_idx_neg_a3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (1 << 15) | (3 << 12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x400;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, a4_ind_idx_neg_d3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (3 << 12) | (-0x20 & 0xff));
    _cpu.a(4) = 0x400;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, a4_ind_idx_neg_d3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (3 << 12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x400;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<true> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, pc_ind_idx_a3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (1 << 15) | (3 << 12) | 0x20);
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, pc_ind_idx_a3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (1 << 15) | (3 << 12) | (1 << 11) | 0x20);
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, pc_ind_idx_d3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (3 << 12) | 0x20);
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode, pc_ind_idx_d3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (3 << 12) | (1 << 11) | 0x20);
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, pc_ind_idx_neg_a3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x402, (1 << 15) | (3 << 12) | 0x20);
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, pc_ind_idx_neg_a3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x402, (1 << 15) | (3 << 12) | (1 << 11) | 0x20);
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, pc_ind_idx_neg_d3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x402, (3 << 12) | 0x20);
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode, pc_ind_idx_neg_d3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x402, (3 << 12) | (1 << 11) | 0x20);
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, pc_ind_idx_a3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (1 << 15) | (3 << 12) | (-0x20 & 0xff));
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, pc_ind_idx_a3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (1 << 15) | (3 << 12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, pc_ind_idx_d3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (3 << 12) | (-0x20 & 0xff));
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, pc_ind_idx_d3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (3 << 12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode, pc_ind_idx_neg_a3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x402, (1 << 15) | (3 << 12) | (-0x20 & 0xff));
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, pc_ind_idx_neg_a3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x402, (1 << 15) | (3 << 12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, pc_ind_idx_neg_d3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x402, (3 << 12) | (-0x20 & 0xff));
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode, pc_ind_idx_neg_d3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x402, (3 << 12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<true> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, a4_ind_idx_a3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_a3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_d3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, a4_ind_idx_d3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_a3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_a3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_d3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_d3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (-0x20 & 0xff));
    _cpu.a(4) = 0x400;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x400;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (-0x20 & 0xff));
    _cpu.a(4) = 0x400;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x400;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | 0x20);
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 11) | 0x20);
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_d3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | 0x20);
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, pc_ind_idx_d3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 11) | 0x20);
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x402, (1<<15) | (3<<12) | 0x20);
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x402, (1<<15) | (3<<12) | (1 << 11) | 0x20);
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_w) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x402,  (3<<12) | 0x20);
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_l) {
    _mem.write<long_t>(0x320, 0x12345678);
    _mem.write<word_t>(0x402,  (3<<12) | (1 << 11) | 0x20);
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (-0x20 & 0xff));
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_d3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (-0x20 & 0xff));
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_d3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x402, (1<<15) | (3<<12) | (-0x20 & 0xff));
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x402, (1<<15) | (3<<12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_w_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x402,  (3<<12) | (-0x20 & 0xff));
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_l_neg_disp) {
    _mem.write<long_t>(0x2e0, 0x12345678);
    _mem.write<word_t>(0x402,  (3<<12) | (1 << 11) | (-0x20 & 0xff));
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_a3_w_scale2) {
    _mem.write<long_t>(0x420, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 9) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_a3_l_scale2) {
    _mem.write<long_t>(0x420, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 9) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_d3_w_scale2) {
    _mem.write<long_t>(0x420, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 9) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, a4_ind_idx_d3_l_scale2) {
    _mem.write<long_t>(0x420, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 9) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_w_scale2) {
    _mem.write<long_t>(0x220, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 9) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_l_scale2) {
    _mem.write<long_t>(0x220, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 9) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_w_scale2) {
    _mem.write<long_t>(0x220, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 9) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_l_scale2) {
    _mem.write<long_t>(0x220, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 9) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_a3_w_neg_disp_scale2) {
    _mem.write<long_t>(0x3e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 9) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_a3_l_neg_disp_scale2) {
    _mem.write<long_t>(0x3e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_d3_w_neg_disp_scale2) {
    _mem.write<long_t>(0x3e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 9) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_d3_l_neg_disp_scale2) {
    _mem.write<long_t>(0x3e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_w_neg_disp_scale2) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 9) | (-0x20 & 0xff));
    _cpu.a(4) = 0x400;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_l_neg_disp_scale2) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x400;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_w_neg_disp_scale2) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 9) | (-0x20 & 0xff));
    _cpu.a(4) = 0x400;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_l_neg_disp_scale2) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x400;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_w_scale2) {
    _mem.write<long_t>(0x520, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 9) | 0x20);
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_l_scale2) {
    _mem.write<long_t>(0x520, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 9) | (1 << 11) | 0x20);
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_d3_w_scale2) {
    _mem.write<long_t>(0x520, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 9) | 0x20);
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, pc_ind_idx_d3_l_scale2) {
    _mem.write<long_t>(0x520, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 9) | (1 << 11) | 0x20);
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_w_scale2) {
    _mem.write<long_t>(0x220, 0x12345678);
    _mem.write<word_t>(0x402, (1<<15) | (3<<12) | (1 << 9) | 0x20);
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_l_scale2) {
    _mem.write<long_t>(0x220, 0x12345678);
    _mem.write<word_t>(0x402, (1<<15) | (3<<12) | (1 << 9) | (1 << 11) | 0x20);
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_w_scale2) {
    _mem.write<long_t>(0x220, 0x12345678);
    _mem.write<word_t>(0x402,  (3<<12) | (1 << 9) | 0x20);
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_l_scale2) {
    _mem.write<long_t>(0x220, 0x12345678);
    _mem.write<word_t>(0x402,  (3<<12) | (1 << 9) | (1 << 11) | 0x20);
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_w_neg_disp_scale2) {
    _mem.write<long_t>(0x4e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 9) | (-0x20 & 0xff));
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_l_neg_disp_scale2) {
    _mem.write<long_t>(0x4e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (1 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_d3_w_neg_disp_scale2) {
    _mem.write<long_t>(0x4e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 9) | (-0x20 & 0xff));
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_d3_l_neg_disp_scale2) {
    _mem.write<long_t>(0x4e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (1 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_w_neg_disp_scale2) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x402, (1<<15) | (3<<12) | (1 << 9) | (-0x20 & 0xff));
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_l_neg_disp_scale2) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x402, (1<<15) | (3<<12) | (1 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_w_neg_disp_scale2) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x402,  (3<<12) | (1 << 9) | (-0x20 & 0xff));
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_l_neg_disp_scale2) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x402,  (3<<12) | (1 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_a3_w_scale4) {
    _mem.write<long_t>(0x620, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (2 << 9) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_a3_l_scale4) {
    _mem.write<long_t>(0x620, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (2 << 9) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_d3_w_scale4) {
    _mem.write<long_t>(0x620, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (2 << 9) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_d3_l_scale4) {
    _mem.write<long_t>(0x620, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (2 << 9) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_w_scale4) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (2 << 9) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_l_scale4) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (2 << 9) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_w_scale4) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (2 << 9) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_l_scale4) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (2 << 9) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x400;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_a3_w_neg_disp_scale4) {
    _mem.write<long_t>(0x5e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (2 << 9) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_a3_l_neg_disp_scale4) {
    _mem.write<long_t>(0x5e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (2 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_d3_w_neg_disp_scale4) {
    _mem.write<long_t>(0x5e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (2 << 9) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_d3_l_neg_disp_scale4) {
    _mem.write<long_t>(0x5e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (2 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_w_neg_disp_scale4) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (2 << 9) | (-0x20 & 0xff));
    _cpu.a(4) = 0x600;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_l_neg_disp_scale4) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (2 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x600;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_w_neg_disp_scale4) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (2 << 9) | (-0x20 & 0xff));
    _cpu.a(4) = 0x600;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_l_neg_disp_scale4) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (2 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x600;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_w_scale4) {
    _mem.write<long_t>(0x920, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (2 << 9) | 0x20);
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_l_scale4) {
    _mem.write<long_t>(0x920, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (2 << 9) | (1 << 11) | 0x20);
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_d3_w_scale4) {
    _mem.write<long_t>(0x920, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (2 << 9) | 0x20);
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, pc_ind_idx_d3_l_scale4) {
    _mem.write<long_t>(0x920, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (2 << 9) | (1 << 11) | 0x20);
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_w_scale4) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x402, (1<<15) | (3<<12) | (2 << 9) | 0x20);
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_l_scale4) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x402, (1<<15) | (3<<12) | (2 << 9) | (1 << 11) | 0x20);
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_w_scale4) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x402,  (3<<12) | (2 << 9) | 0x20);
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_l_scale4) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x402,  (3<<12) | (2 << 9) | (1 << 11) | 0x20);
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x400);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_w_neg_disp_scale4) {
    _mem.write<long_t>(0x8e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (2 << 9) | (-0x20 & 0xff));
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_l_neg_disp_scale4) {
    _mem.write<long_t>(0x8e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (2 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_d3_w_neg_disp_scale4) {
    _mem.write<long_t>(0x8e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (2 << 9) | (-0x20 & 0xff));
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_d3_l_neg_disp_scale4) {
    _mem.write<long_t>(0x8e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (2 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.d(3) = 0x200;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_w_neg_disp_scale4) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x602, (1<<15) | (3<<12) | (2 << 9) | (-0x20 & 0xff));
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x600);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x600);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x600);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_l_neg_disp_scale4) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x602, (1<<15) | (3<<12) | (2 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x600);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x600);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x600);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_w_neg_disp_scale4) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x602,  (3<<12) | (2 << 9) | (-0x20 & 0xff));
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x600);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x600);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x600);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_l_neg_disp_scale4) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x602,  (3<<12) | (2 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x600);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x600);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x600);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_a3_w_scale8) {
    _mem.write<long_t>(0xa20, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (3 << 9) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_a3_l_scale8) {
    _mem.write<long_t>(0xa20, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (3 << 9) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_d3_w_scale8) {
    _mem.write<long_t>(0xa20, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (3 << 9) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_d3_l_scale8) {
    _mem.write<long_t>(0xa20, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (3 << 9) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_w_scale8) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (3 << 9) | 0x20);
    _cpu.a(4) = 0x800;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_l_scale8) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (3 << 9) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x800;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_w_scale8) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (3 << 9) | 0x20);
    _cpu.a(4) = 0x800;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_l_scale8) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (3 << 9) | (1 << 11) | 0x20);
    _cpu.a(4) = 0x800;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_a3_w_neg_disp_scale8) {
    _mem.write<long_t>(0x9e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (3 << 9) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_a3_l_neg_disp_scale8) {
    _mem.write<long_t>(0x9e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (3 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.a(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_d3_w_neg_disp_scale8) {
    _mem.write<long_t>(0x9e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (3 << 9) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_d3_l_neg_disp_scale8) {
    _mem.write<long_t>(0x9e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (3 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0x200;
    _cpu.d(3) = 0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_w_neg_disp_scale8) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (3 << 9) | (-0x20 & 0xff));
    _cpu.a(4) = 0xa00;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_a3_l_neg_disp_scale8) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (3 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0xa00;
    _cpu.a(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_w_neg_disp_scale8) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (3 << 9) | (-0x20 & 0xff));
    _cpu.a(4) = 0xa00;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, a4_ind_idx_neg_d3_l_neg_disp_scale8) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (3 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(4) = 0xa00;
    _cpu.d(3) = -0x100;
    cpu::ea::an_ind_idx<false> a4_ind_idx(4);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(a4_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_w_scale8) {
    _mem.write<long_t>(0x920, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (3 << 9) | 0x20);
    _cpu.a(3) = 0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_l_scale8) {
    _mem.write<long_t>(0x920, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (3 << 9) | (1 << 11) | 0x20);
    _cpu.a(3) = 0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_d3_w_scale8) {
    _mem.write<long_t>(0x920, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (3 << 9) | 0x20);
    _cpu.d(3) = 0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, pc_ind_idx_d3_l_scale8) {
    _mem.write<long_t>(0x920, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (3 << 9) | (1 << 11) | 0x20);
    _cpu.d(3) = 0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_w_scale8) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x802, (1<<15) | (3<<12) | (3 << 9) | 0x20);
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x800);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x800);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x800);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_l_scale8) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x802, (1<<15) | (3<<12) | (3 << 9) | (1 << 11) | 0x20);
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x800);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x800);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x800);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_w_scale8) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x802,  (3<<12) | (3 << 9) | 0x20);
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x800);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x800);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x800);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}


TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_l_scale8) {
    _mem.write<long_t>(0x20, 0x12345678);
    _mem.write<word_t>(0x802,  (3<<12) | (3 << 9) | (1 << 11) | 0x20);
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x800);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x800);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x800);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_w_neg_disp_scale8) {
    _mem.write<long_t>(0x8e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (3 << 9) | (-0x20 & 0xff));
    _cpu.a(3) = 0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_a3_l_neg_disp_scale8) {
    _mem.write<long_t>(0x8e0, 0x12345678);
    _mem.write<word_t>(0x102, (1<<15) | (3<<12) | (3 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(3) = 0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_d3_w_neg_disp_scale8) {
    _mem.write<long_t>(0x8e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (3 << 9) | (-0x20 & 0xff));
    _cpu.d(3) = 0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_d3_l_neg_disp_scale8) {
    _mem.write<long_t>(0x8e0, 0x12345678);
    _mem.write<word_t>(0x102,  (3<<12) | (3 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.d(3) = 0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0x100);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_w_neg_disp_scale8) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0xa02, (1<<15) | (3<<12) | (3 << 9) | (-0x20 & 0xff));
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0xa00);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0xa00);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0xa00);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_a3_l_neg_disp_scale8) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0xa02, (1<<15) | (3<<12) | (3 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.a(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0xa00);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0xa00);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0xa00);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_w_neg_disp_scale8) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0xa02,  (3<<12) | (3 << 9) | (-0x20 & 0xff));
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0xa00);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0xa00);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0xa00);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}

TEST_F(ea_mode_68020, pc_ind_idx_neg_d3_l_neg_disp_scale8) {
    _mem.write<long_t>(0x1e0, 0x12345678);
    _mem.write<word_t>(0xa02,  (3<<12) | (3 << 9) | (1 << 11) | (-0x20 & 0xff));
    _cpu.d(3) = -0x100;
    cpu::ea::pc_ind_idx<false> pc_ind_idx;
    _cpu.pc().set_pc(0xa00);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<long_t>(_ctx), 0x12345678);
    _cpu.pc().set_pc(0xa00);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<word_t>(_ctx), 0x1234);
    _cpu.pc().set_pc(0xa00);
    _cpu.pc().prefetch();
    ASSERT_EQ(pc_ind_idx.read<byte_t>(_ctx), 0x12);
}
#endif
