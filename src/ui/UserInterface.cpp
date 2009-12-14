/**
 * @file
 * This file contains the implementation of UserInterface, which is
 * the base class for different user interfaces.
 *
 * @brief Declaration of UserInterface
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @extends Frame
 * @package parser
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#include "UserInterface.h"


/** Print a message and a newline */
void UserInterface::output(std::string msg) {

    std::cout << msg << std::endl;
}


/** Convert to string and then call output to print message string */
void UserInterface::output(std::ostringstream msg) {

    output(msg.str());
}


