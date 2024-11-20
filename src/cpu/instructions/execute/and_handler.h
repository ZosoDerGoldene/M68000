//
// Created by zoso on 11/19/24.
//

#ifndef AND_HANDLER_H
#define AND_HANDLER_H

#include "../../registers/status_register.h"
#include "../../cpu.h"
#include "../../effective_address.h"
namespace cpu::instructions::execute {
    struct and_handler {
        template<unsigned_integer src_t, unsigned_integer dest_t>
        static inline dest_t execute(const src_t src, const dest_t dest, cpu& cpu) {
            registers::status_register& sr = cpu.sr();
            bool zero, negative;
            dest_t result = dest;
            asm ("and %3, %2":
            "=@ccz"(zero), "=@ccs"(negative), "+r"(result):"r"(src));
            sr.template set_cc<false>((sr.get_cc() & ~(0b1100)) | (zero << 2) | (negative << 3));
            return result;
        }
    };

    struct and_dn_handler : public and_handler {

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = ea::data<mode>;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = std::same_as<ea::dn, mode>;
    };

    struct and_ea_handler : public and_handler {

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = std::same_as<ea::dn, mode>;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::memory_alterable<mode>;
    };

    struct andi_handler : public and_handler {

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = std::same_as<ea::imm, mode>;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::data_alterable<mode>;
    };

    struct andi2ccr_handler {
        template<unsigned_integer src_t>
        static inline void execute(const src_t src, cpu& cpu) {
            registers::status_register& sr = cpu.sr();
            sr.template set_cc<true>(sr.get_cc() & src);
        }
    };
}

#endif //AND_HANDLER_H
