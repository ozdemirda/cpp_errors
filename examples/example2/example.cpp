#include <cpp_errors.h>
#include <code_location.h>
#include <iostream>

errors::error h(char c) {
	return errors::make_error("%s - We have a problem", code_locationc());
}

errors::error g(float) {
	auto err = h('A');

	if (err) {
		err = errors::make_serror(512, "%s - %s", code_locationc(),
			err->cmessage());
	}

	return err;
}

errors::error f(const std::string & s) {
	auto err = g(3.14);

	if (err) {
		err = errors::make_tserror(errors::err_type::invalid_argument,
			512, "%s - %s", code_locationc(), err->cmessage());
	}

	return err;
}

int main() {
	errors::error err = f("blah!");

	if (err) {
		std::cerr << errors::str(err->type()) << ":"
			<< err->message() << std::endl;
	}

	return 0;
}
