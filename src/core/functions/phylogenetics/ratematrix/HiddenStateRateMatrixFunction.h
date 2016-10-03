#ifndef HiddenStateRateMatrixFunction_H
#define HiddenStateRateMatrixFunction_H

#include "RateMatrix_FreeBinary.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    /**
     * @brief Free symmetric rate matrix function.
     *
     * This function creates the free (symmetric) rates matrix object by setting the substitution rates.
     * The rate matrix takes care of the setting of the actual rates and transition probabilities.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2015-09-29
     *
     */
    class HiddenStateRateMatrixFunction : public TypedFunction<RateGenerator> {
        
    public:
        HiddenStateRateMatrixFunction(const TypedDagNode<double> *w, const TypedDagNode<double> *b, size_t n_org, size_t n_cat, bool r);
        virtual                                            ~HiddenStateRateMatrixFunction(void);                                      //!< Virtual destructor
        
        // public member functions
        HiddenStateRateMatrixFunction*                      clone(void) const;                                                          //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        // members
        size_t                                              n_org_states;
        size_t                                              n_categories;
        const TypedDagNode< double >*                       within_category_rate;
        const TypedDagNode< double >*                       between_categories_rates;
        
    };
    
}

#endif
