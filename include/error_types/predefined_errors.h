/*
MIT License

Copyright (c) 2022 Danis Ozdemir

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __DEFINE_ERROR_T
#error __DEFINE_ERROR_T should have been defined, before including this header.
#endif

__DEFINE_ERROR_T(generic_error)
__DEFINE_ERROR_T(null_pointer)
__DEFINE_ERROR_T(inaccessible_non_null_pointer)
__DEFINE_ERROR_T(index_out_of_bounds)

// The rest has shamelessly been stolen from the boost error_code header
// https://www.boost.org/doc/libs/1_60_0/boost/system/error_code.hpp
// with slight modifications.

__DEFINE_ERROR_T(address_family_not_supported)
__DEFINE_ERROR_T(address_in_use)
__DEFINE_ERROR_T(address_not_available)
__DEFINE_ERROR_T(already_connected)
__DEFINE_ERROR_T(already_exists)
__DEFINE_ERROR_T(argument_list_too_long)
__DEFINE_ERROR_T(argument_out_of_domain)
__DEFINE_ERROR_T(bad_address)
__DEFINE_ERROR_T(bad_file_descriptor)
__DEFINE_ERROR_T(bad_message)
__DEFINE_ERROR_T(broken_pipe)
__DEFINE_ERROR_T(connection_aborted)
__DEFINE_ERROR_T(connection_already_in_progress)
__DEFINE_ERROR_T(connection_refused)
__DEFINE_ERROR_T(connection_reset)
__DEFINE_ERROR_T(cross_device_link)
__DEFINE_ERROR_T(destination_address_required)
__DEFINE_ERROR_T(device_or_resource_busy)
__DEFINE_ERROR_T(directory_not_empty)
__DEFINE_ERROR_T(executable_format_error)
__DEFINE_ERROR_T(file_too_large)
__DEFINE_ERROR_T(filename_too_long)
__DEFINE_ERROR_T(function_not_supported)
__DEFINE_ERROR_T(host_unreachable)
__DEFINE_ERROR_T(identifier_removed)
__DEFINE_ERROR_T(illegal_byte_sequence)
__DEFINE_ERROR_T(inappropriate_io_control_operation)
__DEFINE_ERROR_T(interrupted)
__DEFINE_ERROR_T(invalid_argument)
__DEFINE_ERROR_T(invalid_seek)
__DEFINE_ERROR_T(io_error)
__DEFINE_ERROR_T(is_a_directory)
__DEFINE_ERROR_T(message_size_not_right)
__DEFINE_ERROR_T(network_down)
__DEFINE_ERROR_T(network_reset)
__DEFINE_ERROR_T(network_unreachable)
__DEFINE_ERROR_T(no_buffer_space)
__DEFINE_ERROR_T(no_child_process)
__DEFINE_ERROR_T(no_link)
__DEFINE_ERROR_T(no_lock_available)
__DEFINE_ERROR_T(no_message_available)
__DEFINE_ERROR_T(no_message)
__DEFINE_ERROR_T(no_protocol_option)
__DEFINE_ERROR_T(no_space_left_on_device)
__DEFINE_ERROR_T(no_stream_resources)
__DEFINE_ERROR_T(no_such_device_or_address)
__DEFINE_ERROR_T(no_such_device)
__DEFINE_ERROR_T(no_such_file_or_directory)
__DEFINE_ERROR_T(no_such_process)
__DEFINE_ERROR_T(not_a_directory)
__DEFINE_ERROR_T(not_a_socket)
__DEFINE_ERROR_T(not_a_stream)
__DEFINE_ERROR_T(not_connected)
__DEFINE_ERROR_T(not_enough_memory)
__DEFINE_ERROR_T(not_supported)
__DEFINE_ERROR_T(operation_canceled)
__DEFINE_ERROR_T(operation_in_progress)
__DEFINE_ERROR_T(operation_not_permitted)
__DEFINE_ERROR_T(operation_not_supported)
__DEFINE_ERROR_T(operation_would_block)
__DEFINE_ERROR_T(owner_dead)
__DEFINE_ERROR_T(permission_denied)
__DEFINE_ERROR_T(protocol_error)
__DEFINE_ERROR_T(protocol_not_supported)
__DEFINE_ERROR_T(read_only_file_system)
__DEFINE_ERROR_T(resource_deadlock_would_occur)
__DEFINE_ERROR_T(resource_unavailable_try_again)
__DEFINE_ERROR_T(result_out_of_range)
__DEFINE_ERROR_T(state_not_recoverable)
__DEFINE_ERROR_T(stream_timeout)
__DEFINE_ERROR_T(text_file_busy)
__DEFINE_ERROR_T(timed_out)
__DEFINE_ERROR_T(too_many_files_open_in_system)
__DEFINE_ERROR_T(too_many_files_open)
__DEFINE_ERROR_T(too_many_links)
__DEFINE_ERROR_T(too_many_symbolic_link_levels)
__DEFINE_ERROR_T(value_too_large)
__DEFINE_ERROR_T(wrong_protocol_type)
