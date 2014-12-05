/**
 * @file
 * This file contains the declaration of RbVersion,
 * which holds and prints information about the current RevBayes version
 *
 * @brief Declaration of RbVersion
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since version 1.0 2012-02-25
 *
 * $Id$
 */

#include "RbVersion.h"

RbVersion::RbVersion( void ) {
    
}

std::string RbVersion::getDate( void ) const {
    return "2014-11-29";
}

std::string RbVersion::getVersion( void ) const {
    return "1.0.0 beta";
}


std::string RbVersion::getHeader( void ) const
{
    
    std::string header = "";
    header += "\n";
    header += "RevBayes version " + getVersion() + " (" + getDate() + ")\n";
    header += "\n";
    header += "Visit the website www.RevBayes.com for more information about RevBayes.\n";
    header += "\n";
    header += "RevBayes is free software released under the GPL license, version 3. Type 'license()' for details.\n";
    header += "\n";
    header += "To quit RevBayes type 'quit()' or 'q()'.\n";
    
    return header;
    
}



