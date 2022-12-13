A simple, minimalistic group of c++ (c++17 and onwards) header files
providing golang like errors.

The whole implementation resides within the folder `include`. Usage
examples can be found in the folder `examples`.

Still, a simple example may come handy here too:

```c++
// For std::cerr at the bottom
#include <iostream>

// cpp errors themselves are implemented in the header file
// 'include/cpp_errors.h'.
#include <cpp_errors.h>

// The helper macros 'code_location()' and 'code_locationc()'
// are implemented in the header file 'include/code_location.h'
#include <code_location.h>

// A function may return an errors::error
errors::error func(int a, float b, char c) {
    bool all_good = false;
    /*
    Do something meaningfull and set all_good accordingly
    */

    if (!all_good) {
        // Apparently it wasn't all good, return an error
        return errors::make_error("%s - This happened, that happened",
            code_locationc());
    }

    // All went well, return a nullptr indicating no error
    return nullptr;
}

void caller_of_func() {
    auto err = func(-1, 3.141593, 'L');
    if (err) {
        std::cerr << "Houston, we have a problem: "
            << err->message() << std::endl;
            return;
    }

    // Do some other meaningful work
}

```
