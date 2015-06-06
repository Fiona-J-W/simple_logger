
#include "../include/logger.hpp"



int main() {
	logger::log(logger::level::note, "foo", 'b', "ar");
	logger::log(logger::level::error, "Problem: ", 23, " ", 42.0);
}
