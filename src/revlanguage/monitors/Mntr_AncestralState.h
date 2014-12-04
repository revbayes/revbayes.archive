/**
 * @file
 * This file contains the declaration of the RevLanguage wrapper of the AncestralStateMonitor class.
 *
 * @brief Declaration of Mntr_AncestralState
 *
 * (c) Copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Mntr_AncestralState.h $
 */

#ifndef Mntr_AncestralState_H
#define Mntr_AncestralState_H

#include "AncestralStateMonitor.h"
#include "RlMonitor.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
	template <class treeType>
    class Mntr_AncestralState : public Monitor {
        
    public:
        
        Mntr_AncestralState(void);                                                                                                 //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual Mntr_AncestralState*                clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal Mntr_AncestralState.
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


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "OptionRule.h"
#include "Model.h"
#include "AncestralStateMonitor.h"
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
#include "ChromosomesState.h"
#include "NaturalNumbersState.h"
#include "DnaState.h"

using namespace RevLanguage;

template <class treeType>
Mntr_AncestralState<treeType>::Mntr_AncestralState(void) : Monitor() 
{
    
}


/** Clone object */
template <class treeType>
Mntr_AncestralState<treeType>* Mntr_AncestralState<treeType>::clone(void) const 
{
    
	return new Mntr_AncestralState<treeType>(*this);
}


template <class treeType>
void Mntr_AncestralState<treeType>::constructInternalObject( void ) 
{
    const std::string&                  fn      = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const std::string&                  sep     = static_cast<const RlString &>( separator->getRevObject() ).getValue();
    int                                 g       = static_cast<const Natural  &>( printgen->getRevObject() ).getValue();
	RevBayesCore::TypedDagNode<typename treeType::valueType>* t = static_cast<const treeType &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::DagNode*				ch		= ctmc->getRevObject().getDagNode();
	bool                                ap      = static_cast<const RlBoolean &>( append->getRevObject() ).getValue();
	std::string							character = static_cast<const RlString &>( monitorType->getRevObject() ).getValue();
    
	delete value;
	if (character == "Chromosomes") {
		
		RevBayesCore::AncestralStateMonitor<RevBayesCore::ChromosomesState, typename treeType::valueType> *m = new RevBayesCore::AncestralStateMonitor<RevBayesCore::ChromosomesState, typename treeType::valueType>(t, ch, (unsigned long)g, fn, sep);
		m->setAppend( ap );
		value = m;		
		
	} else if (character == "NaturalNumbers") {
		
		RevBayesCore::AncestralStateMonitor<RevBayesCore::NaturalNumbersState, typename treeType::valueType> *m = new RevBayesCore::AncestralStateMonitor<RevBayesCore::NaturalNumbersState, typename treeType::valueType>(t, ch, (unsigned long)g, fn, sep);
		m->setAppend( ap );
		value = m;		
		
	} else if (character == "DNA") {
		
		RevBayesCore::AncestralStateMonitor<RevBayesCore::DnaState, typename treeType::valueType> *m = new RevBayesCore::AncestralStateMonitor<RevBayesCore::DnaState, typename treeType::valueType>(t, ch, (unsigned long)g, fn, sep);
		m->setAppend( ap );
		value = m;		
		
	} else {
		throw RbException( "Incorrect character type specified. Valid options are: NaturalNumbers, DNA, Chromosomes" );
	}
	
    
	
	
}


/** Get Rev type of object */
template <class treeType>
const std::string& Mntr_AncestralState<treeType>::getClassType(void) 
{ 
    
    static std::string revType = "Mntr_AncestralState";
    
	return revType; 
}

/** Get class type spec describing type of object */
template <class treeType>
const TypeSpec& Mntr_AncestralState<treeType>::getClassTypeSpec(void) 
{ 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
template <class treeType>
const MemberRules& Mntr_AncestralState<treeType>::getParameterRules(void) const {
    
    static MemberRules modelMonitorMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
		modelMonitorMemberRules.push_back( new ArgumentRule("tree"           , treeType::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
		modelMonitorMemberRules.push_back( new ArgumentRule("ctmc"           , RevObject::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("filename"      , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE ) );
		modelMonitorMemberRules.push_back( new ArgumentRule("type"      , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("printgen"      , Natural::getClassTypeSpec()  , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("separator"     , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
        modelMonitorMemberRules.push_back( new ArgumentRule("append"        , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        
        rulesSet = true;
    }
    
    return modelMonitorMemberRules;
}

/** Get type spec */
template <class treeType>
const TypeSpec& Mntr_AncestralState<treeType>::getTypeSpec( void ) const 
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
template <class treeType>
void Mntr_AncestralState<treeType>::printValue(std::ostream &o) const 
{
    
    o << "Mntr_AncestralState";
}


/** Set a member variable */
template <class treeType>
void Mntr_AncestralState<treeType>::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) 
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

#endif