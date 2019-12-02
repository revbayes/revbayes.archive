#include <math.h>
#include <stddef.h>
#include <iosfwd>
#include <string>
#include <vector>

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_DuplicationLoss.h"
#include "ModelVector.h"
#include "OptionRule.h"
#include "RealPos.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "ConstantNode.h"
#include "DagMemberFunction.h"
#include "DeterministicNode.h"
#include "DuplicationLossProcess.h"
#include "DynamicNode.h"
#include "ModelObject.h"
#include "RbException.h"
#include "RbHelpReference.h"
#include "RbVector.h"
#include "RevObject.h"
#include "RevPtr.h"
#include "RevVariable.h"
#include "RlDagMemberFunction.h"
#include "RlDeterministicNode.h"
#include "RlStochasticNode.h"
#include "RlTypedDistribution.h"
#include "RlTypedFunction.h"
#include "Tree.h"
#include "TypeSpec.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"
#include "TypedFunction.h"

namespace RevBayesCore { class Taxon; }

using namespace RevLanguage;


/**
 * Default constructor.
 *
 * The default constructor does nothing except allocating the object.
 */
Dist_DuplicationLoss::Dist_DuplicationLoss() : TypedDistribution<TimeTree>()
{
}

/**
 * Clone the object
 *
 * \return a clone of the object.
 */
Dist_DuplicationLoss* Dist_DuplicationLoss::clone(void) const
{
  return new Dist_DuplicationLoss(*this);
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
RevBayesCore::DuplicationLossProcess* Dist_DuplicationLoss::createDistribution( void ) const
{
  // Get the parameters
  RevBayesCore::TypedDagNode<RevBayesCore::Tree>* ind_tree = static_cast<const TimeTree &>( individual_tree->getRevObject() ).getDagNode();
  const std::vector<RevBayesCore::Taxon>      &t  = static_cast<const ModelVector<Taxon> &>( taxa->getRevObject() ).getValue();

  RevBayesCore::TypedDagNode<double>* org = static_cast<const RealPos &>( origin->getRevObject() ).getDagNode();

  // get the number of nodes for the tree
  size_t n_nodes = ind_tree->getValue().getNumberOfNodes();
  size_t n_tips = ind_tree->getValue().getNumberOfTips();

  // condition
  const std::string& cond = static_cast<const RlString &>( condition->getRevObject() ).getValue();


  RevBayesCore::DuplicationLossProcess*   d = new RevBayesCore::DuplicationLossProcess( ind_tree, org, t, cond );

  RevBayesCore::ConstantNode< RevBayesCore::RbVector<double> > *sampling =
    new RevBayesCore::ConstantNode< RevBayesCore::RbVector<double> >("gene_sampling", new RevBayesCore::RbVector<double>(n_tips,1.0) );

  d->setGeneSamplingProbability( sampling );

  if ( lambda->getRequiredTypeSpec().isDerivedOf( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
      RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* lambda_node =
        static_cast<const ModelVector<RealPos> &>( lambda->getRevObject() ).getDagNode();

      // Sanity check.
      if ( n_nodes != lambda_node->getValue().size() )
        {
          throw RbException( "The number of duplication rates does not match the number of branches." );
        }
      d->setDuplicationRate( lambda_node );
    }
  else
    {
      RevBayesCore::TypedDagNode<double>* lambda_node = static_cast<const RealPos &>( lambda->getRevObject() ).getDagNode();
      d->setDuplicationRate( lambda_node );
    }
  if ( mu->getRequiredTypeSpec().isDerivedOf( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
      RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* mu_node = static_cast<const ModelVector<RealPos> &>( mu->getRevObject() ).getDagNode();
      // Sanity check.
      if ( n_nodes != mu_node->getValue().size() )
        {
          throw RbException( "The number of loss rates does not match the number of branches." );
        }
      d->setLossRate( mu_node );
    }
  else
    {
      RevBayesCore::TypedDagNode<double>* mu_node = static_cast<const RealPos &>( mu->getRevObject() ).getDagNode();
      d->setLossRate( mu_node );
    }
  d->redrawValue();
  return d;
}



/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Dist_DuplicationLoss::getClassType(void)
{
  static std::string rev_type = "Dist_DuplicationLoss";
  return rev_type;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_DuplicationLoss::getClassTypeSpec(void)
{
  static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<TimeTree>::getClassTypeSpec() ) );
  return rev_type_spec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_DuplicationLoss::getDistributionFunctionName( void ) const
{
  // create a distribution name variable that is the same for all instance of this class
  std::string d_name = "DuplicationLoss";
  return d_name;
}


/**
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the duplication and loss process are:
 * (1) Haplotype tree.
 * (2) Duplication rate(s).
 * (3) Loss rate(s).
 * (4) Origin time.
 * (5) Gene name to haplotype name map.
 * Optional:
 * (1) Condition on observed genes, or on survival of the process.
 *
 * \return The member rules.
 */
const MemberRules& Dist_DuplicationLoss::getParameterRules(void) const
{
  static MemberRules member_rules;
  static bool rules_set = false;

  if ( !rules_set )
    {
      // (1) Haplotype tree.
      member_rules.push_back( new ArgumentRule( "individualTree", TimeTree::getClassTypeSpec(), "The individual tree in which the gene trees evolve.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

      // (2) Duplication rate(s).
      std::vector<TypeSpec> branch_lambda_types;
      branch_lambda_types.push_back( RealPos::getClassTypeSpec() );
      branch_lambda_types.push_back( ModelVector<RealPos>::getClassTypeSpec() );
      member_rules.push_back( new ArgumentRule( "lambda"    , branch_lambda_types, "The duplication rate(s).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

      // (3) Loss rate(s).
      std::vector<TypeSpec> branch_mu_types;
      branch_mu_types.push_back( RealPos::getClassTypeSpec() );
      branch_mu_types.push_back( ModelVector<RealPos>::getClassTypeSpec() );
      member_rules.push_back( new ArgumentRule( "mu"    , branch_mu_types, "The loss rate(s).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

      // (4) Origin time.
      member_rules.push_back( new ArgumentRule( "origin", RealPos::getClassTypeSpec(), "Time of origin.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

      // (5) Gene name to haplotype name map.
      member_rules.push_back( new ArgumentRule( "taxa"  , ModelVector<Taxon>::getClassTypeSpec(), "The vector of taxa which have species and individual names.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );

      // Optional (1); conditioning on genes or on survival.
      std::vector<std::string> options_condition;
      //        options_condition.push_back( "time" );
      options_condition.push_back( "survival" );
      options_condition.push_back( "genes" );
      member_rules.push_back( new OptionRule( "condition", new RlString("survival"), options_condition, "The condition of the process." ) );

      rules_set = true;
    }
  return member_rules;
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
void Dist_DuplicationLoss::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{

  if ( name == "individualTree" )
    {
      individual_tree = var;
    }
  else if ( name == "lambda" )
    {
      lambda = var;
    }
  else if ( name == "mu" )
    {
      mu = var;
    }
  else if ( name == "origin" )
    {
      origin = var;
    }
  else if ( name == "taxa" )
    {
      taxa = var;
    }
  else if ( name == "condition" )
    {
      condition = var;
    }
  else
    {
      TypedDistribution<TimeTree>::setConstParameter(name, var);
    }
}

/** Get type spec */
const TypeSpec& Dist_DuplicationLoss::getTypeSpec( void ) const
{
  static TypeSpec type_spec = getClassTypeSpec();
  return type_spec;
}

