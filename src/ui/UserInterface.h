/**
 * @file
 * This file contains the declaration of UserInterface, which is
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

#ifndef UserInterface_H
#define UserInterface_H

#include <iostream>
#include <sstream>

class UserInterface {

    public:
        bool            initialize(void) { return true;}       //!< Initialize interface    
        void            output(std::string msg);               //!< Display message from string
        void            output(std::ostringstream msg);        //!< Display message from stringstream

        /** Get user interface */
        static UserInterface& userInterface() {
                static UserInterface theInterface = UserInterface();
                return theInterface;
        }

    protected:
                        UserInterface() {}                                      //!< Prevent construction
                        UserInterface(const UserInterface& x) {}                //!< Prevent copy construction
            virtual     ~UserInterface() {}                                     //!< Destructor

        UserInterface&  operator=(const UserInterface& w) { return (*this); }   //! Prevent assignment
};

#endif

