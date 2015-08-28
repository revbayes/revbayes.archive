/**
 * @file
 * This file contains the declaration of Func_annotateHPDAges, which annotates the 
 * x% Highest Posterior Density (HPD) interval of node ages onto the input TimeTree.
 *
 * @brief Declaration of Func_annotateHPDAges
 *
 * (c) Copyright 2014- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 */

#ifndef Func_annotateHPDAges_H
#define Func_annotateHPDAges_H

#include "Procedure.h"
#include "RbFileManager.h"

#include <string>


namespace RevLanguage {
    
	template <typename treeType>
    class Func_annotateHPDAges :  public Procedure {
        
    public:
        // Basic utility functions
        Func_annotateHPDAges*           clone(void) const;                                                      //!< Clone the object
        static const std::string&       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
        
        // Regular functions
        RevPtr<RevVariable>             execute(void);                                                          //!< Execute function
        const ArgumentRules&            getArgumentRules(void) const;                                           //!< Get argument rules
        const TypeSpec&                 getReturnType(void) const;                                              //!< Get type of return value
        
    private:
        
    };
    
}



#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "ModelVector.h"
#include "NexusWriter.h"
#include "RbException.h"
#include "RevNullObject.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlTreeTrace.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "TreeSummary.h"
#include "TreeTrace.h"

#include <map>
#include <set>
#include <sstream>


/** Clone object */
template <typename treeType>
RevLanguage::Func_annotateHPDAges<treeType>* RevLanguage::Func_annotateHPDAges<treeType>::clone( void ) const {
    
    return new Func_annotateHPDAges<treeType>( *this );
}


/** Execute function */
template <typename treeType>
RevLanguage::RevPtr<RevLanguage::RevVariable> RevLanguage::Func_annotateHPDAges<treeType>::execute( void )
{
    
	// get the x% hpd
	double x = static_cast<const Probability &>(args[0].getVariable()->getRevObject()).getValue();
	
	// get the input tree
	typename treeType::valueType* tree = static_cast<const treeType&>( this->args[1].getVariable()->getRevObject() ).getValue().clone();
	
	// get the  tree trace
    const TreeTrace<treeType>& tt = static_cast<const TreeTrace<treeType>&>( args[2].getVariable()->getRevObject() );
	
	// get the filename
    const std::string& filename = static_cast<const RlString&>( args[3].getVariable()->getRevObject() ).getValue();
	
	// get burnin
	int burnin = static_cast<const Integer &>(args[4].getVariable()->getRevObject()).getValue();
	
    // make a new tree summary object
	RevBayesCore::TreeSummary<typename treeType::valueType> summary = RevBayesCore::TreeSummary<typename treeType::valueType>( tt.getValue() );
	
	// get the tree with x% HPD node ages
	summary.annotateHPDAges(*tree, x, burnin);
	
	// return the tree
    if ( filename != "" )
    {
        
        RevBayesCore::NexusWriter writer(filename);
        writer.openStream();
        
        std::vector<std::string> taxa;
        tree->getRoot().getTaxaStringVector(taxa);
        RevBayesCore::Clade c( taxa, 0.0 );
        writer.writeNexusBlock(c);
        
        writer.writeNexusBlock(*tree);
        
        writer.closeStream();
        
    }
    
    return new RevVariable( new treeType( tree ) );
}



/** Get argument rules */
template <typename treeType>
const RevLanguage::ArgumentRules& RevLanguage::Func_annotateHPDAges<treeType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
		argumentRules.push_back( new ArgumentRule( "hpd"   ,    Probability::getClassTypeSpec()        , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.95) ) );
		argumentRules.push_back( new ArgumentRule( "inputtree", TimeTree::getClassTypeSpec()           , ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "treetrace", TreeTrace<TimeTree>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec()           , ArgumentRule::BY_VALUE ) );
		argumentRules.push_back( new ArgumentRule( "burnin"   , Integer::getClassTypeSpec()            , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Integer(-1) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
template <typename treeType>
const std::string& RevLanguage::Func_annotateHPDAges<treeType>::getClassType(void) { 
    
    static std::string revType = "Func_annotateHPDAges<" + TimeTree::getClassType() + ">";
    
	return revType; 
}

/** Get class type spec describing type of object */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::Func_annotateHPDAges<treeType>::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

/** Get type spec */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::Func_annotateHPDAges<treeType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::Func_annotateHPDAges<treeType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = TimeTree::getClassTypeSpec();
    return returnTypeSpec;
}


#endif

