#ifndef SIMPLE_LOGGER_FORMAT_HPP
#define SIMPLE_LOGGER_FORMAT_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <initializer_list>
#include <tuple>

namespace logger {
namespace format {

template <typename Integer>
struct formated_integer;
struct formated_string;

struct width_t {
	explicit width_t(unsigned v): value{v}{}
	unsigned value = 0u;
};
struct base_t {
	explicit base_t(std::uint8_t v): value{v}{}
	std::uint8_t value = 10u;
};
struct fill_t {
	explicit fill_t(char v): value{v}{}
	char value = ' ';
};
enum class align_t { left, right };

inline namespace literals {
inline width_t operator""_w(unsigned long long value) { return width_t{static_cast<unsigned>(value)}; }
inline base_t operator""_b(unsigned long long value) { return base_t{static_cast<std::uint8_t>(value)}; }
inline fill_t operator""_f(char c) { return fill_t{c}; }
}

struct format_data {
	unsigned width = 0;
	std::uint8_t base = 10;
	char fill = ' ';
	bool align_right = false;

	void set(width_t w) { width = w.value; }
	void set(base_t b) { base = b.value; }
	void set(fill_t f) { fill = f.value; }
	void set(align_t a) { align_right = (a == align_t::right); }
	formated_string operator()(const std::string& str) const;
	template <typename Integer,
	          typename = typename std::enable_if<std::is_integral<Integer>::value>::type>
	formated_integer<Integer> operator()(Integer i) const;
};

template <typename Integer>
struct formated_integer : public format_data {
	formated_integer(Integer i, format_data f) : format_data(f), value{i} {}
	Integer value;
};

struct formated_string : public format_data {
	formated_string(const std::string& s, format_data f)
	        : format_data(f), value(std::move(s)) {}
	const std::string& value;
};

inline formated_string format_data::operator()(const std::string& str) const {
	return {str, *this};
}

template <typename Integer, typename>
inline formated_integer<Integer> format_data::operator()(Integer i) const {
	return {i, *this};
}

template <typename... Args>
formated_string fmt(const std::string& str, const Args&... args) {
	auto format = format_data{};
	std::ignore = std::initializer_list<int>{(format.set(args), 0)...};
	return format(str);
}

template <typename Integer, typename... Args>
formated_integer<Integer> fmt(const Integer i, const Args&... args) {
	auto format = format_data{};
	std::ignore = std::initializer_list<int>{(format.set(args), 0)...};
	return format(i);
}

inline namespace literals { format_data operator""_fmt(const char*, std::size_t); }

} // namespace format

namespace conv {

template <typename Integer>
inline std::string to_string(const format::formated_integer<Integer>& arg) {
	std::ostringstream stream;
	stream << std::setbase(static_cast<int>(arg.base))
	       << std::setw(static_cast<int>(arg.width))
	       << std::setfill(arg.fill)
	       << arg.value;
	return stream.str();
}

std::string to_string(const format::formated_string& arg);

} // namespace conf
} // namespace logger

#endif
