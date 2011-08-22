/**
 * @file
 * This file contains the declaration of VectorCharacters, an
 * abstract base class for different types of character vectors.
 *
 * @brief Declaration of VectorCharacters
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef VectorCharacters_H
#define VectorCharacters_H

#include "Vector.h"
#include "VectorString.h"
#include <ostream>
#include <string>
#include <vector>

class Character;


const std::string VectorCharacters_name = "Character vector";

class VectorCharacters : public Vector {

public:
    virtual Character&                      operator[](size_t i);                                   //!< Index op allowing change
    virtual const Character&                operator[](size_t i) const;                             //!< Const index op

    // Basic utility functions you have to override
    virtual VectorCharacters*               clone(void) const = 0;                                  //!< Clone object
    virtual const VectorString&             getClass(void) const;                                   //!< Get class
    virtual void                            printValue(std::ostream& o) const = 0;                  //!< Print value for user

    // VectorCharacters functions
    Character*                              getElement(size_t index);                               //!< Get the character at position index
    size_t                                  getNumCharacters(void) const { return getLength(); }    //!< How many characters
    std::string                             getTaxonName(void) const { return taxonName; }          //!< Return the name of the character vector
    void                                    push_back( Character* newChar );                        //!< Push back a new character
    void                                    setTaxonName(std::string tn) { taxonName = tn; }        //!< Set the taxon name
    void                                    sort( void );                                           //!< Sort the vector    
    void                                    unique(void);                                           //!< Remove duplicates and resize the vector

    
protected:
                                            VectorCharacters(const std::string& elemType);          //!< Set type spec of container from type of elements

    
    static bool                             comparisonFunction (RbLanguageObject* i,RbLanguageObject* j);                                            //!< Utilitary function for sort

    
private:
    std::string                             taxonName;                                              //!< Name of the taxon for this vector of characters               
};

bool comparisonFunction (RbLanguageObject* i,RbLanguageObject* j);                                            //!< Utilitary function for sort

#endif
