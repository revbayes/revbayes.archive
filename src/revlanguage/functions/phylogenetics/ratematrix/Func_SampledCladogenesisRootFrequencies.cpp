//
//  Func_SampledCladogenesisRootFrequencies.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 8/11/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#include "Func_SampledCladogenesisRootFrequencies.h"
#include "AbstractCharacterHistoryBirthDeathProcess.h"
#include "ConstantNode.h"
#include "Integer.h"
#include "Natural.h"
#include "Probability.h"
#include "RbConstants.h"
#include "Real.h"
#include "RealPos.h"
#include "RlMatrixReal.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "RlTimeTree.h"
#include "SampledCladogenesisRootFrequenciesFunction.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_SampledCladogenesisRootFrequencies::Func_SampledCladogenesisRootFrequencies( void ) : TypedFunction<Simplex>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_SampledCladogenesisRootFrequencies* Func_SampledCladogenesisRootFrequencies::clone( void ) const
{
    
    return new Func_SampledCladogenesisRootFrequencies( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RbVector< double > >* Func_SampledCladogenesisRootFrequencies::createFunction( void ) const
{
    
    // rate matrix
    RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* q = static_cast<const RateGenerator &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    
    // cladogenetic probabilities
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal>* cp = static_cast<const MatrixReal &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    
    // tree
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tmp = static_cast<const TimeTree &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::Tree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Tree> *>( tmp );
    
    // clock
    RevBayesCore::TypedDagNode<double>* r = static_cast<const RealPos &>( this->args[3].getVariable()->getRevObject() ).getDagNode();
    
    // check state space matches
    size_t numStatesQ = q->getValue().getNumberOfStates();
    size_t numStatesCP = cp->getValue().getNumberOfRows();
    if (numStatesQ != numStatesCP) {
        throw RbException("The anagenetic rate matrix and cladogenetic probability matrix do have the same number of states.");
    }
    
    // check tree is sampled speciation birth death process
//    t->getDagNode()->getDistribution();
    bool isSampledSpeciation = ( dynamic_cast<const RevBayesCore::AbstractCharacterHistoryBirthDeathProcess* >( &(t->getDistribution()) ) == NULL );
    
    if (isSampledSpeciation) {
        throw RbException("The tree variable is not a sampled speciation tree.");
    }
    
    RevBayesCore::SampledCladogenesisRootFrequenciesFunction* f = new RevBayesCore::SampledCladogenesisRootFrequenciesFunction( q, cp, t, r );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_SampledCladogenesisRootFrequencies::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "Q", RateGenerator::getClassTypeSpec(), "The anagenetic event rate matrix", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "cladogeneticProbabilities", MatrixReal::getClassTypeSpec(), "The cladogenetic event probabilities", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::DETERMINISTIC ) );
        argumentRules.push_back( new ArgumentRule( "tree", TimeTree::getClassTypeSpec(), "The time-tree variable containtain the sampled speciation events", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        argumentRules.push_back( new ArgumentRule( "clock", RealPos::getClassTypeSpec(), "The anagenetic clock rate", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_SampledCladogenesisRootFrequencies::getClassType(void)
{
    
    static std::string revType = "Func_SampledCladogenesisRootFrequencies";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_SampledCladogenesisRootFrequencies::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_SampledCladogenesisRootFrequencies::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnSampledCladogenesisRootFrequencies";
    
    return f_name;
}


const TypeSpec& Func_SampledCladogenesisRootFrequencies::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}