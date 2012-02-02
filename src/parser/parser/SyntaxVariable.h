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

#include "RbString.h"
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
const std::string SyntaxVariable_name = "Syntax Variable";

class SyntaxVariable : public SyntaxElement {

    public:
                                            SyntaxVariable(RbPtr<RbString> id, RbPtr<std::list<RbPtr<SyntaxElement> > > indx);                          //!< Global variable
                                            SyntaxVariable(RbPtr<SyntaxFunctionCall> fxnCall, RbPtr<std::list<RbPtr<SyntaxElement> > > indx);           //!< Global variable expression
                                            SyntaxVariable(RbPtr<SyntaxVariable> var, RbPtr<RbString> id, RbPtr<std::list<RbPtr<SyntaxElement> > > indx);     //!< Member variable 
                                            SyntaxVariable(RbPtr<SyntaxVariable> var, RbPtr<SyntaxFunctionCall> fxnCall,
                                                   RbPtr<std::list<RbPtr<SyntaxElement> > > indx);                                                      //!< Member variable expression
                                            SyntaxVariable(const SyntaxVariable& x);                                                                    //!< Copy constructor
	    virtual                            ~SyntaxVariable(void);                                                                                       //!< Destructor deletes variable, identifier and index

        // Assignment operator
        SyntaxVariable&                     operator=(const SyntaxVariable& x);                                                     //!< Assignment operator

        // Basic utility functions
        std::string                         briefInfo(void) const;                                                                  //!< Brief info about object
        SyntaxVariable*                     clone(void) const;                                                                      //!< Clone object
        const VectorString&                 getClass(void) const;                                                                   //!< Get class vector 
        const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        void                                print(std::ostream& o) const;                                                           //!< Print info about object

        // Regular functions
        RbPtr<RbString>                     getIdentifier(void) { return identifier; }                                              //!< Get identifier
        VectorNatural                       computeIndex(const RbPtr<Environment>& env);                                            //!< Evaluate index
        std::string                         getFullName(const RbPtr<Environment>& env) const;                                       //!< Get full name, with indices and base obj
        VariableSlot*                       createVariable(const RbPtr<Environment>& env);                                          //!< Get semantic value
        RbPtr<Variable>                     evaluateContent(const RbPtr<Environment>& env);                                         //!< Get semantic value
        bool                                isMemberVariable(void) const { return baseVariable != NULL; }                           //!< Is the variable a member variable?

    protected:
        RbPtr<RbString>                     identifier;                                                                             //!< The name of the variable, if identified by name
        RbPtr<SyntaxFunctionCall>           functionCall;                                                                           //!< Function call giving a reference to a variable (we hope)
        RbPtr<std::list<RbPtr<SyntaxElement> > >  index;                                                                                  //!< Vector of int indices to variable element
        RbPtr<SyntaxVariable>               baseVariable;                                                                           //!< Base variable (pointing to a composite node)
    
    private:
        static const TypeSpec       typeSpec;
};

#endif

