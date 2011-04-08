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
                            
                            TypeSpec(const std::string& type);                                  //!< Simple constructor with type conversion
                            TypeSpec(const std::string& objType,
                                     int                objDim,
                                     bool               ref      = false,
                                     bool               constVar = false);                      //!< Complete constructor

        // Operators
        bool                operator==(const TypeSpec& x) const;                                //!< Equals operator
        bool                operator!=(const TypeSpec& x) const { return !operator==(x); }      //!< Not equals operator
                            operator std::string(void) const;                                   //!< Type conversion to string

        // Regular functions
        int                 getDim(void) const { return dim; }                                  //!< Get field dimensions
        const std::string&  getType(void) const { return type; }                                //!< Get object type
        bool                isConstant(void) const { return constant; }                         //!< Is it a constant type?
        bool                isReference(void) const { return reference; }                       //!< Is it a reference type?
        void                setDim(int i) { if (i >= 0) dim = i; }                              //!< Set field dimensions
        void                setReference(bool refFlag) { reference = refFlag; }                 //!< Set or reset the reference flag
        std::string         toString(void) const;                                               //!< Express as a string

    private:
        // Member variables
        const std::string&  type;                                                               //!< The type of the object or objects
        int                 dim;                                                                //!< The field dimensions (0 = scalar, 1 = vector etc)
        bool                reference;                                                          //!< Flags whether it is a reference type
        bool                constant;                                                           //!< Flags whether it is a constant
};


        // Global functions using the class
        std::ostream&       operator<<(std::ostream& o, const TypeSpec& x);                     //!< Overloaded output operator
        std::string         operator+(const std::string& o, const TypeSpec& x);                 //!< Concatenation to std::string

#endif
