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

#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class Clade;
    
    class ConstantPopulationCoalescent : public TypedDistribution<TimeTree> {
        
    public:
        ConstantPopulationCoalescent(const TypedDagNode<double> *N, size_t nTaxa, const std::vector<std::string> &tn, const std::vector<Clade> &c);        
        ConstantPopulationCoalescent(const ConstantPopulationCoalescent &n);                                                                                          //!< Copy constructor
        virtual                                            ~ConstantPopulationCoalescent(void);                                                                    //!< Virtual destructor
        
        // public member functions
        ConstantPopulationCoalescent*                       clone(void) const;                                                                                  //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);

    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // helper functions
        void                                                attachTimes(TimeTree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> &times);
        void                                                buildRandomBinaryTree(std::vector<TopologyNode *> &tips);
        bool                                                matchesConstraints(void);
        void                                                simulateTree(void);
        
        // members
        std::vector<Clade>                                  constraints;
        const TypedDagNode<double>*                         Ne;
        size_t                                              numTaxa;
        std::vector<std::string>                            taxonNames;
        double                                              logTreeTopologyProb;
        
    };
    
}

#endif
