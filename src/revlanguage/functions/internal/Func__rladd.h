#ifndef Func__rladd_H
#define Func__rladd_H

#include "Procedure.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    template <typename firstValType, typename secondValType, typename retType>
    class Func__rladd : public Procedure {
        
    public:
        Func__rladd( void );
        
        // Basic utility functions
        Func__rladd*                                    clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevPtr<RevVariable>                                execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
        
    private:
        
    };
    
}

#include "StringUtilities.h"

/** default constructor */
template <typename firstValType, typename secondValType, typename retType>
RevLanguage::Func__rladd<firstValType, secondValType, retType>::Func__rladd( void ) : Procedure( ) {
    
}


/** Clone object */
template <typename firstValType, typename secondValType, typename retType>
RevLanguage::Func__rladd<firstValType, secondValType, retType>* RevLanguage::Func__rladd<firstValType, secondValType, retType>::clone( void ) const {
    
    return new Func__rladd( *this );
}


template <typename firstValType, typename secondValType, typename retType>
RevLanguage::RevPtr<RevLanguage::RevVariable> RevLanguage::Func__rladd<firstValType, secondValType, retType>::execute() {
    
    const firstValType& firstArg = static_cast<const firstValType &>( this->args[0].getVariable()->getRevObject() );
    const secondValType& secondArg = static_cast<const secondValType &>( this->args[1].getVariable()->getRevObject() );
    retType* value = firstArg.add( secondArg );
    
    return new RevVariable( value );
}


/* Get argument rules */
template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::ArgumentRules& RevLanguage::Func__rladd<firstValType, secondValType, retType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "first" , firstValType::getClassTypeSpec() , ArgumentRule::BY_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "second", secondValType::getClassTypeSpec(), ArgumentRule::BY_REFERENCE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


template <typename firstValType, typename secondValType, typename retType>
const std::string& RevLanguage::Func__rladd<firstValType, secondValType, retType>::getClassType(void) {
    
    static std::string revClassType = "Func__rladd<" + firstValType::getClassType() + "," + secondValType::getClassType() + "," + retType::getClassType() + ">";
    
	return revClassType;
}

/* Get class type spec describing type of object */
template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func__rladd<firstValType, secondValType, retType>::getClassTypeSpec(void) {
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revClassTypeSpec;
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
