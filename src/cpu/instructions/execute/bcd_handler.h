//
// Created by zoso on 11/20/24.
//

#ifndef BCD_HANDLER_H
#define BCD_HANDLER_H

#include "../../effective_address.h"

namespace cpu::instructions::execute {

    namespace {
        byte_t do_pack(const byte_t src) {
            const byte_t src_low = src & mask<byte_t, 4>;
            const byte_t src_high = mask_and_shift<byte_t, 4, 4>(src) * 10;
            const byte_t real_src = src_low + src_high;
            return real_src;
        }

        byte_t do_unpack(const byte_t src) {
            byte_t src_low = src % 10;
            byte_t src_high = ((src - src_low) / 10) << 4;
            byte_t real_src = src_low | src_high;
            return real_src;
        }
    }

    struct abcd_handler {
        template <unsigned_integer src_t = byte_t , unsigned_integer dest_t = byte_t>
        static inline byte_t execute(const byte_t src, const byte_t dest, cpu& cpu) {
            registers::status_register &sr = cpu.sr();
            static_assert(is_byte<src_t>);
            static_assert(is_byte<dest_t>);
            const byte_t real_src = do_pack(src);
            const byte_t real_dest = do_pack(dest);
            byte_t result = real_src + real_dest;
            if (sr.c()) {
                ++result;
            }
            byte_t cc_mask = 0b11101110;
            byte_t cc = 0;
            if (result > 100) {
                result -= 100;
                cc = 0b10001;
            }
            if (result != 0)  {
                cc_mask ^= 0b100;
            }
            sr.set_cc<true>((sr.get_cc() & cc_mask) | cc);
            return do_unpack(result);
        }
    };

    struct sbcd_handler {
        template <unsigned_integer src_t = byte_t , unsigned_integer dest_t = byte_t>
        static inline byte_t execute(const byte_t src, const byte_t dest, cpu& cpu) {
            registers::status_register &sr = cpu.sr();
            const byte_t real_src = do_pack(src);
            const byte_t real_dest = do_pack(dest);
            byte_t result = real_dest - real_src;
            if (sr.c()) {
                --result;
            }
            byte_t cc_mask = 0b11101110;
            byte_t cc = 0;
            // FIXME: This definitely works differently for SBCD...
            if (result > 100) {
                result -= 100;
                cc = 0b10001;
            }
            if (result != 0)  {
                cc_mask ^= 0b100;
            }
            sr.set_cc<true>((sr.get_cc() & cc_mask) | cc);
            return do_unpack(result);
        }
    };

    struct pack_handler {
        static inline byte_t execute(const word_t src, const word_t adjustment) {
            const word_t adjusted = src + adjustment;
            const word_t packed = (adjusted & mask<word_t, 4>) | mask_and_shift<word_t, 4, 8, 4>(adjusted);
            return static_cast<byte_t>(packed);
        }
    };

    struct unpk_handler {
        static inline word_t execute(const byte_t src, const word_t adjustment) {
            const auto src_w = static_cast<word_t>(src);
            const word_t unpacked = (src_w & mask<word_t, 4>) | mask_and_shift<word_t, 4, 4, 4, false>(src_w);
            return unpacked + adjustment;
        }
    };

    struct pack_unpack_handler {
        template<typename handler, typename src_m>
        static inline void execute(cpu& cpu) {
            const opcode_t opcode = cpu.pc().get_current_opcode();
            src_m src_mode = ea::make_mode<src_m, true>(opcode);
            word_t src = ea::read<src_m, word_t>(src_mode, cpu);
            src_m dst_mode = ea::make_mode<src_m, false>(opcode);
            word_t adjustment = cpu.fetch_extension_word();
            ea::write<src_m, byte_t>(dst_mode, cpu, handler::execute(src, adjustment));
        }
    };
}

#endif //BCD_HANDLER_H
