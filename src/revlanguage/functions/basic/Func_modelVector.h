#ifndef Func_modelVector_H
#define Func_modelVector_H

#include "RlFunction.h"
#include <map>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Func_modelVector: function creating model vectors
     *
     * This templated function constructs vectors and is used for language
     * constructs such as "v( x1, x2, ..., xn)" and "[ x1, x2, ..., xn ]" when
     * the elements are non-abstract model objects with non-abstract value types.
     */
    template <typename valType>
    class Func_modelVector :  public Function {
        
    public:
        Func_modelVector(void);                                                                 //!< Default constructor
        
        // Basic utility functions
        Func_modelVector*           clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
        
        // Execute function
        RevPtr<Variable>            execute(void);                                              //!< Execute function
        
    };
    
}


#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "ModelVector.h"
#include "RbUtil.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "VectorFunction.h"


/** Default constructor */
template <typename valType>
RevLanguage::Func_modelVector<valType>::Func_modelVector() :
    Function()
{
}


/** Clone object */
template <typename valType>
RevLanguage::Func_modelVector<valType>* RevLanguage::Func_modelVector<valType>::clone( void ) const
{
    return new Func_modelVector( *this );
}


/** Execute function: create deterministic ModelVector<valType> object */
template <typename valType>
RevLanguage::RevPtr<RevLanguage::Variable> RevLanguage::Func_modelVector<valType>::execute( void )
{
    std::vector<const RevBayesCore::TypedDagNode<typename valType::valueType>* > params;
    for ( size_t i = 0; i < args.size(); i++ )
    {
        const valType &val = static_cast<const valType &>( args[i].getVariable()->getRevObject() );
        params.push_back( val.getDagNode() );
    }
    
    RevBayesCore::VectorFunction<typename valType::valueType>* func = new RevBayesCore::VectorFunction<typename valType::valueType>( params );
    
    DeterministicNode< RevBayesCore::RbVector<typename valType::valueType> >* detNode = new DeterministicNode< RevBayesCore::RbVector<typename valType::valueType> >("", func, this->clone());
    
    ModelVector<valType>* theVector = new ModelVector<valType>( detNode );
    
    return new Variable( theVector );
}


/** Get argument rules */
template <typename valType>
const RevLanguage::ArgumentRules& RevLanguage::Func_modelVector<valType>::getArgumentRules( void ) const
{
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "", valType::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new Ellipsis (     valType::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object (static version) */
template <typename valType>
const std::string& RevLanguage::Func_modelVector<valType>::getClassType( void )
{
    static std::string revType = "Func_modelVector<" + valType::getClassType() + ">";
    
	return revType;
}


/** Get Rev type spec of object (static version) */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_modelVector<valType>::getClassTypeSpec( void )
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &Function::getClassTypeSpec() );
    
	return revTypeSpec;
}


/** Get Rev type spec of object (dynamic version) */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_modelVector<valType>::getTypeSpec( void ) const
{
    return this->getClassTypeSpec();
}


/** Get return type */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_modelVector<valType>::getReturnType( void ) const
{
    return ModelVector<valType>::getClassTypeSpec();
}


#endif

