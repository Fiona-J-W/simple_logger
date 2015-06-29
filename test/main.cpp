#include "../src/logger.hpp"

#include <iostream>
#include <stdexcept>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( TestLogger )

static inline bool head_and_tail_equal(const std::string& str, const std::string& head, const std::string& tail) {
	return str.size() >= head.size() + tail.size()
		and std::equal(head.begin(), head.end(), str.begin())
		and std::equal(tail.rbegin(), tail.rend(), str.rbegin())
		;
}


BOOST_AUTO_TEST_CASE(basic_log) {
	auto stream = std::ostringstream{};
	auto logger = logger::logger_set{stream};
	logger.log(logger::level::note, "foo", " bar ", 23, ' ', 42.0, " baz");
	BOOST_CHECK(head_and_tail_equal(stream.str(), "[note ][", "]: foo bar 23 42 baz\n"));
}


BOOST_AUTO_TEST_CASE(basic_logf) {
	auto stream = std::ostringstream{};
	auto logger = logger::logger_set{stream};
	logger.logf(logger::level::note, "bla%sblub%s%%", "foo", 42);
	BOOST_CHECK(head_and_tail_equal(stream.str(), "[note ][", "]: blafooblub42%\n"));
}


BOOST_AUTO_TEST_CASE(log_hiding) {
	auto stream1 = std::ostringstream{};
	auto logger1 = logger::logger_set{stream1};
	auto stream2 = std::ostringstream{};
	auto logger2 = logger::logger_set{stream2};
	logger::note("foobar");
	BOOST_CHECK(stream1.str().empty());
	BOOST_CHECK(head_and_tail_equal(stream2.str(), "[note ][", "]: foobar\n"));
}


BOOST_AUTO_TEST_CASE(log_restoration) {
	auto stream1 = std::ostringstream{};
	auto logger1 = logger::logger_set{stream1};
	{
		auto stream2 = std::ostringstream{};
		auto logger2 = logger::logger_set{stream2};
	}
	logger::note("foobar");
	BOOST_CHECK(head_and_tail_equal(stream1.str(), "[note ][", "]: foobar\n"));
}


BOOST_AUTO_TEST_CASE(non_global_log) {
	auto stream1 = std::ostringstream{};
	auto logger1 = logger::logger_set{stream1};
	auto stream2 = std::ostringstream{};
	auto logger2 = logger::logger_set{{stream2}, logger::auto_register::off};
	logger::note("foobar");
	BOOST_CHECK(head_and_tail_equal(stream1.str(), "[note ][", "]: foobar\n"));
	BOOST_CHECK(stream2.str().empty());
}


BOOST_AUTO_TEST_CASE(concat_alias_methods) {
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::debug}};
		logger.debug("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[debug][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::note}};
		logger.note("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[note ][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::warn}};
		logger.warn("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[warn ][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::error}};
		logger.error("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[error][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::fatal}};
		logger.fatal("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[fatal][", "]: foo\n"));
	}
}


BOOST_AUTO_TEST_CASE(format_alias_methods) {
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::debug}};
		logger.debugf("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[debug][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::note}};
		logger.notef("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[note ][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::warn}};
		logger.warnf("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[warn ][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::error}};
		logger.errorf("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[error][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::fatal}};
		logger.fatalf("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[fatal][", "]: foo\n"));
	}
}


BOOST_AUTO_TEST_CASE(concat_alias_functions) {
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::debug}};
		logger::debug("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[debug][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::note}};
		logger::note("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[note ][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::warn}};
		logger::warn("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[warn ][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::error}};
		logger::error("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[error][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::fatal}};
		logger::fatal("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[fatal][", "]: foo\n"));
	}
}


BOOST_AUTO_TEST_CASE(format_alias_functions) {
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::debug}};
		logger::debugf("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[debug][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::note}};
		logger::notef("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[note ][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::warn}};
		logger::warnf("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[warn ][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::error}};
		logger::errorf("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[error][", "]: foo\n"));
	}
	{
		auto stream = std::ostringstream{};
		auto logger = logger::logger_set{{stream, logger::level::fatal}};
		logger::fatalf("foo");
		BOOST_CHECK(head_and_tail_equal(stream.str(), "[fatal][", "]: foo\n"));
	}
}


BOOST_AUTO_TEST_CASE(formatting_exceptions) {
	auto stream = std::ostringstream{};
	auto logger = logger::logger_set{stream};
	BOOST_CHECK_THROW(logger.notef("%"), std::invalid_argument);
	BOOST_CHECK_THROW(logger.notef("%s"), std::invalid_argument);
	BOOST_CHECK_THROW(logger.notef("%e"), std::invalid_argument);
}


BOOST_AUTO_TEST_CASE(multiple_calls) {
	auto stream = std::ostringstream{};
	auto logger = logger::logger_set{stream};
	logger::note("foo1");
	logger::debug("foo2");
	logger::warn("foo3");
	logger::note("foo4");
	const auto result = stream.str();
	const auto foo1 = result.find("foo1");
	const auto foo2 = result.find("foo2");
	const auto foo3 = result.find("foo3");
	const auto foo4 = result.find("foo4");
	BOOST_CHECK_LT(foo1, foo3);
	BOOST_CHECK_LT(foo3, foo4);
	BOOST_CHECK_NE(foo4, std::string::npos);
	BOOST_CHECK_EQUAL(foo2, std::string::npos);
}


BOOST_AUTO_TEST_CASE(closed_filestream_exception) {
	std::ofstream stream;
	BOOST_CHECK_THROW(logger::logger_set{stream}, std::runtime_error);
}

BOOST_AUTO_TEST_CASE(formated_strings) {
	using namespace logger::format::literals;
	using logger::conv::to_string;
	BOOST_CHECK_EQUAL(to_string(""_fmt("foo")), "foo");
	BOOST_CHECK_EQUAL(to_string("x3"_fmt("foo")), "foo");
	BOOST_CHECK_EQUAL(to_string("x6"_fmt("foo")), "fooxxx");
	BOOST_CHECK_EQUAL(to_string("x10l"_fmt("foo")), "fooxxxxxxx");
	BOOST_CHECK_EQUAL(to_string("x10r"_fmt("foo")), "xxxxxxxfoo");
}

BOOST_AUTO_TEST_CASE(formated_ints) {
	using namespace logger::format::literals;
	using logger::conv::to_string;
	BOOST_CHECK_EQUAL(to_string(""_fmt(3)), "3");
	BOOST_CHECK_EQUAL(to_string("03"_fmt(3)), "003");
	BOOST_CHECK_EQUAL(to_string("03"_fmt(13)), "013");
	BOOST_CHECK_EQUAL(to_string("03x"_fmt(13)), "00d");
	BOOST_CHECK_EQUAL(to_string("03o"_fmt(13)), "015");
	BOOST_CHECK_EQUAL(to_string("03d"_fmt(13)), "013");
	BOOST_CHECK_EQUAL(to_string("03s"_fmt(13)), "013");
}

BOOST_AUTO_TEST_SUITE_END()

