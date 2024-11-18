//
// Created by zoso on 11/18/24.
//

#include "layout.h"
namespace memory::amiga_500 {
    /*
     * Amiga 500
     */
    std::vector<layout_descriptor> a500_layout::get_bank_list() const {
        std::vector<layout_descriptor> result;
        result.emplace_back(_chip_mem_banks, memory_type::chip_ram);
        if (_zorro_ii_banks > 0) {
            result.emplace_back(32 - _chip_mem_banks, memory_type::reserved);
            result.emplace_back(_zorro_ii_banks, memory_type::zorro_ii);
            if (_zorro_ii_banks < 128) {
                result.emplace_back(159 - _zorro_ii_banks, memory_type::reserved);
            }
        } else {
            result.emplace_back(191 - _chip_mem_banks, memory_type::reserved);
        }
        result.emplace_back(1, memory_type::cias);
        if (_slow_fast_banks > 0) {
            result.emplace_back(_slow_fast_banks, memory_type::slow_fast_ram);
        }
        result.emplace_back(28 - _slow_fast_banks, memory_type::reserved);
        result.emplace_back(1, memory_type::real_time_clock);
        result.emplace_back(1, memory_type::reserved);
        result.emplace_back(1, memory_type::mainboard);
        result.emplace_back(1, memory_type::custom);
        result.emplace_back(8, memory_type::reserved);
        result.emplace_back(8, memory_type::auto_config);
        result.emplace_back(12, memory_type::reserved);
        result.emplace_back(4, memory_type::rom);
        return result;
    }

    a500_layout::a500_layout(ram_size_t chip_mem_banks, ram_size_t zorro_ii_banks, ram_size_t slow_fast_banks) :
    layout(true, ""),
    _chip_mem_banks(chip_mem_banks), _zorro_ii_banks(zorro_ii_banks), _slow_fast_banks(slow_fast_banks) {
        if (check_size(chip_mem_banks, chip_mem_min_max)) {
            throw std::invalid_argument("a500_layout::_chip_ram_banks");
        }
        if (check_size(zorro_ii_banks, zorro_ii_min_max)) {
            throw std::invalid_argument("a500_layout::_zorro_ii_banks");
        }
        if (check_size(slow_fast_banks, slow_fast_min_max)) {
            throw std::invalid_argument("a500_layout::_slow_fast_banks");
        }
    }

    ram_size_t a500_layout::get_number_of_rom_banks() const { return 4; }
}