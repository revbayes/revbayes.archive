#include "CodonState.h"
#include "RbException.h"

#include <sstream>

using namespace RevBayesCore;

/** Default constructor */
CodonState::CodonState(size_t n) : DiscreteCharacterState( 64 ),
    is_gap( false ),
    is_missing( false ),
    index_single_state( 0 ),
    num_observed_states( 0 ),
    state(64)
{
    
    setState("---");
}


/** Constructor that sets the observation */
CodonState::CodonState(const std::string &s) : DiscreteCharacterState( 64 ),
    is_gap( false ),
    is_missing( false ),
    index_single_state( 0 ),
    num_observed_states( 0 ),
    state(64)
{
    
    setState(s);
}


///** Constructor that sets the observation */
//CodonState::CodonState(size_t index) : DiscreteCharacterState( 64 )
//{
//    
//    setStateByIndex(index);
//}


/* Clone object */
CodonState* CodonState::clone(void) const
{
    
    return new CodonState( *this );
}

AminoAcidState CodonState::getAminoAcidState( void ) const
{
    
    AminoAcidState aa;
    
    std::vector<unsigned int> codon_pos = getTripletStates();
    
    // test if the first codon position is an 'A'
    if ( (codon_pos[2] & 0x100) == 0x100 )
    {
        
        // test if the second codon position is an 'A'
        if ( (codon_pos[1] & 0x010) == 0x010 )
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos[0] & 0x001) == 0x001 || (codon_pos[0] & 0x004) == 0x004 )
            {
                // we have a Lysine
                aa.addState("K");
            }
            else if ( (codon_pos[0] & 0x002) == 0x002 || (codon_pos[0] & 0x008) == 0x008 )
            {
                // we have a Asparagine
                aa.addState("N");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else if ( (codon_pos[1] & 0x020) == 0x020 ) // test if the second codon position is an 'C'
        {
                
            // we have a Threonine
            aa.addState("T");
            
        }
        else if ( (codon_pos[1] & 0x040) == 0x040 ) // test if the second codon position is an 'G'
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos[0] & 0x001) == 0x001 || (codon_pos[0] & 0x004) == 0x004 )
            {
                // we have a Arginine
                aa.addState("R");
            }
            else if ( (codon_pos[0] & 0x002) == 0x002 || (codon_pos[0] & 0x008) == 0x008 )
            {
                // we have a Serine
                aa.addState("S");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else if ( (codon_pos[1] & 0x080) == 0x080 ) // test if the second codon position is an 'T'
        {
            
            // test if the third codon position is an 'A' or a 'C' or a 'T'
            if ( (codon_pos[0] & 0x001) == 0x001 || (codon_pos[0] & 0x002) == 0x002 || (codon_pos[0] & 0x008) == 0x008 )
            {
                // we have a Isoleucine
                aa.addState("I");
            }
            else if ( (codon_pos[0] & 0x004) == 0x004 )
            {
                // we have a Methionine
                aa.addState("M");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else
        {
            throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
        }
        
    }
    else if ( (codon_pos[2] & 0x200) == 0x200 ) // test if the first codon position is a 'C'
    {
        
        // test if the second codon position is an 'A'
        if ( (codon_pos[1] & 0x010) == 0x010 )
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos[0] & 0x001) == 0x001 || (codon_pos[0] & 0x004) == 0x004 )
            {
                // we have a Glutamine
                aa.addState("Q");
            }
            else if ( (codon_pos[0] & 0x002) == 0x002 || (codon_pos[0] & 0x008) == 0x008 )
            {
                // we have a Histidine
                aa.addState("H");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else if ( (codon_pos[1] & 0x020) == 0x020 ) // test if the second codon position is an 'C'
        {
            
            // we have a Proline
            aa.addState("P");
            
        }
        else if ( (codon_pos[1] & 0x040) == 0x040 ) // test if the second codon position is an 'G'
        {
            
            // we have a Arginine
            aa.addState("R");
            
        }
        else if ( (codon_pos[1] & 0x080) == 0x080 ) // test if the second codon position is an 'T'
        {
            
            // we have a Leucine
            aa.addState("L");
            
        }
        else
        {
            throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
        }
        
    }
    else if ( (codon_pos[2] & 0x400) == 0x400 ) // test if the first codon position is a 'G'
    {
        
        // test if the second codon position is an 'A'
        if ( (codon_pos[1] & 0x010) == 0x010 )
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos[0] & 0x001) == 0x001 || (codon_pos[0] & 0x004) == 0x004 )
            {
                // we have a Glutamic Acid
                aa.addState("E");
            }
            else if ( (codon_pos[0] & 0x002) == 0x002 || (codon_pos[0] & 0x008) == 0x008 )
            {
                // we have a Aspartic Acid
                aa.addState("D");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else if ( (codon_pos[1] & 0x020) == 0x020 ) // test if the second codon position is an 'C'
        {
            
            // we have a Alanine
            aa.addState("A");
            
        }
        else if ( (codon_pos[1] & 0x040) == 0x040 ) // test if the second codon position is an 'G'
        {
            
            // we have a Glycine
            aa.addState("G");
            
        }
        else if ( (codon_pos[1] & 0x080) == 0x080 ) // test if the second codon position is an 'T'
        {
            
            // we have a Valine
            aa.addState("V");
            
        }
        else
        {
            throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
        }
        
    }
    else if ( (codon_pos[2] & 0x800) == 0x800 ) // test if the first codon position is an 'T'
    {
        
        // test if the second codon position is an 'A'
        if ( (codon_pos[1] & 0x010) == 0x010 )
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos[0] & 0x001) == 0x001 || (codon_pos[0] & 0x004) == 0x004 )
            {
                // we have a stopp codon
                aa.addState("?");
            }
            else if ( (codon_pos[0] & 0x002) == 0x002 || (codon_pos[0] & 0x008) == 0x008 )
            {
                // we have a Tyrosine
                aa.addState("Y");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else if ( (codon_pos[1] & 0x020) == 0x020 ) // test if the second codon position is an 'C'
        {
            
            // we have a Serine
            aa.addState("S");
            
        }
        else if ( (codon_pos[1] & 0x040) == 0x040 ) // test if the second codon position is an 'G'
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos[0] & 0x001) == 0x001 )
            {
                // we have a stopp codon
                aa.addState("?");
            }
            else if ( (codon_pos[0] & 0x004) == 0x004 )
            {
                // we have a Tryptophan
                aa.addState("W");
            }
            else if ( (codon_pos[0] & 0x002) == 0x002 || (codon_pos[0] & 0x008) == 0x008 )
            {
                // we have a Cysteine
                aa.addState("C");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else if ( (codon_pos[1] & 0x080) == 0x080 ) // test if the second codon position is an 'T'
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos[0] & 0x001) == 0x001 || (codon_pos[0] & 0x004) == 0x004)
            {
                // we have a Leucine
                aa.addState("L");
            }
            else if ( (codon_pos[0] & 0x002) == 0x002 || (codon_pos[0] & 0x008) == 0x008 )
            {
                // we have a Phenylalanine
                aa.addState("F");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else
        {
            throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
        }
        
    }
    else
    {
        throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
    }

    
    return aa;
}


std::string CodonState::getDataType( void ) const
{
    return "CODON";
}


std::string CodonState::getStateLabels( void ) const
{
    
    static const std::string stateLabels = "AAA...TTT";
    
    return stateLabels;
}

std::string CodonState::getStringValue(void) const
{
    
    if ( isMissingState() )
    {
        return "?";
    }
    
    if ( isGapState() )
    {
        return "-";
    }
    
    std::vector<unsigned int> codon_pos = getTripletStates();
    
    std::string str_val = "";
    
    for (size_t i=0; i<3; ++i)
    {
        
        switch ( codon_pos[i] )
        {
            case 0x0:
                str_val += "-";
                break;
            case 0x1:
                str_val += "A";
                break;
            case 0x2:
                str_val += "C";
                break;
            case 0x3:
                str_val += "M";
                break;
            case 0x4:
                str_val += "G";
                break;
            case 0x5:
                str_val += "R";
                break;
            case 0x6:
                str_val += "S";
                break;
            case 0x7:
                str_val += "V";
                break;
            case 0x8:
                str_val += "T";
                break;
            case 0x9:
                str_val += "W";
                break;
            case 0xA:
                str_val += "Y";
                break;
            case 0xB:
                str_val += "H";
                break;
            case 0xC:
                str_val += "K";
                break;
            case 0xD:
                str_val += "D";
                break;
            case 0xE:
                str_val += "B";
                break;
            case 0xF:
                str_val += "N";
                break;
                
            default:
                str_val += "?";
        }
    }
    
    return str_val;
}


std::vector<unsigned int> CodonState::getTripletStates( void ) const
{
    
    
    std::vector<unsigned int> codon_pos = std::vector<unsigned int>(3,0);
    codon_pos[0] = getStateIndex() % 4;
    codon_pos[1] = int(getStateIndex() / 4) % 4;
    codon_pos[2] = int(getStateIndex() / 16) % 4;
    
    return codon_pos;
}


bool CodonState::isStopCodon( void ) const 
{
    
    std::vector<unsigned int> codon_pos = getTripletStates();
    
    
    if ( (codon_pos[2] & 0x800) == 0x800 ) // test if the first codon position is an 'T'
    {
        
        // test if the second codon position is an 'A'
        if ( (codon_pos[1] & 0x010) == 0x010 )
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos[0] & 0x001) == 0x001 || (codon_pos[0] & 0x004) == 0x004 )
            {
                // we have a stopp codon
                return true;
            }
            
        }
        else if ( (codon_pos[1] & 0x040) == 0x040 ) // test if the second codon position is an 'G'
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos[0] & 0x001) == 0x001 )
            {
                // we have a stopp codon
                return true;
            }
            
        }
        
    }
    
    
    return false;
}


bool CodonState::isGapState( void ) const
{
    return is_gap;
}


bool CodonState::isMissingState( void ) const
{
    return is_missing;
}


void CodonState::setGapState( bool tf )
{
    is_gap = tf;
}


void CodonState::setMissingState( bool tf )
{
    is_missing = tf;
}


void CodonState::setState(const std::string &symbol)
{
    
    size_t current_state = 0;
    
    /* A C G T */
    char s1 = char( toupper( symbol[2] ) );
    
    switch ( s1 )
    {
            case 'A':
            current_state += 0;
            break;
            case 'C':
            current_state += 1;
            break;
            case 'G':
            current_state += 2;
            break;
            case 'T':
            current_state += 3;
            break;
            
        default:
            current_state = 65;
    }
    
    /* A C G T */
    char s2 = char( toupper( symbol[1] ) );
    
    switch ( s2 )
    {
            case 'A':
            current_state += 0;
            break;
            case 'C':
            current_state += 4;
            break;
            case 'G':
            current_state += 8;
            break;
            case 'T':
            current_state += 12;
            break;
            
        default:
            current_state = 65;
    }
    
    /* A C G T */
    char s3 = char( toupper( symbol[0] ) );
    
    switch ( s3 )
    {
            case 'A':
            current_state += 0;
            break;
            case 'C':
            current_state += 16;
            break;
            case 'G':
            current_state += 32;
            break;
            case 'T':
            current_state += 48;
            break;
            
        default:
            current_state = 65;
    }
    
    state.clear();
    state.set( current_state );
    index_single_state = current_state;
    num_observed_states = 1;
}


void CodonState::addState(const std::string &symbol)
{
    ++num_observed_states;
    
    std::string labels = getStateLabels();
    size_t pos = labels.find(symbol);
    
    state.set( pos );
    index_single_state = pos;
}



RbBitSet CodonState::getState(void) const
{
    return state;
}


void CodonState::setToFirstState(void)
{
    num_observed_states = 1;
    index_single_state = 0;
    state.clear();
    state.set( 0 );
}


void CodonState::setStateByIndex(size_t index)
{
    
    num_observed_states = 1;
    index_single_state = index;
    state.clear();
    state.set( index );
}


