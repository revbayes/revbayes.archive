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
#include "CladogeneticProbabilityMatrix.h"
#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <sstream>
#include <vector>
#include <map>

namespace RevBayesCore {
    
    class DECCladogeneticStateFunction : public AbstractCladogenicStateFunction, public TypedFunction<CladogeneticProbabilityMatrix> {
        
    public:
        
        DECCladogeneticStateFunction( const TypedDagNode< RbVector< double > >* ep, const TypedDagNode< RbVector< double > >* er, const TypedDagNode<RbVector<RbVector<double> > >* cg, unsigned nc, unsigned ns, std::vector<std::string> et, bool epawa=true, bool wa=false, bool os=true );
        virtual                                                 ~DECCladogeneticStateFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        DECCladogeneticStateFunction*                           clone(void) const;                                                              //!< Create an independent clone
        std::map< std::vector<unsigned>, double >               getEventMap(double t=0.0);
        const std::map< std::vector<unsigned>, double >&        getEventMap(double t=0.0) const;
        void                                                    update(void);
        
    protected:
        
        void                                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        void                                                    buildRanges(void);
        void                                                    buildRangesRecursively(std::set<unsigned> s, std::set<std::set<unsigned> >& r, size_t k);
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
        const TypedDagNode< RbVector<RbVector<double> > >*      connectivityGraph;
        unsigned                                                numCharacters;
        unsigned                                                num_states;
        unsigned                                                numIntStates;
        unsigned                                                numEventTypes;
        unsigned                                                maxRangeSize;
       
        // range codes
        std::vector<std::vector<unsigned> >                     bits;
        std::map<std::vector<unsigned>, unsigned>               inverseBits;
        std::vector<std::vector<std::vector<unsigned> > >       bitsByNumOn;
        std::vector<std::vector<unsigned> >                     statesToBitsByNumOn;
        std::map< std::vector<unsigned>, unsigned>              bitsToStatesByNumOn;

        // range events: types, probs, and counts
        std::map< std::vector<unsigned>, unsigned >             eventMapTypes;
//        std::map< std::vector<unsigned>, double >               eventMapProbs;
        std::vector< std::vector<unsigned> >                    eventMapCounts;

        // manages simplex over event type probabilities
        std::vector<std::string>                                eventTypes;
        std::map<std::string, unsigned>                         eventStringToStateMap;
        
        // manage ranges under connecivity graph
        std::set<unsigned>                                      ranges;
        
        bool                                                    eventProbsAsWeightedAverages;
        bool                                                    wideAllopatry;
        bool                                                    orderStatesByNum;
        
    };
    
}

#endif /* defined(__revbayes_proj__DECCladogeneticStateFunction__) */
