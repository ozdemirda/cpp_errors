#include <cpp_errors.h>
#include <code_location.h>
#include <iostream>

class rectangle {
private:
	long double height;
	long double width;

public:
	typedef struct init_args {
		long double height;
		long double width;

		init_args(long double h = 0, long double w = 0):
			height(h), width(w)
		{}
	} init_args;

	rectangle(const init_args & ria, errors::error & err) {
		// Clearing any leftover errors.
		err = nullptr;

		if (ria.height <= 0) {
			err = errors::make_error("%s - ria.height is not positive: %Lf",
				code_locationc(), ria.height);
			return;
		}

		if (ria.width <= 0) {
			err = errors::make_terror(errors::err_type::invalid_argument,
				"%s - ria.width is not positive: %Lf",
				code_locationc(), ria.width);
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
		std::cerr << "Something went bad - " << errors::str(err->type())
			<< " : " << err->message() << std::endl;
	}

	ria.height = 1;
	rectangle r2(ria, err);
	if (err) {
		std::cerr << "Something went bad - " << errors::str(err->type())
			<< " : " << err->message() << std::endl;
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
