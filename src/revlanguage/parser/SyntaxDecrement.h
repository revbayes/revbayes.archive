#ifndef SyntaxDecrement_H
#define SyntaxDecrement_H

#include "SyntaxElement.h"
#include "SyntaxVariable.h"

#include <iostream>
#include <list>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Decrement operator ('a++' and '++a')
     *
     * The Decrement operator adds one to the value of the variable.
     * Currently we do not distinguish between post- and pre-decrements.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-02-18, version 1.0
     *
     */
    class SyntaxDecrement : public SyntaxElement {
        
    public:
        SyntaxDecrement(SyntaxVariable* v);                                                                                     //!< Constructor with lhs = variable
        SyntaxDecrement(const SyntaxDecrement& x);                                                                              //!< Copy constructor
	    virtual                            ~SyntaxDecrement();                                                                  //!< Destructor
        
        // Assignment operator
        SyntaxDecrement&                    operator=(const SyntaxDecrement& x);                                                //!< Assignment operator
        
        // Basic utility functions
        SyntaxDecrement*                    clone() const;                                                                      //!< Clone object
        void                                printValue(std::ostream& o) const;                                                  //!< Print info about object
        
        // Regular functions
        RbPtr<Variable>                     evaluateContent(Environment& env);                                                  //!< Get semantic value
        void                                replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c);    //!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
        
    protected:
        SyntaxVariable*                     variable;                                                                           //!< The variable expression
        
    };
    
}

#endif

