#include "AbstractRootedTreeDistribution.h"
#include "Clade.h"
#include "TopologyConstrainedTreeDistribution.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "StochasticNode.h"
#include "Taxon.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;


/**
 * Constructor.
 *
 * The constructor connects the parameters of the birth-death process (DAG structure)
 * and initializes the probability density by computing the combinatorial constant of the tree structure.
 *
 * \param[in]    c         Clade constraints.
 */
TopologyConstrainedTreeDistribution::TopologyConstrainedTreeDistribution(TypedDistribution<Tree>* base_dist, const std::vector<Clade> &c, const TypedDagNode<RbVector<Tree> >* bb) : TypedDistribution<Tree>( NULL ),
//    active_backbone_clades( base_dist->getValue().getNumberOfInteriorNodes(), RbBitSet() ),
    active_clades( base_dist->getValue().getNumberOfInteriorNodes(), RbBitSet() ),
//    backbone_mask( base_dist->getValue().getNumberOfTips() ),
    backbone_topologies( bb ),
    base_distribution( base_dist ),
    dirty_nodes( base_dist->getValue().getNumberOfNodes(), true ),
    monophyly_constraints( c )
{
    AbstractRootedTreeDistribution* tree_base_distribution = dynamic_cast<AbstractRootedTreeDistribution*>(base_distribution);
    if(tree_base_distribution == NULL)
    {
        throw(RbException("Can only constrain tree distributions of type AbstractRootedTreeDistribution"));
    }

    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    
    // add the backbone topology
    this->addParameter( backbone_topologies );
    num_backbones = backbone_topologies->getValue().size();
    for (size_t i = 0; i < num_backbones; i++) {
        std::vector<RbBitSet>v( base_dist->getValue().getNumberOfInteriorNodes(), RbBitSet() );
        active_backbone_clades.push_back(v);
    }
    backbone_mask = std::vector<RbBitSet>( num_backbones, base_dist->getValue().getNumberOfInteriorNodes() );

    // add the parameters of the distribution
    const std::vector<const DagNode*>& pars = base_distribution->getParameters();
    for (std::vector<const DagNode*>::const_iterator it = pars.begin(); it != pars.end(); ++it)
    {
        this->addParameter( *it );
    }
    
    value = &base_distribution->getValue();

    initializeBitSets();
    redrawValue();
}


/**
 * Copy Constructor.
 *
 * The constructor connects the parameters of the birth-death process (DAG structure)
 * and initializes the probability density by computing the combinatorial constant of the tree structure.
 *
 * \param[in]    c         Clade constraints.
 */
TopologyConstrainedTreeDistribution::TopologyConstrainedTreeDistribution(const TopologyConstrainedTreeDistribution &d) : TypedDistribution<Tree>( d ),
    active_backbone_clades( d.active_backbone_clades ),
    active_clades( d.active_clades ),
    backbone_constraints( d.backbone_constraints ),
    backbone_mask( d.backbone_mask ),
    backbone_topologies( d.backbone_topologies ),
    base_distribution( d.base_distribution->clone() ),
    dirty_nodes( d.dirty_nodes ),
    monophyly_constraints( d.monophyly_constraints ),
    stored_backbone_clades( d.stored_backbone_clades ),
    stored_clades( d.stored_clades ),
    num_backbones( d.num_backbones )
{
    // the copy constructor of the TypedDistribution creates a new copy of the value
    // however, here we want to hold exactly the same value as the base-distribution
    // thus, we delete the newly created value
    value->getTreeChangeEventHandler().removeListener( this );
    delete value;
    
    // and then set it to the value of the base distribution
    value = &base_distribution->getValue();

    value->getTreeChangeEventHandler().addListener( this );
}



TopologyConstrainedTreeDistribution::~TopologyConstrainedTreeDistribution()
{
    
    delete base_distribution;
    
    //value->getTreeChangeEventHandler().removeListener( this );

    // DO NOT DELETE THE VALUE
    // the base distribution is the actual owner of the value!!!
    // we simply avoid the deletion of the value by setting its pointer to NULL
    // our base class, the TypedDistribution thinks that it owns the value and thus deletes it
    value = NULL;
    
}


TopologyConstrainedTreeDistribution* TopologyConstrainedTreeDistribution::clone( void ) const
{
    
    return new TopologyConstrainedTreeDistribution( *this );
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double TopologyConstrainedTreeDistribution::computeLnProbability( void )
{
    recursivelyUpdateClades( value->getRoot() );
    
    // first check if the current tree matches the clade constraints
    if ( !matchesConstraints() )
    {
        return RbConstants::Double::neginf;
    }
    
    if ( !matchesBackbone() )
    {
        return RbConstants::Double::neginf;
    }
    
    double lnProb = base_distribution->computeLnProbability();
    
    return lnProb;
}


void TopologyConstrainedTreeDistribution::initializeBitSets(void)
{
    // fill the monophyly constraints bitsets
    for (size_t i = 0; i < monophyly_constraints.size(); i++)
    {
        RbBitSet b( value->getNumberOfTips() );
        for (size_t j = 0; j < monophyly_constraints[i].size(); j++)
        {
            const std::map<std::string, size_t> &taxon_map = value->getTaxonBitSetMap();
            const std::string &name = monophyly_constraints[i].getTaxonName(j);
            std::map<std::string, size_t>::const_iterator it = taxon_map.find( name );
            size_t k = it->second;

            b.set(k);
        }

        monophyly_constraints[i].setBitRepresentation( b );
    }

    // reset the backbone constraints and mask
    backbone_constraints.clear();
    backbone_mask.clear();
    backbone_constraints.resize(num_backbones);
    backbone_mask.resize( num_backbones );
    
    // add the backbone constraints
    if ( backbone_topologies != NULL )
    {
        for (size_t i = 0; i < num_backbones; i++) {
            backbone_mask[i] = RbBitSet( value->getNumberOfTips() );
            backbone_mask[i] |= recursivelyAddBackboneConstraints( backbone_topologies->getValue()[i].getRoot(), i );
        }
    }
//    std::sort( backbone_constraints.begin(), backbone_constraints.end() );
//    backbone_constraints.erase( std::unique( backbone_constraints.begin(), backbone_constraints.end() ), backbone_constraints.end() );
}


void TopologyConstrainedTreeDistribution::fireTreeChangeEvent(const TopologyNode &n, const unsigned& m)
{
    if (m == TreeChangeEventMessage::DEFAULT || m == TreeChangeEventMessage::TOPOLOGY)
    {

        recursivelyFlagNodesDirty(n);
    }
}


/**
 * We check here if all the constraints are satisfied.
 * These are hard constraints, that is, the clades must be monophyletic.
 *
 * \return     True if the constraints are matched, false otherwise.
 */
bool TopologyConstrainedTreeDistribution::matchesBackbone( void )
{

    // ensure that each backbone constraint is found in the corresponding active_backbone_clades
    for (size_t i = 0; i < num_backbones; i++)
    {
        for (size_t j = 0; j < backbone_constraints[i].size(); j++)
        {
            std::vector<RbBitSet>::iterator it = std::find(active_backbone_clades[i].begin(), active_backbone_clades[i].end(), backbone_constraints[i][j] );
            
            // the search fails if the backbone constraint is not found in the active_backbone_clades vector
            if (it == active_backbone_clades[i].end())
            {
                return false;
            }
        }
    }
    
    // if no search has failed, then the match succeeds
    return true;
}


/**
 * We check here if all the monophyly constraints are satisfied.
 *
 * \return     True if the constraints are matched, false otherwise.
 */
bool TopologyConstrainedTreeDistribution::matchesConstraints( void )
{
    for(size_t i = 0; i < monophyly_constraints.size(); i++)
    {
        std::vector<RbBitSet>::iterator it = std::find(active_clades.begin(), active_clades.end(), monophyly_constraints[i].getBitRepresentation() );
        if(it == active_clades.end())
        {
            return false;
        }
    }

    return true;
}




void TopologyConstrainedTreeDistribution::recursivelyFlagNodesDirty(const TopologyNode& n)
{


    dirty_nodes[ n.getIndex() ] = true;

    if ( n.isRoot() )
        return;

    recursivelyFlagNodesDirty(n.getParent());

}


RbBitSet TopologyConstrainedTreeDistribution::recursivelyAddBackboneConstraints( const TopologyNode& node, size_t backbone_idx )
{
    RbBitSet tmp( value->getNumberOfTips() );

    if ( node.isTip() )
    {
        const std::map<std::string, size_t>& taxon_map = value->getTaxonBitSetMap();
        const std::string& name = node.getName();
        std::map<std::string, size_t>::const_iterator it = taxon_map.find(name);
        tmp.set( it->second );
    }
    else
    {
        // get the child names
        for (size_t i = 0; i < node.getNumberOfChildren(); i++)
        {
            tmp |= recursivelyAddBackboneConstraints( node.getChild(i), backbone_idx );
        }

        if ( node.isRoot() == false )
        {
            backbone_constraints[backbone_idx].push_back(tmp);
        }
    }
    
    return tmp;
}


RbBitSet TopologyConstrainedTreeDistribution::recursivelyUpdateClades( const TopologyNode& node )
{
    if ( node.isTip() )
    {
        RbBitSet tmp = RbBitSet( value->getNumberOfTips() );
        const std::map<std::string, size_t>& taxon_map = value->getTaxonBitSetMap();
        const std::string& name = node.getName();
        std::map<std::string, size_t>::const_iterator it = taxon_map.find(name);
        tmp.set( it->second );
        return tmp;
    }
    else if ( node.isRoot() )
    {
        if ( dirty_nodes[node.getIndex()] == true )
        {
            for (size_t i = 0; i < node.getNumberOfChildren(); i++)
            {
                recursivelyUpdateClades( node.getChild(i) );
            }

            dirty_nodes[node.getIndex()] = false;
        }

        return RbBitSet( value->getNumberOfTips(), true );
    }
    else
    {
        if ( dirty_nodes[node.getIndex()] == true )
        {
            RbBitSet tmp = RbBitSet( value->getNumberOfTips() );
            for (size_t i = 0; i < node.getNumberOfChildren(); i++)
            {
                tmp |= recursivelyUpdateClades( node.getChild(i) );
            }

            // update the clade
            active_clades[node.getIndex() - value->getNumberOfTips()] = tmp;
            
            for (size_t i = 0; i < num_backbones; i++) {
                
                active_backbone_clades[i][node.getIndex() - value->getNumberOfTips()] = tmp & backbone_mask[i];
            }
            

            dirty_nodes[node.getIndex()] = false;
        }

        return active_clades[node.getIndex() - value->getNumberOfTips()];
    }
}


/**
 * Redraw the current value. We delegate this to the simulate method.
 */
void TopologyConstrainedTreeDistribution::redrawValue( void )
{
    
    Tree* new_value = simulateTree();
    // base_distribution->redrawValue();

    value->getTreeChangeEventHandler().removeListener( this );
    new_value->getTreeChangeEventHandler().addListener( this );

    // if we don't own the tree, then we just replace the current pointer with the pointer
    // to the new value of the base distribution
    value = new_value;
    base_distribution->setValue( value );
    
    // recompute the active clades
    dirty_nodes = std::vector<bool>( value->getNumberOfNodes(), true );

    recursivelyUpdateClades( value->getRoot() );

    stored_clades          = active_clades;
    stored_backbone_clades = active_backbone_clades;
}

/**
 *
 */
Tree* TopologyConstrainedTreeDistribution::simulateTree( void )
{
    
    // the time tree object (topology & times)
    Tree *psi = new Tree();
    
    // internally we treat unrooted topologies the same as rooted
    psi->setRooted( true );
    
    AbstractRootedTreeDistribution* tree_base_distribution = dynamic_cast<AbstractRootedTreeDistribution*>( base_distribution );
    size_t num_taxa = tree_base_distribution->getNumberOfTaxa();
    const std::vector<Taxon> &taxa = tree_base_distribution->getTaxa();
    
    // create the tip nodes
    std::vector<TopologyNode*> nodes;
    for (size_t i=0; i<num_taxa; ++i)
    {
        
        // create the i-th taxon
        TopologyNode* node = new TopologyNode( taxa[i], i );
        
        // set the age of this tip node
        node->setAge( taxa[i].getAge() );
        
        if (node->getAge() > 0)
        {
            node->setFossil(true);
        }
        
        // add the new node to the list
        nodes.push_back( node );
        
    }
    
    
    double ra = tree_base_distribution->getRootAge();
    double max_age = tree_base_distribution->getOriginTime();
    
    // we need a sorted vector of constraints, starting with the smallest
    std::set<Clade> sorted_clades;
    
    for (size_t i = 0; i < monophyly_constraints.size(); ++i)
    {
        if (monophyly_constraints[i].getAge() > max_age)
        {
            throw RbException("Cannot simulate tree: clade constraints are older than the origin age.");
        }
        
        // set the ages of each of the taxa in the constraint
        for (size_t j = 0; j < monophyly_constraints[i].size(); ++j)
        {
            for (size_t k = 0; k < num_taxa; k++)
            {
                if ( taxa[k].getName() == monophyly_constraints[i].getTaxonName(j) )
                {
                    monophyly_constraints[i].setTaxonAge(j, taxa[k].getAge());
                    break;
                }
            }
        }
        
        if ( monophyly_constraints[i].size() > 1 && monophyly_constraints[i].size() < num_taxa )
        {
            sorted_clades.insert( monophyly_constraints[i] );
        }
    }
    
    // create a clade that contains all species
    Clade all_species = Clade(taxa);
    all_species.setAge( ra );
    sorted_clades.insert(all_species);
    
    std::vector<Clade> virtual_taxa;
    int i = -1;
    for(std::set<Clade>::iterator it = sorted_clades.begin(); it != sorted_clades.end(); it++)
    {
        i++;
        const Clade &c = *it;
        std::vector<Taxon> taxa = c.getTaxa();
        std::vector<Clade> clades;
        
        int j = i;
        std::set<Clade>::reverse_iterator jt(it);
        for(; jt != sorted_clades.rend(); jt++)
        {
            j--;
            const Clade &c_nested = *jt;
            const std::vector<Taxon> &taxa_nested = c_nested.getTaxa();
            
            bool found_all = true;
            bool found_some = false;
            for (size_t k = 0; k < taxa_nested.size(); ++k)
            {
                std::vector<Taxon>::iterator it = std::find(taxa.begin(), taxa.end(), taxa_nested[k]);
                if ( it != taxa.end() )
                {
                    taxa.erase( it );
                    found_some = true;
                }
                else
                {
                    found_all = false;
                }
                
            }
            
            if ( found_all == true )
            {
                //                c.addTaxon( virtual_taxa[j] );
                //                taxa.push_back( virtual_taxa[j] );
                clades.push_back( virtual_taxa[j] );
            }
            
            if ( found_all == false && found_some == true )
            {
                throw RbException("Cannot simulate tree: conflicting monophyletic clade constraints. Check that all clade constraints are properly nested.");
            }
            
        }
        
        
        std::vector<TopologyNode*> nodes_in_clade;
        
        
        for (size_t k = 0; k < taxa.size(); ++k)
        {
            Clade c = Clade( taxa[k] );
            c.setAge( taxa[k].getAge() );
            clades.push_back( c );
        }
        
        for (size_t k = 0; k < clades.size(); ++k)
        {
            for (size_t j = 0; j < nodes.size(); ++j)
            {
                if (nodes[j]->getClade() == clades[k])
                {
                    nodes_in_clade.push_back( nodes[j] );
                    nodes.erase( nodes.begin()+j );
                    break;
                }
                
            }
            
        }
        
        double clade_age = c.getAge();
        
        double max_node_age = 0;
        for (size_t j = 0; j < nodes_in_clade.size(); ++j)
        {
            if ( nodes_in_clade[j]->getAge() > max_node_age )
            {
                max_node_age = nodes_in_clade[j]->getAge();
            }
        }
        if ( clade_age <= max_node_age )
        {
            // Get the rng
            RandomNumberGenerator* rng = GLOBAL_RNG;
            
            clade_age = rng->uniform01() * ( max_age - max_node_age ) + max_node_age;
        }

        tree_base_distribution->simulateClade(nodes_in_clade, clade_age, max_age);
        nodes.push_back( nodes_in_clade[0] );
        
        std::vector<Taxon> v_taxa;
        nodes_in_clade[0]->getTaxa(v_taxa);
        Clade new_clade = Clade(v_taxa);
        new_clade.setAge( nodes_in_clade[0]->getAge() );
        virtual_taxa.push_back( new_clade );
    }
    
    TopologyNode *root = nodes[0];
    
    // initialize the topology by setting the root
    psi->setRoot(root, true);

    return psi;
}


/**
 * Set the DAG node.
 */
void TopologyConstrainedTreeDistribution::setStochasticNode( StochasticNode<Tree> *n )
{
    
    // delegate to base class first
    TypedDistribution<Tree>::setStochasticNode( n );
    
    if ( base_distribution != NULL )
    {
        base_distribution->setStochasticNode( n );
    }
    
}


/**
 * Set the current value.
 */
void TopologyConstrainedTreeDistribution::setValue(Tree *v, bool f )
{
    value->getTreeChangeEventHandler().removeListener( this );

    // we set our value to the same value as the base distribution
    // but first we need to make sure that our base class doesn't delete the value
    value = NULL;

    // and the we can set it for both ourselves and the base distribution
    TypedDistribution<Tree>::setValue(v, f);
    base_distribution->setValue(v, f);

    value->getTreeChangeEventHandler().addListener( this );

    initializeBitSets();

    // recompute the active clades
    dirty_nodes = std::vector<bool>( value->getNumberOfNodes(), true );

    recursivelyUpdateClades( value->getRoot() );

    stored_clades          = active_clades;
    stored_backbone_clades = active_backbone_clades;
}


/**
 * Swap the parameters held by this distribution.
 *
 *
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void TopologyConstrainedTreeDistribution::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    
    if ( oldP == backbone_topologies )
    {
        backbone_topologies = static_cast<const TypedDagNode<RbVector<Tree> >* >( newP );
    }
    else
    {
        base_distribution->swapParameter(oldP,newP);
    }
    
}

/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void TopologyConstrainedTreeDistribution::touchSpecialization(DagNode *affecter, bool touchAll)
{
    stored_clades = active_clades;
    stored_backbone_clades = active_backbone_clades;

    // if the root age wasn't the affecter, we'll set it in the base distribution here
    base_distribution->touch(affecter, touchAll);
}

void TopologyConstrainedTreeDistribution::keepSpecialization(DagNode *affecter)
{
    stored_clades = active_clades;
    stored_backbone_clades = active_backbone_clades;

    base_distribution->keep(affecter);
}

void TopologyConstrainedTreeDistribution::restoreSpecialization(DagNode *restorer)
{
    active_clades = stored_clades;
    active_backbone_clades = stored_backbone_clades;

    base_distribution->restore(restorer);

}
