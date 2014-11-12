#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "Dist_constPopMultispCoal.h"
#include "ModelVector.h"
#include "MultispeciesCoalescent.h"
#include "Natural.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"

using namespace RevLanguage;


/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Dist_constPopMultispCoal::Dist_constPopMultispCoal() : TypedDistribution<TimeTree>() 
{
    
}

/**
 * Clone the object 
 *
 * \return a clone of the object.
 */

Dist_constPopMultispCoal* Dist_constPopMultispCoal::clone(void) const {
    
    return new Dist_constPopMultispCoal(*this);
    
}




/**
 * Create a new internal distribution object.
 *
 * This function simply dynamically allocates a new internal distribution object that can be 
 * associated with the variable. The internal distribution object is created by calling its
 * constructor and passing the distribution-parameters (other DAG nodes) as arguments of the 
 * constructor. The distribution constructor takes care of the proper hook-ups.
 *
 * \return A new internal distribution object.
 */
RevBayesCore::MultispeciesCoalescent* Dist_constPopMultispCoal::createDistribution( void ) const 
{
    
    // Get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* st = static_cast<const TimeTree &>( speciesTree->getRevObject() ).getDagNode();
    const std::vector<RevBayesCore::Taxon>      &t  = static_cast<const ModelVector<Taxon> &>( taxa->getRevObject() ).getValue();
    
    // get the number of nodes for the tree
    size_t nNodes = st->getValue().getNumberOfNodes();
    
    
    RevBayesCore::MultispeciesCoalescent*   d = new RevBayesCore::MultispeciesCoalescent( st, t );
    
    if ( Ne->getRevObjectTypeSpec().isDerivedOf( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* neNode = static_cast<const ModelVector<RealPos> &>( Ne->getRevObject() ).getDagNode();
        
        // sanity check
        if ( (nNodes-1) != neNode->getValue().size() )
        {
            throw RbException( "The number of clock rates does not match the number of branches" );
        }
        
        d->setNes( neNode );
    }
    else
    {
        RevBayesCore::TypedDagNode<double>* neNode = static_cast<const RealPos &>( Ne->getRevObject() ).getDagNode();
        d->setNe( neNode );
    }
    d->redrawValue();
    
    return d;
}



/**
 * Get Rev type of object 
 *
 * \return The class' name.
 */
const std::string& Dist_constPopMultispCoal::getClassType(void) 
{ 
    
    static std::string revType = "Dist_constPopMultispCoal";
    
	return revType; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_constPopMultispCoal::getClassTypeSpec(void) 
{ 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<TimeTree>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the Multispecies Coalescent process are:
 * (1) Species tree.
 * (2) Population size.
 * (3) Gene name to species name map.
 * (4) the number of taxa.
 *
 * \return The member rules.
 */
const MemberRules& Dist_constPopMultispCoal::getParameterRules(void) const 
{
    
    static MemberRules distMultiSpeCoalConstPopMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distMultiSpeCoalConstPopMemberRules.push_back( new ArgumentRule( "speciesTree", TimeTree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        std::vector<TypeSpec> branchNeTypes;
        branchNeTypes.push_back( RealPos::getClassTypeSpec() );
        branchNeTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        distMultiSpeCoalConstPopMemberRules.push_back( new ArgumentRule( "Ne"    , branchNeTypes, ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distMultiSpeCoalConstPopMemberRules.push_back( new ArgumentRule( "taxa"  , ModelVector<Taxon>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        rulesSet = true;
    }
    
    return distMultiSpeCoalConstPopMemberRules;
}


/** 
 * Set a member variable.
 * 
 * Sets a member variable with the given name and store the pointer to the variable.
 * The value of the variable might still change but this function needs to be called again if the pointer to
 * the variable changes. The current values will be used to create the distribution object.
 *
 * \param[in]    name     Name of the member variable.
 * \param[in]    var      Pointer to the variable.
 */
void Dist_constPopMultispCoal::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) 
{
    
    if ( name == "speciesTree" ) 
    {
        speciesTree = var;
    }
    else if ( name == "Ne" ) 
    {
        Ne = var;
    }
    else if ( name == "taxa" )
    {
        taxa = var;
    }
    else 
    {
        TypedDistribution<TimeTree>::setConstParameter(name, var);
    }
    
}


/** Get type spec */
const TypeSpec& Dist_constPopMultispCoal::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



