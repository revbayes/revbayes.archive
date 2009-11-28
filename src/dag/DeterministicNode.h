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
#include "RbFunction.h"

class DeterministicNode : public DAGNode {

    public:
            DeterministicNode(RbFunction* func, const std::vector<Argument>& args);         //!< Constructor
	        DeterministicNode(const DeterministicNode& d);                                  //!< Copy constructor

            //TODO: Destructor is not needed if we just use a pointer here and not a full copy.
            virtual ~DeterministicNode() {}                         //!< Destructor (delete function?)

        RbObject&           operator=(const RbObject& o);
        DeterministicNode&  operator=(const DeterministicNode& o);

        // Standard utility functions
        RbObject*  			clone() const;									    //!< Clone this object
        bool                equals(const RbObject* d) const;           //!< Compare
        void                print(std::ostream& c) const;                       //!< Print object
	     const StringVector&    getClass() const { return rbClass; }        //!< Get class
	     void                   printValue(std::ostream& o) const;                    //!< Print value (for user)
         std::string            toString(void) const;     

        // Regular member functions
        RbObject*           getValue();         //!< Get current value
	    double               getLnProbabilityRatio(void);
        
    private:
        std::vector<DAGNode*>   arguments;      //!< Processed arguments
	    RbFunction*             function;       //!< Function calculating value
};

#endif

