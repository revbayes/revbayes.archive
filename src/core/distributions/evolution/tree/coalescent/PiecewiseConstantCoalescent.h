#ifndef PiecewiseConstantCoalescent_H
#define PiecewiseConstantCoalescent_H

#include "AbstractCoalescent.h"
#include "RbVector.h"
#include "Taxon.h"
#include "TimeTree.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class Clade;
    
    
    
    /**
     * @brief Piecewise-constant population size coalescent process.
     *
     *
     * The piecewise-constant population size coalescent process is an extension to the constant
     * population size coalescent process. The process can have one or two parameters:
     * NEs               := the population sizes
     * internvalStarts   := the start of a new interval (0 is implicitely assumed)
     * If the second parameter is omitted, then we assume that the interval are
     * 1) equally distributed over the present time and the time of the root
     * 2) equally distributed over coalescent events
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-04-14, version 1.0
     *
     */
    class PiecewiseConstantCoalescent : public AbstractCoalescent {
        
    public:
        PiecewiseConstantCoalescent(const TypedDagNode<RbVector<double> > *N, const TypedDagNode<RbVector<double> > *i, const std::vector<Taxon> &tn, const std::vector<Clade> &c);
        virtual                                            ~PiecewiseConstantCoalescent(void);                                                                    //!< Virtual destructor
        
        // public member functions
        PiecewiseConstantCoalescent*                        clone(void) const;                                                                                  //!< Create an independent clone
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
        // derived helper functions
        double                                              computeLnProbabilityTimes(void) const;                                                          //!< Compute the log-transformed probability of the current value.
        std::vector<double>                                 simulateCoalescentTime(size_t n) const;                                                         //!< Simulate n coalescent events.
        
        
    private:
        
        
        // members
        const TypedDagNode<RbVector<double> >*              Nes;
        const TypedDagNode<RbVector<double> >*              intervalStarts;
        
    };
    
}

#endif
