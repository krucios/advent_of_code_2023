#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

int main() {
    ifstream fin("inputs/1.in");

    string line;
    size_t sum = 0;

    while (getline(fin, line)) {
        size_t first_digit = 42;
        size_t last_digit = 42;

        //
        // twone - results in a failure as if "one" processed first then
        // "two" is not replaced. As digit names can overlap only by 1
        // letter max, insert digit in the middle of the digit name so
        // code from part 1 can pick up digits and order is presered.
        //
        line = regex_replace(line, regex("one")  , "o1ne");
        line = regex_replace(line, regex("two")  , "t2wo");
        line = regex_replace(line, regex("three"), "th3ree");
        line = regex_replace(line, regex("four") , "fo4ur");
        line = regex_replace(line, regex("five") , "fi5ve");
        line = regex_replace(line, regex("six")  , "si6x");
        line = regex_replace(line, regex("seven"), "sev7en");
        line = regex_replace(line, regex("eight"), "ei8ght");
        line = regex_replace(line, regex("nine") , "ni9ne");

        for (const auto& c: line) {
            if ('0' <= c && c <= '9') {
                size_t digit = c - '0';

                if (first_digit == 42) {
                    first_digit = digit;
                }
                last_digit = digit;
            }
        }

        sum += first_digit * 10 + last_digit;
    }

    cout << sum << endl;

    return 0;
}