/*
 * File:   Dist_branchHeterogeneousOrnsteinUhlenbeck.cpp
 * Author: Bastien Boussau
 *
 * Created on 05/10/14.
 */

#include "ArgumentRule.h"
#include "Dist_branchHeterogeneousOrnsteinUhlenbeck.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RealNodeContainer.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "TimeTree.h"

using namespace RevLanguage;


Dist_branchHeterogeneousOrnsteinUhlenbeck* Dist_branchHeterogeneousOrnsteinUhlenbeck::clone( void ) const {
    return new Dist_branchHeterogeneousOrnsteinUhlenbeck(*this);
}


RevBayesCore::BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess* Dist_branchHeterogeneousOrnsteinUhlenbeck::createDistribution( void ) const {
    // get the parameters
    
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();

    size_t nNodes = tau->getValue().getNumberOfNodes();
    RevBayesCore::BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess* dist = new RevBayesCore::BranchHeterogeneousOrnsteinUhlenbeckPhyloProcess( tau );

    //Sigma:
    if ( sigma->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* s = static_cast<const ModelVector<RealPos> &>( sigma->getRevObject() ).getDagNode();

        // sanity check
        if ( (nNodes-1) != s->getValue().size() )
        {
            throw RbException( "The number of sigma values does not match the number of branches" );
        }
        
        dist->setSigma( s );
    }
    else
    {
        RevBayesCore::TypedDagNode< double >* s = static_cast<const RealPos &>( sigma->getRevObject() ).getDagNode();
        dist->setSigma( s );
    }
    
    //Mean:
    if ( mean->getRevObject().isType( ModelVector<Real>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* m = static_cast<const ModelVector<Real> &>( mean->getRevObject() ).getDagNode();
        
        // sanity check
        if ( (nNodes-1) != m->getValue().size() )
        {
            throw RbException( "The number of mean values does not match the number of branches" );
        }
        
        dist->setMean( m );
    }
    else
    {
        RevBayesCore::TypedDagNode< double >* m = static_cast<const Real &>( mean->getRevObject() ).getDagNode();
        dist->setMean( m );
    }

    //Phi:
    if ( phi->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* p = static_cast<const ModelVector<RealPos> &>( phi->getRevObject() ).getDagNode();
        
        // sanity check
        if ( (nNodes-1) != p->getValue().size() )
        {
            throw RbException( "The number of phi values does not match the number of branches" );
        }
        
        dist->setPhi( p );
    }
    else
    {
        RevBayesCore::TypedDagNode< double >* p = static_cast<const RealPos &>( phi->getRevObject() ).getDagNode();
        dist->setPhi( p );
    }
    
    return dist;
    
}



/* Get Rev type of object */
const std::string& Dist_branchHeterogeneousOrnsteinUhlenbeck::getClassType(void) {
    
    static std::string revType = "Dist_branchHeterogeneousOrnsteinUhlenbeck";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_branchHeterogeneousOrnsteinUhlenbeck::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Dist_branchHeterogeneousOrnsteinUhlenbeck::getMemberRules(void) const {
    
    static MemberRules dist;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        dist.push_back( new ArgumentRule( "tree" , TimeTree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        std::vector<TypeSpec> posTypes;
        posTypes.push_back( RealPos::getClassTypeSpec() );
        posTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        dist.push_back( new ArgumentRule( "sigma", posTypes, ArgumentRule::BY_CONSTANT_REFERENCE ) );

        std::vector<TypeSpec> types;
        types.push_back( RealPos::getClassTypeSpec() );
        types.push_back( ModelVector<Real>::getClassTypeSpec() );
        dist.push_back( new ArgumentRule( "mean" , types, ArgumentRule::BY_CONSTANT_REFERENCE ) );
        dist.push_back( new ArgumentRule( "phi"  , posTypes, ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return dist;
}


const TypeSpec& Dist_branchHeterogeneousOrnsteinUhlenbeck::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}




/** Print value for user */

void Dist_branchHeterogeneousOrnsteinUhlenbeck::printValue(std::ostream& o) const {
    
    o << " branchHeterogeneousOrnsteinUhlenbeck(";
    
    o << "tau=";
    if ( tree != NULL ) {
        o << tree->getName();
    } else {
        o << "?";
    }
    
    o << ",";
    
    o << "sigma=";
    if ( sigma != NULL ) {
        o << sigma->getName();
    } else {
        o << "?";
    }
    
    o << ",";
    
    o << "mean=";
    if ( mean != NULL ) {
        o << mean->getName();
    } else {
        o << "?";
    }
    
    o << ",";
    
    o << "phi=";
    if ( phi != NULL ) {
        o << phi->getName();
    } else {
        o << "?";
    }
    
    o << ")";
}



/** Set a member variable */
void Dist_branchHeterogeneousOrnsteinUhlenbeck::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "sigma" )
    {
        sigma = var;
    }
    else if ( name == "mean" )
    {
        mean = var;
    }
    else if ( name == "phi" )
    {
        phi = var;
    }
    else {
        Distribution::setConstParameter(name, var);
    }
}

