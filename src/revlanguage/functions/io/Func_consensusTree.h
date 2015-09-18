/**
 * @file
 * This file contains the declaration of Func_consensusTree, which calculates the majority-rule consensus tree from a tree
 * trace and adds the node posterior probabilities.
 *
 * @brief Declaration of Func_consensusTree
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-27 16:10:36 +0200 (Sun, 27 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_consensusTree.h 1587 2012-05-27 14:10:36Z seraklop $
 */

#ifndef Func_consensusTree_H
#define Func_consensusTree_H

#include "Procedure.h"
#include "RbFileManager.h"

#include <string>


namespace RevLanguage {

    template <typename treeType>
    class Func_consensusTree :  public Procedure {
        
    public:
        // Basic utility functions
		Func_consensusTree*             clone(void) const;                                                      //!< Clone the object
        static const std::string&       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
        
        // Regular functions
        RevPtr<RevVariable>             execute(void);                                                          //!< Execute function
        const ArgumentRules&            getArgumentRules(void) const;                                           //!< Get argument rules
        const TypeSpec&                 getReturnType(void) const;                                              //!< Get type of return value
        
    private:
//        void                            formatError(RevBayesCore::RbFileManager& fm, std::string& errorStr);                  //!< Format the error string when (mis)reading files
        
    };
    
}


#include "ArgumentRule.h"
#include "BranchLengthTree.h"
#include "ConstantNode.h"
#include "ModelVector.h"
#include "NexusWriter.h"
#include "RbException.h"
#include "RevNullObject.h"
#include "RlBranchLengthTree.h"
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
RevLanguage::Func_consensusTree<treeType>* RevLanguage::Func_consensusTree<treeType>::clone(void) const {
    
	return new Func_consensusTree(*this);
}


/** Execute function */
template <typename treeType>
RevLanguage::RevPtr<RevLanguage::RevVariable> RevLanguage::Func_consensusTree<treeType>::execute(void) {
    
    
	const std::string& filename = static_cast<const RlString&>( args[0].getVariable()->getRevObject() ).getValue();
	double cutoff = static_cast<const RealPos &>(args[1].getVariable()->getRevObject()).getValue();
	int burnin = static_cast<const Integer &>( args[2].getVariable()->getRevObject() ).getValue();

	std::vector<const RevBayesCore::TreeTrace<typename treeType::valueType> > traces;
	for (size_t i = 3; i < args.size(); ++i)
	{
		RevBayesCore::TreeTrace<typename treeType::valueType>& t = static_cast<TreeTrace<treeType>&>( args[i].getVariable()->getRevObject()).getValue();
		traces.push_back(t);
	}

	RevBayesCore::TreeSummary<typename treeType::valueType> summary( traces );
    typename treeType::valueType* tree = summary.conTree(cutoff, burnin);
    
    if ( filename != "" ) {        
        
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
const RevLanguage::ArgumentRules& RevLanguage::Func_consensusTree<treeType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
        
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec()           , ArgumentRule::BY_VALUE ) );
		argumentRules.push_back( new ArgumentRule( "cutoff"   , RealPos::getClassTypeSpec()            , ArgumentRule::BY_VALUE ) );
		argumentRules.push_back( new ArgumentRule( "burnin"   , Integer::getClassTypeSpec()            , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Integer(-1) ) );
		argumentRules.push_back( new ArgumentRule( "", TreeTrace<treeType>::getClassTypeSpec() , ArgumentRule::BY_VALUE ) );
		argumentRules.push_back( new Ellipsis( TreeTrace<treeType>::getClassTypeSpec() ) );
		rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
template <typename treeType>
const std::string& RevLanguage::Func_consensusTree<treeType>::getClassType(void) { 
    
    static std::string revType = "Func_consensusTree<" + treeType::getClassType() + ">";
    
	return revType; 
}

/** Get class type spec describing type of object */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::Func_consensusTree<treeType>::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

/** Get type spec */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::Func_consensusTree<treeType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::Func_consensusTree<treeType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = treeType::getClassTypeSpec();
    return returnTypeSpec;
}




#endif

