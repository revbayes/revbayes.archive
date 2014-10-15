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

#ifndef ConstantRateBirthDeathMassExtinction_H
#define ConstantRateBirthDeathMassExtinction_H

#include "BirthDeathProcess.h"

namespace RevBayesCore {
    
    class Clade;
    
    class ConstantRateBirthDeathMassExtinction : public BirthDeathProcess {
        
    public:
        ConstantRateBirthDeathMassExtinction(const TypedDagNode<double> *o, const TypedDagNode<double> *ro, const TypedDagNode<double> *s, const TypedDagNode<double> *e,
                                      const TypedDagNode< RbVector<double> > *met, const TypedDagNode< RbVector<double> > *mep, 
                                      const TypedDagNode<double> *r, const std::string& ss, const std::string &cdt, 
                                      const std::vector<Taxon> &tn, const std::vector<Clade> &c);
        
        // public member functions
        ConstantRateBirthDeathMassExtinction*               clone(void) const;                                                                                  //!< Create an independent clone
        
        // Parameter management functions
        std::set<const DagNode*>                            getParameters(void) const;                                          //!< Return parameters
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // helper functions
        double                                              lnSpeciationRate(double t) const;
        double                                              rateIntegral(double t_low, double t_high) const;
        double                                              pSurvival(double start, double end) const;
        std::vector<double>*                                simSpeciations(size_t n, double origin, double r) const;
        
        // members
        const TypedDagNode<double>*                         speciation;
        const TypedDagNode<double>*                         extinction;
        const TypedDagNode< RbVector<double> >*          massExtinctionTimes;  
        const TypedDagNode< RbVector<double> >*          massExtinctionSurvivalProbabilities;          
        
    };
    
}

#endif
