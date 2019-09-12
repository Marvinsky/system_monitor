#include <iostream>
#include <string>
#include <sstream>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
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