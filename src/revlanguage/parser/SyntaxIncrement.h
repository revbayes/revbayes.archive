#ifndef SyntaxIncrement_H
#define SyntaxIncrement_H

#include "SyntaxElement.h"
#include "SyntaxVariable.h"

#include <iostream>
#include <list>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Increment operator ('a++' and '++a')
     *
     * The increment operator adds one to the value of the variable.
     * Currently we do not distinguish between post- and pre- increments.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-02-18, version 1.0
     *
     */
    class SyntaxIncrement : public SyntaxElement {
        
    public:
        SyntaxIncrement(SyntaxVariable* v);                                                                                     //!< Constructor with lhs = variable
        SyntaxIncrement(const SyntaxIncrement& x);                                                                              //!< Copy constructor
	    virtual                            ~SyntaxIncrement();                                                                  //!< Destructor
        
        // Assignment operator
        SyntaxIncrement&                    operator=(const SyntaxIncrement& x);                                                //!< Assignment operator
        
        // Basic utility functions
        SyntaxIncrement*                    clone() const;                                                                      //!< Clone object
        void                                printValue(std::ostream& o) const;                                                  //!< Print info about object
        
        // Regular functions
        RevPtr<Variable>                    evaluateContent(Environment& env);                                                  //!< Get semantic value
        void                                replaceVariableWithConstant(const std::string& name, const RevObject& c);           //!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
        
    protected:
        SyntaxVariable*                     variable;                                                                           //!< The variable expression
        
    };
    
}

#endif

