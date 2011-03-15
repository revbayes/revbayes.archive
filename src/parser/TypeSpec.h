/**
 * @file
 * This file contains the declaration of TypeSpec, which is
 * used to hold type specifications.
 *
 * @brief Declaration of TypeSpec
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
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
                            TypeSpec(const std::string& elemType, bool ref=false);              //!< Type of single object/element
                            TypeSpec(const std::string& elemType, int elemDim, bool ref=false); //!< Type of plate/container

        // Equals operator
        bool                operator==(const TypeSpec& x) const;                                //!< Equals operator
        bool                operator!=(const TypeSpec& x) const { return operator==(x); }       //!< Not equals operator

        // Regular functions
        int                 getDim(void) const { return dim; }                                  //!< Get element dimensions
        const std::string&  getType(void) const { return type; }                                //!< Get element type
        bool                isReference(void) const { return reference; }                       //!< Is it a reference type?
        void                setReference(bool refFlag) { reference = refFlag; }                 //!< Set or reset the reference flag
        std::string         toString(void) const;                                               //!< Express as a string

    private:
        // Member variables
        const std::string&  type;               //!< The type of the element or elements
        int                 dim;                //!< The element dimensions (0 = scalar, 1 = vector etc)
        bool                reference;          //!< Flags whether it is a reference type
};

#endif
