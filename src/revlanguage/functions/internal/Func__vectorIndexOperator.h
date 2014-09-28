#ifndef Func__vectorIndexOperator_H
#define Func__vectorIndexOperator_H

#include "Function.h"

#include <string>

namespace RevLanguage {
    
    template <typename valType>
    class Func__vectorIndexOperator :  public Function {
        
    public:
        Func__vectorIndexOperator( void );
        
        // Basic utility functions
        Func__vectorIndexOperator*                      clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevPtr<Variable>                                execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
        
    private:
        
    };
    
}

#include "RlDeterministicNode.h"
#include "ModelVector.h"
#include "VectorIndexOperator.h"
#include "TypedDagNode.h"

/** default constructor */
template <typename valType>
RevLanguage::Func__vectorIndexOperator<valType>::Func__vectorIndexOperator( void ) : Function( )
{
    
}


/** Clone object */
template <typename valType>
RevLanguage::Func__vectorIndexOperator<valType>* RevLanguage::Func__vectorIndexOperator<valType>::clone( void ) const
{
    
    return new Func__vectorIndexOperator( *this );
}


template <typename valType>
RevLanguage::RevPtr<RevLanguage::Variable> RevLanguage::Func__vectorIndexOperator<valType>::execute()
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<typename valType::valueType> >* v = static_cast<const ModelVector<valType> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<int>* index = static_cast<const Natural &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::VectorIndexOperator<typename valType::valueType> *func = new RevBayesCore::VectorIndexOperator<typename valType::valueType>(v, index);
    
    DeterministicNode<typename valType::valueType> *detNode = new DeterministicNode<typename valType::valueType>("", func, this->clone());
    
    valType* value = new valType( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
template <typename valType>
const RevLanguage::ArgumentRules& RevLanguage::Func__vectorIndexOperator<valType>::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "v"    , ModelVector<valType>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "index", Natural::getClassTypeSpec()             , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


template <typename valType>
const std::string& RevLanguage::Func__vectorIndexOperator<valType>::getClassType(void)
{
    
    static std::string revClassType = "Func__vectorIndexOperator<" + valType::getClassType() + ">";
    
	return revClassType;
}

/* Get class type spec describing type of object */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func__vectorIndexOperator<valType>::getClassTypeSpec(void)
{
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revClassTypeSpec;
}


/* Get return type */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func__vectorIndexOperator<valType>::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = valType::getClassTypeSpec();
    
    return returnTypeSpec;
}


template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func__vectorIndexOperator<valType>::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

#endif
