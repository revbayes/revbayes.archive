//
//  ChromosomesCladogenicStateFunction.h
//
//  Created by will freyman on 12/13/15.
//  Copyright (c) 2015 will freyman. All rights reserved.
//

#ifndef ChromosomesCladogenicStateFunction__
#define ChromosomesCladogenicStateFunction__

#include "AbstractCladogenicStateFunction.h"
#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <sstream>
#include <vector>
#include <map>

namespace RevBayesCore {

    class ChromosomesCladogenicStateFunction : public AbstractCladogenicStateFunction, public TypedFunction<MatrixReal> {
        
    public:
        
        ChromosomesCladogenicStateFunction( const TypedDagNode< RbVector< double > >* ep, unsigned mc );
        virtual                                            ~ChromosomesCladogenicStateFunction(void);               
        
        const static unsigned NO_CHANGE                     = 0;         // N -> N
        const static unsigned FISSION                       = 1;         // N -> N + 1
        const static unsigned FUSION                        = 2;         // N -> N - 1
        const static unsigned POLYPLOIDIZATION              = 3;         // N -> N * 2
        const static unsigned DEMIPOLYPLOIDIZATION          = 4;         // N -> N * 1.5
        
        // public member functions
        ChromosomesCladogenicStateFunction*                 clone(void) const;                                                              
        const std::map< std::vector<unsigned>, double >&    getEventMap(void) const;
        void                                                update(void);
        
    protected:
        
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        
        
    private:
        
        void                                                buildEventMap(void);
        void                                                updateProbs(void);
        
        // members
        const TypedDagNode< RbVector<double> >*             eventProbs;
        unsigned                                            maxChromo;
        unsigned                                            numEventTypes;
        std::map< std::vector<unsigned>, unsigned >         eventMapTypes;
        std::map< std::vector<unsigned>, double >           eventMapProbs;
        std::vector< std::vector<unsigned> >                eventMapCounts;

    };
    
}

#endif
