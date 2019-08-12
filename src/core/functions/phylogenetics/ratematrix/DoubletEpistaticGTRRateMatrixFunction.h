#ifndef DoubletEpistaticGTRRateMatrixFunction_H
#define DoubletEpistaticGTRRateMatrixFunction_H

#include "RateMatrix_DoubletEpistaticGTR.h"
#include "RbVector.h"
#include "Simplex.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {

    /**
     * @brief Epistatic doublet model rate matrix function.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0.7, 2017-10-16
     *
     */
    class DoubletEpistaticGTRRateMatrixFunction : public TypedFunction<RateGenerator> {

    public:
        DoubletEpistaticGTRRateMatrixFunction(const TypedDagNode<double> *d, const TypedDagNode< Simplex > *er, const TypedDagNode< Simplex > *df);
        virtual                                            ~DoubletEpistaticGTRRateMatrixFunction(void);                                                    //!< Virtual destructor

        // public member functions
        DoubletEpistaticGTRRateMatrixFunction*      clone(void) const;                                                              //!< Create an independent clone
        void                                                    update(void);

    protected:
        void                                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters

    private:

        // members
        const TypedDagNode< Simplex >*                          doublet_frequencies;
        const TypedDagNode< Simplex >*                          exchange_rates;
        const TypedDagNode<double>*                             epistasis_d;

    };

}

#endif
