//
// Created by zoso on 11/18/24.
//

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <functional>
#include "../ea/effective_address.h"
#include "execute/execution_handlers.h"

namespace cpu::instructions {
    // This is a pointer to the individual functions tied opcodes
    // These functions get an execution context (memory and cpu) to read from and write to
    using instruction_fn = std::function<void(cpu&)>;
    // an array of all the possible functions to execute, organized by opcode
    // using the opcode, we can immediately fetch the function that knows what to do, once we initialized the array
    using instructions_t = std::array<instruction_fn, 0x10000>;

    struct instructions {
        // entry point to generate all the functions that mapped onto opcodes
        static void generate_instructions(instructions_t&);

        /*
         * The templated wrapper of an execution taking two parameters
         * The handler parameter is the actual function being passed in
         * This wrapper handles reading from and writing to the execution context
         */

        template<typename handler, unsigned_integer src_t, unsigned_integer dst_t, ea::ea_mode src_mode, ea::ea_mode dst_mode>
        static void execution_wrapper(cpu& cpu) {
            constexpr bool flip = execute::reverses_src_dst<handler>;
            // TODO: There is a potential speed hack if we enumerate all the Dn, An modes literally
            // So we don't have to analyze the opcode while executing instructions
            // This will, however, lead to many more wrapper functions and will significantly reduce
            // performance; It might be worth to examine this more closely, though;
            // even if it proves as bad as expected to do this for every instruction,
            // it might be worth doing for some select hotspots
            src_mode s_mode = ea::make_mode<src_mode, !flip>(cpu.pc().get_current_opcode());
            src_t src = ea::read<src_mode, src_t>(s_mode, cpu);
            if constexpr(execute::reads_destination<handler>) {
                dst_t dest =
                    read_dst<dst_t, dst_mode, execute::writes_destination<handler>, flip>(cpu);
                dst_t result = do_execute<handler>(src, dest, cpu);
                if constexpr (execute::writes_destination<handler>) {
                    write_dst<dst_mode, dst_t, flip>(cpu, result);
                }
            } else {
                dst_mode dest_rw_mode = ea::make_mode<dst_mode, flip>(cpu.pc().get_current_opcode());
                dst_t result = do_execute<handler, src_t, dst_t>(src, cpu);
                ea::write(dest_rw_mode, cpu, result);
            }
        }

        /*
          * The templated wrapper of an execution taking one parameter
          * The handler parameter is the actual function being passed in
          * This wrapper handles reading from and writing to the execution context
          * Depending on the properties of the handler function, the src  might or might not be preserved
          */

        template<typename handler, unsigned_integer src_t, ea::ea_mode src_mode>
                    static void execution_wrapper(cpu& cpu) {
            src_t src = read_dst<src_t, src_mode, execute::writes_destination<handler>, false>(cpu);
            if constexpr (execute::writes_destination<handler>) {
                src_t result;
                if constexpr(execute::ignores_source<handler>) {
                    result = do_execute<handler, src_t>(cpu);
                } else {
                    result = do_execute<handler, src_t>(src, cpu);
                }
                write_dest<src_mode, src_t, false>(cpu, result);
            } else {
                if constexpr(execute::ignores_source<handler>) {
                    handler::template execute<src_t>(cpu);
                } else {
                    handler::template execute(src, cpu);
                }
            }
        }

        /*
         * In case of a parameterless function, there is no wrapping to do
         */
        template<typename handler>
            static void execution_wrapper(cpu& cpu) {
                handler::execute(cpu);
            }

        private:

            template<ea::ea_mode dst_mode, unsigned_integer dst_t, bool flip>
            static inline void write_dst(cpu& cpu, dst_t result) {
                using write_mode = ea::write_mode<dst_mode>;
                write_mode dst_write_mode = ea::make_mode<write_mode, flip>(cpu.pc().get_current_opcode());
                ea::write(dst_write_mode, cpu, result);
            }

            template<unsigned_integer dst_t, ea::ea_mode dst_mode, bool writes_destination, bool flip>
            static inline dst_t read_dst(cpu& cpu) {
                dst_t dst;
                using read_mode = ea::read_mode<dst_mode>;
                if constexpr(writes_destination) {
                    read_mode dest_read_mode = ea::make_mode<read_mode, flip>(cpu.pc().get_current_opcode());
                    dst = ea::read<read_mode, dst_t>(dest_read_mode, cpu);
                } else {
                    dst_mode dest_rw_mode = ea::make_mode<dst_mode, flip>(cpu.pc().get_current_opcode());
                    dst = ea::read<dst_mode, dst_t>(dest_rw_mode, cpu);
                }
                return dst;
            }

            template<typename handler, unsigned_integer src_t, unsigned_integer dst_t>
            static inline dst_t do_execute(src_t src, dst_t dst, cpu& cpu) {
                return handler::execute(src, dst, cpu);
            }

            template<typename handler, unsigned_integer src_t, unsigned_integer dst_t>
            static inline dst_t do_execute(const src_t src, cpu& cpu) {
                return handler::template execute<src_t, dst_t>(src, cpu);
            }

            template<typename handler, unsigned_integer dst_t>
            static inline dst_t do_execute(dst_t dst, cpu& cpu) {
                return handler::template execute<dst_t>(dst, cpu);
            }

            template<typename handler, unsigned_integer src_t>
            static inline src_t do_execute(cpu& cpu) {
                return handler::template execute<src_t>(cpu);
            }

    };
}



#endif //INSTRUCTIONS_H
