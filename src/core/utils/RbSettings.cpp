/**
 * @file
 * This file contains the implementation of RbSettings, which 
 * contains the settings for many of the variables that are
 * potentially tunable by the user.
 *
 * @brief Declaration of RbSettings
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#include "RbSettings.h"
#include <string>



/** Default constructor: The default settings are first read, and 
 * then potentially overwritten by values contained in a file.  */
RbSettings::RbSettings(void) {

	initializeUserSettings();
	
    // read a file containing the user's alternate default values
}


/** Constructor that takes a file containing the user settings. The
 * default settings are first read, and then potentially overwritten by
 * values contained in a file. */
RbSettings::RbSettings(std::string& defaultFileName) {

	initializeUserSettings();
	
    // read the 'defaultFileName' file containing the user's alternate default values
}



bool RbSettings::getPrintNodeIndex( void ) const {
    
    return printNodeIndex;
}


double RbSettings::getTolerance( void ) const {
    
    return tolerance;
}


/** Initialize the user settings */
void RbSettings::initializeUserSettings(void) {

    tolerance = 10E-10;         // set default value for tolerance comparing doubles
    printNodeIndex = true;      // print node indices of tree nodes as comments
}


void RbSettings::setPrintNodeIndex(bool tf) {
    
    printNodeIndex = tf;
    
}


void RbSettings::setTolerance(double t) {
    
    tolerance = t;
    
}
