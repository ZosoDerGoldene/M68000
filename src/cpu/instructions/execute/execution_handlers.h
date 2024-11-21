//
// Created by zoso on 11/18/24.
//

#ifndef EXECUTION_HANDLERS_H
#define EXECUTION_HANDLERS_H

#include <type_traits>

#include "add_handler.h"
#include "and_handler.h"
#include "asx_handler.h"
#include "bcc_handler.h"
#include "bcd_handler.h"
#include "eor_handler.h"
#include "lea_handler.h"
#include "lsx_handler.h"
#include "move_handler.h"
#include "or_handler.h"
#include "rox_handler.h"
#include "sub_handler.h"

namespace cpu::instructions::execute {

        template<typename handler>
        constexpr bool reverses_src_dst =
            std::is_same_v<add_ea_handler, handler>
            || std::is_same_v<addq_handler, handler>
            || std::is_same_v<subq_handler, handler>
            || std::is_same_v<sub_ea_handler, handler>
            || std::is_same_v<and_ea_handler, handler>
            || std::is_same_v<or_ea_handler, handler>
            || std::is_same_v<eor_ea_handler, handler>
            || std::is_same_v<asx_ea_handler<true>, handler>
            || std::is_same_v<asx_ea_handler<false>, handler>
            || std::is_same_v<lsx_ea_handler<true>, handler>
            || std::is_same_v<lsx_ea_handler<false>, handler>
            || std::is_same_v<rox_ea_handler<true, false>, handler>
            || std::is_same_v<rox_ea_handler<false, false>, handler>
            || std::is_same_v<rox_ea_handler<true, true>, handler>
            || std::is_same_v<rox_ea_handler<false, true>, handler>;

        template<typename handler>
        constexpr bool reads_destination =
        !((std::is_same_v<move_handler, handler>) || (std::is_same_v<movea_handler, handler>) ||
        (std::is_same_v<moveq_handler, handler>));

        template<typename handler>
        constexpr bool writes_destination =
        !(std::is_same_v<andi2ccr_handler, handler> || std::is_same_v<ori2ccr_handler, handler>
        ||    std::is_same_v<eori2ccr_handler, handler>
        ||         std::is_same_v<bcc_handler, handler> || std::is_same_v<bsr_handler, handler>);

        template<typename handler>
        constexpr bool ignores_source = std::is_same_v<moveq_handler, handler>;

        template<typename handler>
        constexpr bool doesnt_read = std::is_same_v<lea_handler, handler>;

        template<typename handler>
        constexpr bool is_immediate_instruction =
            std::is_same_v<addi_handler, handler>
            //|| //std::is_same_v<bchg_i_handler, handler> ||
            || std::is_same_v<andi_handler, handler>
            // || //std::is_same_v<bclr_i_handler, handler> ||
            || std::is_same_v<eori_handler, handler>
            // //std::is_same_v<bset_i_handler, handler> ||
            || std::is_same_v<ori_handler, handler>
            //  || std::is_same_v<btst_i_handler, handler>
            || std::is_same_v<subi_handler, handler>;

        template<typename handler>
        constexpr bool is_bit_instruction = false;
//            std::is_same_v<bchg_handler, handler> || std::is_same_v<bchg_i_handler, handler> ||
//            std::is_same_v<bclr_handler, handler> || std::is_same_v<bclr_i_handler, handler> ||
//            std::is_same_v<bset_handler, handler> || std::is_same_v<bset_i_handler, handler> ||
//            std::is_same_v<btst_handler, handler> || std::is_same_v<btst_i_handler, handler>;
}
#endif //EXECUTION_HANDLERS_H
