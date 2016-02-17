//
//  ChromosomesCladogenicStateFunction.h
//
//  Created by will freyman on 12/13/15.
//  Copyright (c) 2015 will freyman. All rights reserved.
//

#ifndef ChromosomesCladogenicStateFunction__
#define ChromosomesCladogenicStateFunction__

#include "RbVector.h"
#include "MatrixReal.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <sstream>
#include <vector>
#include <map>

namespace RevBayesCore {

    class ChromosomesCladogenicStateFunction : public TypedFunction<MatrixReal> {
        
    public:
        ChromosomesCladogenicStateFunction( const TypedDagNode< RbVector< double > >* ep, size_t mc );
        virtual                                            ~ChromosomesCladogenicStateFunction(void);               
        
        const static size_t NO_CHANGE                       = 0;         // N -> N
        const static size_t FISSION                         = 1;         // N -> N + 1
        const static size_t FUSION                          = 2;         // N -> N - 1
        const static size_t POLYPLOIDIZATION                = 3;         // N -> N * 2
        const static size_t DEMIPOLYPLOIDIZATION            = 4;         // N -> N * 1.5
        
        // public member functions
        ChromosomesCladogenicStateFunction*                 clone(void) const;                                                              
        const std::map< std::vector<size_t>, double >&      getEventMapProbs(void) const;
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        
        
    private:
        
        void                                                buildEventMap(void);
        void                                                updateProbs(void);
        
        // members
        const TypedDagNode< RbVector<double> >*             eventProbs;
        size_t                                              maxChromo;
        size_t                                              numEventTypes;
        std::map< std::vector<size_t>, size_t >             eventMapTypes;
        std::map< std::vector<size_t>, double >             eventMapProbs;
        std::vector< std::vector<size_t> >                  eventMapCounts;
        
    };
    
}

#endif
