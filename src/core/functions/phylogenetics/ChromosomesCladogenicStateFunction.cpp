//
//  ChromosomesCladogenicStateFunction.cpp
//
//  Created by will freyman on 12/13/15.
//  Copyright (c) 2015 will freyman. All rights reserved.
//

#include "ChromosomesCladogenicStateFunction.h"
#include "MatrixReal.h"
#include "RbException.h"


using namespace RevBayesCore;


ChromosomesCladogenicStateFunction::ChromosomesCladogenicStateFunction(const TypedDagNode< RbVector<double> > *ep, unsigned mc):
    TypedFunction<MatrixReal>( new MatrixReal( mc + 1, (mc + 1) * (mc + 1), 0.0 ) ),
    eventProbs( ep ),
    maxChromo(mc),
    numEventTypes( (unsigned)ep->getValue().size() + 1 )
{
    addParameter( eventProbs );
    
    // since the transition probability matrix will be very large (maxChromo by maxChromo^2) but
    // only sparsely filled, the PhyloCTMCClado uses an event map instead
    // of the full matrix for efficiency
    buildEventMap();
    update();
}


ChromosomesCladogenicStateFunction::~ChromosomesCladogenicStateFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


void ChromosomesCladogenicStateFunction::buildEventMap( void ) {
        
    eventMapCounts.resize(maxChromo + 1, std::vector<unsigned>(numEventTypes, 0));
    
    // for each ancestor state build a map of the possible events in the structure:
    // pair< [ancestor_state, daughter_1_state, daughter_2_state], transition_probability >
    std::vector<unsigned> idx(3);
    for (unsigned i = 1; i <= maxChromo; i++)
    {
        // set ancestor state
        idx[0] = i;

        // loop through all possible events
        for (unsigned j = 0; j < numEventTypes; j++)
        {
            // if both daughters have same number chromsomes as ancestor
            if (j == NO_CHANGE)
            {
                idx[1] = i;
                idx[2] = i;
                eventMapTypes[ idx ] = NO_CHANGE;
                eventMapCounts[ i ][ NO_CHANGE ] += 1;
                eventMapProbs[ idx ] = 0.0;
            }
            // if one of the daughters gains a chromosome
            else if (j == FISSION && i + 1 <= maxChromo)
            {
                idx[1] = i + 1;
                idx[2] = i;
                eventMapTypes[ idx ] = FISSION;
                eventMapCounts[ i ][ FISSION ] += 1;
                eventMapProbs[ idx ] = 0.0;

                idx[1] = i;
                idx[2] = i + 1;
                eventMapTypes[ idx ] = FISSION;
                eventMapCounts[ i ][ FISSION ] += 1;
                eventMapProbs[ idx ] = 0.0;
            }
            // if one of the daughters loses a chromosome
            else if ( j == FUSION && i > 1 )
            {
                idx[1] = i - 1;
                idx[2] = i;
                eventMapTypes[ idx ] = FUSION;
                eventMapCounts[ i ][ FUSION ] += 1;
                eventMapProbs[ idx ] = 0.0;

                idx[1] = i;
                idx[2] = i - 1;
                eventMapTypes[ idx ] = FUSION;
                eventMapCounts[ i ][ FUSION ] += 1;
                eventMapProbs[ idx ] = 0.0;
            }
            // if one of the daughters undergoes polyploidizations
            else if ( j == POLYPLOIDIZATION && i * 2 <= maxChromo )
            {
                idx[1] = i * 2;
                idx[2] = i;
                eventMapTypes[ idx ] = POLYPLOIDIZATION;
                eventMapCounts[ i ][ POLYPLOIDIZATION ] += 1;
                eventMapProbs[ idx ] = 0.0;

                idx[1] = i;
                idx[2] = i * 2;
                eventMapTypes[ idx ] = POLYPLOIDIZATION;
                eventMapCounts[ i ][ POLYPLOIDIZATION ] += 1;
                eventMapProbs[ idx ] = 0.0;
            }
            // if one of the daughters undergoes demipolyploidizations
            else if ( j == DEMIPOLYPLOIDIZATION && i * 1.5 <= maxChromo && i > 1 )
            {
                if (i % 2 == 0)
                {
                    idx[1] = i * 1.5;
                    idx[2] = i;
                    eventMapTypes[ idx ] = DEMIPOLYPLOIDIZATION;
                    eventMapCounts[ i ][ DEMIPOLYPLOIDIZATION ] += 1;
                    eventMapProbs[ idx ] = 0.0;

                    idx[1] = i;
                    idx[2] = i * 1.5;
                    eventMapTypes[ idx ] = DEMIPOLYPLOIDIZATION;
                    eventMapCounts[ i ][ DEMIPOLYPLOIDIZATION ] += 1;
                    eventMapProbs[ idx ] = 0.0;
                }
                else
                {
                    // round down
                    idx[1] = (unsigned)( (double)i * 1.5 - 0.5 );
                    idx[2] = i;
                    eventMapTypes[ idx ] = DEMIPOLYPLOIDIZATION;
                    eventMapCounts[ i ][ DEMIPOLYPLOIDIZATION ] += 1;
                    eventMapProbs[ idx ] = 0.0;

                    idx[1] = i;
                    idx[2] = (unsigned)( (double)i * 1.5 - 0.5 );
                    eventMapTypes[ idx ] = DEMIPOLYPLOIDIZATION;
                    eventMapCounts[ i ][ DEMIPOLYPLOIDIZATION ] += 1;
                    eventMapProbs[ idx ] = 0.0;
                    
                    if ( i * 1.5 + 0.5 <= maxChromo )
                    {
                        // round up
                        idx[1] = i;
                        idx[2] = (unsigned)( (double)i * 1.5 + 0.5 );
                        eventMapTypes[ idx ] = DEMIPOLYPLOIDIZATION;
                        eventMapCounts[ i ][ DEMIPOLYPLOIDIZATION ] += 1;
                        eventMapProbs[ idx ] = 0.0;
                        
                        idx[1] = (unsigned)( (double)i * 1.5 + 0.5 );
                        idx[2] = i;
                        eventMapTypes[ idx ] = DEMIPOLYPLOIDIZATION;
                        eventMapCounts[ i ][ DEMIPOLYPLOIDIZATION ] += 1;
                        eventMapProbs[ idx ] = 0.0;
                    }
                }
            }
        }
    }
}


ChromosomesCladogenicStateFunction* ChromosomesCladogenicStateFunction::clone( void ) const
{
    return new ChromosomesCladogenicStateFunction( *this );
}


const std::map< std::vector<unsigned>, double >&  ChromosomesCladogenicStateFunction::getEventMapProbs(void) const
{
    return eventMapProbs;
}


void ChromosomesCladogenicStateFunction::update( void )
{
    const std::vector<double>& ep = eventProbs->getValue();
    
    // loop through all the mapped events
    std::map<std::vector<unsigned>, unsigned>::iterator it;
    for (it = eventMapTypes.begin(); it != eventMapTypes.end(); it++)
    {
        // normalize the transition probability for this event
        const std::vector<unsigned>& idx = it->first;
        double v = 1.0;
        if (it->second != NO_CHANGE)
        {
            double eventProb = 0.0;
            // check for NaN values
            if (ep[ it->second - 1 ] == ep[ it->second - 1 ])
            {
                eventProb = ep[ it->second - 1 ];
            }
            v = eventProb / eventMapCounts[ idx[0] ][ it->second ];
        }

        // save the probability in the transition matrix
        (*value)[ idx[0] ][ (maxChromo + 1) * idx[1] + idx[2] ] = v;

        // save the probability in the event map
        eventMapProbs[ idx ] = v;
        
    }
}


void ChromosomesCladogenicStateFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == eventProbs)
    {
        eventProbs = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    
}


