#ifndef ConstantRateBirthDeathMassExtinction_H
#define ConstantRateBirthDeathMassExtinction_H

#include "BirthDeathProcess.h"

namespace RevBayesCore {
    
    class Clade;
    
    class ConstantRateBirthDeathMassExtinction : public BirthDeathProcess {
        
    public:
        ConstantRateBirthDeathMassExtinction(const TypedDagNode<double> *ro, const TypedDagNode<double> *s, const TypedDagNode<double> *e,
                                      const TypedDagNode< RbVector<double> > *met, const TypedDagNode< RbVector<double> > *mep, 
                                      const TypedDagNode<double> *r, const TypedDagNode<double> *mp, const std::string& ss, const std::vector<Clade> &ic, const std::string &cdt,
                                      const std::vector<Taxon> &tn);
        
        // public member functions
        ConstantRateBirthDeathMassExtinction*               clone(void) const;                                                                          //!< Create an independent clone
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                            //!< Swap a parameter
        
    private:
        
        // helper functions
        double                                              lnSpeciationRate(double t) const;
        double                                              rateIntegral(double t_low, double t_high) const;
        double                                              pSurvival(double start, double end) const;
        double                                              simulateDivergenceTime(double origin, double present, double rho) const;                    //!< Simulate a speciation event.

        // members
        const TypedDagNode<double>*                         speciation;
        const TypedDagNode<double>*                         extinction;
        const TypedDagNode< RbVector<double> >*             massExtinctionTimes;
        const TypedDagNode< RbVector<double> >*             massExtinctionSurvivalProbabilities;          
        
    };
    
}

#endif
