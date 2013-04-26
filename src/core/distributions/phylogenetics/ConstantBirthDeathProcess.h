/**
 * @file
 * This file contains the declaration of the random variable class for constant rate birth-death process.
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

#ifndef ConstantBirthDeathProcess_H
#define ConstantBirthDeathProcess_H

#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class Clade;
    
    class ConstantBirthDeathProcess : public TypedDistribution<TimeTree> {
        
    public:
        ConstantBirthDeathProcess(const TypedDagNode<double> *s, const TypedDagNode<double> *e, const TypedDagNode<double> *r, const TypedDagNode<std::vector<double> > *met, const TypedDagNode<std::vector<double> > *mep, const std::string& ss, const std::string &cdt, unsigned int nTaxa, const std::vector<std::string> &tn, const std::vector<Clade> &c);        
        ConstantBirthDeathProcess(const ConstantBirthDeathProcess &n);                                                                                          //!< Copy constructor
        virtual                                            ~ConstantBirthDeathProcess(void);                                                                    //!< Virtual destructor
        
        // public member functions
        ConstantBirthDeathProcess*                          clone(void) const;                                                                                  //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                            //!< Implementation of swaping parameters
        
    private:
        
        // helper functions
        void                                                attachTimes(TimeTree *psi, std::vector<TopologyNode *> &tips, int index, const std::vector<double> &times, double T);
        void                                                buildRandomBinaryTree(std::vector<TopologyNode *> &tips);
        bool                                                matchesConstraints(void);
        double                                              p1(double t, double T, double r) const;
        double                                              pSurvival(double start, double end, double T, double r) const;
        double                                              simOrigin(void);
        double                                              simSpeciation(double origin, double r);
        void                                                simulateTree(void);
        
        // members
        std::string                                         condition;
        std::vector<Clade>                                  constraints;
        const TypedDagNode<double>*                         speciation;
        const TypedDagNode<double>*                         extinction;
        const TypedDagNode<double>*                         rho;
        const TypedDagNode< std::vector<double> >*          massExtinctionTimes;  
        const TypedDagNode< std::vector<double> >*          massExtinctionSurvivalProrbabilities;          
        unsigned int                                        numTaxa;
        std::string                                         samplingStrategy;
        std::vector<std::string>                            taxonNames;
        double                                              logTreeTopologyProb;
        
    };
    
}

#endif
