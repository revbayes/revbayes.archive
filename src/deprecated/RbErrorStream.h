/**
 * @file
 * This file contains the declaration of UserInterface, which is
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
 * $Id: RlUserInterface.h 2001 2013-04-26 11:16:09Z hoehna $
 */

#ifndef RbErrorStream_H
#define RbErrorStream_H

#include "UserInterface.h"

#include <iostream>
#include <sstream>

namespace RevBayesCore {
    
#define RBOUT(m) RevBayesCore::RbErrorStream::userInterface().output((m))
    
    class RbErrorStream {
        
    public:
//        bool                        ask(std::string msg);                                      //!< Ask user a question
//        bool                        initialize(void) { return true; }                          //!< Initialize interface    
        void                        output(std::string msg);                                   //!< Display message from RlString
        void                        output(std::string msg, const bool hasPadding);            //!< Display message from RlString with control of padding
        void                        output(std::ostringstream msg);                            //!< Display message from RlStringstream
        static RbErrorStream&       userInterface(void)                                        //!< Get the user interface
        {
            static RbErrorStream theInterface = RbErrorStream();
            return theInterface;
        }
        
    protected:
        
        RbErrorStream(void) {}                                     //!< Prevent construction
        RbErrorStream(const RbErrorStream& x) {}                   //!< Prevent copy construction
        virtual                    ~RbErrorStream(void) {}                                     //!< Destructor
        RbErrorStream&              operator=(const RbErrorStream& w) { return (*this); }      //!< Prevent assignment
    };
    
}

#endif

