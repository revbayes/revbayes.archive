#include "AbstractHomologousDiscreteCharacterData.h"
#include "BranchHeterogeneousMorphospeciationProcess.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "Clade.h"
#include "HomologousDiscreteCharacterData.h"
#include "DeterministicNode.h"
#include "MatrixReal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "RealPos.h"
#include "RlString.h"
#include "StandardState.h"
#include "StochasticNode.h"
#include "TopologyNode.h"

#include <boost/lexical_cast.hpp>
#include <boost/numeric/odeint.hpp>


using namespace RevBayesCore;


/**
 * Constructor.
 *
 * The constructor connects the parameters of the birth-death process (DAG structure)
 * and initializes the probability density by computing the combinatorial constant of the tree structure.
 */
BranchHeterogeneousMorphospeciationProcess::BranchHeterogeneousMorphospeciationProcess(const TypedDagNode<double> *age,
                                                   const TypedDagNode<double> *asym,
                                                   const TypedDagNode<double> *ana,
                                                   const TypedDagNode<double> *ext,
                                                   const TypedDagNode<double> *foss,
                                                   const TypedDagNode<double> *rh,
                                                   const std::string &cdt,
                                                   size_t n,
                                                   size_t k,
                                                   bool uo,
                                                   size_t min_num_lineages,
                                                   size_t max_num_lineages,
                                                   size_t exact_num_lineages,
                                                   double max_t,
                                                   bool prune,
                                                   bool condition_on_tip_states,
                                                   bool condition_on_num_tips,
                                                   bool condition_on_tree) : TypedDistribution<Tree>( new TreeDiscreteCharacterData() ),
    num_sites( n ),
    num_states( k ),
    num_site_rates( 1 ),
    transition_prob_matrices(),
    condition( cdt ),
    active_likelihood( std::vector<bool>(5, 0) ),
    changed_nodes( std::vector<bool>(5, false) ),
    dirty_nodes( std::vector<bool>(5, true) ),
    node_partial_likelihoods( std::vector<std::vector<std::vector<std::vector<std::vector<double> > > > >(1, std::vector<std::vector<std::vector<std::vector<double> > > >(2, std::vector<std::vector<std::vector<double> > >(1, std::vector<std::vector<double> >(num_sites, std::vector<double>(num_states, 0.0))))) ),
    scaling_factors( std::vector<std::vector< std::vector<double> > >(1, std::vector<std::vector<double> >(2, std::vector<double>(num_sites, 0.0) ) ) ),
    use_origin( uo ),
    process_age( age ),
    lambda(asym),
    lambda_a(ana),
    mu( ext ),
    psi( foss),
    rho( rh ),
    site_rates(NULL),
    speciation_chars(std::vector<size_t>(num_sites,0)),
    rate_allocation(std::vector<std::vector<size_t> >(2, std::vector<size_t>(num_sites,0)) ),
    active_rate_allocation(false),
    rate_variation_across_sites(false),
    mean_site_rate(1.0),
    tmp_allocation(0),
    NUM_TIME_SLICES( 500.0 )
{
    addParameter( process_age );
    addParameter( lambda );
    addParameter( lambda_a );
    addParameter( mu );
    addParameter( psi );
    addParameter( rho );
    
    if ( min_num_lineages > max_num_lineages )
    {
        throw RbException("minNumLineages cannot be greater than maxNumLineages.");
    }
    
    // set the length of the time slices used by the ODE for numerical integration
    dt = process_age->getValue() / NUM_TIME_SLICES * 10.0;

    value->getTreeChangeEventHandler().addListener( this );
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
BranchHeterogeneousMorphospeciationProcess* BranchHeterogeneousMorphospeciationProcess::clone( void ) const
{
    BranchHeterogeneousMorphospeciationProcess* tmp = new BranchHeterogeneousMorphospeciationProcess( *this );
    tmp->getValue().getTreeChangeEventHandler().addListener(tmp);
    return tmp;
}


/**
 * Destructor. Because we added ourselves as a reference to tau when we added a listener to its
 * TreeChangeEventHandler, we need to remove ourselves as a reference and possibly delete tau
 * when we die. All other parameters are handled by others.
 */
BranchHeterogeneousMorphospeciationProcess::~BranchHeterogeneousMorphospeciationProcess( void )
{
    // We don't delete the params, because they might be used somewhere else too. The model needs to do that!

    // remove myself from the tree listeners
    value->getTreeChangeEventHandler().removeListener( this );
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double BranchHeterogeneousMorphospeciationProcess::computeLnProbability( void )
{
    
    // check that the ages are in correct chronological order
    // i.e., no child is older than its parent
    const std::vector<TopologyNode*>& nodes = value->getNodes();
    for (std::vector<TopologyNode*>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        
        const TopologyNode &the_node = *(*it);
        if ( the_node.isRoot() == false )
        {
            
            if ( (the_node.getAge() - (*it)->getParent().getAge()) > 0 && the_node.isSampledAncestor() == false )
            {
                return RbConstants::Double::neginf;
            }
            else if ( (the_node.getAge() - (*it)->getParent().getAge()) > 0 && the_node.isSampledAncestor() == true )
            {
                return RbConstants::Double::neginf;
            }
            
        }
        
    }
    
    // check that the sampled ancestor nodes have a zero branch length
    for (std::vector<TopologyNode*>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        
        const TopologyNode &the_node = *(*it);
        if ( the_node.isSampledAncestor() == true )
        {
            
            if ( the_node.isFossil() == false )
            {
                return RbConstants::Double::neginf;
            }
            else if ( the_node.getBranchLength() > 0 )
            {
                return RbConstants::Double::neginf;
            }
            
        }
        
    }
    
    double num_initial_lineages = 2; // this needs to be a double!
    const TopologyNode& root = value->getRoot();

    if ( use_origin == true )
    {
        // If we are conditioning on survival from the origin,
        // then we must divide by 2 the log survival probability computed by AbstractBirthDeathProcess
        num_initial_lineages = 1;
    }
    // if conditioning on root, root node must be a "true" bifurcation event
    else if (root.getChild(0).isSampledAncestor() || root.getChild(1).isSampledAncestor())
    {
        return RbConstants::Double::neginf;
    }

    // present time
    double ra = root.getAge();
    double process_time = getOriginAge();
    
    if ( ra > process_time || ra != getRootAge() )
    {
        return RbConstants::Double::neginf;
    }
    
    const std::vector<TopologyNode*> &c = root.getChildren();

    for (std::vector<TopologyNode*>::const_iterator it = c.begin(); it != c.end(); ++it)
    {
        if ( ra < (*it)->getAge() )
        {
            return RbConstants::Double::neginf;
        }
    }

    if ( value->getNumberOfNodes() != dirty_nodes.size() )
    {
        resizeVectors();
    }
    
    return computeRootLikelihood() + lnProbTreeShape();
}


void BranchHeterogeneousMorphospeciationProcess::computeNodeProbability(const RevBayesCore::TopologyNode &node, size_t node_index)
{
    // check for recomputation
    if ( dirty_nodes[node_index] == true )
    {
        updateTransitionProbabilities(node_index);

        // mark as computed
        dirty_nodes[node_index] = false;
        
        std::vector<std::vector<std::vector<double> > > &node_likelihood  = node_partial_likelihoods[node_index][active_likelihood[node_index]];

        std::vector<double> max(num_sites, 0.0);

        if ( node.isTip() == true )
        {
            // this is a tip node
            TreeDiscreteCharacterData* tree = static_cast<TreeDiscreteCharacterData*>( this->value );
            
            RbBitSet obs_state(num_states, true);
            bool gap = true;

            if ( tree->hasCharacterData() == true )
            {
                const DiscreteCharacterState &state = tree->getCharacterData().getTaxonData( node.getTaxon().getName() )[0];
                obs_state = state.getState();
                gap = (state.isMissingState() == true || state.isGapState() == true);
            }

            double sampling = node.isFossil() ? mu->getValue() : rho->getValue();

            for (size_t i = 0; i < num_sites; ++i)
            {
                size_t mixture = rate_allocation[active_rate_allocation][i];

                // the transition probability matrix for this mixture category
                const std::vector<std::vector<double> >& tp = this->transition_prob_matrices[mixture];

                std::vector<double> &site_likelihood = node_likelihood[mixture][i];

                for (size_t from = 0; from < num_states; ++from)
                {
                    site_likelihood[from] = 0.0;

                    for (size_t to = 0; to < num_states; ++to)
                    {
                        if ( obs_state.isSet( to ) == true || gap == true )
                        {
                            site_likelihood[from] += tp[from][to];
                        }
                    }

                    site_likelihood[from] *= sampling;

                    if( site_likelihood[from] > max[i] )
                    {
                        max[i] = site_likelihood[from];
                    }
                }
            }
        }
        else
        {
            
            // this is an internal node
            const TopologyNode          &left           = node.getChild(0);
            size_t                      left_index      = left.getIndex();
            computeNodeProbability( left, left_index );
            const TopologyNode          &right          = node.getChild(1);
            size_t                      right_index     = right.getIndex();
            computeNodeProbability( right, right_index );
            
            // update species index
            node_taxa[node.getIndex()] = right.isSampledAncestor() ? node_taxa[right_index] : node_taxa[left_index];

            // update transition probabilities after updating species indices
            updateTransitionProbabilities(node_index);

            // get the likelihoods of descendant nodes
            const std::vector<std::vector<std::vector<double> > > &left_likelihoods  = node_partial_likelihoods[left_index][active_likelihood[left_index]];
            const std::vector<std::vector<std::vector<double> > > &right_likelihoods = node_partial_likelihoods[right_index][active_likelihood[right_index]];

            bool anagenetic_node = ( left.isSampledAncestor() || right.isSampledAncestor() );

            // merge descendant likelihoods
            for (size_t i=0; i<num_sites; ++i)
            {
                size_t mixture = rate_allocation[active_rate_allocation][i];

                // the transition probability matrix for this mixture category
                const std::vector<std::vector<double> >& tp = this->transition_prob_matrices[mixture];

                const std::vector<double> &site_likelihood = node_likelihood[mixture][i];
                const std::vector<double> &left_site_likelihood = left_likelihoods[mixture][i];
                const std::vector<double> &right_site_likelihood = right_likelihoods[mixture][i];

                for (size_t from=0; from<num_states; ++from)
                {
                    site_likelihood[from] = 0.0;

                    for (size_t to=0; to<num_states; ++to)
                    {
                        double merged_probability = 0.0;

                        if( i == speciation_chars[node_index])
                        {
                            for (size_t k=0; k<num_states; ++k)
                            {
                                if( k != to )
                                {
                                    if( anagenetic_node )
                                    {
                                        merged_probability += left.isSampledAncestor() ? left_site_likelihood[k] : right_site_likelihood[k];
                                    }
                                    else
                                    {
                                        merged_probability += left_site_likelihood[k] * right_site_likelihood[to];
                                    }
                                }
                            }

                            merged_probability /= double(num_states - 1);
                        }
                        else
                        {
                            if( anagenetic_node )
                            {
                                merged_probability = left.isSampledAncestor() ? left_site_likelihood[to] : right_site_likelihood[to];
                            }
                            else
                            {
                                merged_probability = left_site_likelihood[to] * right_site_likelihood[to];
                            }

                        }

                        site_likelihood[from] += merged_probability * tp[from][to];
                    }

                    site_likelihood[from] *= anagenetic_node ? lambda_a->getValue() : lambda->getValue();

                    if( site_likelihood[from] > max[i] )
                    {
                        max[i] = site_likelihood[from];
                    }
                }
            }
            
        }
        
        if ( RbSettings::userSettings().getUseScaling() == true ) //&& node_index % RbSettings::userSettings().getScalingDensity() == 0 )
        {
            for (size_t i = 0; i < num_sites; ++i)
            {
                for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
                {
                    std::vector<double> &site_likelihood = node_likelihood[mixture][i];

                    for (size_t j = 0; j < num_states; ++j)
                    {
                        site_likelihood[j] /= max[i];
                    }
                }

                scaling_factors[node_index][active_likelihood[node_index]][i] = log(max[i]);

                if ( node.isTip() == false )
                {
                    const TopologyNode          &left           = node.getChild(0);
                    size_t                      left_index      = left.getIndex();
                    const TopologyNode          &right          = node.getChild(1);
                    size_t                      right_index     = right.getIndex();
                    scaling_factors[node_index][active_likelihood[node_index]][i] += scaling_factors[left_index][active_likelihood[left_index]][i] + scaling_factors[right_index][active_likelihood[right_index]][i];
                }
            }
        }
    }
    
}


double BranchHeterogeneousMorphospeciationProcess::computeRootLikelihood( void )
{
    updateMeanSiteRate();

    // get the likelihoods of descendant nodes
    const TopologyNode     &root            = value->getRoot();
    size_t                  node_index      = root.getIndex();
    const TopologyNode     &left            = root.getChild(0);
    size_t                  left_index      = left.getIndex();
    computeNodeProbability( left, left_index );
    const TopologyNode     &right           = root.getChild(1);
    size_t                  right_index     = right.getIndex();
    computeNodeProbability( right, right_index );

    updateTransitionProbabilities(node_index);

    // get the likelihoods of descendant nodes
    const std::vector<std::vector<std::vector<double> > > &left_likelihoods  = node_partial_likelihoods[left_index][active_likelihood[left_index]];
    const std::vector<std::vector<std::vector<double> > > &right_likelihoods = node_partial_likelihoods[right_index][active_likelihood[right_index]];

    std::vector<std::vector<std::vector<double> > > &node_likelihood  = node_partial_likelihoods[node_index][active_likelihood[node_index]];

    bool anagenetic_node = ( left.isSampledAncestor() || right.isSampledAncestor() );

    double lnProb = 0.0;

    // merge descendant likelihoods
    for (size_t i=0; i<num_sites; ++i)
    {
        double site_prob = 0.0;

        size_t mixture = rate_allocation[active_rate_allocation][i];

        // the transition probability matrix for this mixture category
        const std::vector<std::vector<double> >& tp = this->transition_prob_matrices[mixture];

        const std::vector<double> &site_likelihood = node_likelihood[mixture][i];
        const std::vector<double> &left_site_likelihood = left_likelihoods[mixture][i];
        const std::vector<double> &right_site_likelihood = right_likelihoods[mixture][i];

        for (size_t from=0; from<num_states; ++from)
        {
            site_likelihood[from] = 0.0;

            for (size_t to=0; to<num_states; ++to)
            {
                double merged_probability = 0.0;

                if( i == speciation_chars[node_index])
                {
                    for (size_t k=0; k<num_states; ++k)
                    {
                        if( k != to )
                        {
                            if( anagenetic_node )
                            {
                                merged_probability += left.isSampledAncestor() ? left_site_likelihood[k] : right_site_likelihood[k];
                            }
                            else
                            {
                                merged_probability += left_site_likelihood[k] * right_site_likelihood[to];
                            }
                        }
                    }

                    merged_probability /= double(num_states - 1);
                }
                else
                {
                    if( anagenetic_node )
                    {
                        merged_probability = left.isSampledAncestor() ? left_site_likelihood[to] : right_site_likelihood[to];
                    }
                    else
                    {
                        merged_probability = left_site_likelihood[to] * right_site_likelihood[to];
                    }

                }

                site_likelihood[from] += merged_probability * tp[from][to];
            }

            site_likelihood[from] *= anagenetic_node ? lambda_a->getValue() : lambda->getValue();

            site_prob += site_likelihood[from];
        }

        scaling_factors[node_index][active_likelihood[node_index]][i] = scaling_factors[left_index][active_likelihood[left_index]][i] + scaling_factors[right_index][active_likelihood[right_index]][i];

        lnProb += log(site_prob) + scaling_factors[node_index][active_likelihood[node_index]][i];
    }
    
    return lnProb - num_sites * log(num_states);
}


void BranchHeterogeneousMorphospeciationProcess::fireTreeChangeEvent( const RevBayesCore::TopologyNode &n, const unsigned& m )
{
    // call a recursive flagging of all node above (closer to the root) and including this node
    recursivelyFlagNodeDirty( n );

}


const RevBayesCore::AbstractHomologousDiscreteCharacterData& BranchHeterogeneousMorphospeciationProcess::getCharacterData() const
{
    return static_cast<TreeDiscreteCharacterData*>(this->value)->getCharacterData();
}


void BranchHeterogeneousMorphospeciationProcess::recursivelyFlagNodeDirty( const RevBayesCore::TopologyNode &n ) {

    // we need to flag this node and all ancestral nodes for recomputation
    size_t index = n.getIndex();

    // if this node is already dirty, the also all the ancestral nodes must have been flagged as dirty
    if ( dirty_nodes[index] == false )
    {
        // the root doesn't have an ancestor
        if ( n.isRoot() == false )
        {
            recursivelyFlagNodeDirty( n.getParent() );
        }

        // set the flag
        dirty_nodes[index] = true;

        // if we previously haven't touched this node, then we need to change the active likelihood pointer
        if ( changed_nodes[index] == false )
        {
            active_likelihood[index] = (active_likelihood[index] == 0 ? 1 : 0);
            changed_nodes[index] = true;
        }

    }

}


RevLanguage::RevPtr<RevLanguage::RevVariable> BranchHeterogeneousMorphospeciationProcess::executeProcedure(const std::string &name, const std::vector<DagNode *> args, bool &found)
{    
    if (name == "clampCharData")
    {
        found = true;
        
        const AbstractHomologousDiscreteCharacterData& v = static_cast<const TypedDagNode<AbstractHomologousDiscreteCharacterData > *>( args[0] )->getValue();
    
        // check if the tip names match
        bool match = true;
        std::vector<string> tips = value->getTipNames();
        for (size_t i = 0; i < tips.size(); i++)
        {
            found = false;
            for (size_t j = 0; j < v.getNumberOfTaxa(); j++)
            {
                if (tips[i] == v[j].getTaxonName()) 
                {
                    found = true;
                    break;
                }
            }
            if (found == false)
            {
                match = false;
                break;
            }
        }
        if (match == false)
        {
            throw RbException("To clamp a character data object all taxa present in the tree must be present in the character data.");
        }
        
        static_cast<TreeDiscreteCharacterData*>(this->value)->setCharacterData( v.clone() );

        return NULL;
    }
    
    if (name == "getCharData") 
    { 
        found = true;
        RevLanguage::AbstractHomologousDiscreteCharacterData *tip_states = new RevLanguage::AbstractHomologousDiscreteCharacterData( getCharacterData() );
        return new RevLanguage::RevVariable( tip_states );
    }
    return TypedDistribution<Tree>::executeProcedure( name, args, found );
}


void BranchHeterogeneousMorphospeciationProcess::executeMethod(const std::string &name, const std::vector<const DagNode *> &args, RbVector<double> &rv) const
{
   
    throw RbException("The character dependent birth-death process does not have a member method called '" + name + "'.");

}


/**
 * Get the affected nodes by a change of this node.
 * If the root age has changed than we need to call get affected again.
 */
void BranchHeterogeneousMorphospeciationProcess::getAffected(RbOrderedSet<DagNode *> &affected, RevBayesCore::DagNode *affecter)
{
    
    if ( affecter == process_age)
    {
        dag_node->initiateGetAffectedNodes( affected );
    }
    
}


double BranchHeterogeneousMorphospeciationProcess::getOriginAge( void ) const
{

    return process_age->getValue();
}


/**
 * By default, the root age is assumed to be equal to the origin time.
 * This should be overridden if a distinct root age is needed
 */
double BranchHeterogeneousMorphospeciationProcess::getRootAge( void ) const
{

    if (use_origin)
    {
        if (value->getNumberOfNodes() > 0)
        {
            return value->getRoot().getAge();
        }
        else
        {
            return 0;
        }
    }
    else
        return getOriginAge();
}


/**
 * Keep the current value and reset some internal flags. Nothing to do here.
 */
void BranchHeterogeneousMorphospeciationProcess::keepSpecialization(DagNode *affecter)
{
    
    if ( affecter == process_age )
    {
        dag_node->keepAffected();
    }
    
    // reset all flags
    for (std::vector<bool>::iterator it = this->dirty_nodes.begin(); it != this->dirty_nodes.end(); ++it)
    {
        (*it) = false;
    }

    for (std::vector<bool>::iterator it = this->changed_nodes.begin(); it != this->changed_nodes.end(); ++it)
    {
        (*it) = false;
    }

}


double BranchHeterogeneousMorphospeciationProcess::lnProbTreeShape(void) const
{
    // the birth death divergence times density is derived for a (ranked) unlabeled oriented tree
    // so we convert to a (ranked) labeled non-oriented tree probability by multiplying by 2^{n+m-1} / n!
    // where n is the number of extant tips, m is the number of extinct tips

    int num_taxa = (int)value->getNumberOfTips();
    int num_extinct = (int)value->getNumberOfExtinctTips();
    int num_sa = (int)value->getNumberOfSampledAncestors();

    return (num_taxa - num_sa - 1) * RbConstants::LN2 - RbMath::lnFactorial(num_taxa - num_extinct);
}



/**
 * Redraw the current value. We delegate this to the simulate method.
 */
void BranchHeterogeneousMorphospeciationProcess::redrawValue( void )
{
    node_taxa = std::vector<size_t>(value->getNumberOfTips(), 0);

    for(size_t i = 0; i < value->getNumberOfTips(); i++)
    {
        node_taxa[value->getTipNode(i)] = find(taxa.begin(), taxa.end(), value->getTipNode(i).getTaxon()) - taxa.begin();
    }
}



/**
 * Restore the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void BranchHeterogeneousMorphospeciationProcess::restoreSpecialization(DagNode *affecter)
{
    
    if ( affecter == process_age )
    {
        if ( use_origin == false )
        {
            value->getRoot().setAge( process_age->getValue() );
        }

        if ( dag_node != NULL )
        {
            dag_node->restoreAffected();
        }
    }
    
    active_rate_allocation = false ? true : false;

    // reset the flags
    for (std::vector<bool>::iterator it = dirty_nodes.begin(); it != dirty_nodes.end(); ++it)
    {
        (*it) = false;
    }

    // restore the active likelihoods vector
    for (size_t index = 0; index < changed_nodes.size(); ++index)
    {
        // we have to restore, that means if we have changed the active likelihood vector
        // then we need to revert this change
        if ( changed_nodes[index] == true )
        {
            active_likelihood[index] = (active_likelihood[index] == 0 ? 1 : 0);
        }

        // set all flags to false
        changed_nodes[index] = false;
    }

}


void BranchHeterogeneousMorphospeciationProcess::setNumberOfTimeSlices( double n )
{
    
    NUM_TIME_SLICES = n;
    dt = process_age->getValue() / NUM_TIME_SLICES;
    
}


void BranchHeterogeneousMorphospeciationProcess::setSiteRates(const TypedDagNode< RbVector< double > > *r)
{

    // remove the old parameter first
    if ( site_rates != NULL )
    {
        this->removeParameter( site_rates );
        site_rates = NULL;
    }

    if ( r != NULL )
    {
        // set the value
        rate_variation_across_sites = true;
        site_rates = r;
        this->num_site_rates = r->getValue().size();
    }
    else
    {
        // set the value
        rate_variation_across_sites = false;
        site_rates = NULL;
        this->num_site_rates = 1;
    }

    this->resizeVectors();

    // add the new parameter
    this->addParameter( site_rates );

    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
}


/**
 * Set the current value.
 */
void BranchHeterogeneousMorphospeciationProcess::setValue(Tree *v, bool f )
{
    if (v->isBinary() == false)
    {
        throw RbException("The character-dependent birth death process is only implemented for binary trees.");
    }

    value->getTreeChangeEventHandler().removeListener( this );

    // delegate to super class
    //    TypedDistribution<Tree>::setValue(v, f);
    static_cast<TreeDiscreteCharacterData *>(this->value)->setTree( *v );

    resizeVectors();
    
    // clear memory
    delete v;
    
    value->getTreeChangeEventHandler().addListener( this );
    
    if ( process_age != NULL && use_origin == false )
    {
        const StochasticNode<double> *stoch_process_age = dynamic_cast<const StochasticNode<double>* >(process_age);
        if ( stoch_process_age != NULL )
        {
            const_cast<StochasticNode<double> *>(stoch_process_age)->setValue( new double( value->getRoot().getAge() ), f);
        }
        else
        {
            value->getRoot().setAge( process_age->getValue() );
        }
        
    }

    // make character data objects -- all unknown/missing
    std::vector<string> tips = value->getTipNames();
    HomologousDiscreteCharacterData<NaturalNumbersState> *tip_data = new HomologousDiscreteCharacterData<NaturalNumbersState>();
    for (size_t i = 0; i < tips.size(); i++)
    {
        DiscreteTaxonData<NaturalNumbersState> this_tip_data = DiscreteTaxonData<NaturalNumbersState>(tips[i]);
        NaturalNumbersState state = NaturalNumbersState(0, num_states);
        state.setState("?");
        this_tip_data.addCharacter(state);
        tip_data->addTaxonData(this_tip_data);
    }
    static_cast<TreeDiscreteCharacterData*>(this->value)->setCharacterData(tip_data);
}


/**
 *
 */
bool BranchHeterogeneousMorphospeciationProcess::simulateTree( size_t attempts )
{

    if ( use_origin == true && condition_on_num_tips == false )
    {
        // if originAge is set we start with one lineage
        // if rootAge is set we start with two lineages and their speciation event
        throw RbException("Simulations are currently only implemented when rootAge is set. You set the originAge.");
    }
    
    if (exact_num_lineages < 2 && condition_on_num_tips == true)
    {
        throw RbException("When simulating conditioned on the number of tips exactNumLineages must be 2 or more.");
    }

    RandomNumberGenerator* rng = GLOBAL_RNG;

    // a vector keeping track of the lineages currently surviving in each state
    // as we simulate forward in time
    std::vector< std::vector<size_t> > lineages_in_state = std::vector< std::vector<size_t> >(num_states, std::vector<size_t>());
    std::vector< std::vector<size_t> > extinct_lineages_in_state = std::vector< std::vector<size_t> >(num_states, std::vector<size_t>());

    // CharacterData object to hold the tip states
    HomologousDiscreteCharacterData<NaturalNumbersState> *tip_data = new HomologousDiscreteCharacterData<NaturalNumbersState>();

    // vectors keeping track of the total rate of all
    // cladogenetic/anagenetic/extinction events for each state
    std::vector<double> extinction_rates = mu->getValue();
    std::vector<double> total_rate_for_state = std::vector<double>(num_states, 0);
    for (size_t i = 0; i < num_states; i++)
    {
        total_rate_for_state[i] = extinction_rates[i] + total_speciation_rates[i] + total_anagenetic_rates[i];
    }

    // get the speciation rates, extinction rates, and Q matrix
    std::map<std::vector<unsigned>, double> eventMap;
    std::vector<double> speciation_rates;
    std::map<std::vector<unsigned>, double>::iterator it;
    if ( use_cladogenetic_events == true )
    {
        eventMap = cladogenesis_matrix->getValue().getEventMap();
    }
    else
    {
        speciation_rates = lambda->getValue();
    }
    const RateGenerator *rate_matrix = &getEventRateMatrix();

    // a vector of all nodes in our simulated tree
    std::vector<TopologyNode*> nodes;

    // initialize the root node
    TopologyNode* root = new TopologyNode(0);
    double t = process_age->getValue();
    if (condition_on_num_tips == true)
    {
        t = 0.0;
    }
    root->setAge(t);
    root->setNodeType(false, true, true);
    root->setTimeInStates(std::vector<double>(num_states, 0.0));
    nodes.push_back(root);

    // now draw a state for the root cladogenetic event
    
    // get root frequencies
    const RbVector<double> &freqs = getRootFrequencies();
    
    std::map<std::vector<unsigned>, double> sample_probs;
    double sample_probs_sum = 0.0;
    
    // calculate probabilities for each state
    if ( use_cladogenetic_events == true )
    {
        // iterate over each cladogenetic event possible
        // and initialize probabilities for each clado event
        for (it = eventMap.begin(); it != eventMap.end(); it++)
        {
            const std::vector<unsigned>& states = it->first;
            double speciation_rate = it->second;
            
            // we need to sample from the ancestor, left, and right states jointly,
            // so keep track of the probability of each clado event
            double prob = freqs[states[0]] * speciation_rate;
            sample_probs[ states ] = prob;
            sample_probs_sum += prob;
        }
    }
    else
    {
        for (size_t i = 0; i < num_states; i++)
        {
            std::vector<unsigned> states = boost::assign::list_of(i)(i)(i);
            sample_probs[ states ] = speciation_rates[i] * freqs[i];
            sample_probs_sum += speciation_rates[i] * freqs[i];
        }
    }
    
    // sample left and right character states from probs
    size_t l = 0, r = 0;
    
    if (sample_probs_sum == 0)
    {
        size_t u = rng->uniform01() * sample_probs.size();
        size_t v = 0;
        for (it = sample_probs.begin(); it != sample_probs.end(); it++)
        {
            if (u < v)
            {
                const std::vector<unsigned>& states = it->first;
                l = states[1];
                r = states[2];
                break;
            }
            v++;
        }
    }
    else
    {
        double u = rng->uniform01() * sample_probs_sum;
        
        for (it = sample_probs.begin(); it != sample_probs.end(); it++)
        {
            u -= it->second;
            if (u < 0.0)
            {
                const std::vector<unsigned>& states = it->first;
                l = states[1];
                r = states[2];
                break;
            }
        }
    }

    // make nodes for each daughter
    TopologyNode* left = new TopologyNode(1);
    left->setAge(t);
    root->addChild(left);
    left->setParent(root);
    left->setNodeType(true, false, false);
    left->setTimeInStates(std::vector<double>(num_states, 0.0));
    lineages_in_state[l].push_back(1);
    nodes.push_back(left);

    TopologyNode* right = new TopologyNode(2);
    right->setAge(t);
    root->addChild(right);
    right->setParent(root);
    right->setNodeType(true, false, false);
    right->setTimeInStates(std::vector<double>(num_states, 0.0));
    lineages_in_state[r].push_back(2);
    nodes.push_back(right);

    // simulate moving forward in time
    while (true) {

        // sum over all rates for all states (multiplied by num lineages in each state)
        double total_rate = 0;
        for (size_t i = 0; i < num_states; i++)
        {
            total_rate += total_rate_for_state[i] * lineages_in_state[i].size();    
        }
        
        // draw the time to next event
        double dt = RbStatistics::Exponential::rv( total_rate, *rng );
        if (condition_on_num_tips == true)
        {
            t = t + dt;
        }
        else
        {
            t = t - dt;
        }

        if (t < 0 and condition_on_num_tips == false)
        {
            dt = dt - (0 - t);
            t = 0;
        }

        // extend all surviving branches to the new time
        size_t num_lineages = 0;
        for (size_t i = 0; i < num_states; i++)
        {
            for (size_t j = 0; j < lineages_in_state[i].size(); j++)
            {
                size_t idx = lineages_in_state[i][j];
                nodes[idx]->setAge(t);
                num_lineages++;
                std::vector<double> state_times = nodes[idx]->getTimeInStates();
                state_times[i] += dt;
                nodes[idx]->setTimeInStates(state_times);
            }
        }

        // stop and retry if we have too many surviving lineages
        if (num_lineages > max_num_lineages && condition_on_num_tips == false)
        {
            nodes.clear();
            delete tip_data;
            return false;
        }

        // stop and retry if we reached the max time
        if (t > max_time && condition_on_num_tips == true)
        {
            nodes.clear();
            delete tip_data;
            return false;
        }

        // stop if we reached the present when conditioning on root age
        if (t == 0 && condition_on_num_tips == false) 
        {
            for (size_t i = 0; i < nodes.size(); i++)
            {
                if (nodes[i]->getAge() == t) 
                {
                    std::stringstream ss;
                    ss << "sp" << i;
                    std::string name = ss.str();
                    nodes[i]->setName(name);
                    nodes[i]->setNodeType(true, false, false);
                }
            }

            // set CharacterData object for each tip state
            for (size_t i = 0; i < num_states; i++)
            {
                for (size_t j = 0; j < lineages_in_state[i].size(); j++)
                {
                    size_t this_node = lineages_in_state[i][j];
                    if (nodes[this_node]->isTip() == true)
                    {
                        DiscreteTaxonData<NaturalNumbersState> this_tip_data = DiscreteTaxonData<NaturalNumbersState>(nodes[this_node]->getName());
                        NaturalNumbersState state = NaturalNumbersState(i, num_states);
                        this_tip_data.addCharacter(state);
                        tip_data->addTaxonData(this_tip_data);
                    }
                }
                if (prune_extinct_lineages == false)
                {
                    for (size_t j = 0; j < extinct_lineages_in_state[i].size(); j++)
                    {
                        size_t this_node = extinct_lineages_in_state[i][j];
                        if (nodes[this_node]->isTip() == true)
                        {
                            DiscreteTaxonData<NaturalNumbersState> this_tip_data = DiscreteTaxonData<NaturalNumbersState>(nodes[this_node]->getName());
                            NaturalNumbersState state = NaturalNumbersState(i, num_states);
                            this_tip_data.addCharacter(state);
                            tip_data->addTaxonData(this_tip_data);
                        }
                    }
                }
            }
            break;
        } 

        // determine the state for the event that occurred
        size_t event_state = 0; 
        double u = rng->uniform01() * total_rate;
        for (size_t i = 0; i < num_states; i++)
        {
            u -= total_rate_for_state[i] * lineages_in_state[i].size();
            if (u < 0)
            {
                event_state = i;
                break;
            }
        }

        // determine the type of event
        std::string event_type = ""; 
        u = rng->uniform01() * total_rate_for_state[event_state];
        while (true) {
            u = u - extinction_rates[event_state];
            if (u < 0) 
            {
                event_type = "extinction";
                break;
            }
            u = u - total_speciation_rates[event_state];
            if (u < 0) 
            {
                event_type = "speciation";
                break;
            }
            u = u - total_anagenetic_rates[event_state];
            if (u < 0) 
            {
                event_type = "anagenetic";
                break;
            }
        }

        // determine which lineage gets the event
        size_t event_index = 0;
        u = rng->uniform01() * static_cast<double>(lineages_in_state[event_state].size());
        event_index = lineages_in_state[event_state][floor(u)];

        if (event_type == "extinction")
        {
            extinct_lineages_in_state[event_state].push_back(event_index);
            lineages_in_state[event_state].erase(std::remove(lineages_in_state[event_state].begin(), lineages_in_state[event_state].end(), event_index), lineages_in_state[event_state].end());
            std::stringstream ss;
            ss << "ex" << event_index;
            std::string name = ss.str();
            nodes[event_index]->setName(name);
            nodes[event_index]->setNodeType(true, false, false);
        }
        
        if (event_type == "anagenetic")
        {
            // remove this lineage from the current state
            lineages_in_state[event_state].erase(std::remove(lineages_in_state[event_state].begin(), lineages_in_state[event_state].end(), event_index), lineages_in_state[event_state].end());

            // draw a new state
            size_t new_state = 0;
            u = rng->uniform01() * total_anagenetic_rates[event_state];
            for (size_t i = 0; i < this->num_states; i++)
            {
                if (i != event_state)
                {
                    u -= rate_matrix->getRate( event_state, i, 0, getEventRate() );
                    if (u < 0.0)
                    {
                        new_state = i;
                        break;
                    }
                }
            } 
            lineages_in_state[new_state].push_back(event_index);
        }
        
        if (event_type == "speciation")
        {

            // stop if we reached the right number of lineages when conditioning on num tips
            if (num_lineages == exact_num_lineages && condition_on_num_tips == true) 
            {
                // TODO trim off uniformly distributed time since last speciation event
                
                for (size_t i = 0; i < nodes.size(); i++)
                {
                    if (nodes[i]->getAge() == t) 
                    {
                        std::stringstream ss;
                        ss << "sp" << i;
                        std::string name = ss.str();
                        nodes[i]->setName(name);
                        nodes[i]->setNodeType(true, false, false);
                    }
                }
                
                // shift node times back so root starts at 0.0
                for (size_t i = 0; i < nodes.size(); i++)
                {
                    nodes[i]->setAge( t - nodes[i]->getAge() );
                }
                
                // set CharacterData object for each tip state
                for (size_t i = 0; i < num_states; i++)
                {
                    for (size_t j = 0; j < lineages_in_state[i].size(); j++)
                    {
                        size_t this_node = lineages_in_state[i][j];
                        if (nodes[this_node]->isTip() == true)
                        {
                            DiscreteTaxonData<NaturalNumbersState> this_tip_data = DiscreteTaxonData<NaturalNumbersState>(nodes[this_node]->getName());
                            NaturalNumbersState state = NaturalNumbersState(i, num_states);
                            this_tip_data.addCharacter(state);
                            tip_data->addTaxonData(this_tip_data);
                        }
                    }
                    if (prune_extinct_lineages == false)
                    {
                        for (size_t j = 0; j < extinct_lineages_in_state[i].size(); j++)
                        {
                            size_t this_node = extinct_lineages_in_state[i][j];
                            if (nodes[this_node]->isTip() == true)
                            {
                                DiscreteTaxonData<NaturalNumbersState> this_tip_data = DiscreteTaxonData<NaturalNumbersState>(nodes[this_node]->getName());
                                NaturalNumbersState state = NaturalNumbersState(i, num_states);
                                this_tip_data.addCharacter(state);
                                tip_data->addTaxonData(this_tip_data);
                            }
                        }
                    }
                }
                break;
            }

            // gather the probabilities for each type of cladogenetic event
            std::map<std::vector<unsigned>, double> sample_probs;
            double sample_probs_sum = 0.0;
            if ( use_cladogenetic_events == true )
            {
                // iterate over each cladogenetic event possible
                for (it = eventMap.begin(); it != eventMap.end(); it++)
                {
                    const std::vector<unsigned>& states = it->first;
                    double speciation_rate = it->second;
                    if (states[0] == event_state) 
                    {
                        // we need to sample from the ancestor, left, and right states jointly,
                        // so keep track of the probability of each clado event
                        double prob = speciation_rate;
                        sample_probs[ states ] = prob;
                        sample_probs_sum += prob;
                    }
                }
            }
            else
            {
                std::vector<unsigned> states = boost::assign::list_of(event_state)(event_state)(event_state);
                sample_probs[ states ] = speciation_rates[event_state];
                sample_probs_sum += speciation_rates[event_state];
            }
            
            // sample left and right character states from probs
            size_t l = 0, r = 0;
            
            if (sample_probs_sum == 0)
            {
                size_t u = rng->uniform01() * sample_probs.size();
                size_t v = 0;
                for (it = sample_probs.begin(); it != sample_probs.end(); it++)
                {
                    if (u < v)
                    {
                        const std::vector<unsigned>& states = it->first;
                        l = states[1];
                        r = states[2];
                        break;
                    }
                    v++;
                }
            }
            else
            {
                double u = rng->uniform01() * sample_probs_sum;
                
                for (it = sample_probs.begin(); it != sample_probs.end(); it++)
                {
                    u -= it->second;
                    if (u < 0.0)
                    {
                        const std::vector<unsigned>& states = it->first;
                        l = states[1];
                        r = states[2];
                        break;
                    }
                }
            }
            
            // make nodes for each daughter
            size_t index = nodes.size();
            TopologyNode* left = new TopologyNode(index);
            left->setAge(t);
            nodes[event_index]->addChild(left);
            left->setParent(nodes[event_index]);
            left->setNodeType(true, false, false);
            left->setTimeInStates(std::vector<double>(num_states, 0.0));
            lineages_in_state[l].push_back(index);
            nodes.push_back(left);

            index = nodes.size();
            TopologyNode* right = new TopologyNode(index);
            right->setAge(t);
            nodes[event_index]->addChild(right);
            right->setParent(nodes[event_index]);
            right->setNodeType(true, false, false);
            right->setTimeInStates(std::vector<double>(num_states, 0.0));
            lineages_in_state[r].push_back(index);
            nodes.push_back(right);
           
            // remove the parent node from our vector of current lineages
            nodes[event_index]->setNodeType(false, false, true);
            lineages_in_state[event_state].erase(std::remove(lineages_in_state[event_state].begin(), lineages_in_state[event_state].end(), event_index), lineages_in_state[event_state].end());
        }
    }
   
    // make a tree object 
    Tree *psi = new Tree();
    psi->setRoot(root, true);
    psi->setRooted(true);
        
    // stop and retry if we have too few surviving lineages
    size_t num_lineages = 0;
    for (size_t i = 0; i < num_states; i++)
    {
        num_lineages += lineages_in_state[i].size();
    }
    if (num_lineages < min_num_lineages and condition_on_num_tips == false)
    {
        delete tip_data;
        nodes.clear();
        delete psi;
        return false;
    }
  
    // prune extinct lineage if necessary
    if (prune_extinct_lineages == true)
    {
        for (size_t i = 0; i < num_states; i++)
        {
            for (size_t j = 0; j < extinct_lineages_in_state[i].size(); j++)
            {
                size_t this_node = extinct_lineages_in_state[i][j];
                if (nodes[this_node]->isTip() == true)
                {
                    psi->dropTipNodeWithName( nodes[this_node]->getName() );
                }
            }
        }
    }
    
    // set the simulated values
    value->getTreeChangeEventHandler().removeListener( this );
    static_cast<TreeDiscreteCharacterData *>(this->value)->setTree( *psi );
    delete psi;

    resizeVectors();
    value->getTreeChangeEventHandler().addListener( this );
    static_cast<TreeDiscreteCharacterData*>(this->value)->setCharacterData(tip_data);
    return true;
    
}



/**
 * Swap the parameters held by this distribution.
 *
 *
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void BranchHeterogeneousMorphospeciationProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if ( oldP == process_age )
    {
        process_age = static_cast<const TypedDagNode<double>* >( newP );
    }
    if ( oldP == mu )
    {
        mu = static_cast<const TypedDagNode<double>* >( newP );
    }
    if ( oldP == lambda )
    {
        lambda = static_cast<const TypedDagNode<double>* >( newP );
    }
    if ( oldP == lambda_a )
    {
        lambda_a = static_cast<const TypedDagNode<double>* >( newP );
    }
    if ( oldP == psi )
    {
        psi = static_cast<const TypedDagNode<double>* >( newP );
    }
    if ( oldP == rho )
    {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    if ( oldP == site_rates )
    {
        site_rates = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
    }
}



/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void BranchHeterogeneousMorphospeciationProcess::touchSpecialization(DagNode *affecter, bool touchAll)
{
    
    if ( affecter == process_age )
    {
        if ( use_origin == false)
        {
            value->getRoot().setAge( process_age->getValue() );
        }

        if ( dag_node != NULL )
        {
            dag_node->touchAffected();
        }
    }

    if ( affecter != this->dag_node )
    {
        
        for (std::vector<bool>::iterator it = dirty_nodes.begin(); it != dirty_nodes.end(); ++it)
        {
            (*it) = true;
        }
        
        // flip the active likelihood pointers
        for (size_t index = 0; index < changed_nodes.size(); ++index)
        {
            if ( changed_nodes[index] == false )
            {
                active_likelihood[index] = (active_likelihood[index] == 0 ? 1 : 0);
                changed_nodes[index] = true;
            }
        }
    }
    
}


void BranchHeterogeneousMorphospeciationProcess::updateTransitionProbabilities(size_t node_idx)
{
    const TopologyNode* node = this->value->getNodes()[node_idx];

    // second, get the clock rate for the branch
    double end_age = node->getAge();

    // if the tree is not a time tree, then the age will be not a number
    if ( RbMath::isFinite(end_age) == false )
    {
        // we assume by default that the end is at time 0
        end_age = 0.0;
    }
    double start_age = end_age + node->getBranchLength();

    // first, get the rate matrix for this branch
    for (size_t j = 0; j < this->num_site_rates; ++j)
    {
        tmp_allocation = j;

        for( size_t from = 0; from < this->num_states; from++)
        {
            numericallyIntegrateProcess( start_age, end_age, this->transition_prob_matrices[j][from]);
        }
    }
}

/**
 * Wrapper function for the ODE time stepper function.
 */
void BranchHeterogeneousMorphospeciationProcess::numericallyIntegrateProcess(double begin_age, double end_age, state_type &transition_probs) const
{
    typedef boost::numeric::odeint::runge_kutta_dopri5< state_type > stepper_type;
    boost::numeric::odeint::integrate_adaptive( make_controlled( 1E-7, 1E-7, stepper_type() ) , *this , transition_probs, begin_age , end_age , dt );

    // catch negative extinction probabilities that can result from
    // rounding errors in the ODE stepper
    for (size_t i = 0; i < this->num_states; ++i)
    {
        // Sebastian: The likelihoods here are probability densities (not log-transformed).
        // These are densities because they are multiplied by the probability density of the speciation event happening.
        transition_probs[i] = ( transition_probs[i] < 0.0 ? 0.0 : transition_probs[i] );

    }
    
}


void BranchHeterogeneousMorphospeciationProcess::operator()(const state_type &x, state_type &dxdt, const double t)
{
    // catch negative extinction probabilities that can result from
    // rounding errors in the ODE stepper
    state_type safe_x = x;
    for (size_t i = 0; i < num_states; ++i)
    {
        safe_x[i] = ( x[i] < 0.0 ? 0.0 : x[i] );
    }

    double age = 0.0;
    for (size_t i = 0; i < num_states; ++i)
    {

        // no event
        double no_event_rate = site_rates[tmp_allocation] * (lambda->getValue() + lambda_a->getValue()) + mu->getValue() + psi->getValue();

        dxdt[i] = - no_event_rate * safe_x[i];

        dxdt[i] += site_rates[tmp_allocation] * lambda->getValue() * p(t) * safe_x[i];

        // anagenetic state change
        for (size_t j = 0; j < num_states; ++j)
        {
            double total = 0.0;

            if ( i != j )
            {
                total += safe_x[j];
            }

            dxdt[i] += total * site_rates[tmp_allocation] * (lambda->getValue() * p(t) + lambda_a->getValue()) / double(num_states - 1);
        }

        dxdt[i] /= double(num_sites);

    } // end for num_states

}


void BranchHeterogeneousMorphospeciationProcess::updateMeanSiteRate(void) const
{
    mean_site_rate = 0.0;

    for(size_t i = 0; i < this->num_sites; i++)
    {
        mean_site_rate += this->site_rates[rate_allocation[active_rate_allocation][i]];
    }

    mean_site_rate /= double(this->num_sites);
}


/**
 * p(t)
 */
double BranchHeterogeneousMorphospeciationProcess::p( double dt ) const
{
    if ( dt == 0) return 1.0;

    // get the parameters
    double b = lambda->getValue() * mean_site_rate;
    double d = mu->getValue();
    double f = psi->getValue();
    double r = rho->getValue();

    double diff = b - d - f;

    double A = sqrt( diff*diff + 4.0*b*f);
    double B = - (diff - 2 * b * r) / A;

    double ln_e = -A*dt;

    double tmp = (1.0 + B) + exp(ln_e)*(1.0 - B);

    return (b + d + f - A * ((1.0+B)-exp(ln_e)*(1.0-B))/tmp)/(2.0*b);
}


/**
 * Resize various vectors depending on the current number of nodes.
 */
void BranchHeterogeneousMorphospeciationProcess::resizeVectors()
{
    size_t num_nodes = value->getNumberOfNodes();

    active_likelihood = std::vector<bool>(num_nodes, false);
    changed_nodes = std::vector<bool>(num_nodes, false);
    dirty_nodes = std::vector<bool>(num_nodes, true);
    node_partial_likelihoods = std::vector<std::vector<std::vector<std::vector<std::vector<double> > > > >(num_nodes, std::vector<std::vector<std::vector<std::vector<double> > > >(2, std::vector<std::vector<std::vector<double> > >(num_site_rates, std::vector<std::vector<double> >(num_sites, std::vector<double>(num_states, 0.0)))));
    scaling_factors = std::vector<std::vector<std::vector<double> > >(num_nodes, std::vector<double>(2, std::vector<double>(num_sites, 0.0) ) );
}
