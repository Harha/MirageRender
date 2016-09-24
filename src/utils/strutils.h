#ifndef STRUTIL_H
#define STRUTIL_H

// std includes
#include <string>
#include <cstring>
#include <algorithm>
#include <functional>
#include <cctype>

#ifdef _MSC_VER
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

namespace mirage
{

	// ---------------------------------------------------------------------------
	// istwospace
	// Comparator for finding extra whitespace in a string.
	// ---------------------------------------------------------------------------
	inline bool istwospace(const char lhs, const char rhs) {
		return isspace(lhs) && isspace(rhs);
	}

	// ---------------------------------------------------------------------------
	// strcompcasei
	// Compares two std::strings, case insensitive.
	// ---------------------------------------------------------------------------
	inline bool strcompcasei(const std::string & a, const std::string & b)
	{
		return strncasecmp(a.c_str(), b.c_str(), std::min(a.length(), b.length())) == 0;
	}

	// ---------------------------------------------------------------------------
	// cstr2int
	// Converts a C type string to integer.
	// ---------------------------------------------------------------------------
	constexpr unsigned int cstr2int(const char* str, int h = 0)
	{
		return !str[h] ? 5381 : (cstr2int(str, h + 1) * 33) ^ str[h];
	}

	// ---------------------------------------------------------------------------
	// filetofilepath
	// Converts a filepath ending to a file into a path to the folder of the file.
	// ---------------------------------------------------------------------------
	inline std::string filetofilepath(const std::string & file)
	{
		return file.substr(0, file.find_last_of("/\\")) + "/";
	}

	// ---------------------------------------------------------------------------
	// filetoextension
	// Converts a filepath ending to a file into the file extension.
	// ---------------------------------------------------------------------------
	inline std::string filetoextension(const std::string & file)
	{
		return file.substr(file.find_last_of(".") + 1);
	}

	// ---------------------------------------------------------------------------
	// ltrim
	// Trim from start (in place).
	// ---------------------------------------------------------------------------
	static inline void ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
	}

	// ---------------------------------------------------------------------------
	// rtrim
	// Trim from end (in place).
	// ---------------------------------------------------------------------------
	static inline void rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	}

	// ---------------------------------------------------------------------------
	// trim
	// Trim from both ends (in place).
	// ---------------------------------------------------------------------------
	static inline void trim(std::string &s) {
		ltrim(s);
		rtrim(s);
	}

}

#endif // STRUTIL_H