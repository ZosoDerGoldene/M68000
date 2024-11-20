//
// Created by zoso on 11/19/24.
//

#ifndef ASX_HANDLER_H
#define ASX_HANDLER_H

#include "../../effective_address.h"

namespace cpu::instructions::execute {
    template<bool left>
    struct asx_dn_handler {
        template<unsigned_integer src_t, unsigned_integer dst_t>
        static inline dst_t execute(const src_t src, const dst_t dst, cpu& cpu) {
            registers::status_register& sr = cpu.sr();
            bool carry, overflow, zero, negative;
            dst_t result = dst;
            byte_t tmp = src & mask<byte_t, 6>;
            if(tmp == 0) {
                carry = false;
                zero = (result == 0);
                negative = is_negative(result);
                sr.template set_cc<false>(carry | (zero << 2) | (negative << 3));
            } else {
                if constexpr(left) {
                    if (tmp >= num_bits<dst_t>) {
                        result = 0;
                        negative = false;
                        zero = true;
                        carry = tmp == num_bits<dst_t> ? (dst & 1) : false;
                        overflow = false;
                    } else {
                        asm ("mov %5, %%cl\n"
                             "sal %%cl, %4" :
                        "=@ccc"(carry), "=@cco"(overflow), "=@ccz"(zero), "=@ccs"(negative),
                        "+r"(result) : "r"(tmp));
                    }
                } else {
                    if (tmp >= num_bits<dst_t>) {
                        result = is_negative(dst) ? type_mask<dst_t, dst_t> : 0;
                        negative = is_negative(dst);
                        zero = !is_negative(dst);
                        overflow = false;
                        carry = is_negative(dst);
                    } else {
                        asm ("mov %5, %%cl\n"
                             "sar %%cl, %4" :
                        "=@ccc"(carry), "=@cco"(overflow), "=@ccz"(zero), "=@ccs"(negative),
                        "+r"(result) : "r"(tmp));
                    }
                }
                sr.template set_cc<true>(
                    carry | (overflow << 1) | (zero << 2) | (negative << 3) | (carry << 4));
            }
            return result;
        }
    };

    template<bool left>
    struct asx_implicit_handler {
        template<unsigned_integer src_t, unsigned_integer dst_t>
        static inline dst_t execute(const dst_t dst, cpu& cpu) {
            long_t shift = (cpu.pc().get_current_opcode() >> 9) & 0b111;
            return asx_dn_handler<left>::template execute<byte_t, dst_t>(shift, dst, cpu);
        }
    };

    template<bool left>
    struct asx_ea_handler {
        template<unsigned_integer src_t, unsigned_integer dst_t>
        static inline dst_t execute(const dst_t dst, cpu& cpu) {
            return asx_dn_handler<left>::template execute<byte_t, dst_t>(1, dst, cpu);
        }

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::memory_alterable<mode>;
    };
}
#endif //ASX_HANDLER_H
