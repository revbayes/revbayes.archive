#ifndef Func__scalarVectorDiv_H
#define Func__scalarVectorDiv_H

#include "ModelVector.h"
#include "RlTypedFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    
    /**
     * The RevLanguage wrapper of the scalar-vector-Division function.
     *
     * The RevLanguage wrapper of the scalar-vector-Division function connects
     * the variables/parameters of the function and creates the internal ScalarVectorDivision object.
     * RevBayes > a <- [1,2,3]
     * RevBayes > b <- 2
     * RevBayes > a / b
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-07-31, version 1.0
     *
     */
    template <typename firstValType, typename secondValType, typename retType>
    class Func__scalarVectorDiv :  public TypedFunction< ModelVector< retType > > {
        
    public:
        Func__scalarVectorDiv( void );
        
        // Basic utility functions
        Func__scalarVectorDiv*                                          clone(void) const;                              //!< Clone the object
        static const std::string&                                       getClassType(void);                             //!< Get class name
        static const TypeSpec&                                          getClassTypeSpec(void);                         //!< Get class type spec
        const TypeSpec&                                                 getTypeSpec(void) const;                        //!< Get the type spec of the instance
        
        // Implementations of pure virtual functions of the base class(es)
        RevBayesCore::TypedFunction< RevBayesCore::RbVector<typename retType::valueType> >*       createFunction(void) const ;                    //!< Create a random variable from this distribution
        const ArgumentRules&                                            getArgumentRules(void) const;                   //!< Get argument rules
        const TypeSpec&                                                 getReturnType(void) const;                      //!< Get type of return value
        
    private:
        
    };
    
}

#include "ScalarVectorDivision.h"
#include "RlDeterministicNode.h"
#include "StringUtilities.h"
#include "TypedDagNode.h"

/** default constructor */
template <typename firstValType, typename secondValType, typename retType>
RevLanguage::Func__scalarVectorDiv<firstValType, secondValType, retType>::Func__scalarVectorDiv( void ) : TypedFunction<ModelVector<retType> >( )
{
    
}


/** Clone object */
template <typename firstValType, typename secondValType, typename retType>
RevLanguage::Func__scalarVectorDiv<firstValType, secondValType, retType>* RevLanguage::Func__scalarVectorDiv<firstValType, secondValType, retType>::clone( void ) const
{
    
    return new Func__scalarVectorDiv( *this );
}


template <typename firstValType, typename secondValType, typename retType>
RevBayesCore::TypedFunction< RevBayesCore::RbVector<typename retType::valueType> >* RevLanguage::Func__scalarVectorDiv<firstValType, secondValType, retType>::createFunction( void ) const
{
    RevBayesCore::TypedDagNode<typename firstValType::valueType>* firstArg = static_cast<const firstValType &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<typename secondValType::valueType> >* secondArg = static_cast<const ModelVector<secondValType> &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::ScalarVectorDivision<typename firstValType::valueType, typename secondValType::valueType, typename retType::valueType> *func = new RevBayesCore::ScalarVectorDivision<typename firstValType::valueType, typename secondValType::valueType, typename retType::valueType>(firstArg, secondArg);
    
    return func;
}


/* Get argument rules */
template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::ArgumentRules& RevLanguage::Func__scalarVectorDiv<firstValType, secondValType, retType>::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "first" , firstValType::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "second", ModelVector<secondValType>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


template <typename firstValType, typename secondValType, typename retType>
const std::string& RevLanguage::Func__scalarVectorDiv<firstValType, secondValType, retType>::getClassType(void)
{
    static std::string revType = "Func__scalarVectorDiv<" + firstValType::getClassType() + "," + secondValType::getClassType() + "," + retType::getClassType() + ">";
    
	return revType;
}

/* Get class type spec describing type of object */
template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func__scalarVectorDiv<firstValType, secondValType, retType>::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( Func__scalarVectorDiv<firstValType, secondValType, retType>::getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func__scalarVectorDiv<firstValType, secondValType, retType>::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = retType::getClassTypeSpec();
    
    return returnTypeSpec;
}


template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func__scalarVectorDiv<firstValType, secondValType, retType>::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

#endif
