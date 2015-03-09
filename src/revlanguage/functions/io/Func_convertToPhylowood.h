//
//  Func_convertToPhylowood.h
//  revbayes-proj
//
//  Created by Michael Landis on 3/2/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__Func_convertToPhylowood__
#define __revbayes_proj__Func_convertToPhylowood__

#include "Procedure.h"
#include "RbFileManager.h"

#include <string>


namespace RevLanguage {
    
    template <typename treeType>
    class Func_convertToPhylowood :  public Procedure {
        
    public:
        // Basic utility functions
        Func_convertToPhylowood*        clone(void) const;                                                      //!< Clone the object
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
#include "BranchLengthTree.h"
#include "ConstantNode.h"
#include "ModelVector.h"
#include "NexusWriter.h"
#include "PhylowoodConverter.h"
#include "Probability.h"
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
#include "WorkspaceVector.h"

#include <map>
#include <set>
#include <sstream>


/** Clone object */
template <typename treeType>
RevLanguage::Func_convertToPhylowood<treeType>* RevLanguage::Func_convertToPhylowood<treeType>::clone( void ) const {
    
    return new Func_convertToPhylowood( *this );
}


/** Execute function */
template <typename treeType>
RevLanguage::RevPtr<RevLanguage::RevVariable> RevLanguage::Func_convertToPhylowood<treeType>::execute( void ) {

    
    const std::string& stateFilename = static_cast<const RlString&>( args[0].getVariable()->getRevObject() ).getValue();
    const std::string& treeFilename  = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    const std::string& geoFilename   = static_cast<const RlString&>( args[2].getVariable()->getRevObject() ).getValue();
    const std::string& phwFilename   = static_cast<const RlString&>( args[3].getVariable()->getRevObject() ).getValue();
    double burn = static_cast<const Probability&>( args[4].getVariable()->getRevObject() ).getValue();
    const std::string& chartype      = static_cast<const RlString&>( args[5].getVariable()->getRevObject() ).getValue();
    const std::string& bgtype      = static_cast<const RlString&>( args[6].getVariable()->getRevObject() ).getValue();
    
    RevBayesCore::PhylowoodConverter phw(stateFilename, treeFilename, geoFilename, phwFilename, burn, chartype, bgtype);
//    RevBayesCore::PhylowoodConverter phw;
	
    return NULL;
}



/** Get argument rules */
template <typename treeType>
const RevLanguage::ArgumentRules& RevLanguage::Func_convertToPhylowood<treeType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
		argumentRules.push_back( new ArgumentRule( "statefile", RlString::getClassTypeSpec()           , ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "treefile",  RlString::getClassTypeSpec()           , ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "geofile",   RlString::getClassTypeSpec()           , ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "outfile",   RlString::getClassTypeSpec()           , ArgumentRule::BY_VALUE ) );
		argumentRules.push_back( new ArgumentRule( "burnin",    Probability::getClassTypeSpec()            , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Integer(-1) ) );
        
        std::vector<std::string> options;
        options.push_back( "NaturalNumbers" );
        options.push_back( "Standard" );
        argumentRules.push_back( new OptionRule( "chartype", new RlString("NaturalNumbers"), options ) );
        
        std::vector<std::string> options2;
        options2.push_back( "Range" );
        options2.push_back( "Area" );
        argumentRules.push_back( new OptionRule( "bgtype", new RlString("Area"), options2 ) );

        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
template <typename treeType>
const std::string& RevLanguage::Func_convertToPhylowood<treeType>::getClassType(void) {
    
    static std::string revType = "Func_convertToPhylowood<" + treeType::getClassType() + ">";
    
	return revType;
}

/** Get class type spec describing type of object */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::Func_convertToPhylowood<treeType>::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}

/** Get type spec */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::Func_convertToPhylowood<treeType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::Func_convertToPhylowood<treeType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = treeType::getClassTypeSpec();
    return returnTypeSpec;
}

#endif /* defined(__revbayes_proj__Func_convertToPhylowood__) */
