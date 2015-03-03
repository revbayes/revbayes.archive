//
//  Func_structureDispersalExtinction.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 3/3/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "DispersalExtinctionRateStructureFunction.h"
#include "Func_structureDispersalExtinction.h"
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
Func_structureDispersalExtinction::Func_structureDispersalExtinction( void ) : TypedFunction<ModelVector<RealPos> >( )
{
    
}


/** Clone object */
Func_structureDispersalExtinction* Func_structureDispersalExtinction::clone( void ) const
{
    
    return new Func_structureDispersalExtinction( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RbVector<double> >* Func_structureDispersalExtinction::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RbVector<double> > >* dr = static_cast<const ModelVector<ModelVector<RealPos> > &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* er = static_cast<const ModelVector<RealPos> &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    
    if (dr->getValue().size() != er->getValue().size())
    {
        RbException("ERROR: fnDispersalExtinction dispersalRates.size() != extinctionRates.size()!");
    }
    
    RevBayesCore::DispersalExtinctionRateStructureFunction* f = new RevBayesCore::DispersalExtinctionRateStructureFunction( dr,er );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_structureDispersalExtinction::getArgumentRules( void ) const {
    
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "dispersalRates", ModelVector<ModelVector<RealPos> >::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "extinctionRates", ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );

        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_structureDispersalExtinction::getClassType(void) {
    
    static std::string revType = "Func_structureDispersalExtinction";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_structureDispersalExtinction::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


const TypeSpec& Func_structureDispersalExtinction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
