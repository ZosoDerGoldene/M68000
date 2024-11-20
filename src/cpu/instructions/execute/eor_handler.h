//
// Created by zoso on 11/20/24.
//

#ifndef EOR_HANDLER_H
#define EOR_HANDLER_H

#include "../../effective_address.h"

namespace cpu::instructions::execute {
    struct eor_handler {
        template<unsigned_integer src_t, unsigned_integer dest_t>
        static inline dest_t execute(const src_t src, const dest_t dest, cpu& cpu) {
            registers::status_register& sr = cpu.sr();
            bool zero, negative;
            dest_t result = dest;
            asm ("xor %3, %2":
            "=@ccz"(zero), "=@ccs"(negative), "+r"(result):"r"(src));
            sr.template set_cc<false>((sr.get_cc() & ~(0b1100)) | (zero << 2) | (negative << 3));
            return result;
        }
    };

    struct eor_ea_handler : public eor_handler {

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = std::same_as<ea::dn, mode>;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::data_alterable<mode>;
    };

    struct eori_handler : public eor_handler {

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = std::same_as<ea::imm, mode>;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::data_alterable<mode>;
    };

    struct eori2ccr_handler {
        template<unsigned_integer src_t>
        static inline void execute(const src_t src, cpu& cpu) {
            registers::status_register &sr = cpu.sr();
            sr.template set_cc<true>(sr.get_cc() ^ src);
        }
    };
}
#endif //EOR_HANDLER_H
