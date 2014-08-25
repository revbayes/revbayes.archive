
//
//  Func_polymorphicStateConverter.cpp
//  RevBayesCore
//
//  Created by Bastien Boussau on 22/8/14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_polymorphicStateConverter.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RlAbstractDiscreteCharacterData.h"
#include "RlDnaState.h"
#include "RlDeterministicNode.h"
#include "RlTaxon.h"
#include "PolymorphicStateConverter.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_polymorphicStateConverter::Func_polymorphicStateConverter( void ) : Function( ) {
    
}


/** Clone object */
Func_polymorphicStateConverter* Func_polymorphicStateConverter::clone( void ) const {
    
    return new Func_polymorphicStateConverter( *this );
}


RevPtr<Variable> Func_polymorphicStateConverter::execute() {
    
    const RevBayesCore::TypedDagNode<RevBayesCore::AbstractCharacterData>* aln = static_cast<const AbstractCharacterData&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode< int >* n = static_cast<const Natural &>( this->args[1].getVariable()->getRevObject() ).getDagNode();


    RevBayesCore::PolymorphicStateConverter* c = new RevBayesCore::PolymorphicStateConverter(  );
    
    RevBayesCore::TypedDagNode< std::vector<RevBayesCore::Taxon> >* taxa  = static_cast< const ModelVector<Taxon> &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    
    std::map <std::string, std::string > gene2species;

    for (std::vector<RevBayesCore::Taxon>::const_iterator it=taxa->getValue().begin(); it!=taxa->getValue().end(); ++it)
    {
        gene2species[it->getName()] = it->getSpeciesName();
    }


    
    AbstractDiscreteCharacterData PomoAln = c->convertData( aln->getValue(), n->getValue(), gene2species ) ;
        
    return new Variable( new AbstractDiscreteCharacterData( PomoAln ) );
}


/* Get argument rules */
const ArgumentRules& Func_polymorphicStateConverter::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "aln", true, AbstractDiscreteCharacterData::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "virtualNe"    , true, Natural::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "taxa"  , true, ModelVector<Taxon>::getClassTypeSpec() ) );

        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_polymorphicStateConverter::getClassType(void) {
    
    static std::string revType = "Func_polymorphicStateConverter";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_polymorphicStateConverter::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_polymorphicStateConverter::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = AbstractDiscreteCharacterData::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_polymorphicStateConverter::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
