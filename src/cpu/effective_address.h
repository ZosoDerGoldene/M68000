//
// Created by zoso on 11/18/24.
//

#ifndef EFFECTIVE_ADDRESS_H
#define EFFECTIVE_ADDRESS_H
#include "../types.h"
#include "cpu.h"

namespace cpu {

    struct execution_context {
        execution_context(cpu& c, memory::memory& m, const opcode_t opcode = 0) : _cpu(c), _memory(m), _opcode(opcode) {}
        cpu& _cpu;
        memory::memory& _memory;
        opcode_t _opcode;
    };

    namespace ea {
        struct register_based {
            explicit register_based(const opcode_t reg) noexcept : _register(reg) {}
        protected:
            opcode_t _register;
        };

        namespace {
            template<typename T>
            constexpr opcode_t offset() {
                if constexpr(is_long<T>) {
                    return 4;
                } else if constexpr(is_word<T>) {
                    return 2;
                } else if constexpr(is_byte<T>) {
                    return 1;
                }
            }

            template<bool brief_extension_word>
            inline address_t calc_index(cpu& c, const word_t extension_word) {
                const word_t reg_num = (extension_word & 0b0111000000000000) >> 12;
                address_t index;
                if (extension_word & (1 << 15)) {
                    const registers::address_register reg = c.a(reg_num);
                    if (extension_word & 0b100000000000) {
                        index = reg.l();
                    } else {
                        index = sign_extend<word_t>(reg.w());
                    }
                } else {
                    const registers::data_register reg = c.d(reg_num);
                    if (extension_word & 0b100000000000) {
                        index = reg.l();
                    } else {
                        index = sign_extend<word_t>(reg.w());
                    }
                }
                if constexpr(!brief_extension_word) {
                    // scale
                    index = index << ((extension_word & (0b11 << 9)) >> 9);
                }
                return index;
            }

            template<typename T>
            address_t get_displacement(cpu &c) {
                address_t displacement = 0;
                if constexpr (is_long<T>) {
                    displacement = static_cast<address_t>(c.fetch_extension_word()) << 16;
                    displacement |= static_cast<address_t>(c.fetch_extension_word());
                } else if constexpr(is_word<T>) {
                    displacement = sign_extend<word_t>(c.fetch_extension_word());
                }
                return displacement;
            }

            inline address_t
            handle_full_extension_word(const address_t base_address, cpu &cpu, const word_t extension_word) {
                memory::memory& mem = cpu.get_memory();
                const bool base_suppress = extension_word & (1 << 7);
                const bool index_suppress = extension_word & (1 << 6);
                const opcode_t bd_size = extension_word & (0b11 << 4);
                const opcode_t index_indirect_select = extension_word & 0b111;
                address_t address;
                if (base_suppress) {
                    address = 0;
                } else {
                    address = base_address;
                }
                switch (bd_size) {
                    case 0b000000:
                        //throw new std::runtime_error("reserved base_displacement_size");
                            break;
                    case 0b100000:
                        address += get_displacement<word_t>(cpu);
                    break;
                    case 0b110000: {
                        address += get_displacement<address_t>(cpu);
                    }
                    break;
                    default:
                    case 0b010000:
                        //nothing to do here, null base displacement
                        break;
                }
                switch (index_indirect_select) {
                    case 0b000:
                        // no memory indirect action
                            if (index_suppress) {
                                // nothing more to do
                                return address;
                            } else {
                                address += calc_index<false>(cpu, extension_word);
                            }
                    break;
                    case 0b001:
                        // null outer displacement
                            if (index_suppress) {
                                // read from memory
                            } else {
                                // add scaled index, read from memory
                                address += calc_index<false>(cpu, extension_word);
                                address = mem.read<address_t>(address);
                            }
                    break;
                    case 0b010:
                        // word outer displacement
                            if (index_suppress) {
                                // read from memory, add word outer displacment
                                address = mem.read<address_t>(address);
                                address += sign_extend<opcode_t>(cpu.fetch_extension_word());
                            } else {
                                address += calc_index<false>(cpu, extension_word);
                                address = mem.read<address_t>(address);
                                address += sign_extend<opcode_t>(cpu.fetch_extension_word());
                                // add scaled index, read from memory, add word outer displacement
                            }
                    break;
                    case 0b011: {
                        // long outer displacement
                        if (index_suppress) {
                            address = mem.read<address_t>(address);
                            // read from memory, add long outer displacment
                        } else {
                            address += calc_index<false>(cpu, extension_word);
                            address = mem.read<address_t>(address);
                            // add scaled index, read from memory, add long outer displacement
                        }
                        long_t outer_displacement = get_displacement<long_t>(cpu);
                        address += outer_displacement;
                    }
                    break;
                    case 0b100:
                        throw std::runtime_error("reserved index_indirect_select");
                    break;
                    case 0b101:
                        if (index_suppress) {
                            throw std::runtime_error("reserved index_indirect_select");
                        } else {
                            // read from memory, add scaled index
                            address = mem.read<address_t>(address);
                            address += calc_index<false>(cpu, extension_word);
                        }
                    break;
                    case 0b110:
                        if (index_suppress) {
                            throw std::runtime_error("reserved index_indirect_select");
                        } else {
                            // read from memory, add scaled index, add word outer displacement
                            address = mem.read<address_t>(address);
                            address += calc_index<false>(cpu, extension_word);
                            address += sign_extend<opcode_t>(cpu.fetch_extension_word());
                        }
                    break;
                    case 0b111:
                        if (index_suppress) {
                            throw std::runtime_error("reserved index_indirect_select");
                        } else {
                            // read from memory, add scaled index, add long outer displacment
                            address = mem.read<address_t>(address);
                            address += calc_index<false>(cpu, extension_word);
                            address += get_displacement<long_t>(cpu);;
                        }
                    break;
                    default:
                        throw std::runtime_error("Control fell through");
                }
                return address;
            }

            template<bool brief_extension_word>
            inline address_t handle_brief_extension_word(
                address_t base_address, cpu& cpu, word_t extension_word) {
                slong_t displacement = sign_extend<byte_t>(static_cast<byte_t>(extension_word));
                address_t index = calc_index<brief_extension_word>(cpu, extension_word);
                return base_address + displacement + index;
            }

            // "brief extension word" here refers to processors that are only able to process brief extension words,
            // i.e. 68000, 68008, 68010. We can't know what the extension word's going to be before we've seen it,
            // but the aforementioned processors would not only not recognize a full extension word, they're also
            // going to choke on the "scale" part in the brief extension word.
            template<bool brief_extension_word>
            inline address_t calc_extension_word_address(address_t base_address, cpu& cpu) {
                word_t extension_word = cpu.fetch_extension_word();
                if constexpr (brief_extension_word) {
                    return handle_brief_extension_word<true>(base_address, cpu, extension_word);
                } else {
                    if (extension_word & 0b100000000) {
                        //full extension word
                        return handle_full_extension_word(base_address, cpu, extension_word);
                    } else {
                        // brief extension word
                        return handle_brief_extension_word<false>(base_address, cpu, extension_word);
                    }
                }
            }
        }

        struct dn : public register_based {
            constexpr static opcode_t mask = 0b000;

            explicit dn(opcode_t reg) noexcept : register_based(reg) {}

            template<typename T>
            inline T read(cpu& cpu) const { return cpu.d(_register). template read<T>();}

            template<typename T>
            inline void write(cpu& cpu, T value) { cpu.d(_register) = value; }
        };

        struct an : public register_based {
            constexpr static opcode_t mask = 0b001;

            explicit an(opcode_t reg) noexcept : register_based(reg) {}

            template<typename T>
            inline T read(cpu& cpu) { return cpu.a(_register). template read<T>();}

            template<typename T>
            inline void write(cpu& cpu, T value) { cpu.a(_register) = value; }
        };

        struct an_ind : public register_based {
            constexpr static opcode_t mask = 0b010;

            explicit an_ind(opcode_t reg) noexcept : register_based(reg) {}

            template<typename T>
            inline T read(cpu& cpu) {
                const auto address = cpu.a(_register). template read<address_t>();
                return cpu.get_memory().read<T>(address);
            }

            template<typename T>
            inline void write(cpu& cpu, T value) {
                auto address = cpu.a(_register). template read<address_t>();
                return cpu.get_memory().write(address, value);
            }
        };

        struct an_ind_post_incr : public register_based {
            constexpr static opcode_t mask = 0b011;

            explicit an_ind_post_incr(const opcode_t reg) noexcept : register_based(reg) {}

            template<typename T>
            inline T read(cpu& cpu) {
                const address_t address = calc_address<T>(cpu);
                return cpu.get_memory().read<T>(address);
            }

            template<typename T>
            inline void write(cpu& cpu, T value) {
                address_t address = calc_address<T>(cpu);
                return cpu.get_memory().write(address, value);
            }

        private:
            template<typename T>
            inline address_t calc_address(cpu& cpu) const {
                auto address = cpu.a(_register). template read<address_t>();
                if constexpr(is_byte<T>) {
                    if (_register == 7) {
                        cpu.a(_register) = address + (offset<T>() << 1);
                    } else {
                        cpu.a(_register) = address + offset<T>();
                    }
                } else {
                    cpu.a(_register) = address + offset<T>();
                }
                return address;
            }
        };

        struct an_ind_pre_decr : public register_based {
            constexpr static opcode_t mask = 0b100;

            explicit an_ind_pre_decr(opcode_t reg) noexcept : register_based(reg) {}

            template<typename T>
            inline T read(cpu& cpu) {
                const address_t address = calc_address<T>(cpu);
                return cpu.get_memory().read<T>(address);
            }

            template<typename T>
            inline void write(cpu& cpu, T value) {
                address_t address = calc_address<T>(cpu);
                return cpu.get_memory().write(address, value);
            }

        private:
            template<typename T>
            inline address_t calc_address(cpu& cpu) const {
                auto address = cpu.a(_register).template
                    read<address_t>();
                if constexpr(is_byte<T>) {
                    if (_register == 7) {
                        address -= offset<T>() * 2;
                    } else {
                        address -= offset<T>();
                    }
                } else {
                    address -= offset<T>();
                }
                cpu.a(_register) = address;
                return address;
            }

        };

        struct an_ind_disp : public register_based {
            constexpr static opcode_t mask = 0b101;

            explicit an_ind_disp(const opcode_t reg) noexcept : register_based(reg) {}

            template<typename T>
            inline T read(cpu& cpu) {
                const address_t address = this->calc_address(cpu);
                return cpu.get_memory().read<T>(address);
            }

            template<typename T>
            inline void write(cpu& cpu, T value) {
                address_t address = this->calc_address(cpu);
                return cpu.get_memory().write(address, value);
            }
        private:
            inline address_t calc_address(cpu& c) const {
                const auto address = c.a(_register). template read<address_t>();
                const address_t displacement = sign_extend(c.pc().fetch_extension_word());
                return address + displacement;
            }
        };

        template<bool brief_extension_word = true>
        struct an_ind_idx : public register_based {
            constexpr static opcode_t mask = 0b110;

            explicit an_ind_idx(const opcode_t reg) : register_based(reg) {}

            template<typename T>
            inline T read(cpu& cpu) {
                const address_t address = calc_extension_word_address<brief_extension_word>(cpu.a(_register).l(),
                                                                                            cpu);
                return cpu.get_memory(). read<T>(address);
            }

            template<typename T>
            inline void write(cpu& cpu, T value) {
                address_t address = calc_extension_word_address<brief_extension_word>(cpu.a(_register).l(),
                                                                                      cpu);
                return cpu.get_memory(). write(address, value);
            }
        };

        struct abs_short {
            constexpr static opcode_t reg = 0b000;

            template<typename T>
            static inline T read(cpu& cpu) {
                const address_t address = calc_address(cpu);
                return cpu.get_memory().template read<T>(address);
            }

            template<typename T>
            static inline void write(cpu& cpu, T value) {
                address_t address = calc_address(cpu);
                cpu.get_memory(). template write(address, value);
            }

        private:
            static inline address_t calc_address(cpu& c) { return sign_extend<word_t>(c.fetch_extension_word());}
        };

        struct abs_long {
            constexpr static opcode_t reg = 0b001;

            template<typename T>
            static inline T read(cpu& cpu) {
                const address_t address = calc_address(cpu);
                return cpu.get_memory(). template read<T>(address);
            }

            template<typename T>
            static inline void write(cpu& cpu, T value) {
                address_t address = calc_address(cpu);
                cpu.get_memory(). template write<T>(address, value);
            }
        private:
            static inline address_t calc_address(cpu& c) {
                return
                (static_cast<address_t>(c.fetch_extension_word()) << 16) |
                static_cast<address_t>(c.fetch_extension_word());
            }
        };

        struct pc_ind_disp {
            constexpr static opcode_t reg = 0b010;

            template<typename T>
            static inline T read(cpu& cpu) {
                address_t address = cpu.pc().get_pc();
                address += sign_extend<word_t>(cpu.fetch_extension_word());
                return cpu.get_memory().template read<T>(address);
            }
        };

        template<bool brief_extension_word = true>
        struct pc_ind_idx {
            constexpr static opcode_t reg = 0b011;

            template<typename T>
            static inline T read(cpu& cpu) {
                // TODO: Verify whether get_pc() or get_next_pc() is correct
                const address_t address = calc_extension_word_address<brief_extension_word>(cpu.pc().get_pc(),
                                                                                            cpu);
                return cpu.get_memory().template read<T>(address);
            }
        };

        struct imm {
            constexpr static opcode_t reg = 0b100;

            template<typename T>
            inline T read(cpu& cpu) {
                if constexpr(is_long<T>) {
                    return
                        (static_cast<long_t>(cpu.fetch_extension_word()) << 16) |
                        static_cast<long_t>(cpu.fetch_extension_word());
                } else if constexpr(is_word<T>) {
                    return cpu.fetch_extension_word();
                } else if constexpr(is_byte<T>){
                    return static_cast<T>(cpu.fetch_extension_word());
                }
            }
        };

        template<typename mode>
        concept register_mode = std::is_base_of_v<register_based, mode>;


        template<typename mode>
        concept registerless_mode = std::same_as<imm, mode> || std::same_as<pc_ind_disp, mode> ||
                                  std::same_as<pc_ind_idx<true>, mode> || std::same_as<pc_ind_idx<false>, mode> ||
                                  std::same_as<abs_long, mode> || std::same_as<abs_short, mode>;


        template<typename mode>
        concept ea_mode =
            (register_mode<mode> || registerless_mode<mode>) &&
            requires (mode m, cpu& cpu) {
            {m.template read<byte_t>(cpu)} -> std::same_as<byte_t>;
            {m.template read<word_t>(cpu)} -> std::same_as<word_t>;
            {m.template read<long_t>(cpu)} -> std::same_as<long_t>;
            };

        template<typename mode>
        concept data = ea_mode<mode> && !std::same_as<an, mode>;

        template<typename mode>
        concept is_memory = ea::data<mode> && !std::same_as<dn, mode>;

        template<typename mode>
        concept control = ea::is_memory<mode> && !std::same_as<an_ind_post_incr, mode> &&
                                    !std::same_as<an_ind_pre_decr, mode> && !std::same_as<imm, mode>;

        template<typename mode>
        concept alterable = register_mode<mode> || std::same_as<abs_short, mode> || std::same_as<abs_long, mode>;

        template<typename mode>
        concept data_alterable = data<mode> && alterable<mode>;

        template<typename mode>
        concept memory_alterable = is_memory<mode> && alterable<mode>;

        template<typename mode, bool is_source>
        static inline mode make_mode(opcode_t opcode) {
            if constexpr(ea::register_mode<mode>) {
                if constexpr(is_source) {
                    return mode(opcode & 7);
                } else {
                    return mode((opcode >> 9) & 7);
                }
            } else {
                return mode();
            }
        }

        template<ea_mode mode, integer T>
        static inline T read(mode m, cpu& cpu) {
            static_assert(ea_mode<mode>);
            return m. template read<T>(cpu);
        }

        template<alterable mode, integer T>
        static inline void write(mode m, cpu& cpu, T value) {
            m.template write<T>(cpu, value);
        }

        template<typename mode>
        using read_mode = typename std::conditional<std::same_as<mode, an_ind_post_incr>, an_ind,
        mode>::type;

        template<typename mode>
        using write_mode = typename std::conditional<std::same_as<mode, an_ind_pre_decr>, an_ind,
            mode>::type;

        template<typename mode, bool source>
        constexpr opcode_t mode_mask() {
            static_assert(registerless_mode<mode>);
            if constexpr (source) {
                return (0b111 << 3) | mode::reg;
            } else {
                return (0b111 << 6) | (mode::reg << 9);
            }
        }

        template<typename mode, bool source>
        constexpr std::array<opcode_t, 8> mode_masks() {
            static_assert(register_mode<mode>);
            std::array<opcode_t, 8> result = std::array<opcode_t, 8>();
            for (opcode_t i = 0; i < 8; i++) {
                if constexpr (source) {
                    result[i] = i | (mode::mask << 3);
                } else {
                    result[i] = (i << 9) | (mode::mask << 6);
                }
            }
            return result;
        }
    }
}

#endif //EFFECTIVE_ADDRESS_H
