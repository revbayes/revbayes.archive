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

class Sequence : public AbstractVector {
    
public:
    Sequence(const std::string& elemType, const std::string tname="");                              //!< Set type spec of container from type of elements
    
    Character&                              operator[](size_t i);                                   //!< Index op allowing change
    const Character&                        operator[](size_t i) const;                             //!< Const index op
    
    // Basic utility functions you have to override
    Sequence*                               clone(void) const;                                      //!< Clone object
    const VectorString&                     getClass(void) const;                                   //!< Get class
    const TypeSpec&                         getTypeSpec(void) const;                                //!< Get language type of the object
    void                                    printValue(std::ostream& o) const;                      //!< Print value for user
    std::string                             richInfo(void) const;                                   //!< Complete info about object
    
    // Container functions you have to override
    void                                    pop_back(void);                                         //!< Drop element at back
    void                                    pop_front(void);                                        //!< Drop element from front
    void                                    push_back(RbObject* x);                                 //!< Append element to end
    void                                    push_front(RbObject* x);                                //!< Add element in front
    void                                    resize(size_t n);                                       //!< Resize to new AbstractVector of length n
    size_t                                  size(void) const;                                       //!< get the number of elements in the AbstractVector
    
    void                                    clear(void);                                            //!< Clear
    Character*                              getElement(size_t index) const;                         //!< Get element
    void                                    setElement(const size_t index, RbLanguageObject* elem); //!< Set element with type conversion
    void                                    sort(void);                                             //!< sort the AbstractVector
    void                                    unique(void);                                           //!< removes consecutive duplicates

    
    // Sequence functions
    void                                    addCharacter( Character* newChar );                     //!< Push back a new character
    Character*                              getCharacter(size_t index) const;                       //!< Get the character at position index
    size_t                                  getNumberOfCharacters(void) const;                      //!< How many characters
    const std::string&                      getTaxonName(void) const;                               //!< Return the name of the character vector
    void                                    setTaxonName(std::string tn);                           //!< Set the taxon name
    
    
private:
    std::string                             taxonName;                                              //!< Name of the taxon for this vector of characters               
    Vector                                  characters;                                             //!< The vector containing the characters of the sequence
    
    static const TypeSpec                   typeSpec;
};

#endif