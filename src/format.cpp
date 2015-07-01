#include "format.hpp"

#include <algorithm>
#include <cctype>

namespace logger {
namespace format {

inline namespace literals {

format_data operator""_fmt(const char* it, std::size_t len) {
	const auto end = it + len;
	auto retval = format_data{};
	if (it == end) {
		return retval;
	}
	if (*it == '0' or !std::isalnum(*it)) {
		retval.fill = *it;
		++it;
	}
	if (it == end) {
		return retval;
	}
	if (std::isdigit(*it)) {
		const auto w_end = std::find_if_not(it, end,
				[](char c){return std::isdigit(c);});
		retval.width = std::stoul(std::string{it, w_end});
		it = w_end;
	}
	if (it == end) {
		return retval;
	}
	switch(*it) {
		case 's':
			break;
		case 'd':
			retval.base = 10;
			break;
		case 'x':
			retval.base = 16;
			break;
		case 'o':
			retval.base = 8;
			break;
		case 'l':
			retval.align_right = false;
			break;
		case 'r':
			retval.align_right = true;
			break;
		default:
			throw std::invalid_argument{"invalid format_data-string"};
	}
	++it;
	if (it != end) {
		throw std::invalid_argument{"invalid format_data-string"};
	}

	return retval;
}
} // inline namespace literals
} // namespace format

namespace conv {
std::string to_string(const format::formated_string& arg) {
	if (arg.value.size() >= arg.width) {
		return arg.value;
	}
	auto str = std::string{};
	str.reserve(arg.width);
	if (arg.align_right) {
		str.append(arg.width - arg.value.size(), arg.fill);
		str.append(arg.value);
	} else {
		str.append(arg.value);
		str.append(arg.width - arg.value.size(), arg.fill);
	}
	return str;
}
} // namespace conv
} // namespace logger


