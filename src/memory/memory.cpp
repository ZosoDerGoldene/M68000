//
// Created by zoso on 11/18/24.
//

#include "memory.h"
#include <cassert>
namespace memory {
       void memory::reserve(const layout& layout) {
           const ram_size_t num_banks = layout.get_num_banks();
        _mask = num_banks == 256 ? mask<address_t, 24> : mask<address_t, 32>;
        _banks = std::make_unique<bank[]>(num_banks);
        std::vector<layout_descriptor> bank_list = layout.get_bank_list();
        ram_size_t i = 0;
        for (auto & it : bank_list) {
            char* real_mem_p;
            ram_size_t count = it.get_count();
            ram_size_t size = (count * bank_size)+8;
            switch (it.get_type()) {
                case memory_type::chip_ram:
                    _chip_ram = std::make_unique<char[]>(size);
                    real_mem_p = _chip_ram.get();
                    break;
                case memory_type::zorro_ii:
                    _zorro_ii_ram = std::make_unique<char[]>(size);
                    real_mem_p = _zorro_ii_ram.get();
                    break;
                case memory_type::slow_fast_ram:
                    _slow_fast_ram = std::make_unique<char[]>(size);
                    real_mem_p = _slow_fast_ram.get();
                    break;
                case memory_type::rom:
                {
                    _rom = std::make_unique<char[]>(size);
                    real_mem_p = _rom.get();
                    // std::ifstream in_file;
                    // in_file.open(""
                    //              , std::ios::binary);
                    // if (in_file.is_open()) {
                    //     in_file.read(real_mem_p, 4 * bank_size);
                    //     in_file.close();
                    // }
                }
                    break;
                default:
                    real_mem_p = nullptr;
                    break;
            }
            for (unsigned int j = 0; j < count; j++, i++) {
                char* bank_base_p = (real_mem_p == nullptr) ? nullptr : real_mem_p + (j * bank_size);
                _banks.get()[i] = bank(it.get_type(), bank_base_p);
            }
        }
        assert(i == layout.get_num_banks());
    }
}