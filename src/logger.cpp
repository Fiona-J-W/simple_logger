
#include "logger.hpp"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <ostream>
#include <iterator>

namespace logger {

logger_set::logger_set(std::initializer_list<log_target> lst): m_loggers{lst}, m_min_level{default_level} {
	if (lst.size() == 0) {
		return;
	}
	m_min_level = std::min_element(lst.begin(), lst.end(),
		[](const log_target& l, const log_target& r) {
			return l.min_level < r.min_level;
		})->min_level;
}

void logger_set::log_impl(level l, const std::string& msg) {
	for(auto& logger: m_loggers) {
		if(l >= logger.min_level) {
			*logger.stream << msg << std::flush;
		}
	}
}


logger_set& std_log() {
	static auto log = logger_set{std::cout};
	return log;
}
namespace {

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


std::string concat_msg(level l, const std::vector<std::string>& args) {
	auto msg = make_prefix(l);
	const auto prefix_length = msg.length();
	for(const auto& arg: args) {
		msg += replace_newlines(arg, prefix_length);
	}
	msg += '\n';
	return msg;
}

std::string format_msg(level l, const std::string& format, std::vector<std::string> args) {
	const auto prefix = make_prefix(l);
	const auto length = prefix.length();
	const auto fmt = replace_newlines(format, length);
	std::transform(args.begin(), args.end(), args.begin(),
	               [=](const std::string& str){return replace_newlines(str, length);});

	auto msg = prefix;
	auto arg_index = std::size_t{0};
	auto it = fmt.begin();
	const auto end = fmt.end();
	while(it != end) {
		auto pos = std::find(it, end, '%');
		msg.append(it, pos);
		if (pos == end) {
			break;
		}
		++pos;
		if (pos == end) {
			throw std::invalid_argument{"Invalid formatstring (ends on single '%')"};
		}
		switch(*pos) {
			case '%':
				msg.push_back('%');
				break;
			case 's':
				if (arg_index >= args.size()) {
					throw std::invalid_argument{"Invalid formatstring (not enough arguments)"};
				}
				msg.append(args[arg_index++]);
				break;
			default:
				throw std::invalid_argument{"Invalid formatstring (unknown format-character)"};
		}
		it = std::next(pos);
	}
	msg.push_back('\n');
	return msg;
}

} //  namespace impl

} // namespace logger

