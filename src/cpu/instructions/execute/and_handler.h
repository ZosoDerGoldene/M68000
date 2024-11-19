//
// Created by zoso on 11/19/24.
//

#ifndef AND_HANDLER_H
#define AND_HANDLER_H

namespace cpu::instructions::execute {
    struct and_ {
        template<unsigned_integer src_t, unsigned_integer dest_t>
        static inline dest_t execute(const src_t src, const dest_t dest, execution_context& context) {
            registers::status_register& sr = context._cpu.sr();
            bool zero, negative;
            dest_t result = dest;
            asm ("and %3, %2":
            "=@ccz"(zero), "=@ccs"(negative), "+r"(result):"r"(src));
            sr.template set_cc<false>((sr.get_cc() & ~(0b1100)) | (zero << 2) | (negative << 3));
            return result;
        }
    };

    struct and_dn : public and_ {

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = ea::data<mode>;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = std::same_as<ea::dn, mode>;
    };

    struct and_ea : public and_ {

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = std::same_as<ea::dn, mode>;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::memory_alterable<mode>;
    };

    struct andi : public and_ {

        template<ea::ea_mode mode>
        static constexpr bool is_valid_source = std::same_as<ea::imm, mode>;

        template<ea::ea_mode mode>
        static constexpr bool is_valid_destination = ea::data_alterable<mode>;
    };

    struct andi2ccr {
        template<unsigned_integer src_t>
        static inline void execute(const src_t src, execution_context& context) {
            registers::status_register& sr = context._cpu.sr();
            sr.template set_cc<true>(sr.get_cc() & src);
        }
    };
}

#endif //AND_HANDLER_H
