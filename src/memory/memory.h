//
// Created by zoso on 11/18/24.
//

#ifndef MEMORY_H
#define MEMORY_H
#include <memory>

#include "layout.h"
#include "../types.h"


namespace memory {
    class memory {
        public:
        void reserve(const layout &layout);
        ~memory() noexcept = default;

        template<typename T>
        void write(const address_t address, const T value) {
            bank& bank = bank_for_address(address);
            bank.write(address & mask<address_t, 16>, value);
        }

        template<typename T>
        T read(const address_t address) {
            bank& bank = bank_for_address(address);
            return bank.read<T>(address & mask<address_t, 16>);
        }

        [[nodiscard]] inline address_t get_mask() const {
            return _mask;
        }

    private:
        std::unique_ptr<bank[]> _banks;
        std::unique_ptr<char[]> _chip_ram;
        std::unique_ptr<char[]> _zorro_ii_ram;
        std::unique_ptr<char[]> _slow_fast_ram;
        std::unique_ptr<char[]> _rom;
        address_t _mask = mask<address_t, 24>;
        inline bank& bank_for_address(const address_t address) {
            return _banks[(address & _mask) >> 16];
        }
    };
}



#endif //MEMORY_H
