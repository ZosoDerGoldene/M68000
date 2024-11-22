//
// Created by zoso on 11/22/24.
//

#ifndef CLR_HANDLER_H
#define CLR_HANDLER_H

#include "../instructions.h"
namespace cpu::instructions::execute {
    struct clr_handler {
        template<unsigned_integer src_t>
        inline static src_t execute(src_t src, cpu& cpu) {
            cpu.sr().set_cc<false>(0b100); // Zero set, everything else cleared, X unaffected (arithmetic == false)
            return 0;
        }

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::data_alterable<mode>;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = false;
    };
}

#endif //CLR_HANDLER_H
