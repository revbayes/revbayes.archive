/**
 * @file
 * This file contains the declaration of SyntaxForCondition, which is
 * used to hold expressions for the loop in a for statement.
 *
 * @brief Declaration of SyntaxForCondition
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxForCondition_H
#define SyntaxForCondition_H

#include "SyntaxElement.h"
#include "Vector.h"

#include <iostream>
#include <list>

class ValueRule;
class RbString;
class Container;


class SyntaxForCondition : public SyntaxElement {

    public:
                                    SyntaxForCondition(RbString* identifier, SyntaxElement* inExpr);                //!< Standard constructor
                                    SyntaxForCondition(const SyntaxForCondition& x);                                //!< Copy constructor
	    virtual                    ~SyntaxForCondition();                                                           //!< Destructor

        // Assignment operator
        SyntaxForCondition&         operator=(const SyntaxForCondition& x);                                         //!< Assignment operator
        
        // Basic utility functions
        SyntaxElement*              clone() const;                                                                  //!< Clone object
        static const std::string&   getClassName(void);                                                             //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                        //!< Get language type of the object
        void                        printValue(std::ostream& o) const;                                              //!< Print info about object

        // Regular functions
        RbVariablePtr               evaluateContent( Environment& env );                                            //!< Get semantic value
        void                        finalizeLoop(void);                                                             //!< Finalize loop
        const RbString&             getIndexVarName(void) const;                                                    //!< Get the name of the index variable
        RbLanguageObject&           getNextLoopState(void);                                                         //!< Get next state of loop
        bool                        isFinished() const;                                                             //!< Have we iterated over the whole loop?
        void                        initializeLoop(Environment& env);                                               //!< Initialize loop
        void                        replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c);//!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.

    protected:
        RbString*                   varName;                                                                        //!< The name of the loop variable
        SyntaxElement*              inExpression;                                                                   //!< The in expression (a vector of values)
        Container*                  vector;                                                                         //!< Vector result of 'in' expression
        int                         nextElement;                                                                    //!< Next element in vector
    
};

#endif

