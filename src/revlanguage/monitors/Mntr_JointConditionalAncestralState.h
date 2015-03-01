//
//  Mntr_JointConditionalJointConditionalAncestralState.h
//  revbayes-proj
//
//  Created by Michael Landis on 2/28/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__Mntr_JointConditionalJointConditionalAncestralState__
#define __revbayes_proj__Mntr_JointConditionalJointConditionalAncestralState__


#include "JointConditionalAncestralStateMonitor.h"
#include "RlMonitor.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
	template <class treeType>
    class Mntr_JointConditionalAncestralState : public Monitor {
        
    public:
        
        Mntr_JointConditionalAncestralState(void);                                                                          //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual Mntr_JointConditionalAncestralState*                clone(void) const;                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal Mntr_JointConditionalAncestralState.
        static const std::string&                   getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                          //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
		std::vector<RevPtr<const RevVariable> >     vars;
        RevPtr<const RevVariable>                   filename;
        RevPtr<const RevVariable>                   printgen;
		RevPtr<const RevVariable>                   tree;
		RevPtr<const RevVariable>                   ctmc;
        RevPtr<const RevVariable>                   separator;
        RevPtr<const RevVariable>                   append;
		RevPtr<const RevVariable>                   monitorType;
    };
    
}

#include "AbstractCharacterData.h"
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "OptionRule.h"
#include "Model.h"
#include "RlMonitor.h"
#include "ModelVector.h"
#include "Ellipsis.h"
#include "Natural.h"
#include "RbException.h"
#include "RevObject.h"
#include "RlModel.h"
#include "RlTimeTree.h"
#include "RlBranchLengthTree.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "NaturalNumbersState.h"
#include "DnaState.h"

using namespace RevLanguage;

template <class treeType>
Mntr_JointConditionalAncestralState<treeType>::Mntr_JointConditionalAncestralState(void) : Monitor()
{
    
}


/** Clone object */
template <class treeType>
Mntr_JointConditionalAncestralState<treeType>* Mntr_JointConditionalAncestralState<treeType>::clone(void) const
{
    
	return new Mntr_JointConditionalAncestralState<treeType>(*this);
}


template <class treeType>
void Mntr_JointConditionalAncestralState<treeType>::constructInternalObject( void )
{
    const std::string&                  fn      = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const std::string&                  sep     = static_cast<const RlString &>( separator->getRevObject() ).getValue();
    int                                 g       = static_cast<const Natural  &>( printgen->getRevObject() ).getValue();
	RevBayesCore::TypedDagNode<typename treeType::valueType>* t = static_cast<const treeType &>( tree->getRevObject() ).getDagNode();
    
    
    RevBayesCore::TypedDagNode<RevBayesCore::AbstractDiscreteCharacterData>* ctmc_tdn = static_cast<const RevLanguage::AbstractDiscreteCharacterData&>( ctmc->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::AbstractDiscreteCharacterData>* ctmc_sn  = static_cast<RevBayesCore::StochasticNode<RevBayesCore::AbstractDiscreteCharacterData>* >(ctmc_tdn);

	bool                                ap      = static_cast<const RlBoolean &>( append->getRevObject() ).getValue();
	std::string							character = static_cast<const RlString &>( monitorType->getRevObject() ).getValue();
    
	delete value;
	if (character == "NaturalNumbers") {
        RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::NaturalNumbersState, typename treeType::valueType> *m;
        m = new RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::NaturalNumbersState, typename treeType::valueType>(t, ctmc_sn, (unsigned long)g, fn, sep);
		m->setAppend( ap );
		value = m;
		
	} else if (character == "DNA") {
		RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::DnaState, typename treeType::valueType> *m;
        m = new RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::DnaState, typename treeType::valueType>(t, ctmc_sn, (unsigned long)g, fn, sep);
		m->setAppend( ap );
		value = m;
		
	} else {
		throw RbException( "Incorrect character type specified. Valid options are: NaturalNumbers, DNA" );
	}
}


/** Get Rev type of object */
template <class treeType>
const std::string& Mntr_JointConditionalAncestralState<treeType>::getClassType(void)
{
    
    static std::string revType = "Mntr_JointConditionalAncestralState";
    
	return revType;
}

/** Get class type spec describing type of object */
template <class treeType>
const TypeSpec& Mntr_JointConditionalAncestralState<treeType>::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
template <class treeType>
const MemberRules& Mntr_JointConditionalAncestralState<treeType>::getParameterRules(void) const {
    
    static MemberRules asMonitorMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
		asMonitorMemberRules.push_back( new ArgumentRule("tree"          , treeType::getClassTypeSpec() , ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
		asMonitorMemberRules.push_back( new ArgumentRule("ctmc"          , AbstractDiscreteCharacterData::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        asMonitorMemberRules.push_back( new ArgumentRule("filename"      , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE ) );
		asMonitorMemberRules.push_back( new ArgumentRule("type"          , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE ) );
        asMonitorMemberRules.push_back( new ArgumentRule("printgen"      , Natural::getClassTypeSpec()  , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
        asMonitorMemberRules.push_back( new ArgumentRule("separator"     , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
        asMonitorMemberRules.push_back( new ArgumentRule("append"        , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        
        rulesSet = true;
    }
    
    return asMonitorMemberRules;
}

/** Get type spec */
template <class treeType>
const TypeSpec& Mntr_JointConditionalAncestralState<treeType>::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
template <class treeType>
void Mntr_JointConditionalAncestralState<treeType>::printValue(std::ostream &o) const
{
    
    o << "Mntr_JointConditionalAncestralState";
}


/** Set a member variable */
template <class treeType>
void Mntr_JointConditionalAncestralState<treeType>::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "" ) {
        vars.push_back( var );
    }
	else if ( name == "filename" )
    {
        filename = var;
    }
    else if ( name == "separator" )
    {
        separator = var;
    }
	else if ( name == "tree" ) {
		tree = var;
    }
	else if ( name == "type" )
    {
        monitorType = var;
    }
	else if ( name == "ctmc" )
    {
        ctmc = var;
    }
    else if ( name == "printgen" )
    {
        printgen = var;
    }
    else if ( name == "append" )
    {
        append = var;
    }
    else
    {
        Monitor::setConstParameter(name, var);
    }
    
}


#endif /* defined(__revbayes_proj__Mntr_JointConditionalJointConditionalAncestralState__) */
