#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "RlMultispeciesCoalescentConstantPopulationProcess.h"
#include "MultispeciesCoalescent.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "Vector.h"

using namespace RevLanguage;


/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
MultispeciesCoalescentConstantPopulationProcess::MultispeciesCoalescentConstantPopulationProcess() : TypedDistribution<TimeTree>() 
{
    
}

/**
 * Clone the object 
 *
 * \return a clone of the object.
 */

MultispeciesCoalescentConstantPopulationProcess* MultispeciesCoalescentConstantPopulationProcess::clone(void) const {
    
    return new MultispeciesCoalescentConstantPopulationProcess(*this);
    
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
RevBayesCore::MultispeciesCoalescent* MultispeciesCoalescentConstantPopulationProcess::createDistribution( void ) const 
{
    
    // get the parameters
    // species tree
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* RbSpeciesTree       = static_cast<const TimeTree &>( speciesTree->getValue() ).getValueNode();

    // taxons
    std::map <std::string, std::string> g2s;
    RevBayesCore::TypedDagNode< std::vector<RevBayesCore::Taxon> > *t = static_cast<const Vector<Taxon> &>( taxons->getValue() ).getValueNode(); 
    for (size_t i = 0 ; i < t->getValue().size(); ++i)  {
        g2s[t->getValue()[i].getName()] = g2s[t->getValue()[i].getSpeciesName()];
    }

    // create the internal distribution object
    RevBayesCore::MultispeciesCoalescent*   d = new RevBayesCore::MultispeciesCoalescent(RbSpeciesTree, g2s);

    // the effective population size
    RevBayesCore::TypedDagNode<std::vector<double> >* RbNes;
    if ( Ne->getValueTypeSpec().isDerivedOf( Vector<RealPos>::getClassTypeSpec() ) ) 
        {
            RbNes = static_cast<const Vector<RealPos> &>( Ne->getValue() ).getValueNode();
            d->setNes(RbNes);

        }
        else 
        {
            RevBayesCore::TypedDagNode< double >* NeValue = static_cast<const RealPos &>( Ne->getValue() ).getValueNode();
            RevBayesCore::TypedDagNode< double >* RbNe = static_cast<const RealPos &>( Ne->getValue() ).getValueNode();
            d->setNe(RbNe);
        }
    //RevBayesCore::TypedDagNode<std::vector< double> >* RbNe       = static_cast<const Vector<RealPos> &>( Ne->getValue() ).getValueNode();
    
    return d;
}



/**
 * Get class name of object 
 *
 * \return The class' name.
 */
const std::string& MultispeciesCoalescentConstantPopulationProcess::getClassName(void) 
{ 
    
    static std::string rbClassName = "Multispecies Coalescent Constant Population Process";
    
	return rbClassName; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& MultispeciesCoalescentConstantPopulationProcess::getClassTypeSpec(void) 
{ 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( TypedDistribution<TimeTree>::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the homogeneous birth-death process are:
 * (1) Species tree.
 * (2) Population size.
 * (3) Gene name to species name map.
 * (4) the number of taxa.
 *
 * \return The member rules.
 */
const MemberRules& MultispeciesCoalescentConstantPopulationProcess::getMemberRules(void) const 
{
    
    static MemberRules distMultiSpeCoalConstPopMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distMultiSpeCoalConstPopMemberRules.push_back( new ArgumentRule( "speciesTree", true, TimeTree::getClassTypeSpec() ) );
        std::vector<TypeSpec> branchNeTypes;
        branchNeTypes.push_back( RealPos::getClassTypeSpec() );
        branchNeTypes.push_back( Vector<RealPos>::getClassTypeSpec() );
        distMultiSpeCoalConstPopMemberRules.push_back( new ArgumentRule( "branchNes"    , true, branchNeTypes ) );
        distMultiSpeCoalConstPopMemberRules.push_back( new ArgumentRule( "taxons"  , true, Vector<Taxon>::getClassTypeSpec() ) );
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
void MultispeciesCoalescentConstantPopulationProcess::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) 
{
    
    if ( name == "speciesTree" ) 
    {
        speciesTree = var;
    }
    else if ( name == "Ne" ) 
    {
        Ne = var;
    }
    else if ( name == "taxons" ) 
    {
        taxons = var;
    }
    else 
    {
        TypedDistribution<TimeTree>::setConstMemberVariable(name, var);
    }
    
}


/** Get type spec */
const TypeSpec& MultispeciesCoalescentConstantPopulationProcess::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



