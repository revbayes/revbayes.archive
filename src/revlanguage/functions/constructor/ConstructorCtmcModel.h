///**
// * @file
// * This file contains the declaration of ConstructorCtmcModel, which is used to construct a distribution object
// * of the CTMCModel in various different flavours.
// *
// * @brief Declaration of ConstructorCtmcModel
// *
// * (c) Copyright 2009- under GPL version 3
// * @date Last modified: $Date: 2012-06-01 14:40:37 +0200 (Fri, 01 Jun 2012) $
// * @author The RevBayes Development Core Team
// * @license GPL version 3
// * @version 1.0
// *
// * $Id: ConstructorFunction.h 1602 2012-06-01 12:40:37Z hoehna $
// */
//
//#ifndef ConstructorCtmcModel_H
//#define ConstructorCtmcModel_H
//
//#include "RlFunction.h"
//
//#include <string>
//
//namespace RevLanguage {
//    
//    
//    template <class treeType>
//    class ConstructorCtmcModel :  public Function {
//        
//    public:
//        ConstructorCtmcModel(void);                                                                                             //!< Object constructor
//        
//        
//        // Basic utility functions
//        ConstructorCtmcModel*                   clone(void) const;                                                              //!< Clone the object
//        static const std::string&               getClassName(void);                                                             //!< Get class name
//        static const TypeSpec&                  getClassTypeSpec(void);                                                         //!< Get class type spec
//        const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get language type of the object
//        
//        // Regular functions
//        RbLanguageObject*                       execute(void);                                                                  //!< Execute the function. This is the function one has to overwrite for single return values.
//        const ArgumentRules&                    getArgumentRules(void) const;                                                   //!< Get argument rules
//        const TypeSpec&                         getReturnType(void) const;                                                      //!< Get type of return value
//                        
//    };
//    
//}
//
//
//#include "ArgumentRule.h"
//#include "OptionRule.h"
//#include "RlAminoAcidState.h"
//#include "RlDnaState.h"
//#include "RlRnaState.h"
//#include "TypeSpec.h"
//
///** Constructor */
//template <class treeType>
//RevLanguage::ConstructorCtmcModel<treeType>::ConstructorCtmcModel( void ) : Function() {
//    
//}
//
//
///** Clone the object */
//template <class treeType>
//RevLanguage::ConstructorCtmcModel<treeType>* RevLanguage::ConstructorCtmcModel<treeType>::clone(void) const {
//    
//    return new ConstructorCtmcModel<treeType>(*this);
//}
//
//
///** Execute function: we reset our template object here and give out a copy */
//template <class treeType>
//RevLanguage::RbLanguageObject* RevLanguage::ConstructorCtmcModel<treeType>::execute( void ) {
//    
//    const RateMatrix& q = static_cast<const RateMatrix &>( args[1].getVariable()->getValue() );
//    const RealPos& rate = static_cast<const RealPos &>( args[2].getVariable()->getValue() );
//    int nSites = static_cast<const Natural &>( args[3].getVariable()->getValue() ).getValue();
//    const std::string& type = static_cast<const RlString &>( args[4].getVariable()->getValue() ).getValue();
//    
//    Distribution *d = NULL;
//    
//    if ( type == "DNA" ) 
//    {
//        d = new CharacterStateEvolutionAlongTree<DnaState, treeType>();
//    } 
//    else if ( type == "RNA" )
//    {
//        d = new CharacterStateEvolutionAlongTree<RnaState, treeType>();
//    }
//    else if ( type == "AA" || type == "Protein" )
//    {
//        d = new CharacterStateEvolutionAlongTree<AminoAcidState, treeType>();
//    }
//    
//    return d;
//}
//
//
///** Get argument rules */
//template <class treeType>
//const RevLanguage::ArgumentRules& RevLanguage::ConstructorCtmcModel<treeType>::getArgumentRules(void) const {
//    
//    static ArgumentRules argumentRules = ArgumentRules();
//    static bool          rulesSet = false;
//    
//    if ( !rulesSet ) 
//    {
//        
//        argumentRules.push_back( new ArgumentRule( "tree",   true, treeType::getClassTypeSpec() ) );
//        argumentRules.push_back( new ArgumentRule( "Q",      true, RateMatrix::getClassTypeSpec() ) );
//        argumentRules.push_back( new ArgumentRule( "rate",   true, RealPos::getClassTypeSpec(), new RealPos(1.0) ) );
//        argumentRules.push_back( new ArgumentRule( "nSites", true, Natural::getClassTypeSpec() ) );
//        
//        Vector<RlString> options;
//        options.push_back( RlString("DNA") );
//        options.push_back( RlString("RNA") );
//        options.push_back( RlString("AA") );
//        options.push_back( RlString("Protein") );
//        argumentRules.push_back( new OptionRule( "type", new RlString("DNA"), options ) );
//        
//        rulesSet = true;
//    }
//    
//    return argumentRules;
//}
//
//
///** Get class name of object */
//template <class treeType>
//const std::string& RevLanguage::ConstructorCtmcModel<treeType>::getClassName(void) { 
//    
//    static std::string rbClassName = "CtmcModel Constructor function";
//    
//	return rbClassName; 
//}
//
///** Get class type spec describing type of object */
//template <class treeType>
//const RevLanguage::TypeSpec& RevLanguage::ConstructorCtmcModel<treeType>::getClassTypeSpec(void) { 
//    
//    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
//    
//	return rbClass; 
//}
//
///** Get type spec */
//template <class treeType>
//const RevLanguage::TypeSpec& RevLanguage::ConstructorCtmcModel<treeType>::getTypeSpec( void ) const {
//    
//    static TypeSpec typeSpec = getClassTypeSpec();
//    
//    return typeSpec;
//}
//
//
///** Get return type */
//template <class treeType>
//const RevLanguage::TypeSpec& RevLanguage::ConstructorCtmcModel<treeType>::getReturnType(void) const {
//    
//    return CharacterStateEvolutionAlongTree<DnaState,treeType>::getClassTypeSpec();
//}
//
//
//
//#endif
