#include "../src/logger.hpp"

#include <iostream>
#include <stdexcept>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( TestLogger )

BOOST_AUTO_TEST_CASE(basic_log) {
	std::ostringstream stream;
	logger::set_stream(stream);
	logger::log(logger::level::note, "foo", 'b', std::string{"ar"});
	const auto result = stream.str();

	const auto expected_head = std::string{"[note ]["};
	const auto expected_tail = std::string{"]: foobar\n"};

	BOOST_CHECK(std::equal(expected_head.begin(), expected_head.end(), result.begin()));
	BOOST_CHECK(std::equal(expected_tail.rbegin(), expected_tail.rend(), result.rbegin()));
}

BOOST_AUTO_TEST_CASE(basic_debug) {
	std::ostringstream stream;
	logger::set_stream(stream);
	logger::debug("foo", 'b', std::string{"ar"});
	const auto result = stream.str();

	const auto expected_head = std::string{"[debug]["};
	const auto expected_tail = std::string{"]: foobar\n"};

	BOOST_CHECK(std::equal(expected_head.begin(), expected_head.end(), result.begin()));
	BOOST_CHECK(std::equal(expected_tail.rbegin(), expected_tail.rend(), result.rbegin()));
}

BOOST_AUTO_TEST_CASE(basic_warn) {
	std::ostringstream stream;
	logger::set_stream(stream);
	logger::warn ("foo", 'b', std::string{"ar"});
	const auto result = stream.str();

	const auto expected_head = std::string{"[warn ]["};
	const auto expected_tail = std::string{"]: foobar\n"};

	BOOST_CHECK(std::equal(expected_head.begin(), expected_head.end(), result.begin()));
	BOOST_CHECK(std::equal(expected_tail.rbegin(), expected_tail.rend(), result.rbegin()));
}

BOOST_AUTO_TEST_CASE(basic_note) {
	std::ostringstream stream;
	logger::set_stream(stream);
	logger::note ("foo", 'b', std::string{"ar"});
	const auto result = stream.str();

	const auto expected_head = std::string{"[note ]["};
	const auto expected_tail = std::string{"]: foobar\n"};

	BOOST_CHECK(std::equal(expected_head.begin(), expected_head.end(), result.begin()));
	BOOST_CHECK(std::equal(expected_tail.rbegin(), expected_tail.rend(), result.rbegin()));
}

BOOST_AUTO_TEST_CASE(basic_error ) {
	std::ostringstream stream;
	logger::set_stream(stream);
	logger::error ("foo", 'b', std::string{"ar"});
	const auto result = stream.str();

	const auto expected_head = std::string{"[error]["};
	const auto expected_tail = std::string{"]: foobar\n"};

	BOOST_CHECK(std::equal(expected_head.begin(), expected_head.end(), result.begin()));
	BOOST_CHECK(std::equal(expected_tail.rbegin(), expected_tail.rend(), result.rbegin()));
}

BOOST_AUTO_TEST_CASE(basic_fatal) {
	std::ostringstream stream;
	logger::set_stream(stream);
	logger::fatal("foo", 'b', std::string{"ar"});
	const auto result = stream.str();

	const auto expected_head = std::string{"[fatal]["};
	const auto expected_tail = std::string{"]: foobar\n"};

	BOOST_CHECK(std::equal(expected_head.begin(), expected_head.end(), result.begin()));
	BOOST_CHECK(std::equal(expected_tail.rbegin(), expected_tail.rend(), result.rbegin()));
}

BOOST_AUTO_TEST_CASE(basic_logf) {
	std::ostringstream stream;
	logger::set_stream(stream);
	logger::logf(logger::level::note, "Testing a %s with %%-characters and\nnewlines. And even"
	             " more raw data: %s %s %s", "formatstring", 23, 42.0, 'x');
	const auto result = stream.str();

	const auto expected_head = std::string{"[note ]["};
	const auto expected_tail = std::string{"]: Testing a formatstring with %-characters and\n"
		"                                   newlines. And even more raw data: 23 42 x\n"};

	BOOST_CHECK(std::equal(expected_head.begin(), expected_head.end(), result.begin()));
	BOOST_CHECK(std::equal(expected_tail.rbegin(), expected_tail.rend(), result.rbegin()));
}

BOOST_AUTO_TEST_CASE(basic_debugf) {
	std::ostringstream stream;
	logger::set_stream(stream);
	logger::debugf("Testing a %s with %%-characters and\nnewlines. And even"
	             " more raw data: %s %s %s", "formatstring", 23, 42.0, 'x');
	const auto result = stream.str();

	const auto expected_head = std::string{"[debug]["};
	const auto expected_tail = std::string{"]: Testing a formatstring with %-characters and\n"
		"                                   newlines. And even more raw data: 23 42 x\n"};

	BOOST_CHECK(std::equal(expected_head.begin(), expected_head.end(), result.begin()));
	BOOST_CHECK(std::equal(expected_tail.rbegin(), expected_tail.rend(), result.rbegin()));
}


BOOST_AUTO_TEST_CASE(basic_notef) {
	std::ostringstream stream;
	logger::set_stream(stream);
	logger::notef("Testing a %s with %%-characters and\nnewlines. And even"
	             " more raw data: %s %s %s", "formatstring", 23, 42.0, 'x');
	const auto result = stream.str();

	const auto expected_head = std::string{"[note ]["};
	const auto expected_tail = std::string{"]: Testing a formatstring with %-characters and\n"
		"                                   newlines. And even more raw data: 23 42 x\n"};

	BOOST_CHECK(std::equal(expected_head.begin(), expected_head.end(), result.begin()));
	BOOST_CHECK(std::equal(expected_tail.rbegin(), expected_tail.rend(), result.rbegin()));
}

BOOST_AUTO_TEST_CASE(basic_warnf) {
	std::ostringstream stream;
	logger::set_stream(stream);
	logger::warnf("Testing a %s with %%-characters and\nnewlines. And even"
	             " more raw data: %s %s %s", "formatstring", 23, 42.0, 'x');
	const auto result = stream.str();

	const auto expected_head = std::string{"[warn ]["};
	const auto expected_tail = std::string{"]: Testing a formatstring with %-characters and\n"
		"                                   newlines. And even more raw data: 23 42 x\n"};

	BOOST_CHECK(std::equal(expected_head.begin(), expected_head.end(), result.begin()));
	BOOST_CHECK(std::equal(expected_tail.rbegin(), expected_tail.rend(), result.rbegin()));
}

BOOST_AUTO_TEST_CASE(basic_errorf) {
	std::ostringstream stream;
	logger::set_stream(stream);
	logger::errorf("Testing a %s with %%-characters and\nnewlines. And even"
	             " more raw data: %s %s %s", "formatstring", 23, 42.0, 'x');
	const auto result = stream.str();

	const auto expected_head = std::string{"[error]["};
	const auto expected_tail = std::string{"]: Testing a formatstring with %-characters and\n"
		"                                   newlines. And even more raw data: 23 42 x\n"};

	BOOST_CHECK(std::equal(expected_head.begin(), expected_head.end(), result.begin()));
	BOOST_CHECK(std::equal(expected_tail.rbegin(), expected_tail.rend(), result.rbegin()));
}

BOOST_AUTO_TEST_CASE(basic_fatalf) {
	std::ostringstream stream;
	logger::set_stream(stream);
	logger::fatalf("Testing a %s with %%-characters and\nnewlines. And even"
	             " more raw data: %s %s %s", "formatstring", 23, 42.0, 'x');
	const auto result = stream.str();

	const auto expected_head = std::string{"[fatal]["};
	const auto expected_tail = std::string{"]: Testing a formatstring with %-characters and\n"
		"                                   newlines. And even more raw data: 23 42 x\n"};

	BOOST_CHECK(std::equal(expected_head.begin(), expected_head.end(), result.begin()));
	BOOST_CHECK(std::equal(expected_tail.rbegin(), expected_tail.rend(), result.rbegin()));
}


BOOST_AUTO_TEST_CASE(formatting_exceptions) {
	std::ostringstream stream;
	logger::set_stream(stream);
	BOOST_CHECK_THROW(logger::notef("%"), std::invalid_argument);
	BOOST_CHECK_THROW(logger::notef("%s"), std::invalid_argument);
	BOOST_CHECK_THROW(logger::notef("%e"), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END();

