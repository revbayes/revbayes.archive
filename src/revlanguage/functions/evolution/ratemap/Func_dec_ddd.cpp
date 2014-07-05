//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "DistanceDependentDispersalFunction.h"
#include "Func_dec_ddd.h"
#include "GeographyRateModifier.h"
#include "Real.h"
#include "RealPos.h"
#include "RlGeographyRateModifier.h"
#include "RlRateMap.h"
#include "RlSimplex.h"
#include "TimeAtlas.h"
#include "TypedDagNode.h"
#include "Vector.h"

using namespace RevLanguage;

/** default constructor */
Func_dec_ddd::Func_dec_ddd( void ) : Function( ) {
    
}


/** Clone object */
Func_dec_ddd* Func_dec_ddd::clone( void ) const {
    
    return new Func_dec_ddd( *this );
}


RevObject* Func_dec_ddd::execute() {
    
    //        GeographyRateModifier( TimeAtlas* ta,  bool uadj=false, bool uav=false, bool udd=false, int index=0, double dp=10e-6, double threshhold=1e-6, std::string dt="haversine" );

    RevBayesCore::TypedDagNode<double>* dp = static_cast<const RealPos&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
//    unsigned nc = static_cast<const Natural&>( this->args[1].getVariable()->getRevObject() ).getValue();
//    bool fe = static_cast<const RlBoolean &>( this->args[2].getVariable()->getRevObject() ).getValue();
    //    RevBayesCore::TypedDagNode<std::vector<double> >* r = static_cast<const Vector<RealPos> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    

//    f->setGainLossRates(glr);
//    
//    dp = new ContinuousStochasticNode("distancePower", new NormalDistribution(new ConstantNode<double>("dp_mu",new double(0.0)), dp_pr));
//    dp_pr = new ConstantNode<double>( "distancePowerPrior", new double(10.0));
//    dp = new ContinuousStochasticNode("distancePower", new ExponentialDistribution(dp_pr));
//    dp->setValue(new double(0.00001));
//    //        dp->setValue(new double(1.0));
//    ddd = new DeterministicNode<GeographyRateModifier>("dddFunction", new DistanceDependentDispersalFunction(dp, ta, useAdjacency, useAvailable, useDistances));
    
    RevBayesCore::TimeAtlas* atlas = NULL;
    bool uadj = true;
    bool uav = true;
    bool udd = true;
    
    RevBayesCore::DistanceDependentDispersalFunction* f = new RevBayesCore::DistanceDependentDispersalFunction(dp, atlas, uadj, uav, udd);
    
    RevBayesCore::DeterministicNode<RevBayesCore::GeographyRateModifier> *detNode = new RevBayesCore::DeterministicNode<RevBayesCore::GeographyRateModifier>("", f);
    
    RlGeographyRateModifier* value = new RlGeographyRateModifier( detNode );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& Func_dec_ddd::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "distancePower", true, RealPos::getClassTypeSpec(), new RealPos(0.0) ) );
        argumentRules.push_back( new ArgumentRule( "useAdjacency", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        argumentRules.push_back( new ArgumentRule( "useDistances", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_dec_ddd::getClassName(void) {
    
    static std::string rbClassName = "Func_dec_ddd";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_dec_ddd::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass;
}


/* Get return type */
const TypeSpec& Func_dec_ddd::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMap::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_dec_ddd::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
