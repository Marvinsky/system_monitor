#include <iostream>
#include <string>
#include <sstream>

#include "format.h"

using std::string;
using std::to_string;


std::string Format::Pad(string s, char c) {
    s.insert(s.begin(), 2 - s.size(), c);
    return s;
}

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    long h, m, s;
    h = seconds/3600;
    seconds = seconds%3600;
    m = seconds/60;
    s = seconds%60;
    //std::cout<<"Formato: "<<h<<":"<<m<<":"<<s<<"\n";

    std::ostringstream stream;
    stream<<((h<10)?"0"+to_string(h):to_string(h)) <<":"<< ((m<10)?"0"+to_string(m):to_string(m)) <<":"<< ((s<10)?"0"+to_string(s):to_string(s)) ;

    return stream.str(); 
}

std::string Format::ElapsedTimePad(long seconds) {
    long h, m, s;
    h = seconds/(60*60);
    m = (seconds/60)%60;
    s = seconds%60;
    return std::string(Format::Pad(to_string(h), '0') + ":" + Format::Pad(to_string(m), '0') + ":" + Format::Pad(to_string(s), '0'));
}
