/**
 * @file
 * This file contains the declaration of RbException, which
 * is used to handle exceptions in RevBayes.
 *
 * @brief Declaration of RbException
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RbException_H
#define RbException_H


#include <iostream>
#include <sstream>
#include <string>
#include <vector>


class RbException {

    public:
        // Exception types
        enum                        exceptionT { DEFAULT, QUIT, MISSING_VARIABLE };         //!< Exception types
        static std::string          exceptionName[];                                        //!< Exception type names

                                    RbException(void);                                      //!< Default constructor
                                    RbException(const std::string& msg);                    //!< Default with message 
                                    RbException(exceptionT type, const std::string& msg="");//!< General constructor

        // Implemented abstract/virtual functions from base classes
//        RbException*                clone(void) const;                                      //!< Clone this object

        // Regular functions
        exceptionT                  getExceptionType(void) { return exceptionType; }        //!< Get exception type 
        void                        setMessage(std::string msg);
        std::string                 getMessage(void) const;
        void                        print(std::ostream &o) const;                           //!< Print the exception
     
    private:
	    exceptionT                  exceptionType;                                          //!< Exception type
	    std::string                 message;                                                //!< Error message
    
};

#endif

