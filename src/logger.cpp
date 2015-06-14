
#include "logger.hpp"

#include <cassert>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <ostream>
#include <iterator>

namespace logger {

namespace impl {

/**
 * Manages the standard-logger and the logger-stack.
 *
 * CAREFULL: THIS FUNCTION CONTAINS GLOBAL STATE!
 */
std::vector<logger_set*>& logger_stack() {
	static auto stack = std::vector<logger_set*>{};
	// To avoid infinite recursion, the base-logger must
	// not auto-register but be added manually
	static auto std_logger = logger_set{{std::cout}, auto_register::off};
	// in order to avoid use-after-free bugs, the logger must be created after
	// the stack, to avoid that it's destructor tries to access
	// parts of the destroyed stack
	
	static auto dummy = [&]{
			stack.push_back(&std_logger);
			return 0;
		}();
	(void) dummy;
	return stack;
}

void reassign_stack_pointer(logger_set*& ptr) {
	const auto old_ptr = ptr;
	if (ptr) {
		ptr->m_stackpointer = &ptr;
	}
	(void) old_ptr;
	assert(ptr == old_ptr);
}

void register_logger(logger_set& set) {
	auto& stack = logger_stack();
	// we need to reassign everything if the vector reallocated:
	const auto old_capacity = stack.capacity();
	stack.push_back(&set);
	if (stack.capacity() == old_capacity) {
		reassign_stack_pointer(stack.back());
	} else {
		for(auto& ptr: stack) {
			reassign_stack_pointer(ptr);
		}
	}
}

/**
 * Pops loggers from the stack until the last one is not a nullptr
 */
void pop_loggers() {
	auto& stack = logger_stack();
	while(!stack.empty() and stack.back() == nullptr) {
		stack.pop_back();
	}
	assert(stack.empty() or stack.back() != nullptr);
}

logger_set& active_logger() {
	const auto result = logger_stack().back();
	assert(result != nullptr);
	return *result;
}

} // namespace impl

logger_set::logger_set(std::initializer_list<log_target> lst, auto_register r):
		m_loggers{lst}, m_min_level{default_level} {
	if (lst.size() > 0) {
		m_min_level = std::min_element(lst.begin(), lst.end(),
			[](const log_target& l, const log_target& r) {
				return l.min_level < r.min_level;
			})->min_level;
	}
	if (r == auto_register::on) {
		impl::register_logger(*this);
	}
}

logger_set::~logger_set() {
	if (m_stackpointer) {
		*m_stackpointer = nullptr;
		impl::pop_loggers();
	}
}

logger_set::logger_set(logger_set&& other) noexcept:
		m_loggers{std::move(other.m_loggers)},
		m_stackpointer{other.m_stackpointer},
		m_min_level{other.m_min_level}
{
	other.m_stackpointer = nullptr;
	if (m_stackpointer) {
		*m_stackpointer = this;
	}
}


logger_set& logger_set::operator=(logger_set&& other) noexcept {
	if (m_stackpointer) {
		*m_stackpointer = nullptr;
		impl::pop_loggers();
	}
	m_loggers = std::move(other.m_loggers);
	m_stackpointer = other.m_stackpointer;
	m_min_level = other.m_min_level;
	other.m_stackpointer = nullptr;
	if (m_stackpointer) {
		*m_stackpointer = this;
	}
	return *this;
}

void logger_set::log_impl(level l, const std::string& msg) {
	for(auto& logger: m_loggers) {
		if(l >= logger.min_level) {
			*logger.stream << msg << std::flush;
		}
	}
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

