/**
 * @file
 * This file contains the declaration of Sequence, an
 * abstract base class for different types of character vectors.
 *
 * @brief Declaration of Sequence
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Sequence_H
#define Sequence_H

#include "ConstantMemberObject.h"
#include "Vector.h"
#include "VectorString.h"

#include <ostream>
#include <string>
#include <vector>

class Character;


const std::string Sequence_name = "Sequence";

class Sequence : public Vector {
    
public:
    Sequence(const std::string& elemType, const std::string tname="");                              //!< Set type spec of container from type of elements
    
    RbPtr<Character>                        operator[](size_t i);                                   //!< Index op allowing change
    const RbPtr<Character>                  operator[](size_t i) const;                             //!< Const index op
    
    // Basic utility functions you have to override
    Sequence*                               clone(void) const;                                      //!< Clone object
    const VectorString&                     getClass(void) const;                                   //!< Get class
    const TypeSpec&                         getTypeSpec(void) const;                                //!< Get language type of the object
    void                                    printValue(std::ostream& o) const;                      //!< Print value for user
    std::string                             richInfo(void) const;                                   //!< Complete info about object
    
    
    // Sequence functions
    void                                    addCharacter( RbPtr<Character> newChar );                     //!< Push back a new character
    RbPtr<Character>                        getCharacter(size_t index) const;                       //!< Get the character at position index
    size_t                                  getNumberOfCharacters(void) const;                      //!< How many characters
    const std::string&                      getTaxonName(void) const;                               //!< Return the name of the character vector
    void                                    setTaxonName(std::string tn);                           //!< Set the taxon name
    
    
private:
    std::string                             taxonName;                                              //!< Name of the taxon for this vector of characters               
    
    static const TypeSpec                   typeSpec;
};

#endif