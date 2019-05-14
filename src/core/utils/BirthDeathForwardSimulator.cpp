#include "BirthDeathForwardSimulator.h"

#include "DistributionExponential.h"
#include "RandomNumberGenerator.h"
#include "RandomNumberFactory.h"


#include "TopologyNode.h"

using namespace RevBayesCore;


BirthDeathForwardSimulator::BirthDeathForwardSimulator( void )
{
    
}


Tree* BirthDeathForwardSimulator::simulateTreeConditionTime(double start_age)
{
    size_t NUM_TIME_INTERVALS = timeline.size();
    size_t NUM_CATEGORIES = lambda[0].size();
    
    RandomNumberGenerator *rng = GLOBAL_RNG;
    
    // we define here a set of all sampled nodes
    // this set will be necessary for sampled fossils
    std::set<TopologyNode*> sampled_nodes;
    std::set<TopologyNode*> extinct_not_sampled_nodes;

    std::vector< std::set<TopologyNode*> > active_nodes_in_actegories = std::vector< std::set<TopologyNode*> >( NUM_CATEGORIES, std::set<TopologyNode*>() );
//    std::map<TopologyNode*, size_t> node_categories;
    
    size_t current_time_index = 0;
    while ( current_time_index < NUM_TIME_INTERVALS && timeline[current_time_index] > start_age )
    {
        ++current_time_index;
    }
    
    // draw the index of the root category
    size_t root_cat_index = 0;
    double u = rng->uniform01();
    while ( root_cat_index < NUM_CATEGORIES && u > root_cat_probability[root_cat_index] )
    {
        u -= root_cat_probability[root_cat_index];
        ++root_cat_index;
    }
    
    // for now we assume that we always start with the root
    // we should make the an argument
    size_t current_num_active_nodes = 2;
    TopologyNode *root = new TopologyNode();
    root->setAge( start_age );
    // left child
    TopologyNode *left = new TopologyNode();
    root->addChild( left );
    left->setParent( root );
    // right child
    TopologyNode *right = new TopologyNode();
    root->addChild( right );
    right->setParent( root );
    
    // now add the two children to the active nodes
    active_nodes_in_actegories[root_cat_index].insert( left  );
    active_nodes_in_actegories[root_cat_index].insert( right );
    
    std::vector<double> current_lambda   = lambda[current_time_index];
    std::vector<double> current_mu       = mu[current_time_index];
    std::vector<double> current_phi      = phi[current_time_index];
    std::vector<double> current_r        = r[current_time_index];
    
    double current_lambda_total     = 0;
    double current_mu_total         = 0;
    double current_phi_total        = 0;
    for ( size_t i=0; i<NUM_CATEGORIES; ++i)
    {
        double num_lineages_in_category = double( active_nodes_in_actegories[i].size() );
        current_lambda_total    += num_lineages_in_category * current_lambda[i];
        current_mu_total        += num_lineages_in_category * current_mu[i];
        current_phi_total       += num_lineages_in_category * current_phi[i];
    }
    double current_rate_total = current_lambda_total + current_mu_total + current_phi_total;
    
    double current_age = start_age;
    while ( current_age > 0.0 && current_num_active_nodes > 0 )
    {
        // draw the next event
        double next_age = current_age - RbStatistics::Exponential::rv(current_rate_total, *rng);
        
        // check if the next event is passed the current epoch
        if ( next_age < timeline[current_time_index] )
        {
            double event_age = timeline[current_time_index];
             // now we perform the different events
            
            for ( size_t i=0; i<NUM_CATEGORIES; ++i )
            {
                // (1) a burst event
                double this_burst_prob = Lambda[current_time_index][i];
                if ( this_burst_prob > 0.0 )
                {
                    std::set<TopologyNode*> old_active_nodes_in_category = active_nodes_in_actegories[i];
                    for ( std::set<TopologyNode*>::const_iterator it=old_active_nodes_in_category.begin(); it!=old_active_nodes_in_category.end(); ++it)
                    {
                        u = rng->uniform01();
                        if ( u < this_burst_prob )
                        {
                            TopologyNode *this_node = *it;
                            this_node->setAge( event_age );
                            // left child
                            TopologyNode *left = new TopologyNode();
                            this_node->addChild( left );
                            left->setParent( this_node );
                            // right child
                            TopologyNode *right = new TopologyNode();
                            this_node->addChild( right );
                            right->setParent( this_node );
                            
                            // update the active node set
                            active_nodes_in_actegories[i].erase( this_node );
                            active_nodes_in_actegories[i].insert( left );
                            active_nodes_in_actegories[i].insert( right );

                        } // end-if there was a burst speciation event for this node

                    } // end-for all nodes in this rate category
                    
                } // end-if this category has a non-zero burst probability
                
                
                // (2) a mass-extinction event
                double this_mass_extinction_prob = Mu[current_time_index][i];
                if ( this_mass_extinction_prob < 1.0 )
                {
                    std::set<TopologyNode*> old_active_nodes_in_category = active_nodes_in_actegories[i];
                    for ( std::set<TopologyNode*>::const_iterator it=old_active_nodes_in_category.begin(); it!=old_active_nodes_in_category.end(); ++it)
                    {
                        u = rng->uniform01();
                        if ( u < this_mass_extinction_prob )
                        {
                            TopologyNode *this_node = *it;
                            this_node->setAge( event_age );
                            
                            // update the active node set
                            active_nodes_in_actegories[i].erase( this_node );
                            
                            // remember this node to prune later
                            extinct_not_sampled_nodes.insert( this_node );
                            
                        } // end-if there was a mass-extinction event for this node
                        
                    } // end-for all nodes in this rate category
                    
                } // end-if this category has a non-zero mass-extinction probability
                
                
                // (3) a sampling event
                double this_sampling_prob = Phi[current_time_index][i];
                if ( this_sampling_prob > 0.0 )
                {
                    double this_sampling_extinction_prob = R[current_time_index][i];
                    std::set<TopologyNode*> old_active_nodes_in_category = active_nodes_in_actegories[i];
                    for ( std::set<TopologyNode*>::const_iterator it=old_active_nodes_in_category.begin(); it!=old_active_nodes_in_category.end(); ++it)
                    {
                        u = rng->uniform01();
                        if ( u < this_sampling_prob )
                        {
                            TopologyNode *this_node = *it;
                            this_node->setAge( event_age );
                            
                            // store this node
                            sampled_nodes.insert( this_node );
                            
                            // simulate also if this sampling lead to a death event (i.e., treatment)
                            u = rng->uniform01();
                            if ( u < this_sampling_extinction_prob )
                            {
                                // update the active node set
                                active_nodes_in_actegories[i].erase( this_node );
                            }
                            
                        } // end-if there was a sampling event for this node
                        
                    } // end-for all nodes in this rate category
                    
                } // end-if this category has a non-zero sampling probability
                
            } // end-for over all categories
            
            // update the age
            current_age = event_age;
            
            // update the interval index
            ++current_time_index;
            
            if ( current_time_index < NUM_TIME_INTERVALS )
            {
                // update all the rate summaries
                current_lambda   = lambda[current_time_index];
                current_mu       = mu[current_time_index];
                current_phi      = phi[current_time_index];
                current_r        = r[current_time_index];
            
                current_num_active_nodes = 0;
            
                current_lambda_total     = 0;
                current_mu_total         = 0;
                current_phi_total        = 0;
                for ( size_t i=0; i<NUM_CATEGORIES; ++i)
                {
                    current_num_active_nodes += active_nodes_in_actegories[i].size();
                
                    double num_lineages_in_category = double( active_nodes_in_actegories[i].size() );
                    current_lambda_total    += num_lineages_in_category * current_lambda[i];
                    current_mu_total        += num_lineages_in_category * current_mu[i];
                    current_phi_total       += num_lineages_in_category * current_phi[i];
                }
                current_rate_total = current_lambda_total + current_mu_total + current_phi_total;
            }
            
        } // end-if the time passed the epoch boundary
        else
        {
            
            // check which type of event we had
            u = rng->uniform01();
            if ( u < (current_lambda_total)/current_rate_total )
            {
                // it was a speciation event
                
                u = rng->uniform01();
                for ( size_t i=0; i<NUM_CATEGORIES; ++i)
                {
                    double num_lineages_in_category = double( active_nodes_in_actegories[i].size() );
                    double this_cat_lambda = current_lambda[i];
                    if ( u < (num_lineages_in_category*this_cat_lambda)/current_lambda_total )
                    {
                        size_t this_node_index = size_t( floor(u*num_lineages_in_category) );
                        std::set<TopologyNode*>::const_iterator it = active_nodes_in_actegories[i].begin();
                        std::advance(it, this_node_index);
                        
                        TopologyNode *this_node = *it;
                        this_node->setAge( next_age );
                        // left child
                        TopologyNode *left = new TopologyNode();
                        this_node->addChild( left );
                        left->setParent( this_node );
                        // right child
                        TopologyNode *right = new TopologyNode();
                        this_node->addChild( right );
                        right->setParent( this_node );
                        
                        // update the active node set
                        active_nodes_in_actegories[i].erase( this_node );
                        active_nodes_in_actegories[i].insert( left );
                        active_nodes_in_actegories[i].insert( right );
                        
                        // update counters
                        ++current_num_active_nodes;
                        current_lambda_total    += this_cat_lambda;
                        current_mu_total        += current_mu[i];
                        current_phi_total       += current_phi[i];
                        current_rate_total      += this_cat_lambda + current_mu[i] + current_phi[i];
                        
                        // now stop the loop
                        break;
                    }
                    else
                    {
                        u -= (num_lineages_in_category*this_cat_lambda)/current_lambda_total;
                    } // end-if-else the speciation event was in this category
                    
                } // end-for loop over all categories
                
            } // end-if it was a speciation event
            else if ( u < (current_lambda_total+current_mu_total)/current_rate_total )
            {
                // it was an extinction event
                
                u = rng->uniform01();
                for ( size_t i=0; i<NUM_CATEGORIES; ++i)
                {
                    double num_lineages_in_category = double( active_nodes_in_actegories[i].size() );
                    double this_cat_mu = current_mu[i];
                    if ( u < (num_lineages_in_category*this_cat_mu)/current_mu_total )
                    {
                        size_t this_node_index = size_t( floor(u*num_lineages_in_category) );
                        std::set<TopologyNode*>::const_iterator it = active_nodes_in_actegories[i].begin();
                        std::advance(it, this_node_index);
                        
                        TopologyNode *this_node = *it;
                        this_node->setAge( next_age );
                        
                        // update the active node set
                        active_nodes_in_actegories[i].erase( this_node );
                        
                        // remember this node to prune later
                        extinct_not_sampled_nodes.insert( this_node );
                        
                        // update counters
                        --current_num_active_nodes;
                        current_lambda_total    -= current_lambda[i];
                        current_mu_total        -= this_cat_mu;
                        current_phi_total       -= current_phi[i];
                        current_rate_total      -= (current_lambda[i] + this_cat_mu + current_phi[i]);
                        
                        // now stop the loop
                        break;
                    }
                    else
                    {
                        u -= (num_lineages_in_category*this_cat_mu)/current_mu_total;
                    } // end-if-else the speciation event was in this category
                    
                } // end-for loop over all categories
                
            } // end-if it was an extinction event
            else if ( u < (current_lambda_total+current_mu_total+current_phi_total)/current_rate_total )
            {
                // it was a sampling event
                
                u = rng->uniform01();
                for ( size_t i=0; i<NUM_CATEGORIES; ++i)
                {
                    double num_lineages_in_category = double( active_nodes_in_actegories[i].size() );
                    double this_cat_phi = current_phi[i];
                    double this_cat_r = current_r[i];
                    if ( u < (num_lineages_in_category*this_cat_phi)/current_mu_total )
                    {
                        size_t this_node_index = size_t( floor(u*num_lineages_in_category) );
                        std::set<TopologyNode*>::const_iterator it = active_nodes_in_actegories[i].begin();
                        std::advance(it, this_node_index);
                        
                        TopologyNode *this_node = *it;
                        this_node->setAge( next_age );
                        
                        // the current node to our set of sampled nodes
                        sampled_nodes.insert( this_node );
                        
                        // now draw if this node died during sampling (i.e., treatment)
                        u = rng->uniform01();
                        if ( u < this_cat_r )
                        {
                            // update the active node set
                            active_nodes_in_actegories[i].erase( this_node );
                        
                            // update counters
                            --current_num_active_nodes;
                            current_lambda_total    -= current_lambda[i];
                            current_mu_total        -= current_mu[i];
                            current_phi_total       -= this_cat_phi;
                            current_rate_total      -= (current_lambda[i] + current_mu[i] + this_cat_phi);
                        } // end-of this node died during sampling
                        
                        // now stop the loop
                        break;
                    }
                    else
                    {
                        u -= (num_lineages_in_category*this_cat_phi)/current_phi_total;
                    } // end-if-else the speciation event was in this category
                    
                } // end-for loop over all categories
                
            } // end-if it was a sampling event
            else
            {
                // this should not happen
                throw RbException("Problem in birth-death forward simulator: could not determine which type of event happened");
            }
            
            current_age = next_age;
        }
                
    } // end-while we still have time and nodes to simulate forward
    
    
    // set the ages of all remaining active nodes to 0.0
    for ( size_t i=0; i<NUM_CATEGORIES; ++i)
    {
        const std::set<TopologyNode*> &active_nodes_in_category = active_nodes_in_actegories[i];
        for ( std::set<TopologyNode*>::const_iterator it=active_nodes_in_category.begin(); it!=active_nodes_in_category.end(); ++it)
        {
            TopologyNode *this_node = *it;
            this_node->setAge( 0.0 );
        }
    }
    

    // now prune away all the extinct nodes
    if ( current_num_active_nodes > 0 )
    {
        for ( std::set<TopologyNode*>::const_iterator it=extinct_not_sampled_nodes.begin(); it!=extinct_not_sampled_nodes.end(); ++it)
        {
            TopologyNode *this_node = *it;
        
            if ( this_node->isRoot() == false ) // this should truly never happen, only if there was no survivor
            {
                TopologyNode &parent = this_node->getParent();
                TopologyNode *sibling = &parent.getChild( 0 );
                if ( sibling == this_node ) sibling = &parent.getChild( 1 );
            
                if ( parent.isRoot() == false )
                {
                    TopologyNode &grandparent = parent.getParent();
                    TopologyNode *uncle = &grandparent.getChild( 0 );
                    if ( uncle == &parent ) uncle = &grandparent.getChild( 1 );
                    
                    parent.removeChild( sibling );
                    grandparent.removeChild( &parent );
                    
                    sibling->setParent( &grandparent );
                    grandparent.addChild( sibling );
                    
                    delete &parent; // this will also delete this child node
                }
                else
                {
                    parent.removeChild( sibling );
                    root = sibling;
                    
                    delete &parent;
                }
                
            }
            
        }
        
    }
    
    // build and return the tree
    Tree *my_tree = new Tree();
    my_tree->setRoot(root, true);
    
    return my_tree;
}


void BirthDeathForwardSimulator::setBurstProbability( const std::vector<std::vector< double > > &l )
{
    Lambda = l;
}


void BirthDeathForwardSimulator::setExtinctionRate( const std::vector<std::vector< double > > &m )
{
    mu = m;
}


void BirthDeathForwardSimulator::setMassExtinctionProbability( const std::vector<std::vector< double > > &m )
{
    Mu = m;
}


void BirthDeathForwardSimulator::setRootCategoryProbabilities( const std::vector<double> &p )
{
    root_cat_probability = p;
}



void BirthDeathForwardSimulator::setSamplingProbability( const std::vector<std::vector< double > > &p )
{
    Phi = p;
}


void BirthDeathForwardSimulator::setSamplingExtinctionProbability( const std::vector<std::vector< double > > &r )
{
    R = r;
}


void BirthDeathForwardSimulator::setSamplingRate( const std::vector<std::vector< double > > &p )
{
    phi = p;
}


void BirthDeathForwardSimulator::setSamplingExtinctionRate( const std::vector<std::vector< double > > &r )
{
    this->r = r;
}


void BirthDeathForwardSimulator::setSpeciationRate( const std::vector<std::vector< double > > &l )
{
    lambda = l;
}


void BirthDeathForwardSimulator::setTimeline( const std::vector< double > &t )
{
    timeline = t;
}
