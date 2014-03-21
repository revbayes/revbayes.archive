/* 
 * File:   utils.h
 * Author: johadunf
 *
 * Created on March 18, 2014, 2:10 PM
 */

#ifndef UTILS_H
#define	UTILS_H

#include <string>
#include <cctype>

class Utils {
public:

    static unsigned int charLen(const char* buf) { // must be a null terminated char pointer
        int max = 10000;
        int i = 0;
        while ((buf[i++] != '\0') && (i < max));

        if (i == max) {
            // throw error
        }
        return (i - 1);
    }

    static bool contains(char c, const char* buf) {
        for (unsigned int i = 0; i < charLen(buf); i++) {
            if (buf[i] == c) {
                return true;
            }
        }
        return false;
    }

    static bool lastCharContains(const char* buf, const char* test) {
        if(charLen(buf) <= 0){
            return false;
        }
        return contains(buf[charLen(buf) -1], test);
    }

    static const char* getTail(const char* buf, int fromPosition) {
        std::string tmp;
        for (unsigned int i = fromPosition; i < charLen(buf); i++) {
            tmp += buf[i];
        }

        return tmp.c_str();
    }

    static const char* extractSubject(const char* cmd) {
        std::string s = "";
        bool foundStart = false;

        // get rid of all non alpha in beginning, and all non alphaNumeric in end
        // todo: consider space in beginning of subject
        for (unsigned int i = 0; i < charLen(cmd); i++) {
            char c = cmd[i];

            if(!foundStart && isalpha(c)){
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
        return s.c_str();
    }
};
#endif	/* UTILS_H */

