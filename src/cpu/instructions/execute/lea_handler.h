//
// Created by zoso on 11/21/24.
//

#ifndef LEA_HANDLER_H
#define LEA_HANDLER_H

#include "../../effective_address.h"

namespace cpu::instructions::execute {

    struct lea_handler {
        static inline address_t execute(const address_t src, const address_t dst, cpu& cpu) {
            return src;
        }

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = ea::control<mode>;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = std::same_as<ea::an, mode>;
    };
}
#endif //LEA_HANDLER_H
