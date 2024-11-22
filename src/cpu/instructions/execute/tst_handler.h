//
// Created by zoso on 11/22/24.
//

#ifndef TST_HANDLER_H
#define TST_HANDLER_H

#include "../instructions.h"

namespace cpu::instructions::execute {

    struct tst_handler {
        template<unsigned_integer src_t>
         inline static src_t execute(src_t src, cpu& cpu) {
            const bool zero = (src == 0);
            const bool negative = is_negative(src);
            cpu.sr().template set_cc<false>((zero << 2) | (negative << 3));
            return src;
        }

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::data_alterable<mode>;
    };
}

#endif //TST_HANDLER_H
