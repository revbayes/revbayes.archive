/**
 * @file
 * This file contains the declaration of SyntaxElement, which is
 * the base class used to hold elements (nodes) in the syntax tree.
 *
 * @brief Declaration of SyntaxElement
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @interface SyntaxElement
 * @since Version 1.0, 2009-09-02
 *
 * $Id$
 */

#ifndef SyntaxElement_H
#define SyntaxElement_H

#include "RbLanguageObject.h"
#include "RbPtr.h"
#include "Variable.h"

#include <iostream>
#include <string>

namespace RevLanguage {

/* Forward declarations */
class Environment;

/**
 *  @brief Interface for syntax elements
 *
 *  SyntaxElement is the base class for nodes in the syntax tree. The nodes either
 *  have one or more operands, which are themselves syntax elements, or they have
 *  no operands and simply a predefined result of type (pointer to) value. In
 *  the former case, the elements correspond to interior nodes in the syntax tree
 *  and in the latter case, they correspond to terminal nodes.
 *
 *  The destructor of syntax elements should delete the entire syntax tree rooted at the
 *  element. This is accomplish by simply cycling through all syntax elements referenced
 *  by the syntax element to be destroyed, calling the destructor of each element in turn.
 *  The base class does not reference any other syntax elements and therefore has an empty
 *  destructor.
 */

    class SyntaxElement {

    public:
        virtual                        ~SyntaxElement(void) {}                                                              //!< Destructor; delete syntax subtree

        // Basic utility functions you have to override
        virtual SyntaxElement*          clone(void) const = 0;                                                              //!< Clone object
        virtual void                    printValue(std::ostream& o) const = 0;                                              //!< Print info about object

        // Regular functions
        virtual RbPtr<Variable>         evaluateContent(Environment& env) = 0;                                              //!< Get semantic value
        virtual bool                    isAssignment(void) const;
        virtual bool                    isConstExpression(void) const;                                                      //!< Is subtree constant expr?
        virtual void                    replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c) = 0;//!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.

    protected:
                                        SyntaxElement(void) {}                                                              //!< Protected constructor, just in case
};

}

#endif

