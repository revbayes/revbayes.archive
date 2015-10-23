//
//  Func_DECRates.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 3/3/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "DispersalExtinctionRateStructureFunction.h"
#include "Func_DECRates.h"
#include "Natural.h"
#include "RateMatrix_JC.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDagNode.h"
#include <vector>

using namespace RevLanguage;

/** default constructor */
Func_DECRates::Func_DECRates( void ) : TypedFunction<ModelVector<RealPos> >( )
{
    
}


/** Clone object */
Func_DECRates* Func_DECRates::clone( void ) const
{
    
    return new Func_DECRates( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RbVector<double> >* Func_DECRates::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RbVector<double> > >* dr = static_cast<const ModelVector<ModelVector<RealPos> > &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* er = static_cast<const ModelVector<RealPos> &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<int>* rs = static_cast<const Natural&>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    
    if (dr->getValue().size() != er->getValue().size())
    {
        RbException("ERROR: fnDispersalExtinction dispersalRates.size() != extinctionRates.size()!");
    }
    
    RevBayesCore::DispersalExtinctionRateStructureFunction* f = new RevBayesCore::DispersalExtinctionRateStructureFunction( dr,er,rs );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_DECRates::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "dispersalRates", ModelVector<ModelVector<RealPos> >::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new ArgumentRule( "extinctionRates", ModelVector<RealPos>::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new ArgumentRule( "maxRangeSize", Natural::getClassTypeSpec(), "", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(RbConstants::Integer::max) ) );

        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_DECRates::getClassType(void)
{
    
    static std::string revType = "Func_DECRates";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_DECRates::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


const TypeSpec& Func_DECRates::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
