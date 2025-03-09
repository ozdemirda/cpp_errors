A simple, minimalistic group of c++ (c++17 and onwards) header files
providing golang like errors and results.

The whole implementation resides within the folder `include`. Usage
examples can be found in the folder `examples`.

Still, a simple example may come in handy here too:

```c++
// cpp errors themselves are implemented in the header file
// 'include/cpp_errors.h'.
#include <cpp_errors.h>

// The helper macro 'code_location()' is implemented in the
// header file 'include/code_location.h'
#include <code_location.h>

// The result and result_pair types are implemented in the
// header file 'include/cpp_results.h'. These two are alternative
// containers to wrap a data entry and an error pointer. One can
// use either of them to accomplish the same thing, they differ
// from each other with respect to coding style they were written for.
#include <cpp_results.h>

#include <iostream> // For std::cerr


// A function may return an errors::error
errors::error func(int a, float b, char c) {
    bool all_good = false;
    /*
    Do something meaningfull and set all_good accordingly
    */

    if (!all_good) {
        // Apparently it wasn't all good, return an error
        return errors::make_error("%s - This happened, that happened",
            code_location());
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

results::result<int> some_func(bool flag) {
    if (flag) {
        return results::result<int>(5);
    }
    return results::result<int>(errors::make_error("flag was not set"));
}

void caller_of_some_func(bool flag) {
    auto rw = some_func(flag);
    if (auto err = rw.error(); err) {
        // Things went south, there was an error
        std::cerr << err->message() << std::endl;
    }
    // There was no error, use the real result value and do some other work
    auto r = rw.value();
}

results::result_pair<double> some_other_func(bool flag) {
    if (flag) {
        return results::result_pair<double>(5.0);
    }
    return results::result_pair<double>(
        errors::make_error("flag was not set"));
}

void caller_of_some_other_func(bool flag) {
    auto [r, err] = some_other_func(flag);
    if (err) {
        // There was an error, r does not contain meaningful information
        std::cerr << err->message() << std::endl;
    }
    // All went well, r contains meaningful information
}
```
