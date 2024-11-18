//
// Created by zoso on 11/18/24.
//

#ifndef MEMORY_BANK_H
#define MEMORY_BANK_H
#include <functional>
#include <cassert>
#include <stdexcept>
#include "../types.h"

namespace memory {
    namespace {
        using row_t = unsigned long int;
        inline int calc_shift(address_t a) { return static_cast<int>(a & mask<address_t, 3>) << 3; }

        inline address_t row(address_t a) { return a >> 3; }

        template<typename T>
        inline bool is_overlapping_long(address_t address) {return (is_long<T>) && ((address & mask<address_t, 16>) ==
        0xFFFE);}

        template<typename T>
        inline bool is_odd_access(address_t address) {return (is_long<T> || is_word<T>) && is_odd(address);}
    }
    constexpr int bank_size = 0x10000;

    enum memory_type {
        reserved
        , chip_ram
        , zorro_ii
        , cias
        , slow_fast_ram
        , pseudo_fast_ram
        , chip_register_shadow
        , clock_port
        , ide_controller
        , scsi_ide_controller
        , real_time_clock
        , mainboard
        , custom
        , rom
        , auto_config
        , secondary_auto_config
        , diagnostic_rom
        , extended_rom
        , kickstart_rom
        , fast_ram
        , zorro_iii
        , type_max
    };
    struct bank;

    template<typename T>
    using fn_fetch = std::function<T(bank&, address_t)>;

    template<typename T>
    static fn_fetch<T> fetch_array[type_max];

    template<typename T>
    using fn_store=std::function<void(bank&, address_t, T)>;

    template<typename T>
    static fn_store<T> store_array[type_max];
    struct bank {
    public:
        bank() : bank(memory_type::reserved) {}
        explicit bank(memory_type type) : bank(type, nullptr) {}
        bank(memory_type type, char* mem) : _type(type), _mem(mem) {}

        inline memory_type get_type() { return _type; }
        inline char* get_mem() {return _mem;}

        template<typename T>
        void write(address_t address, const T value) {
            store_array<T>[_type](*this, address, value);
        }

        template<typename T>
        T read(address_t address) {
            return fetch_array<T>[_type](*this, address);
        }


    protected:
        memory_type _type;
        char* _mem;
    };

    template<typename derived_bank, memory_type bank_type, bool can_write = true>
    class bank_definition : public bank {
    public:
        static const memory_type _bank_type;
        template<typename T>
        static T call_fetch(bank& bank, address_t address) {
            return static_cast<derived_bank &>(bank).template read<T>(address);
        }
        template<typename T>
        static void call_store(bank& bank, address_t address, T value) {
            return static_cast<derived_bank &>(bank).template write<T>(address, value);
        }
        static constexpr memory_type reg() {
            fetch_array<byte_t>[bank_type] = call_fetch<byte_t>;
            fetch_array<sbyte_t>[bank_type] = call_fetch<sbyte_t>;
            fetch_array<word_t>[bank_type] = call_fetch<word_t>;
            fetch_array<sword_t>[bank_type] = call_fetch<sword_t>;
            fetch_array<long_t>[bank_type] = call_fetch<long_t>;
            fetch_array<slong_t>[bank_type] = call_fetch<slong_t>;
            store_array<byte_t>[bank_type] = call_store<byte_t>;
            store_array<sbyte_t>[bank_type] = call_store<sbyte_t>;
            store_array<word_t>[bank_type] = call_store<word_t>;
            store_array<sword_t>[bank_type] = call_store<sword_t>;
            store_array<long_t>[bank_type] = call_store<long_t>;
            store_array<slong_t>[bank_type] = call_store<slong_t>;
            return bank_type;
        }
        bank_definition() noexcept : bank(_bank_type) {}

        // Fallback store
        template<typename T>
        void write(address_t address, T value) {
            if constexpr(can_write) {
                assert(!is_odd_access<T>(address));
                assert(!is_overlapping_long<T>(address));
                T *real_mem = reinterpret_cast<T *>(_mem + address);
                if constexpr(is_byte<T>) {
                    *real_mem = value;
                } else if constexpr(is_word<T>) {
                    *real_mem = __builtin_bswap16(value);
                } else if constexpr(is_long<T>) {
                    *real_mem = __builtin_bswap32(value);
                }
            }
        }

        // Fallback fetch
        template<typename T>
        T read(address_t address) {
            assert(!is_odd_access<T>(address));
            assert(!is_overlapping_long<T>(address));
            T* real_mem = reinterpret_cast<T*>(_mem+address);
            if constexpr(is_byte<T>) {
                return *real_mem;
            } else if constexpr(is_word<T>) {
                return __builtin_bswap16(*real_mem);
            } else if constexpr(is_long<T>) {
                return __builtin_bswap32(*real_mem);
            }
            throw std::runtime_error("reach end of read()");
        }
    };

    template<typename Bank, memory_type bank_type, bool can_write>
    const memory_type bank_definition<Bank, bank_type, can_write>::_bank_type =
        bank_definition<Bank,bank_type,can_write>::reg();

    struct chip_ram_bank : public bank_definition<chip_ram_bank, memory_type::chip_ram> {};
    struct zorro_ii_ram_bank : public bank_definition<zorro_ii_ram_bank, memory_type::zorro_ii> {};
    struct slow_fast_ram_bank : public bank_definition<slow_fast_ram_bank, memory_type::slow_fast_ram> {};
    struct rom_bank : public bank_definition<rom_bank, memory_type::rom, false> {};

    namespace {
        // make sure all the bank types' function pointers are actually registered
        static chip_ram_bank cb;
        static zorro_ii_ram_bank zb;
        static slow_fast_ram_bank sfb;
        static rom_bank rb;
    }
}
#endif //MEMORY_BANK_H
