/**
 * @file
 * This file contains the declaration of Func_mapTree, finds the maximum-a-posteriori (MAP) tree from a tree
 * trace and adds the node posterior probabilities.
 *
 * @brief Declaration of Func_mapTree
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-27 16:10:36 +0200 (Sun, 27 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_readDiscreteCharacterData.h 1587 2012-05-27 14:10:36Z hoehna $
 */

#ifndef Func_mapTree_H
#define Func_mapTree_H

#include "Procedure.h"
#include "RbFileManager.h"

#include <string>


namespace RevLanguage {
    
    template <typename treeType>
    class Func_mapTree : public Procedure {
        
    public:
        // Basic utility functions
        Func_mapTree*                   clone(void) const;                                                      //!< Clone the object
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
#include "RlUtils.h"
#include "StringUtilities.h"
#include "TreeSummary.h"
#include "TreeTrace.h"
#include "UserInterface.h"

#include <map>
#include <set>
#include <sstream>


/** Clone object */
template <typename treeType>
RevLanguage::Func_mapTree<treeType>* RevLanguage::Func_mapTree<treeType>::clone( void ) const {
    
    return new Func_mapTree( *this );
}


/** Execute function */
template <typename treeType>
RevLanguage::RevPtr<RevLanguage::Variable> RevLanguage::Func_mapTree<treeType>::execute( void ) {
    
    
    const TreeTrace<treeType>& tt = static_cast<const TreeTrace<treeType>&>( args[0].getVariable()->getRevObject() );
    const std::string& filename = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    RevBayesCore::TreeSummary<typename treeType::valueType> summary = RevBayesCore::TreeSummary<typename treeType::valueType>( tt.getValue() );
    typename treeType::valueType* tree = summary.map();
    
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
const RevLanguage::ArgumentRules& RevLanguage::Func_mapTree<treeType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
        
        argumentRules.push_back( new ArgumentRule( "treetrace", TreeTrace<treeType>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec()           , ArgumentRule::BY_VALUE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
template <typename treeType>
const std::string& RevLanguage::Func_mapTree<treeType>::getClassType(void) { 
    
    static std::string revType = "Func_mapTree<" + treeType::getClassType() + ">";
    
	return revType; 
}

/** Get class type spec describing type of object */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::Func_mapTree<treeType>::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

/** Get type spec */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::Func_mapTree<treeType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::Func_mapTree<treeType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = treeType::getClassTypeSpec();
    return returnTypeSpec;
}




#endif

