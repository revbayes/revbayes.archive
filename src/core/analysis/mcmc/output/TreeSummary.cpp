#include "ProgressBar.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathLogic.h"
#include "RbVectorUtilities.h"
#include "Sample.h"
#include "StringUtilities.h"
#include "TopologyNode.h"
#include "TreeSummary.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <iomanip>
#include <vector>
#include <limits>
#include <cmath>


using namespace RevBayesCore;

TreeSummary::TreeSummary( void ) :
    clock( true ),
    rooted( true ),
    summarized( false ),
    trace( false ),
    use_tree_trace( false )
{
    setBurnin( 0 );
}

TreeSummary::TreeSummary( const TraceTree &t ) :
    clock( t.isClock() ),
    rooted( t.objectAt(0).isRooted() ),
    summarized( false ),
    trace( t ),
    use_tree_trace( true ),
    cladeAges( CladeComparator(rooted) ),
    conditionalCladeAges( CladeComparator(rooted) )
{
    setBurnin( t.getBurnin() );
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */

TreeSummary* TreeSummary::clone(void) const
{

    return new TreeSummary(*this);
}


/**
 *
 * Helper function for ancestralStateTree() and cladoAncestralStateTree() that collects and sorts joint ancestral state samples.
 *
 */
void TreeSummary::collectJointAncestralStateSamples(std::vector<AncestralStateTrace> &ancestral_state_traces, int b, int site, size_t num_sampled_states, size_t num_sampled_trees, Tree &final_summary_tree, const std::vector<TopologyNode*> &summary_nodes, std::vector<std::vector<double> > &pp, std::vector<std::vector<std::string> > &end_states, std::vector<std::vector<std::string> > &start_states, bool clado, ProgressBar &progress, bool verbose)
{
    // loop through all ancestral state samples
    
        // check if sampled tree topology matches the summary tree
    
        // if so collect sampled states
    
    // done looping through samples, now sort them
    
    // save the top 3 in start/end_states and pp vectors
}


/**
 *
 * Helper function for ancestralStateTree() and cladoAncestralStateTree() that traverses the tree from root to tips collecting ancestral state samples.
 *
 */
void TreeSummary::recursivelyCollectAncestralStateSamples(size_t node_index, std::string map_parent_state, bool root, bool conditional, std::vector<AncestralStateTrace> &ancestral_state_traces, int b, int site, size_t num_sampled_states, size_t num_sampled_trees, Tree &final_summary_tree, const std::vector<TopologyNode*> &summary_nodes, std::vector<std::vector<double> > &pp, std::vector<std::vector<std::string> > &end_states, std::vector<std::vector<std::string> > &start_states, bool clado, ProgressBar &progress, size_t &num_finished_nodes, bool verbose)
{
    
    size_t parent_node_index = 0;
    if (root == false)
    {
        parent_node_index = summary_nodes[node_index]->getParent().getIndex();
    }
    
    size_t sample_clade_index;
    size_t parent_sample_clade_index;
    AncestralStateTrace ancestral_state_trace_end_state;
    AncestralStateTrace ancestral_state_trace_start_1;
    AncestralStateTrace ancestral_state_trace_start_2;
    AncestralStateTrace parent_ancestral_state_trace;
    bool trace_found_end_state = false;
    bool trace_found_start_1 = false;
    bool trace_found_start_2 = false;
    bool parent_trace_found = false;
    
    size_t num_samples = 0;
    
    // loop through all the ancestral state samples
    for (size_t j = burnin; j < num_sampled_states; ++j)
    {
        if ( verbose == true )
        {
            progress.update( num_finished_nodes * num_sampled_states + num_sampled_states * (j - burnin) / (num_sampled_states - burnin) );
        }
        
        // if necessary, get the sampled tree from the tree trace
        const Tree &sample_tree = (use_tree_trace) ? trace.objectAt( j ) : final_summary_tree;
        const TopologyNode& sample_root = sample_tree.getRoot();
        
        if ( use_tree_trace == true )
        {
            // check if the clade in the summary tree is also in the sampled tree
            sample_clade_index = sample_root.getCladeIndex( summary_nodes[node_index] );
            parent_sample_clade_index = sample_root.getCladeIndex( summary_nodes[parent_node_index] );
            
            // then we must find the ancestral state traces for this sampled node
            trace_found_end_state = false;
            trace_found_start_1 = false;
            trace_found_start_2 = false;
            parent_trace_found = false;
        }
        else
        {
            sample_clade_index = summary_nodes[node_index]->getIndex();
            parent_sample_clade_index = parent_node_index;
        }
        
        if ( RbMath::isFinite( sample_clade_index ) == true )
        {
            size_t sample_clade_index_child_1 = 0;
            size_t sample_clade_index_child_2 = 0;
            
            if ( !summary_nodes[node_index]->isTip() && clado == true )
            {
                sample_clade_index_child_1 = sample_root.getCladeIndex( &summary_nodes[node_index]->getChild(0) );
                sample_clade_index_child_2 = sample_root.getCladeIndex( &summary_nodes[node_index]->getChild(1) );
            }
            
            // if necessary find the AncestralStateTraces for the sampled node
            if ( trace_found_end_state == false )
            {
                for (size_t k = 0; k < ancestral_state_traces.size(); ++k)
                {
                    // if we have an ancestral state trace from an anagenetic-only process
                    if (ancestral_state_traces[k].getParameterName() == StringUtilities::toString(sample_clade_index + 1))
                    {
                        ancestral_state_trace_end_state = ancestral_state_traces[k];
                        trace_found_end_state = true;
                        break;
                    }
                    // if we have an ancestral state trace from a cladogenetic process
                    if (ancestral_state_traces[k].getParameterName() == "end_" + StringUtilities::toString(sample_clade_index + 1))
                    {
                        ancestral_state_trace_end_state = ancestral_state_traces[k];
                        trace_found_end_state = true;
                        break;
                    }
                    
                    // find start state traces if necessary
                    if ( clado == true && !summary_nodes[node_index]->isTip() )
                    {
                        if (ancestral_state_traces[k].getParameterName() == "start_" + StringUtilities::toString(sample_clade_index_child_1 + 1))
                        {
                            ancestral_state_trace_start_1 = ancestral_state_traces[k];
                            trace_found_start_1 = true;
                        }
                        
                        if (ancestral_state_traces[k].getParameterName() == "start_" + StringUtilities::toString(sample_clade_index_child_2 + 1))
                        {
                            ancestral_state_trace_start_2 = ancestral_state_traces[k];
                            trace_found_start_2 = true;
                        }
                    }
                    else
                    {
                        trace_found_start_1 = true;
                        trace_found_start_2 = true;
                    }
                    
                    if ( clado == false && trace_found_end_state )
                    {
                        break;
                    }
                    
                    if ( clado == true && trace_found_end_state && trace_found_start_1 && trace_found_start_2)
                    {
                        break;
                    }
                }
            }
            // if we are conditioning on the parent's state we must get the corresponding sample from the parent
            if ( conditional == true && root == false && parent_trace_found == false )
            {
                for (size_t k = 0; k < ancestral_state_traces.size(); ++k)
                {
                    // if we have an ancestral state trace from an anagenetic-only process
                    if (ancestral_state_traces[k].getParameterName() == StringUtilities::toString(parent_sample_clade_index + 1))
                    {
                        parent_ancestral_state_trace = ancestral_state_traces[k];
                        parent_trace_found = true;
                        break;
                    }
                    // if we have an ancestral state trace from a cladogenetic process
                    if (ancestral_state_traces[k].getParameterName() == "end_" + StringUtilities::toString(parent_sample_clade_index + 1))
                    {
                        parent_ancestral_state_trace = ancestral_state_traces[k];
                        parent_trace_found = true;
                        break;
                    }
                }
            }
            
            // get the sampled ancestral state for this iteration
            const std::vector<std::string>& ancestral_state_vector_end = ancestral_state_trace_end_state.getValues();
            std::string ancestral_state_end = getSiteState( ancestral_state_vector_end[j], site );
            
            // get the sampled ancestral state from the parent node
            bool count_sample = false;
            std::string sampled_parent_state = "";
            if ( conditional == true && root == false && parent_trace_found == true )
            {
                const std::vector<std::string>& parent_ancestral_state_vector = parent_ancestral_state_trace.getValues();
                sampled_parent_state = getSiteState( parent_ancestral_state_vector[j], site );
                
                // condition on the parent state?
                if ( sampled_parent_state == map_parent_state )
                {
                    count_sample = true;
                }
            }
                
            if ( conditional == false || root == true )
            {
                count_sample = true;
            }
            
            // finally add the sample to our vectors of samples
            if ( count_sample == true )
            {
                if ( clado == true && summary_nodes[node_index]->isTip() == false )
                {
                    std::vector<std::string> ancestral_state_trace_start_1_vector = ancestral_state_trace_start_1.getValues();
                    std::string ancestral_state_start_1 = getSiteState( ancestral_state_trace_start_1_vector[j], site );
                    
                    std::vector<std::string> ancestral_state_trace_start_2_vector = ancestral_state_trace_start_2.getValues();
                    std::string ancestral_state_start_2 = getSiteState( ancestral_state_trace_start_2_vector[j], site );
                    
                    size_t child1 = summary_nodes[node_index]->getChild(0).getIndex();
                    size_t child2 = summary_nodes[node_index]->getChild(1).getIndex();
                    
                    bool state_found = false;
                    int k = 0;
                    for (; k < pp[node_index].size(); k++)
                    {
                        if (end_states[node_index][k] == ancestral_state_end && start_states[child1][k] == ancestral_state_start_1 && start_states[child2][k] == ancestral_state_start_2)
                        {
                            state_found = true;
                            break;
                        }
                    }
                    // update the pp and states vectors
                    if ( state_found == false )
                    {
                        pp[node_index].push_back(1.0);
                        end_states[node_index].push_back( ancestral_state_end );
                        start_states[child1].push_back( ancestral_state_start_1 );
                        start_states[child2].push_back( ancestral_state_start_2 );
                    }
                    else
                    {
                        pp[node_index][k] += 1.0;
                    }
                }
                else
                {
                    bool state_found = false;
                    size_t k = 0;
                    for (; k < pp[node_index].size(); k++)
                    {
                        if ( end_states[node_index][k] == ancestral_state_end )
                        {
                            state_found = true;
                            break;
                        }
                    }
                    // update the pp and states vectors
                    if ( state_found == false )
                    {
                        pp[node_index].push_back(1.0);
                        end_states[node_index].push_back(ancestral_state_end);
                    }
                    else
                    {
                        pp[node_index][k] += 1.0;
                    }
                }
                num_samples += 1;
            }
        }
    }
    
    // scale posteriors
    for (size_t i = 0; i < pp[node_index].size(); i++)
    {
        pp[node_index][i] /= num_samples;
    }
    
    std::string map_state = "";
    double max_pp = 0.0;
    if ( conditional == true )
    {
        // find the MAP state
        for (size_t i = 0; i < end_states[node_index].size(); i++)
        {
            if (pp[node_index][i] > max_pp)
            {
                map_state = end_states[node_index][i];
                max_pp = pp[node_index][i];
            }
        }
    }
    
    // update nodes finished for the progress bar
    num_finished_nodes += 1;
    
    // recurse through tree
    std::vector<int> children_indices = summary_nodes[node_index]->getChildrenIndices();
    for (size_t i = 0; i < children_indices.size(); i++)
    {
        recursivelyCollectAncestralStateSamples(children_indices[i], map_state, false, conditional, ancestral_state_traces, b, site, num_sampled_states, num_sampled_trees, final_summary_tree, summary_nodes, pp, end_states, start_states, clado, progress, num_finished_nodes, verbose);
    }
    
}


/*
 * This method calculates ancestral character states for the nodes on the input_summary_tree. When summary_stat = MAP, it annotates
 * the summary tree with the posterior probabilities of the 3 most probable states. The method requires a vector of traces containing
 * sampled ancestral states, and optionally works with a trace containing sampled trees that correspond to the
 * ancestral state samples, enabling ancestral states to be estimated over a distribution of trees. In this case the annotated
 * posterior probability for a given character state is the probability of the node existing times the probability of the 
 * node being in the character state (see Pagel et al. 2004).
 *
 * This method summarizes both conditional and marginal ancestral states. When conditional = false the method summarizes marginal
 * ancestral states. conditional = true should only be used if the states were sampled using the JointConditionalAncestralStateMonitor.
 * When summarizing conditional ancestral states the posterior probabilities of each state are conditioned on the parent node being
 * in the MAP state.
 */
Tree* TreeSummary::ancestralStateTree(const Tree &input_summary_tree, std::vector<AncestralStateTrace> &ancestralstate_traces, int b, std::string summary_stat, int site, bool conditional, bool joint, bool verbose )
{

    if ( summary_stat != "MAP" && (conditional == true || joint == true) )
    {
        throw RbException("Invalid reconstruction type: mean ancestral states can only be calculated with a marginal reconstruction.");
    }

    // get the number of ancestral state samples and the number of tree samples
    size_t num_sampled_states = ancestralstate_traces[0].getValues().size();
    size_t num_sampled_trees;
    if (use_tree_trace == false)
    {
        // the ancestral states were sampled over the same tree
        num_sampled_trees = 1;
    }
    else
    {
        // the ancestral states were sampled over different trees each iteration
        num_sampled_trees = trace.size();
    }
    
    setBurnin(b);
    if ( b >= num_sampled_states )
    {
        throw RbException("Burnin size is too large for the ancestral state trace.");
    }
    
    if ( use_tree_trace == true &&  num_sampled_trees != num_sampled_states )
    {
        throw RbException("The tree trace and the ancestral state trace must contain the same number of samples.");
    }
    
    std::stringstream ss;
    ss << "Compiling " << summary_stat << " ancestral states from " << num_sampled_states << " samples in the ancestral state trace, using a burnin of " << burnin << " samples.\n";
    RBOUT(ss.str());
    
    RBOUT("Calculating ancestral state posteriors...\n");
    
    // allocate memory for the new summary tree
    Tree* final_summary_tree = new Tree( input_summary_tree );
    
    // 2-d vectors to keep the data (posteriors and ancestral states) of the input_summary_tree nodes: [node][data]
    const std::vector<TopologyNode*> &summary_nodes = final_summary_tree->getNodes();
    std::vector<std::vector<double> > pp( summary_nodes.size(), std::vector<double>() );
    std::vector<std::vector<std::string> > states( summary_nodes.size(), std::vector<std::string>() );
    
    size_t num_finished_nodes = 0;
    ProgressBar progress = ProgressBar( summary_nodes.size() * num_sampled_states, 0 );
    if ( verbose == true )
    {
        progress.start();
    }
    
    if ( joint == true )
    {
        // collect and sort the joint samples
        collectJointAncestralStateSamples(ancestralstate_traces, b, site, num_sampled_states, num_sampled_trees, *final_summary_tree, summary_nodes, pp, states, states, false, progress, verbose);
    }
    else
    {
        // recurse through summary tree and collect ancestral state samples
        size_t node_index = final_summary_tree->getRoot().getIndex();
        recursivelyCollectAncestralStateSamples(node_index, "", true, conditional, ancestralstate_traces, b, site, num_sampled_states, num_sampled_trees, *final_summary_tree, summary_nodes, pp, states, states, false, progress, num_finished_nodes, verbose);
    }

    if ( verbose == true )
    {
        progress.finish();
    }
    
    // summarize ancestral states
    if (summary_stat == "MAP")
    {
        // find the 3 most probable ancestral states for each node and add them to the tree as parameters
        std::vector<std::string*> anc_state_1;
        std::vector<std::string*> anc_state_2;
        std::vector<std::string*> anc_state_3;
        std::vector<double> anc_state_1_pp;
        std::vector<double> anc_state_2_pp;
        std::vector<double> anc_state_3_pp;
        std::vector<double> anc_state_other_pp;

        std::vector<double> posteriors;

        for (int i = 0; i < summary_nodes.size(); i++)
        {

            if ( summary_nodes[i]->isTip() )
            {

                posteriors.push_back(1.0);
                
                anc_state_1.push_back(new std::string(states[i][0]));
                anc_state_1_pp.push_back(1.0);
                anc_state_2.push_back(new std::string("NA"));
                anc_state_2_pp.push_back(0.0);
                anc_state_3.push_back(new std::string("NA"));
                anc_state_3_pp.push_back(0.0);
                
            }
            else
            {
                
                double state1_pp = 0.0;
                double state2_pp = 0.0;
                double state3_pp = 0.0;
                double other_pp = 0.0;
                double total_node_pp = 0.0;
                
                std::string state1 = "";
                std::string state2 = "";
                std::string state3 = "";
                
                // loop through all states for this node
                for (int j = 0; j < pp[i].size(); j++)
                {
                    total_node_pp += pp[i][j];
                    if (pp[i][j] > state1_pp)
                    {
                        state3_pp = state2_pp;
                        state2_pp = state1_pp;
                        state1_pp = pp[i][j];
                        state3 = state2;
                        state2 = state1;
                        state1 = states[i][j];
                    }
                    else if (pp[i][j] > state2_pp)
                    {
                        state3_pp = state2_pp;
                        state2_pp = pp[i][j];
                        state3 = state2;
                        state2 = states[i][j];
                    }
                    else if (pp[i][j] > state3_pp)
                    {
                        state3_pp = pp[i][j];
                        state3 = states[i][j];
                    }
                }
                
                posteriors.push_back(total_node_pp);
                
                if (state1_pp > 0.0001)
                {
                    anc_state_1.push_back(new std::string(state1));
                    anc_state_1_pp.push_back(state1_pp);
                }
                else
                {
                    anc_state_1.push_back(new std::string("NA"));
                    anc_state_1_pp.push_back(0.0);
                }
                
                if (state2_pp > 0.0001)
                {
                    anc_state_2.push_back(new std::string(state2));
                    anc_state_2_pp.push_back(state2_pp);
                }
                else
                {
                    anc_state_2.push_back(new std::string("NA"));
                    anc_state_2_pp.push_back(0.0);
                }
                
                if (state3_pp > 0.0001)
                {
                    anc_state_3.push_back(new std::string(state3));
                    anc_state_3_pp.push_back(state3_pp);
                }
                else
                {
                    anc_state_3.push_back(new std::string("NA"));
                    anc_state_3_pp.push_back(0.0);
                }
                
                if (other_pp > 0.0001)
                {
                    anc_state_other_pp.push_back(other_pp);
                }
                else
                {
                    anc_state_other_pp.push_back(0.0);
                }
                
            }
        }
        
        final_summary_tree->clearNodeParameters();
        final_summary_tree->addNodeParameter("posterior", posteriors, false);
        final_summary_tree->addNodeParameter("anc_state_1", anc_state_1, false);
        final_summary_tree->addNodeParameter("anc_state_2", anc_state_2, false);
        final_summary_tree->addNodeParameter("anc_state_3", anc_state_3, false);
        final_summary_tree->addNodeParameter("anc_state_1_pp", anc_state_1_pp, false);
        final_summary_tree->addNodeParameter("anc_state_2_pp", anc_state_2_pp, false);
        final_summary_tree->addNodeParameter("anc_state_3_pp", anc_state_3_pp, false);
        final_summary_tree->addNodeParameter("anc_state_other_pp", anc_state_other_pp, false);
    
    }
    else
    {
        // calculate the mean and 95% CI and add to the tree as annotation
        double hpd = 0.95;
        std::vector<double> means( summary_nodes.size(), 0.0 );
        std::vector<double> uppers( summary_nodes.size(), 0.0 );
        std::vector<double> lowers( summary_nodes.size(), 0.0 );
        std::vector<double> posteriors( summary_nodes.size(), 0.0 );
        
        for (int i = 0; i < summary_nodes.size(); i++)
        {
            
            if ( summary_nodes[i]->isTip() )
            {
                posteriors[i] = 1.0;
                means[i] = boost::lexical_cast<double>( states[i][0] );
            }
            else
            {
                double node_pp = 0.0;
                std::vector<double> state_samples;
                
                // loop through all states for this node and collect samples
                for (int j = 0; j < pp[i].size(); j++)
                {
                    node_pp += pp[i][j];
                    state_samples.push_back( boost::lexical_cast<double>( states[i][j] ) );
                }
                posteriors[i] = node_pp;
                
                // calculate mean value
                double samples_sum = std::accumulate(state_samples.begin(), state_samples.end(), 0.0);
                double mean = samples_sum / state_samples.size();
                means[i] = mean;
                
                // sort the samples by frequency and calculate interval
                std::sort(state_samples.begin(), state_samples.end());
                size_t interval_start = ( (1.0 - hpd) / 2.0 ) * state_samples.size();
                size_t interval_end = ( 1.0 - (1.0 - hpd) / 2.0 ) * state_samples.size();
                interval_end = (interval_end >= state_samples.size() ? state_samples.size()-1 : interval_end);
                
                double lower = state_samples[interval_start];
                double upper = state_samples[interval_end];
                
                lowers[i] = lower;
                uppers[i] = upper;
            }
        }
        
        final_summary_tree->clearNodeParameters();
        final_summary_tree->addNodeParameter("posterior", posteriors, true);
        final_summary_tree->addNodeParameter("mean", means, false);
        final_summary_tree->addNodeParameter("lower_95%_CI", lowers, true);
        final_summary_tree->addNodeParameter("upper_95%_CI", uppers, true);
        
    }
    
    return final_summary_tree;
}


/*
 * This method calculates the MAP ancestral character states for the nodes on the input_tree. This method
 * is identical to the ancestralStateTree function except that is calculates the MAP states resulting from 
 * a cladogenetic event, so for each node the MAP state includes the end state and the starting states for 
 * the two daughter lineages.
 */
Tree* TreeSummary::cladoAncestralStateTree(const Tree &input_summary_tree, std::vector<AncestralStateTrace> &ancestralstate_traces, int b, std::string summary_stat, int site, bool conditional, bool joint, bool verbose )
{
    
    if ( summary_stat != "MAP" && (conditional == true || joint == true) )
    {
        throw RbException("Invalid reconstruction type: mean ancestral states can only be calculated with a marginal reconstruction.");
    }
    
    // get the number of ancestral state samples and the number of tree samples
    size_t num_sampled_states = ancestralstate_traces[0].getValues().size();
    size_t num_sampled_trees;
    if (use_tree_trace == false)
    {
        // the ancestral states were sampled over the same tree
        num_sampled_trees = 1;
    }
    else
    {
        // the ancestral states were sampled over different trees each iteration
        num_sampled_trees = trace.size();
    }
    
    setBurnin(b);
    if ( b >= num_sampled_states )
    {
        throw RbException("Burnin size is too large for the ancestral state trace.");
    }
    
    if ( use_tree_trace == true &&  num_sampled_trees != num_sampled_states )
    {
        throw RbException("The tree trace and the ancestral state trace must contain the same number of samples.");
    }
    
    std::stringstream ss;
    ss << "Compiling " << summary_stat << " ancestral states from " << num_sampled_states << " samples in the ancestral state trace, using a burnin of " << burnin << " samples.\n";
    RBOUT(ss.str());
    
    RBOUT("Calculating ancestral state posteriors...\n");
    
    // allocate memory for the new tree
    Tree* final_summary_tree = new Tree(input_summary_tree);
    
    // 2-d vectors to keep the data (posteriors and states) of the inputTree nodes: [node][data]
    const std::vector<TopologyNode*> &summary_nodes = final_summary_tree->getNodes();
    std::vector<std::vector<double> > pp( summary_nodes.size(), std::vector<double>() );
    std::vector<std::vector<std::string> > end_states( summary_nodes.size(), std::vector<std::string>() );
    std::vector<std::vector<std::string> > start_states( summary_nodes.size(), std::vector<std::string>() );
    
    size_t num_finished_nodes = 0;
    ProgressBar progress = ProgressBar( summary_nodes.size() * num_sampled_states, 0 );
    if ( verbose == true )
    {
        progress.start();
    }
    
    if ( joint == true )
    {
        // collect and sort the joint samples
        collectJointAncestralStateSamples(ancestralstate_traces, b, site, num_sampled_states, num_sampled_trees, *final_summary_tree, summary_nodes, pp, end_states, start_states, false, progress, verbose);
    }
    else
    {
        // recurse through summary tree and collect ancestral state samples
        size_t node_index = final_summary_tree->getRoot().getIndex();
        recursivelyCollectAncestralStateSamples(node_index, "", true, conditional, ancestralstate_traces, b, site, num_sampled_states, num_sampled_trees, *final_summary_tree, summary_nodes, pp, end_states, start_states, true, progress, num_finished_nodes, verbose);
    }
    
    if ( verbose == true )
    {
        progress.finish();
    }
    
    // summarize ancestral states
    if (summary_stat == "MAP")
    {
        // find the 3 most probable ancestral states for each node and add them to the tree as annotations
        
        std::vector<std::string*> end_state_1( summary_nodes.size(), new std::string("NA") );
        std::vector<std::string*> end_state_2( summary_nodes.size(), new std::string("NA") );
        std::vector<std::string*> end_state_3( summary_nodes.size(), new std::string("NA") );
        
        std::vector<double> end_state_1_pp( summary_nodes.size(), 0.0 );
        std::vector<double> end_state_2_pp( summary_nodes.size(), 0.0 );
        std::vector<double> end_state_3_pp( summary_nodes.size(), 0.0 );
        std::vector<double> end_state_other_pp( summary_nodes.size(), 0.0 );
        
        std::vector<std::string*> start_state_1( summary_nodes.size(), new std::string("NA") );
        std::vector<std::string*> start_state_2( summary_nodes.size(), new std::string("NA") );
        std::vector<std::string*> start_state_3( summary_nodes.size(), new std::string("NA") );
        
        std::vector<double> start_state_1_pp( summary_nodes.size(), 0.0 );
        std::vector<double> start_state_2_pp( summary_nodes.size(), 0.0 );
        std::vector<double> start_state_3_pp( summary_nodes.size(), 0.0 );
        std::vector<double> start_state_other_pp( summary_nodes.size(), 0.0 );
        
        std::vector<double> posteriors( summary_nodes.size(), 0.0 );
        
        for (int i = 0; i < summary_nodes.size(); i++)
        {
            
            if ( summary_nodes[i]->isTip() )
            {
                end_state_1[i] = new std::string(end_states[i][0]);
                end_state_1_pp[i] = 1.0;
                posteriors[i] = 1.0;
            }
            else
            {
                double total_node_pp = 0.0;
                
                double end_state1_pp = 0.0;
                double end_state2_pp = 0.0;
                double end_state3_pp = 0.0;
                double end_other_pp = 0.0;
                
                std::string end_state1 = "";
                std::string end_state2 = "";
                std::string end_state3 = "";
                
                std::string start_child1_state1 = "";
                std::string start_child1_state2 = "";
                std::string start_child1_state3 = "";
                
                std::string start_child2_state1 = "";
                std::string start_child2_state2 = "";
                std::string start_child2_state3 = "";
                
                size_t child1 = summary_nodes[i]->getChild(0).getIndex();
                size_t child2 = summary_nodes[i]->getChild(1).getIndex();
                
                // loop through all sampled combinations of states for this node and find the
                // 3 with the highest probability
                for (int j = 0; j < pp[i].size(); j++)
                {
                    total_node_pp += pp[i][j];
                    
                    if (pp[i][j] > end_state1_pp)
                    {
                        end_state3_pp = end_state2_pp;
                        end_state2_pp = end_state1_pp;
                        end_state1_pp = pp[i][j];
                        
                        end_state3 = end_state2;
                        end_state2 = end_state1;
                        end_state1 = end_states[i][j];
                        
                        start_child1_state3 = start_child1_state2;
                        start_child1_state2 = start_child1_state1;
                        start_child1_state1 = start_states[child1][j];
                        
                        start_child2_state3 = start_child2_state2;
                        start_child2_state2 = start_child2_state1;
                        start_child2_state1 = start_states[child2][j];
                    }
                    else if (pp[i][j] > end_state2_pp)
                    {
                        end_state3_pp = end_state2_pp;
                        end_state2_pp = pp[i][j];
                        
                        end_state3 = end_state2;
                        end_state2 = end_states[i][j];
                        
                        start_child1_state3 = start_child1_state2;
                        start_child1_state2 = start_states[child1][j];
                        
                        start_child2_state3 = start_child2_state2;
                        start_child2_state2 = start_states[child2][j];
                    }
                    else if (pp[i][j] > end_state3_pp)
                    {
                        end_state3_pp = pp[i][j];
                        end_state3 = end_states[i][j];
                        start_child1_state3 = start_states[child1][j];
                        start_child2_state3 = start_states[child2][j];
                    }
                }
                
                posteriors[i] = total_node_pp;
                
                if (end_state1_pp > 0.0001)
                {
                    end_state_1[i] = new std::string(end_state1);
                    end_state_1_pp[i] = end_state1_pp;
                    
                    start_state_1[child1] = new std::string(start_child1_state1);
                    start_state_1[child2] = new std::string(start_child2_state1);
                    
                    start_state_1_pp[child1] = end_state1_pp;
                    start_state_1_pp[child2] = end_state1_pp;
                    
                } else
                {
                    end_state_1[i] = new std::string("NA");
                    end_state_1_pp[i] = 0.0;
                    
                    start_state_1[child1] = new std::string("NA");
                    start_state_1[child2] = new std::string("NA");
                    
                    start_state_1_pp[child1] = 0.0;
                    start_state_1_pp[child2] = 0.0;
                }
                
                if (end_state2_pp > 0.0001)
                {
                    end_state_2[i] = new std::string(end_state2);
                    end_state_2_pp[i] = end_state2_pp;
                    
                    start_state_2[child1] = new std::string(start_child1_state2);
                    start_state_2[child2] = new std::string(start_child2_state2);
                    
                    start_state_2_pp[child1] = end_state2_pp;
                    start_state_2_pp[child2] = end_state2_pp;
                    
                } else
                {
                    end_state_2[i] = new std::string("NA");
                    end_state_2_pp[i] = 0.0;
                    
                    start_state_2[child1] = new std::string("NA");
                    start_state_2[child2] = new std::string("NA");
                    
                    start_state_2_pp[child1] = 0.0;
                    start_state_2_pp[child2] = 0.0;
                }
                
                if (end_state3_pp > 0.0001)
                {
                    end_state_3[i] = new std::string(end_state3);
                    end_state_3_pp[i] = end_state3_pp;
                    
                    start_state_3[child1] = new std::string(start_child1_state3);
                    start_state_3[child2] = new std::string(start_child2_state3);
                    
                    start_state_3_pp[child1] = end_state3_pp;
                    start_state_3_pp[child2] = end_state3_pp;
                    
                } else
                {
                    end_state_3[i] = new std::string("NA");
                    end_state_3_pp[i] = 0.0;
                    
                    start_state_3[child1] = new std::string("NA");
                    start_state_3[child2] = new std::string("NA");
                    
                    start_state_3_pp[child1] = 0.0;
                    start_state_3_pp[child2] = 0.0;
                }
                
                if (end_other_pp > 0.0001)
                {
                    end_state_other_pp[i] = end_other_pp;
                    start_state_other_pp[child1] = end_other_pp;
                    start_state_other_pp[child2] = end_other_pp;
                } else {
                    end_state_other_pp[i] = 0.0;
                    start_state_other_pp[child1] = 0.0;
                    start_state_other_pp[child2] = 0.0;
                }
                
                if (i == final_summary_tree->getRoot().getIndex())
                {
                    start_state_1[i] = end_state_1[i];
                    start_state_2[i] = end_state_2[i];
                    start_state_3[i] = end_state_3[i];
                    
                    start_state_1_pp[i] = end_state_1_pp[i];
                    start_state_2_pp[i] = end_state_2_pp[i];
                    start_state_3_pp[i] = end_state_3_pp[i];
                    start_state_other_pp[i] = end_state_other_pp[i];
                }
                
            }
        }
        
        final_summary_tree->clearNodeParameters();
        final_summary_tree->addNodeParameter("posterior", posteriors, false);
        
        final_summary_tree->addNodeParameter("end_state_1", end_state_1, false);
        final_summary_tree->addNodeParameter("end_state_2", end_state_2, false);
        final_summary_tree->addNodeParameter("end_state_3", end_state_3, false);
        final_summary_tree->addNodeParameter("end_state_1_pp", end_state_1_pp, false);
        final_summary_tree->addNodeParameter("end_state_2_pp", end_state_2_pp, false);
        final_summary_tree->addNodeParameter("end_state_3_pp", end_state_3_pp, false);
        final_summary_tree->addNodeParameter("end_state_other_pp", end_state_other_pp, false);

        final_summary_tree->addNodeParameter("start_state_1", start_state_1, false);
        final_summary_tree->addNodeParameter("start_state_2", start_state_2, false);
        final_summary_tree->addNodeParameter("start_state_3", start_state_3, false);
        final_summary_tree->addNodeParameter("start_state_1_pp", start_state_1_pp, false);
        final_summary_tree->addNodeParameter("start_state_2_pp", start_state_2_pp, false);
        final_summary_tree->addNodeParameter("start_state_3_pp", start_state_3_pp, false);
        final_summary_tree->addNodeParameter("start_state_other_pp", start_state_other_pp, false);
        
    }
    else
    {
        // calculate the mean and 95% CI and add to the tree as annotation
        double hpd = 0.95;
        std::vector<double> start_means( summary_nodes.size(), 0.0 );
        std::vector<double> start_uppers( summary_nodes.size(), 0.0 );
        std::vector<double> start_lowers( summary_nodes.size(), 0.0 );
        std::vector<double> end_means( summary_nodes.size(), 0.0 );
        std::vector<double> end_uppers( summary_nodes.size(), 0.0 );
        std::vector<double> end_lowers( summary_nodes.size(), 0.0 );
        std::vector<double> posteriors( summary_nodes.size(), 0.0 );
        
        for (int i = 0; i < summary_nodes.size(); i++)
        {
            
            if ( summary_nodes[i]->isTip() )
            {
                posteriors[i] = 1.0;
                end_means[i] = boost::lexical_cast<double>( end_states[i][0] );
            }
            else
            {
                double node_pp = 0.0;
                std::vector<double> state_samples_end;
                std::vector<double> state_samples_start;
                
                // loop through all states for this node and collect samples
                for (int j = 0; j < pp[i].size(); j++)
                {
                    node_pp += pp[i][j];
                    state_samples_end.push_back( boost::lexical_cast<double>( end_states[i][j] ) );
                    state_samples_start.push_back( boost::lexical_cast<double>( start_states[i][j] ) );
                }
                posteriors[i] = node_pp;
                
                // calculate mean value for end states
                double samples_sum = std::accumulate(state_samples_end.begin(), state_samples_end.end(), 0.0);
                double mean = samples_sum / state_samples_end.size();
                end_means[i] = mean;
                
                // calculate mean value for start states
                samples_sum = std::accumulate(state_samples_start.begin(), state_samples_start.end(), 0.0);
                mean = samples_sum / state_samples_start.size();
                start_means[i] = mean;
                
                // sort the samples by frequency and calculate interval for the end states
                std::sort(state_samples_end.begin(), state_samples_end.end());
                size_t interval_start = ( (1.0 - hpd) / 2.0 ) * state_samples_end.size();
                size_t interval_end = ( 1.0 - (1.0 - hpd) / 2.0 ) * state_samples_end.size();
                interval_end = (interval_end >= state_samples_end.size() ? state_samples_end.size()-1 : interval_end);
                
                double lower = state_samples_end[interval_start];
                double upper = state_samples_end[interval_end];
                
                end_lowers[i] = lower;
                end_uppers[i] = upper;
                
                // sort the samples by frequency and calculate interval for the start states
                std::sort(state_samples_start.begin(), state_samples_start.end());
                interval_start = ( (1.0 - hpd) / 2.0 ) * state_samples_start.size();
                interval_end = ( 1.0 - (1.0 - hpd) / 2.0 ) * state_samples_start.size();
                interval_end = (interval_end >= state_samples_start.size() ? state_samples_start.size()-1 : interval_end);
                
                lower = state_samples_start[interval_start];
                upper = state_samples_start[interval_end];
                
                start_lowers[i] = lower;
                start_uppers[i] = upper;
            }
        }
        final_summary_tree->clearNodeParameters();
        final_summary_tree->addNodeParameter("posterior", posteriors, true);
        final_summary_tree->addNodeParameter("end_mean", end_means, false);
        final_summary_tree->addNodeParameter("end_lower_95%_CI", end_lowers, true);
        final_summary_tree->addNodeParameter("end_upper_95%_CI", end_uppers, true);
        final_summary_tree->addNodeParameter("start_mean", start_means, true);
        final_summary_tree->addNodeParameter("start_lower_95%_CI", start_lowers, true);
        final_summary_tree->addNodeParameter("start_upper_95%_CI", start_uppers, true);
    }
    
    return final_summary_tree;
}


/**
 *
 * Helper function for characterMapTree() that traverses the tree from root to tips collecting stochastic character map samples.
 *
 */
void TreeSummary::recursivelyCollectCharacterMapSamples(size_t node_index, size_t map_parent_state, bool root, bool conditional, std::vector<AncestralStateTrace> &ancestral_state_traces, int b, size_t num_sampled_states, size_t num_sampled_trees, Tree &final_summary_tree, const std::vector<TopologyNode*> &summary_nodes, std::vector<std::string*> &map_character_history, std::vector<std::string*> &map_character_history_posteriors, ProgressBar &progress, size_t &num_finished_nodes, int NUM_TIME_SLICES, bool verbose)
{
    
    double dt = final_summary_tree.getRoot().getAge() / double(NUM_TIME_SLICES);
        
    size_t sample_clade_index;
    bool trace_found = false;
    bool parent_trace_found = false;
    AncestralStateTrace ancestralstate_trace;
    AncestralStateTrace parent_trace;
    
    std::vector< std::vector< std::pair<size_t, double> > > branch_maps_all = std::vector< std::vector< std::pair<size_t, double> > >();
    std::vector< std::vector< std::pair<size_t, double> > > branch_maps_conditional = std::vector< std::vector< std::pair<size_t, double> > >();
    
    // loop through all the sampled character histories for this branch
    for (size_t j = burnin; j < num_sampled_states; ++j)
    {
        
        if ( verbose == true )
        {
            progress.update( num_finished_nodes * num_sampled_states + num_sampled_states * (j - burnin) / (num_sampled_states - burnin) );
        }
        
        // if necessary, get the sampled tree from the tree trace
        const Tree &sample_tree = (use_tree_trace) ? trace.objectAt( j ) : final_summary_tree;
        const TopologyNode& sample_root = sample_tree.getRoot();
        
        if ( use_tree_trace == true )
        {
            // check if the clade in the summary tree is also in the sampled tree
            sample_clade_index = sample_root.getCladeIndex( summary_nodes[node_index] );
            
            // and we must also find the trace for this node index
            trace_found = false;
        }
        else
        {
            sample_clade_index = summary_nodes[node_index]->getIndex();
        }
        
        if ( RbMath::isFinite( sample_clade_index ) == true )
        {
   
            bool use_sample = true;

            // check if we must condition on the parent's end state
            if ( conditional == true && root == false )
            {
                size_t sample_parent_index;
                if ( use_tree_trace == true )
                {
                    sample_parent_index = sample_tree.getNode( sample_clade_index ).getParent().getIndex();
                    parent_trace_found = false;
                }
                else
                {
                    sample_parent_index = summary_nodes[sample_clade_index]->getParent().getIndex();
                }

                if ( RbMath::isFinite( sample_parent_index ) == false )
                {
                    use_sample = false;
                }
                else
                {
                    
                    if ( parent_trace_found == false )
                    {
                        for (size_t k = 0; k < ancestral_state_traces.size(); ++k)
                        {
                            if (ancestral_state_traces[k].getParameterName() == StringUtilities::toString(sample_parent_index + 1))
                            {
                                parent_trace = ancestral_state_traces[k];
                                parent_trace_found = true;
                                break;
                            }
                        }
                    }

                    // get the sampled character history for the parent for this iteration
                    const std::vector<std::string>& parent_vector = parent_trace.getValues();
                    std::string character_history = parent_vector[j];
                    
                    // parse sampled SIMMAP string
                    std::vector< std::pair<size_t, double> > parent_branch_map = parseSIMMAPForNode(character_history);

                    // finally check against the map state of the parent
                    size_t parent_end_state = parent_branch_map[ parent_branch_map.size() - 1 ].first;
                    if ( parent_end_state != map_parent_state )
                    {
                        use_sample = false;
                    }
                }
            }

            // if necessary find the AncestralStateTrace for the sampled node
            if ( trace_found == false )
            {
                for (size_t k = 0; k < ancestral_state_traces.size(); ++k)
                {
                    if (ancestral_state_traces[k].getParameterName() == StringUtilities::toString(sample_clade_index + 1))
                    {
                        ancestralstate_trace = ancestral_state_traces[k];
                        trace_found = true;
                        break;
                    }
                }
            }
            
            // get the sampled character history for this iteration
            const std::vector<std::string>& ancestralstate_vector = ancestralstate_trace.getValues();
            std::string character_history = ancestralstate_vector[j];
            
            // parse sampled SIMMAP string
            std::vector< std::pair<size_t, double> > this_branch_map = parseSIMMAPForNode(character_history);
            
            if ( use_sample == true )
            {
                branch_maps_conditional.push_back(this_branch_map);
            } 
            branch_maps_all.push_back(this_branch_map);
        }
    }
    
    // now summarize maximum a posteriori states in each time slice along the branch
    
    double branch_len = summary_nodes[node_index]->getBranchLength();
    
    // strings to hold the SIMMAP strings for the summarized character history
    std::string branch_map_history = "}";
    std::string branch_map_history_posteriors = "}";
    
    // loop through each time slice along this branch
    double current_time = 0.0;
    size_t current_dt = 1;
    bool finished_branch = false;
    size_t map_state = 0;
    size_t old_map_state = 0;
    while ( finished_branch == false )
    {
        std::vector< std::vector< std::pair<size_t, double> > > branch_maps;
        if ( current_dt == 1 && conditional == true && root == false )
        {
            branch_maps = branch_maps_conditional;
        }
        else
        {
            branch_maps = branch_maps_all;
        }

        current_time = current_dt * dt;
        if ( current_time >= branch_len )
        {
            current_time = branch_len;
            finished_branch = true;
        }
        
        std::vector<size_t> states = std::vector<size_t>();
        std::vector<double> posteriors = std::vector<double>();
       
        // loop through all sampled character histories for this branch
        // and compile each state sampled in this time slice
        for (size_t k = 0; k < branch_maps.size(); k++)
        {
            double sample_time = 0.0;
            
            bool use_sample = true;
            if ( conditional == true && current_dt > 1 && root == false )
            {
                use_sample = false;
            }

            // find this time slice in this sampled character history
            for (size_t l = 0; l < branch_maps[k].size(); l++)
            {
                sample_time += branch_maps[k][l].second;
                
                // if we are conditioning on the previous time slice's MAP state
                // check whether or not we should use this sample by checking
                // the state in the last time slice
                // for current_dt == 1 we have already excluded branch_maps whose
                // parent didn't end with the parent MAP end state
                if ( conditional == true && current_dt > 1 )
                {
                    size_t window_size = 1;
                    if ( current_time - (window_size * dt) <= sample_time || l == branch_maps[k].size() - 1)
                    {
                        if ( branch_maps[k][l].first == old_map_state )
                        {
                            use_sample = true;
                        }
                    }
                }

                if ( use_sample == true && ( current_time <= sample_time || l == branch_maps[k].size() - 1 ) )
                {
                    bool state_found = false;
                    for (size_t m = 0; m < states.size(); m++)
                    {
                        if (states[m] == branch_maps[k][l].first)
                        {
                            state_found = true;
                            posteriors[m] += 1.0;
                            break;
                        }
                    }
                    if (state_found == false)
                    {
                        states.push_back(branch_maps[k][l].first);
                        posteriors.push_back(1.0);
                    }
                    break;
                }
            }
        }


        // scale posteriors
        double sum_posteriors = 0.0;
        for (size_t i = 0; i < posteriors.size(); i++)
        {
            sum_posteriors += posteriors[i];
        }
        for (size_t i = 0; i < posteriors.size(); i++)
        {
            posteriors[i] /= sum_posteriors;
        }
        
        // find the MAP state for this time slice
        double map_state_pp = 0.0;
        for (size_t k = 0; k < states.size(); k++)
        {
            if (posteriors[k] > map_state_pp)
            {
                map_state = states[k];
                map_state_pp = posteriors[k];
            }
        }
        old_map_state = map_state;
        
        // now add this time slice to the SIMMAP strings
        
        double time_slice_length = current_time - ((current_dt - 1) * dt);
        
        branch_map_history = "," + StringUtilities::toString(time_slice_length) + branch_map_history;
        branch_map_history_posteriors = "," + StringUtilities::toString(time_slice_length) + branch_map_history_posteriors;
        
        branch_map_history = StringUtilities::toString(map_state) + branch_map_history;
        branch_map_history_posteriors = StringUtilities::toString(int(map_state_pp * 100)) + branch_map_history_posteriors;
        
        if (finished_branch == false )
        {
            branch_map_history = ":" + branch_map_history;
            branch_map_history_posteriors = ":" + branch_map_history_posteriors;
        }

        current_dt++;
    }
    
    // finally finish the SIMMAP strings for this node
    branch_map_history = "{" + branch_map_history;
    branch_map_history_posteriors = "{" + branch_map_history_posteriors;
    map_character_history[node_index] = new std::string(branch_map_history);
    map_character_history_posteriors[node_index] = new std::string(branch_map_history_posteriors);

    // update nodes finished for the progress bar
    num_finished_nodes += 1;
             
    // recurse through tree
    std::vector<int> children_indices = summary_nodes[node_index]->getChildrenIndices();
    for (size_t i = 0; i < children_indices.size(); i++)
    {
        recursivelyCollectCharacterMapSamples(children_indices[i], map_state, false, conditional, ancestral_state_traces, b, num_sampled_states, num_sampled_trees, final_summary_tree, summary_nodes, map_character_history, map_character_history_posteriors, progress, num_finished_nodes, NUM_TIME_SLICES, verbose);
    }
}


/*
 * This method summarizes marginal, joint and conditional character histories over a give summary tree from a distribution of stochastic character map samples. 
 */
Tree* TreeSummary::characterMapTree(const Tree &input_summary_tree, std::vector<AncestralStateTrace> &ancestralstate_traces, int b, int NUM_TIME_SLICES, bool conditional, bool joint, bool verbose)
{
    
    // get the number of ancestral state samples and the number of tree samples
    size_t num_sampled_states = ancestralstate_traces[0].getValues().size();
    size_t num_sampled_trees;
    if (use_tree_trace == false)
    {
        // the ancestral states were sampled over the same tree
        num_sampled_trees = 1;
    }
    else
    {
        // the ancestral states were sampled over different trees each iteration
        num_sampled_trees = trace.size();
    }
    
    setBurnin(b);
    if ( b >= num_sampled_states )
    {
        throw RbException("Burnin size is too large for the character map trace.");
    }
    
    if ( use_tree_trace == true &&  num_sampled_trees != num_sampled_states )
    {
        throw RbException("The tree trace and the character map trace must contain the same number of samples.");
    }
    
    if ( NUM_TIME_SLICES < 1 )
    {
        throw RbException("The number of time slices must be at least 1.");
    }
    
    std::stringstream ss;
    ss << "Calculating maximum a posteriori character map from " << num_sampled_states << " samples in the character map trace, using a burnin of " << burnin << " samples.\n";
    RBOUT(ss.str());
    
    RBOUT("Compiling character maps...\n");
    
    // allocate memory for the new summary tree
    Tree* final_summary_tree = new Tree( input_summary_tree );
    const std::vector<TopologyNode*> &summary_nodes = final_summary_tree->getNodes();
    
    std::vector<std::string*> map_character_history( summary_nodes.size(), new std::string() );
    std::vector<std::string*> map_character_history_posteriors( summary_nodes.size(), new std::string() );
    
    bool process_active = true;
    ProgressBar progress = ProgressBar( summary_nodes.size() * num_sampled_states, 0 );
    if ( verbose == true && process_active == true )
    {
        progress.start();
    }

    // recurse through summary tree and collect ancestral state samples
    size_t node_index = final_summary_tree->getRoot().getIndex();
    size_t num_finished_nodes = 0;
    recursivelyCollectCharacterMapSamples(node_index, 0, true, conditional, ancestralstate_traces, b, num_sampled_states, num_sampled_trees, *final_summary_tree, summary_nodes, map_character_history, map_character_history_posteriors, progress, num_finished_nodes, NUM_TIME_SLICES, verbose);
    
    if ( verbose == true && process_active == true )
    {
        progress.finish();
    }
    
    // add all the SIMMAP strings to the tree
    final_summary_tree->clearNodeParameters();
    final_summary_tree->addNodeParameter("map_character_history", map_character_history, false);
    final_summary_tree->addNodeParameter("map_character_history_posteriors", map_character_history_posteriors, false);
    
    return final_summary_tree;
    
}
                

/*
 * Helper function that parses a SIMMAP character history for a single branch.
 * These strings represent character histories for a single branch in the form
 * {state_2,time_in_state_2:state_1,time_in_state_1} where the states are
 * listed left to right from the tip to the root (backward time). We loop through 
 * the string from right to left to store events in forward time (root to tip).
 * Returns vector of events: [<state_1, time_in_state_1>, <state_2, time_in_state_2>]
 */
std::vector< std::pair<size_t, double> > TreeSummary::parseSIMMAPForNode(std::string character_history)
{

    boost::trim(character_history);
    
    // Now parse the sampled SIMMAP string:
    bool parsed_time = false;
    std::vector< std::pair<size_t, double> > this_branch_map = std::vector< std::pair<size_t, double> >();
    std::pair<size_t, double> this_event = std::pair<size_t, double>();
    std::string state = "";
    std::string time = "";
    size_t k = character_history.size();
    
    while (true) {
        
        if ( k == (character_history.size() - 1) &&
                            std::string(1, character_history[0]).compare("{") != 0 &&
                            std::string(1, character_history[k]).compare("}") != 0 )
        {
            throw RbException("Error while summarizing character maps: trace does not contain valid SIMMAP string.");
        }
        else if ( std::string(1, character_history[k]).compare(",") == 0 )
        {
            parsed_time = true;
            this_event.second = std::atof( time.c_str() );
        }
        else if ( std::string(1, character_history[k]).compare(":") == 0 || k == 0 )
        {
            this_event.first = std::atoi( state.c_str() );
            this_branch_map.push_back( this_event );
            if (k == 0)
            {
                break;
            }
            else
            {
                state = "";
                time = "";
                parsed_time = false;
            }
        }
        else if ( parsed_time == false )
        {
            time = std::string(1, character_history[k]) + time;
        }
        else
        {
            state = std::string(1, character_history[k]) + state;
        }
        k--;
    }

    return this_branch_map;
}


// annotate the MAP node/branch parameters
void TreeSummary::mapParameters( Tree &tree ) const
{
    
    const Tree& sample_tree = trace.objectAt( 0 );
    
    // first we annotate the node parameters
    // we need an internal node because the root might not have all parameter (e.g. rates)
    // and the tips might neither have all parameters
    const TopologyNode *n = &sample_tree.getRoot().getChild( 0 );
    if ( n->isTip() == true )
    {
        n = &sample_tree.getRoot().getChild( 1 );
    }
    const std::vector<std::string> &nodeParameters = n->getNodeParameters();
    for (size_t i = 0; i < nodeParameters.size(); ++i)
    {
        
        std::string tmp = nodeParameters[i];
        if ( tmp[0] == '&')
        {
            tmp = tmp.substr(1,tmp.size());
        }
        std::vector<std::string> pair;
        StringUtilities::stringSplit(tmp, "=", pair);
        
        if ( pair[0] == "index" ) continue;
        
        if ( StringUtilities::isNumber( pair[1] ) && !StringUtilities::isIntegerNumber( pair[1] ) )
        {
            mapContinuous(tree, pair[0], i, 0.95, true);
        }
        else
        {
            mapDiscrete(tree, pair[0], i, 3, true);
        }
        
    }
    
    // then we annotate the branch parameters
    const std::vector<std::string> &leftBranchParameters = sample_tree.getRoot().getChild(0).getBranchParameters();
    const std::vector<std::string> &rightBranchParameters = sample_tree.getRoot().getChild(1).getBranchParameters();
    
    std::vector<std::string> branchParameters;
    if ( leftBranchParameters.size() > rightBranchParameters.size() )
    {
        branchParameters = leftBranchParameters;
    }
    else
    {
        branchParameters = rightBranchParameters;
    }
    
    for (size_t i = 0; i < branchParameters.size(); ++i)
    {
        
        std::string tmp = branchParameters[i];
        if ( tmp[0] == '&')
        {
            tmp = tmp.substr(1,tmp.size());
        }
        std::vector<std::string> pair;
        StringUtilities::stringSplit(tmp, "=", pair);
        
        if ( pair[0] == "index" ) continue;
        
        if ( StringUtilities::isNumber( pair[1] ) )
        {
            mapContinuous(tree, pair[0], i, 0.95, false);
        }
        else
        {
            mapDiscrete(tree, pair[0], i, 3, false);
        }
        
    }
    
}



/*
 * this method calculates the MAP ancestral character states for the nodes on the input_tree
 */
void TreeSummary::mapDiscrete(Tree &tree, const std::string &n, size_t paramIndex, size_t num, bool isNodeParameter ) const
{
    
    // 2-d vectors to keep the data (posteriors and states) of the inputTree nodes: [node][data]
    const std::vector<TopologyNode*> &summary_nodes = tree.getNodes();
    std::vector<std::map<std::string, Sample<std::string> > > stateAbsencePresence(summary_nodes.size(), std::map<std::string, Sample<std::string> >());
    
    bool interiorOnly = true;
    bool tipsChecked = false;
    //    bool useRoot = true;
    
    // loop through all trees in tree trace
    for (size_t iteration = burnin; iteration < trace.size(); ++iteration)
    {
        const Tree &sample_tree = trace.objectAt( iteration );
        const TopologyNode& sample_root = sample_tree.getRoot();
        
        // loop through all nodes in inputTree
        for (size_t node_index = 0; node_index < summary_nodes.size(); ++node_index)
        {
            TopologyNode *node = summary_nodes[node_index];
            
            if ( node->isTip() == true )
            {
                if ( tipsChecked == false )
                {
                    
                    tipsChecked = true;
                    size_t sample_clade_index = sample_root.getCladeIndex( node );
                    
                    const TopologyNode &sample_node = sample_tree.getNode( sample_clade_index );
                    
                    std::vector<std::string> params;
                    if ( isNodeParameter == true )
                    {
                        params = sample_node.getNodeParameters();
                    }
                    else
                    {
                        params = sample_node.getBranchParameters();
                    }
                    
                    // check if this parameter exists
                    if ( params.size() > paramIndex )
                    {
                        
                        std::string tmp = params[paramIndex];
                        if ( tmp[0] == '&')
                        {
                            tmp = tmp.substr(1,tmp.size());
                        }
                        std::vector<std::string> pair;
                        StringUtilities::stringSplit(tmp, "=", pair);
                        
                        // check if this parameter has the correct name
                        interiorOnly = pair[0] != n;
                    }
                    
                    
                }
                
                if ( interiorOnly == true )
                {
                    continue;
                }
            }
            
            if ( sample_root.containsClade(node, true) )
            {
                // if the inputTree node is also in the sample tree
                // we get the ancestral character state from the ancestral state trace
                size_t sample_clade_index = sample_root.getCladeIndex( node );
                
                const TopologyNode &sample_node = sample_tree.getNode( sample_clade_index );
                
                std::vector<std::string> params;
                if ( isNodeParameter == true )
                {
                    params = sample_node.getNodeParameters();
                }
                else
                {
                    params = sample_node.getBranchParameters();
                }
                
                // check if this parameter exists
                if ( params.size() <= paramIndex )
                {
                    if ( sample_node.isRoot() == true )
                    {
                        continue;
                    }
                    else
                    {
                        throw RbException("Too few parameter for this tree during the tree annotation.");
                    }
                    
                }
                
                std::string tmp = params[paramIndex];
                if ( tmp[0] == '&')
                {
                    tmp = tmp.substr(1,tmp.size());
                }
                std::vector<std::string> pair;
                StringUtilities::stringSplit(tmp, "=", pair);
                
                // check if this parameter has the correct name
                if ( pair[0] != n )
                {
                    throw RbException("The parameter for this tree doesn't match during the tree annotation.");
                }
                
                const std::string &state = pair[1];
                
                std::map<std::string, Sample<std::string> >::iterator entry = stateAbsencePresence[node_index].find( state );
                
                if ( entry == stateAbsencePresence[node_index].end() )
                {
                    Sample<std::string> stateSample = Sample<std::string>(state, 0);
                    if (iteration > burnin)
                    {
                        stateSample.setTrace(std::vector<double>(iteration - burnin, 0.0));
                    }
                    else
                    {
                        stateSample.setTrace(std::vector<double>());
                    }
                    stateAbsencePresence[node_index].insert(std::pair<std::string, Sample<std::string> >(state, stateSample));
                }
                
                
                for (std::map<std::string, Sample<std::string> >::iterator it=stateAbsencePresence[node_index].begin(); it!=stateAbsencePresence[node_index].end(); ++it )
                {
                    
                    const Sample<std::string> &s = it->second;
                    if ( s.getValue() == state )
                    {
                        it->second.addObservation( true );
                    }
                    else
                    {
                        it->second.addObservation( false );
                    }
                    
                } // end loop over all samples for this node
                
            } // end if the sampled tree contained this clade
            
        } // end loop over all nodes in the tree
        
    } // end loop over each iteration in the trace
    
    
    std::vector<double> posteriors;
    for (int i = 0; i < summary_nodes.size(); i++)
    {
        
        TopologyNode &node = *summary_nodes[i];
        if ( node.isTip() && interiorOnly == true )
        {
            // make parameter string for this node
            if ( isNodeParameter == true )
            {
                node.addNodeParameter(n,"{}");
            }
            else
            {
                node.addBranchParameter(n,"{}");
            }
        }
        else
        {
            
            // collect the samples
            std::vector<Sample<std::string> > stateSamples;
            for (std::map<std::string, Sample<std::string> >::iterator it = stateAbsencePresence[i].begin(); it != stateAbsencePresence[i].end(); ++it)
            {
                it->second.computeStatistics();
                stateSamples.push_back(it->second);
            }
            
            // sort the samples by frequency
            sort(stateSamples.begin(), stateSamples.end());
            
            double total_node_pp = 0.0;
            std::string final_state = "{";
            for (size_t j = 0; j < num && j < stateSamples.size(); ++j)
            {
                if ( total_node_pp > 0.9999 ) continue;
                
                if (j > 0)
                {
                    final_state += ",";
                }
                
                double pp = stateSamples[j].getFrequency() / double(stateSamples[j].getSampleSize());
                final_state += stateSamples[j].getValue() + "=" + StringUtilities::toString(pp);
                total_node_pp += pp;
                
            }
            
            final_state += "}";
            
            // make parameter string for this node
            if ( isNodeParameter == true )
            {
                node.addNodeParameter(n,final_state);
            }
            else
            {
                node.addBranchParameter(n,final_state);
            }
        }
    }
    
}


/*
 * this method calculates the MAP ancestral character states for the nodes on the input_tree
 */
void TreeSummary::mapContinuous(Tree &tree, const std::string &n, size_t paramIndex, double hpd, bool isNodeParameter ) const
{
    
    // 2-d vectors to keep the data (posteriors and states) of the inputTree nodes: [node][data]
    const std::vector<TopologyNode*> &summary_nodes = tree.getNodes();
    std::vector<std::vector<double> > samples(summary_nodes.size(),std::vector<double>());
    
    // flag if only interior nodes are used
    bool interiorOnly = false;
    bool tipsChecked = false;
    bool rootChecked = false;
    bool useRoot = true;
    
    // loop through all trees in tree trace
    for (size_t i = burnin; i < trace.size(); i++)
    {
        const Tree &sample_tree = trace.objectAt( i );
        const TopologyNode& sample_root = sample_tree.getRoot();
        
        // loop through all nodes in inputTree
        for (size_t j = 0; j < summary_nodes.size(); j++)
        {
            TopologyNode *node = summary_nodes[j];
            if ( node->isTip() == true )
            {
                if ( tipsChecked == false )
                {
                    
                    tipsChecked = true;
                    size_t sample_clade_index = sample_root.getCladeIndex( node );
                    
                    const TopologyNode &sample_node = sample_tree.getNode( sample_clade_index );
                    
                    std::vector<std::string> params;
                    if ( isNodeParameter == true )
                    {
                        params = sample_node.getNodeParameters();
                    }
                    else
                    {
                        params = sample_node.getBranchParameters();
                    }
                    
                    // check if this parameter exists
                    if ( params.size() > paramIndex )
                    {
                        
                        std::string tmp = params[paramIndex];
                        if ( tmp[0] == '&')
                        {
                            tmp = tmp.substr(1,tmp.size());
                        }
                        std::vector<std::string> pair;
                        StringUtilities::stringSplit(tmp, "=", pair);
                        
                        // check if this parameter has the correct name
                        interiorOnly = pair[0] != n;
                    }
                    else
                    {
                        interiorOnly = true;
                    }
                    
                    
                }
                
                if ( interiorOnly == true )
                {
                    continue;
                }
            }
            
            if ( node->isRoot() == true )
            {
                if ( rootChecked == false )
                {
                    
                    rootChecked = true;
                    
                    size_t sample_clade_index = sample_root.getCladeIndex( node );
                    
                    const TopologyNode &sample_node = sample_tree.getNode( sample_clade_index );
                    
                    std::vector<std::string> params;
                    if ( isNodeParameter == true )
                    {
                        params = sample_node.getNodeParameters();
                    }
                    else
                    {
                        params = sample_node.getBranchParameters();
                    }
                    
                    // check if this parameter exists
                    if ( params.size() > paramIndex )
                    {
                        
                        std::string tmp = params[paramIndex];
                        if ( tmp[0] == '&')
                        {
                            tmp = tmp.substr(1,tmp.size());
                        }
                        std::vector<std::string> pair;
                        StringUtilities::stringSplit(tmp, "=", pair);
                        
                        // check if this parameter has the correct name
                        useRoot = pair[0] == n;
                    }
                    else
                    {
                        useRoot = false;
                    }
                    
                    
                }
                
                if ( useRoot == false )
                {
                    continue;
                }
                
            }
            
            if ( sample_root.containsClade(node, true) )
            {
                // if the inputTree node is also in the sample tree
                // we get the ancestral character state from the ancestral state trace
                size_t sample_clade_index = sample_root.getCladeIndex( node );
                
                const TopologyNode &sample_node = sample_tree.getNode( sample_clade_index );
                
                std::vector<std::string> params;
                if ( isNodeParameter == true )
                {
                    params = sample_node.getNodeParameters();
                }
                else
                {
                    params = sample_node.getBranchParameters();
                }
                
                // check if this parameter exists
                if ( params.size() <= paramIndex )
                {
                    throw RbException("Too few parameter for this tree during the tree annotation.");
                }
                
                std::string tmp = params[paramIndex];
                if ( tmp[0] == '&')
                {
                    tmp = tmp.substr(1,tmp.size());
                }
                std::vector<std::string> pair;
                StringUtilities::stringSplit(tmp, "=", pair);
                
                // check if this parameter has the correct name
                if ( pair[0] != n )
                {
                    
                    throw RbException("The parameter for this tree doesn't match during the tree annotation.");
                }
                
                double state = atof(pair[1].c_str());
                
                std::vector<double> &entries = samples[j];
                entries.push_back( state );
                
            } // end if the sampled tree contained this clade
            
        } // end loop over all nodes in the tree
        
    } // end loop over each iteration in the trace
    
    
    std::vector<double> posteriors;
    for (int idx = 0; idx < summary_nodes.size(); ++idx)
    {
        
        TopologyNode &node = *summary_nodes[idx];
        if ( ( node.isTip() == false || interiorOnly == false ) && ( node.isRoot() == false || useRoot == true ) )
        {
            
            // collect the samples
            std::vector<double> stateSamples = samples[idx];
            
            // sort the samples by frequency
            sort(stateSamples.begin(), stateSamples.end());
            
            
            size_t interval_start = ((1.0-hpd)/2.0) * stateSamples.size();
            size_t interval_median = 0.5 * stateSamples.size();
            size_t interval_end = (1.0-(1.0-hpd)/2.0) * stateSamples.size();
            interval_end = (interval_end >= stateSamples.size() ? stateSamples.size()-1 : interval_end);
            double lower = stateSamples[interval_start];
            double median = stateSamples[interval_median];
            double upper = stateSamples[interval_end];
            
            // make node age annotation
            std::string param = "{" + StringUtilities::toString(lower)
            + "," + StringUtilities::toString(upper) + "}";
            
            if ( isNodeParameter == true )
            {
                // make parameter string for this node
                node.addNodeParameter(n+"_range",param);
                
                // make parameter string for this node
                node.addNodeParameter(n,median);
            }
            else
            {
                
                // make parameter string for this node
                node.addBranchParameter(n+"_range",param);
                
                // make parameter string for this node
                node.addBranchParameter(n,median);
            }
            
        }
        
    }
    
}


void TreeSummary::annotateTree( Tree &tree, AnnotationReport report, bool verbose )
{
    summarize( verbose );

    RBOUT("Annotating tree ...");

    std::string newick;
    
    if( report.tree_ages )
    {
        Tree* tmp_tree = NULL;
        if ( clock == true )
        {
            tmp_tree = TreeUtilities::convertTree( tree );
        }
        else
        {
            tmp_tree = tree.clone();
        }

        if( tmp_tree->isRooted() == false && rooted == false )
        {
            tmp_tree->reroot( trace.objectAt(0).getTipNames()[0], true );
        }
        else if( tmp_tree->isRooted() != rooted )
        {
            throw(RbException("Rooting of input tree differs from the tree sample"));
        }

        newick = TreeUtilities::uniqueNewickTopology( *tmp_tree );

        delete tmp_tree;

        if( treeCladeAges.find(newick) == treeCladeAges.end() )
        {
            throw(RbException("Could not find input tree in tree sample"));
        }
    }

    const std::vector<TopologyNode*> &nodes = tree.getNodes();
    
    double sampleSize = trace.size() - burnin;
    
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        TopologyNode* n = nodes[i];

        Clade c = n->getClade();

        // annotate clade posterior prob
        if ( ( !n->isTip() || ( n->isRoot() && !c.getMrca().empty() ) ) && report.posterior )
        {
            double cladeFreq = findCladeSample( c ).getFrequency();
            double pp = cladeFreq / sampleSize;
            n->addNodeParameter("posterior",pp);
        }

        // are we using sampled ancestors?
        if( sampledAncestorSamples.empty() == false )
        {
            double saFreq = sampledAncestorSamples[n->getTaxon()].getFrequency();

            // annotate sampled ancestor prob
            if( ((n->isTip() && n->isFossil()) || saFreq > 0) && report.sa )
            {
                n->addNodeParameter("sampled_ancestor", saFreq / sampleSize);
            }
        }

        // annotate conditional clade probs and get node ages
        std::vector<double> nodeAges;

        if ( !n->isRoot() )
        {
            Clade parent = n->getParent().getClade();
            std::map<Clade, std::vector<double>, CladeComparator >& condCladeAges = conditionalCladeAges[parent];
            nodeAges = report.cc_ages ? condCladeAges[c] : cladeAges[c];

            // annotate CCPs
            if( !n->isTip() && report.ccp )
            {
                double parentCladeFreq = findCladeSample( parent ).getFrequency();
                double ccp = condCladeAges[c].size() / parentCladeFreq;
                n->addNodeParameter("ccp",ccp);
            }
        }
        else
        {
            nodeAges = cladeAges[c];
        }

        if ( report.tree_ages )
        {
            nodeAges = treeCladeAges[newick][ c ];
        }
            
        // set the node ages/branch lengths
        if( report.ages )
        {
            double age = 0.0;

            if ( report.mean )
            {
                // finally, we compute the mean conditional age
                for (size_t i = 0; i<nodeAges.size(); ++i)
                {
                    age += nodeAges[i];
                }
                age /= nodeAges.size();
            }
            else // use median
            {

                size_t idx = nodeAges.size() / 2;
                std::sort( nodeAges.begin(), nodeAges.end() );
                if (nodeAges.size() % 2 == 1)
                {
                    age = nodeAges[idx];
                }
                else
                {
                    age = (nodeAges[idx-1] + nodeAges[idx]) / 2;
                }

            }

            // finally, we set the age/length
            if ( clock )
            {
                n->setAge( age );
            }
            else
            {
                n->setBranchLength( age );
            }
        }

        // annotate the HPD node age intervals
        if( report.hpd )
        {
            //nodeAges = cladeAges[c];

            std::sort(nodeAges.begin(), nodeAges.end());

            size_t total_branch_lengths = nodeAges.size();
            double min_range = std::numeric_limits<double>::max();

            size_t interval_start = 0;
            int interval_size = (int)(report.hpd * (double)total_branch_lengths);

            // find the smallest interval that contains x% of the samples
            for (size_t j = 0; j <= (total_branch_lengths - interval_size); j++)
            {
                double temp_lower = nodeAges[j];
                double temp_upper = nodeAges[j + interval_size - 1];
                double temp_range = std::fabs(temp_upper - temp_lower);
                if (temp_range < min_range)
                {
                    min_range = temp_range;
                    interval_start = j;
                }
            }
            double lower = nodeAges[interval_start];
            double upper = nodeAges[interval_start + interval_size - 1];

            // make node age annotation
            std::string interval = "{" + StringUtilities::toString(lower)
                                 + "," + StringUtilities::toString(upper) + "}";

            if( clock )
            {
                if( !n->isTip() || ( ( n->isFossil() || upper != lower) && !n->isSampledAncestor() ) )
                {
                    std::string label = "age_" + StringUtilities::toString( (int)(report.hpd * 100) ) + "%_HPD";
                    n->addNodeParameter(label, interval);
                }
            }
            else if( !n->isRoot() )
            {
                std::string label = "brlen_" + StringUtilities::toString( (int)(report.hpd * 100) ) + "%_HPD";
                n->addBranchParameter(label, interval);
            }
        }

    }
    
    /*if( !report.tree_ages && clock )
    {
        enforceNonnegativeBranchLengths( tree.getRoot() );
    }*/

    if( report.map_parameters )
    {
        mapParameters( tree );
    }

}



double TreeSummary::cladeProbability(const RevBayesCore::Clade &c, bool verbose )
{
    summarize(verbose);
    
    double f = 0.0;
    Clade tmp = c;
    tmp.resetTaxonBitset( getTreeTrace().getValues()[0].getTaxonBitSetMap() );
    
    try
    {
        const Sample<Clade> &s = findCladeSample( tmp );
        f = double(s.getFrequency()) / s.getSampleSize();
    }
    catch (RbException e)
    {
        // do nothing
    }

    return f;
}


double TreeSummary::computeEntropy( double credible_interval_size, bool verbose )
{
    summarize( verbose );
    
    NewickConverter converter;
    double total_prob = 0;
    double total_samples = trace.size();
    double entropy = 0.0;
    double tree_count = 0.0;
    for (std::vector<Sample<std::string> >::const_reverse_iterator it = treeSamples.rbegin(); it != treeSamples.rend(); ++it)
    {
        double freq = it->getFrequency();
        double p = freq/(total_samples-burnin);
        total_prob += p;
        
        ++tree_count;
        entropy += (p * log(p));
        
        if ( total_prob >= credible_interval_size )
        {
            break;
        }
        
    }
    
    entropy += log( tree_count );
    
    return entropy;
}


std::vector<double> TreeSummary::computePairwiseRFDistance( double credible_interval_size, bool verbose )
{
    summarize( verbose );
    
    std::vector<Tree> unique_trees;
    std::vector<size_t> sample_count;
    NewickConverter converter;
    double total_prob = 0;
    double total_samples = trace.size();
    for (std::vector<Sample<std::string> >::const_reverse_iterator it = treeSamples.rbegin(); it != treeSamples.rend(); ++it)
    {
        double freq = it->getFrequency();
        double p = freq/(total_samples-burnin);
        total_prob += p;

        sample_count.push_back( freq );

        Tree* current_tree = converter.convertFromNewick( it->getValue() );
        unique_trees.push_back( *current_tree );
        delete current_tree;
        if ( total_prob >= credible_interval_size )
        {
            break;
        }
        
    }
    
    std::vector<double> rf_distances;
    for (size_t i=0; i<unique_trees.size(); ++i)
    {
        // first we need to compare the tree to 'itself'
        for (size_t k=0; k<(sample_count[i]*(sample_count[i]-1)); ++k )
        {
            rf_distances.push_back( 0.0 );
        }
        
        for (size_t j=i+1; j<unique_trees.size(); ++j)
        {
            const Tree &a = unique_trees[i];
            const Tree &b = unique_trees[j];
            double rf = TreeUtilities::computeRobinsonFouldDistance(a, b);
            
            for (size_t k=0; k<(sample_count[i]*sample_count[j]); ++k )
            {
                rf_distances.push_back( rf );
            }
        }
    }
    
    return rf_distances;
}


std::vector<double> TreeSummary::computeTreeLengths( void )
{
    
    std::vector<double> tree_lengths;
    NewickConverter converter;

    for (size_t i = burnin; i < trace.size(); ++i)
    {
        
        const Tree &tree = trace.objectAt(i);
        tree_lengths.push_back( tree.getTreeLength() );
        
    }
    
    return tree_lengths;
}


void TreeSummary::enforceNonnegativeBranchLengths(TopologyNode& node) const
{
    std::vector<TopologyNode*> children = node.getChildren();

    for (size_t i = 0; i < children.size(); i++)
    {
        if (children[i]->getAge() > node.getAge())
        {
            children[i]->setAge( node.getAge() );
        }
        enforceNonnegativeBranchLengths( *children[i] );
    }
}


//filling in clades and clade ages - including tip nodes in clade sample - to get age for serially sampled tips in time trees
Clade TreeSummary::fillConditionalClades(const TopologyNode &n, std::map<Clade, std::set<Clade, CladeComparator>, CladeComparator> &cond_clades)
{
    
    Clade parent = n.getClade();
    parent.setAge( (clock == true ? n.getAge() : n.getBranchLength() ) );
    
    std::set<Clade, CladeComparator> children( (CladeComparator(rooted)) );

    for (size_t i = 0; i < n.getNumberOfChildren(); i++)
    {
        const TopologyNode &child_node = n.getChild(i);

        Clade child_clade = fillConditionalClades(child_node, cond_clades);
        children.insert(child_clade);
    }
    
    cond_clades[parent] = children;

    return parent;
}


const Sample<Clade>& TreeSummary::findCladeSample(const Clade &n) const
{
    
    std::vector<Sample<Clade> >::const_iterator it = find_if(cladeSamples.begin(), cladeSamples.end(), CladeComparator(rooted, n) );

    if ( it != cladeSamples.end() )
    {
        return *it;
    }
    
    throw RbException("Couldn't find a clade with name '" + n.toString() + "'.");
}


TopologyNode* TreeSummary::findParentNode(TopologyNode& n, const Clade& tmp, std::vector<TopologyNode*>& children, RbBitSet& child_b ) const
{
    const RbBitSet& node = n.getClade().getBitRepresentation();

    Clade c = tmp;
    const RbBitSet& clade  = c.getBitRepresentation();

    RbBitSetGeneral mask  = node | clade;

    bool compatible = (mask == node);
    bool child      = (mask == clade);

    // check if the flipped unrooted split is compatible
    if ( !rooted && !compatible && !child)
    {
        RbBitSet clade_flip = clade; ~clade_flip;
        mask  = node | clade_flip;

        compatible = (mask == node);

        if ( compatible )
        {
            c.setBitRepresentation(clade_flip);
        }
    }

    TopologyNode* parent = NULL;

    if (compatible)
    {
        parent = &n;

        std::vector<TopologyNode*> x = n.getChildren();

        std::vector<TopologyNode*> new_children;

        // keep track of which taxa we found in the children
        RbBitSetGeneral child_mask(clade.size());

        for (size_t i = 0; i < x.size(); i++)
        {
            RbBitSetGeneral child_b(clade.size());

            TopologyNode* child = findParentNode(*x[i], c, new_children, child_b );

            // add this child to the mask
            child_mask = (child_b | child_mask);

            // check if child is a compatible parent
            if (child != NULL)
            {
                parent = child;
                break;
            }
        }

        children = new_children;

        // check that we found all the children
        if ( parent == &n && child_mask != clade)
        {
            parent = NULL;
        }
    }
    else if(child)
    {
        child_b = node;
        children.push_back(&n);
    }

    return parent;
}


int TreeSummary::getBurnin( void ) const
{

    return burnin;
}


/*
 * Split a string of sampled states for multiple sites (e.g. "5,12,3") and return the sample for a single site.
 */
std::string TreeSummary::getSiteState( const std::string &site_sample, size_t site )
{
    std::vector<std::string> states;
    std::istringstream ss( site_sample );
    std::string state;
    
    while(std::getline(ss, state, ','))
    {
        states.push_back(state);
    }
    if (site >= states.size())
    {
        site = 0;
    }
    return states[site];
}


const RevBayesCore::TraceTree & TreeSummary::getTreeTrace(void) const
{
    return trace;
}


int TreeSummary::getTopologyFrequency(const RevBayesCore::Tree &tree, bool verbose)
{
    summarize( verbose );
    
    std::string outgroup = trace.objectAt(0).getTipNames()[0];

    Tree t = tree;

    if( t.isRooted() == false && rooted == false )
    {
        t.reroot( outgroup, true );
    }

    std::string newick = TreeUtilities::uniqueNewickTopology( t );
    
    //double total_samples = trace.size();
    double freq = 0;
    
    for (std::vector<Sample<std::string> >::const_reverse_iterator it = treeSamples.rbegin(); it != treeSamples.rend(); ++it)
    {
        
        if ( newick == it->getValue() )
        {
            freq =it->getFrequency();
//            p = freq/(total_samples-burnin);
            
            // now we found it
            break;
        }
        
    }
    
    return freq;
}


std::vector<Tree> TreeSummary::getUniqueTrees( double credible_interval_size, bool verbose )
{
    summarize( verbose );
    
    std::vector<Tree> unique_trees;
    NewickConverter converter;
    double total_prob = 0;
    double total_samples = trace.size();
    for (std::vector<Sample<std::string> >::const_reverse_iterator it = treeSamples.rbegin(); it != treeSamples.rend(); ++it)
    {
        double freq =it->getFrequency();
        double p =freq/(total_samples-burnin);
        total_prob += p;
        
        Tree* current_tree = converter.convertFromNewick( it->getValue() );
        unique_trees.push_back( *current_tree );
        delete current_tree;
        if ( total_prob >= credible_interval_size )
        {
            break;
        }
        
    }
    
    return unique_trees;
}



bool TreeSummary::isTreeContainedInCredibleInterval(const RevBayesCore::Tree &t, double size, bool verbose)
{

    summarize( verbose );
    
    RandomNumberGenerator *rng = GLOBAL_RNG;

    std::string outgroup = trace.objectAt(0).getTipNames()[0];

    Tree tree = t;

    if( tree.isRooted() == false && rooted == false )
    {
        tree.reroot( outgroup, true );
    }

    std::string newick = TreeUtilities::uniqueNewickTopology(tree);
   
    double totalSamples = trace.size();
    double totalProb = 0.0;
    for (std::vector<Sample<std::string> >::reverse_iterator it = treeSamples.rbegin(); it != treeSamples.rend(); ++it)
    {
        
        double p =it->getFrequency()/(totalSamples-burnin);
        double include_prob = (size-totalProb)/p;
//        double include_prob = p*size;
        
        if ( include_prob > rng->uniform01() )
        {
            const std::string &current_sample = it->getValue();
            if ( newick == current_sample )
            {
                return true;
            }
            
        }
        
        totalProb += p;
        
        
        if ( totalProb >= size )
        {
            break;
        }
        
    }
    
    return false;
}


Tree* TreeSummary::mapTree( AnnotationReport report, bool verbose )
{
    std::stringstream ss;
    ss << "Compiling maximum a posteriori tree from " << trace.size() << " trees in tree trace, using a burnin of " << burnin << " trees.\n";
    RBOUT(ss.str());
    
    summarize( verbose );
    
    // get the tree with the highest posterior probability
    std::string bestNewick = treeSamples.rbegin()->getValue();
    NewickConverter converter;
    Tree* tmp_best_tree = converter.convertFromNewick( bestNewick );
    
    Tree* tmp_tree = NULL;

    if ( clock == true )
    {
        tmp_tree = TreeUtilities::convertTree( *tmp_best_tree );
    }
    else
    {
        tmp_tree = tmp_best_tree->clone();
    }

    delete tmp_best_tree;

    TaxonMap tm = TaxonMap( trace.objectAt(0) );
    tmp_tree->setTaxonIndices( tm );

    report.ages            = true;
    report.map_parameters  = true;
    annotateTree(*tmp_tree, report, verbose );
    
    return tmp_tree;
}


Tree* TreeSummary::mccTree( AnnotationReport report, bool verbose )
{
    std::stringstream ss;
    ss << "Compiling maximum clade credibility tree from " << trace.size() << " trees in tree trace, using a burnin of " << burnin << " trees.\n";
    RBOUT(ss.str());

    summarize( verbose );

    Tree* best_tree = NULL;
    double max_cc = 0;

    // find the clade credibility score for each tree
    for(size_t t = 0; t < treeSamples.size(); t++)
    {
        std::string newick = treeSamples[t].getValue();

        // now we summarize the clades for the best tree
        std::map<Clade, std::vector<double>, CladeComparator> cladeAges = treeCladeAges[newick];

        double cc = 0;

        // find the product of the clade frequencies
        for(std::map<Clade, std::vector<double>, CladeComparator>::iterator clade = cladeAges.begin(); clade != cladeAges.end(); clade++)
        {
            cc += log( findCladeSample( clade->first ).getFrequency() );
        }

        if(cc > max_cc)
        {
            max_cc = cc;

            delete best_tree;

            NewickConverter converter;
            Tree* tmp_tree = converter.convertFromNewick( newick );
            if ( clock == true )
            {
                best_tree = TreeUtilities::convertTree( *tmp_tree );
            }
            else
            {
                best_tree = tmp_tree->clone();
            }

            TaxonMap tm = TaxonMap( trace.objectAt(0) );
            best_tree->setTaxonIndices( tm );

            delete tmp_tree;
        }
    }

    report.ages = true;
    annotateTree(*best_tree, report, verbose );

    return best_tree;
}


Tree* TreeSummary::mrTree(AnnotationReport report, double cutoff, bool verbose)
{
    if (cutoff < 0.0 || cutoff > 1.0) cutoff = 0.5;

    std::stringstream ss;
    ss << "Compiling majority rule consensus tree (cutoff = " << cutoff << ") from " << trace.size() << " trees in tree trace, using a burnin of " << burnin << " trees.\n";
    RBOUT(ss.str());

    //fill in clades, use all above 50% to resolve the bush with the consensus partitions
    summarize( verbose );        //fills std::vector<Sample<std::string> > cladeSamples, sorts them by descending freq

    //set up variables for consensus tree assembly
    std::vector<std::string> tipNames = trace.objectAt(0).getTipNames();

    //first create a bush
    TopologyNode* root = new TopologyNode(tipNames.size()); //construct root node with index = nb Tips
    root->setNodeType(false, true, true);

    for (size_t i = 0; i < tipNames.size(); i++)
    {
        TopologyNode* tipNode = new TopologyNode(tipNames.at(i), i); //Topology node constructor adding tip name and index=taxon nb
        tipNode->setNodeType(true, false, false);

        // set the parent-child relationship
        root->addChild(tipNode);
        tipNode->setParent(root);
    }

    //now put the tree together
    Tree* consensusTree = new Tree();
    consensusTree->setRoot(root, true);

    size_t nIndex = tipNames.size();

    for (size_t j = 0; j < cladeSamples.size(); j++)
    {
        size_t rIndex = cladeSamples.size() - 1 - j;    //reverse pass through because clades are sorted in ascending frequency
        float cladeFreq = cladeSamples[rIndex].getFrequency() / (float)(trace.size() - burnin);
        if (cladeFreq < cutoff)  break;

        Clade clade = cladeSamples[rIndex].getValue();

        //make sure we have an internal node
        if (clade.size() == 1 || clade.size() == tipNames.size())  continue;

        //find parent node
        std::vector<TopologyNode*> children;
        RbBitSet tmp;
        TopologyNode* parentNode = findParentNode(*root, clade, children, tmp );

        if(parentNode != NULL )
        {
            // skip this node if we've already found a clade compatible with it
            if( children.size() == parentNode->getNumberOfChildren() ) continue;

            std::vector<TopologyNode*> mrca;

            // find the mrca child if it exists
            if( !clade.getMrca().empty() )
            {
                for (size_t i = 0; i < children.size(); i++)
                {
                    if( children[i]->isTip() && std::find(clade.getMrca().begin(), clade.getMrca().end(), children[i]->getTaxon() ) != clade.getMrca().end() )
                    {
                        mrca.push_back(children[i]);
                    }
                }

                // if we couldn't find the mrca, then this clade is not compatible
                if( mrca.size() != clade.getMrca().size() )
                {
                    continue;
                }
                else
                {
                    for (size_t i = 0; i < mrca.size(); i++)
                    {
                        mrca[i]->setFossil(true);
                        mrca[i]->setSampledAncestor(true);
                    }
                }
            }

            nIndex++;   //increment node index
            TopologyNode* intNode = new TopologyNode(nIndex); //Topology node constructor, with proper node index
            intNode->setNodeType(false, false, true);


            // move the children to a new internal node
            for (size_t i = 0; i < children.size(); i++)
            {
                parentNode->removeChild(children[i]);
                intNode->addChild(children[i]);
                children[i]->setParent(intNode);
            }

            intNode->setParent(parentNode);
            parentNode->addChild(intNode);

            // add a mrca child if it exists and there is more than one non-mrca taxa
            if( !mrca.empty() && children.size() > 2 )
            {
                TopologyNode* old_parent = parentNode;

                nIndex++;   //increment node index
                parentNode = new TopologyNode(nIndex); //Topology node constructor, with proper node index
                parentNode->setNodeType(false, false, true);

                intNode->removeChild(mrca[0]);
                parentNode->addChild(mrca[0]);
                mrca[0]->setParent(parentNode);

                old_parent->removeChild(intNode);
                old_parent->addChild(parentNode);
                parentNode->setParent(old_parent);

                parentNode->addChild(intNode);
                intNode->setParent(parentNode);
            }
        }

        root->setTree(consensusTree);
    }

    //now put the tree together
    consensusTree->setRoot(root, true);

    report.ages      = true;
    report.cc_ages   = false;
    report.ccp       = false;
    report.tree_ages = false;
    annotateTree(*consensusTree, report, verbose );

    return consensusTree;
}


void TreeSummary::printCladeSummary(std::ostream &o, double minCladeProbability, bool verbose)
{
    summarize( verbose );
    
    std::stringstream ss;
    ss << std::fixed;
    ss << std::setprecision(4);
    
    o << std::endl;
    o << "=========================================" << std::endl;
    o << "Printing Posterior Distribution of Clades" << std::endl;
    o << "=========================================" << std::endl;
    o << std::endl;
    
    // now the printing
    std::string s = "Samples";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << "\n" << s;
    s = "Posterior";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;
    /*s = "ESS";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;*/
    s = "Clade";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;
    o << std::endl;
    o << "--------------------------------------------------------------" << std::endl;
    
    double totalSamples = trace.size();
    
    for (std::vector<Sample<Clade> >::reverse_iterator it = cladeSamples.rbegin(); it != cladeSamples.rend(); ++it)
    {
        size_t num_taxa = it->getValue().size();

        if( num_taxa == 1 ) continue;

        double freq =it->getFrequency();
        double p =it->getFrequency()/(totalSamples-burnin);
        
        
        if ( p < minCladeProbability )
        {
            break;
        }
        
        ss.str(std::string());
        ss << freq;
        s = ss.str();
        StringUtilities::fillWithSpaces(s, 16, true);
        o << s;
        
        ss.str(std::string());
        ss << p;
        s = ss.str();
        StringUtilities::fillWithSpaces(s, 16, true);
        o << s;
        
        /*ss.str(std::string());
        if ( it->getFrequency() <  totalSamples - burnin && it->getFrequency() > 0 )
        {
            ss << it->getEss();
        }
        else
        {
            ss << " - ";
            
        }
        s = ss.str();
        StringUtilities::fillWithSpaces(s, 16, true);
        o << s;*/
        
        o << it->getValue();
        o << std::endl;
        
    }
    
    o << std::endl;
    o << std::endl;
    
}



void TreeSummary::printTreeSummary(std::ostream &o, double credibleIntervalSize, bool verbose)
{
    summarize( verbose );
    
    std::stringstream ss;
    ss << std::fixed;
    ss << std::setprecision(4);
    
    o << std::endl;
    o << "========================================" << std::endl;
    o << "Printing Posterior Distribution of Trees" << std::endl;
    o << "========================================" << std::endl;
    o << std::endl;
    
    // now the printing
    std::string s = "Cum. Prob.";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;
    s = "Samples";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;
    s = "Posterior";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;
    /*s = "ESS";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;*/
    s = "Tree";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;
    o << std::endl;
    o << "----------------------------------------------------------------" << std::endl;
    double totalSamples = trace.size();
    double totalProb = 0.0;
    for (std::vector<Sample<std::string> >::reverse_iterator it = treeSamples.rbegin(); it != treeSamples.rend(); ++it)
    {
        double freq =it->getFrequency();
        double p =it->getFrequency()/(totalSamples-burnin);
        totalProb += p;
        
        ss.str(std::string());
        ss << totalProb;
        s = ss.str();
        StringUtilities::fillWithSpaces(s, 16, true);
        o << s;
        
        ss.str(std::string());
        ss << freq;
        s = ss.str();
        StringUtilities::fillWithSpaces(s, 16, true);
        o << s;
        
        ss.str(std::string());
        ss << p;
        s = ss.str();
        StringUtilities::fillWithSpaces(s, 16, true);
        o << s;
        
        /*ss.str(std::string());
        ss << it->getEss();
        s = ss.str();
        StringUtilities::fillWithSpaces(s, 16, true);
        o << s;*/
        
        o << it->getValue();
        o << std::endl;
        
        if ( totalProb >= credibleIntervalSize )
        {
            break;
        }
        
    }
    
    o << std::endl;
    o << std::endl;
    
}


void TreeSummary::setBurnin(int b)
{
    size_t old = burnin;

    // make sure burnin is proper
    if ( b >= static_cast<int>(trace.size()) && use_tree_trace)
    {
        throw RbException("Burnin size is too large for the tree trace.");
    }
    else if (b == -1)
    {
        burnin = trace.size() / 4;
    }
    else
    {
        burnin = size_t(b);
    }

    summarized = summarized && old == burnin;
}


int TreeSummary::size( bool post ) const
{

    double total_samples = trace.size();

    if (post) total_samples -= burnin;

    return total_samples;
}


void TreeSummary::summarize( bool verbose )
{
    if( summarized ) return;

    cladeAges.clear();
    conditionalCladeAges.clear();
    sampledAncestorSamples.clear();
    treeCladeAges.clear();
    
    std::map<Clade, Sample<Clade>, CladeComparator > cladeSampleMap( (CladeComparator(rooted)) );
    std::map<std::string, Sample<std::string> >      treeSampleMap;

    ProgressBar progress = ProgressBar(trace.size(), burnin);
    if ( verbose )
    {
        RBOUT("Summarizing clades ...\n");
        progress.start();
    }
    
    std::string outgroup = trace.objectAt(0).getTipNames()[0];

    for (size_t i = burnin; i < trace.size(); ++i)
    {
        
        if ( verbose )
        {
            progress.update(i);
        }
        
        Tree tree = trace.objectAt(i);

        if( rooted == false )
        {
            tree.reroot( outgroup, true );
        }

        std::string newick = TreeUtilities::uniqueNewickTopology( tree );

        Sample<std::string> treeSample(newick, 0);
        treeSample.setTrace(std::vector<double>(i - burnin, 0.0));
        treeSampleMap.insert(std::pair<std::string, Sample<std::string> >(newick, treeSample));
        treeCladeAges.insert(std::pair<std::string, std::map<Clade, std::vector<double>, CladeComparator > >( newick, std::map<Clade, std::vector<double>, CladeComparator >( (CladeComparator(rooted)) ) ) );

        // add empty observations for all non-matching trees
        for (std::map<std::string, Sample<std::string> >::iterator it = treeSampleMap.begin(); it != treeSampleMap.end(); ++it)
        {
            if (it->first == newick)
            {
                it->second.addObservation(true);
            }
            else
            {
                it->second.addObservation(false);
            }

        }
        
        // get the clades for this tree
        std::map<Clade, std::set<Clade, CladeComparator>, CladeComparator> condClades;
        fillConditionalClades(tree.getRoot(), condClades);
        
        // collect clade ages and increment the clade frequency counter
        for (std::map<Clade, std::set<Clade, CladeComparator>, CladeComparator >::iterator it=condClades.begin(); it!=condClades.end(); ++it )
        {
            const Clade& c = it->first;

            // insert a new sample
            Sample<Clade> cladeSample = Sample<Clade>(c, 0);
            cladeSample.setTrace(std::vector<double>(i - burnin, 0.0));
            // insert silently fails if clade has already been seen
            cladeSampleMap.insert(std::pair<Clade, Sample<Clade> >(c, cladeSample));

            // store the age for this clade
            // or create a new entry for the age of the clade
            cladeAges[c].push_back( c.getAge() );
            treeCladeAges[newick][c].push_back( c.getAge() );

            // this is an empty set if c is a tip node
            const std::set<Clade, CladeComparator>& children = it->second;

            // add conditional clade ages
            for (std::set<Clade, CladeComparator>::const_iterator child=children.begin(); child!=children.end(); ++child )
            {
                // inserts new entries if doesn't already exist
                conditionalCladeAges[c][*child].push_back( child->getAge() );
            }
        }

        for (std::map<Clade, Sample<Clade>, CladeComparator >::iterator it=cladeSampleMap.begin(); it!=cladeSampleMap.end(); ++it )
        {
            if ( condClades.find( it->first ) != condClades.end() )
            {
                it->second.addObservation( true );
            }
            else
            {
                it->second.addObservation( false );
            }

        }

        // collect sampled ancestor probs
        for (size_t j = 0; j < tree.getNumberOfTips(); j++)
        {
            const TopologyNode& tip = tree.getTipNode(j);

            Taxon taxon = tip.getTaxon();

            Sample<Taxon> taxonSample(taxon, 0);
            taxonSample.setTrace(std::vector<double>(i - burnin, 0.0));

            sampledAncestorSamples.insert( std::pair<Taxon, Sample<Taxon> >(taxon, taxonSample) );

            sampledAncestorSamples[taxon].addObservation( tip.isSampledAncestor() );
        }
    }
    
    // finish progress bar
    if ( verbose )
    {
        progress.finish();
        RBOUT("Collecting samples ...\n");
    }

    // collect the samples
    cladeSamples.clear();
    for (std::map<Clade, Sample<Clade>, CladeComparator >::iterator it = cladeSampleMap.begin(); it != cladeSampleMap.end(); ++it)
    {
        it->second.computeStatistics();
        cladeSamples.push_back(it->second);
    }
    
    // sort the samples by frequency
    VectorUtilities::sort( cladeSamples );
    

    // collect the samples
    treeSamples.clear();
    for (std::map<std::string, Sample<std::string> >::iterator it = treeSampleMap.begin(); it != treeSampleMap.end(); ++it)
    {
        it->second.computeStatistics();
        treeSamples.push_back(it->second);
    }

    // sort the samples by frequency
    VectorUtilities::sort( treeSamples );


    // collect the samples
    bool using_sampled_ancestors = false;

    for (std::map<Taxon, Sample<Taxon> >::iterator it = sampledAncestorSamples.begin(); it != sampledAncestorSamples.end(); ++it)
    {
        it->second.computeStatistics();

        using_sampled_ancestors = using_sampled_ancestors || it->second.getFrequency() > 0;
    }

    if( using_sampled_ancestors == false ) sampledAncestorSamples.clear();

    summarized = true;
}


/**
 *
 * Summarizes sampled character histories for a vector of stochastic character map traces for each node of a given summary tree.
 *
 */
void TreeSummary::summarizeCharacterMaps(Tree input_tree, std::vector<AncestralStateTrace> &ancestralstate_traces, std::string filename, int burnin, bool verbose, std::string separator)
{
    // get the number of ancestral state samples
    size_t num_sampled_states = ancestralstate_traces[0].getValues().size();
    
    // get the number of sampled trees
    size_t num_sampled_trees;
    if (use_tree_trace == false)
    {
        // the ancestral states were sampled over the same tree
        num_sampled_trees = 1;
    }
    else
    {
        // the ancestral states were sampled over different trees each iteration
        num_sampled_trees = trace.size();
    }
    
    setBurnin(burnin);
    if ( burnin >= num_sampled_states )
    {
        throw RbException("Burnin size is too large for the ancestral state trace.");
    }
    
    if ( use_tree_trace == true &&  num_sampled_trees != num_sampled_states )
    {
        throw RbException("The tree trace and the ancestral state trace must contain the same number of samples.");
    }
    
    std::vector<TopologyNode*> summary_nodes;
    bool condition_on_tree = false;

    if ( input_tree.getNumberOfTips() != 0 )
    {
        summary_nodes = input_tree.getNodes();
        condition_on_tree = true;
    }

    if ( condition_on_tree == false && use_tree_trace == false )
    {
        throw RbException("You must enter a tree trace, a tree, or both to summarize stochastic character maps.");
    }
    
    // get the number of nodes
    size_t num_nodes = 0;
    if (condition_on_tree == true)
    {
        num_nodes = input_tree.getNumberOfNodes();
    }
    else
    {
        num_nodes = trace.objectAt(0).getNumberOfNodes();
    }
    
    std::stringstream ss;
    ss << "Compiling stochastic character maps from " << num_sampled_states << " samples in the character map trace, using a burnin of " << burnin << " samples.\n";
    RBOUT(ss.str());

    ProgressBar progress = ProgressBar( num_nodes * num_sampled_states, 0 );
    if ( verbose == true )
    {
        progress.start();
    }

    // open stream for file output
    RbFileManager f = RbFileManager(filename);
    f.createDirectoryForFile();
    std::fstream out;
    out.open( filename.c_str(), std::fstream::out);
    
    // write column headers
    out << "iteration" << separator; 
    out << "node_index" << separator; 
    out << "branch_start_time" << separator;
    out << "branch_end_time" << separator;
    out << "start_state" << separator;
    out << "end_state" << separator;
    out << "transition_time" << separator;
    out << "transition_type" << std::endl; 

    // get the iteration trace
    AncestralStateTrace iteration_trace;
    bool trace_found = false;
    for (size_t k = 0; k < ancestralstate_traces.size(); k++)
    {
        if (ancestralstate_traces[k].getParameterName() == "Iteration")
        {
            iteration_trace = ancestralstate_traces[k];
            trace_found = true;
            break;
        }
    }
    const std::vector<std::string>& iteration_vector = iteration_trace.getValues();
    if ( trace_found == false )
    {
        throw RbException("The ancestral state trace is missing the 'Iteration' column.");
    }

    // loop through all nodes
    for (size_t i = 0; i < num_nodes; ++i)
    {
        size_t sample_clade_index = i;
        trace_found = false;
        AncestralStateTrace character_map_trace;
        
        // loop through all the stochastic character map samples
        for (size_t j = burnin; j < num_sampled_states; ++j)
        {
            
            if ( verbose == true )
            {
                progress.update( i * num_sampled_states + num_sampled_states * (j - burnin) / (num_sampled_states - burnin) );
            }
            
            // if necessary, get the sampled tree from the tree trace
            const Tree &sample_tree = (use_tree_trace) ? trace.objectAt( j ) : input_tree;

            if ( condition_on_tree == true )
            {
                if ( use_tree_trace == true )
                {
                    // check if the clade in the summary tree is also in the sampled tree
                    const TopologyNode& sample_root = sample_tree.getRoot();
                    sample_clade_index = sample_root.getCladeIndex( summary_nodes[i] );

                    // and we must also find the trace for this node index
                    trace_found = false;
                }
                else
                {
                    sample_clade_index = summary_nodes[i]->getIndex();
                }
            }

            if ( RbMath::isFinite( sample_clade_index ) == true )
            {
                
                // if necessary find the AncestralStateTrace for the sampled node
                if ( trace_found == false )
                {
                    for (size_t k = 0; k < ancestralstate_traces.size(); k++)
                    {
                        if (ancestralstate_traces[k].getParameterName() == StringUtilities::toString(sample_clade_index + 1))
                        {
                            character_map_trace = ancestralstate_traces[k];
                            trace_found = true;
                            break;
                        }
                    }
                }
                
                // get the sampled character history for this node for this iteration
                const std::vector<std::string>& ancestralstate_vector = character_map_trace.getValues();
                std::string character_history = ancestralstate_vector[j];

                // get the iteration
                std::string iteration = iteration_vector[j];

                // parse sampled SIMMAP string
                std::vector< std::pair<size_t, double> > this_branch_map = parseSIMMAPForNode(character_history);

                double start_time = sample_tree.getNode( sample_clade_index ).getAge() + sample_tree.getNode( sample_clade_index ).getBranchLength();
                double end_time = sample_tree.getNode( sample_clade_index ).getAge();

                double current_time = start_time;
                size_t current_state;
                size_t end_state;
                if ( this_branch_map.size() > 0 )
                {
                    current_state = this_branch_map[0].first;
                    end_state = this_branch_map[ this_branch_map.size() - 1 ].first;
                }
                else
                {
                    throw RbException("There were no sampled character histories for node " + StringUtilities::toString(sample_clade_index + 1) + " in the summary tree.");
                }
                
                // write output for each transition along the branch in forward time (towards tips)
                for (size_t k = 1; k < this_branch_map.size(); k++)
                {
                    if (this_branch_map[k].first != current_state)
                    {
                        // write node index
                        out << iteration << separator; 
                        
                        // write node index
                        if (condition_on_tree && use_tree_trace)
                        {
                            out << summary_nodes[i]->getIndex() + 1 << separator;
                        }
                        else
                        {
                            out << sample_clade_index + 1 << separator; 
                        }

                        // write branch start/end times
                        out << start_time << separator;
                        out << end_time << separator;
                        
                        // write start state
                        out << current_state << separator;
                        
                        // write end state
                        out << this_branch_map[k].first << separator;

                        // write transition time
                        out << current_time - this_branch_map[k - 1].second << separator;

                        out << "anagenetic" << std::endl; 
                        
                    }
                    current_state = this_branch_map[k].first;
                    current_time = current_time - this_branch_map[k - 1].second;
                }   

                // now check this node's children's start states to see if there were any cladogenetic transitions
                std::vector<int> children_indices = sample_tree.getNode( sample_clade_index ).getChildrenIndices();

                for (int k = 0; k < children_indices.size(); k++)
                {
                    size_t child_index = children_indices[k];

                    for (size_t l = 0; l < ancestralstate_traces.size(); l++)
                    {
                        if (ancestralstate_traces[l].getParameterName() == StringUtilities::toString(child_index + 1))
                        {
                            character_map_trace = ancestralstate_traces[l];
                            break;
                        }
                    }
                
                    // get the sampled character history for the child for this iteration
                    const std::vector<std::string>& ancestralstate_vector_child = character_map_trace.getValues();
                    std::string character_history_child = ancestralstate_vector_child[j];

                    // parse sampled SIMMAP string
                    std::vector< std::pair<size_t, double> > child_branch_map = parseSIMMAPForNode(character_history_child);
                   
                    // get child's start state
                    size_t child_start_state = child_branch_map[0].first;

                    if (end_state != child_start_state)
                    {
                        // write node index
                        out << iteration << separator; 
                        
                        // write node index
                        if (condition_on_tree && use_tree_trace)
                        {
                            out << summary_nodes[i]->getIndex() + 1 << separator;
                        }
                        else
                        {
                            out << sample_clade_index + 1 << separator; 
                        }
                        
                        // write branch start/end times
                        out << start_time << separator;
                        out << end_time << separator;
                        
                        // write start state
                        out << end_state << separator;
                        
                        // write end state
                        out << child_start_state << separator;
                        
                        // write transition time
                        out << end_time << separator;

                        out << "cladogenetic" << std::endl; 
                    }
                }
                        
            }
        }
    }  
   
    out.close();

    if ( verbose == true )
    {
        progress.finish();
    }

}

/*
 * Sort rooted clades as normal, or by mrcas
 * Sort unrooted clades (splits) by min(bitset,~bitset)
 */
bool CladeComparator::operator()(const Clade& a, const Clade& b) const
{
    const RbBitSet& ab = a.getBitRepresentation();
    const RbBitSet& bb = b.getBitRepresentation();

    // If clades are rooted or clades are from different sized trees,
    // do taxon-wise comparison, taking mrcas into account.
    if( rooted || ab.size() != bb.size() )
    {
        if( ab.size() != bb.size() || a.getMrca() == b.getMrca() )
        {
            return a < b;
        }
        else
        {
            return a.getMrca() < b.getMrca();
        }
    }

    if( ab.empty() || bb.empty() )
    {
        throw(RbException("Cannot compare unrooted clades (splits) with empty bitsets"));
    }

    // do bitwise comparison

    // if the first bit is 1, negate the bitset
    bool neg_ab = ab[0];
    bool neg_bb = bb[0];

    for(size_t i = 0; i < ab.size(); i++)
    {
        bool vab = ab[i];
        bool vbb = bb[i];

        // get bit from min bitset for each clade
        bool mab = neg_ab ? !vab : vab;
        bool mbb = neg_bb ? !vbb : vbb;

        // return result from first mismatch
        if(mab != mbb)
        {
            return mab < mbb;
        }
    }

    // ignore mrca in unrooted comparison
    return false;
}

/*
 * Rooted clades are equal iff their taxa and mrcas are equal
 * Unrooted splits are equal iff their taxa are equal or the intersection of their bitsets is empty
 */
bool CladeComparator::operator()(const Sample<Clade>& s) const
{
    const Clade& a = s.getValue();

    const RbBitSet& ab = a.getBitRepresentation();
    const RbBitSet& bb = clade.getBitRepresentation();

    // If clades are rooted or clades are from different sized trees,
    // do taxon-wise comparison, taking mrcas into account.
    if( rooted || ab.size() != bb.size() )
    {
        return a == clade && a.getMrca() == clade.getMrca();
    }

    if( ab.empty() || bb.empty() )
    {
        throw RbException("Cannot compare unrooted clades (splits) with empty bitsets");
    }

    // do bitwise comparison

    // if the first bit is 1, negate the bitset
    bool neg_ab = ab[0];
    bool neg_bb = bb[0];

    for (size_t i = 0; i < ab.size(); i++)
    {
        bool vab = ab[i];
        bool vbb = bb[i];

        // get bit from min bitset for each clade
        bool mab = neg_ab ? !vab : vab;
        bool mbb = neg_bb ? !vbb : vbb;

        // return result from first mismatch
        if (mab != mbb)
        {
            return false;
        }
    }

    // ignore mrca in unrooted comparison
    return true;
}

