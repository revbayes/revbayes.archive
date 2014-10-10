/**
 * @file
 * This file contains the declaration of Integer, which is
 * a RevBayes wrapper around a regular int.
 *
 * @brief Declaration of Integer
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */

#ifndef Integer_H
#define Integer_H

#include "ModelObject.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Real;

    class Integer : public ModelObject<int> {

    public:
        Integer(void);                                                                                          //!< Default constructor
        Integer(RevBayesCore::TypedDagNode<int> *v);                                                            //!< Constructor from DAG node
        Integer(int v);                                                                                         //!< Constructor from int
        Integer(unsigned int v);                                                                                //!< Constructor from unsigned int

        // Basic operator functions
        virtual RevObject*              add(const RevObject &rhs) const;                                        //!< Addition operator used for example in '+=' statements
        Integer*                        add(const Integer &rhs) const;                                          //!< Addition operator used for example in '+=' statements
        Real*                           add(const Real &rhs) const;                                             //!< Addition operator used for example in '+=' statements
        void                            decrement(void);                                                        //!< Decrement operator used for example in 'a--' statements
        virtual RevObject*              divide(const RevObject &rhs) const;                                     //!< Division operator used for example in '/=' statements
        Real*                           divide(const Integer &rhs) const;                                       //!< Division operator used for example in '/=' statements
        Real*                           divide(const Real &rhs) const;                                          //!< Division operator used for example in '/=' statements
        void                            increment(void);                                                        //!< Increment operator used for example in 'a++' statements
        virtual RevObject*              multiply(const RevObject &rhs) const;                                   //!< Multiplication operator used for example in '*=' statements
        Integer*                        multiply(const Integer &rhs) const;                                     //!< Multiplication operator used for example in '*=' statements
        Real*                           multiply(const Real &rhs) const;                                        //!< Multiplication operator used for example in '*=' statements
        virtual RevObject*              subtract(const RevObject &rhs) const;                                   //!< Subtraction operator used for example in '-=' statements
        Integer*                        subtract(const Integer &rhs) const;                                     //!< Subtraction operator used for example in '-=' statements
        Real*                           subtract(const Real &rhs) const;                                        //!< Subtraction operator used for example in '-=' statements

        // Basic utility functions
        virtual Integer*                clone(void) const;                                                      //!< Clone object
        virtual RevObject*              convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        virtual const TypeSpec&         getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual bool                    isConvertibleTo(const TypeSpec& type, bool once) const;                 //!< Is convertible to type?
        
    };
    
}

#endif

