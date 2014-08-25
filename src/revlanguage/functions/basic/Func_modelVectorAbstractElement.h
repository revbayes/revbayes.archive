#ifndef Func_modelVectorAbstractElement_H
#define Func_modelVectorAbstractElement_H

#include "Function.h"
#include <map>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Func_modelVectorAbstractElement: function creating model vectors
     *
     * This templated function constructs vectors and is used for language
     * constructs such as "v( x1, x2, ..., xn)" and "[ x1, x2, ..., xn ]" when
     * the elements are abstract model objects.
     */
    template <typename valType>
    class Func_modelVectorAbstractElement :  public Function {
        
    public:
        Func_modelVectorAbstractElement(void);                                                          //!< Default constructor
        
        // Basic utility functions
        Func_modelVectorAbstractElement*    clone(void) const;                                          //!< Clone the object
        static const std::string&           getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&              getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&                getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&                     getReturnType(void) const;                                  //!< Get type of return value
        
        // Execute function
        RevPtr<Variable>                    execute(void);                                              //!< Execute function
        
    };
    
}


#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "ModelVectorAbstractElement.h"
#include "RbUtil.h"
#include "RbVectorFunction.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


/** Default constructor */
template <typename valType>
RevLanguage::Func_modelVectorAbstractElement<valType>::Func_modelVectorAbstractElement() :
Function()
{
}


/** Clone object */
template <typename valType>
RevLanguage::Func_modelVectorAbstractElement<valType>* RevLanguage::Func_modelVectorAbstractElement<valType>::clone( void ) const
{
    return new Func_modelVectorAbstractElement( *this );
}


/** Execute function: create deterministic ModelVectorAbstractElement<valType> object */
template <typename valType>
RevLanguage::RevPtr<RevLanguage::Variable> RevLanguage::Func_modelVectorAbstractElement<valType>::execute( void )
{
    std::vector<const RevBayesCore::TypedDagNode<typename valType::valueType>* > params;
    for ( size_t i = 0; i < args.size(); i++ )
    {
        const valType &val = static_cast<const valType &>( args[i].getVariable()->getRevObject() );
        params.push_back( val.getDagNode() );
    }
    
    RevBayesCore::RbVectorFunction<typename valType::valueType>* func = new RevBayesCore::RbVectorFunction<typename valType::valueType>( params );
    
    DeterministicNode<RevBayesCore::RbVector<typename valType::valueType> >* detNode = new DeterministicNode<RevBayesCore::RbVector<typename valType::valueType> >("", func, this->clone());
    
    ModelVectorAbstractElement<valType>* theVector = new ModelVectorAbstractElement<valType>( detNode );
    
    return new Variable( theVector );
}


/** Get argument rules */
template <typename valType>
const RevLanguage::ArgumentRules& RevLanguage::Func_modelVectorAbstractElement<valType>::getArgumentRules( void ) const
{
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "", true, valType::getClassTypeSpec() ) );
        argumentRules.push_back( new Ellipsis (     valType::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object (static version) */
template <typename valType>
const std::string& RevLanguage::Func_modelVectorAbstractElement<valType>::getClassType( void )
{
    static std::string revType = "Func_modelVectorAbstractElement<" + valType::getClassType() + ">";
    
	return revType;
}


/** Get Rev type spec of object (static version) */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_modelVectorAbstractElement<valType>::getClassTypeSpec( void )
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &Function::getClassTypeSpec() );
    
	return revTypeSpec;
}


/** Get Rev type spec of object (dynamic version) */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_modelVectorAbstractElement<valType>::getTypeSpec( void ) const
{
    return this->getClassTypeSpec();
}


/** Get return type */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_modelVectorAbstractElement<valType>::getReturnType( void ) const
{
    return ModelVectorAbstractElement<valType>::getClassTypeSpec();
}


#endif

