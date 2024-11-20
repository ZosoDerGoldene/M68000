//
// Created by zoso on 11/19/24.
//

#ifndef LSX_HANDLER_H
#define LSX_HANDLER_H

#include "../../effective_address.h"

namespace cpu::instructions::execute{
    template<bool left>
    struct lsx_dn_handler {
        template<unsigned_integer src_t, unsigned_integer dst_t>
        static inline dst_t execute(const src_t src, const dst_t dst, cpu& cpu) {
            registers::status_register &sr = cpu.sr();
            bool carry, overflow, zero, negative;
            dst_t result = dst;
            byte_t tmp = src & mask<byte_t, 6>;
            if constexpr(left) {
                asm ("mov  %5, %%cl\n"
                     "shl %%cl, %4" :
                "=@ccc"(carry), "=@cco"(overflow), "=@ccz"(zero), "=@ccs"(negative),
                "+r"(result) : "r"(tmp));
            } else {
                asm ("mov %5, %%cl\n"
                     "shr %%cl, %4" :
                "=@ccc"(carry), "=@cco"(overflow), "=@ccz"(zero), "=@ccs"(negative),
                "+r"(result) : "r"(tmp));
            }
            if(src == 0) {
                carry = false;
            }
            sr.template set_cc<false>(carry | (overflow << 1) | (zero << 2) | (negative << 3));
            return result;
        }
    };

    template<bool left>
    struct lsx_implicit_handler {
        template<unsigned_integer src_t, unsigned_integer dst_t>
        static inline dst_t execute(const dst_t dst, cpu& cpu) {
            long_t shift = (cpu.pc().get_current_opcode() >> 9) & 0b111;
            return lsx_dn_handler<left>::template execute<byte_t, dst_t>(shift, dst, cpu);
        }
    };

    template<bool left>
    struct lsx_ea_handler {
        template<unsigned_integer src_t, unsigned_integer dst_t>
        static inline dst_t execute(const dst_t dst, cpu& cpu) {
            return lsx_dn_handler<left>::template execute<byte_t, dst_t>(1, dst, cpu);
        }

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::memory_alterable<mode>;
    };
}
#endif //LSX_HANDLER_H
