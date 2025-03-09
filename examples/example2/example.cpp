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

#include <cpp_errors.h>
#include <code_location.h>
#include <iostream>

errors::error h(char c) {
  // Create a 'generic' error with the default message size.
  return errors::make_error("%s - We have a problem", code_location());
}

errors::error g(float) {
  auto err = h('A');

  if (err) {
    // Create a 'sized' error of type 'generic'.
    err = errors::make_serror(512, "%s - %s", code_location(), err->cmessage());
  }

  return err;
}

errors::error f(const std::string& s) {
  auto err = g(3.14);

  if (err) {
    // Create a 'typed' and 'sized' error.
    err = errors::make_tserror(errors::err_type::invalid_argument, 512, "%s - %s", code_location(), err->cmessage());
  }

  return err;
}

int main() {
  // Call a function which may potentially fail
  errors::error err = f("blah!");

  // Chech whether the function f returned an error
  if (err) {
    // If f returned an error, print the error type and
    // error message to the standard error stream.
    std::cerr << errors::str(err->type()) << ":" << err->message() << std::endl;
    return 1;
  }

  return 0;
}
