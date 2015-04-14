/**
 * @file
 * This file contains the declaration of the random variable class for constant population-size coalescent process.
 * This class is derived from the stochastic node and each instance will represent a random variable.
 *
 * @brief Declaration of the constant rate Birth-Death process class.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-17, version 1.0
 * @interface TypedDagNode
 *
 * $Id:$
 */

#ifndef ConstantPopulationCoalescent_H
#define ConstantPopulationCoalescent_H

#include "AbstractCoalescent.h"
#include "Taxon.h"
#include "TimeTree.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class Clade;
    
    class ConstantPopulationCoalescent : public AbstractCoalescent {
        
    public:
        ConstantPopulationCoalescent(const TypedDagNode<double> *N, const std::vector<Taxon> &tn, const std::vector<Clade> &c);
        virtual                                            ~ConstantPopulationCoalescent(void);                                                                    //!< Virtual destructor
        
        // public member functions
        ConstantPopulationCoalescent*                       clone(void) const;                                                                                  //!< Create an independent clone

    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
        // derived helper functions
        double                                              computeLnProbabilityTimes(void) const;                                                          //!< Compute the log-transformed probability of the current value.
        std::vector<double>                                 simulateCoalescentTime(size_t n) const;                                                         //!< Simulate n coalescent events.

        
    private:
        
        
        // members
        const TypedDagNode<double>*                         Ne;
        
    };
    
}

#endif
