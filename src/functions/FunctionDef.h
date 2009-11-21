/**
 * @file
 * This file contains the declaration of FunctionDef, which is
 * used to describe class member functions.
 *
 * @brief Declaration of FunctionDef
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#ifndef FunctionDef_H
#define FunctionDef_H

#include <string>

#include "RbFunction.h"
#include "RbObject.h"

class FunctionDef : public RbObject {

    public:

                    FunctionDef(const std::string& name, RbFunction* func);     //!< Constructor 
                    FunctionDef(const FunctionDef& fd);                         //!< Copy constructor 
            virtual ~FunctionDef() { delete function; }                         //!< Destructor 

        static const StringVector   rbClass;            //!< Static class attribute

        // Basic utility functions
        virtual std::string         briefInfo() const;                          //!< Brief info about object
        virtual FunctionDef*        clone() const { return new FunctionDef(*this); }    //!< Clone object
        virtual bool                equals(const RbObject* obj) const;          //!< Equals comparison
        virtual const StringVector& getClass() const { return rbClass; }        //!< Get class
        virtual void                print(std::ostream& o) const;               //!< Print complete object info
        virtual void                printValue(std::ostream& o) const;          //!< Print value (for user)

        // Regular functions
        std::string         getLabel() const { return label; }                  //!< Get label of argument
        const RbFunction*   getFunction() const { return function; }            //!< Get function

    protected:
        const std::string   label;          //!< Name of function
        const RbFunction*   function;       //!< Function
};

#endif


