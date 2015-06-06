
#include "../include/logger.hpp"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <iterator>

namespace logger {


namespace {
std::ostream*& ostream_pointer() {
	static std::ostream* stream = &std::cout;
	return stream;
}

std::ostream& get_stream() {
	return *ostream_pointer();
}

std::string make_prefix(level l) {
	auto prefix = std::string{};
	switch (l) {
		case level::debug:
			prefix = "[debug][";
			break;
		case level::note:
			prefix = "[note ][";
			break;
		case level::warn:
			prefix = "[warn ][";
			break;
		case level::error:
			prefix = "[error][";
			break;
		case level::fatal:
			prefix = "[fatal][";
			break;
	}
	using clock = std::chrono::system_clock;
	const auto now = clock::to_time_t(clock::now());
	// ctime appends a newline, we don't want that here:
	auto time_str = std::ctime(&now);
	prefix.append(time_str, time_str + std::strlen(time_str) -1);
	prefix += "]: ";
	return prefix;
}

} // anonymous namespace

void set_stream(std::ostream& stream) {
	ostream_pointer() = &stream;
}


namespace impl {

std::string replace_newlines(const std::string& str, std::size_t length) {
	auto returnstring = std::string{};
	auto it = str.begin();
	const auto end = str.end();
	auto nl_it = it;
	while((nl_it = std::find(it, end, '\n')) != end) {
		++nl_it;
		returnstring.append(it, nl_it);
		returnstring.append(length, ' ');
		it = nl_it;
	}
	returnstring.append(it, end);
	return returnstring;
}


void log(level l, const std::vector<std::string>& args) {
	const auto prefix = make_prefix(l);
	const auto length = prefix.length();
	get_stream() << prefix;
	std::transform(args.begin(), args.end(), std::ostream_iterator<std::string>{get_stream()},
	               [=](const std::string& str){return replace_newlines(str, length);});
	get_stream() << '\n' << std::flush;
}

void logf(level l, const std::string& format, std::vector<std::string> args) {
	const auto prefix = make_prefix(l);
	const auto length = prefix.length();
	auto fmt = replace_newlines(format, length);
	std::transform(args.begin(), args.end(), args.begin(),
	               [=](const std::string& str){return replace_newlines(str, length);});

	auto mesg = prefix;
	auto arg_index = std::size_t{0};
	auto it = fmt.begin();
	const auto end = fmt.end();
	while(it != end) {
		auto pos = std::find(it, end, '%');
		mesg.append(it, pos);
		if (pos == end) {
			break;
		}
		++pos;
		if (pos == end) {
			throw std::invalid_argument{"Invalid formatstring (ends on single '%')"};
		}
		switch(*pos) {
			case '%':
				mesg.push_back('%');
				break;
			case 's':
				if (arg_index >= args.size()) {
					throw std::invalid_argument{"Invalid formatstring (not enough arguments)"};
				}
				mesg.append(args[arg_index++]);
				break;
			default:
				throw std::invalid_argument{"Invalid formatstring (unknown format-character)"};
		}
		it = std::next(pos);
	}
	mesg.push_back('\n');
	get_stream() << mesg << std::flush;
}

} //  namespace impl

} // namespace logger

