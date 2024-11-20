//
// Created by zoso on 11/19/24.
//

#ifndef BCC_HANDLER_H
#define BCC_HANDLER_H

#include "../../effective_address.h"
namespace cpu::instructions::execute {
    struct bcc_handler {
        inline static void execute(cpu& cpu) {
            if (do_branch(cpu.sr(), cpu.pc().get_current_opcode())) {
                byte_t displacement = static_cast<byte_t>(cpu.pc().get_current_opcode());
                cpu.pc().add_displacement(sign_extend(displacement));
            }
        }

        template<unsigned_integer src_t>
        inline static void execute(src_t src, cpu& cpu) {
            if (do_branch(cpu.sr(), cpu.pc().get_current_opcode())) {
                cpu.pc().add_displacement(sign_extend(src));
            }
        }

    private:
        inline static bool do_branch(const registers::status_register& sr, const opcode_t opcode) {
            bool result;
            switch (opcode & 0x0f00) {
                case 0x0000:
                    result = true;
                    break;
                case 0x0200:
                    result = sr.hi();
                    break;
                case 0x0300:
                    result = sr.ls();
                    break;
                case 0x0400:
                    result = sr.cc();
                    break;
                case 0x0500:
                    result = sr.cs();
                    break;
                case 0x0600:
                    result = sr.ne();
                    break;
                case 0x0700:
                    result = sr.eq();
                    break;
                case 0x0800:
                    result = sr.vc();
                    break;
                case 0x0900:
                    result = sr.vs();
                    break;
                case 0x0a00:
                    result = sr.pl();
                    break;
                case 0x0b00:
                    result = sr.mi();
                    break;
                case 0x0c00:
                    result = sr.ge();
                    break;
                case 0x0d00:
                    result = sr.lt();
                    break;
                case 0x0e00:
                    result = sr.gt();
                    break;
                case 0x0f00:
                    result = sr.le();
                    break;
                default:
                    assert(false);
                    throw std::runtime_error("bsr opcode encountered by bcc");
                    break;
            }
            return result;
        }
    };

    struct bsr_handler {
        inline static void execute(cpu& cpu) {
            ea::an_ind_pre_decr(7).write(cpu, cpu.pc().get_next_pc());
            byte_t displacement = static_cast<byte_t>(cpu.pc().get_current_opcode());
            cpu.pc().add_displacement(sign_extend(displacement));
        }

        template<unsigned_integer src_t>
        inline static void execute(src_t src, cpu& cpu) {
            ea::an_ind_pre_decr(7).write(cpu, cpu.pc().get_next_pc());
            cpu.pc().add_displacement(sign_extend(src));
        }
    };
}
#endif //BCC_HANDLER_H
