//
//  Func_DECRateMatrix.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 3/16/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//


#include "ConstantNode.h"
#include "DECRateMatrixFunction.h"
#include "Func_DECRateMatrix.h"
#include "Integer.h"
#include "Natural.h"
#include "RbConstants.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_DECRateMatrix::Func_DECRateMatrix( void ) : TypedFunction<RateMatrix>( ) {
    
}


/** Clone object */
Func_DECRateMatrix* Func_DECRateMatrix::clone( void ) const {
    
    return new Func_DECRateMatrix( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateMatrix >* Func_DECRateMatrix::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<RevBayesCore::RbVector<double> > >* dr;
    dr = static_cast<const ModelVector<ModelVector<RealPos> > &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* er;
    er = static_cast<const ModelVector<RealPos> &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    
    size_t numStatesEr = er->getValue().size();
    std::vector<size_t> numStatesDr;
    for (size_t i = 0; i < dr->getValue().size(); i++)
    {
        numStatesDr.push_back( dr->getValue()[i].size() );
        if (numStatesDr[i] != numStatesEr)
        {
            throw RbException("The dimension between dispersal and extirpation rates does not match.");
        }
        for (size_t j = 0; j < i; j++)
        {
            if (numStatesDr[i] != numStatesDr[j])
            {
                throw RbException("The dispersal matrix is not square.");
            }
        }
    }
    if (dr->getValue().size() != numStatesEr)
    {
        throw RbException("The dimension between dispersal and extirpation rates does not match.");
    }

    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* rs = NULL;
    if ( this->args[2].getVariable() != NULL && this->args[2].getVariable()->getRevObject() != RevNullObject::getInstance()) {
        rs = static_cast<const Simplex&>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    }
    else {
        size_t n = numStatesEr+1;
        double p = 1.0 / n;
        rs = new RevBayesCore::ConstantNode<RevBayesCore::RbVector<double> >("", new RevBayesCore::RbVector<double>(n,p));
    }
//    RevBayesCore::TypedDagNode<int>* mrs = static_cast<const Natural&>( this->args[3].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::DECRateMatrixFunction* f = new RevBayesCore::DECRateMatrixFunction( dr, er, rs );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_DECRateMatrix::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "dispersalRates"  , ModelVector<ModelVector<RealPos> >::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "extirpationRates", ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "rangeSize",        Simplex::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
//        argumentRules.push_back( new ArgumentRule( "maxRangeSize", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(RbConstants::Integer::max) ) );
        
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_DECRateMatrix::getClassType(void) {
    
    static std::string revType = "Func_DECRateMatrix";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_DECRateMatrix::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


const TypeSpec& Func_DECRateMatrix::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}