#ifndef FORMAT_H
#define FORMAT_H

#include <string>
using namespace std;

namespace Format {
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
std::string Pad(string s, char c);
std::string ElapsedTimePad(long times);
};                                    // namespace Format
#endif