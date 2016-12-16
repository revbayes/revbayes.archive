#ifndef Func_append_H
#define Func_append_H

#include "Procedure.h"

#include <string>

namespace RevLanguage {
    
    /**
     * @brief Func_append: function appending elements to a vector vectors
     *
     * This templated function appends elements to a vector.
     */
    template <typename valType>
    class Func_append : public Procedure {
        
    public:
        Func_append(void);                                                                 //!< Default constructor
        
        // Basic utility functions
        Func_append*                    clone(void) const;                                          //!< Clone the object
        static const std::string&       getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                     //!< Get class type spec
        std::string                     getFunctionName(void) const;                                //!< Get the primary name of the function in Rev
        const TypeSpec&                 getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&            getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&                 getReturnType(void) const;                                  //!< Get type of return value
        
        // Execute function
        RevPtr<RevVariable>             execute(void);                                              //!< Execute function
        
    };
    
}


#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "RbUtil.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "WorkspaceVector.h"


/** Default constructor */
template <typename valType>
RevLanguage::Func_append<valType>::Func_append() : Procedure()
{
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
template <typename valType>
RevLanguage::Func_append<valType>* RevLanguage::Func_append<valType>::clone( void ) const
{
    return new Func_append( *this );
}


/** Execute function: assemble a workspace vector */
template <typename valType>
RevLanguage::RevPtr<RevLanguage::RevVariable> RevLanguage::Func_append<valType>::execute( void )
{
    RevBayesCore::RbVector<valType> params;
    ModelVector<valType> *the_vector = static_cast<const ModelVector<valType> &>( args[0].getVariable()->getRevObject() ).clone();
    for ( size_t i = 1; i < args.size(); ++i )
    {
        
        const valType &val = static_cast<const valType &>( args[i].getVariable()->getRevObject() );
        the_vector->push_back( val );
    }
    
    return new RevVariable( the_vector );
}


/** Get argument rules */
template <typename valType>
const RevLanguage::ArgumentRules& RevLanguage::Func_append<valType>::getArgumentRules( void ) const
{
    static ArgumentRules argument_rules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( rules_set == false )
    {
        argument_rules.push_back( new ArgumentRule( "vector", ModelVector<valType>::getClassTypeSpec(), "The original vector.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        argument_rules.push_back( new ArgumentRule( "x", valType::getClassTypeSpec(), "First value to append", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        argument_rules.push_back( new Ellipsis ( "More values ...", valType::getClassTypeSpec() ) );
        rules_set = true;
    }
    
    return argument_rules;
}


/** Get Rev type of object (static version) */
template <typename valType>
const std::string& RevLanguage::Func_append<valType>::getClassType( void )
{
    static std::string rev_type = "Func_append<" + valType::getClassType() + ">";
    
    return rev_type;
}


/** Get Rev type spec of object (static version) */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_append<valType>::getClassTypeSpec( void )
{
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), &Function::getClassTypeSpec() );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
template <typename valType>
std::string RevLanguage::Func_append<valType>::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "append";
    
    return f_name;
}


/** Get Rev type spec of object (dynamic version) */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_append<valType>::getTypeSpec( void ) const
{
    return this->getClassTypeSpec();
}


/** Get return type */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_append<valType>::getReturnType( void ) const
{
    return WorkspaceVector<valType>::getClassTypeSpec();
}


#endif

