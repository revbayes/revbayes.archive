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
                            TypeSpec(const std::string& objType);                               //!< Complete constructor
                            TypeSpec(const TypeSpec& ts);                                       //!< Copy Constructor
    
        // Operators
        bool                operator==(const TypeSpec& x) const;                                //!< Equals operator
        bool                operator!=(const TypeSpec& x) const { return !operator==(x); }      //!< Not equals operator
                            operator std::string(void) const;                                   //!< Type conversion to string

        // Regular functions
        const std::string&  getType(void) const { return type; }                                //!< Get object type
        std::string         toString(void) const;                                               //!< Express as a string

    private:
        // Member variables
        const std::string   type;                                                               //!< The type of the object or objects
};


        // Global functions using the class
        std::ostream&       operator<<(std::ostream& o, const TypeSpec& x);                     //!< Overloaded output operator
        std::string         operator+(const std::string& o, const TypeSpec& x);                 //!< Concatenation to std::string

#endif
