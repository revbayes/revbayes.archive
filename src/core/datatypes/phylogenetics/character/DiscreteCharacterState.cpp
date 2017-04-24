#include "DiscreteCharacterState.h"
#include "RbException.h"

#include <string>
#include <vector>


using namespace RevBayesCore;



/** Constructor */
DiscreteCharacterState::DiscreteCharacterState(size_t n) : CharacterState(),
    index_single_state( 0 ),
    num_observed_states( 0 ),
//    state(n),
    weighted( false )
{


}

/** Equals comparison */
bool DiscreteCharacterState::operator==(const CharacterState& x) const
{

    const DiscreteCharacterState* derivedX = dynamic_cast<const DiscreteCharacterState*>( &x );

    if (derivedX != NULL)
    {
        return derivedX->getState() == getState();
    }

    return false;
}


/** Not equals comparison */
bool DiscreteCharacterState::operator!=(const CharacterState& x) const
{

    return !operator==(x);
}


bool DiscreteCharacterState::operator<(const CharacterState &x) const
{

    const DiscreteCharacterState* derivedX = static_cast<const DiscreteCharacterState*>(&x);
    if ( derivedX != NULL )
    {
        const RbBitSet& myState = getState();
        const RbBitSet& yourState = derivedX->getState();
        return ( myState < yourState );
    }

    return false;
}


void DiscreteCharacterState::operator++( void )
{

    if ( isAmbiguous() == true )
    {
        throw RbException("Cannot increment an ambiguous character.");
    }

    // unset the current state
    getState().unset( index_single_state );

    // incremement our state index;
    ++index_single_state;

    // now set the bit of the state
    getState().set(index_single_state);

}


void DiscreteCharacterState::operator++( int i )
{

    if ( isAmbiguous() == true )
    {
        throw RbException("Cannot increment an ambiguous character.");
    }

    // unset the current state
    getState().unset( index_single_state );

    // incremement our state index;
    ++index_single_state;

    // now set the bit of the state
    getState().set(index_single_state);

}

void DiscreteCharacterState::operator+=( int i )
{

    if ( isAmbiguous() == true )
    {
        throw RbException("Cannot increment an ambiguous character.");
    }

    // unset the current state
    getState().unset( index_single_state );

    // incremement our state index;
    index_single_state += i;

    // now set the bit of the state
    getState().set(index_single_state);

}

void DiscreteCharacterState::operator--( void )
{

    if ( isAmbiguous() == true )
    {
        throw RbException("Cannot decrement an ambiguous character.");
    }

    // unset the current state
    getState().unset( index_single_state );

    // incremement our state index;
    --index_single_state;

    // now set the bit of the state
    getState().set(index_single_state);

}


void DiscreteCharacterState::operator--( int i )
{

    if ( isAmbiguous() == true )
    {
        throw RbException("Cannot decrement an ambiguous character.");
    }

    // unset the current state
    getState().unset( index_single_state );

    // incremement our state index;
    --index_single_state;

    // now set the bit of the state
    getState().set(index_single_state);

}

void DiscreteCharacterState::operator-=( int i )
{

    if ( isAmbiguous() == true )
    {
        throw RbException("Cannot decrement an ambiguous character.");
    }

    // unset the current state
    getState().unset( index_single_state );

    // incremement our state index;
    index_single_state -= i;

    // now set the bit of the state
    getState().set(index_single_state);

}


void DiscreteCharacterState::addState(const std::string &symbol)
{
    ++num_observed_states;

    std::string labels = getStateLabels();
    size_t pos = labels.find(symbol);

    getState().set( pos );
    index_single_state = pos;
}

size_t DiscreteCharacterState::getNumberObservedStates(void) const
{
    return num_observed_states;
}


size_t DiscreteCharacterState::getNumberOfStates(void) const
{
    return getState().size();
}

std::string DiscreteCharacterState::getStateDescription(void) const
{
    return getStringValue();
}

std::vector<std::string> DiscreteCharacterState::getStateDescriptions(void) const
{
    std::string state_labels = getStateLabels();
    
    std::vector<std::string> state_descriptions(state_labels.size());
    for (size_t i = 0; i < state_labels.size(); i++)
    {
        state_descriptions[i] = state_labels[i];
    }
    
    return state_descriptions;
}


size_t DiscreteCharacterState::getStateIndex(void) const
{
    if ( isAmbiguous() == true )
    {
        throw RbException("Cannot get the index of an ambiguous state.");
    }

    return index_single_state;
}



std::string DiscreteCharacterState::getStringValue(void) const
{

    if ( isMissingState() )
    {
        return "?";
    }

    if ( isGapState() )
    {
        return "-";
    }

    std::string tmp_val = "";
    std::string labels = getStateLabels();
    size_t size = labels.size();
    for (size_t i = 0; i < size; ++i)
    {
        bool isSet = getState().isSet( i );
        if (isSet)
        {
            tmp_val += labels[i];
        }
    }

    std::string val = "";
    if ( tmp_val.size() > 1 )
    {
        val = "(" + tmp_val.substr(0,1);
        for ( size_t i = 1; i<tmp_val.size(); ++i )
        {
            val += " " + tmp_val.substr(i,1);
        }
        val += ")";
    }
    else
    {
        val = tmp_val;
    }

    return val;
}


bool DiscreteCharacterState::isAmbiguous( void ) const
{

    return isMissingState() || isGapState() || ( getNumberObservedStates() > 1 );
}


bool DiscreteCharacterState::isStateSet(size_t index) const
{
    return getState().isSet(index);
}


void DiscreteCharacterState::setToFirstState(void)
{
    num_observed_states = 1;
    index_single_state = 0;
    
    getState().clearBits();
    getState().set( 0 );
}


void DiscreteCharacterState::setStateByIndex(size_t index)
{

    num_observed_states = 1;
    index_single_state = index;
    getState().clearBits();
    getState().set( index );
}



void DiscreteCharacterState::setState(const std::string &s)
{

    std::string labels = getStateLabels();

    num_observed_states = 0;
    getState().clearBits();

    for (size_t i = 0; i < s.size(); i++)
    {
        ++num_observed_states;

        size_t pos = labels.find(s[i]);
        getState().set(pos);
        index_single_state = pos;
    }

}


const std::vector<double> DiscreteCharacterState::getWeights() const {
    
    // @Bastien: Change this function as needed!beta
    return std::vector<double>(num_observed_states, 1.0);
}



bool DiscreteCharacterState::isWeighted() const {

  return weighted;

}


void DiscreteCharacterState::setWeighted(bool wd) {

  weighted = wd;
  return ;

}
