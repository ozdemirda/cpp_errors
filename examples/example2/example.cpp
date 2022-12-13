#include <cpp_errors.h>
#include <code_location.h>
#include <iostream>

errors::error h(char c) {
	// Create a 'generic' error with the default message size.
	return errors::make_error("%s - We have a problem", code_locationc());
}

errors::error g(float) {
	auto err = h('A');

	if (err) {
		// Create a 'sized' error of type 'generic'.
		err = errors::make_serror(512, "%s - %s", code_locationc(),
			err->cmessage());
	}

	return err;
}

errors::error f(const std::string & s) {
	auto err = g(3.14);

	if (err) {
		// Create a 'typed' and 'sized' error.
		err = errors::make_tserror(errors::err_type::invalid_argument,
			512, "%s - %s", code_locationc(), err->cmessage());
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
		std::cerr << errors::str(err->type()) << ":"
			<< err->message() << std::endl;
		return 1;
	}

	return 0;
}
