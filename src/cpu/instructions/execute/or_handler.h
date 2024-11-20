//
// Created by zoso on 11/19/24.
//

#ifndef OR_HANDLER_H
#define OR_HANDLER_H

#include "../../effective_address.h"

namespace cpu::instructions::execute {
    struct or_handler {
        template<unsigned_integer src_t, unsigned_integer dst_t>
        static inline dst_t execute(const src_t src, const dst_t dst, cpu& cpu) {
            registers::status_register& sr= cpu.sr();
            bool zero, negative;
            dst_t result = dst;
            asm ("or %3, %2":
            "=@ccz"(zero), "=@ccs"(negative), "+r"(result):"r"(src));
            sr.template set_cc<false>((sr.get_cc() & ~(0b1100)) | (zero << 2) | (negative << 3));
            return result;
        }
    };

    struct or_dn_handler : public or_handler {
        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = ea::data<mode>;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = std::same_as<ea::dn, mode>;
    };

    struct or_ea_handler : public or_handler {

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = std::same_as<ea::dn, mode>;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::memory_alterable<mode>;
    };

    struct ori_handler : public or_handler {

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = std::same_as<ea::imm, mode>;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::data_alterable<mode>;
    };

    struct ori2ccr_handler {
        template<unsigned_integer src_t>
        static inline void execute(const src_t src, cpu& cpu) {
            registers::status_register& sr = cpu.sr();
            sr.template set_cc<true>(sr.get_cc() | src);
        }
    };
}
#endif //OR_HANDLER_H
