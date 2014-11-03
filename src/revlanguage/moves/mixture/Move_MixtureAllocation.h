#ifndef Move_MixtureAllocation_H
#define Move_MixtureAllocation_H

#include "SimpleMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Rev Wrapper of a mixture re-allocation move.
     *
     * This class is the RevLanguage wrapper of MixtureAllocationMove.
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     * @since 2014-08-18, version 1.0
     */
    template <class rlValueType>
    class Move_MixtureAllocation : public Move {
        
    public:
        
        Move_MixtureAllocation(void);                                                                                                                   //!< Default constructor
        
        // Basic utility functions
        virtual Move_MixtureAllocation*             clone(void) const;                                                                              //!< Clone the object
        void                                        constructInternalObject(void);                                                                  //!< We construct the a new internal move.
        static const std::string&                   getClassType(void);                                                                             //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                         //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                                                     //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                        //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                                              //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const Variable> &var);             //!< Set member variable
        
        RevPtr<const Variable>                      x;                                                                                              //!< The variable holding the real valued vector.
        
    };
    
}


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ModelVector.h"
#include "MixtureAllocationMove.h"
#include "RbException.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;


template <class rlValueType>
RevLanguage::Move_MixtureAllocation<rlValueType>::Move_MixtureAllocation() : Move()
{
    
}


/** Clone object */
template <class rlValueType>
RevLanguage::Move_MixtureAllocation<rlValueType>* Move_MixtureAllocation<rlValueType>::clone(void) const
{
    
	return new Move_MixtureAllocation<rlValueType>(*this);
}


template <class rlValueType>
void RevLanguage::Move_MixtureAllocation<rlValueType>::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new vector-scale move
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<typename rlValueType::valueType>* tmp = static_cast<const rlValueType &>( x->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode< typename rlValueType::valueType > *sn = static_cast<RevBayesCore::StochasticNode< typename rlValueType::valueType > *>( tmp );
    
    value = new RevBayesCore::MixtureAllocationMove<typename rlValueType::valueType>(sn, w);
}


/** Get Rev type of object */
template <class rlValueType>
const std::string& RevLanguage::Move_MixtureAllocation<rlValueType>::getClassType(void)
{
    
//    static std::string revType = "Move_MixtureAllocation<" + rlValueType::getClassType() + ">";
    static std::string revType = "Move_MixtureAllocation__" + rlValueType::getClassType();
    
	return revType;
}

/** Get class type spec describing type of object */
template <class rlValueType>
const RevLanguage::TypeSpec& RevLanguage::Move_MixtureAllocation<rlValueType>::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
template <class rlValueType>
const RevLanguage::MemberRules& RevLanguage::Move_MixtureAllocation<rlValueType>::getParameterRules(void) const
{
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        moveMemberRules.push_back( new ArgumentRule( "x", rlValueType::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return moveMemberRules;
}

/** Get type spec */
template <class rlValueType>
const RevLanguage::TypeSpec& RevLanguage::Move_MixtureAllocation<rlValueType>::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
template <class rlValueType>
void RevLanguage::Move_MixtureAllocation<rlValueType>::printValue(std::ostream &o) const
{
    
    o << "Move_MixtureAllocation(";
    if (x != NULL)
    {
        o << x->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
template <class rlValueType>
void RevLanguage::Move_MixtureAllocation<rlValueType>::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "x" )
    {
        x = var;
    }
    else
    {
        Move::setConstParameter(name, var);
    }
}


#endif
