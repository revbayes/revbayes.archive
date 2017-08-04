//
//  ChromosomesCladogenicBirthDeathFunction.cpp
//
//  Created by Will Freyman on 6/22/16.
//


#include "ChromosomesCladogenicBirthDeathFunction.h"
#include "MatrixReal.h"
#include "RbException.h"


using namespace RevBayesCore;


ChromosomesCladogenicBirthDeathFunction::ChromosomesCladogenicBirthDeathFunction(const TypedDagNode< RbVector<double> > *sr, unsigned mc):
TypedFunction<MatrixReal>( new MatrixReal( mc + 1, (mc + 1) * (mc + 1), 0.0 ) ),
speciationRates( sr ),
maxChromo(mc),
numEventTypes( (unsigned)sr->getValue().size() )
{
    addParameter( speciationRates );
    
    // since the transition rate matrix will be very large (maxChromo by maxChromo^2) but
    // only sparsely filled, the we use an event map instead
    // of the full matrix for efficiency
    buildEventMap();
    update();
}


ChromosomesCladogenicBirthDeathFunction::~ChromosomesCladogenicBirthDeathFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


void ChromosomesCladogenicBirthDeathFunction::buildEventMap( void ) {
    
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
                eventMapTypes[ idx ].push_back(unsigned(NO_CHANGE));
                eventMapCounts[ i ][ NO_CHANGE ] += 1;
                eventMap[ idx ] = 0.0;
            }
            // if one of the daughters gains a chromosome
            else if (j == FISSION && i + 1 <= maxChromo)
            {
                idx[1] = i + 1;
                idx[2] = i;
                eventMapTypes[ idx ].push_back(unsigned(FISSION));
                eventMapCounts[ i ][ FISSION ] += 1;
                eventMap[ idx ] = 0.0;
                
                idx[1] = i;
                idx[2] = i + 1;
                eventMapTypes[ idx ].push_back(unsigned(FISSION));
                eventMapCounts[ i ][ FISSION ] += 1;
                eventMap[ idx ] = 0.0;
            }
            // if one of the daughters loses a chromosome
            else if ( j == FUSION && i > 1 )
            {
                idx[1] = i - 1;
                idx[2] = i;
                eventMapTypes[ idx ].push_back(unsigned(FUSION));
                eventMapCounts[ i ][ FUSION ] += 1;
                eventMap[ idx ] = 0.0;
                
                idx[1] = i;
                idx[2] = i - 1;
                eventMapTypes[ idx ].push_back(unsigned(FUSION));
                eventMapCounts[ i ][ FUSION ] += 1;
                eventMap[ idx ] = 0.0;
            }
            // if one of the daughters undergoes polyploidizations
            else if ( j == POLYPLOIDIZATION && i * 2 <= maxChromo )
            {
                idx[1] = i * 2;
                idx[2] = i;
                eventMapTypes[ idx ].push_back(unsigned(POLYPLOIDIZATION));
                eventMapCounts[ i ][ POLYPLOIDIZATION ] += 1;
                eventMap[ idx ] = 0.0;
                
                idx[1] = i;
                idx[2] = i * 2;
                eventMapTypes[ idx ].push_back(unsigned(POLYPLOIDIZATION));
                eventMapCounts[ i ][ POLYPLOIDIZATION ] += 1;
                eventMap[ idx ] = 0.0;
            }
            // if one of the daughters undergoes demipolyploidizations
            else if ( j == DEMIPOLYPLOIDIZATION && i * 1.5 <= maxChromo && i > 1 )
            {
                if (i % 2 == 0)
                {
                    idx[1] = i * 1.5;
                    idx[2] = i;
                    eventMapTypes[ idx ].push_back(unsigned(DEMIPOLYPLOIDIZATION));
                    eventMapCounts[ i ][ DEMIPOLYPLOIDIZATION ] += 1;
                    eventMap[ idx ] = 0.0;
                    
                    idx[1] = i;
                    idx[2] = i * 1.5;
                    eventMapTypes[ idx ].push_back(unsigned(DEMIPOLYPLOIDIZATION));
                    eventMapCounts[ i ][ DEMIPOLYPLOIDIZATION ] += 1;
                    eventMap[ idx ] = 0.0;
                }
                else
                {
                    // round down
                    idx[1] = (unsigned)( (double)i * 1.5 - 0.5 );
                    idx[2] = i;
                    eventMapTypes[ idx ].push_back(unsigned(DEMIPOLYPLOIDIZATION));
                    eventMapCounts[ i ][ DEMIPOLYPLOIDIZATION ] += 1;
                    eventMap[ idx ] = 0.0;
                    
                    idx[1] = i;
                    idx[2] = (unsigned)( (double)i * 1.5 - 0.5 );
                    eventMapTypes[ idx ].push_back(unsigned(DEMIPOLYPLOIDIZATION));
                    eventMapCounts[ i ][ DEMIPOLYPLOIDIZATION ] += 1;
                    eventMap[ idx ] = 0.0;
                    
                    if ( i * 1.5 + 0.5 <= maxChromo )
                    {
                        // round up
                        idx[1] = i;
                        idx[2] = (unsigned)( (double)i * 1.5 + 0.5 );
                        eventMapTypes[ idx ].push_back(unsigned(DEMIPOLYPLOIDIZATION));
                        eventMapCounts[ i ][ DEMIPOLYPLOIDIZATION ] += 1;
                        eventMap[ idx ] = 0.0;
                        
                        idx[1] = (unsigned)( (double)i * 1.5 + 0.5 );
                        idx[2] = i;
                        eventMapTypes[ idx ].push_back(unsigned(DEMIPOLYPLOIDIZATION));
                        eventMapCounts[ i ][ DEMIPOLYPLOIDIZATION ] += 1;
                        eventMap[ idx ] = 0.0;
                    }
                }
            }
        }
    }
}


ChromosomesCladogenicBirthDeathFunction* ChromosomesCladogenicBirthDeathFunction::clone( void ) const
{
    return new ChromosomesCladogenicBirthDeathFunction( *this );
}


std::map< std::vector<unsigned>, double >  ChromosomesCladogenicBirthDeathFunction::getEventMap(double t)
{
    return eventMap;
}

const std::map< std::vector<unsigned>, double >&  ChromosomesCladogenicBirthDeathFunction::getEventMap(double t) const
{
    return eventMap;
}


void ChromosomesCladogenicBirthDeathFunction::update( void )
{
    // reset the transition matrix
    delete value;
    value = new MatrixReal( maxChromo + 1, (maxChromo + 1) * (maxChromo + 1), 0.0 );
    
    const std::vector<double>& sr = speciationRates->getValue();
    
    // assign the correct rate to each event
    for (unsigned i = 1; i <= maxChromo; i++)
    {
        std::map<std::vector<unsigned>, std::vector<unsigned> >::iterator it;
        for (it = eventMapTypes.begin(); it != eventMapTypes.end(); it++)
        {
            const std::vector<unsigned>& idx = it->first;
            if (idx[0] == i)
            {
                // reset all rates to 0.0
                eventMap[ idx ] = 0.0;
                (*value)[ idx[0] ][ (maxChromo + 1) * idx[1] + idx[2] ] = 0.0;
            }
        }

        for (it = eventMapTypes.begin(); it != eventMapTypes.end(); it++)
        {
            const std::vector<unsigned>& idx = it->first;
            if (idx[0] == i)
            {
                std::vector<unsigned> event_types = it->second;
                for (size_t e = 0; e < event_types.size(); e++)
                {
                    double speciation_rate = 0.0;
                
                    // check for NaN values
                    if (sr[ event_types[e] ] == sr[ event_types[e] ])
                    {
                        speciation_rate = sr[ event_types[e] ];
                    }
                    
                    // normalize for all possible instances of this event type
                    double v = ( speciation_rate / eventMapCounts[ i ][ event_types[e] ] );
                    
                    // save the rate in the rate matrix
                    (*value)[ idx[0] ][ (maxChromo + 1) * idx[1] + idx[2] ] += v;
                    
                    // save the rate in the event map
                    eventMap[ idx ] += v;
                }
            }
        }
    }
}


void ChromosomesCladogenicBirthDeathFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == speciationRates)
    {
        speciationRates = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    
}
