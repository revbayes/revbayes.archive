#ifndef Func__vectorIndexOperator_H
#define Func__vectorIndexOperator_H

#include "RlTypedFunction.h"

#include <string>

namespace RevLanguage {
    
    template <typename valType>
    class Func__vectorIndexOperator : public TypedFunction<valType> {
        
    public:
        Func__vectorIndexOperator( void );
        
        // Basic utility functions
        Func__vectorIndexOperator*                                      clone(void) const;                              //!< Clone the object
        static const std::string&                                       getClassType(void);                             //!< Get class name
        static const TypeSpec&                                          getClassTypeSpec(void);                         //!< Get class type spec
        const TypeSpec&                                                 getTypeSpec(void) const;                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction<typename valType::valueType>*       createFunction(void) const ;                    //!< Create a new internal function object
        const ArgumentRules&                                            getArgumentRules(void) const;                   //!< Get argument rules
        
    private:
        
    };
    
}

#include "RlDeterministicNode.h"
#include "ModelVector.h"
#include "VectorIndexOperator.h"
#include "TypedDagNode.h"

/** default constructor */
template <typename valType>
RevLanguage::Func__vectorIndexOperator<valType>::Func__vectorIndexOperator( void ) : TypedFunction<valType>( )
{
    
}


/** Clone object */
template <typename valType>
RevLanguage::Func__vectorIndexOperator<valType>* RevLanguage::Func__vectorIndexOperator<valType>::clone( void ) const
{
    
    return new Func__vectorIndexOperator( *this );
}


template <typename valType>
RevBayesCore::TypedFunction< typename valType::valueType >* RevLanguage::Func__vectorIndexOperator<valType>::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<typename valType::valueType> >* v = static_cast<const ModelVector<valType> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<int>* index = static_cast<const Natural &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::VectorIndexOperator<typename valType::valueType> *func = new RevBayesCore::VectorIndexOperator<typename valType::valueType>(v, index);
    
    return func;
}


/* Get argument rules */
template <typename valType>
const RevLanguage::ArgumentRules& RevLanguage::Func__vectorIndexOperator<valType>::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "v"    , ModelVector<valType>::getClassTypeSpec(), "The vector.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new ArgumentRule( "index", Natural::getClassTypeSpec()             , "The index.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
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


template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func__vectorIndexOperator<valType>::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

#endif
