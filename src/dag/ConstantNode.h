/**
 * @file
 * This file contains the declaration of ConstantNode, which is derived
 * from DAGNode. ConstantNode is used for DAG nodes holding constant
 * values.
 *
 * @brief Declaration of ConstantNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-11-19 17:29:33 +0100 (Tor, 19 Nov 2009) $
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: ConstantNode.h 69 2009-11-19 16:29:33Z ronquist $
 */

#ifndef ConstantNode_H
#define ConstantNode_H

#include "DAGNode.h"

class ConstantNode : public DAGNode {

    public:
	                ConstantNode(RbObject* val);        //!< Constructor from value
	                ConstantNode(ConstantNode& c);      //!< Copy constructor
	        virtual ~ConstantNode();                    //!< Destructor deletes object

        ConstantNode*   clone(ConstantNode& c);         //!< Clone the stochastic node
        bool            equals(RbObject* obj);          //!< Object comparison
        void            print(std::ostream& o) const;   //!< Print object
};

#endif

