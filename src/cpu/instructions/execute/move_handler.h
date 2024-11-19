//
// Created by zoso on 11/19/24.
//

#ifndef MOVE_HANDLER_H
#define MOVE_HANDLER_H

#include "../../ea/effective_address.h"

namespace cpu::instructions::execute {
    struct move_handler {
        template<unsigned_integer src_t, unsigned_integer dst_t>
        static inline dst_t execute(const src_t src, cpu& cpu) {
            registers::status_register& sr = cpu.sr();
            byte_t flags = 0;
            if (src == 0) {
                flags = 0b100;
            }
            if (is_negative<src_t>(src)) {
                flags |= 01000;
            }
            sr.set_cc<false>(flags);
            return src;
        }

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = true;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::data_alterable<mode>;
    };

    // No need to sign_extend the return value of movea; the address register implementation handles that
    struct movea_handler : public move_handler {

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = true;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = std::same_as<ea::an, mode>;
    };

    struct moveq_handler {
        template<unsigned_integer dst_t>
        static inline dst_t execute(cpu& cpu) {
            const long_t src = sign_extend(static_cast<byte_t>(cpu.pc().get_current_opcode() & 0xff));
            return move_handler::execute<long_t, long_t>(src, cpu);
        }

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = std::same_as<ea::dn, mode>;
    };
}
#endif //MOVE_HANDLER_H
