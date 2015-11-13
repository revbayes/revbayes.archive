#ifndef Func_rep_H
#define Func_rep_H

#include "Procedure.h"
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Function that creates a numerical repuence by replicating an element n times.
     *
     * This function is the equivalent to the 'R' function rep().
     * The function creates a vector by replicating it n times:
     *
     * a <- rep(1.23,4)
     *
     * Which gives:
     * {1.23,1.23,1.23,1.23}
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2014-08-22
     *
     */
    
    template <typename valType>
    class Func_rep : public Procedure {
        
    public:
        Func_rep();
        
        // Basic utility functions
        Func_rep*                   clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        std::string                 getFunctionName(void) const;                                //!< Get the primary name of the function in Rev
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
        
        
        RevPtr<RevVariable>         execute(void);                                              //!< Execute function
        
    };
    
}


#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "RbUtil.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"



template <typename valType>
RevLanguage::Func_rep<valType>::Func_rep() : Procedure()
{
    
}

/* Clone object */
template <typename valType>
RevLanguage::Func_rep<valType>* RevLanguage::Func_rep<valType>::clone( void ) const
{
    
    return new Func_rep( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
template <typename valType>
RevLanguage::RevPtr<RevLanguage::RevVariable> RevLanguage::Func_rep<valType>::execute( void )
{
    
    typename valType::valueType v   = static_cast<const valType &>( args[0].getVariable()->getRevObject() ).getValue();
    int                         n   = static_cast<const Natural &>( args[1].getVariable()->getRevObject() ).getValue();
    
    ModelVector<valType> *rep = new ModelVector<valType>();
    for ( int i=0; i<n; ++i )
    {
        rep->push_back( valType( v ) );
    }
    
    return new RevVariable( rep );
}


/** Get argument rules */
template <typename valType>
const RevLanguage::ArgumentRules& RevLanguage::Func_rep<valType>::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", valType::getClassTypeSpec(), "The value that we replicate.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "n", Natural::getClassTypeSpec(), "How often we replicate the value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
template <typename valType>
const std::string& RevLanguage::Func_rep<valType>::getClassType(void)
{
    
    static std::string revType = "Func_rep<" + valType::getClassType() + ">";
    
	return revType;
}


/** Get class type spec describing type of object */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_rep<valType>::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
template <typename valType>
std::string RevLanguage::Func_rep<valType>::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "rep";
    
    return f_name;
}


/** Get type spec */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_rep<valType>::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_rep<valType>::getReturnType( void ) const
{
    
    return ModelVector<valType>::getClassTypeSpec();
}


#endif


