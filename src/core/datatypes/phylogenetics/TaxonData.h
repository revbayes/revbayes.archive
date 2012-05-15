/**
 * @file
 * This file contains the declaration of TaxonData, an
 * abstract base class for different types of character vectors.
 *
 * @brief Declaration of TaxonData
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef TaxonData_H
#define TaxonData_H

#include "MemberObject.h"
#include "RbVector.h"

#include <ostream>
#include <string>
#include <vector>

class Character;


class TaxonData : public MemberObject {
    
    public:
                                                TaxonData(const std::string& elemType, const std::string tname=""); //!< Set type spec of container from type of elements
        
        Character&                              operator[](size_t i);                                               //!< Index op allowing change
        const Character&                        operator[](size_t i) const;                                         //!< Const index op
        
        // Basic utility functions you have to override
        TaxonData*                              clone(void) const;                                                  //!< Clone object
        static const std::string&               getClassName(void);                                                 //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                             //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                            //!< Get language type of the object
        void                                    printValue(std::ostream& o) const;                                  //!< Print value for user
        
        // Member object methods 
        RbPtr<RbLanguageObject>                 executeSimpleMethod(const std::string& name, const std::vector<const RbObject*>& args);            //!< Override to map member methods to internal functions
        const MemberRules&                      getMemberRules(void) const;                                         //!< Get member rules
        virtual const MethodTable&              getMethods(void) const;                                             //!< Get methods
        void                                    setSimpleMemberValue(const std::string &name, const RbPtr<const RbLanguageObject> &var);    //!< Set a member variable. We catch here setting of variable nodes
       
        // TaxonData functions
        void                                    addCharacter( Character* newChar );                                 //!< Push back a new character
        const Character&                        getCharacter(size_t index) const;                                   //!< Get the character at position index
        Character&                              getCharacter(size_t index);                                         //!< Get the character at position index (non-const to return non-const character)
        size_t                                  getNumberOfCharacters(void) const;                                  //!< How many characters
        const std::string&                      getTaxonName(void) const;                                           //!< Return the name of the character vector
        void                                    setTaxonName(std::string tn);                                       //!< Set the taxon name
 
        
    private:
        std::string                             taxonName;                                                          //!< Name of the taxon for this vector of characters               
        RbVector                                sequence;
    

};

#endif