#ifndef SIMPLE_LOGGER_FORMAT_HPP
#define SIMPLE_LOGGER_FORMAT_HPP

#include <string>
#include <sstream>
#include <iomanip>

namespace logger {
namespace format {

template<typename Integer>
struct formated_integer;
struct formated_string;

struct format_data {
	unsigned width = 0;
	std::uint8_t base = 10;
	char fill = ' ';
	bool align_right = false;

	formated_string operator()(const std::string& str) const;
	template<typename Integer, typename = typename std::enable_if<std::is_integral<Integer>::value>::type>
	formated_integer<Integer> operator()(Integer i) const;
};

template<typename Integer>
struct formated_integer: public format_data {
	formated_integer(Integer i, format_data f):
		format_data(f), value{i} {}
	Integer value;
};

struct formated_string: public format_data {
	formated_string(const std::string& s, format_data f):
		format_data(f), value{std::move(s)} {}
	const std::string& value;
};

inline formated_string format_data::operator()(const std::string& str) const {
	return {str, *this};
}

template<typename Integer, typename>
inline formated_integer<Integer> format_data::operator()(Integer i) const {
	return {i, *this};
}

inline namespace literals {
format_data operator""_fmt(const char*, std::size_t);
}

} // namespace format

namespace conv {

template<typename Integer>
inline std::string to_string(const format::formated_integer<Integer>& arg) {
	std::ostringstream stream;
	stream << std::setbase(arg.base) << std::setw(arg.width) << std::setfill(arg.fill) << arg.value;
	return stream.str();
}

std::string to_string(const format::formated_string& arg);

} // namespace conf
} // namespace logger


#endif
