#ifndef GtrRateMatrixFunction_H
#define GtrRateMatrixFunction_H

#include "RateMatrix_GTR.h"
#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    /**
     * @brief GTR rate matrix function.
     *
     * This function creates the GTR rates matrix object by setting the exchangeability rates
     * and the base frequencies. The rate matrix takes care of the setting of the actual rates and transition probabilities.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2014-07-04
     *
     */
    class GtrRateMatrixFunction : public TypedFunction<RateGenerator> {
        
    public:
        GtrRateMatrixFunction(const TypedDagNode< RbVector<double> > *er, const TypedDagNode< RbVector<double> > *bf);
        virtual                                            ~GtrRateMatrixFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        GtrRateMatrixFunction*                              clone(void) const;                                                              //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        // members
        
        const TypedDagNode< RbVector<double> >*             exchangeability_rates;
        const TypedDagNode< RbVector<double> >*             base_frequencies;
        
    };
    
}

#endif
