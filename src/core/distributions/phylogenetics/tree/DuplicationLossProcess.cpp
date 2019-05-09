#include "Clade.h"
#include "ConstantNode.h"
#include "DuplicationLossProcess.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

DuplicationLossProcess::DuplicationLossProcess(const TypedDagNode<Tree> *it, const TypedDagNode<double> *org, const std::vector<Taxon> &t, const std::string &cond)
  : TypedDistribution<Tree>( NULL ),
    taxa(t),
    individual_tree( it ),
    num_taxa( taxa.size() ),
    log_tree_topology_prob (0.0),
    origin( org ),
    duplication_rate( new ConstantNode<double>("", new double(1.0)) ),
    loss_rate( new ConstantNode<double>("", new double(0.5)) ),
    condition( cond )
{
  // Test that the origin time is older than the root age of the haplotype
  // tree.
  double org_time = origin->getValue();
  TopologyNode haplotype_root_node = individual_tree->getValue().getRoot();
  double haplotype_root_age = haplotype_root_node.getAge();
  if ( org_time < haplotype_root_age )
    {
      std::string err = "Origin time " + StringUtilities::to_string(org_time);
      err += " lower than root age of haplotype tree " + StringUtilities::to_string(haplotype_root_age);
      throw RbException(err);
    }

  // TODO: For now, we set the origin time of the haplotype tree to the origin
  // time of the duplication and loss process. In the future, we might remove
  // the origin time parameter, and rather only use the haplotype origin time,
  // which is certainly cleaner.
  double hap_root_length = org_time - haplotype_root_age;
  haplotype_root_node.setBranchLength(hap_root_length);

  // add the parameters to our set (in the base class) in that way other
  // classes can easily access the set of our parameters this will also ensure
  // that the parameters are not getting deleted before we do
  addParameter( individual_tree );
  addParameter( origin );

  addParameter( duplication_rate );
  addParameter( loss_rate );

}

DuplicationLossProcess::~DuplicationLossProcess()
{
}

DuplicationLossProcess* DuplicationLossProcess::clone(void) const
{
  return new DuplicationLossProcess(*this);
}

double DuplicationLossProcess::computeLnProbability( void )
{
  // Test that the origin time is older than the root age of the haplotype
  // tree.
  double org_time = origin->getValue();
  TopologyNode haplotype_root_node = individual_tree->getValue().getRoot();
  double haplotype_root_age = haplotype_root_node.getAge();
  if ( org_time < haplotype_root_age )
    {
      return RbConstants::Double::neginf;
    }

  // TODO: For now, we set the origin time of the haplotype tree to the origin
  // time of the duplication and loss process. In the future, we might remove
  // the origin time parameter, and rather only use the haplotype origin time,
  // which is certainly cleaner.
  double hap_root_length = org_time - haplotype_root_age;
  haplotype_root_node.setBranchLength(hap_root_length);

  // To be safe, we always reset the tip allications at the beginning of the
  // probability computation. We could check if it is sufficient to do so only
  // once at initialization, when values are set.
  resetTipAllocations();

  // Same for extinction probabilities.
  extinction_probs = std::vector<double>(individual_tree->getValue().getNumberOfNodes(), 0.0);
  computeAllEs(haplotype_root_node);

  // Now calculate the probability of observing the gene tree (recursively).
  double ln_prob_dl = recursivelyComputeLnProbability( haplotype_root_node );

  if ( condition == "survival" )
    {
      // NOTE: We have to use the ORIGIN OF THE GENE TREE, not the INDIVIDUAL
      // TREE. For now, the origin of the haplotype is set to the origin of the
      // gene trees. See constructor.
      size_t root_index = haplotype_root_node.getIndex();
      ln_prob_dl -= log( 1.0 - extinction_probs[root_index] );
    }

  return ln_prob_dl;
}

double DuplicationLossProcess::computeLnDuplicationLossProbability(size_t num_genes_recent, const std::vector<double> &dupl_ages, double age_recent, double age_ancient, const TopologyNode &node_individual, bool is_root)
{
  double ln_prob = 0.0;
  // size_t index_individual = node_individual.getIndex();
  double dupl_rate = duplication_rate->getValue();
  double current_age = age_recent;
  double current_ext_prob = 0.0;

  // Get extinction probability at the bottom of this branch.
  if ( node_individual.isTip() )
    {
      // TODO: We need to use the actual sampling probabilities.
      current_ext_prob = 0.0;
      // current_ext_prob = 1.0 - gene_sampling_probabilities->getValue()[index_individual];
    }
  else
    {
      size_t index_left = node_individual.getChild(0).getIndex();
      size_t index_right = node_individual.getChild(1).getIndex();
      double ext_prob_left  = extinction_probs[index_left];
      double ext_prob_right = extinction_probs[index_right];
      current_ext_prob = ext_prob_left * ext_prob_right;
    }

  if (dupl_ages.size() >= num_genes_recent)
    {
      throw std::length_error("There are more duplication events than branches on this branch of the individual tree.");
    }

  // Handle duplications.
  for ( size_t i=0; i<dupl_ages.size(); ++i )
    {
      double this_dupl_age = dupl_ages[i];
      double dt = this_dupl_age - current_age;

      // Walk up dt to the next duplication on the branch of the individual tree.
      // It contains 'N = num_genes_recent-i' genes.
      // FIXME: We need to tell D what dop-loss rates to use when we want to use haplotype-branch-specific rates
      ln_prob += (num_genes_recent-i) * log( computeD(dt, current_ext_prob) );

      // Have a duplication.
      ln_prob += log( dupl_rate );
      // Combinatorial factor. The duplication can happen on any of the 'N-1'
      // branches existing earlier (i.e., before the duplication happens when
      // going forwards in time), but we require it to happen on a specific
      // branch.
      ln_prob += log( num_genes_recent - i - 1 );

      current_age = this_dupl_age;
      // FIXME: We need to tell D what dop-loss rates to use when we want to use haplotype-branch-specific rates
      current_ext_prob = computeE( dt, current_ext_prob );
    }

  // Final branch segment before coalescence on individual tree, i.e., there are
  // no more duplications.
  if ( is_root == true )
    {
      if ( num_genes_recent - dupl_ages.size() != 1  )
        {
          throw RbException("Problem in the probability computation of the duplication and loss process! We are at the root node. There are " + StringUtilities::to_string(num_genes_recent) + " genes at the recent end of the branch, and had " + StringUtilities::to_string(dupl_ages.size()) + " duplication events.");
        }
      double dt = origin->getValue() - current_age;
      ln_prob += log( computeD(dt, current_ext_prob) );
      // Do not set these here, but calculate them beforehand.
      // extinction_probs[index_individual] = computeE( dt, current_ext_prob );
      return ln_prob;
    }
  else
    {
      double dt = age_ancient - current_age;
      ln_prob += (num_genes_recent-dupl_ages.size()) * log( computeD(dt, current_ext_prob) );
      // Do not set these here, but calculate them beforehand.
      // extinction_probs[index_individual] = computeE( dt, current_ext_prob );
      return ln_prob;
    }
}

double DuplicationLossProcess::computeD(double dt, double p_e)
{
  const double la = duplication_rate->getValue();
  const double mu = loss_rate->getValue();

  // Tue Mar  5 14:34:30 CET 2019: Discrepancies between formulas have been resolved.

  // Sebastian's formula (see below) matches Eq (1) and (2) in Stadler, T.
  // (2011). Mammalian phylogeny reveals recent diversification rate shifts.
  // Proceedings of the National Academy of Sciences, 108(15), 6187–6192.
  // http://doi.org/10.1073/pnas.1016876108

  // A different parametrization (boundary values of E and D) is used in Eq (1)
  // and (2) in Stadler, T. (2010). Sampling-through-time in birth-death trees.
  // Journal of Theoretical Biology, 267(3), 396–404.
  // http://doi.org/10.1016/j.jtbi.2010.09.010

  // Wandrille also uses a formula that looks different, see mail from Gergely
  // 2018-05-16 "probabilities for birth death on a tree". I did not validate
  // the equality of his formula to the ones used here.

  // Formula from Sebastian.
  // double A = la - mu;
  // double B = ( (1.0 - 2*e)*la+mu ) / A;
  // double et = exp(-A * dt);
  // double D = 4.0 * et;
  // D /= (1.0+B+et*(1-B));
  // D /= (1.0+B+et*(1-B));
  // return D;

  // Eq (2) from PNAS paper of Stadler.
  // e       :: Index of the considered branch.
  // t       :: Time on complete phylogeny (0 at present, increases into the past).
  // g(t, e) :: Probability density that the species/gene on edge e at time t
  //            evolved between t and the present as observed in the phylogeny.
  // dt      :: Time on the branch (dt=0 means bottom of branch).
  // p_e     :: Probability of going extinct between the bottom of branch e and the
  //            present. Also called E(0).

  // It turns out that the dependency on time t of g(t,e) can be factored out
  // and that g(t,e) only depends on the time dt running on the considered
  // branch e, and on the boundary condition p_e at the bottom of the considered
  // branch.

  // Technically, we also need to know g(t, f) and g(t, h), where f and h are
  // the daughter branches of the considered branch e. See the variable d_{i,e}
  // in Stadlers paper. However, since this is only a multiplicative factor, we
  // apply it when calculating the likelihood. See
  // 'DuplicationLossProcess::computeLnDuplicationLossProbability'.
  double d           = la - mu;
  double et          = exp(-d * dt);
  double nominator   = d * d *et;
  double denominator = la*(p_e - 1.0) + (mu - p_e*la)*et;
  double g_te        = nominator / denominator / denominator;
  return g_te;
}

// Tue Apr 16 13:54:41 CEST 2019 BUGFIX. The extinction probability for branches
// without genes was not calculated. This is now done separately before the
// actual likelihood calculcation.
void DuplicationLossProcess::computeAllEs(const TopologyNode &hap_node) {
  size_t index = hap_node.getIndex();
  double length = hap_node.getBranchLength();

  // TODO: Sampling probabilitites.
  if ( hap_node.isTip() )
    extinction_probs[index] = computeE(length, 0.0);
  else {
    const TopologyNode &left = hap_node.getChild(0);
    size_t left_index = left.getIndex();
    const TopologyNode &right = hap_node.getChild(1);
    size_t right_index = right.getIndex();
    computeAllEs(left);
    computeAllEs(right);

    double ext_prob_bottom = extinction_probs[left_index] * extinction_probs[right_index];
    extinction_probs[index] = computeE(length, ext_prob_bottom);
    }
}

double DuplicationLossProcess::computeE(double dt, double p_e)
{
  const double la = duplication_rate->getValue();
  const double mu = loss_rate->getValue();

  // See comments in 'computeD()'.

  // Formula from Sebastian.
  // double A = la - mu;
  // double B = ( (1.0 - 2*e)*la+mu ) / A;
  // double et = exp(A * dt);
  // double E = la + mu - A *(1.0+B-et*(1.0-B))/(1.0+B+et*(1.0-B));
  // E /= (2*la);

  // Eq. (1) from PNAS paper of Stadler.
  double d           = la - mu;
  double et          = exp(-d * dt);
  double A           = mu - p_e * la;
  double nominator   = mu*(p_e - 1.0) + A*et;
  double denominator = la*(p_e - 1.0) + A*et;
  double p_te        = nominator / denominator;
  return p_te;
}

double DuplicationLossProcess::recursivelyComputeLnProbability( const RevBayesCore::TopologyNode &individual_node )
{
  double ln_prob_dupl_loss = 0;
  double individual_age = individual_node.getAge();

  // Parent individual age is infinite, if at root.
  double parent_individual_age = RbConstants::Double::inf;
  if ( individual_node.isRoot() == false )
    {
      const TopologyNode &individual_parent_node = individual_node.getParent();
      parent_individual_age = individual_parent_node.getAge();
    }

  //////////////////////////////////////////////////
  // 1. Handle inner nodes. At the leaves, the genes have to be set a priori,
  // e.g., during the simulation.

  if ( individual_node.isTip() == false )
    {
      // The genes at inner nodes (genes_per_branch_recent) will be set when
      // traversing the children (using their respective
      // genes_per_branch_ancient).
      genes_per_branch_recent[ individual_node.getIndex() ].clear();
      // Add likelihood from children.
      for (size_t i=0; i<individual_node.getNumberOfChildren(); ++i)
        {
          ln_prob_dupl_loss += recursivelyComputeLnProbability( individual_node.getChild(i) );
        }

      // Handle coalescence of individuals on gene tree. I.e., if genes are
      // merging at the same time, consider this event being part of the
      // coalescence. If, on the other hand, they do not join, a loss must have
      // happened (and also a duplication at a previous time point). Here we
      // assume that the tree is BIFURCATING.
      size_t left_index  = individual_node.getChild(0).getIndex();
      size_t right_index = individual_node.getChild(1).getIndex();
      std::set< const TopologyNode* > &genes_for_this_individual = genes_per_branch_recent[ individual_node.getIndex() ];
      const std::set< const TopologyNode* > &genes_for_left_descendant  = genes_per_branch_ancient[ left_index ];
      std::set< const TopologyNode* > genes_to_remove;
      std::set< const TopologyNode* > genes_to_insert;
      for (std::set<const TopologyNode*>::iterator it=genes_for_this_individual.begin(); it!=genes_for_this_individual.end(); ++it)
        {
          const TopologyNode *this_gene_node = *it;
          double this_age = RbConstants::Double::inf;
          if ( this_gene_node->isRoot() == false )
            {
              this_age = this_gene_node->getParent().getAge();
            }
          else
            {
              continue;
            }

          if ( fabs( this_age - individual_age) < EPS_COAL )
            {
              // Coalescent event; likelihood unchanged.
              genes_to_remove.insert( this_gene_node );
              genes_to_insert.insert( &this_gene_node->getParent() );

            }
          else
            {
              // Check if this gene comes from the left or right descendant.
              if ( genes_for_left_descendant.find(this_gene_node) != genes_for_left_descendant.end() )
                {
                  // Found in the left descendant.
                  // Multiply with probability of loss in right descendant.
                  ln_prob_dupl_loss += log(extinction_probs[right_index]);
                }
              else
                {
                  // Not found in the left descendant. We expect only two
                  // descendants and hence, the loss happens in the left
                  // descendant.
                  ln_prob_dupl_loss += log(extinction_probs[left_index]);
                }
            }
        }


      // Now we remove the gene that were coalescent events.
      for (std::set<const TopologyNode*>::const_iterator it=genes_to_remove.begin(); it!=genes_to_remove.end(); ++it)
        {
          genes_for_this_individual.erase( *it );
        }
      for (std::set<const TopologyNode*>::const_iterator it=genes_to_insert.begin(); it!=genes_to_insert.end(); ++it)
        {
          genes_for_this_individual.insert( *it );
        }
    }


  //////////////////////////////////////////////////
  // 2. Handle branch attached to node (bottom up, recent to ancient).

  // Create a local copy of the genes per branch.
  // initial_genes   :: Genes present at the bottom of the branch (more recent to the present, 'genes_per_branch_recent').
  // remaining_genes :: Genes that remain at the top of the individual branch (may be less because of duplication events, will be 'genes_per_branch_ancient').
  const std::set<const TopologyNode*> &initial_genes = genes_per_branch_recent[individual_node.getIndex()];
  std::set<const TopologyNode*> remaining_genes = initial_genes;

  //////////
  // 2a. Get duplication events on the individual branch.
  std::vector<double> duplication_times;
  std::map<double, const TopologyNode *> branching_times_to_nodes;
  for ( std::set<const TopologyNode*>::iterator it = initial_genes.begin(); it != initial_genes.end(); ++it)
    {
      const TopologyNode *gene = (*it);
      if ( gene->isRoot() == false )
        {
          // Add the previous branching event on the gene tree.
          const TopologyNode &parent = gene->getParent();
          double parent_age = parent.getAge();
          branching_times_to_nodes[ parent_age ] = &parent;
        }
    }
  double current_time = individual_age;
  // Iteration ends at the previous coalescence of the individual branch (or
  // when there are no more branching events).
  while ( current_time < parent_individual_age && branching_times_to_nodes.size() > 0 )
    {
      // Maps are sorted by their key, so this will give us the next branching
      // event up on the gene tree (since at each iteration, we always erase the
      // first element).
      const TopologyNode *parent = branching_times_to_nodes.begin()->second;
      double parent_age = parent->getAge();
      current_time = parent_age;

      if ( parent_age < parent_individual_age )
        // Duplication in the individual tree branch.
        {
          const TopologyNode *left = &parent->getChild( 0 );
          const TopologyNode *right = &parent->getChild( 1 );
          if ( remaining_genes.find( left ) == remaining_genes.end() || remaining_genes.find( right ) == remaining_genes.end() )
            {
              // One of the daughter-genes does not belong to this individual tree branch.
              return RbConstants::Double::neginf;
            }

          // Remove the branching event and the daughter genes.
          branching_times_to_nodes.erase( branching_times_to_nodes.begin() );
          remaining_genes.erase( remaining_genes.find( left ) );
          remaining_genes.erase( remaining_genes.find( right ) );

          // Insert the parent into vector of genes in this branch.
          remaining_genes.insert( parent );
          // Since more than one duplication event can affect the same gene, we
          // have to check for further duplications up the gene branch.
          if ( parent->isRoot() == false )
            {
              const TopologyNode *grand_parent = &parent->getParent();
              branching_times_to_nodes[ grand_parent->getAge() ] = grand_parent;
            }
          if ( (fabs( parent_age - individual_age) > EPS_COAL) &&       // The branching event didn't happen close to the bottom of the individual branch (which would be a coalescence).
               (fabs( parent_age - parent_individual_age) > EPS_COAL) ) // Nor did it happen close to the top of the individual branch (which would also be a coalescence).
            {
              duplication_times.push_back( parent_age );
            }
        }
      // No more duplication in this branch of the individual tree. Jump out of
      // the while loop.
      else
        {
          break;
        }
    }

  if ( initial_genes.size() >= 1 )
    {
      ln_prob_dupl_loss += computeLnDuplicationLossProbability(initial_genes.size(), duplication_times, individual_age, parent_individual_age, individual_node, individual_node.isRoot());
    }

  // Merge the two sets of genes that go into the next individual.
  if ( ! individual_node.isRoot() )
    {
      // Do not clear the genes here, because both daughter branches will insert
      // genes and the other branch may already have inserted its genes. Rather,
      // add the remaining genes.
      std::set<const TopologyNode *> &genes_parent_branch_recent = genes_per_branch_recent[ individual_node.getParent().getIndex() ];
      genes_parent_branch_recent.insert( remaining_genes.begin(), remaining_genes.end());
    }

  std::set<const TopologyNode *> &genes_this_branch_ancient = genes_per_branch_ancient[ individual_node.getIndex() ];
  genes_this_branch_ancient.clear();
  genes_this_branch_ancient.insert( remaining_genes.begin(), remaining_genes.end());

  return ln_prob_dupl_loss;
}


void DuplicationLossProcess::redrawValue( void )
{

  size_t attempts = 0;

  if ( condition == "genes" )
    {

      while (attempts < ATTEMPTS)
        {
          bool success = false;

          success = simulateTreeRejectionSampling();

          if (success == true)
            {
              return;
            }
          ++attempts;
        }

      std::string err="After " + StringUtilities::to_string(ATTEMPTS);
      err += " attempts a duplication-loss tree could not be simulated.";
      throw RbException(err);
    }
  else if ( condition == "survival" )
    {

      while (attempts < ATTEMPTS)
        {
          bool success = false;

          simulateTree();

          success = value->getNumberOfTips() > 0 && value->getTipNode(0).isRoot() == false;

          if (success == true)
            {
              return;
            }
          ++attempts;
        }

      std::string err="After " + StringUtilities::to_string(ATTEMPTS);
      err += " attempts a duplication-loss tree could not be simulated.";
      throw RbException(err);
    }

  {
    simulateTree();
  }

}


void DuplicationLossProcess::resetTipAllocations( void )
{
  num_taxa = value->getNumberOfTips();

  // Check for a degenerate tree to avoid segmentation faults.
  if ( value->getRoot().getNumberOfChildren() == 0 )
    {
      return;
    }

  const Tree &ind_tree = individual_tree->getValue();
  const std::vector< TopologyNode* > &individual_tree_nodes = ind_tree.getNodes();

  // first let's create a map from individual names to the nodes of the individual tree
  std::map<std::string, TopologyNode * > individual_names_2_individual_nodes;
  for (std::vector< TopologyNode *>::const_iterator it = individual_tree_nodes.begin(); it != individual_tree_nodes.end(); ++it)
    {
      TopologyNode *this_ind_node = (*it);
      if ( this_ind_node->isTip() )
        {
          const std::string &ind_name = this_ind_node->getName();
          individual_names_2_individual_nodes[ind_name] = *it;
        }
    }

  // create a map for the individuals to branches
  genes_per_branch_recent  = std::vector< std::set< const TopologyNode* > >(ind_tree.getNumberOfNodes(), std::set< const TopologyNode* >() );
  genes_per_branch_ancient = std::vector< std::set< const TopologyNode* > >(ind_tree.getNumberOfNodes(), std::set< const TopologyNode* >() );
  for (size_t i=0; i<num_taxa; ++i)
    {
      const TopologyNode &n = value->getNode( i );
      const std::string &individual_name = n.getSpeciesName();
      TopologyNode *individual_node = individual_names_2_individual_nodes[individual_name];
      if ( individual_node == NULL )
        {
          throw RbException("There was an unexpected problem when inializing the tips of the gene tree in the duplication loss process. Could not find a node in the haplotype tree with name '" + individual_name + "'.");
        }
      size_t index = individual_node->getIndex();
      genes_per_branch_recent[ index ].insert( &n );
    }
}


/**
 * Set the current value.
 */
void DuplicationLossProcess::setValue(Tree *v, bool f )
{
  // delegate to super class
  TypedDistribution<Tree>::setValue(v, f);

  size_t n_tips = value->getNumberOfTips();
  size_t n_taxa = taxa.size();

  if ( n_tips != n_taxa )
    {
      throw RbException("The number of taxa in the tree and the taxon list need to be the same when setting the tree.");
    }

  for (size_t i=0; i<n_tips; ++i)
    {
      // let's get the i-th tip of the gene tree
      TopologyNode &n = value->getTipNode( i );
      const std::string &name =n.getName();

      bool found = false;
      for (size_t j=0; j<n_taxa; ++j)
        {
          // check if the name of the tip corresponds to this taxon of our taxon list
          if ( name == taxa[j].getName() )
            {
              n.setTaxon( taxa[j] );
              found = true;
              break;
            }
        }

      if ( found == false )
        {
          throw RbException("We could not find a taxon with name '" +name+ "' in our taxon list.");
        }
    }

  resetTipAllocations();
}



void DuplicationLossProcess::setDuplicationRate(TypedDagNode<RbVector<double> >* d)
{
  removeParameter( duplication_rate );
  removeParameter( duplication_rates );

  duplication_rate  = NULL;
  duplication_rates = d;

  addParameter( duplication_rates );
}


void DuplicationLossProcess::setDuplicationRate(TypedDagNode<double>* d)
{
  removeParameter( duplication_rate );
  removeParameter( duplication_rates );

  duplication_rate  = d;
  duplication_rates = NULL;

  addParameter( duplication_rate );
}


void DuplicationLossProcess::setLossRate(TypedDagNode<RbVector<double> >* l)
{
  removeParameter( loss_rate );
  removeParameter( loss_rates );

  loss_rate  = NULL;
  loss_rates = l;

  addParameter( loss_rates );
}


void DuplicationLossProcess::setLossRate(TypedDagNode<double>* l)
{
  removeParameter( loss_rate );
  removeParameter( loss_rates );

  loss_rate = l;
  loss_rates = NULL;

  addParameter( loss_rate );
}


void DuplicationLossProcess::setGeneSamplingProbability(TypedDagNode<RbVector<double> >* s)
{
  removeParameter( gene_sampling_probabilities );

  gene_sampling_probabilities = s;

  addParameter( gene_sampling_probabilities );
}

// age_begin :: Age at the top of the considered branch.
// i_node    :: Pointer to individual node at the bottom of the branch.
// genes     :: A vector of pointers to the gene nodes at the top of the branch.
void DuplicationLossProcess::recursivelySimulateTreeForward(double age_begin, const TopologyNode *i_node, std::vector<TopologyNode *> genes)
{
  RandomNumberGenerator* rng = GLOBAL_RNG;

  double age_end = i_node->getAge();
  // FIXME: make this work for haplotype-branch specific dup-loss rates
  const double la = duplication_rate->getValue();
  const double mu = loss_rate->getValue();
  const double ra = la + mu;

  // Handle branch.
  double current_age = age_begin;

  while (true) {
    size_t n_genes = genes.size();
    double dt = RbStatistics::Exponential::rv( n_genes*ra, *rng);
    current_age -= dt;
    if (current_age <= age_end) {
      // This may not be necessary.
      current_age = age_end;
      // Exit loop, not event happened before bottom of branch.
      break;
    }
    // Which gene is affected?
    size_t gene_index = static_cast<size_t>( floor(rng->uniform01()*n_genes ) );
    TopologyNode *gene = genes[gene_index];
    gene->setAge(current_age);
    genes.erase(genes.begin() + gene_index);
    // Which event?
    double u  = rng->uniform01();
    if (u < la/(la+mu)) {
      // A duplication.
      // Create children and attach them.
      TopologyNode *left = new TopologyNode();
      TopologyNode *right = new TopologyNode();
      gene->addChild(left);
      left->setParent(gene);
      genes.push_back(left);
      gene->addChild(right);
      right->setParent(gene);
      genes.push_back(right);
    }
    else {
      // An extinction. Gene has already been removed from vector.
    }
  }

  // Handle coalescence, which corresponds to a huge "duplication like" event
  // when going forward in time.
  if (! i_node->isTip()) {
    std::vector<TopologyNode *> genes_left;
    std::vector<TopologyNode *> genes_right;
    for (std::vector<TopologyNode *>::iterator it = genes.begin(); it != genes.end(); ++it) {
      TopologyNode *g_node = *it;
      double age = i_node->getAge();
      g_node->setAge(age);
      // Create children and attach them.
      TopologyNode *left = new TopologyNode();
      TopologyNode *right = new TopologyNode();
      g_node->addChild(left);
      left->setParent(g_node);
      genes_left.push_back(left);
      g_node->addChild(right);
      right->setParent(g_node);
      genes_right.push_back(right);
    }
    // Handle next coalescence.
    const TopologyNode *i_left = &(i_node->getChild(0));
    const TopologyNode *i_right = &(i_node->getChild(1));
    recursivelySimulateTreeForward(age_end, i_left, genes_left);
    recursivelySimulateTreeForward(age_end, i_right, genes_right);
  }
  // Handle tip.
  else {
    double age = i_node->getAge();
    // XXX: But what if individuals are sampled not at the present?
    if (age != 0.0) throw RbException("Uhh, go to your phone and call a revBayes maintainer; sorry ...");
    for (std::vector<TopologyNode *>::iterator it = genes.begin(); it != genes.end(); ++it) {
      TopologyNode *g_node = *it;
      g_node->setAge(age);
      g_node->setSpeciesName( i_node->getName() );
    }
    // Set the extant genes (genes_per_branch_recent) at extant branches.
    // std::vector< std::set< const TopologyNode* > > genes_per_branch_recent;
    //    genes_per_branch_recent[i_node->getIndex()].insert( genes.begin(), genes.end());
    return;
  }
  return;
}

// For a given individual tree, walk down from the root and simulate a birth and
// death process. Only if we end up with the correct amount of genes at each
// leaf, we keep the tree.

// Simulate a gene tree matching the haplotype tree and the required number of
// extant genes.

// FIXME: Of course, this is a naive method that will most likely take a loooong
// time. Can't we use the simulation method of reconstructed trees introduced by
// Tanja Stadler? No. Maybe?
bool DuplicationLossProcess::simulateTreeRejectionSampling(void)
{
  // FIXME: At least, we could reject the simulation already when reaching the
  // first tip in the haplotype tree where the number of genes mismatch.
  simulateTree();


  if ( value->getRoot().getNumberOfChildren() == 0 )
    {
      return false;
    }

  // Check that the number of genes in the simulated tree matches the number of genes specified in the taxon vector.
  const Tree &ind = individual_tree->getValue();

  size_t num_ind = ind.getNumberOfTips();

  // Check if the tree matches.
  // Assign gene taxa to individual taxa.
  for ( size_t i=0; i<num_ind; ++i )
    {
      const TopologyNode &ind_node = ind.getTipNode( i );
      const std::string &ind_name = ind_node.getName();

      size_t num_genes_for_ind = 0;

      // TODO: Write this more efficiently. Perhaps that can even be computed
      // once before all simulations. HINT: Use a map.
      for (std::vector< Taxon >::iterator it = taxa.begin(); it != taxa.end(); ++it)
        {
          const Taxon &n = *(it);
          if ( ind_name == n.getSpeciesName() )
            {
              ++num_genes_for_ind;
            }

        }

      if ( num_genes_for_ind != genes_per_branch_recent[ind_node.getIndex()].size() )
        {
          return false;
        }

    }


  return true;
}

// Simulate a gene tree matching the haplotype tree.
void DuplicationLossProcess::simulateTree( void )
{
  const Tree &ind = individual_tree->getValue();

  // Delete old tree, if found.
  if (value != NULL) {
    delete value;
  }

  // Test that the origin time is older than the root age of the haplotype
  // tree.
  double org_time = origin->getValue();
  double haplotype_root_age = individual_tree->getValue().getRoot().getAge();
  if ( org_time < haplotype_root_age )
    {
      std::string err = "Origin time " + StringUtilities::to_string(org_time);
      err += " lower than root age of haplotype tree " + StringUtilities::to_string(haplotype_root_age);
      throw RbException(err);
    }

  // The root of the tree.
  TopologyNode *root = new TopologyNode();

  // Traverse individual tree from top to bottom.
  std::vector< TopologyNode* > genes;
  genes.push_back(root);

  // Start the tree traversal.
  const TopologyNode *i_root = &ind.getRoot();
  recursivelySimulateTreeForward(org_time, i_root, genes);

  // Create the tree object.
  // The time tree object (topology + times).
  Tree *psi = new Tree();
  // Internally we treat unrooted topologies the same as rooted.
  psi->setRooted(true);
  // Initialize the gene topology by setting the root and reindex the tree.
  psi->setRoot(root, true);

  // Set length of root branch.
  double root_age = root->getAge();
  double root_branch_length = org_time - root_age;
  if (root_branch_length < 0) {
    throw RbException("Root length smaller than zero.");
  }
  root->setBranchLength(root_branch_length);

  // Remove extinct genes.
  std::vector<size_t> fossil_leaves_idxs;
  while ( (fossil_leaves_idxs = psi->Tree::getFossilLeavesIdxs()).size() > 0)
    {
      psi->dropTipNode( fossil_leaves_idxs[0] );
    }

  // We probably need to re-index the tip nodes here.
  psi->reindex();

  // Set the names of the tips.
  psi->setDefaultTipNames("Tip_",false);

  // Finally store the new tree.
  value = psi;

  resetTipAllocations();
}


/** Swap a parameter of the distribution */
void DuplicationLossProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{

  if ( oldP == individual_tree )
    {
      individual_tree = static_cast<const TypedDagNode< Tree >* >( newP );
    }

  if ( oldP == duplication_rate )
    {
      duplication_rate = static_cast<const TypedDagNode< double >* >( newP );
    }

  if ( oldP == duplication_rates )
    {
      duplication_rates = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }

  if ( oldP == loss_rate )
    {
      loss_rate = static_cast<const TypedDagNode< double >* >( newP );
    }

  if ( oldP == loss_rates )
    {
      loss_rates = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }

  if ( oldP == origin )
    {
      origin = static_cast<const TypedDagNode< double >* >( newP );
    }

  if ( oldP == gene_sampling_probabilities )
    {
      gene_sampling_probabilities = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }

}
