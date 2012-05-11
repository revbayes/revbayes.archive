/**
 * @file
 * This file contains the declaration of SyntaxVariable, which is
 * used to hold variable references in the syntax tree.
 *
 * @brief Declaration of SyntaxVariable
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxVariable_H
#define SyntaxVariable_H

#include "Natural.h"
#include "RbString.h"
#include "RbVector.h"
#include "SyntaxElement.h"

#include <iostream>
#include <list>

class SyntaxFunctionCall;
class VariableSlot;


/**
 * This is the class used to hold variables in the syntax tree.
 *
 * We store the identifier, the index vector and the base variable
 * here so that we can wrap these things into a DAG node expression
 * if needed.
 *
 */

class SyntaxVariable : public SyntaxElement {

    public:
                                            SyntaxVariable(RbString* id, std::list<SyntaxElement*>* indx);                          //!< Global variable
                                            SyntaxVariable(SyntaxFunctionCall* fxnCall, std::list<SyntaxElement*>* indx);           //!< Global variable expression
                                            SyntaxVariable(SyntaxVariable* var, RbString* id, std::list<SyntaxElement*>* indx);     //!< Member variable 
                                            SyntaxVariable(SyntaxVariable* var, SyntaxFunctionCall* fxnCall,
                                                           std::list<SyntaxElement*>* indx);                                        //!< Member variable expression
                                            SyntaxVariable(const SyntaxVariable& x);                                                //!< Copy constructor
	    virtual                            ~SyntaxVariable(void);                                                                   //!< Destructor deletes variable, identifier and index

        // Assignment operator
        SyntaxVariable&                     operator=(const SyntaxVariable& x);                                                     //!< Assignment operator

        // Basic utility functions
        SyntaxVariable*                     clone(void) const;                                                                      //!< Clone object
        static const std::string&           getClassName(void);                                                                     //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                                 //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        void                                printValue(std::ostream& o) const;                                                      //!< Print info about object

        // Regular functions
        RbVector                            computeIndex(Environment& env);                                                         //!< Evaluate index
        VariableSlot&                       createVariable(Environment& env);                                                       //!< Get semantic value
        RbPtr<Variable>                     evaluateContent(Environment& env);                                                      //!< Get semantic value
        RbString&                           getIdentifier(void) { return *identifier; }                                             //!< Get identifier
        std::string                         getFullName(Environment& env) const;                                                    //!< Get full name, with indices and base obj
        bool                                hasFunctionCall(void) const;                                                            //!< Does this variable have a function call (e.g. a.xxx() )
        bool                                isMemberVariable(void) const { return baseVariable != NULL; }                           //!< Is the variable a member variable?
        void                                replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c);        //!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.

    protected:
        RbString*                           identifier;                                                                             //!< The name of the variable, if identified by name
        SyntaxFunctionCall*                 functionCall;                                                                           //!< Function call giving a reference to a variable (we hope)
        std::list<SyntaxElement*>*          index;                                                                                  //!< Vector of int indices to variable element
        SyntaxVariable*                     baseVariable;                                                                           //!< Base variable (pointing to a composite node)
        RbLanguageObject*                   replacementValue;
};

#endif

