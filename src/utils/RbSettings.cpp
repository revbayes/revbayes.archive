#include "RbSettings.h"

#include <string>



RbSettings::RbSettings(void) {

    // read a file containing the user's alternate default values
}

RbSettings::RbSettings(std::string& defaultFileName) {

    // read the 'defaultFileName' file containing the user's alternate default values
}

void RbSettings::initializeUserSettings(void) {

    tolerance = 0.000001;      // set default value for tolerance comparing doubles
}