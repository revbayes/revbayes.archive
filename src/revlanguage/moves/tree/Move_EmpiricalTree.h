#ifndef Move_EmpiricalTree_H
#define Move_EmpiricalTree_H

#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Rev Wrapper of an empirical tree distribution move.
     *
     * This class is the RevLanguage wrapper of EmpiricalTreeMove.
     *
     * @author The RevBayes Development Core Team 
     * @copyright GPL version 3
     */
    template <class rlValueType>
    class Move_EmpiricalTree : public Move {
        
    public:
        
        Move_EmpiricalTree(void);                                                                                                                   //!< Default constructor
        
        // Basic utility functions
        virtual Move_EmpiricalTree*                 clone(void) const;                                                                              //!< Clone the object
        void                                        constructInternalObject(void);                                                                  //!< We construct the a new internal move.
        static const std::string&                   getClassType(void);                                                                             //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                         //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                                                     //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                        //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                                              //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);             //!< Set member variable
        
        RevPtr<const RevVariable>                   tree;                                                                                                      
    };
    
}


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "EmpiricalTreeProposal.h"
#include "ModelVector.h"
#include "MetropolisHastingsMove.h"
#include "RbException.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;


template <class rlValueType>
RevLanguage::Move_EmpiricalTree<rlValueType>::Move_EmpiricalTree() : Move()
{
    
}


/** Clone object */
template <class rlValueType>
RevLanguage::Move_EmpiricalTree<rlValueType>* Move_EmpiricalTree<rlValueType>::clone(void) const
{
    
	return new Move_EmpiricalTree<rlValueType>(*this);
}


template <class rlValueType>
void RevLanguage::Move_EmpiricalTree<rlValueType>::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new empirical tree move
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
	
    RevBayesCore::TypedDagNode<typename rlValueType::valueType>* tmp = static_cast<const rlValueType &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode< typename rlValueType::valueType > *t = static_cast<RevBayesCore::StochasticNode< typename rlValueType::valueType > *>( tmp );
    
    RevBayesCore::Proposal *p = new RevBayesCore::EmpiricalTreeProposal<typename rlValueType::valueType>( t );
    value = new RevBayesCore::MetropolisHastingsMove(p, w, false);

}


/** Get Rev type of object */
template <class rlValueType>
const std::string& RevLanguage::Move_EmpiricalTree<rlValueType>::getClassType(void)
{
    static std::string revType = "Move_EmpiricalTree__" + rlValueType::getClassType();
    
	return revType;
}

/** Get class type spec describing type of object */
template <class rlValueType>
const RevLanguage::TypeSpec& RevLanguage::Move_EmpiricalTree<rlValueType>::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
template <class rlValueType>
const RevLanguage::MemberRules& RevLanguage::Move_EmpiricalTree<rlValueType>::getParameterRules(void) const
{
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        moveMemberRules.push_back( new ArgumentRule( "tree"   , rlValueType::getClassTypeSpec() , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return moveMemberRules;
}

/** Get type spec */
template <class rlValueType>
const RevLanguage::TypeSpec& RevLanguage::Move_EmpiricalTree<rlValueType>::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
template <class rlValueType>
void RevLanguage::Move_EmpiricalTree<rlValueType>::printValue(std::ostream &o) const
{
    
    o << "Move_EmpiricalTree(";
    if (tree != NULL)
    {
        o << tree->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
template <class rlValueType>
void RevLanguage::Move_EmpiricalTree<rlValueType>::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "tree" )
    {
        tree = var;
    }
    else
    {
        Move::setConstParameter(name, var);
    }
}


#endif
