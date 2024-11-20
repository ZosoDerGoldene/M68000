//
// Created by zoso on 11/18/24.
//

#ifndef ADD_HANDLER_H
#define ADD_HANDLER_H
#include "../../cpu.h"
#include "../../effective_address.h"

namespace cpu::instructions::execute {

    struct add_handler {
        template<unsigned_integer src_t, unsigned_integer dst_t>
        static inline dst_t execute(const src_t src, const dst_t dst, cpu& cpu) {
            registers::status_register& sr = cpu.sr();
            bool carry, overflow, zero, negative;
            dst_t result = dst;
            asm ("add %5, %4":
            "=@ccc"(carry), "=@cco"(overflow), "=@ccz"(zero), "=@ccs"(negative), "+r"(result):"r"(src));
            sr.template set_cc<true>(carry | (overflow << 1) | (zero << 2) | (negative << 3) | (carry << 4));
            return result;
        }
    };

    struct add_dn_handler : public add_handler {

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = true;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = std::same_as<ea::dn, mode>;
    };

    struct add_ea_handler : public add_handler {

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = std::same_as<ea::dn, mode>;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::memory_alterable<mode>;
    };

    struct adda_handler : public add_handler {

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = true;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = std::same_as<ea::an, mode>;
    };

    struct addi_handler : public add_handler {

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = std::same_as<ea::imm, mode>;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::data_alterable<mode>;
    };

    struct addq_handler {
        template<unsigned_integer dst_t>
        static inline dst_t execute(dst_t dst, cpu& cpu) {
            registers::status_register& sr = cpu.sr();
            bool carry, overflow, zero, negative;
            dst_t result = dst;
            dst = static_cast<dst_t>((cpu.pc().get_current_opcode() >> 9) & 0x7);
            asm ("add %5, %4":
            "=@ccc"(carry), "=@cco"(overflow), "=@ccz"(zero), "=@ccs"(negative), "+r"(result):"r"(dst));
            sr.template set_cc<true>(carry | (overflow << 1) | (zero << 2) | (negative << 3) | (carry << 4));
            return result;
        }

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::alterable<mode>;
    };

    struct addx_handler {
        template<unsigned_integer src_t, unsigned_integer dest_t>
        static inline dest_t execute(const src_t src, const dest_t dest, cpu& cpu) {
            registers::status_register &sr = cpu.sr();
            bool carry, overflow, zero, negative;
            dest_t result = dest;
            if (sr.x()) {
                asm ("stc\n\tadc %5,%4":
                "=@ccc"(carry), "=@cco"(overflow), "=@ccz"(zero), "=@ccs"(negative), "+r"(result):"r"(src));
            } else {
                asm ("add %5,%4":
                "=@ccc"(carry), "=@cco"(overflow), "=@ccz"(zero), "=@ccs"(negative), "+r"(result):"r"(src));
            }
            sr.template set_cc<true>(carry | (overflow << 1) | (zero << 2) | (negative << 3) | (carry << 4));
            return result;
        }
    };
}
#endif //ADD_HANDLER_H
