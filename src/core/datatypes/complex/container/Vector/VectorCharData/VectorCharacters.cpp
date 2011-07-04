/**
 * @file
 * This file contains the implementation of VectorCharacters, an
 * abstract base class for different types of character vectors.
 *
 * @brief Implementation of VectorCharacterObservations
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#include "Character.h"
#include "CharacterStateDiscrete.h"
#include "CondLike.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "VectorCharacters.h"



/** Constructor with element type, used to properly construct vectors */
VectorCharacters::VectorCharacters(const std::string& elemType) : Vector(elemType) {

}


/** Subscript operator */
Character& VectorCharacters::operator[](size_t i) {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *( static_cast<Character*>(elements[i]) );
}


/** Subscript const operator */
const Character& VectorCharacters::operator[](size_t i) const {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *( static_cast<Character*>(elements[i]) );
}


/** Get class vector describing type of object */
const VectorString& VectorCharacters::getClass(void) const {

    static VectorString rbClass = VectorString(VectorCharacters_name) + Vector::getClass();
    return rbClass;
}


/** Initialize and return a conditional likelihood for this state */
CondLike* VectorCharacters::getCondLike(void) {

    // get the number of characters and states
    size_t nc = elements.size();
    size_t ns = (*this)[0].getNumStates();
    if ( ns == 0 || nc == 0 )
        throw ( RbException("Problem creating conditional likelihood vector") );
    
    // allocate a new conditional likelihood object
    CondLike* cl = new CondLike(nc, ns);
    
    // fill in the conditional likelihood vector
    double* p = &(*cl)[0];
    for (size_t i=0; i<elements.size(); i++) 
        {
        CharacterStateDiscrete* discP = dynamic_cast<CharacterStateDiscrete*>( &(*this)[i] );
        std::vector<bool> s = discP->getStateVector();
        for (size_t i=0; i<ns; i++)
            {
            if (s[i] == true)
                p[i] = 1.0;
            else
                p[i] = 0.0;
            }
        p += ns;
        }
    return cl;
}


/** Push back a new character */
void VectorCharacters::push_back( Character* newChar ) {

    if ( newChar == NULL || !newChar->isType( elementType ) )
        throw RbException( "Inappropriate character type" );
    
    elements.push_back( newChar );
    length[0]++;
}

