#ifndef Func__rlvectorIndexOperator_H
#define Func__rlvectorIndexOperator_H

#include "Function.h"

#include <string>

namespace RevLanguage {
    
    template <typename valType>
    class Func__rlvectorIndexOperator :  public Function {
        
    public:
        Func__rlvectorIndexOperator( void );
        
        // Basic utility functions
        Func__rlvectorIndexOperator*                    clone(void) const;                                                              //!< Clone the object
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
#include "WorkspaceVector.h"
#include "TypedDagNode.h"

/** default constructor */
template <typename valType>
RevLanguage::Func__rlvectorIndexOperator<valType>::Func__rlvectorIndexOperator( void ) : Function( )
{
    
}


/** Clone object */
template <typename valType>
RevLanguage::Func__rlvectorIndexOperator<valType>* RevLanguage::Func__rlvectorIndexOperator<valType>::clone( void ) const
{
    
    return new Func__rlvectorIndexOperator( *this );
}


template <typename valType>
RevLanguage::RevPtr<RevLanguage::Variable> RevLanguage::Func__rlvectorIndexOperator<valType>::execute()
{
    
    // get the arguments
    const WorkspaceVector<valType> &theVector = static_cast<const WorkspaceVector<valType> &>( this->args[0].getVariable()->getRevObject() );
    RevBayesCore::TypedDagNode<int>* index = static_cast<const Natural &>( this->args[1].getVariable()->getRevObject() ).getDagNode();

    // retrieve the i-th element from the vector
    valType* theElement = theVector.getElement( index->getValue() );
    
    // return the element
    return new Variable( theElement );
}


/* Get argument rules */
template <typename valType>
const RevLanguage::ArgumentRules& RevLanguage::Func__rlvectorIndexOperator<valType>::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "v", true, WorkspaceVector<valType>::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "index", true, Natural::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


template <typename valType>
const std::string& RevLanguage::Func__rlvectorIndexOperator<valType>::getClassType(void)
{
    
    static std::string revClassType = "Func__rlvectorIndexOperator<" + valType::getClassType() + ">";
    
	return revClassType;
}

/* Get class type spec describing type of object */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func__rlvectorIndexOperator<valType>::getClassTypeSpec(void)
{
    
    static TypeSpec revClassTypeSpec = TypeSpec( Func__rlvectorIndexOperator<valType>::getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revClassTypeSpec;
}


/* Get return type */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func__rlvectorIndexOperator<valType>::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = valType::getClassTypeSpec();
    
    return returnTypeSpec;
}


template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func__rlvectorIndexOperator<valType>::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

#endif
