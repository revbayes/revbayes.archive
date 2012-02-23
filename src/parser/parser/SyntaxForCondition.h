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


class SyntaxForCondition : public SyntaxElement {

    public:
                                    SyntaxForCondition(RbString* identifier, SyntaxElement* inExpr);        //!< Standard constructor
                                    SyntaxForCondition(const SyntaxForCondition& x);                        //!< Copy constructor
	    virtual                    ~SyntaxForCondition();                                                   //!< Destructor

        // Assignment operator
        SyntaxForCondition&         operator=(const SyntaxForCondition& x);                                 //!< Assignment operator
        
        // Basic utility functions
        SyntaxElement*              clone() const;                                                          //!< Clone object
        static const std::string&   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object
        void                        printValue(std::ostream& o) const;                                      //!< Print info about object

        // Regular functions
        void                        finalizeLoop(void);                                                     //!< Finalize loop
        bool                        getNextLoopState(Environment& env);                                     //!< Get next state of loop
        RbVariablePtr               evaluateContent( Environment& env );                                    //!< Get semantic value
        void                        initializeLoop(Environment& env);                                       //!< Initialize loop

    protected:
        RbString*                   varName;                                                                //!< The name of the loop variable
        SyntaxElement*              inExpression;                                                           //!< The in expression (a vector of values)
        Container*                  vector;                                                                 //!< Vector result of 'in' expression
        int                         nextElement;                                                            //!< Next element in vector
    
};

#endif

