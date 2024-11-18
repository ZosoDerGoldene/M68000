//
// Created by zoso on 11/18/24.
//

#ifndef LAYOUT_H
#define LAYOUT_H
#include "../types.h"
#include "memory_bank.h"
namespace memory {
    using min_max = std::pair<ram_size_t, ram_size_t>;

    [[maybe_unused]] static bool check_size(const ram_size_t value, const min_max &bounds) {
        return ((value < bounds.first) || (value > bounds.second));
    }

    struct layout_descriptor {
        layout_descriptor(ram_size_t count, memory_type type) : _count(count), _type(type) {}
        [[nodiscard]] ram_size_t get_count() const {return _count;}
        [[nodiscard]] memory_type get_type() const {return _type;}
    private:
        ram_size_t _count;
        memory_type _type;
    };

    struct layout {
    public:
        virtual ~layout() = default;

        layout(const bool is_24bit, std::string rom_file_name) : _24bit(is_24bit), _rom_file_name(std::move(rom_file_name)) {}
        [[nodiscard]] bool is_24bit() const {return _24bit;}
        [[nodiscard]] ram_size_t get_num_banks() const { return _24bit ? 0x100 : 0x10000;}
        [[nodiscard]] virtual std::vector<layout_descriptor> get_bank_list() const = 0;
        [[nodiscard]] virtual ram_size_t get_number_of_rom_banks() const  = 0;
    private:
        bool _24bit;
        std::string _rom_file_name;
    };

    namespace amiga_500 {
        struct a500_layout final : public layout {
            a500_layout(ram_size_t chip_mem_banks, ram_size_t zorro_ii_banks, ram_size_t slow_fast_banks);

            static constexpr min_max chip_mem_min_max = min_max(8, 16);
            static constexpr min_max zorro_ii_min_max = min_max(0, 128);
            static constexpr min_max slow_fast_min_max = min_max(0, 24);

            [[nodiscard]] std::vector<layout_descriptor> get_bank_list() const override;

        private:
            ram_size_t _chip_mem_banks;
            ram_size_t _zorro_ii_banks;
            ram_size_t _slow_fast_banks;

            [[nodiscard]] ram_size_t get_number_of_rom_banks() const override;
        };
    }
}



#endif //LAYOUT_H
