/**
 * @file
 * This file contains the declaration of SyntaxForLoop, which is
 * used to hold expressions for the loop in a for statement.
 *
 * @brief Declaration of SyntaxForLoop
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxForLoop_H
#define SyntaxForLoop_H

#include "Container.h"
#include "SyntaxElement.h"

#include <iostream>
#include <list>

namespace RevLanguage {

    class SyntaxForLoop : public SyntaxElement {

    public:
        SyntaxForLoop(const std::string &identifier, SyntaxElement* inExpr);                                        //!< Standard constructor
        SyntaxForLoop(const SyntaxForLoop& x);                                                                      //!< Copy constructor
	    
        virtual                     ~SyntaxForLoop();                                                               //!< Destructor

        // Assignment operator
        SyntaxForLoop&              operator=(const SyntaxForLoop& x);                                              //!< Assignment operator
        
        // Basic utility functions
        SyntaxElement*              clone() const;                                                                  //!< Clone object
        void                        printValue(std::ostream& o) const;                                              //!< Print info about object

        // Regular functions
        RevPtr<Variable>            evaluateContent( Environment& env );                                            //!< Get semantic value
        void                        finalizeLoop(void);                                                             //!< Finalize loop
        const std::string&          getIndexVarName(void) const;                                                    //!< Get the name of the index variable
        RevObject*                  getNextLoopState(void);                                                         //!< Get next state of loop
        bool                        isFinished() const;                                                             //!< Have we iterated over the whole loop?
        void                        initializeLoop(Environment& env);                                               //!< Initialize loop
        void                        replaceVariableWithConstant(const std::string& name, const RevObject& c);       //!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.

    protected:
        std::string                 varName;                                                                        //!< The name of the loop variable
        SyntaxElement*              inExpression;                                                                   //!< The in expression (a vector of values)
        Container*                  stateSpace;                                                                     //!< Vector result of 'in' expression
        int                         nextElement;                                                                    //!< Next element in vector
    
    };
    
}

#endif

