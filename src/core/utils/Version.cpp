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

#include "Version.h"

RbVersion::RbVersion( void ) {
    
}

std::string RbVersion::getDate( void ) const {
    return "$Date$";
}

std::string RbVersion::getVersion( void ) const {
    return "Version 1.0.0 pre-beta";
}


std::string RbVersion::getHeader( void ) const {
    
    std::string header = "";
    header += "\n\n";
    header += "                             RevBayes\n";
    header += "                      " + getVersion() + "\n";
    header += "                            " + getDate() + "\n\n";
    header += "          An R-like environment for phylogenetic analysis\n";
    header += "                     using Bayesian inference\n\n";
    header += "             Released under the GPL license, version 3\n\n";
    
    return header;

}




