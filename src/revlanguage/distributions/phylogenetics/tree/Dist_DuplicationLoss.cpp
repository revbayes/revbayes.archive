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

/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Dist_DuplicationLoss::getHelpAuthor(void) const
{
  // create a vector of authors for this function
  std::vector<std::string> authors;
  authors.push_back( "Dominik Schrempf, Sebastian Höhna." );
  return authors;
}

/**
 * Get the (brief) description for this function
 */
std::string Dist_DuplicationLoss::getHelpDescription(void) const
{
  // create a variable for the description of the function
  std::string description = "Duplication and loss distribution describing how gene trees can be generated from within a haplotype tree given a set of duplication and loss rate(s), and an origin time.";
  return description;
}

/**
 * Get the more detailed description of the function
 */
std::string Dist_DuplicationLoss::getHelpDetails(void) const
{
  // create a variable for the description of the function
  std::string details = "";
  details += "The haplotype tree must be ultrametric.\n";
  return details;
}

/**
 * Get an executable and instructive example.
 * These examples should help the users to show how this function works but
 * are also used to test if this function still works.
 */
std::string Dist_DuplicationLoss::getHelpExample(void) const
{
  // create an example as a single string variable.
  std::string example = "";
  // TODO.
  // example += "# We are going to save the trees we simulate in the folder simulatedTrees:\n";
  // example += "dataFolder = \"simulatedTrees/\" \n";
  // example += "# Let’s simulate a species tree with 10 taxa, 2 gene trees, 3 alleles per species:\n";
  // example += "n_species <- 10\n";
  // example += "n_genes <- 2\n";
  // example += "n_alleles <- 3\n";
  // example += "# we simulate an ultrametric species tree:\n";
  // example += "# Species names:\n";
  // example += "for (i in 1:n_species) {\n";
  // example += "        species[i] <- taxon(taxonName=\"Species_\"+i, speciesName=\"Species_\"+i)\n";
  // example += "}\n";
  // example += "spTree ~ dnBirthDeath(lambda=0.3, mu=0.2, rootAge=10, rho=1, samplingStrategy=\"uniform\", condition=\"nTaxa\", taxa=species)\n";
  // example += "print(spTree)\n";
  // example += "# let's pick a constant effective population size of 50:\n";
  // example += "popSize <- 50\n";
  // example += "# let's simulate gene trees now:\n";
  // example += "# taxa names:\n";
  // example += "for (g in 1:n_genes) {\n";
  // example += "  for (i in 1:n_species) {\n";
  // example += "    for (j in 1:n_alleles) {\n";
  // example += "        taxons[g][(i-1)*n_alleles+j] <- taxon(taxonName=\"Species_\"+i+\"_\"+j, speciesName=\"Species_\"+i)\n";
  // example += "    }\n";
  // example += "  }\n";
  // example += "  geneTrees[g] ~ dnMultiSpeciesCoalescent(speciesTree=spTree, Ne=popSize, taxa=taxons[g])\n";
  // example += "  print(geneTrees[g])\n";
  // example += "}\n";
  // example += "# We can save the species tree and the gene trees: \n";
  // example += "write(spTree, filename=dataFolder+\"speciesTree\")\n";
  // example += "# Saving the gene trees\n";
  // example += "for (i in 1:(n_genes)) {\n";
  // example += "  write(geneTrees[i], filename=dataFolder+\"geneTree_\"+i+\".tree\")\n";
  // example += "}\n";
  // example += "\n";
  return example;
}

/**
 * Get some references/citations for this function
 *
 */
std::vector<RevBayesCore::RbHelpReference> Dist_DuplicationLoss::getHelpReferences(void) const
{
  // create an entry for each reference
  std::vector<RevBayesCore::RbHelpReference> references;
  RevBayesCore::RbHelpReference ref = RevBayesCore::RbHelpReference();
  ref.setCitation("Unpublished.");
  ref.setDoi("");
  ref.setUrl("");
  references.push_back(ref);
  return references;
}

/**
 * Get the names of similar and suggested other functions.
 */
std::vector<std::string> Dist_DuplicationLoss::getHelpSeeAlso(void) const
{
  // create an entry for each suggested function
  std::vector<std::string> see_also;
  see_also.push_back( "dnTimeVaryingStateDependentSpeciationExtinction" );
  return see_also;
}

/**
 * Get the title of this help entry.
 */
std::string Dist_DuplicationLoss::getHelpTitle(void) const
{
  std::string title = "Duplication and loss process.";
  return title;
}
