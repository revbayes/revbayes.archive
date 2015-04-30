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

#ifndef ConstantRateBirthDeathProcess_H
#define ConstantRateBirthDeathProcess_H

#include "BirthDeathProcess.h"

#include <vector>

namespace RevBayesCore {
    
    class Clade;
    
    class ConstantRateBirthDeathProcess : public BirthDeathProcess {
        
    public:
        ConstantRateBirthDeathProcess(const TypedDagNode<double> *org, const TypedDagNode<double> *ra, const TypedDagNode<double> *s, const TypedDagNode<double> *e,
                                      const TypedDagNode<double> *r, const std::string& ss, const std::string &cdt,
                                      const std::vector<Taxon> &tn, const std::vector<Clade> &c);
        
        // public member functions
        ConstantRateBirthDeathProcess*                      clone(void) const;                                                                                  //!< Create an independent clone
        
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // helper functions
        double                                              lnSpeciationRate(double t) const;
        double                                              rateIntegral(double t_low, double t_high) const;
        double                                              pSurvival(double start, double end) const;
        std::vector<double>*                                simSpeciations(size_t n, double origin, double r) const;
        
        // members
        const TypedDagNode<double>*                         speciation;
        const TypedDagNode<double>*                         extinction;
        
    };
    
}

#endif
