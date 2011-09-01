/**
 * @file
 * This file contains the declaration of TypeSpec, which is
 * used to hold type specifications.
 *
 * @brief Declaration of TypeSpec
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-09-06, version 1.0
 *
 * $Id$
 */

#ifndef TypeSpec_H 
#define TypeSpec_H

#include <string>

class TypeSpec {
    
public:
                        TypeSpec(const std::string& objType, TypeSpec *elemType=NULL);      //!< Complete constructor
                        TypeSpec(const TypeSpec& ts);                                       //!< Copy Constructor
    
    // Operators
    TypeSpec&           operator=(const TypeSpec& x);                                       //!< Assignment operator
    bool                operator==(const TypeSpec& x) const;                                //!< Equals operator
    bool                operator!=(const TypeSpec& x) const { return !operator==(x); }      //!< Not equals operator
                        operator std::string(void) const;                                   //!< Type conversion to string

    // Regular functions
    TypeSpec*           getElementType(void) const { return elementType; }                  //!< Get the element type
    const std::string&  getType(void) const { return type; }                                //!< Get object type
    std::string         toString(void) const;                                               //!< Express as a string

private:
    // Member variables
    const std::string   baseType;                                                           //!< The base type of the object or objects
    std::string         type;                                                               //!< The full type including base and element types
    TypeSpec           *elementType;                                                        //!< The type of the elements if this is a container
};


    // Global functions using the class
    std::ostream&       operator<<(std::ostream& o, const TypeSpec& x);                     //!< Overloaded output operator
    std::string         operator+(const std::string& o, const TypeSpec& x);                 //!< Concatenation to std::string

#endif
