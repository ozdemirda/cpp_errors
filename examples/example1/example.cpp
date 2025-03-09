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

// Example usage of errors with constructors.
// As some constructors may try doing some work,
// and that work may fail, constructors themselves
// may also fail. Instead of keeping track of such
// a state via member variables, and checking for
// their values (like 'bool init_successful' etc),
// reference to an error pointer may be used. This
// example tries to give an idea about how to do that.

class rectangle {
 private:
  long double height;
  long double width;

 public:
  typedef struct init_args {
    long double height;
    long double width;

    init_args(long double h = 0, long double w = 0) : height(h), width(w) {}
  } init_args;

  rectangle(const init_args& ria, errors::error& err) {
    // Clearing any leftover errors.
    err = nullptr;

    if (ria.height <= 0) {
      err = errors::make_error("%s - ria.height is not positive: %Lf", code_location(), ria.height);
      return;
    }

    if (ria.width <= 0) {
      err = errors::make_terror(errors::err_type::invalid_argument, "%s - ria.width is not positive: %Lf",
                                code_location(), ria.width);
      return;
    }

    height = ria.height;
    width = ria.width;
  }
};

int main() {
  rectangle::init_args ria(-1, -2);

  errors::error err;

  rectangle r1(ria, err);
  if (err) {
    std::cerr << "Something went bad - " << errors::str(err->type()) << " : " << err->message() << std::endl;
  }

  ria.height = 1;
  rectangle r2(ria, err);
  if (err) {
    std::cerr << "Something went bad - " << errors::str(err->type()) << " : " << err->message() << std::endl;
  }

  ria.width = 2;
  rectangle r3(ria, err);
  if (err) {
    std::cerr << "Something went bad: " << err->message() << std::endl;
  } else {
    std::cerr << "This time everything went well." << std::endl;
  }

  return 0;
}
