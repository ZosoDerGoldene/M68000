//
// Created by zoso on 11/22/24.
//

#ifndef CMP_HANDLER_H
#define CMP_HANDLER_H

#include "sub_handler.h"
namespace cpu::instructions::execute {

    struct cmp_handler : sub_dn_handler {};

    struct cmpa_handler : suba_handler {};

    struct cmpi_handler : subi_handler{};
}

#endif //CMP_HANDLER_H
