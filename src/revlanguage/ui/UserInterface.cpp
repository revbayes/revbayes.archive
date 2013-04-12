/**
 * @file
 * This file contains the implementation of UserInterface, which is
 * the base class for different user interfaces.
 *
 * @brief Declaration of UserInterface
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @extends Frame
 * @package parser
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#include "RbUtil.h"
#include "StringUtilities.h"
#include "UserInterface.h"

using namespace RevLanguage;

/** Ask user a yes/no question */
bool UserInterface::ask(std::string msg) {

    std::string answer, dummy;
	RBOUT(msg + "? (yes/no) ");
    std::cin >> answer;
    for (size_t i=0; i<answer.size(); i++)
        answer[i] = tolower(answer[i]);

    while (answer!="y" && answer!="yes" && answer!="n" && answer!="no") 
		{
        std::getline(std::cin, dummy);
        std::cout << std::endl;
		RBOUT("Please answer yes or no.");
		RBOUT(msg + "? (yes/no) ");

        std::cin >> answer;
        for (size_t i=0; i<answer.size(); i++)
            answer[i] = tolower(answer[i]);
		}
    std::getline(std::cin, dummy);

    if (answer[0] == 'y')
        return true;
    else
        return false;
}


/** Print a message and a newline */
void UserInterface::output(std::string msg) {

    std::cout << StringUtilities::formatStringForScreen( msg, RevBayesCore::RbUtils::PAD, RevBayesCore::RbUtils::PAD, 80 );
}


/** Print a message and a newline without the padding */
void UserInterface::output(std::string msg, const bool hasPadding) {

    if (hasPadding == true)
        output(msg);
    else
        std::cout << msg << std::endl;
}


/** Convert to string and then call output to print message string */
void UserInterface::output(std::ostringstream msg) {

    output( msg.str() );
}

