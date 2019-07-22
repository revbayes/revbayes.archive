#ifndef ExperimentalCodonModelRateMatrixVectorFunction_H
#define ExperimentalCodonModelRateMatrixVectorFunction_H

#include "RateMatrix_ExperimentalCodonModel.h"
#include "RbVector.h"
#include "Simplex.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {

    /**
     * @brief HKY rate matrix function.
     *
     * This function creates the HKY rates matrix object by setting the transition-transversion parameter kappa
     * and the base frequencies. The rate matrix takes care of the setting of the actual rates and transition probabilities.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0.7, 2017-10-16
     *
     */
    class ExperimentalCodonModelRateMatrixVectorFunction : public TypedFunction<RbVector<RateGenerator> > {

    public:
        ExperimentalCodonModelRateMatrixVectorFunction(const TypedDagNode<double> *b, const TypedDagNode<double> *o, const TypedDagNode<double> *k, const TypedDagNode< RbVector<Simplex> > *aap, const TypedDagNode< Simplex > *bf, const TypedDagNode< RbVector<long> > *s);
        virtual                                            ~ExperimentalCodonModelRateMatrixVectorFunction(void);                                                    //!< Virtual destructor

        // public member functions
        ExperimentalCodonModelRateMatrixVectorFunction*      clone(void) const;                                                              //!< Create an independent clone
        void                                                    update(void);

    protected:
        void                                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters

    private:

        // members
        const TypedDagNode< RbVector<Simplex> >*                amino_acid_preferences;
        const TypedDagNode< Simplex >*                          base_frequencies;
        const TypedDagNode<double>*                             beta;
        const TypedDagNode<double>*                             kappa;
        const TypedDagNode<double>*                             omega;
        const TypedDagNode< RbVector<long> >*                   site_assignments;

    };

}

#endif
