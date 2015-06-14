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

BOOST_AUTO_TEST_CASE(concat_aliases) {
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
BOOST_AUTO_TEST_CASE(format_aliases) {
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


BOOST_AUTO_TEST_CASE(formatting_exceptions) {
	auto stream = std::ostringstream{};
	auto logger = logger::logger_set{stream};
	BOOST_CHECK_THROW(logger.notef("%"), std::invalid_argument);
	BOOST_CHECK_THROW(logger.notef("%s"), std::invalid_argument);
	BOOST_CHECK_THROW(logger.notef("%e"), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

