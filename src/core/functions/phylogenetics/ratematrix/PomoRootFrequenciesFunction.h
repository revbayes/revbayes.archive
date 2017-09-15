/**
 * @file
 * This file contains the declaration of the Pomo root frequencies funcion class.
 * This class is derived from the function class and is used to
 * compute the root frequencies of the Pomo model.
 *
 * @brief Declaration of the Pomo root frequencies function.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-06, version 1.0
 * @interface Function
 *
 * $Id$
 */



#ifndef PomoRootFrequenciesFunction_H
#define PomoRootFrequenciesFunction_H

#include "RateMatrix.h"
#include "RbVector.h"
#include "Simplex.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class PomoRootFrequenciesFunction : public TypedFunction< Simplex > {
        
    public:
        PomoRootFrequenciesFunction(const TypedDagNode< Simplex > *fnrf, const TypedDagNode< double > *fopar, const TypedDagNode< RbVector<double> > *mr, const TypedDagNode< long > *ps);
        
        PomoRootFrequenciesFunction(const TypedDagNode< Simplex > *fnrf, const TypedDagNode< double > *fopar, const TypedDagNode< RateGenerator > *mm, const TypedDagNode< long > *ps);

        virtual                                            ~PomoRootFrequenciesFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        PomoRootFrequenciesFunction*                        clone(void) const;                                                              //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode< Simplex >*                      fixedNucleotideRootFrequencies;
        const TypedDagNode< double >*                       frequencyOfPolymorphismsAtTheRoot;
        const TypedDagNode< RbVector<double> >*             mutationRates;
        const TypedDagNode< RateGenerator >*                mutationMatrix;
        const TypedDagNode< long >*                          populationSize;
        bool                                                useMutationMatrix;

        // Private member functions
        std::vector<double> setMutationRates(const RateGenerator& mm) ;

        
    };
    
}

#endif
