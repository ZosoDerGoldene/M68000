//
// Created by zoso on 11/18/24.
//

#ifndef CPU_H
#define CPU_H

#include <cassert>
#include "../memory/memory.h"
#include "registers/program_counter.h"
#include "registers/registers.h"
#include "registers/status_register.h"

namespace cpu {
    struct cpu {
        explicit cpu(memory::memory& mem) : _pc(registers::program_counter(mem)), _mem(mem) {
        }

        void reset() {
            // TODO: A reset is, of course, much more complicated... (and differs from model to model wrt rom etc.)
            _pc.set_pc(_mem.read<long_t>(0xfc0004));
        }


        registers::data_register& d(const unsigned i) {
            assert((i & 7) == i);
            return _d[i];
        }

        registers::address_register& a(const unsigned i) {
            assert((i & 7) == i);
            return _a[i];
        }

        registers::status_register& sr() { return _sr;}

        registers::program_counter& pc() { return _pc;}

        inline word_t fetch_extension_word() { return pc().fetch_extension_word();}

    private:
        registers::data_register _d[8];
        registers::address_register _a[8];
        registers::status_register _sr;
        registers::program_counter _pc;
        memory::memory& _mem;
    };

}



#endif //CPU_H
