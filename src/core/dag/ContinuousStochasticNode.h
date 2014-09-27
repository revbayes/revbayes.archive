/**
 * @file
 * This file contains the declaration of a continuous random variable interface. Additionally to all
 * methods specified by stochastic nodes, a continuous random variable also has a minimum and maximum.
 * These additional methods are helpful for moves, etc.
 *
 * We assume that all continuous random variables are of type double
 *
 * @brief Declaration of the interface for continuous random variables.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-11, version 1.0
 * @interface StochasticNode
 *
 * $Id:$
 */


#ifndef ContinuousStochasticNode_H
#define ContinuousStochasticNode_H

#include "ContinuousDistribution.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    class ContinuousStochasticNode : public StochasticNode<double> {
        
    public:
        ContinuousStochasticNode(const std::string &name, ContinuousDistribution *d);
        virtual                                            ~ContinuousStochasticNode(void) {}                                 //!< Virtual destructor
        
        // public member functions
        virtual ContinuousStochasticNode*                   clone(void) const;                                          //!< Create an independent clone
        virtual double                                      getMax(void) const;                                         //!< Get the maximum value the variable can be
        virtual double                                      getMin(void) const;                                         //!< Get the minimum value the variable can be
                
    };
    
}

#endif
