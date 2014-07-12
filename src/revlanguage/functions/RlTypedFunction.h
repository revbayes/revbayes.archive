/**
 * @file
 * This file contains the declaration of TypedFunction, which is
 * the interface and abstract base class for RevLanguage typed function objects.
 * This interface mainly implements the execute function by delegating the call to the derived class'
 * createFunction and then creating a deterministic node with it.
 *
 * @brief Declaration of TypedFunction.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-08-03 17:51:49 +0200 (Fri, 03 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface Distribution
 * @package distributions
 * @since Version 1.0, 2012-08-06
 *
 * $Id: Function.h 1734 2012-08-03 15:51:49Z hoehna $
 */

#ifndef RlTypedFunction_H
#define RlTypedFunction_H

#include "RlFunction.h"
#include "TypedFunction.h"

#if 0
namespace RevLanguage {
    
    template <typename valueType>
    class TypedFunction : public Function {
        
    public:
        virtual                                         ~TypedFunction(void);                                                                  //!< Destructor
        TypedFunction<valueType>(const TypedFunction<valueType> &x);                                                                //!< Copy constuctor
        
        virtual valueType*                              execute(void);                                                   //!< Create a random variable from this distribution        
        
        // Basic utility functions you have to override
        virtual TypedFunction<valueType>*               clone(void) const = 0;                                                              //!< Clone object
        static const std::string&                       getClassType(void);                                                                 //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                             //!< Get class type spec
        
        
        // Class-specific functions you have to override
        virtual RevBayesCore::TypedFunction<typename valueType::valueType>*     createFunction(void) const = 0;                                                 //!< Create a random variable from this distribution
        
        
    protected:
        TypedFunction<valueType>(void);                                                                                                 //!< Basic constructor
        
    };
    
}

#include "DeterministicNode.h"

template <typename valueType>
RevLanguage::TypedFunction<valueType>::TypedFunction() : Function() {
    
}



template <typename valueType>
RevLanguage::TypedFunction<valueType>::TypedFunction( const TypedFunction<valueType> &d ) : Function(d) {
    
}



template <typename valueType>
RevLanguage::TypedFunction<valueType>::~TypedFunction() {
    
}



template <typename valueType>
valueType* RevLanguage::TypedFunction<valueType>::execute(void) { 
    
    RevBayesCore::TypedFunction<typename valueType::valueType>* d = createFunction();
    RevBayesCore::TypedDagNode<typename valueType::valueType>* rv  = new RevBayesCore::DeterministicNode<typename valueType::valueType>("", d);
    
    return new valueType(rv);
}



/* Get class name of object */
template <typename valueType>
const std::string& RevLanguage::TypedFunction<valueType>::getClassType(void) { 
    
    static std::string rbClassName = "Function<"+ valueType::getClassType() +">";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename valueType>
const RevLanguage::TypeSpec& RevLanguage::TypedFunction<valueType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

#endif

#endif

