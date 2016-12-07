#include "PomoState.h"
#include "RbException.h"
#include <assert.h>
#include <sstream>
#include <iostream>

#include <boost/lexical_cast.hpp>

using namespace RevBayesCore;

/** Default constructor */
PomoState::PomoState(void) : DiscreteCharacterState( 4 + 6 * (10 - 1) ),
    chromosome_ (""), position_(0), virtualPopulationSize_ ( 10 )
{

}

/** Constructor with virtual population size */
PomoState::PomoState(size_t vps): DiscreteCharacterState( 4 + 6 * (vps - 1) ),
    chromosome_ (""), position_(0), virtualPopulationSize_ ( vps )
{


}


/** Constructor that sets the observation */
PomoState::PomoState(const std::string &s) : DiscreteCharacterState( 4 + 6 * (10 - 1) ),
    chromosome_ ( "" ), position_( 0 ), virtualPopulationSize_ ( 10 )
{

    //assert( s <= 15 );

    setState(s);
}



/** Constructor that sets the observation and the other fields */
PomoState::PomoState(const std::string &s, const std::string chromosome, const size_t position, const size_t virtualPopulationSize ) : DiscreteCharacterState( 4 + 6 * (10 - 1) ),
    chromosome_ ( chromosome ), position_( position ), virtualPopulationSize_ ( virtualPopulationSize )
{

    setState(s);
}


PomoState* PomoState::clone( void ) const
{

    return new PomoState( *this );
}


void PomoState::setState(const std::string &symbol)
{
    /* Example with only ten states:
     A C G T A10C90 A20C80 A30C70...A90C10 A10G90 A20G80...A10T90...C10G90...C10T90...G10T90
     */

    size_t index = 0;
    if (symbol.length()==1)
    {
        if (symbol == "-")
        {
            index = 0;
        }
        else if (symbol ==  "A")
        {
            index = 1;
        }
        else if (symbol ==  "C")
        {
            index = 2;
        }
        else if (symbol ==  "G")
        {
            index = 3;
        }
        else if (symbol ==  "T")
        {
            index = 4;
        }
        else
        {
            index =  0;
        }
    }
    else if (symbol.length()!=6 )
    {
        throw RbException( "Pomo string state with fewer or more than 6 characters. Should be 6, no more, no less." );
    }
    else
    {
        std::string firstChar = symbol.substr(0,1);
        int firstFreq = atoi ( symbol.substr(1,2).c_str() );
        std::string secondChar = symbol.substr(3,1);
        int secondFreq = atoi ( symbol.substr(4,2).c_str() );
        if ( firstFreq + secondFreq > virtualPopulationSize_ )
        {
            throw RbException( "Pomo string state with frequencies that do not add up to the current virtual population size." );
        }
        if ( firstChar >= secondChar ) {
            throw RbException( "Pomo string state with first state greater or equal to second state." );
        }

        int stepSize = 100 / virtualPopulationSize_;
        int numStep = firstFreq / stepSize -1;

        if (firstChar ==  "A")
        {
            if (secondChar ==  "C")
            {
                index = 5 + numStep;
            }
            else if (secondChar ==  "G")
            {
                index = 5 + virtualPopulationSize_ - 1 + numStep;
            }
            else if (secondChar ==  "T")
            {
                index = 5 + 2*(virtualPopulationSize_ - 1) + numStep;
            }
            else
            {
                throw RbException( "Pomo string state with incorrect second state: should be A, C, G or T." );
            }
        }
        if (firstChar == "C")
        {
            if (secondChar ==  "G")
            {
                index = 5 + 3*(virtualPopulationSize_ - 1) + numStep;
            }
            else if (secondChar ==  "T")
            {
                index = 5 + 4*(virtualPopulationSize_ - 1) + numStep;
            }
            else
            {
                throw RbException( "Pomo string state with incorrect second state: should be A, C, G or T." );
            }
        }
        if (firstChar == "G")
        {
            if (secondChar ==  "T")
            {
                index = 5 + 5*(virtualPopulationSize_ - 1) + numStep;
            }
            else
            {
                throw RbException( "Pomo string state with incorrect second state: should be A, C, G or T." );
            }

        }
        else
        {
            throw RbException( "Pomo string state with incorrect first state: should be A, C, G or T." );
        }

    }

    state.clear();
    state.set( index );
    index_single_state = index;
    num_observed_states = 1;

}


std::string PomoState::getDataType( void ) const
{

    return "Pomo";
}


const std::string& PomoState::getStateLabels( void ) const
{

    static std::string labels = "A C G T ";
    std::string acgt( "ACGT" );
    std::vector< size_t > frequencies;
    int stepSize = 100 / virtualPopulationSize_;
    for (size_t i = 1; i < virtualPopulationSize_; ++i)
    {
        frequencies.push_back(i*stepSize);
    }
    for( size_t k = 0; k < acgt.size(); ++k )
    {
        char ch = acgt[k];

        for ( size_t j = k+1; j < acgt.size(); ++j )
        {
            char ch2 = acgt[j];
            for (size_t i = 0; i < virtualPopulationSize_-1; ++i)
            {
                labels += ch + boost::lexical_cast<std::string>(frequencies[i]) + ch2 + boost::lexical_cast<std::string>(frequencies[virtualPopulationSize_ - 2 - i]) + " ";
            }
        }
    }

    return labels;
}

std::string PomoState::getStringValue(void) const
{

    if ( isMissingState() )
    {
        return "?";
    }

    if ( isGapState() )
    {
        return "-";
    }

    int stepSize = 100 / virtualPopulationSize_ - 1;
    size_t index = getStateIndex();
    if (index < 5)
    {
        switch ( index )
        {
            case 0:
                return "-";
            case 1:
                return "A";
            case 2:
                return "C";
            case 3:
                return "G";
            case 4:
                return "T";
        }
    }
    int stateMinus5 = int(index) - 5;
    int typeOfPomoState = stateMinus5 / stepSize;
    int typeOfFrequency = stateMinus5 % stepSize +1;
    int freqi = typeOfFrequency*virtualPopulationSize_;
    int freqj = 100 - freqi;
    switch ( typeOfPomoState )
    {
        case 0: //AC
            return "A"+ boost::lexical_cast<std::string>(freqi) + "C" + boost::lexical_cast<std::string>(freqj) ;
        case 1: //AG
            return "A"+ boost::lexical_cast<std::string>(freqi) + "G" + boost::lexical_cast<std::string>(freqj) ;
        case 2: //AT
            return "A"+ boost::lexical_cast<std::string>(freqi) + "T" + boost::lexical_cast<std::string>(freqj) ;
        case 3: //CG
            return "C"+ boost::lexical_cast<std::string>(freqi) + "G" + boost::lexical_cast<std::string>(freqj) ;
        case 4: //CT
            return "C"+ boost::lexical_cast<std::string>(freqi) + "T" + boost::lexical_cast<std::string>(freqj) ;
        case 5: //GT
            return "G"+ boost::lexical_cast<std::string>(freqi) + "T" + boost::lexical_cast<std::string>(freqj) ;
    }
     throw RbException( "getStringValue called on a non-standard state." );
}

void PomoState::setVirtualPopulationSize(size_t populationSize)
{
    if (populationSize > 100)
    {
        throw RbException( "The virtual population size should be < 100 and should be a divisor of 100." );
    }
    if (100 % populationSize != 0)
    {
        throw RbException( "The virtual population size should be a divisor of 100." );
    }
    virtualPopulationSize_ = populationSize;
}

void PomoState::setChromosome(std::string chromosome){
  chromosome_ = chromosome;
}
void PomoState::setPosition(size_t position){
  position_ = position;
}

const std::string PomoState::getChromosome( void ){
  return chromosome_;
}

const size_t PomoState::getPosition( void ){
  return position_;
}
