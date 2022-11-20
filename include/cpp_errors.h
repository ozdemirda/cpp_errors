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
Footer
*/

#pragma once

#include <memory>
#include <vector>
#include <string>
#include <cstdarg>
#include <cstdio>

#ifdef __DEFINE_ERROR_T
#error __DEFINE_ERROR_T is defined elsewwhere
#endif

#ifdef __stringfy_err
#error __stringfy_err is defined elsewhere
#endif

namespace {
// The default max size for error message buffer
const static std::size_t default_error_message_size = 256;
const static std::string static_default_message = "";
}

namespace errors {
// err_type enum encapsulates the types of errors.
enum class err_type {
#define __DEFINE_ERROR_T(e) e,
	#include <error_types/predefined_errors.h>
	#include <error_types/user_defined_errors.h>
#undef __DEFINE_ERROR_T
};

// Function c_str returns a c-string describing
// the err_type e, when it's given e as input
// parameter.
inline const char * c_str(err_type e) {
	static const char * conversion_array[] = {
#define __stringfy_err(a) #a
#define __DEFINE_ERROR_T(e) __stringfy_err(e),
	#include <error_types/predefined_errors.h>
	#include <error_types/user_defined_errors.h>
#undef __DEFINE_ERROR_T
#undef __stringfy_err
	};

	return conversion_array[(int)e];
}

// Function str returns an std::string describing
// the err_type e, when it's given e as input
// parameter.
inline std::string str(err_type e) {
	std::string result = c_str(e);
	return result;
}

// Forward declaration of __error to define error type.
class __error;

// 'error' is a std::unique_ptr that encapsulates a vector of
// error couples and functions to modify them.
typedef std::unique_ptr<__error> error;

// Definition of 'error_couple'
typedef struct __error_couple_s {
	err_type type;
	std::string message;

	// error_couple encapsulates the error information
	__error_couple_s(err_type t, const char * m):
		type(t), message(m)
	{}
} error_couple;

class __error {
private:
	std::vector<error_couple> m_error_couples;

	// The __append function is quite similar to printf family.
	// Error couple containing the error message gets pushed
	// to the end of m_error_couples.
	void __append(err_type type,
			std::size_t size, const char * fmt, ...) {
		va_list args;
		va_start(args, fmt);

		char* buffer = new(std::nothrow) char[size];
		if (!buffer) {
			fprintf(stderr, "Failed to allocate memory for error message\n");
			vfprintf(stderr, fmt, args);
			return;
		}

		vsnprintf(buffer, size, fmt, args);

		m_error_couples.emplace_back(type, buffer);

		delete[] buffer;

		va_end(args);
	}

public:
	// The real constructor function of 'error'
	// This function is meant to be invoked through
	// functions make_error, make_serror, make_terror
	// and make_tserror.
	template <typename... Args>
	__error(err_type type,
			std::size_t size, const char * fmt, Args... args) {
		__append(type, size, fmt, args...);
	}

	// The function append can be used to append an ordinary
	// error couple to the existing couples.
	template <typename... Args>
	void append(const char* fmt, Args... args) {
		__append(err_type::generic_error,
				default_error_message_size, fmt, args...);
	}

	// The function sappend can be used to append an error
	// couple with a specific size limit for its message to
	// the existing error couples.
	template <typename... Args>
	void sappend(std::size_t size, const char* fmt, Args... args) {
		__append(err_type::generic_error,
				size, fmt, args...);
	}

	// The function tappend can used to append an error
	// couple with a specific error type to the existing
	// error couples.
	template <typename... Args>
	void tappend(err_type type,
			const char* fmt, Args... args) {
		__append(type, default_error_message_size, fmt, args...);
	}

	// The function tsappend can be used to append an error
	// couple with a specific size and a specific error type
	// to the existing error couples.
	template <typename... Args>
	void tsappend(err_type type,
			std::size_t size, const char* fmt, Args... args) {
		__append(type, size, fmt, args...);
	}

	// The function message can be used to get the message
	// of the first error couple. It's quite useful for simple
	// errors that are represented by one couple.
	const std::string& message() {
		if (m_error_couples.size() > 0) {
			return m_error_couples[0].message;
		}

		fprintf(stderr, "Function message() was called on an empty error\n");
		return static_default_message;
	}

	// The function type can be used to get the error type
	// of the first error couple. It's quite useful for simple
	// errors that are represented by one couple.
	err_type type() {
		if (m_error_couples.size() > 0) {
			return m_error_couples[0].type;
		}

		fprintf(stderr, "Function type() was called on an empty error\n");
		return err_type::generic_error;
	}

	// The function cmessage can be used to get the c-like message
	// of the first error couple. It's quite useful for simple
	// errors that are represented by one couple.
	const char * cmessage() {
		if (m_error_couples.size() > 0) {
			return m_error_couples[0].message.c_str();
		}

		fprintf(stderr, "Function cmessage() was called on an empty error\n");
		return static_default_message.c_str();
	}

	// The function couples can be used to retrieve the vector of
	// error couples.
	const std::vector<error_couple>& couples() {
		return m_error_couples;
	}
};

// The function make_error can be used to create a new error.
// It provides a signature that feels like printf. It creates
// a generic_error with the default_error_message_size limit.
template <typename... Args>
inline error make_error(const char* fmt, Args... args) {
	return std::make_unique<__error>(err_type::generic_error,
			default_error_message_size, fmt, args...);
}

// The function make_serror can be used to create errors of type
// generic_error with a specific message size limit. Please notice
// that the limit will not be inherited to the following errors
// that get appended to the base error object returned by this
// function.
template <typename... Args>
inline error make_serror(std::size_t size, const char* fmt, Args... args) {
	return std::make_unique<__error>(err_type::generic_error,
			size, fmt, args...);
}

// The function make_terror can be used to create errors of a
// specific type with the default message size limit. The type
// does not get inherited by the appended messages to the object
// returned by this function.
template <typename... Args>
inline error make_terror(err_type type,
		const char* fmt, Args... args) {
	return std::make_unique<__error>(type,
			default_error_message_size, fmt, args...);
}

// The function make_tserror can be used to create errors of
// a specific type with a specific message size limit. These
// specific values do not get inherited by the appended error
// couples to the object returned by this function.
template <typename... Args>
inline error make_tserror(err_type type, std::size_t size,
		const char* fmt, Args... args) {
	return std::make_unique<__error>(type, size, fmt, args...);
}
} // namespace error
