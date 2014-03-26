/**
 * @file
 * This file contains the declaration of the RevLanguage control variable wrapper, which is
 * the RevLanguage abstract base class for all language objects wrapping core datatypes that are used
 * to control the analysis, such as monitors, moves, etc.
 *
 * @brief Declaration of RlControlVariableWrapper
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-03 17:51:49 +0200 (Fri, 03 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-08-06
 *
 * $Id: RbLanguageObject.h 1734 2012-08-03 15:51:49Z hoehna $
 */

#ifndef RlControlVariableWrapper_H
#define RlControlVariableWrapper_H

#include "RbLanguageObject.h"
#include "TypedDagNode.h"

namespace RevLanguage {
    
    template <typename rbType>
    class RlControlVariableWrapper : public RbLanguageObject {
        
    public:
        virtual                            ~RlControlVariableWrapper(void);                                         //!< Virtual destructor
        
        RlControlVariableWrapper&           operator=(const RlControlVariableWrapper& x);                           //!< Assignment operator    
        
        // the value type definition
        typedef rbType valueType;
        
        // Basic utility functions you have to override (also getClassTypeSpec()!)
        virtual RlControlVariableWrapper*   clone(void) const = 0;                                                  //!< Clone object
        virtual void                        constructInternalObject(void) = 0;                                      //!< We construct the a new internal object.
        static const std::string&           getClassName(void);                                                     //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                 //!< Get class type spec
        virtual const TypeSpec&             getTypeSpec(void) const = 0;                                            //!< Get the type spec of the instance
        virtual void                        printStructure(std::ostream& o) const {}                                //!< Print structure of language object for user
        virtual void                        printValue(std::ostream& o) const = 0;                                  //!< Print value for user

                
        // getters and setters
        const rbType&                       getValue(void) const;
        void                                setValue(const rbType& x);
        
    protected:
        RlControlVariableWrapper(void);
        RlControlVariableWrapper(rbType *v);
        RlControlVariableWrapper(const RlControlVariableWrapper &v);
        
        rbType*                             value;
    };
    
}


#include "ConstantNode.h"
#include "TypeSpec.h"

template <typename rbType>
RevLanguage::RlControlVariableWrapper<rbType>::RlControlVariableWrapper() : RbLanguageObject(), value( NULL ) {
    
}



template <typename rbType>
RevLanguage::RlControlVariableWrapper<rbType>::RlControlVariableWrapper(rbType *v) : RbLanguageObject(), value( v ) {
    
}



template <typename rbType>
RevLanguage::RlControlVariableWrapper<rbType>::RlControlVariableWrapper(const RlControlVariableWrapper &v) : RbLanguageObject( v ), value( NULL ) {
    
    if ( v.value != NULL ) {
        value = v.value->clone();
    }
}



template <typename rbType>
RevLanguage::RlControlVariableWrapper<rbType>::~RlControlVariableWrapper() {
    
    delete value;
}


template <typename rbType>
RevLanguage::RlControlVariableWrapper<rbType>& RevLanguage::RlControlVariableWrapper<rbType>::operator=(const RlControlVariableWrapper &v) {
    
    if ( this != &v ) {
        // free the memory
        delete value;
        value = NULL;
        
        // create own copy
        if ( v.value != NULL ) {
            value = v.value->clone();
        }
    }
    
    return *this;
}


template <typename rbType>
const std::string& RevLanguage::RlControlVariableWrapper<rbType>::getClassName(void) { 
    
    static std::string className = "Control variable";
    
	return className; 
}


/** Get class type spec describing type of object */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::RlControlVariableWrapper<rlType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


template <typename rbType>
const rbType& RevLanguage::RlControlVariableWrapper<rbType>::getValue( void ) const {
    
    return *value;
}



template <typename rbType>
void RevLanguage::RlControlVariableWrapper<rbType>::setValue(const rbType &x) {
    
    // free memory
    delete value;
    value = new rbType( x );
}


#endif
