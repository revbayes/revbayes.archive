/**
 * @file
 * This file contains the declaration of Func__range, which is used
 * to look up a variable.
 *
 * @brief Declaration of Func__range
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__range_H
#define Func__range_H

#include "RbFunction.h"
#include "RbVector.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;

class Func__range :  public RbFunction {

    public:
                                    Func__range( void );
                                    Func__range( const Func__range& f);
        virtual                    ~Func__range( void );
    
        // overloaded operators
        Func__range&                operator=( const Func__range& f);
    
        // Basic utility functions
        Func__range*                clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value

    protected:
        const RbLanguageObject&     executeFunction(void);                                      //!< Execute function
        void                        setArgumentVariable(const std::string& name, const Variable* var);
 
    private:

        // Arguments
        const Variable*             first;
        const Variable*             last;

        // memberfunction return values
        RbVector<Integer>           range;
};

#endif

