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

#pragma once

#include <cpp_errors.h>
#include <assert.h>
#include <iostream>
#include <utility>
#include <variant>

namespace results {
// result can be thought of as a union (although it uses std::variant under the hood) that can contain either a real
// result value or an error in a mutually exclusive manner. A simple use case would look like the following code block:
//
// result<std::string> func(bool failure_flag) {
//   if (!failure_flag) {
//     return result<std::string>("no problem");
//   }
//   return result<std::string>(errors::make_error("something went wrong!"));
// }
//
// errors::error caller(bool flag) {
//   result<std::string> r = func(flag);
//   if (auto err = r.error(); err) {
//     // Something went wrong and we should not call r.value()
//     return errors::make_error("%s - %s", code_location(), err->cmessage());
//   }
//   std::string str = r.value(); // no error was returned, we can safely call val r.value() and use the returned value
//   do_something(str);
//   return nullptr;
// }
//
// Apart from sharing the same namespace, result does not have anything else in common with result_pair, they exist
// for different ways of doing similar things.
template <typename T>
class result {
 public:
  explicit result(T&& val) : m_variant(std::move(val)) {}

  explicit result(errors::error&& err) : m_variant(std::move(err)) {
    if (std::get<errors::error>(m_variant).get() == nullptr) {
      std::cerr << "Detected a NULL error when the result was not available" << std::endl;
      assert(false);
    }
  }

  result() = delete;

  errors::error error() {
    if (std::holds_alternative<errors::error>(m_variant)) {
      return std::move(std::get<errors::error>(m_variant));
    }
    return nullptr;
  }

  T&& value() {
    if (std::holds_alternative<T>(m_variant)) {
      return std::move(std::get<T>(m_variant));
    }

    // value should only get called after making sure that err() returns nullptr
    std::cerr << "Detected a call to value when the result was not set" << std::endl;
    assert(false);
  }

 private:
  std::variant<T, errors::error> m_variant;
};

// result_pair can be used to have a regulated std::pair of T and errors::error which can be returned as a bundle
// to indicate whether the returned data is valid (if the error part is nil, the data is valid, otherwise the error
// should convey information about what went wrong). This concept is shamelessly stolen from go, as it's quite
// useful. An example use case is below:
//
// result_pair<std::string> func(bool failure_flag) {
//   if (!failure_flag) {
//     return result_pair<std::string>("no problem");
//   }
//   return result_pair<std::string>(errors::make_error("something went wrong!"));
// }
//
// errors::error caller(bool flag) {
//   auto [str, err] = func(flag);
//   if (err) {
//     // Something went wrong and str just contains an empty string
//     return errors::make_error("%s - %s", code_location(), err->cmessage());
//   }
//   // Things went well and str is valid, it can be used
//   do_something(str);
//   return nullptr;
// }
//
// Apart from sharing the same namespace, result_pair is completely independent from the result data type.
template <typename T>
class result_pair : public std::pair<T, errors::error> {
 public:
  explicit result_pair(errors::error&& err)
      : std::pair<T, errors::error>(std::move(std::make_pair<T, errors::error>(T{}, std::move(err)))) {
    static_assert(!std::is_same<T, errors::error>());
    if (this->second.get() == nullptr) {
      assert(false);
    }
  }

  explicit result_pair(T&& t = {})
      : std::pair<T, errors::error>(std::move(std::make_pair<T, errors::error>(std::move(t), nullptr))) {
    static_assert(!std::is_same<T, errors::error>());
  }
};
}  // namespace results
