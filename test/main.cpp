
#include "../include/logger.hpp"



int main() {
	logger::log(logger::level::note, "foo", 'b', "ar");
	logger::log(logger::level::error, "Problem: ", 23, " ", 42.0, "\n", 5, " after a newline");
	logger::logf(logger::level::warn, "Testing a %s with %%-characters and\nnewlines. And even"
			" more raw data: %s %s %s", "formatstring", 23, 42.0, 'x');
}
