/**
 * @file
 * This file contains the declaration of Real, which is the
 * primitive RevBayes type for real numbers.
 *
 * @brief Declaration of Real
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

#ifndef Real_H
#define Real_H

#include "RlModelVariableWrapper.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

/**
 * Real is the class used to hold a real value. Internally, the real is represented by
 * a double.
 *
 * @note Some functions are virtual because RealPos is derived from Real
 */

namespace RevLanguage {
    
    class Integer;

    class Real : public RlModelVariableWrapper<double> {

        public:

        Real(void);                                                                                             //!< Default constructor (0.0)
        Real(RevBayesCore::TypedDagNode<double> *v);                                                            //!< Construct from DAG node
        Real(double v);                                                                                         //!< Construct from double
        Real(int v);                                                                                            //!< Construct from int 
        Real(const Real& x);                                                                                    //!< Copy constructor
        
        // Basic operator functions
        virtual RbLanguageObject*       add(const RbLanguageObject &rhs) const;                                 //!< Addition operator used for example in '+=' statements
        Real*                           add(const Real &rhs) const;                                             //!< Addition operator used for example in '+=' statements
        Real*                           add(const Integer &rhs) const;                                          //!< Addition operator used for example in '+=' statements
        virtual RbLanguageObject*       divide(const RbLanguageObject &rhs) const;                              //!< Division operator used for example in '/=' statements
        Real*                           divide(const Real &rhs) const;                                          //!< Division operator used for example in '/=' statements
        Real*                           divide(const Integer &rhs) const;                                       //!< Division operator used for example in '/=' statements
        virtual RbLanguageObject*       multiply(const RbLanguageObject &rhs) const;                            //!< Multiplication operator used for example in '*=' statements
        Real*                           multiply(const Real &rhs) const;                                        //!< Multiplication operator used for example in '*=' statements
        Real*                           multiply(const Integer &rhs) const;                                     //!< Multiplication operator used for example in '*=' statements
        virtual RbLanguageObject*       subtract(const RbLanguageObject &rhs) const;                            //!< Subtraction operator used for example in '-=' statements
        Real*                           subtract(const Real &rhs) const;                                        //!< Subtraction operator used for example in '-=' statements
        Real*                           subtract(const Integer &rhs) const;                                     //!< Subtraction operator used for example in '-=' statements

        // Basic utility functions
        virtual Real*                   clone(void) const;                                                      //!< Clone object
        virtual RbLanguageObject*       convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&       getClassName(void);                                                     //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        virtual const TypeSpec&         getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual bool                    isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type?
        void                            printValue(std::ostream& o) const;                                      //!< Print value (for user)

    
    };
    
}

#endif

