#include "IndependentTopologyProposal.h"
#include "RandomNumberGenerator.h"
#include "RandomNumberFactory.h"
#include "Move.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"

using namespace RevBayesCore;

IndependentTopologyProposal::IndependentTopologyProposal( StochasticNode<Tree> *v, TypedDistribution<Tree>* dist, Clade og ) : Proposal(),
    outgroup( og ),
    proposal_distribution( dist ),
    variable( v ),
    stored_ln_num_rankings( 0.0 )
{
    
    addNode( variable );

    outgroup.resetTaxonBitset( variable->getValue().getTaxonBitSetMap() );

    recursivelyRank( variable->getValue().getRoot(), stored_ln_num_rankings );

}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
IndependentTopologyProposal* IndependentTopologyProposal::clone( void ) const
{
    
    return new IndependentTopologyProposal( *this );
}


/** Clean the proposal */
void IndependentTopologyProposal::cleanProposal( void )
{
    stored_ln_num_rankings = proposal_ln_num_rankings;
    // nothing to do
}

const std::string& IndependentTopologyProposal::getProposalName( void ) const
{
    static std::string name = "IndependentTopologyMove";
    
    return name;
}



/** Perform the move */
double IndependentTopologyProposal::doProposal( void )
{
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    stored_tree = variable->getValue();

    // draw a new tree
    proposal_distribution->redrawValue();

    Tree proposal_tree = proposal_distribution->getValue();

    // if necessary, root the new tree topology
    if( proposal_tree.isRooted() == false )
    {
        TopologyNode *node;

        // get the outgroup node
        if( outgroup.size() > 0 )
        {
            proposal_tree.reroot( outgroup, true );
            node = proposal_tree.getRoot().getNode( outgroup, true );
        }
        // get a non-root node
        else
        {
            node = &proposal_tree.getNode( size_t( rng->uniform01() * (proposal_tree.getNumberOfNodes() - 1) ) );
            proposal_tree.reroot(*node, true);
        }

        TopologyNode& parent = node->getParent();

        // create a new root
        TopologyNode* root = new TopologyNode( );

        // attach the root between the node and its parent
        parent.removeChild(node);
        parent.addChild( root );
        root->setParent(&parent);

        root->addChild(node);
        node->setParent(root);

        // reroot the tree
        proposal_tree.reroot(*node, true);
        proposal_tree.setRooted(true);

        // restore tip indices
        for (size_t i=0; i<proposal_tree.getNumberOfTips(); i++)
        {
            proposal_tree.getTipNodeWithName(variable->getValue().getTipNode(i).getName()).setIndex(i);
        }
        proposal_tree.orderNodesByIndex();
    }

    // get the sorted node ages
    const std::vector<TopologyNode*>& stored_nodes = variable->getValue().getNodes();

    std::vector<double> ages;
    for(size_t i = 0; i < stored_nodes.size(); i++)
    {
        if( stored_nodes[i]->isInternal() == true && stored_nodes[i]->isSampledAncestor() == false )
        {
            ages.push_back( stored_nodes[i]->getAge() );
        }
    }
    std::sort(ages.begin(), ages.end());


    // choose a new ranking uniformly at random
    proposal_ln_num_rankings = 0;
    std::vector<size_t> ranking = recursivelyRank( proposal_tree.getRoot(), proposal_ln_num_rankings );

    // update the ranked node ages
    const std::vector<TopologyNode*>& proposal_nodes = proposal_tree.getNodes();
    for(size_t i = 0; i < ranking.size(); i++)
    {
        proposal_nodes[ranking[i]]->setAge( ages[i] );
    }

    // set the tip ages
    for(size_t i = 0; i < proposal_tree.getNumberOfTips(); i++)
    {
        proposal_nodes[i]->setAge( variable->getValue().getTipNode(i).getAge() );
    }

    variable->setValue( proposal_tree.clone() );

    return  proposal_ln_num_rankings - stored_ln_num_rankings;
}


/** Prepare the proposal */
void IndependentTopologyProposal::prepareProposal( void )
{
    
    // nothing to do
}



/** Print the parameter summary */
void IndependentTopologyProposal::printParameterSummary(std::ostream &o) const
{
    
    // nothing to do
}


/** Recursively choose a random subtree node ranking */
std::vector<size_t> IndependentTopologyProposal::recursivelyRank( const TopologyNode& n, double& ln_num_rankings )
{
    RandomNumberGenerator* rng = GLOBAL_RNG;

    std::vector<size_t> ranking;

    for(size_t i = 0; i < n.getNumberOfChildren(); i++)
    {
        if( n.getChild(i).isTip() )
        {
            continue;
        }

        // get the child ranking
        std::vector<size_t> child_ranking = recursivelyRank( n.getChild(i), ln_num_rankings );

        // choose new ranks for the child nodes
        std::vector<size_t> pos;
        for(size_t j = 0; j < child_ranking.size(); j++)
        {
            size_t p = rng->uniform01()*(ranking.size() + 1.0);
            pos.push_back(p);
        }

        std::sort(pos.begin(), pos.end());

        // update the ranking
        for(size_t j = 0; j < pos.size() ; j++)
        {
            ranking.insert(ranking.begin() + pos[j], child_ranking[j]);

            for(size_t k = j+1; k < pos.size(); k++)
            {
                pos[k]++;
            }
        }

        // update the number of rankings
        ln_num_rankings += RbMath::lnChoose(ranking.size(), child_ranking.size());
    }

    if( n.isSampledAncestor() == false )
    {
        // put the current node at the highest rank
        ranking.push_back( n.getIndex() );
    }

    return ranking;
}



void IndependentTopologyProposal::undoProposal( void )
{
    
    // reset to the old tree
    variable->setValue( stored_tree.clone() );
    
}


/** Tune the proposal */
void IndependentTopologyProposal::tune(double r)
{
    
    // nothing to do
}


void IndependentTopologyProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    variable = static_cast<StochasticNode<Tree>* >( newN );
    
}

