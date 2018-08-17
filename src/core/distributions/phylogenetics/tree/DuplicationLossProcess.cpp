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

DuplicationLossProcess::DuplicationLossProcess(const TypedDagNode<Tree> *it, const std::vector<Taxon> &t) : TypedDistribution<Tree>( NULL ),
    taxa(t),
    individual_tree( it ),
    num_taxa( taxa.size() ),
    log_tree_topology_prob (0.0)
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( individual_tree );

    double ln_fact = RbMath::lnFactorial((int)(num_taxa));

    log_tree_topology_prob = (num_taxa - 1) * RbConstants::LN2 - 2.0 * ln_fact - std::log( num_taxa ) ;

    redrawValue();
}


DuplicationLossProcess::~DuplicationLossProcess()
{

}

void DuplicationLossProcess::attachTimes(Tree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> &times)
{

    if (index < num_taxa-1)
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;

        // randomly draw one node from the list of tips
        size_t tip_index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );

        // get the node from the list
        TopologyNode* parent = tips.at(tip_index);
        psi->getNode( parent->getIndex() ).setAge( times[num_taxa - index - 2] );

        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+tip_index);

        // add a left child
        TopologyNode* leftChild = &parent->getChild(0);
        if ( !leftChild->isTip() )
        {
            tips.push_back(leftChild);
        }

        // add a right child
        TopologyNode* rightChild = &parent->getChild(1);
        if ( !rightChild->isTip() )
        {
            tips.push_back(rightChild);
        }

        // recursive call to this function
        attachTimes(psi, tips, index+1, times);
    }
}


void DuplicationLossProcess::buildRandomBinaryTree(std::vector<TopologyNode*> &tips)
{
    if (tips.size() < num_taxa)
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;

        // randomly draw one node from the list of tips
        size_t index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );

        // get the node from the list
        TopologyNode* parent = tips.at(index);

        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+index);

        // add a left child
        TopologyNode* leftChild = new TopologyNode(0);
        parent->addChild(leftChild);
        leftChild->setParent(parent);
        tips.push_back(leftChild);

        // add a right child
        TopologyNode* rightChild = new TopologyNode(0);
        parent->addChild(rightChild);
        rightChild->setParent(parent);
        tips.push_back(rightChild);

        // recursive call to this function
        buildRandomBinaryTree(tips);
    }
}


DuplicationLossProcess* DuplicationLossProcess::clone(void) const
{
    return new DuplicationLossProcess(*this);
}

double DuplicationLossProcess::computeLnProbability( void )
{
    resetTipAllocations();

    // variable declarations and initialization
    double ln_prob_coal = 0;

    const Tree &it = individual_tree->getValue();

    ln_prob_coal = recursivelyComputeLnProbability( it.getRoot() );


    return ln_prob_coal; // + logTreeTopologyProb;
}

double DuplicationLossProcess::computeLnDuplicationLossProbability(size_t num_genes_recent, const std::vector<double> &dupl_ages, double age_recent, double age_ancient, const TopologyNode &node_individual, bool is_root)
{
  double ln_prob = 0.0;

  size_t index_individual = node_individual.getIndex();
  double dupl_rate = duplication_rate->getValue();

  double current_age = age_recent;
  double current_ext_prob = 0.0;

  // Get extinction probability at the bottom of this branch.
  if ( node_individual.isTip() )
    {
      current_ext_prob = 1.0 - gene_sampling_probability->getValue()[index_individual];
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
    throw std::length_error("There are more duplication events than branches on this branch of the individual tree.");

  // Handle duplications.
  for ( size_t i=0; i<dupl_ages.size(); ++i )
    {
      double this_dupl_age = dupl_ages[i];
      double dt = this_dupl_age - current_age;

      // Walk up dt to the next duplication on the branch of the individual tree.
      // It contains 'N = num_genes_recent-i' genes.
      ln_prob += (num_genes_recent-i) * log( computeD(dt, current_ext_prob) );
      // Have a duplication.
      ln_prob += log( dupl_rate );
      // Combinatorial factor. The duplication can happen on any of the 'N-1'
      // branches existing earlier (i.e., before the duplication happens when
      // going forwards in time), but we require it to happen on a specific
      // branch.
      ln_prob += log( num_genes_recent - i - 1 );

      current_age = this_dupl_age;
      current_ext_prob = computeE( dt, current_ext_prob );
    }

  // Final branch segment before coalescence on individual tree, i.e., there are
  // no more duplications.
  if (is_root)
    {
      // What happens when we are at the root? dt=inf -> D=0 -> log(D)=-inf; certainly not what we want.
      // throw RbException("At root, should we handle this in a special way?");
      // Assume that we are done.
      // TODO @Sebastian. Is this OK?
      return ln_prob;
    }
  else
    {
      double dt = age_ancient - current_age;
      ln_prob += (num_genes_recent-dupl_ages.size()) * log( computeD(dt, current_ext_prob) );
      extinction_probs[index_individual] = computeE( dt, current_ext_prob );
      return ln_prob;
    }
}

double DuplicationLossProcess::computeD(double dt, double p_e)
{
  const double la = duplication_rate->getValue();
  const double mu = loss_rate->getValue();

  //////////////////////////////
  // Discrepancies between formulas from
  // 1. Sebastian
  // 2. Wandrille
  // 3. Eq (1) and (2) in Stadler, T. (2010). Sampling-through-time in birth-death trees. Journal of Theoretical Biology, 267(3), 396–404. http://doi.org/10.1016/j.jtbi.2010.09.010
  // 4. Eq (1) and (2) in Stadler, T. (2011). Mammalian phylogeny reveals recent diversification rate shifts. Proceedings of the National Academy of Sciences, 108(15), 6187–6192. http://doi.org/10.1073/pnas.1016876108
  // Total confusion :-).
  // I USE METHOD 4!

  // I think a big part of the confusion is the name of the parameters as well
  // as variables. For instance, the time can be measured on the branch only
  // (0 at the bottom closest to present), or from the bottom of the phylogny
  // (0 means the present).

  //////////////////////////////
  // Formula from Sebastian. This formula is SIMILAR BUT NOT THE SAME to the
  // one below. Notably, there is a difference in B (+mu instead of -mu, so
  // that 'B = -c' from below!). Also, an 'G0 = 1- E0' is missing in the
  // nominator and E0 is used instead of G0 in the calculation of B. Is this
  // an error, or did we copy the wrong formulas? However, this formula fits
  // apart from a missing G[0] in the nominator with the one from Wandrille
  // (see mail from Gergely 2018-05-16 "probabilities for birth death on a
  // tree").

  // double A = la - mu;
  // double B = ( (1.0 - 2*e)*la+mu ) / A;
  // double et = exp(-A * dt);
  // double D = 4.0 * et;
  // D /= (1.0+B+et*(1-B));
  // D /= (1.0+B+et*(1-B));
  // return D;

  //////////////////////////////
  // Eq. (2) with psi=0 in Stadler, T. (2010). Sampling-through-time in
  // birth-death trees. Journal of Theoretical Biology, 267(3), 396–404.
  // http://doi.org/10.1016/j.jtbi.2010.09.010
  // double G0  = 1 - E0;
  // double d  = la - mu;
  // double c  = ( (1.0 - 2*G0)*la - mu ) / d;
  // double et = exp(-d * dt);
  // double G  = 4.0 * G0 * et;
  // double dn = ( (1.0 + c) + (1 - c)*et );
  // G /= dn;
  // G /= dn;
  // return G;

  //////////////////////////////
  // Eq (2) from PNAS paper of Stadler.
  // e       :: Index of this branch.
  // t       :: Time on phylogeny (0 at present, increases into the past).
  // g(t, e) :: Probability density that the species corresponding to edge e at time t evolved between t and the present as observed in the phylogeny.
  // dt      :: Time on the branch (dt=0 means bottom of branch).
  // p_e     :: Probability of going extinct between the bottom of branch e and the present. Also called E(0) sometimes.
  // It turns out that g(t,e) only depends on dt and on p_e, and not on the actual time t.

  // Technically, we also need to know g(t, f) and g(t, h), where f and h are the daughter branches of branch e. See the variable d_{i,e} in Stadlers paper.
  // However, since this is only a multiplicative factor, we can also apply it when calculating the likelihood. See 'DuplicationLossProcess::computeLnDuplicationLossProbability'.
  // double d_ie        = 1.0;
  double d           = la - mu;
  double et          = exp(-d * dt);
  // double nominator   = d_ie * d * d *et;
  double nominator   = d * d *et;
  double denominator = la*(p_e - 1.0) + (mu - p_e*la)*et;
  double g_te        = nominator / denominator / denominator;
  return g_te;
}

double DuplicationLossProcess::computeE(double dt, double p_e)
{
  const double la = duplication_rate->getValue();
  const double mu = loss_rate->getValue();

  // See discussion at 'DuplicationLossProcess::computeD'.

  //////////////////////////////
  // Formula from Sebastian. Again, this is SIMILAR BUT NOT THE SAME to the
  // one below. Possible sign error before the large fraction in the
  // nominator. I am not sure what's the correct formula.

  // double A = la - mu;
  // double B = ( (1.0 - 2*e)*la+mu ) / A;
  // double et = exp(A * dt);
  // double E = la + mu - A *(1.0+B-et*(1.0-B))/(1.0+B+et*(1.0-B));
  // E /= (2*la);

  //////////////////////////////
  // Eq. (1) with psi=0 in Stadler, T. (2010). Sampling-through-time in
  // birth-death trees. Journal of Theoretical Biology, 267(3), 396–404.
  // http://doi.org/10.1016/j.jtbi.2010.09.010
  // double G0  = 1 - E0;
  // double d  = la - mu;
  // double c  = ( (1.0 - 2*G0)*la - mu ) / d;
  // double et = exp(-d * dt);
  // double nom = ( et*(1.0-c) - (1.0+c) ) / ( et*(1.0-c) + (1.0+c) );
  // double E = la + mu + d*nom;
  // E /= (2*la);
  // return E;

  //////////////////////////////
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
  // 1. Handle node.

  if ( ! individual_node.isTip() )
    {
      // The genes at inner nodes (genes_per_branch_recent) will be set when
      // traversing the children (using their respective
      // genes_per_branch_ancient). At the leaves, the genes have to be set a
      // priori, e.g., during the simulation.
      genes_per_branch_recent[ individual_node.getIndex() ].clear();
      // Add likelihood from children.
      for (size_t i=0; i<individual_node.getNumberOfChildren(); ++i)
          ln_prob_dupl_loss += recursivelyComputeLnProbability( individual_node.getChild(i) );
      // Handle coalescence of individuals on gene tree. I.e., if genes are
      // merging at the same time, consider this event being part of the
      // coalescence. If, on the other hand, they do not join, a loss must have
      // happened (and also a duplication at a previous time point). Here we
      // assume that the tree BIFURCATING.
      size_t left_index  = individual_node.getChild(0).getIndex();
      size_t right_index = individual_node.getChild(1).getIndex();
      std::set< const TopologyNode* > &genes_for_this_individual = genes_per_branch_recent[ individual_node.getIndex() ];
      const std::set< const TopologyNode* > &genes_for_left_descendant  = genes_per_branch_ancient[ left_index ];
      for (std::set<const TopologyNode*>::iterator it=genes_for_this_individual.begin(); it!=genes_for_this_individual.end(); ++it)
        {
          const TopologyNode *this_gene_node = *it;
          double this_age = this_gene_node->getParent().getAge();
          if ( fabs( this_age - individual_age) < EPS_COAL )
            {
              // Coalescent event; likelihood unchanged.
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
    }

  //////////////////////////////////////////////////
  // 2. Handle branch attached to node.

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
      if ( ! gene->isRoot() )
        // Add the previous branching event on the gene tree.
        {
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
      // Maps are sorted by their key, so this will give as the next branching
      // event up on the gene tree.
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
          // // TODO @Dominik @Sebastian. Why do we bother about the grandparent here?
          //  if ( ! parent->isRoot() )
          //    {
          //      const TopologyNode *grand_parent = &parent->getParent();
          //      branching_times_to_nodes[ grand_parent->getAge() ] = grand_parent;
          //    }
          if ( (fabs( parent_age - individual_age) > EPS_COAL) &&       // The branching event didn't happen close to the bottom of the individual branch (which would be a coalescence).
               (fabs( parent_age - parent_individual_age) > EPS_COAL) ) // Nor did it happen close to the top of the individual branch (which would also be a coalescence).
            {
              duplication_times.push_back( parent_age );
            }
        }
      // No more duplication in this branch of the individual tree. Jump out of
      // the while loop.
      else
          break;
    }

  if ( initial_genes.size() >= 1 )
      ln_prob_dupl_loss += computeLnDuplicationLossProbability(initial_genes.size(), duplication_times, individual_age, parent_individual_age, individual_node.getIndex(), individual_node.isRoot());

  // Merge the two sets of genes that go into the next individual.
  if ( ! individual_node.isRoot() )
    {
      std::set<const TopologyNode *> &genes_parent_branch_recent = genes_per_branch_recent[ individual_node.getParent().getIndex() ];
      // Do not clear the genes here, because both daughter branches will insert genes and the other branch may already have inserted its genes.
      genes_parent_branch_recent.insert( remaining_genes.begin(), remaining_genes.end());
    }

  std::set<const TopologyNode *> &genes_this_branch_ancient = genes_per_branch_ancient[ individual_node.getIndex() ];
  genes_this_branch_ancient.clear();
  genes_this_branch_ancient.insert( remaining_genes.begin(), remaining_genes.end());

  return ln_prob_dupl_loss;
}


void DuplicationLossProcess::redrawValue( void ) {
  simulateTree();
}


void DuplicationLossProcess::resetTipAllocations( void )
{
    const Tree &ind_tree = individual_tree->getValue();
    const std::vector< TopologyNode* > &individual_tree_nodes = ind_tree.getNodes();

    // first let's create a map from individual names to the nodes of the individual tree
    std::map<std::string, TopologyNode * > individual_names_2_individual_nodes;
    for (std::vector< TopologyNode *>::const_iterator it = individual_tree_nodes.begin(); it != individual_tree_nodes.end(); ++it)
    {
        TopologyNode *this_node = (*it);
        if ( this_node->isTip() )
        {
            const std::string &name = this_node->getName();
            individual_names_2_individual_nodes[name] = *it;
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
        genes_per_branch_recent[ individual_node->getIndex() ].insert( &n );
    }
}


/**
 * Set the current value.
 */
void DuplicationLossProcess::setValue(Tree *v, bool f )
{
    // delegate to super class
    TypedDistribution<Tree>::setValue(v, f);

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
    removeParameter( gene_sampling_probability );

    gene_sampling_probability = s;

    addParameter( gene_sampling_probability );
}

// age_begin :: Age at the top of the considered branch.
// i_node    :: Pointer to individual node at the bottom of the branch.
// genes     :: A vector of pointers to the gene nodes at the top of the branch.
void DuplicationLossProcess::recursivelySimulateTreeForward(double age_begin, const TopologyNode *i_node, std::vector<TopologyNode *> genes) {
  RandomNumberGenerator* rng = GLOBAL_RNG;
  double age_end = i_node->getAge();
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

  if (! i_node->isTip()) {
    // Handle coalescence.
    std::vector<TopologyNode *> genes_left;
    std::vector<TopologyNode *> genes_right;
    for (std::vector<TopologyNode *>::iterator it = genes.begin(); it != genes.end(); it++) {
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
  else {
    // Handle tip.
    double age = i_node->getAge();
    if (age != 0.0) throw RbException("Uhh, call a maintainer.");
    for (std::vector<TopologyNode *>::iterator it = genes.begin(); it != genes.end(); it++) {
      TopologyNode *g_node = *it;
      g_node->setAge(age);
    }
    // Set the extant genes (genes_per_branch_recent) at extant branches.
    // std::vector< std::set< const TopologyNode* > > genes_per_branch_recent;
    genes_per_branch_recent[i_node->getIndex()].insert( genes.begin(), genes.end());
    return;
  }
  return;
}

// For a given individual tree, walk down from the root and simulate a birth and
// death process. Only if we end up with the correct amount of genes at each
// leaf, we keep the tree. Of course, this is a naive method that will most
// likely take a looong time.
void DuplicationLossProcess::simulateTreeRejectionSampling(void)
{
  const Tree &ind = individual_tree->getValue();

  // A map from an individual node to the genes that are present at that node.
  std::map< const TopologyNode *, std::vector< TopologyNode* > > genes_per_branch;

  // The time tree object (topology + times).
  Tree *psi = NULL;

  // TODO: Set stop condition (we get a valid tree). For now, we take any tree.
  while (true) {
    // Delete old tree, if found.
    if (psi != NULL) {
      delete psi;
    }

    // The root of the tree.
    TopologyNode *root = new TopologyNode();

    // Traverse individual tree from top to bottom.
    std::vector< TopologyNode* > genes;
    genes.push_back(root);

    // TODO: Here, we assume that the gene tree also stops at the root of the
    // individual tree. I.e., we impose that one gene was present at the MRCA of
    // all individuals. Of course, this may be not the case. How do we determine
    // where to start our forward process? Should we draw the time from a
    // uniform distribution until infinity (which is not really possible), like
    // it is done in the papers of Tanja Stadler?

    // Start the tree traversal.
    const TopologyNode *i_root = &ind.getRoot();
    double root_age = i_root->getAge();
    root->setAge(root_age);
    recursivelySimulateTreeForward(root_age, i_root, genes);

    // Create the tree object.
    psi = new Tree();
    // Internally we treat unrooted topologies the same as rooted.
    psi->setRooted(true);
    // Initialize the gene topology by setting the root and reindex the tree.
    psi->setRoot(root, true);

    // Remove extinct genes.
    std::vector<size_t> fossil_leaves_idxs;
    while ( (fossil_leaves_idxs = psi->Tree::getFossilLeavesIdxs()).size() > 0) {
      for (std::vector<size_t>::iterator it=fossil_leaves_idxs.begin(); it != fossil_leaves_idxs.end(); ++it)
        psi->dropTipNode(*it);
    }

    // // TODO.
    // // Check if the tree matches.
    // // Assign gene taxa to individual taxa.
    // for (std::vector< Taxon >::iterator it = taxa.begin(); it != taxa.end(); ++it)
    //   {
    //     const std::string &individual_name = n->getIndividualName();

    //     if ( individual_name == "" )
    //       {
    //         throw RbException("Cannot match a taxon without individual to a tip in the individual tree. The taxon map is probably wrong.");
    //       }

    //     TopologyNode *individual_node = individual_names_to_nodes[individual_name];

    //     if ( individual_node == NULL )
    //       {
    //         throw RbException("Could not match a taxon with name" + individual_name + " to any of the tips in the individual tree.");
    //       }

    //     n->setAge( 0.0 );
    //     std::vector< TopologyNode * > &nodes_at_node = genes_per_branch[ individual_node ];
    //     nodes_at_node.push_back( n );
    //   }

  }

  /*   for (std::map<std::string,std::string>::iterator it = gene2species.begin(); it != gene2species.end(); ++it)
       {
       const std::string &tipName = it->first;
       TopologyNode *n = new TopologyNode( tipName );
       const std::string &speciesName = it->second;
       TopologyNode *speciesNode = speciesNames2Nodes[speciesName];
       std::vector< TopologyNode * > &nodesAtNode = individualsPerBranch[ speciesNode ];
       nodesAtNode.push_back( n );
       }
  */

  // std::map<TopologyNode *, double> nodes_to_ages;
  // // we loop over the nodes of the individual tree in phylogenetic traversal
  // for (std::vector<TopologyNode *>::const_iterator it = individual_tree_nodes.begin(); it != individual_tree_nodes.end(); ++it)
  //   {
  //     TopologyNode *ind_node = *it;
  //     const TopologyNode *ind_parent_node = NULL;
  //     double branch_length = RbConstants::Double::inf;
  //     if ( ind_node->isRoot() == false )
  //       {
  //         ind_parent_node = &ind_node->getParent();
  //         branch_length = ind_parent_node->getAge() - ind_node->getAge();
  //       }

  //     std::vector<TopologyNode*> initial_individuals_at_branch = genes_per_branch[ind_node];
  //     double branch_ne = 1.0;

  //     double theta = 1.0 / branch_ne;

  //     double prev_coalescent_time = 0.0;

  //     size_t j = initial_individuals_at_branch.size();
  //     double n_pairs = j * (j-1) / 2.0;
  //     double lambda = n_pairs * theta;
  //     double u = RbStatistics::Exponential::rv( lambda, *rng);
  //     double next_coalescent_time = prev_coalescent_time + u;

  //     while ( next_coalescent_time < branch_length && j > 1 )
  //       {
  //         // randomly coalesce two lineages
  //         size_t index = static_cast<size_t>( floor(rng->uniform01()*initial_individuals_at_branch.size()) );
  //         TopologyNode *left = initial_individuals_at_branch[index];
  //         initial_individuals_at_branch.erase( initial_individuals_at_branch.begin() + index);

  //         index = static_cast<size_t>( floor(rng->uniform01()*initial_individuals_at_branch.size()) );
  //         TopologyNode *right = initial_individuals_at_branch[index];
  //         initial_individuals_at_branch.erase( initial_individuals_at_branch.begin() + index);

  //         TopologyNode *new_parent = new TopologyNode();
  //         new_parent->addChild(left);
  //         left->setParent(new_parent);
  //         new_parent->addChild(right);
  //         right->setParent(new_parent);

  //         root = new_parent;

  //         if ( root == NULL )
  //           {
  //             std::cerr << "Oh, the root is NULL :(" << std::endl;
  //           }

  //         initial_individuals_at_branch.push_back( new_parent );

  //         nodes_2_ages[new_parent] = next_coalescent_time + sp_node->getAge();


  //         prev_coalescent_time = next_coalescent_time;
  //         j--;
  //         n_pairs = j * (j-1) / 2.0;
  //         lambda = n_pairs * theta ;
  //         u = RbStatistics::Exponential::rv( lambda, *rng);
  //         next_coalescent_time = prev_coalescent_time + u;
  //       }

  //     if ( sp_parent_node != NULL )
  //       {
  //         std::vector<TopologyNode *> &incoming_lineages = individuals_per_branch[sp_parent_node];
  //         incoming_lineages.insert(incoming_lineages.end(), initial_individuals_at_branch.begin(), initial_individuals_at_branch.end());
  //       }


  //   }


  // for ( std::map<TopologyNode*, double>::iterator it = nodes_2_ages.begin(); it != nodes_2_ages.end(); ++it)
  //   {
  //     TopologyNode *node = it->first;
  //     node->setAge( it->second );
  //   }

  // Finally store the new tree.
  value = psi;
  resetTipAllocations();
  return;
}

void DuplicationLossProcess::simulateTree( void )
{
  simulateTreeRejectionSampling();
  return;
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

    if ( oldP == gene_sampling_probability )
    {
        gene_sampling_probability = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
}
