//
// Created by zoso on 11/18/24.
//

#ifndef PROGRAM_COUNTER_H
#define PROGRAM_COUNTER_H

#include "../../types.h"
#include "../../memory/memory.h"

namespace cpu::registers {
    struct program_counter {
        explicit program_counter(memory::memory &mem) noexcept: _mem(mem) {}

        inline void set_pc(const address_t pc) { this->_pc = pc; }

        [[nodiscard]] inline address_t get_pc() const { return this->_pc; }

        [[nodiscard]] inline address_t get_next_pc() const { return this->_pc + this->_offset;}

        inline void add_displacement(const long_t displacement) {
            //this->_pc = (this->_pc + this->_offset + this->_offset+displacement) & this->_mem.get_mask();
            this->_pc = (this->_pc + displacement) & this->_mem.get_mask();
            this->prefetch();
            this->prefetch();
        }

        inline word_t fetch_extension_word() {
            const word_t old_irc = _irc;
            this->_offset += 2;
            this->_irc = _mem.read<opcode_t>(_pc + _offset);
            return old_irc;
        }

        [[nodiscard]] inline opcode_t get_current_opcode() const {
            return this->_ird;
        }

        inline opcode_t get_next_opcode() {
            this->do_prefetch();
            return this->_ird;
        }

        inline void prefetch() {
            this->_offset = 2;
            this->_ir = this->_irc;
            this->_irc = _mem.read<opcode_t>(_pc + _offset);
            this->_ird = this->_ir;
        }


    private:
        address_t _pc = 0;
        word_t _offset = 0;
        opcode_t _irc = 0;
        opcode_t _ir = 0;
        opcode_t _ird = 0;
        bool _initial = true;
        memory::memory &_mem;

        inline void initial_prefetch() {
            this->_irc = _mem.read<opcode_t>(_pc);
            this->prefetch();
        }

        inline void do_prefetch() {
            this->_pc += this->_offset;
            this->_offset = 0;
            if (this->_initial) {
                this->_initial = false;
                this->initial_prefetch();
            } else {
                this->prefetch();
            }
        }
    };

}
#endif //PROGRAM_COUNTER_H
