
/**
 * @file
 * This file contains the declaration of Func_ancestralStateTree, which finds the maximum-a-posteriori (MAP) ancestral states
 * on the nodes of the input tree.
 *
 * @brief Declaration of Func_ancestralStateTree
 *
 */

#ifndef Func_ancestralStateTree_H
#define Func_ancestralStateTree_H

#include "RlFunction.h"
#include "RbFileManager.h"

#include <string>


namespace RevLanguage {
    
    template <typename treeType>
    class Func_ancestralStateTree :  public Function {
        
    public:
        // Basic utility functions
        Func_ancestralStateTree*         clone(void) const;                                                      //!< Clone the object
        static const std::string&       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
        
        // Regular functions
        RevPtr<Variable>                execute(void);                                                          //!< Execute function
        const ArgumentRules&            getArgumentRules(void) const;                                           //!< Get argument rules
        const TypeSpec&                 getReturnType(void) const;                                              //!< Get type of return value
        
    private:
        
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
#include "RlAncestralStateTrace.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "TreeSummary.h"
#include "TreeTrace.h"
#include "AncestralStateTrace.h"
#include "UserInterface.h"
#include "WorkspaceVector.h"

#include <map>
#include <set>
#include <sstream>


/** Clone object */
template <typename treeType>
RevLanguage::Func_ancestralStateTree<treeType>* RevLanguage::Func_ancestralStateTree<treeType>::clone( void ) const {
    
    return new Func_ancestralStateTree( *this );
}


/** Execute function */
template <typename treeType>
RevLanguage::RevPtr<RevLanguage::Variable> RevLanguage::Func_ancestralStateTree<treeType>::execute( void ) {
    
	// get the input tree
	const treeType& it = static_cast<const treeType&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
	
    // get vector of ancestral state traces
	const WorkspaceVector<AncestralStateTrace>& ast_vector = static_cast<const WorkspaceVector<AncestralStateTrace> &>( args[1].getVariable()->getRevObject() );
	std::vector<RevBayesCore::AncestralStateTrace> ancestralstate_traces;
	for (int i = 0; i < ast_vector.size(); ++i) {
		ancestralstate_traces.push_back( ast_vector[i].getValue() );
	}
	
	// get the ancestral state tree trace
    const TreeTrace<treeType>& tt = static_cast<const TreeTrace<treeType>&>( args[2].getVariable()->getRevObject() );
	
	// get the filename
    const std::string& filename = static_cast<const RlString&>( args[3].getVariable()->getRevObject() ).getValue();

    // make a new tree summary object
	RevBayesCore::TreeSummary<typename treeType::valueType> summary = RevBayesCore::TreeSummary<typename treeType::valueType>( tt.getValue() );
	
	// get the tree with ancestral states 
	typename treeType::valueType* tree = summary.ancestralStateTree(it.getValue(), ancestralstate_traces, -1);
	
	// return the tree
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
    
    return new Variable( new treeType( tree ) );
}



/** Get argument rules */
template <typename treeType>
const RevLanguage::ArgumentRules& RevLanguage::Func_ancestralStateTree<treeType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
		argumentRules.push_back( new ArgumentRule( "inputtree", treeType::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
		argumentRules.push_back( new ArgumentRule( "ancestralstatetrace_vector", WorkspaceVector<AncestralStateTrace>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "treetrace", TreeTrace<treeType>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec()           , ArgumentRule::BY_VALUE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
template <typename treeType>
const std::string& RevLanguage::Func_ancestralStateTree<treeType>::getClassType(void) { 
    
    static std::string revType = "Func_ancestralStateTree<" + treeType::getClassType() + ">";
    
	return revType; 
}

/** Get class type spec describing type of object */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::Func_ancestralStateTree<treeType>::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

/** Get type spec */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::Func_ancestralStateTree<treeType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::Func_ancestralStateTree<treeType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = treeType::getClassTypeSpec();
    return returnTypeSpec;
}


#endif

