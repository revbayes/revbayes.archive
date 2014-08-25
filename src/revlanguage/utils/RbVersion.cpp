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
    return "2014-08-23";
}

std::string RbVersion::getVersion( void ) const {
    return "1.0.0 beta";
}


std::string RbVersion::getHeader( void ) const {
    
    std::string header = "";
    header += "\n";
    header += "RevBayes version " + getVersion() + " (" + getDate() + ")\n";
    header += "\n";
    header += "RevBayes: Bayesian phylogenetic inference\n";
    header += "          using probabilistic graphical models and\n";
    header += "          an interpreted language.\n";
    header += "\n";
    header += "RevBayes is free software released under the GPL license,\n";
    header += "version 3. Type 'license()' for details.\n";
    header += "\n";
    header += "RevBayes is a collaborative project with many contributors.\n";
    header += "Type 'contributors()' for more information. Type 'citation()'\n";
    header += "for information on how to cite RevBayes.\n";
    header += "\n";
    header += "For help on using RevBayes, type 'help()'. To quit RevBayes\n";
    header += "type 'quit()' or 'q()'.\n";
    
    return header;
    
}



