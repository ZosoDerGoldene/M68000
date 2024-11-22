//
// Created by zoso on 11/22/24.
//

#ifndef SWAP_HANDLER_H
#define SWAP_HANDLER_H

#include "../instructions.h"

namespace cpu::instructions::execute {
    struct swap_handler {
        template<unsigned_integer src_t>
        inline static long_t execute(src_t src, cpu& cpu) {
            const long_t upper_half = (src & 0xFFFF) << 16;
            const long_t lower_half = src >> 16;
            const long_t result = upper_half | lower_half;
            const bool zero = (result == 0);
            const bool negative = is_negative(result);
            cpu.sr().template set_cc<false>((zero << 2) | (negative << 3));
            return result;
        }

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = std::same_as<ea::dn, mode>;
    };
}

#endif //SWAP_HANDLER_H
