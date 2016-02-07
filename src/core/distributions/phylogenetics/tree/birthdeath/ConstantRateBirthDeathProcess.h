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
                                      const std::vector<Taxon> &tn);
        
        // public member functions
        ConstantRateBirthDeathProcess*                      clone(void) const;                                                                      //!< Create an independent clone
        
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Swap a parameter
        
    private:
        
        // helper functions
        double                                              lnSpeciationRate(double t) const;
        double                                              rateIntegral(double t_low, double t_high) const;
        double                                              pSurvival(double start, double end) const;
        double                                              simulateDivergenceTime(double origin, double present, double rho) const;                //!< Simulate a speciation event.

        // members
        const TypedDagNode<double>*                         speciation;
        const TypedDagNode<double>*                         extinction;
        
    };
    
}

#endif
