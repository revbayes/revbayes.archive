/**
 * @file
 * This file contains the implementation of NucleotideState, which is
 * the base class for nucleotide character data types in RevBayes.
 *
 * @brief Implementation of NucleotideState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "NucleotideState.h"
#include "RbUtil.h"



/** Default constructor */
NucleotideState::NucleotideState(void) : CharacterStateDiscrete( 4 ) {

}


/** Set the value */
void NucleotideState::addState(const char s) {

    char c = tolower(s);
    if (c == 'u')
        c = 't';
        
	if (c == 'a')
		{
		value[0] = true;
		}
	else if (c == 'c')
		{
		value[1] = true;
		}
	else if (c == 'm')
		{
		value[0] = true;
		value[1] = true;
		}
	else if (c == 'g')
		{
		value[2] = true;
		}
	else if (c == 'r')
		{
		value[0] = true;
		value[2] = true;
		}
	else if (c == 's')
		{
		value[1] = true;
		value[2] = true;
		}
	else if (c == 'v')
		{
		value[0] = true;
		value[1] = true;
		value[2] = true;
		}
	else if (c == 't')
		{
		value[3] = true;
		}
	else if (c == 'w')
		{
		value[0] = true;
		value[3] = true;
		}
	else if (c == 'y')
		{
		value[1] = true;
		value[3] = true;
		}
	else if (c == 'h')
		{
		value[0] = true;
		value[1] = true;
		value[3] = true;
		}
	else if (c == 'k')
		{
		value[2] = true;
		value[3] = true;
		}
	else if (c == 'd')
		{
		value[0] = true;
		value[2] = true;
		value[3] = true;
		}
	else if (c == 'b')
		{
		value[1] = true;
		value[2] = true;
		value[3] = true;
		}
	else if (c == 'n' || c == '-')
		{
		value[0] = true;
		value[1] = true;
		value[2] = true;
		value[3] = true;
		}
    if (c == '-')
        isGapState = true;
}


/** Get class name of object */
const std::string& NucleotideState::getClassName(void) { 
    
    static std::string rbClassName = "Nucleotide";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& NucleotideState::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( CharacterStateDiscrete::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Set the value */
void NucleotideState::setState(const char s) {

    char c = tolower(s);
    if (c == 'u')
        c = 't';
        
	if (c == 'a')
		{
		value[0] = true;
		value[1] = false;
		value[2] = false;
		value[3] = false;
		}
	else if (c == 'c')
		{
		value[0] = false;
		value[1] = true;
		value[2] = false;
		value[3] = false;
		}
	else if (c == 'm')
		{
		value[0] = true;
		value[1] = true;
		value[2] = false;
		value[3] = false;
		}
	else if (c == 'g')
		{
		value[0] = false;
		value[1] = false;
		value[2] = true;
		value[3] = false;
		}
	else if (c == 'r')
		{
		value[0] = true;
		value[1] = false;
		value[2] = true;
		value[3] = false;
		}
	else if (c == 's')
		{
		value[0] = false;
		value[1] = true;
		value[2] = true;
		value[3] = false;
		}
	else if (c == 'v')
		{
		value[0] = true;
		value[1] = true;
		value[2] = true;
		value[3] = false;
		}
	else if (c == 't')
		{
		value[0] = false;
		value[1] = false;
		value[2] = false;
		value[3] = true;
		}
	else if (c == 'w')
		{
		value[0] = true;
		value[1] = false;
		value[2] = false;
		value[3] = true;
		}
	else if (c == 'y')
		{
		value[0] = false;
		value[1] = true;
		value[2] = false;
		value[3] = true;
		}
	else if (c == 'h')
		{
		value[0] = true;
		value[1] = true;
		value[2] = false;
		value[3] = true;
		}
	else if (c == 'k')
		{
		value[0] = false;
		value[1] = false;
		value[2] = true;
		value[3] = true;
		}
	else if (c == 'd')
		{
		value[0] = true;
		value[1] = false;
		value[2] = true;
		value[3] = true;
		}
	else if (c == 'b')
		{
		value[0] = false;
		value[1] = true;
		value[2] = true;
		value[3] = true;
		}
	else if (c == 'n' || c == '-')
		{
		value[0] = true;
		value[1] = true;
		value[2] = true;
		value[3] = true;
		}
    if (c == '-')
        isGapState = true;
}

