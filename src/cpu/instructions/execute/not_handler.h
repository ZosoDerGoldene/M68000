//
// Created by zoso on 11/22/24.
//

#ifndef NOT_HANDLER_H
#define NOT_HANDLER_H

#include "../instructions.h"

namespace cpu::instructions::execute {
    struct not_handler {
        template<unsigned_integer src_t>
        inline static long_t execute(src_t src, cpu& cpu) {
            registers::status_register& sr = cpu.sr();
            bool zero, negative;
            src_t result = src;
            asm ("not %0" :
                "=r" (result), "=@ccz" (zero), "=@ccs" (negative) :
                "0" (result));
            cpu.sr().template set_cc<false>((zero << 2) | (negative << 3));
            return result;
        }

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::data_alterable<mode>;
    };
}

#endif //NOT_HANDLER_H
