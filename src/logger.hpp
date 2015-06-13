#include <ostream>
#include <string>
#include <sstream>
#include <vector>

namespace logger {

enum class level { debug, note, warn, error, fatal };

/**
 * conv::to_string will be used to convert whatever argument is send
 * to the logger to a string. If another type shall be supported,
 * just add the appropriate overload and you can start using it right
 * away. The default conversion will use a stringstream.
 */
namespace conv {
template <typename T>
inline std::string to_string(const T& arg) {
	std::ostringstream stream;
	stream << arg;
	return stream.str();
}
inline std::string to_string(std::string&& arg) { return arg; }
inline std::string to_string(const std::string& arg) { return arg; }
inline std::string to_string(const char* arg) { return arg; }
}

namespace impl {
void log(level, const std::vector<std::string>& args);
void logf(level, const std::string&, std::vector<std::string> args);
}

void set_stream(std::ostream&);

template <typename... Args>
void log(level l, Args&&... data) {
	impl::log(l, {conv::to_string(std::forward<Args>(data))...});
}

template <typename... Args>
void logf(level l, const std::string& format, Args&&... data) {
	impl::logf(l, format, {conv::to_string(std::forward<Args>(data))...});
}

template <typename... Args>
void debug(Args&&... args) {
	log(level::debug, std::forward<Args>(args)...);
}
template <typename... Args>
void note(Args&&... args) {
	log(level::note, std::forward<Args>(args)...);
}
template <typename... Args>
void warn(Args&&... args) {
	log(level::warn, std::forward<Args>(args)...);
}
template <typename... Args>
void error(Args&&... args) {
	log(level::error, std::forward<Args>(args)...);
}
template <typename... Args>
void fatal(Args&&... args) {
	log(level::fatal, std::forward<Args>(args)...);
}

template <typename... Args>
void debugf(const std::string& fmt, Args&&... args) {
	logf(level::debug, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
void notef(const std::string& fmt, Args&&... args) {
	logf(level::note, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
void warnf(const std::string& fmt, Args&&... args) {
	logf(level::warn, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
void errorf(const std::string& fmt, Args&&... args) {
	logf(level::error, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
void fatalf(const std::string& fmt, Args&&... args) {
	logf(level::fatal, fmt, std::forward<Args>(args)...);
}
}
