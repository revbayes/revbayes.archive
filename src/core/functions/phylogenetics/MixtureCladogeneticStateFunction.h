//
//  MixtureCladogeneticStateFunction.h
//  revbayes-proj
//
//  Created by Michael Landis on 10/22/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#ifndef MixtureCladogeneticStateFunction_h
#define MixtureCladogeneticStateFunction_h


#include "AbstractCladogenicStateFunction.h"
#include "CladogeneticProbabilityMatrix.h"
#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <sstream>
#include <vector>
#include <map>

namespace RevBayesCore {
    
    class MixtureCladogeneticStateFunction : public AbstractCladogenicStateFunction, public TypedFunction<CladogeneticProbabilityMatrix> {
        
    public:
        
        MixtureCladogeneticStateFunction( const TypedDagNode< RbVector< double > >* mw, const TypedDagNode< RbVector< CladogeneticProbabilityMatrix > >* cp, unsigned nc, unsigned ns);
        virtual                                                 ~MixtureCladogeneticStateFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        MixtureCladogeneticStateFunction*                           clone(void) const;                                                              //!< Create an independent clone
        const std::map< std::vector<unsigned>, double >&        getEventMap(void) const;
        void                                                    update(void);
        
    protected:
        
        void                                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        void                                                    buildEventMap(void);
        
        // members
        const TypedDagNode< RbVector<double> >*                 mixtureWeights;
        const TypedDagNode< RbVector<CladogeneticProbabilityMatrix> >*             cladoProbs;
        unsigned                                                numCharacters;
        unsigned                                                numStates;
        size_t                                                  numComponents;
        
        std::map< std::vector<unsigned>, unsigned >             eventMapTypes;
        std::map< std::vector<unsigned>, double >               eventMapProbs;
        std::vector< std::vector<unsigned> >                    eventMapCounts;
        
    };
    
}


#endif /* MixtureCladogeneticStateFunction_h */
