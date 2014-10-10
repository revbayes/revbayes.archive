//
//  RlAtlas.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 4/5/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRule.h"
#include "TimeAtlas.h"
#include "GeographicArea.h"
#include "ModelVector.h"
#include "MemberProcedure.h"
#include "Natural.h"
#include "RlAtlas.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlTaxonData.h"
#include "TimeAtlas.h"
#include "Variable.h"
#include <vector>

using namespace RevLanguage;

RlAtlas::RlAtlas(void) : ModelObject<RevBayesCore::TimeAtlas>( ) {

}


RlAtlas::RlAtlas( RevBayesCore::TimeAtlas *v) : ModelObject<RevBayesCore::TimeAtlas>( v ) {

}


RlAtlas::RlAtlas( RevBayesCore::TypedDagNode<RevBayesCore::TimeAtlas> *m) : ModelObject<RevBayesCore::TimeAtlas>( m ) {

}


RlAtlas* RlAtlas::clone() const {
    return new RlAtlas( *this );
}


/* Map calls to member methods */
RevPtr<Variable> RlAtlas::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    
    if (name == "nAreas")
    {
        return new Variable(new Natural((int)this->dagNode->getValue().getNumAreas())) ;
    }
    else if (name == "nEpochs")
    {
        return new Variable(new Natural((int)this->dagNode->getValue().getNumEpochs())) ;
    }
    else if (name == "names")
    {
        ModelVector<RlString> *n = new ModelVector<RlString>();
        const std::vector<std::vector<RevBayesCore::GeographicArea*> >& areas = this->dagNode->getValue().getAreas();
        for (size_t i = 0; i < areas[0].size(); ++i)
        {
            std::string name = areas[0][i]->getName();
            n->push_back( name );
        }
        return new Variable( n );
    }
    return ModelObject<RevBayesCore::TimeAtlas>::executeMethod( name, args );
}


/* Get Rev type of object */
const std::string& RlAtlas::getClassType(void) {
    
    static std::string revType = "RlAtlas";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& RlAtlas::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RlAtlas::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}

void RlAtlas::initializeMethods( void ) const
{
    
    // Insert inherited methods
    ModelObject<RevBayesCore::TimeAtlas>::initializeMethods();
    
    ArgumentRules* nAreasArgRules               = new ArgumentRules();
    ArgumentRules* nEpochsArgRules              = new ArgumentRules();
    ArgumentRules* namesArgRules                = new ArgumentRules();
    
    methods.addFunction("names",               new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules           ) );
    methods.addFunction("nAreas",              new MemberProcedure(Natural::getClassTypeSpec(),               nAreasArgRules          ) );
    methods.addFunction("nEpochs",             new MemberProcedure(Natural::getClassTypeSpec(),               nEpochsArgRules         ) );
    
    // Add method for call "size" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction("size",  new MemberProcedure( Natural::getClassTypeSpec(), sizeArgRules) );
    
}

