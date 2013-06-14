/**
 * @file
 * This file contains the implementation of UserInterface, which is
 * the base class for different user interfaces.
 *
 * @brief Declaration of UserInterface
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2013-04-26 13:16:09 +0200 (Fri, 26 Apr 2013) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @extends Frame
 * @package parser
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id: RlUserInterface.cpp 2001 2013-04-26 11:16:09Z hoehna $
 */

#include "RbUtil.h"
#include "StringUtilities.h"
#include "RbErrorStream.h"

using namespace RevBayesCore;


/** Print a message and a newline */
void RbErrorStream::output(std::string msg) {
    
    std::cerr << StringUtilities::formatStringForScreen( msg, RevBayesCore::RbUtils::PAD, RevBayesCore::RbUtils::PAD, 80 );
}


/** Print a message and a newline without the padding */
void RbErrorStream::output(std::string msg, const bool hasPadding) {
    
    if (hasPadding == true)
        output(msg);
    else
        std::cerr << msg << std::endl;
}


/** Convert to string and then call output to print message string */
void RbErrorStream::output(std::ostringstream msg) {
    
    output( msg.str() );
}

