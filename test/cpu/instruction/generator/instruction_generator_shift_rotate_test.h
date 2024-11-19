//
// Created by Benjamin Landorff on 6/24/21.
//

#ifndef INSTRUCTION_GENERATOR_SHIFT_ROTATE_TEST_H
#define INSTRUCTION_GENERATOR_SHIFT_ROTATE_TEST_H

#include "instruction_generator_common.h"
#include "../../../../src/cpu/instructions/generate/shift_rotation_generator.h"

constexpr counter_t asx_count = 3*2*2*(8*8)+2*(5*8+2);
constexpr counter_t asx_dn_count =3*2*(8*8);
constexpr counter_t asx_implicit_count =3*2*(8*8);
constexpr counter_t asx_ea_count = 2*(5*8+2);

constexpr counter_t lsx_count = 3*2*2*(8*8)+2*(5*8+2);
constexpr counter_t lsx_dn_count =3*2*(8*8);
constexpr counter_t lsx_implicit_count =3*2*(8*8);
constexpr counter_t lsx_ea_count = 2*(5*8+2);

constexpr counter_t rox_count = 3*2*2*(8*8)+2*(5*8+2);
constexpr counter_t rox_dn_count =3*2*(8*8);
constexpr counter_t rox_implicit_count =3*2*(8*8);
constexpr counter_t rox_ea_count = 2*(5*8+2);

constexpr counter_t roxx_count = 3*2*2*(8*8)+2*(5*8+2);
constexpr counter_t roxx_dn_count =3*2*(8*8);
constexpr counter_t roxx_implicit_count =3*2*(8*8);
constexpr counter_t roxx_ea_count = 2*(5*8+2);


#endif //INSTRUCTION_GENERATOR_SHIFT_ROTATE_TEST_H
