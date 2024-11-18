//
// Created by zoso on 11/18/24.
//

#ifndef EXECUTION_PROPERTIES_H
#define EXECUTION_PROPERTIES_H
#include <compare>

namespace cpu::instructions {
    template<typename execution> constexpr bool reverse_src_dst =
        std::is_same_v<execute::add_ea, ec::add_ea>;
}
#endif //EXECUTION_PROPERTIES_H
