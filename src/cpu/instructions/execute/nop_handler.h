//
// Created by zoso on 11/22/24.
//

#ifndef NOP_HANDLER_H
#define NOP_HANDLER_H

namespace cpu::instructions::execute {
    struct nop_handler {
        static void execute(cpu& cpu) {}
    };
}

#endif //NOP_HANDLER_H
