#include "ProgressBar.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathLogic.h"
#include "RbVectorUtilities.h"
#include "Sample.h"
#include "StringUtilities.h"
#include "TopologyNode.h"
#include "TreeSummary.h"

#include <boost/lexical_cast.hpp>
#include <iomanip>
#include <vector>
#include <limits>
#include <cmath>


using namespace RevBayesCore;



TreeSummary::TreeSummary( void ) :
    burnin( 0 ),
    trace( false ),
    use_tree_trace( false )
{
    
}

TreeSummary::TreeSummary( const TraceTree &t ) :
    burnin( 0 ),
    trace( t ),
    use_tree_trace( true )
{
    
}


/*
 * This method calculates ancestral character states for the nodes on the input_summary_tree. It annotates the summary
 * tree with the posterior probabilities of the 3 most probable states. The method requires a vector of traces containing 
 * sampled ancestral states, and optionally will also work with a trace containing sampled trees that correspond to the 
 * ancestral state samples, enabling ancestral states to be estimated over a distribution of trees. In this case the annotated
 * posterior probability for a given character state is the probability of the node existing times the probability of the 
 * node being in the character state (see Pagel et al. 2004).
 */
Tree* TreeSummary::ancestralStateTree(const Tree &input_summary_tree, std::vector<AncestralStateTrace> &ancestralstate_traces, int b, std::string summary_stat, int site, bool verbose )
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
        throw RbException("Burnin size is too large for the ancestral state trace.");
    }
    
    if ( use_tree_trace == true &&  num_sampled_trees != num_sampled_states )
    {
        throw RbException("The tree trace and the ancestral state trace must contain the same number of samples.");
    }
    
    std::stringstream ss;
    ss << "Compiling " << summary_stat << " ancestral states from " << num_sampled_states << " samples in ancestral state trace, using a burnin of " << burnin << " samples.\n";
    RBOUT(ss.str());
    
    RBOUT("Calculating ancestral state posteriors...\n");
    
    // allocate memory for the new summary tree
    Tree* final_summary_tree = new Tree( input_summary_tree );
    
    // 2-d vectors to keep the data (posteriors and ancestral states) of the input_summary_tree nodes: [node][data]
    const std::vector<TopologyNode*> &summary_nodes = final_summary_tree->getNodes();
    std::vector<std::vector<double> > pp( summary_nodes.size(),std::vector<double>() );
    std::vector<std::vector<std::string> > states( summary_nodes.size(),std::vector<std::string>() );
    
    double weight = 1.0 / ( num_sampled_states - burnin );
    
    bool process_active = true;
    ProgressBar progress = ProgressBar( summary_nodes.size() * num_sampled_states, summary_nodes.size() * burnin );
    if ( verbose == true && process_active == true )
    {
        progress.start();
    }
        
    // loop through all nodes in the summary tree
    for (size_t i = 0; i < summary_nodes.size(); ++i)
    {
        size_t sample_clade_index;
        bool trace_found = false;
        AncestralStateTrace ancestralstate_trace;
        
        // loop through all the ancestral state samples
        for (size_t j = burnin; j < num_sampled_states; ++j)
        {
            
            if ( verbose == true && process_active == true )
            {
                progress.update( i * num_sampled_states + num_sampled_states * (j - burnin) / (num_sampled_states - burnin) );
            }
            
            // if necessary, get the sampled tree from the tree trace
            const Tree &sample_tree = (use_tree_trace) ? trace.objectAt( j ) : *final_summary_tree;
            const TopologyNode& sample_root = sample_tree.getRoot();
            
            if ( use_tree_trace == true )
            {
                // check if the clade in the summary tree is also in the sampled tree
                sample_clade_index = sample_root.getCladeIndex( summary_nodes[i] );
                
                // and we must also find the trace for this node index
                trace_found = false;
            }
            else
            {
                sample_clade_index = summary_nodes[i]->getIndex();
            }
            
            if ( RbMath::isFinite( sample_clade_index ) == true )
            {
                
                // if necessary find the AncestralStateTrace for the sampled node
                if ( trace_found == false )
                {
                    for (size_t k = 0; k < ancestralstate_traces.size(); ++k)
                    {
                        // if we have an ancestral state trace from an anagenetic-only process
                        if (ancestralstate_traces[k].getParameterName() == StringUtilities::toString(sample_clade_index + 1))
                        {
                            ancestralstate_trace = ancestralstate_traces[k];
                            trace_found = true;
                            break;
                        }
                        // if we have an ancestral state trace from a cladogenetic process
                        // if you need to annotate start states too, use cladoAncestralStateTree
                        if (ancestralstate_traces[k].getParameterName() == "end_" + StringUtilities::toString(sample_clade_index + 1))
                        {
                            ancestralstate_trace = ancestralstate_traces[k];
                            trace_found = true;
                            break;
                        }
                    }
                }
                
                // get the sampled ancestral state for this iteration
                const std::vector<std::string>& ancestralstate_vector = ancestralstate_trace.getValues();
                std::string ancestralstate = getSiteState( ancestralstate_vector[j], site );
                
                bool state_found = false;
                size_t k = 0;
                for (; k < pp[i].size(); k++)
                {
                    if ( states[i][k] == ancestralstate )
                    {
                        state_found = true;
                        break;
                    }
                }
                // update the pp and states vectors
                if ( state_found == false )
                {
                    pp[i].push_back(weight);
                    states[i].push_back(ancestralstate);
                }
                else
                {
                    pp[i][k] += weight;
                }
            }
        }
    }
    
//    // loop through all the ancestral state samples
//    for (size_t i = burnin; i < num_sampled_states; ++i)
//    {
//        
//        if ( verbose == true && process_active == true )
//        {
//            progress.update( i );
//        }
//        
//        // if necessary, get the sampled tree from the tree trace
//        const Tree &sample_tree = (use_tree_trace) ? trace.objectAt( i ) : *final_summary_tree;
//        const TopologyNode& sample_root = sample_tree.getRoot();
//
//        // loop through all nodes in the summary tree
//        for (size_t j = 0; j < summary_nodes.size(); ++j)
//        {
//            
//            size_t sample_clade_index;
//            if ( use_tree_trace == true )
//            {
//                // check if the clade in the summary tree is also in the sampled tree
//                sample_clade_index = sample_root.getCladeIndex( summary_nodes[j] );
//            }
//            else
//            {
//                sample_clade_index = summary_nodes[j]->getIndex();
//            }
//            
//            if ( RbMath::isFinite( sample_clade_index ) == true )
//            {
//                
//                // get AncestralStateTrace for the sampled node
//                AncestralStateTrace ancestralstate_trace;
//                for (size_t k = 0; k < ancestralstate_traces.size(); ++k)
//                {
//                    // if we have an ancestral state trace from an anagenetic-only process
//                    if (ancestralstate_traces[k].getParameterName() == StringUtilities::toString(sample_clade_index + 1))
//                    {
//                        ancestralstate_trace = ancestralstate_traces[k];
//                        break;
//                    }
//                    // if we have an ancestral state trace from a cladogenetic process
//                    // if you need to annotate start states too, use cladoAncestralStateTree
//                    if (ancestralstate_traces[k].getParameterName() == "end_" + StringUtilities::toString(sample_clade_index + 1))
//                    {
//                        ancestralstate_trace = ancestralstate_traces[k];
//                        break;
//                    }
//                }
//                
//                // get ancestral state vector for this iteration
//                const std::vector<std::string>& ancestralstate_vector = ancestralstate_trace.getValues();
//                std::string ancestralstate = getSiteState( ancestralstate_vector[i], site );
//                
//                bool state_found = false;
//                int k = 0;
//                for (; k < pp[j].size(); k++)
//                {
//                    if (states[j][k] == ancestralstate)
//                    {
//                        state_found = true;
//                        break;
//                    }
//                }
//                // update the pp and states vectors
//                if ( state_found == false )
//                {
//                    pp[j].push_back(weight);
//                    states[j].push_back(ancestralstate);
//                }
//                else
//                {
//                    pp[j][k] += weight;
//                }
//            }
//        }
//    }
    
    if ( verbose == true && process_active == true )
    {
        progress.finish();
    }

    
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
        final_summary_tree->addNodeParameter("posterior", posteriors, true);
        final_summary_tree->addNodeParameter("anc_state_1", anc_state_1, false);
        final_summary_tree->addNodeParameter("anc_state_2", anc_state_2, true);
        final_summary_tree->addNodeParameter("anc_state_3", anc_state_3, true);
        final_summary_tree->addNodeParameter("anc_state_1_pp", anc_state_1_pp, false);
        final_summary_tree->addNodeParameter("anc_state_2_pp", anc_state_2_pp, true);
        final_summary_tree->addNodeParameter("anc_state_3_pp", anc_state_3_pp, true);
        final_summary_tree->addNodeParameter("anc_state_other_pp", anc_state_other_pp, true);
    
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
 * differs from ancestralStateTree by calculating the MAP states resulting from a cladogenetic event,
 * so for each node the MAP state includes the end state and the starting states for the two daughter lineages.
 */
Tree* TreeSummary::cladoAncestralStateTree(const Tree &input_summary_tree, std::vector<AncestralStateTrace> &ancestralstate_traces, int b, std::string summary_stat, int site, bool verbose )
{
    setBurnin(b);
    
    std::stringstream ss;
    ss << "Compiling " << summary_stat << " ancestral states from " << trace.size() << " samples in ancestral state trace, using a burnin of " << burnin << " samples.\n";
    RBOUT(ss.str());
    
    RBOUT("Calculating ancestral state posteriors...\n");
    
    // allocate memory for the new tree
    Tree* final_summary_tree = new Tree(input_summary_tree);
    
    // 2-d vectors to keep the data (posteriors and states) of the inputTree nodes: [node][data]
    const std::vector<TopologyNode*> &summary_nodes = final_summary_tree->getNodes();
    std::vector<std::vector<double> > pp( summary_nodes.size(), std::vector<double>() );
    std::vector<std::vector<std::string> > end_states( summary_nodes.size(), std::vector<std::string>() );
    std::vector<std::vector<std::string> > start_states( summary_nodes.size(), std::vector<std::string>() );
    
    double weight = 1.0 / (trace.size() - burnin);
    
    bool process_active = true;
    ProgressBar progress = ProgressBar(trace.size(), burnin);
    if ( verbose == true && process_active == true )
    {
        progress.start();
    }
    
    // first gather all the samples from the traces
    
    // loop through all trees in tree trace
    for (size_t i = burnin; i < trace.size(); i++)
    {
        
        if ( verbose == true && process_active == true)
        {
            progress.update(i);
        }
        
        const Tree &sample_tree = trace.objectAt( i );
        const TopologyNode& sample_root = sample_tree.getRoot();
        
        // loop through all nodes in inputTree
        for (size_t j = 0; j < summary_nodes.size(); j++)
        {
            // check if this node is in the sampled tree
            size_t sample_clade_index = sample_root.getCladeIndex( summary_nodes[j] );
                
            if ( RbMath::isFinite( sample_clade_index ) == true )
            {
                
                size_t sample_clade_index_child_1 = 0;
                size_t sample_clade_index_child_2 = 0;
                
                if ( !summary_nodes[j]->isTip() )
                {
                    sample_clade_index_child_1 = sample_root.getCladeIndex( &summary_nodes[j]->getChild(0) );
                    sample_clade_index_child_2 = sample_root.getCladeIndex( &summary_nodes[j]->getChild(1) );
                }
                
                // get AncestralStateTrace for this node
                AncestralStateTrace ancestralstate_trace_end;
                AncestralStateTrace ancestralstate_trace_start_1;
                AncestralStateTrace ancestralstate_trace_start_2;
                
                bool found_end = false;
                bool found_start_1 = false;
                bool found_start_2 = false;
                
                for (size_t k = 0; k < ancestralstate_traces.size(); k++)
                {
                    if (ancestralstate_traces[k].getParameterName() == "end_" + StringUtilities::toString(sample_clade_index + 1))
                    {
                        ancestralstate_trace_end = ancestralstate_traces[k];
                        found_end = true;
                    }
                    
                    if ( !summary_nodes[j]->isTip() )
                    {
                        if (ancestralstate_traces[k].getParameterName() == "start_" + StringUtilities::toString(sample_clade_index_child_1 + 1))
                        {
                            ancestralstate_trace_start_1 = ancestralstate_traces[k];
                            found_start_1 = true;
                        }
                        
                        if (ancestralstate_traces[k].getParameterName() == "start_" + StringUtilities::toString(sample_clade_index_child_2 + 1))
                        {
                            ancestralstate_trace_start_2 = ancestralstate_traces[k];
                            found_start_2 = true;
                        }
                    }
                    else
                    {
                        found_start_1 = true;
                        found_start_2 = true;
                    }
                    
                    if (found_end && found_start_1 && found_start_2)
                    {
                        break;
                    }
                }
                
                // get ancestral states for this iteration
                std::vector<std::string> ancestralstate_trace_end_vector = ancestralstate_trace_end.getValues();
                std::string ancestralstate_end = getSiteState( ancestralstate_trace_end_vector[i], site );
                
                if ( !summary_nodes[j]->isTip() )
                {
                    std::vector<std::string> ancestralstate_trace_start_1_vector = ancestralstate_trace_start_1.getValues();
                    std::string ancestralstate_start_1 = getSiteState( ancestralstate_trace_start_1_vector[i], site );
                    
                    std::vector<std::string> ancestralstate_trace_start_2_vector = ancestralstate_trace_start_2.getValues();
                    std::string ancestralstate_start_2 = getSiteState( ancestralstate_trace_start_2_vector[i], site );
                    
                    size_t child1 = summary_nodes[j]->getChild(0).getIndex();
                    size_t child2 = summary_nodes[j]->getChild(1).getIndex();
                    
                    bool state_found = false;
                    int k = 0;
                    for (; k < pp[j].size(); k++)
                    {
                        if (end_states[j][k] == ancestralstate_end && start_states[child1][k] == ancestralstate_start_1 && start_states[child2][k] == ancestralstate_start_2)
                        {
                            state_found = true;
                            break;
                        }
                    }
                    // update the pp and states vectors
                    if (!state_found)
                    {
                        pp[j].push_back(weight);
                        end_states[j].push_back( ancestralstate_end );
                        start_states[child1].push_back( ancestralstate_start_1 );
                        start_states[child2].push_back( ancestralstate_start_2 );
                    }
                    else
                    {
                        pp[j][k] += weight;
                    }
                }
                else
                {
                    bool state_found = false;
                    int k = 0;
                    for (; k < pp[j].size(); k++)
                    {
                        if (end_states[j][k] == ancestralstate_end)
                        {
                            state_found = true;
                            break;
                        }
                    }
                    // update the pp and states vectors
                    if (!state_found)
                    {
                        pp[j].push_back(weight);
                        end_states[j].push_back( ancestralstate_end );
                    }
                    else
                    {
                        pp[j][k] += weight;
                    }
                }
            }
        }
    }
    
    if ( verbose == true && process_active == true )
    {
        progress.finish();
    }
    
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
        final_summary_tree->addNodeParameter("posterior", posteriors, true);
        
        final_summary_tree->addNodeParameter("end_state_1", end_state_1, false);
        final_summary_tree->addNodeParameter("end_state_2", end_state_2, true);
        final_summary_tree->addNodeParameter("end_state_3", end_state_3, true);
        final_summary_tree->addNodeParameter("end_state_1_pp", end_state_1_pp, false);
        final_summary_tree->addNodeParameter("end_state_2_pp", end_state_2_pp, true);
        final_summary_tree->addNodeParameter("end_state_3_pp", end_state_3_pp, true);
        final_summary_tree->addNodeParameter("end_state_other_pp", end_state_other_pp, true);

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

void TreeSummary::annotate( Tree &tree )
{
    
    RBOUT("Annotating tree ...");
    
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
            annotateContinuous(tree, pair[0], i, 0.95, true);
        }
        else
        {
            annotateDiscrete(tree, pair[0], i, 3, true);
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
            annotateContinuous(tree, pair[0], i, 0.95, false);
        }
        else
        {
            annotateDiscrete(tree, pair[0], i, 3, false);
        }
        
    }
    
}



/*
 * this method calculates the MAP ancestral character states for the nodes on the input_tree
 */
void TreeSummary::annotateDiscrete(Tree &tree, const std::string &n, size_t paramIndex, size_t num, bool isNodeParameter )
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
void TreeSummary::annotateContinuous(Tree &tree, const std::string &n, size_t paramIndex, double hpd, bool isNodeParameter )
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
            else
            {
                sample_root.containsClade(node, true);
                throw RbException("Clade not found!");
            }
            
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


void TreeSummary::annotateHPDAges(Tree &tree, double hpd, bool verbose )
{
    
    std::stringstream ss;
    ss << "Compiling " << hpd * 100 << "% HPD node ages from " << trace.size() << " total trees in tree trace, using a burnin of " << burnin << " trees.\n";
    RBOUT(ss.str());
    
    
    summarizeClades( true, verbose );
    
    const std::vector<TopologyNode*> &nodes = tree.getNodes();
    std::vector<std::string*> node_intervals(nodes.size());
    
    
    for (size_t i = 0; i < nodes.size(); i++)
    {
        // first get all the node ages for this node and sort them
        std::vector<Taxon> taxa;
        nodes[i]->getTaxa(taxa);
        Clade c(taxa); // clade age not used here
        std::map<Clade, std::vector<double> >::iterator entry_clade_age = cladeAges.find( c );
        
        // check that there is this clade
        // we may have ommited tip ages ...
        if ( entry_clade_age == cladeAges.end() ) continue;
        
        std::vector<double> branch_lengths = entry_clade_age->second;
        std::sort(branch_lengths.begin(), branch_lengths.end());
        
        size_t total_branch_lengths = branch_lengths.size();
        double min_range = std::numeric_limits<double>::max();
        
        size_t interval_start = 0;
        int interval_size = (int)(hpd * (double)total_branch_lengths);
        
        // find the smallest interval that contains x% of the samples
        for (size_t j = 0; j <= (total_branch_lengths - interval_size); j++)
        {
            double temp_lower = branch_lengths[j];
            double temp_upper = branch_lengths[j + interval_size - 1];
            double temp_range = std::fabs(temp_upper - temp_lower);
            if (temp_range < min_range)
            {
                min_range = temp_range;
                interval_start = j;
            }
        }
        double lower = branch_lengths[interval_start];
        double upper = branch_lengths[interval_start + interval_size - 1];
        
        // make node age annotation
        node_intervals[i] = new std::string("{" + StringUtilities::toString(lower)
                                            + "," + StringUtilities::toString(upper) + "}");
    }
    
    std::string label = "height_" + StringUtilities::toString( (int)(hpd * 100) ) + "%_HPD";
    tree.addNodeParameter(label, node_intervals, true);
    
}


void TreeSummary::annotateTree(RevBayesCore::Tree &reference_tree, bool clock)
{
    
    const std::vector<TopologyNode*> &nodes = reference_tree.getNodes();
    
    double sampleSize = trace.size() - burnin;
    
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        TopologyNode* n = nodes[i];
        if ( n->isTip() == false || clock == false )
        {
            // first we compute the posterior probability of the clade
            std::vector<Taxon> taxa;
            n->getTaxa(taxa);
            Clade c( taxa );
            
            double cladeFreq = findCladeSample( c ).getFrequency();
            double pp = cladeFreq / sampleSize;
            n->addNodeParameter("posterior",pp);
            
            // second we compute the conditional clade probability
            double ccp = 1.0;
            if ( n->isRoot() == false )
            {
                std::vector<Taxon> parentTaxa;
                n->getParent().getTaxa(parentTaxa);
                Clade parent( parentTaxa);
                std::map<Clade, std::vector<double> >& condCladeFreqs = conditionalCladeFrequencies[parent];
                double parentCladeFreq = findCladeSample( parent ).getFrequency();
                std::vector<double> condCladeSamples = condCladeFreqs[c];
                size_t condCladeSampleSize = condCladeSamples.size();
                ccp = condCladeSampleSize / parentCladeFreq;
                
            }
            n->addNodeParameter("ccp",ccp);
            
        }
        
    }
    
}


TopologyNode* TreeSummary::assembleConsensusTopology(std::vector<TopologyNode*> *nodes, std::vector<std::string> tipNames, std::vector<double> *pp, double cutoff)
{
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
        nodes->push_back(tipNode); //add the tip nodes to the node vector (they need to be first in the vector)
        pp->push_back(1.0);
    }
    
    nodes->push_back(root); //finally add the root node, is next index after the tips
    pp->push_back(1.0);
    
    size_t nIndex = tipNames.size();
    
    for (size_t j = 0; j < cladeSamples.size(); j++)
    {
        size_t rIndex = cladeSamples.size() - 1 - j;	//reverse pass through because clades are sorted in ascending frequency
        float cladeFreq = cladeSamples[rIndex].getFrequency() / (float)(trace.size() - burnin);
        if (cladeFreq < cutoff)  break;
        
        Clade clade = cladeSamples[rIndex].getValue();
        std::vector<Taxon> intNodeTaxa = clade.getTaxa();
        
        //make sure we have an internal node
        if (intNodeTaxa.size() == 1 || intNodeTaxa.size() == tipNames.size())  continue;
        
        nIndex++;	//increment node index
        TopologyNode* intNode = new TopologyNode(nIndex); //Topology node constructor, with proper node index
        intNode->setNodeType(false, false, true);
        
        //find parent node
        Taxon firstTaxon = intNodeTaxa.at(0);
        TopologyNode* parentNode = NULL;
        bool isCompatible = true;
        
        for (size_t l = 0; l < tipNames.size(); l++)
        {
            if (firstTaxon != nodes->at(l)->getTaxon() )
            {
                parentNode = &nodes->at(l)->getParent();
                break;
            }
        }
        while (parentNode != NULL)
        {
            std::vector<Taxon> subtendedTaxa;
            parentNode->getTaxa(subtendedTaxa);
            if (subtendedTaxa.size() >= intNodeTaxa.size())
            {
                //check if parent is compatible with new node
                size_t inBoth = 0;
                for (size_t ll = 0; ll < intNodeTaxa.size(); ll++)
                {
                    for (size_t lll = 0; lll < subtendedTaxa.size(); lll++)
                    {
                        if ( intNodeTaxa[ll] != subtendedTaxa[lll] )
                            inBoth++;
                    }
                }
                if (inBoth != intNodeTaxa.size()) {
                    isCompatible = false;
                }
                
                break;
            }
            parentNode = &parentNode->getParent();
        }
        
        if (!isCompatible)
        {
            delete intNode;
            nIndex--;
            continue;
        }
        
        //go through nodes subtended by parent, get children
        std::vector<TopologyNode*> children = parentNode->getChildren();
        std::vector<bool> isChild(children.size(), false);
        TopologyNode* childNode;
        
        for (size_t k = 0; k < children.size(); k++)
        {
            childNode = children[k];
            std::vector<Taxon> childTaxa;
            int found = 0;
            childNode->getTaxa(childTaxa);
            
            for (size_t i = 0; i < childTaxa.size(); i++)
            {
                for (size_t ii = 0; ii < intNodeTaxa.size(); ii++)
                {
                    if ( childTaxa[i] != intNodeTaxa[ii] )
                    {
                        found++;
                    }
                }
            }
            
            if (found == childTaxa.size())
            {
                isChild[k] = true;
            }
            else if (found > 0 && found < childTaxa.size()) {
                isCompatible = false;
                break;
            }
        }
        
        if (!isCompatible)
        {
            delete intNode;
            nIndex--;
            continue;
        }
        
        for (size_t kk = 0; kk < children.size(); kk++)
        {
            if (isChild[kk])
            {
                childNode = children[kk];
                parentNode->removeChild(childNode);
                intNode->addChild(childNode);
                childNode->setParent(intNode);
            }
        }
        intNode->setParent(parentNode);
        parentNode->addChild(intNode);
        nodes->push_back(intNode); //add internal node to node vector
        pp->push_back(cladeFreq);
    }
    
    return(root);
}



/*
 *	recursively calculates the median node age from the root, and tests that it is not older than its parent node
 */
void TreeSummary::calculateMedianAges(TopologyNode* n, double parentAge, std::vector<double> *ages)
{
    if (cladeAges.size() < 1)
        return;
    
    std::vector<Taxon> taxa;
    n->getTaxa(taxa);
    Clade c (taxa);
    c.setAge( n->getAge() );
    
    std::vector<double> ageVec = cladeAges.find(c)->second;
    std::sort(ageVec.begin(), ageVec.end());
    double medianAge;
    
    size_t nbValues = ageVec.size();
    if ((nbValues % 2) == 0)
    {
        medianAge = (ageVec[nbValues / 2] + ageVec[(nbValues / 2) - 1]) / 2.0;
    }
    else
    {
        medianAge = ageVec[nbValues / 2];
    }
    if (parentAge < medianAge) medianAge = parentAge;
    ages->at( n->getIndex() ) = medianAge;
    
    for (size_t i = 0; i < n->getNumberOfChildren(); i++)
    {
        
        TopologyNode *child = &(n->getChild(i));
        if ( !child->isTip() )
        {
            calculateMedianAges(child, medianAge, ages);
        }
        
    }
    
}


double TreeSummary::cladeProbability(const RevBayesCore::Clade &c ) const
{
    
    double n_samples = trace.size() - burnin;
    double occurences = 0.0;
    for (size_t i = burnin; i < trace.size(); ++i)
    {
        const Tree &tree = trace.objectAt(i);
        
        bool contains = tree.getRoot().containsClade(c, true);
        if ( contains == true )
        {
            ++occurences;
        }
        
    }
    
    return occurences / n_samples;
}



Tree* TreeSummary::conTree(double cutoff, bool verbose)
{
    
    std::stringstream ss;
    ss << "Compiling consensus tree from " << trace.size() << " trees in tree trace, using a burnin of " << burnin << " trees.\n";
    RBOUT(ss.str());
    
    //fill in clades, use all above 50% to resolve the bush with the consensus partitions
    summarizeClades( false, verbose );		//fills std::vector<Sample<std::string> > cladeSamples, sorts them by descending freq
    
    //set up variables for consensus tree assembly
    const Tree &temptree = trace.objectAt(0);
    std::vector<std::string> tipNames = temptree.getTipNames();
    std::vector<double> pp;
    std::vector<TopologyNode*> nodes;
    
    if (cutoff < 0.0 || cutoff > 1.0) cutoff = 0.5;
    
    ss << "Assembling clades into a MR consensus tree, using a " << cutoff << " cutoff...\n";
    RBOUT(ss.str());
    TopologyNode* root = assembleConsensusTopology(&nodes, tipNames, &pp, cutoff);
    
    //now put the tree together
    Tree* consensusTree = new Tree();
    consensusTree->addNodeParameter("posterior", pp, true);
    
    //calculate node ages for consensus tree
    RBOUT("Calculating node ages...\n");
    double maxAge = 2147483647; //just a very high nb, so that age of root will be smaller
    std::vector<double> ages(nodes.size(), 0.0);
    calculateMedianAges(root, maxAge, &ages);
    
    for (size_t x = 0; x < ages.size(); x++)
    {
        consensusTree->getNode(x).setAge(ages[x]);
    }
    
    return consensusTree;
}



//filling in clades and clade ages - including tip nodes in clade sample - to get age for serially sampled tips in time trees
Clade TreeSummary::fillClades(const TopologyNode &n, std::vector<Clade> &clades, bool clock)
{
    std::vector<Taxon> taxa;
    n.getTaxa(taxa);
    Clade parentClade( taxa );
    parentClade.setAge( (clock == true ? n.getAge() : n.getBranchLength() ) );
    clades.push_back(parentClade);
    
    if ( n.isTip() == false )
    {
        for (size_t i = 0; i < n.getNumberOfChildren(); i++)
        {
            const TopologyNode &childNode = n.getChild(i);
            Clade ChildClade = fillClades(childNode, clades, clock);
        }
    }
    
    return parentClade;
}


Clade TreeSummary::fillConditionalClades(const TopologyNode &n, std::vector<ConditionalClade> &condClades, std::vector<Clade> &clades, bool clock)
{
    
    std::vector<Taxon> taxa;
    n.getTaxa(taxa);
    
    double a = 0;
    if ( clock == true )
    {
        a = n.getAge();
    }
    else
    {
        a = n.getBranchLength();
    }
    Clade parent(taxa);
    parent.setAge( a );
    clades.push_back(parent);
    
    for (size_t i = 0; i < n.getNumberOfChildren(); i++)
    {
        const TopologyNode &childNode = n.getChild(i);
        if ( !childNode.isTip() )
        {
            Clade ChildClade = fillConditionalClades(childNode, condClades, clades, clock);
            ConditionalClade cc = ConditionalClade(parent, ChildClade);
            condClades.push_back(cc);
        }
        else if ( clock == false )
        {
            Clade ChildClade = fillConditionalClades(childNode, condClades, clades, clock);
            ConditionalClade cc = ConditionalClade(parent, ChildClade);
            condClades.push_back(cc);
        }
    }
    
    return parent;
}


Sample<Clade>& TreeSummary::findCladeSample(const Clade &n)
{
    
    for (std::vector<Sample<Clade> >::iterator it=cladeSamples.begin(); it!= cladeSamples.end(); ++it)
    {
        if ( it->getValue() == n )
        {
            return *it;
        }
        
    }
    
    throw RbException("Couldn't find a clade with name '" + n.toString() + "'.");
}


int TreeSummary::getNumberSamples( void ) const
{
    
    double total_samples = trace.size();
    
    return total_samples - burnin;
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



int TreeSummary::getTopologyFrequency(const RevBayesCore::Tree &tree) const
{
    
    std::string newick = TreeUtilities::uniqueNewickTopology( tree );
    
    double total_samples = trace.size();
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


std::vector<Tree> TreeSummary::getUniqueTrees( double credible_interval_size ) const
{
    
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
    
    bool clock = true;

    // use the default burnin
    int b = -1;
    setBurnin( b );
    
    summarizeClades( clock, verbose );
    summarizeTrees( verbose );
    
//    double meanRootAge = 0.0;
//    std::vector<double> rootAgeSamples;
//    if ( clock == true )
//    {
//        for (size_t i = burnin; i < trace.size(); ++i)
//        {
//            // get the sampled tree
//            const Tree &tree = trace.objectAt( i );
//            
//            // add this root age to our variable
//            meanRootAge += tree.getRoot().getAge();
//            
//            rootAgeSamples.push_back(tree.getRoot().getAge());
//            
//        }
//        
//    }
    
    RandomNumberGenerator *rng = GLOBAL_RNG;
   
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
            NewickConverter converter;
            Tree* current_tree = converter.convertFromNewick( current_sample );
            if ( current_tree->hasSameTopology( t ) )
            {
                delete current_tree;
                return true;
            }
            delete current_tree;
            
        }
        
        totalProb += p;
        
        
        if ( totalProb >= size )
        {
            break;
        }
        
    }
    
    return false;
}


Tree* TreeSummary::map( bool clock, bool verbose )
{
    bool useMean = true;
    // should we use the ages only from the best tree???
    bool use_best_tree = true;
    
    std::stringstream ss;
    ss << "Compiling MAP tree from " << trace.size() << " trees in tree trace, using a burnin of " << burnin << " trees.\n";
    RBOUT(ss.str());
    
    summarizeClades( clock, verbose );
    summarizeConditionalClades( clock, verbose );
    summarizeTrees( verbose );
    
    // get the tree with the highest posterior probability
    std::string bestNewick = treeSamples.rbegin()->getValue();
    NewickConverter converter;
    Tree* tmp_best_tree = converter.convertFromNewick( bestNewick );
    Tree* best_tree = NULL;
    if ( clock == true )
    {
        best_tree = TreeUtilities::convertTree( *tmp_best_tree );
    }
    else
    {
        best_tree = tmp_best_tree->clone();
    }
    size_t num_taxa = best_tree->getNumberOfTips();
    
    TaxonMap tm = TaxonMap( trace.objectAt(0) );
    best_tree->setTaxonIndices( tm );

    // now we summarize the clades for the best tree
    summarizeCladesForTree(*best_tree, clock, verbose);

    const std::vector<TopologyNode*> &nodes = best_tree->getNodes();
    
    
    double sampleSize = trace.size() - burnin;
    
    double meanRootAge = 0.0;
    std::vector<double> rootAgeSamples;
    if ( clock == true )
    {
        for (size_t i = burnin; i < trace.size(); ++i)
        {
            // get the sampled tree
            const Tree &tree = trace.objectAt( i );
        
            // add this root age to our variable
            meanRootAge += tree.getRoot().getAge();
        
            rootAgeSamples.push_back(tree.getRoot().getAge());
        
        }
        
    }
    
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        TopologyNode* n = nodes[i];
        if ( n->isTip() == false || clock == false )
        {
            // first we compute the posterior probability of the clade
            std::vector<Taxon> taxa;
            n->getTaxa(taxa);
            Clade c( taxa );
            
            double cladeFreq = findCladeSample( c ).getFrequency();
            double pp = cladeFreq / sampleSize;
            n->addNodeParameter("posterior",pp);
            
            // second we compute the conditional clade probability
            double ccp = 1.0;
            double age = 0.0;
            if ( !n->isRoot() )
            {
                std::vector<Taxon> parentTaxa;
                n->getParent().getTaxa(parentTaxa);
                Clade parent( parentTaxa );
                std::map<Clade, std::vector<double> >& condCladeFreqs = conditionalCladeFrequencies[parent];
                double parentCladeFreq = findCladeSample( parent ).getFrequency();
                std::vector<double> condCladeSamples = condCladeFreqs[c];
                size_t condCladeSampleSize = condCladeSamples.size();
                ccp = condCladeSampleSize / parentCladeFreq;
                
                if ( use_best_tree )
                {
                    const std::vector<double> &age_samples = cladeAgesOfBestTree[ c ];
                    size_t age_sample_size = age_samples.size();
                    // finally, we compute the mean conditional age
                    for (size_t i = 0; i<age_sample_size; ++i)
                    {
                        age += age_samples[i];
                    }
                    age /= age_sample_size;
                }
                else if ( useMean == true )
                {
                    // finally, we compute the mean conditional age
                    for (size_t i = 0; i<condCladeSampleSize; ++i)
                    {
                        age += condCladeSamples[i];
                    }
                    age /= condCladeSampleSize;
                }
                else
                {
                    
                    size_t idx = condCladeSampleSize / 2;
                    std::sort( condCladeSamples.begin(), condCladeSamples.end() );
                    if (condCladeSampleSize % 2 == 1)
                    {
                        age = condCladeSamples[idx];
                    }
                    else
                    {
                        age = (condCladeSamples[idx-1] + condCladeSamples[idx]) / 2;
                    }
                    
                }
                
            }
            else
            {
                if ( useMean == true )
                {
                    age = meanRootAge / sampleSize;
                }
                else
                {
                    std::sort( rootAgeSamples.begin(), rootAgeSamples.end() );
                    size_t idx = rootAgeSamples.size() / 2;
                    if (rootAgeSamples.size() % 2 == 1)
                    {
                        age = rootAgeSamples[idx];
                    }
                    else
                    {
                        age = (rootAgeSamples[idx-1] + rootAgeSamples[idx]) / 2;
                    }
                }
            }
            n->addNodeParameter("ccp",ccp);
            
            // finally, we compute the mean conditional age
            if ( clock == true )
            {
                best_tree->getNode( i ).setAge( age );
            }
            else
            {
                best_tree->getNode( i ).setBranchLength( age );
            }
            
        }
        
    }
    
    if ( clock == true )
    {
        const Tree &firstTree = trace.objectAt( 0 );
        const std::vector<TopologyNode*> &firstNodes = firstTree.getNodes();
        for (size_t i = 0; i < num_taxa; i++)
        {
            
            if (firstNodes[i]->isTip())
            {
                
                for (size_t j = 0; j < num_taxa; j++)
                {
                    if (firstNodes[i]->getName() == nodes[j]->getName())
                    {
                        best_tree->getNode( j ).setAge( firstNodes[i]->getAge() );
                    
                    }
            
                }
        
            }
    
        }
        
    }
    
    return best_tree;
}


void TreeSummary::printCladeSummary(std::ostream &o, double minCladeProbability)
{
    
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
    s = "ESS";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;
    s = "Clade";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;
    o << std::endl;
    o << "--------------------------------------------------------------" << std::endl;
    
    double totalSamples = trace.size();
    
    for (std::vector<Sample<Clade> >::reverse_iterator it = cladeSamples.rbegin(); it != cladeSamples.rend(); ++it)
    {
        
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
        
        ss.str(std::string());
        if ( it->getFrequency() <  totalSamples && it->getFrequency() > 0 )
        {
            ss << it->getEss();
        }
        else
        {
            ss << " - ";
            
        }
        s = ss.str();
        StringUtilities::fillWithSpaces(s, 16, true);
        o << s;
        
        o << it->getValue();
        o << std::endl;
        
    }
    
    o << std::endl;
    o << std::endl;
    
}



void TreeSummary::printTreeSummary(std::ostream &o, double credibleIntervalSize)
{
    
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
    s = "ESS";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;
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
        
        ss.str(std::string());
        ss << it->getEss();
        s = ss.str();
        StringUtilities::fillWithSpaces(s, 16, true);
        o << s;
        
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
}


void TreeSummary::summarizeClades( bool clock, bool verbose )
{
    
    std::map<Clade, Sample<Clade> > cladeAbsencePresence;
    
    bool process_active = true;
    ProgressBar progress = ProgressBar(trace.size(), burnin);
    if ( verbose == true && process_active == true )
    {
        RBOUT("Summarizing clades ...\n");
        progress.start();
    }
    
    std::string outgroup = "";
    for (size_t i = burnin; i < trace.size(); ++i)
    {
        
        if ( verbose == true && process_active == true)
        {
            progress.update(i);
        }
        
        const Tree &tree = trace.objectAt(i);
        
        // get the clades for this tree
        std::vector<Clade> clades;
        fillClades(tree.getRoot(), clades, clock);
        
        // collect clade ages and increment the clade frequency counter
        for (size_t j = 0; j < clades.size(); ++j)
        {
            const Clade & c = clades[j];
            
            if ( c.size() <= 1 ) continue;
            
            Sample<Clade> thisSample = Sample<Clade>(c, 0);
            
            const std::map<Clade, Sample<Clade> >::iterator& entry = cladeAbsencePresence.find(c);
            if (entry == cladeAbsencePresence.end())
            {
                
                Sample<Clade> cladeSample = Sample<Clade>(c, 0);
                if (i > burnin)
                {
                    cladeSample.setTrace(std::vector<double>(i - burnin, 0.0));
                }
                else
                {
                    cladeSample.setTrace(std::vector<double>());
                }
                cladeAbsencePresence.insert(std::pair<Clade, Sample<Clade> >(c, cladeSample));
                
                // create a new entry for the age of the clade
                std::vector<double> tempAgeVec;
                cladeAges.insert(std::pair<Clade, std::vector<double> >(c, tempAgeVec));
            }
            
            // store the age for this clade
            std::map<Clade, std::vector<double> >::iterator entry_age = cladeAges.find(c);
            entry_age->second.push_back(c.getAge());
            
        }
        
        for (std::map<Clade, Sample<Clade> >::iterator it=cladeAbsencePresence.begin(); it!=cladeAbsencePresence.end(); ++it )
        {
            bool found = false;
            for (size_t i = 0; i < clades.size(); ++i)
            {
                Clade c = clades[i];
                if ( it->first == c )
                {
                    found = true;
                    break;
                }
                
            }
            
            if ( found == true )
            {
                it->second.addObservation( true );
            }
            else
            {
                it->second.addObservation( false );
            }
            
        }
        
    }
    
    if ( verbose == true && process_active == true )
    {
        progress.finish();
    }
    
    // collect the samples
    cladeSamples.clear();
    for (std::map<Clade, Sample<Clade> >::iterator it = cladeAbsencePresence.begin(); it != cladeAbsencePresence.end(); ++it)
    {
        it->second.computeStatistics();
        cladeSamples.push_back(it->second);
    }
    
    // sort the samples by frequency
    VectorUtilities::sort( cladeSamples );
    
}


void TreeSummary::summarizeCladesForTree(const Tree &reference_tree, bool clock, bool verbose)
{
    
    cladeAgesOfBestTree.clear();
    
    bool process_active = true;
    ProgressBar progress = ProgressBar(trace.size(), burnin);
    if ( verbose == true && process_active == true )
    {
        RBOUT("Summarizing clades for tree ...\n");
        progress.start();
    }
    
    
    // get the newick string for the reference tree
    std::string reference_tree_newick = TreeUtilities::uniqueNewickTopology( reference_tree );
    
    std::string outgroup = "";
    for (size_t i = burnin; i < trace.size(); ++i)
    {
        
        if ( verbose == true && process_active == true)
        {
            progress.update(i);
        }
        
        const Tree &tree = trace.objectAt(i);
        
        // get the newick string for the current tree
        std::string current_tree_newick = TreeUtilities::uniqueNewickTopology( tree );

        // if this tree does not equal the reference tree then we skip it
        if ( current_tree_newick != reference_tree_newick )
        {
            continue;
        }
        
        // get the clades for this tree
        std::vector<Clade> clades;
        fillClades(tree.getRoot(), clades, clock);
        
        // collect clade ages and increment the clade frequency counter
        for (size_t j = 0; j < clades.size(); ++j)
        {
            const Clade & c = clades[j];
            
            if ( c.size() <= 1 && clock == true) continue;
                        
            const std::map<Clade, std::vector<double> >::iterator& entry = cladeAgesOfBestTree.find(c);
            if (entry == cladeAgesOfBestTree.end())
            {
                // create a new entry for the age of the clade
                std::vector<double> tempAgeVec;
                cladeAgesOfBestTree.insert(std::pair<Clade, std::vector<double> >(c, tempAgeVec));
            }
            
            // store the age for this clade
            std::map<Clade, std::vector<double> >::iterator entry_age = cladeAgesOfBestTree.find(c);
            entry_age->second.push_back(c.getAge());
            
        }
        
    }
    
    if ( verbose == true && process_active == true )
    {
        progress.finish();
    }
    
}


void TreeSummary::summarizeConditionalClades( bool clock, bool verbose )
{
    std::map<Clade, Sample<Clade> > cladeAbsencePresence;
    
    bool process_active = true;
    ProgressBar progress = ProgressBar(trace.size(), burnin);
    if ( verbose == true && process_active == true )
    {
        RBOUT("Summarizing conditional clades ...\n");
        progress.start();
    }
    
    for (size_t i = burnin; i < trace.size(); ++i)
    {
        
        if ( verbose == true && process_active == true)
        {
            progress.update(i);
        }
        
        const Tree &tree = trace.objectAt( i );
        
        // get the conditional clades for this
        std::vector<ConditionalClade> condClades;
        std::vector<Clade> clades;
        fillConditionalClades(tree.getRoot(), condClades, clades, clock);
        
        // first increment the clade frequency counter
        // there need to be two loops because otherwise we count the the parent clade twice
        for (size_t j = 0; j < clades.size(); ++j)
        {
            const Clade & c = clades[j];
            const std::map<Clade, Sample<Clade> >::iterator& entry = cladeAbsencePresence.find( c );
            if ( entry == cladeAbsencePresence.end() )
            {
                Sample<Clade> cladeSample = Sample<Clade>(c,0);
                if ( i > burnin )
                {
                    cladeSample.setTrace( std::vector<double>(i - burnin,0.0) );
                }
                else
                {
                    cladeSample.setTrace( std::vector<double>() );
                }
                cladeAbsencePresence.insert( std::pair<Clade, Sample<Clade> >(c, cladeSample));
                
                conditionalCladeFrequencies.insert( std::pair<Clade, std::map<Clade, std::vector<double> > >(c, std::map<Clade, std::vector<double> >()) );
                
            }
            
        }
        
        
        
        for (std::map<Clade, Sample<Clade> >::iterator it=cladeAbsencePresence.begin(); it!=cladeAbsencePresence.end(); ++it )
        {
            bool found = false;
            for (size_t j = 0; j < clades.size(); ++j)
            {
                Clade c = clades[j];
                if ( it->first == c )
                {
                    found = true;
                    break;
                }
                
            }
            
            if ( found == true )
            {
                it->second.addObservation( true );
            }
            else
            {
                it->second.addObservation( false );
            }
            
        }
        
        for (size_t j = 0; j < condClades.size(); ++j)
        {
            const ConditionalClade & cc = condClades[j];
            const Clade &parent = cc.getParent();
            const Clade &child  = cc.getChild();
            
            // now increment the conditional clade frequency counter
            double childAge = child.getAge();
            
            std::map<Clade, std::vector<double> >& parentEntry = conditionalCladeFrequencies.find( parent )->second;
            const std::map<Clade, std::vector<double> >::iterator& childEntry = parentEntry.find( child );
            if ( childEntry == parentEntry.end() )
            {
                parentEntry.insert( std::pair<Clade, std::vector<double> >(child, std::vector<double>(1,childAge) ));
            }
            else
            {
                std::vector<double> &samples = childEntry->second;
                samples.push_back( childAge );
            }
            
        }
        
    }
    
    if ( verbose == true && process_active == true )
    {
        progress.finish();
    }
    
    // collect the samples
    cladeSamples.clear();
    for (std::map<Clade, Sample<Clade> >::iterator it = cladeAbsencePresence.begin(); it != cladeAbsencePresence.end(); ++it)
    {
        it->second.computeStatistics();
        cladeSamples.push_back( it->second );
    }
    
    // sort the samples by frequency
    sort(cladeSamples.begin(), cladeSamples.end());
}


void TreeSummary::summarizeTrees( bool verbose )
{
    
    std::map<std::string, Sample<std::string> > treeAbsencePresence;
    
    bool process_active = true;
    ProgressBar progress = ProgressBar(trace.size(), burnin);
    if ( verbose == true && process_active == true )
    {
        RBOUT("Summarizing trees ...\n");
        progress.start();
    }
    
    std::string outgroup = "";
    for (size_t i = burnin; i < trace.size(); ++i)
    {
        
        if ( verbose == true && process_active == true)
        {
            progress.update(i);
        }
        
        const Tree &tree = trace.objectAt(i);
        std::string newick = TreeUtilities::uniqueNewickTopology( tree );
        
        const std::map<std::string, Sample<std::string> >::iterator& entry = treeAbsencePresence.find(newick);
        if (entry == treeAbsencePresence.end())
        {
            Sample<std::string> treeSample = Sample<std::string>(newick, 0);
            if (i > burnin)
            {
                treeSample.setTrace(std::vector<double>(i - burnin, 0.0));
            }
            else
            {
                treeSample.setTrace(std::vector<double>());
            }
            treeAbsencePresence.insert(std::pair<std::string, Sample<std::string> >(newick, treeSample));
        }
        
        for (std::map<std::string, Sample<std::string> >::iterator it = treeAbsencePresence.begin(); it != treeAbsencePresence.end(); ++it)
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
        
    }
    
    if ( verbose == true && process_active == true )
    {
        progress.finish();
    }
    
    // collect the samples
    treeSamples.clear();
    for (std::map<std::string, Sample<std::string> >::iterator it = treeAbsencePresence.begin(); it != treeAbsencePresence.end(); ++it)
    {
        it->second.computeStatistics();
        treeSamples.push_back(it->second);
    }
    
    // sort the samples by frequency
    sort(treeSamples.begin(), treeSamples.end());
    
}

