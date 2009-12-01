/**
 * @file
 * This file contains the declaration of StochasticNode, which is derived
 * from DAGNode. StochasticNode is used for DAG nodes holding stochastic
 * variables with an associated probability density function.
 *
 * @brief Declaration of StochasticNode
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

#ifndef StochasticNode_H
#define StochasticNode_H

#include "DAGNode.h"

class Distribution;
class RandomNumberGenerator;

class StochasticNode : public DAGNode {

    public:
	                StochasticNode(Distribution* d, RandomNumberGenerator* r);         //!< Constructor from distribution
	                StochasticNode(const StochasticNode& s);    //!< Copy constructor
	        virtual ~StochasticNode();                          //!< Destructor

        RbObject&           operator=(const RbObject& o);
        DAGNode&            operator=(const DAGNode& o);
        StochasticNode&     operator=(const StochasticNode& o);

       // Basic utility functions
        StochasticNode* clone() const { return new StochasticNode(*this); } //!< Clone the stochastic node
        void            print(std::ostream& o) const;       //!< Print object

        // Regular functions
        void            initializeValue(RbObject* v);
	    double          getLnProbabilityRatio(void);
	    double          getLnProbability(void);
        void            clamp(RbObject* observedVal);       //!< Clamp the node with an observed value
        void            touchAffected();                    //!< Touch affected nodes
        void            unclamp() { clamped = false; }      //!< Unclamp the node


        void    	            keepAffectedChildren();                         //!< Keep value of affected nodes recursively
        void	                keepAffectedParents();                         //!< Keep value of affected nodes recursively
        void         	  		restoreAffectedChildren();                      //!< Restore affected nodes recursively
        void         	  		restoreAffectedParents();                      //!< Restore affected nodes recursively
        void    	       		touchAffectedChildren();                        //!< Mark affected nodes recursively
        void	           		touchAffectedParents();                        //!< Mark affected nodes recursively
        
    private:
        bool            clamped;                    //!< Is the stochastic node clamped, ie, does it have data?
        Distribution*   distribution;               //!< Distribution (density functions, random draw function)
        RandomNumberGenerator* rng;
        
};

#endif

