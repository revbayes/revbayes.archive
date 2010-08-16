/**
 * @file
 * This file contains the declaration of UserInterface, which is
 * the base class for different user interfaces.
 *
 * @brief Declaration of UserInterface
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-12-13 15:50:43 +0100 (Sön, 13 Dec 2009) $
 * @author The RevBayes development core team
 * @license GPL version 3
 * @extends Frame
 * @package parser
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id: UserInterface.h 188 2009-12-13 14:50:43Z ronquist $
 */

#ifndef UserInterface_H
#define UserInterface_H

#include <iostream>
#include <sstream>

#define RBOUT(m) UserInterface::userInterface().output((m))

class UserInterface {

    public:
        bool                        ask(std::string msg);                                      //!< Ask user a question
        bool                        initialize(void) { return true; }                          //!< Initialize interface    
        void                        output(std::string msg);                                   //!< Display message from string
        void                        output(std::ostringstream msg);                            //!< Display message from stringstream
        static UserInterface&       userInterface(void)                                        //!< Get the user interface
		                               {
		                               static UserInterface theInterface = UserInterface();
		                               return theInterface;
		                               }

    protected:
                                    UserInterface(void) {}                                     //!< Prevent construction
                                    UserInterface(const UserInterface& x) {}                   //!< Prevent copy construction
            virtual                ~UserInterface(void) {}                                     //!< Destructor
        UserInterface&              operator=(const UserInterface& w) { return (*this); }      //!< Prevent assignment
};

#endif

