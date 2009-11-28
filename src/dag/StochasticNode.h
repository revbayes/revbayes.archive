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

class StochasticNode : public DAGNode {

    public:
	                StochasticNode(void);         //!< Constructor from distribution
	                StochasticNode(const StochasticNode& s);    //!< Copy constructor
	        virtual ~StochasticNode();                          //!< Destructor

        RbObject&           operator=(const RbObject& o);
        StochasticNode&     operator=(const StochasticNode& o);

       // Basic utility functions
        StochasticNode* clone() const { return new StochasticNode(*this); } //!< Clone the stochastic node
        void            print(std::ostream& o) const;       //!< Print object

        // Regular functions
        void            assignDistribution(Distribution* d);
        void            initializeValue(RbObject* v);
	    double          getLnProbabilityRatio(void);
        void            clamp(RbObject* observedVal);       //!< Clamp the node with an observed value
        double          lnProb() const;                     //!< Return ln probability
        double          lnProbRatio() const;                //!< Return ln prob ratio of value to stored value
        double          lnProbRatioAffected() const;        //!< Return ln prob ratio of affected DAG subgraph
        void            proposeValue(RbObject* val);        //!< Propose a new value for the variable
        void            setValue(RbObject* val);            //!< Unconditionally set value
        void            touchAffected();                    //!< Touch affected nodes
        void            unclamp() { clamped = false; }      //!< Unclamp the node

    private:
        bool            clamped;                    //!< Is the stochastic node clamped, ie, does it have data?
        Distribution*   distribution;               //!< Distribution (density functions, random draw function)
};

#endif

