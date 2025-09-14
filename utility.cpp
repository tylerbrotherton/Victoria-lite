#include "utility.h"
#include <sstream>
#include <iomanip>

//display a million as 3.2M rather than 3200000 to make it not look like 320000(320K)
// Because nobody has time to count zeros like some kind of medieval accountant
std::string displayNumber(int number) {

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    //if (number >= 1'000'000'000'000)
    //    oss << number / 1'000'000'000'000.0 << "T"; 
    if (number >= 1'000'000'000)
        oss << number / 1'000'000'000.0 << "B"; // You're basically Jeff Bezos now
    else if (number >= 1'000'000)
         oss << number / 1'000'000.0 << "M"; // Welcome to the millionaire's club
    else if (number >= 1'000)
        oss << number / 1'000.0 << "k";// At least you're not broke
    else
        oss << number;// I got 99 problems but a peasent isn't one - can't even afford proper spelling
     return oss.str();
}