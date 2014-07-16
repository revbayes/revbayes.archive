#ifndef Func__rladd_H
#define Func__rladd_H

#include "Function.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    template <typename firstValType, typename secondValType, typename retType>
    class Func__rladd :  public Function {
        
    public:
        Func__rladd( void );
        
        // Basic utility functions
        Func__rladd*                                    clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevObject*                                      execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
        
    private:
        
    };
    
}

#include "StringUtilities.h"

/** default constructor */
template <typename firstValType, typename secondValType, typename retType>
RevLanguage::Func__rladd<firstValType, secondValType, retType>::Func__rladd( void ) : Function( ) {
    
}


/** Clone object */
template <typename firstValType, typename secondValType, typename retType>
RevLanguage::Func__rladd<firstValType, secondValType, retType>* RevLanguage::Func__rladd<firstValType, secondValType, retType>::clone( void ) const {
    
    return new Func__rladd( *this );
}


template <typename firstValType, typename secondValType, typename retType>
RevLanguage::RevObject* RevLanguage::Func__rladd<firstValType, secondValType, retType>::execute() {
    
    const firstValType& firstArg = static_cast<const firstValType &>( this->args[0].getVariable()->getRevObject() );
    const secondValType& secondArg = static_cast<const secondValType &>( this->args[1].getVariable()->getRevObject() );
    
    retType* value = firstArg.clone();
    value->add( secondArg );
    
    return value;
}


/* Get argument rules */
template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::ArgumentRules& RevLanguage::Func__rladd<firstValType, secondValType, retType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "first", true, firstValType::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "second", true, secondValType::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


template <typename firstValType, typename secondValType, typename retType>
const std::string& RevLanguage::Func__rladd<firstValType, secondValType, retType>::getClassName(void) {
    
    static std::string rbClassName = "Func__rladd<" + firstValType::getClassName() + "," + secondValType::getClassName() + "," + retType::getClassName() + ">";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func__rladd<firstValType, secondValType, retType>::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass;
}


/* Get return type */
template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func__rladd<firstValType, secondValType, retType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = retType::getClassTypeSpec();
    
    return returnTypeSpec;
}


template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func__rladd<firstValType, secondValType, retType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

#endif
