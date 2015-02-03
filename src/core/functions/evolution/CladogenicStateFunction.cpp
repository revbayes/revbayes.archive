//
//  CladogenicStateFunction.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 1/19/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "CladogenicStateFunction.h"
#include "BiogeographicCladoEvent.h"
#include "RbException.h"

using namespace RevBayesCore;

CladogenicStateFunction::CladogenicStateFunction(const TypedDagNode< RbVector<double> > *ep, const TypedDagNode< RbVector<double> > *er, unsigned nc, unsigned ns):
    TypedFunction<MatrixReal>( new MatrixReal( pow(ns,nc), pow(ns,nc*2), 0.0) ),
    eventProbs( ep ),
    eventRates( er ),
    numCharacters(nc),
    numStates(2),
    numIntStates(pow(numStates,nc)),
    numEventTypes( ep->getValue().size() + 1 )
{
    // add the lambda parameter as a parent
    addParameter( eventProbs );
    addParameter( eventRates );
    
    buildBits();
    buildEventMap();
    
    update();
}

CladogenicStateFunction::~CladogenicStateFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}

std::vector<unsigned> CladogenicStateFunction::bitAllopatryComplement( const std::vector<unsigned>& mask, const std::vector<unsigned>& base )
{
    std::vector<unsigned> ret = mask;
    for (size_t i = 0; i < base.size(); i++)
    {
        if (base[i] == 1)
            ret[i] = 0;
    }
    return ret;
}

void CladogenicStateFunction::bitCombinations(std::vector<std::vector<unsigned> >& comb, std::vector<unsigned> array, int i, std::vector<unsigned> accum)
{
    if (i == array.size()) // end recursion
    {
        unsigned n = sumBits(accum);

        if ( n == 0 || n == sumBits(array) )
            ;  // ignore all-0, all-1 vectors
        else
            comb.push_back(accum);
    }
    else
    {
        unsigned b = array[i];
        std::vector<unsigned> tmp0(accum);
        std::vector<unsigned> tmp1(accum);
        tmp0.push_back(0);
        bitCombinations(comb,array,i+1,tmp0);
        if (b == 1)
        {
            tmp1.push_back(1);
            bitCombinations(comb,array,i+1,tmp1);
        }
    }
}

unsigned CladogenicStateFunction::sumBits(const std::vector<unsigned>& b)
{
    unsigned n = 0;
    for (int i = 0; i < b.size(); i++)
        n += b[i];
    return n;
}

unsigned CladogenicStateFunction::bitsToState( const std::vector<unsigned>& b )
{
    unsigned n = 0;
    for (int i = 0; i < b.size(); i++)
    {
        unsigned j = numCharacters - i - 1;
        n += b[i] * pow(numStates, j);
    }
    return n;
}

std::string CladogenicStateFunction::bitsToString( const std::vector<unsigned>& b )
{
    std::stringstream ss;
    for (size_t i = 0; i < b.size(); i++)
    {
        ss << b[i];
    }
    return ss.str();
}

void CladogenicStateFunction::buildBits( void )
{
    bits.resize(numIntStates);
    for (size_t i = 0; i < numIntStates; i++) {
        std::vector<unsigned> b(numCharacters, 0);
        unsigned v = i;
        for (int j = numCharacters - 1; j >= 0; j--)
        {
            b[j] = v % numStates;
            v /= numStates;
            if (v == 0)
                break;
        }
        bits[i] = b;
    }
}

void CladogenicStateFunction::buildEventMap( void ) {
    
    int allopatricEvent = 0;
    int sympatricEvent = 1;
    
    eventMapCounts.resize(numIntStates, std::vector<unsigned>(numEventTypes, 0));
    
    // get L,R states per A state
    std::vector<unsigned> idx(3);
    for (size_t i = 0; i < numIntStates; i++) {
        
        idx[0] = i;
        
//        std::cout << "State " << i << "\n";
//        std::cout << "Bits  " << bitsToString(bits[i]) << "\n";
        
        // get on bits for A
        const std::vector<unsigned>& ba = bits[i];
        std::vector<unsigned> on;
        for (size_t j = 0; j < ba.size(); j++)
        {
            if (ba[j] == 1)
                on.push_back(j);
        }
        
        std::vector<unsigned> bl(numCharacters, 0);
        std::vector<unsigned> br(numCharacters, 0);
        
        // narrow sympatry
        if (sumBits(ba) == 1)
        {
            
//            std::cout << "Narrow sympatry\n";
            idx[1] = i;
            idx[2] = i;
            eventMapTypes[ idx ] = BiogeographicCladoEvent::SYMPATRY_NARROW;
            eventMapCounts[ i ][  BiogeographicCladoEvent::SYMPATRY_NARROW ] += 1;
            eventMapProbs[ idx ] = 0.0;
            
//            std::cout << "A " << bitsToState(bits[i]) << " " << bitsToString(bits[i]) << "\n";
//            std::cout << "L " << bitsToState(bits[i]) << " " << bitsToString(bits[i]) << "\n";
//            std::cout << "R " << bitsToState(bits[i]) << " " << bitsToString(bits[i]) << "\n\n";

        }
        
        // subset/widespread sympatry
        else if (sumBits(ba) > 1)
        {
//            std::cout << "Widespread sympatry\n";
            
            idx[1] = i;
            idx[2] = i;
            eventMapTypes[ idx ] = BiogeographicCladoEvent::SYMPATRY_WIDESPREAD;
            eventMapCounts[ i ][  BiogeographicCladoEvent::SYMPATRY_WIDESPREAD ] += 1;
            eventMapProbs[ idx ] = 0.0;

//            std::cout << "A " << bitsToState(bits[i]) << " " << bitsToString(bits[i]) << "\n";
//            std::cout << "L " << bitsToState(bits[i]) << " " << bitsToString(bits[i]) << "\n";
//            std::cout << "R " << bitsToState(bits[i]) << " " << bitsToString(bits[i]) << "\n\n";
            
            
//            std::cout << "Subset sympatry (L-trunk, R-bud)\n";
            // get set of possible sympatric events for L-trunk, R-bud
            for (size_t j = 0; j < on.size(); j++)
            {
                br = std::vector<unsigned>(numCharacters, 0);
                br[ on[j] ] = 1;
                unsigned sr = bitsToState(br);
                idx[1] = i;
                idx[2] = sr;
                eventMapTypes[ idx ] = BiogeographicCladoEvent::SYMPATRY_SUBSET;
                eventMapCounts[ i ][  BiogeographicCladoEvent::SYMPATRY_SUBSET ] += 1;
                eventMapProbs[ idx ] = 0.0;
                
//                std::cout << "A " << bitsToState(bits[i]) << " " << bitsToString(bits[i]) << "\n";
//                std::cout << "L " << bitsToState(bits[i]) << " " << bitsToString(bits[i]) << "\n";
//                std::cout << "R " << bitsToState(br) << " " << bitsToString(br) << "\n\n";
                
                br[ on[j] ] = 0;
            }
        
//            std::cout << "Subset sympatry (L-bud, R-trunk)\n";
            // get set of possible sympatric events for R-trunk, L-bud
            for (size_t j = 0; j < on.size(); j++)
            {
                bl = std::vector<unsigned>(numCharacters, 0);
    
                bl[ on[j] ] = 1;
                unsigned sl = bitsToState(bl);
                idx[1] = sl;
                idx[2] = i;
                eventMapTypes[ idx ] =  BiogeographicCladoEvent::SYMPATRY_SUBSET;
                eventMapCounts[ i ][  BiogeographicCladoEvent::SYMPATRY_SUBSET ] += 1;
                eventMapProbs[ idx ] = 0.0;
                
//                std::cout << "A " << bitsToState(bits[i]) << " "<< bitsToString(bits[i]) << "\n";
//                std::cout << "L " << bitsToState(bl) << " "<< bitsToString(bl) << "\n";
//                std::cout << "R " << bitsToState(bits[i]) << " " << bitsToString(bits[i]) << "\n\n";
                
                bl[ on[j] ] = 0;
    
            }
            
            // get set of possible allopatry events
            bl = ba;
            std::vector<std::vector<unsigned> > bc;
            bitCombinations(bc, ba, 0, std::vector<unsigned>());
            
//            std::cout << "Allopatry combinations\n";
//            std::cout << "A " << bitsToState(ba) << " " << bitsToString(ba) << "\n";
            
            for (size_t j = 0; j < bc.size(); j++)
            {
                
                bl = bc[j];
                br = bitAllopatryComplement(ba, bl);
                
//                std::cout << "L " << bitsToState(bl) << " " << bitsToString(bl) << "\n";
//                std::cout << "R " << bitsToState(br) << " " << bitsToString(br) << "\n";
                
                unsigned sl = bitsToState(bl);
                unsigned sr = bitsToState(br);
                idx[1] = sl;
                idx[2] = sr;
                
                eventMapTypes[ idx ] = BiogeographicCladoEvent::ALLOPATRY;
                eventMapCounts[ i ][  BiogeographicCladoEvent::ALLOPATRY ] += 1;
                eventMapProbs[ idx ] = 0.0;
                
//                std::cout << "\n";
            }
        }
//        std::cout << "\n\n";
    }
    
//    std::cout << "------\n";
}

CladogenicStateFunction* CladogenicStateFunction::clone( void ) const
{
    return new CladogenicStateFunction( *this );
}

const std::map< std::vector<unsigned>, double >&  CladogenicStateFunction::getEventMapProbs(void) const
{
    return eventMapProbs;
}

void CladogenicStateFunction::update( void )
{
    
    // get the information from the arguments for reading the file
    const std::vector<double>& ep = eventProbs->getValue();
    const std::vector<double>& er = eventRates->getValue();
    
    // normalize tx probs
    std::vector<double> z( numIntStates, 0.0 );
    for (size_t i = 0; i < numIntStates; i++)
    {
        for (size_t j = 1; j < numEventTypes; j++)
        {
            z[i] += eventMapCounts[i][j] * ep[j-1];
        }
    }
    
    std::map<std::vector<unsigned>, unsigned>::iterator it;
    for (it = eventMapTypes.begin(); it != eventMapTypes.end(); it++)
    {
        const std::vector<unsigned>& idx = it->first;
        double v = 1.0;
        if (it->second != BiogeographicCladoEvent::SYMPATRY_NARROW)
            v = ep[ it->second - 1 ] / eventMapCounts[ idx[0] ][ it->second ];

        (*value)[ idx[0] ][ numIntStates * idx[1] + idx[2] ] = v;
        eventMapProbs[ idx ] = v;
        
    }
}



void CladogenicStateFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == eventProbs)
    {
        eventProbs = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == eventRates)
    {
        eventRates = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    
}


