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
    using instruction_fn = std::function<void(execution_context&)>;
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
        static void execution_wrapper(execution_context& context) {
            constexpr bool flip = execute::reverses_src_dst<handler>;
            src_mode s_mode = ea::make_mode<src_mode, !flip>(context._opcode);
            src_t src = ea::read<src_mode, src_t>(s_mode, context);
            if constexpr(execute::reads_destination<handler>) {
                dst_t dest =
                    read_dst<dst_t, dst_mode, execute::writes_destination<handler>, flip>(context);
                dst_t result = do_execute<handler>(src, dest, context);
                if constexpr (execute::writes_destination<handler>) {
                    write_dst<dst_mode, dst_t, flip>(context, result);
                }
            } else {
                dst_mode dest_rw_mode = ea::make_mode<dst_mode, flip>(context._opcode);
                dst_t result = do_execute<handler, src_t, dst_t>(src, context);
                ea::write(dest_rw_mode, context, result);
            }
        }

        /*
          * The templated wrapper of an execution taking one parameter
          * The handler parameter is the actual function being passed in
          * This wrapper handles reading from and writing to the execution context
          * Depending on the properties of the handler function, the src  might or might not be preserved
          */

        template<typename handler, unsigned_integer src_t, ea::ea_mode src_mode>
                    static void execution_wrapper(execution_context& context) {
            src_t src = read_dst<src_t, src_mode, execute::writes_destination<handler>, false>(context);
            if constexpr (execute::writes_destination<handler>) {
                src_t result;
                if constexpr(execute::ignores_source<handler>) {
                    result = do_execute<handler, src_t>(context);
                } else {
                    result = do_execute<handler, src_t>(src, context);
                }
                write_dest<src_mode, src_t, false>(context, result);
            } else {
                if constexpr(execute::ignores_source<handler>) {
                    handler::template execute<src_t>(context);
                } else {
                    handler::template execute(src, context);
                }
            }
        }

        /*
         * In case of a parameterless function, there is no wrapping to do
         */
        template<typename handler>
            static void execution_wrapper(execution_context& context) {
                handler::execute(context);
            }

        private:

            template<ea::ea_mode dst_mode, unsigned_integer dst_t, bool flip>
            static inline void write_dst(execution_context& context, dst_t result) {
                using write_mode = ea::write_mode<dst_mode>;
                write_mode dst_write_mode = ea::make_mode<write_mode, flip>(context._opcode);
                ea::write(dst_write_mode, context, result);
            }

            template<unsigned_integer dst_t, ea::ea_mode dst_mode, bool writes_destination, bool flip>
            static inline dst_t read_dst(execution_context& context) {
                dst_t dst;
                using read_mode = ea::read_mode<dst_mode>;
                if constexpr(writes_destination) {
                    read_mode dest_read_mode = ea::make_mode<read_mode, flip>(context._opcode);
                    dst = ea::read<read_mode, dst_t>(dest_read_mode, context);
                } else {
                    dst_mode dest_rw_mode = ea::make_mode<dst_mode, flip>(context._opcode);
                    dst = ea::read<dst_mode, dst_t>(dest_rw_mode, context);
                }
                return dst;
            }

            template<typename handler, unsigned_integer src_t, unsigned_integer dst_t>
            static inline dst_t do_execute(src_t src, dst_t dst, execution_context& context) {
                return handler::execute(src, dst, context);
            }

            template<typename handler, unsigned_integer src_t, unsigned_integer dst_t>
            static inline dst_t do_execute(const src_t src, execution_context& context) {
                return handler::template execute<src_t, dst_t>(src, context);
            }

            template<typename handler, unsigned_integer dst_t>
            static inline dst_t do_execute(dst_t dst, execution_context& context) {
                return handler::template execute<dst_t>(dst, context);
            }

            template<typename handler, unsigned_integer src_t>
            static inline src_t do_execute(execution_context& context) {
                return handler::template execute<src_t>(context);
            }

    };
}



#endif //INSTRUCTIONS_H
