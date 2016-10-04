//
//  DECCladogeneticStateFunction.h
//  revbayes-proj
//
//  Created by Michael Landis on 1/19/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__DECCladogeneticStateFunction__
#define __revbayes_proj__DECCladogeneticStateFunction__

#include "AbstractCladogenicStateFunction.h"
#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <sstream>
#include <vector>
#include <map>

namespace RevBayesCore {
    
    class DECCladogeneticStateFunction : public AbstractCladogenicStateFunction, public TypedFunction<MatrixReal> {
        
    public:
        
        DECCladogeneticStateFunction( const TypedDagNode< RbVector< double > >* ep, const TypedDagNode< RbVector< double > >* er, unsigned nc, unsigned ns, bool epawa=true, bool wa=false, bool os=true );
        virtual                                                 ~DECCladogeneticStateFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        DECCladogeneticStateFunction*                                clone(void) const;                                                              //!< Create an independent clone
        const std::map< std::vector<unsigned>, double >&        getEventMap(void) const;
        void                                                    update(void);
        
    protected:
        
        void                                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        void                                                    buildBits(void);
        void                                                    buildEventMap(void);
        unsigned                                                bitsToState( const std::vector<unsigned>& b );
        std::string                                             bitsToString( const std::vector<unsigned>& b );
        std::vector<unsigned>                                   bitAllopatryComplement( const std::vector<unsigned>& mask, const std::vector<unsigned>& base );
        void                                                    bitCombinations(std::vector<std::vector<unsigned> >& comb, std::vector<unsigned> array, int i, std::vector<unsigned> accum);
        unsigned                                                sumBits(const std::vector<unsigned>& b);
        void                                                    updateProbs(void);
        
        // members
        const TypedDagNode< RbVector<double> >*                 eventProbs;
        const TypedDagNode< RbVector<double> >*                 eventRates;
        unsigned                                                numCharacters;
        unsigned                                                num_states;
        unsigned                                                numIntStates;
        unsigned                                                numEventTypes;
        std::vector<std::vector<unsigned> >                     bits;
        std::map<std::vector<unsigned>, unsigned>               inverseBits;
        std::vector<std::vector<std::vector<unsigned> > >       bitsByNumOn;
        std::vector<std::vector<unsigned> >                     statesToBitsByNumOn;
        std::map< std::vector<unsigned>, unsigned>              bitsToStatesByNumOn;
        std::map< std::vector<unsigned>, unsigned >             eventMapTypes;
        std::map< std::vector<unsigned>, double >               eventMapProbs;
//        std::map< unsigned, std::map< std::vector<unsigned>, unsigned> > eventMap;
        std::vector< std::vector<unsigned> >                    eventMapCounts;
        std::vector<double>                                     eventMapNormalize;
        bool                                                    eventProbsAsWeightedAverages;
        bool                                                    wideAllopatry;
        bool                                                    orderStatesByNum;
        
    };
    
}

#endif /* defined(__revbayes_proj__DECCladogeneticStateFunction__) */
