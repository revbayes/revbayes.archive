#ifndef UTILS_H
#define	UTILS_H

#include <string>
#include <cctype>

class LineEditUtils {
public:

    static std::string reverseString(std::string s) {
        std::string result = "";
        for (int i = 0; i < s.size(); i++) {
            result = s.at(i) + result;
        }
        return result;
    }

    static bool contains(char c, std::string buf) {
        std::string s(buf);        
        for (unsigned int i = 0; i < s.size(); i++) {
            if (s.at(i) == c) {
                return true;
            }
        }
        return false;
    }

    static bool lastCharContains(std::string buf, std::string test) {
        if (buf.size() <= 0) {
            return false;
        }
        return contains(buf.at(buf.size() -1), test);
    }
    
    static std::string extractSubject(std::string cmd) {
        std::string s = "";
        bool foundStart = false;

        for (int i = cmd.size() - 1; i >= 0; i--) {
            char c = cmd.at(i);
            if (!foundStart && isalpha(c)) {
                foundStart = true;
            }

            if (foundStart) {
                if (isalnum(c)) {
                    s += c;
                } else {
                    break;
                }
            }
        }
        return reverseString(s);
    }


};
#endif	/* UTILS_H */

