//
// Created by zoso on 11/19/24.
//

#ifndef ROX_HANDLER_H
#define ROX_HANDLER_H

#include "../../effective_address.h"
namespace cpu::instructions::execute {
    template<bool left, bool with_carry>
    struct rox_dn_handler {
        template<unsigned_integer src_t, unsigned_integer dst_t>
        static inline dst_t execute(const src_t src, const dst_t dst, cpu& cpu) {
            registers::status_register &sr = cpu.sr();
            bool carry, overflow, zero, negative;
            dst_t result = dst;
            byte_t tmp = src & mask<byte_t, 6>;
            if constexpr(with_carry) {
                if (sr.c()) {
                    if constexpr(left) {
                        asm ("mov %5, %%cl \n"
                             "stc\n"
                             "rcl %%cl, %4" :
                        "=@ccc"(carry), "=@cco"(overflow), "=@ccz"(zero), "=@ccs"(negative),
                        "+r"(result) : "r"(tmp));
                    } else {
                        asm ("mov %5, %%cl\n"
                             "stc\n"
                             "rcr %%cl, %4" :
                        "=@ccc"(carry), "=@cco"(overflow), "=@ccz"(zero), "=@ccs"(negative),
                        "+r"(result) : "r"(tmp));
                    }
                } else {
                    if constexpr(left) {
                        asm ("mov %5, %%cl\n"
                             "rol %%cl, %4" :
                        "=@ccc"(carry), "=@cco"(overflow), "=@ccz"(zero), "=@ccs"(negative),
                        "+r"(result) : "r"(tmp));
                    } else {
                        asm ("mov %5, %%cl\n"
                             "ror %%cl, %4" :
                        "=@ccc"(carry), "=@cco"(overflow), "=@ccz"(zero), "=@ccs"(negative),
                        "+r"(result) : "r"(tmp));
                    }
                }
            } else {
                if constexpr(left) {
                    asm ("mov %5, %%cl\n"
                         "rol %%cl, %4" :
                    "=@ccc"(carry), "=@cco"(overflow), "=@ccz"(zero), "=@ccs"(negative),
                    "+r"(result) : "r"(tmp));
                } else {
                    asm ("mov %5, %%cl\n"
                         "ror %%cl, %4" :
                    "=@ccc"(carry), "=@cco"(overflow), "=@ccz"(zero), "=@ccs"(negative),
                    "+r"(result) : "r"(tmp));
                }
            }
            if(src == 0) {
                carry = false;
            }
            sr.template set_cc<false>(carry | (overflow << 1) | (zero << 2) | (negative << 3));
            return result;
        }
    };

    template<bool left, bool with_carry>
    struct rox_implicit_handler {
        template<unsigned_integer src_t, unsigned_integer dst_t>
        static inline dst_t execute(const dst_t dst, cpu& cpu) {
            long_t shift = (cpu.pc().get_current_opcode() >> 9) & 0b111;
            return rox_dn_handler<left, with_carry>::template execute<byte_t, dst_t>(shift, dst, cpu);
        }
    };

    template<bool left, bool with_carry>
    struct rox_ea_handler {
        template<unsigned_integer src_t, unsigned_integer dst_t>
        static inline dst_t execute(const dst_t dst, cpu& cpu) {
            return rox_dn_handler<left, with_carry>::template execute<byte_t, dst_t>(1, dst, cpu);
        }

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::memory_alterable<mode>;
    };
}
    
#endif //ROX_HANDLER_H
