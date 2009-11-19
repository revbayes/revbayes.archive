/**
 * @file
 * This file contains the declaration of DeterministicNode, which is derived
 * from DAGNode. DeterministicNode is used for DAG nodes associated with
 * an expression (equation) that determines their value.
 *
 * @brief Declaration of DeterministicNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id$
 */

#ifndef DeterministicNode_H
#define DeterministicNode_H

#include <string>
#include <vector>
#include "DAGNode.h"

class RbDataType;
class SyntaxElement;


class DeterministicNode : public DAGNode {

    public:
	                DeterministicNode(RbFunction* func);                //!< Constructor from function
	                DeterministicNode(const DeterministicNode& d);      //!< Copy constructor
            virtual ~DeterministicNode();                               //!< Destructor (delete function)

        // Standard utility functions
        RbObject*   clone() const;                                      //!< Clone this object
        bool        equals(const DeterministicNode& d) const;           //!< Compare
        void        print(std::ostream& c) const;                       //!< Print object

        // Regular member functions
        RbObject*   getValue();                                         //!< Get current value
        
    private:
	    RbFunction* function;           //!< Function calculating value
};

#endif
