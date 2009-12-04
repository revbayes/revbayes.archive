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
        DAGNode&            operator=(const DAGNode& o);
        DeterministicNode&  operator=(const DeterministicNode& o);

        // Standard utility functions
        RbObject*  			clone() const;									    //!< Clone this object
        bool                equals(const RbObject* d) const;           //!< Compare
         const StringVector&    getClass() const { return rbClass; }        //!< Get class
	     void                   printValue(std::ostream& o) const;                    //!< Print value (for user)
         std::string            toString(void) const;     

        // Regular member functions
        RbObject*           getValue();         //!< Get current value
	    double               getLnProbabilityRatio(void);
	    double               getLnProbability(void);
	    
        void    	            keepAffectedChildren();                         //!< Keep value of affected nodes recursively
        void	                keepAffectedParents();                         //!< Keep value of affected nodes recursively
        void         	  		restoreAffectedChildren();                      //!< Restore affected nodes recursively
        void         	  		restoreAffectedParents();                      //!< Restore affected nodes recursively
        void    	       		touchAffectedChildren();                        //!< Mark affected nodes recursively
        void	           		touchAffectedParents();                        //!< Mark affected nodes recursively
        
    private:
	    RbFunction*             function;       //!< Function calculating value
	    
};

#endif

