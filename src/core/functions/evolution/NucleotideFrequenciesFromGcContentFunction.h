/**
 * @file
 * This file contains the declaration of the GTR rate matrix function class.
 * This class is derived from the function class and is used to
 * compute the rate matrix of a general time reversible (GTR) Markov chain.
 *
 * @brief Declaration of the GTR rate matrix function.
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



#ifndef NucleotideFrequenciesFromGcContentFunction_H
#define NucleotideFrequenciesFromGcContentFunction_H

#include "RateMatrix.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class NucleotideFrequenciesFromGcContentFunction : public TypedFunction< RbVector<double> > {
        
    public:
        NucleotideFrequenciesFromGcContentFunction(const TypedDagNode<double > *Gc);
        NucleotideFrequenciesFromGcContentFunction(const NucleotideFrequenciesFromGcContentFunction &n);                                                                              //!< Copy constructor
        virtual                                            ~NucleotideFrequenciesFromGcContentFunction(void);                                                         //!< Virtual destructor
        
        // public member functions
        NucleotideFrequenciesFromGcContentFunction*         clone(void) const;                                                                        //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                          //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode< double >*                       gc;        
    };
    
}

#endif
