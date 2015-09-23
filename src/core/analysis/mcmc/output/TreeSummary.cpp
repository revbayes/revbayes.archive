#include "RbException.h"
#include "Sample.h"
#include "StringUtilities.h"
#include "TopologyNode.h"
#include "TreeSummary.h"

#include <iomanip>
#include <vector>
#include <limits>
#include <cmath>


using namespace RevBayesCore;



TreeSummary::TreeSummary( const TreeTrace &t) :
    burnin( 0 ),
    trace( t )
{
    
}

/*
 *	recursively calculates the median node age from the root, and tests that it is not older than its parent node
 */
void TreeSummary::calculateMedianAges(TopologyNode* n, double parentAge, std::vector<double> *ages)
{
    if (cladeAges.size() < 1)
        return;
    
    std::vector<std::string> taxa;
    n->getTaxaStringVector(taxa);
    Clade c (taxa, n->getAge());
    
    std::vector<double> ageVec = cladeAges.find(c.toString())->second;
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

/*
 * this method calculates the MAP ancestral character states for the nodes on the input_tree
 */
Tree* TreeSummary::ancestralStateTree(const Tree &inputTree, std::vector<AncestralStateTrace> &ancestralstate_traces, int b )
{
    setBurnin(b);
    
    std::stringstream ss;
    ss << "Compiling MAP ancestral states from " << trace.size() << " samples in ancestral state trace, using a burnin of " << burnin << " sample.\n";
    RBOUT(ss.str());
    
    RBOUT("Calculating ancestral state posteriors...\n");
    
    // allocate memory for the new tree
    Tree* finalInputTree = new Tree(inputTree);
    
    // 2-d vectors to keep the data (posteriors and states) of the inputTree nodes: [node][data]
    const std::vector<TopologyNode*> &input_nodes = finalInputTree->getNodes();
    std::vector<std::vector<double> > pp(input_nodes.size(),std::vector<double>());
    std::vector<std::vector<std::string> > states(input_nodes.size(),std::vector<std::string>());
    
    double weight = 1.0 / (trace.size()-burnin);
    
    // loop through all trees in tree trace
    for (size_t i = burnin; i < trace.size(); i++)
    {
        const Tree &sample_tree = trace.objectAt( i );
        const TopologyNode& sample_root = sample_tree.getRoot();
        
        // loop through all nodes in inputTree
        for (size_t j = 0; j < input_nodes.size(); j++)
        {
            if ( sample_root.containsClade(input_nodes[j], true) && !input_nodes[j]->isTip() )
            {
                // if the inputTree node is also in the sample tree
                // we get the ancestral character state from the ancestral state trace
                size_t sampleCladeIndex = sample_root.getCladeIndex( input_nodes[j] );
                
                // get AncestralStateTrace for this node
                AncestralStateTrace ancestralstate_trace;
                for (size_t k = 0; k < ancestralstate_traces.size(); k++)
                {
                    if (ancestralstate_traces[k].getParameterName() == StringUtilities::toString(sampleCladeIndex))
                    {
                        ancestralstate_trace = ancestralstate_traces[k];
                        break;
                    }
                }
                
                // get ancestral state vector for this iteration
                std::vector<std::string> ancestralstate_vector = ancestralstate_trace.getValues();
                std::string ancestralstate = ancestralstate_vector[i];
                
                bool state_found = false;
                int k = 0;
                for (; k < pp[j].size(); k++)
                {
                    if (states[j][k] == ancestralstate)
                    {
                        state_found = true;
                        break;
                    }
                }
                // update the pp and states vectors
                if (!state_found)
                {
                    pp[j].push_back(weight);
                    states[j].push_back(ancestralstate);
                }
                else
                {
                    pp[j][k] += weight;
                }
            }
        }
    }
    // find the 3 most probable ancestral states for each node and add them to the tree as parameters
    std::vector<std::string*> best_states;
    std::vector<double> posteriors;
    for (int i = 0; i < input_nodes.size(); i++)
    {
        
        if ( input_nodes[i]->isTip() )
        {
            
            std::string *s = new std::string("{}");
            best_states.push_back(s);
            posteriors.push_back(1.0);
            
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
            
            std::string final_state = "{";
            bool states = false;
            if (state1_pp > 0.0001)
            {
                final_state += state1 + "=" + StringUtilities::toString(state1_pp);
                states = true;
                if (state2_pp > 0.0001)
                {
                    final_state += "," + state2 + "=" + StringUtilities::toString(state2_pp);
                    if (state3_pp > 0.0001)
                    {
                        final_state += "," + state3 + "=" + StringUtilities::toString(state3_pp);
                        if (other_pp > 0.0001)
                        {
                            final_state += ",other=" + StringUtilities::toString(other_pp);
                        }
                    }
                }
            }
            if (1.0-total_node_pp > 0.0001)
            {
                if (states)
                {
                    final_state += ",";
                }
                final_state += "node_doesnt_exist=" + StringUtilities::toString(1.0-total_node_pp);
            }
            final_state += "}";
            
            // make parameter string for this node
            std::string *s = new std::string(final_state);
            best_states.push_back(s);
        }
    }
    
    finalInputTree->clearNodeParameters();
    //finalInputTree.addNodeParameter("posterior",posteriors,true);
    finalInputTree->addNodeParameter("ancestralstates",best_states,true);
    
    return finalInputTree;
}


void TreeSummary::annotate(Tree &tree, int b )
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
            annotateContinuous(tree, pair[0], i, 0.95, b, true);
        }
        else
        {
            annotateDiscrete(tree, pair[0], i, 3, b, true);
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
            annotateContinuous(tree, pair[0], i, 0.95, b, false);
        }
        else
        {
            annotateDiscrete(tree, pair[0], i, 3, b, false);
        }
        
    }
    
}



/*
 * this method calculates the MAP ancestral character states for the nodes on the input_tree
 */
void TreeSummary::annotateDiscrete(Tree &tree, const std::string &n, size_t paramIndex, size_t num, int b, bool isNodeParameter )
{
    
    setBurnin(b);
    
    // 2-d vectors to keep the data (posteriors and states) of the inputTree nodes: [node][data]
    const std::vector<TopologyNode*> &input_nodes = tree.getNodes();
    std::vector<std::map<std::string, Sample<std::string> > > stateAbsencePresence(input_nodes.size(), std::map<std::string, Sample<std::string> >());
    
    bool interiorOnly = true;
    bool tipsChecked = false;
    //    bool useRoot = true;
    
    // loop through all trees in tree trace
    for (size_t iteration = burnin; iteration < trace.size(); ++iteration)
    {
        const Tree &sample_tree = trace.objectAt( iteration );
        const TopologyNode& sample_root = sample_tree.getRoot();
        
        // loop through all nodes in inputTree
        for (size_t node_index = 0; node_index < input_nodes.size(); ++node_index)
        {
            TopologyNode *node = input_nodes[node_index];
            
            if ( node->isTip() == true )
            {
                if ( tipsChecked == false )
                {
                    
                    tipsChecked = true;
                    size_t sampleCladeIndex = sample_root.getCladeIndex( node );
                    
                    const TopologyNode &sample_node = sample_tree.getNode( sampleCladeIndex );
                    
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
                size_t sampleCladeIndex = sample_root.getCladeIndex( node );
                
                const TopologyNode &sample_node = sample_tree.getNode( sampleCladeIndex );
                
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
    for (int i = 0; i < input_nodes.size(); i++)
    {
        
        TopologyNode &node = *input_nodes[i];
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
void TreeSummary::annotateContinuous(Tree &tree, const std::string &n, size_t paramIndex, double hpd, int b, bool isNodeParameter )
{
    setBurnin(b);
    
    // 2-d vectors to keep the data (posteriors and states) of the inputTree nodes: [node][data]
    const std::vector<TopologyNode*> &input_nodes = tree.getNodes();
    std::vector<std::vector<double> > samples(input_nodes.size(),std::vector<double>());
    
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
        for (size_t j = 0; j < input_nodes.size(); j++)
        {
            TopologyNode *node = input_nodes[j];
            if ( node->isTip() == true )
            {
                if ( tipsChecked == false )
                {
                    
                    tipsChecked = true;
                    size_t sampleCladeIndex = sample_root.getCladeIndex( node );
                    
                    const TopologyNode &sample_node = sample_tree.getNode( sampleCladeIndex );
                    
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
                    
                    size_t sampleCladeIndex = sample_root.getCladeIndex( node );
                    
                    const TopologyNode &sample_node = sample_tree.getNode( sampleCladeIndex );
                    
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
                size_t sampleCladeIndex = sample_root.getCladeIndex( node );
                
                const TopologyNode &sample_node = sample_tree.getNode( sampleCladeIndex );
                
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
    for (int idx = 0; idx < input_nodes.size(); ++idx)
    {
        
        TopologyNode &node = *input_nodes[idx];
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


void TreeSummary::annotateHPDAges(Tree &tree, double hpd, int b )
{
    setBurnin(b);
    
    std::stringstream ss;
    ss << "Compiling " << hpd * 100 << "% HPD node ages from " << trace.size() << " total trees in tree trace, using a burnin of " << burnin << " trees.\n";
    RBOUT(ss.str());
    
    
    summarizeClades(b);
    
    const std::vector<TopologyNode*> &nodes = tree.getNodes();
    std::vector<std::string*> node_intervals(nodes.size());
    
    
    for (size_t i = 0; i < nodes.size(); i++)
    {
        // first get all the node ages for this node and sort them
        std::vector<std::string> taxa;
        nodes[i]->getTaxaStringVector(taxa);
        Clade c(taxa, 0.0); // clade age not used here
        const std::string &cladeName = c.toString();
        std::map<std::string, std::vector<double> >::iterator entry_clade_age = cladeAges.find( cladeName );
        
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


Clade TreeSummary::fillConditionalClades(const TopologyNode &n, std::vector<ConditionalClade> &condClades, std::vector<Clade> &clades)
{
    
    std::vector<std::string> taxa;
    n.getTaxaStringVector(taxa);
    Clade parent(taxa, n.getAge());
    clades.push_back(parent);
    
    for (size_t i = 0; i < n.getNumberOfChildren(); i++)	//SK: extended for multifurcating trees
    {
        const TopologyNode &childNode = n.getChild(i);
        if (!childNode.isTip())
        {
            Clade ChildClade = fillConditionalClades(childNode, condClades, clades);
            ConditionalClade cc = ConditionalClade(parent, ChildClade);
            condClades.push_back(cc);
        }
    }
    
    return parent;
}


Sample<std::string>& TreeSummary::findCladeSample(const std::string &n)
{
    
    for (std::vector<Sample<std::string> >::iterator it=cladeSamples.begin(); it!= cladeSamples.end(); ++it)
    {
        if ( it->getValue() == n )
        {
            return *it;
        }
        
    }
    
    throw RbException("Couldn't find a clade with name '" + n + "'.");
}


void TreeSummary::summarizeClades(int b)
{
    //	setBurnin(b);
    //
    //    // first we need to get a taxon map
    ////    TaxonMap;
    //
    //
    //
    //	for (size_t i = burnin; i < trace.size(); ++i)
    //	{
    //		Tree* tree = trace.objectAt(i);
    //
    //		// get the clades for this tree
    //		std::vector<Clade> clades;
    //		fillClades(tree->getRoot(), clades);
    //
    //		// collect clade ages and increment the clade frequency counter
    //		for (size_t j = 0; j < clades.size(); ++j)
    //		{
    //			const Clade & c = clades[j];
    //			std::string cladeString = c.toString();
    //			Sample<std::string> thisSample = Sample<std::string>(cladeString, 0);
    //
    //			if (cladeSamples.size() == 0)
    //            {
    //				cladeSamples.push_back(thisSample);
    //				cladeSamples[0].addObservation(true);
    //				std::vector<double> tempAgeVec(1, c.getAge());
    //				cladeAges.insert(std::pair<std::string, std::vector<double> >(cladeString, tempAgeVec));
    //			}
    //			else
    //			{
    //				bool found = false;
    //				for (size_t ii = 0; ii < cladeSamples.size(); ii++)
    //				{
    //					if (!cladeString.compare(cladeSamples[ii].getValue()) )
    //					{
    //						found = true;
    //						cladeSamples[ii].addObservation(true);
    //
    //						std::map<std::string, std::vector<double> >::iterator entry = cladeAges.find(cladeString);
    //						if (entry != cladeAges.end())
    //						{
    //							entry->second.push_back(c.getAge());
    //						}
    //						break;
    //					}
    //					else
    //					{
    //						cladeSamples[ii].addObservation(false);
    //					}
    //				}
    //
    //				if (!found)
    //				{
    //					cladeSamples.push_back(thisSample);
    //					cladeSamples.rbegin()->addObservation(true);
    //					std::vector<double> tempAgeVec(1, c.getAge());
    //					cladeAges.insert(std::pair<std::string, std::vector<double> >(cladeString, tempAgeVec));
    //
    //				}
    //			}
    //		}
    //	}
    //
    //    for (size_t ii = 0; ii < cladeSamples.size(); ii++)
    //    {
    //        cladeSamples[ii].computeStatistics();
    //    }
    //
    //	// sort the samples by frequency
    //	sort(cladeSamples.begin(), cladeSamples.end());
    
    
    
    std::map<std::string, Sample<std::string> > cladeAbsencePresence;
    
    setBurnin(b);
    
    std::string outgroup = "";
    for (size_t i = burnin; i < trace.size(); ++i)
    {
        const Tree &tree = trace.objectAt(i);
        
        // get the clades for this tree
        std::vector<Clade> clades;
        fillClades(tree.getRoot(), clades);
        
        // collect clade ages and increment the clade frequency counter
        for (size_t j = 0; j < clades.size(); ++j)
        {
            const Clade & c = clades[j];
            
            if ( c.size() <= 1 ) continue;
            
            std::string cladeString = c.toString();
            Sample<std::string> thisSample = Sample<std::string>(cladeString, 0);
            
            const std::map<std::string, Sample<std::string> >::iterator& entry = cladeAbsencePresence.find(cladeString);
            if (entry == cladeAbsencePresence.end())
            {
                Sample<std::string> cladeSample = Sample<std::string>(cladeString, 0);
                if (i > burnin)
                {
                    cladeSample.setTrace(std::vector<double>(i - burnin, 0.0));
                }
                else
                {
                    cladeSample.setTrace(std::vector<double>());
                }
                cladeAbsencePresence.insert(std::pair<std::string, Sample<std::string> >(cladeString, cladeSample));
                
                // create a new entry for the age of the clade
                std::vector<double> tempAgeVec;
                cladeAges.insert(std::pair<std::string, std::vector<double> >(cladeString, tempAgeVec));
            }
            
            // store the age for this clade
            std::map<std::string, std::vector<double> >::iterator entry_age = cladeAges.find(cladeString);
            entry_age->second.push_back(c.getAge());
            
        }
        
        for (std::map<std::string, Sample<std::string> >::iterator it=cladeAbsencePresence.begin(); it!=cladeAbsencePresence.end(); ++it )
        {
            bool found = false;
            for (size_t i = 0; i < clades.size(); ++i)
            {
                std::string c = clades[i].toString();
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
    
    // collect the samples
    cladeSamples.clear();
    for (std::map<std::string, Sample<std::string> >::iterator it = cladeAbsencePresence.begin(); it != cladeAbsencePresence.end(); ++it)
    {
        it->second.computeStatistics();
        cladeSamples.push_back(it->second);
    }
    
    // sort the samples by frequency
    sort(cladeSamples.begin(), cladeSamples.end());
    
}

void TreeSummary::summarizeConditionalClades( int b )
{
    std::map<std::string, Sample<std::string> > cladeAbsencePresence;
    
    setBurnin(b);
    
    for (size_t i = burnin; i < trace.size(); ++i)
    {
        const Tree &tree = trace.objectAt( i );
        
        // get the conditional clades for this
        std::vector<ConditionalClade> condClades;
        std::vector<Clade> clades;
        fillConditionalClades(tree.getRoot(), condClades, clades);
        
        // first increment the clade frequency counter
        // there need to be two loops because otherwise we count the the parent clade twice
        for (size_t i = 0; i < clades.size(); ++i)
        {
            const Clade & c = clades[i];
            std::string parentString = c.toString();
            const std::map<std::string, Sample<std::string> >::iterator& entry = cladeAbsencePresence.find( parentString );
            if ( entry == cladeAbsencePresence.end() )
            {
                Sample<std::string> cladeSample = Sample<std::string>(parentString,0);
                if ( i > burnin )
                {
                    cladeSample.setTrace( std::vector<double>(i - burnin,0.0) );
                }
                else
                {
                    cladeSample.setTrace( std::vector<double>() );
                }
                cladeAbsencePresence.insert( std::pair<std::string, Sample<std::string> >(parentString, cladeSample));
                
                conditionalCladeFrequencies.insert( std::pair<std::string, std::map<std::string, std::vector<double> > >(parentString, std::map<std::string, std::vector<double> >()) );
                
            }
            
        }
        
        
        
        for (std::map<std::string, Sample<std::string> >::iterator it=cladeAbsencePresence.begin(); it!=cladeAbsencePresence.end(); ++it )
        {
            bool found = false;
            for (size_t i = 0; i < clades.size(); ++i)
            {
                std::string c = clades[i].toString();
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
        
        for (size_t i = 0; i < condClades.size(); ++i)
        {
            const ConditionalClade & cc = condClades[i];
            const Clade &parent = cc.getParent();
            const Clade &child  = cc.getChild();
            
            // now increment the conditional clade frequency counter
            std::string parentString = parent.toString();
            std::string childString = child.toString();
            double childAge = child.getAge();
            std::map<std::string, std::vector<double> >& parentEntry = conditionalCladeFrequencies.find( parentString )->second;
            const std::map<std::string, std::vector<double> >::iterator& childEntry = parentEntry.find( childString );
            if ( childEntry == parentEntry.end() )
            {
                parentEntry.insert( std::pair<std::string, std::vector<double> >(childString, std::vector<double>(1,childAge) ));
            }
            else
            {
                std::vector<double> &samples = childEntry->second;
                samples.push_back( childAge );
            }
            
        }
    }
    
    // collect the samples
    cladeSamples.clear();
    for (std::map<std::string, Sample<std::string> >::iterator it = cladeAbsencePresence.begin(); it != cladeAbsencePresence.end(); ++it)
    {
        it->second.computeStatistics();
        cladeSamples.push_back( it->second );
    }
    
    // sort the samples by frequency
    sort(cladeSamples.begin(), cladeSamples.end());
}


void TreeSummary::summarizeTrees(int b)
{
    
    std::map<std::string, Sample<std::string> > treeAbsencePresence;
    
    setBurnin(b);
    
    std::string outgroup = "";
    for (size_t i = burnin; i < trace.size(); ++i)
    {
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
    
    for (std::vector<Sample<std::string> >::reverse_iterator it = cladeSamples.rbegin(); it != cladeSamples.rend(); ++it)
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


TopologyNode* TreeSummary::assembleConsensusTopology(std::vector<TopologyNode*> *nodes, std::vector<std::string> tipNames, std::vector<double> *pp, double cutoff, double burnin)
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
        
        Clade clade = Clade(cladeSamples[rIndex].getValue(), 0.0);
        std::vector<std::string> intNodeTaxa = clade.getTaxonNames();
        
        //make sure we have an internal node
        if (intNodeTaxa.size() == 1 || intNodeTaxa.size() == tipNames.size())  continue;
        
        nIndex++;	//increment node index
        TopologyNode* intNode = new TopologyNode(nIndex); //Topology node constructor, with proper node index
        intNode->setNodeType(false, false, true);
        
        //find parent node
        std::string firstTaxon = intNodeTaxa.at(0);
        TopologyNode* parentNode = NULL;
        bool isCompatible = true;
        
        for (size_t l = 0; l < tipNames.size(); l++)
        {
            if (!firstTaxon.compare(nodes->at(l)->getName()))
            {
                parentNode = &nodes->at(l)->getParent();
                break;
            }
        }
        while (parentNode != NULL)
        {
            std::vector<std::string> subtendedTaxa;
            parentNode->getTaxaStringVector(subtendedTaxa);
            if (subtendedTaxa.size() >= intNodeTaxa.size())
            {
                //check if parent is compatible with new node
                size_t inBoth = 0;
                for (size_t ll = 0; ll < intNodeTaxa.size(); ll++)
                {
                    for (size_t lll = 0; lll < subtendedTaxa.size(); lll++)
                    {
                        if (!intNodeTaxa[ll].compare(subtendedTaxa[lll]))
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
            std::vector<std::string> childTaxa;
            int found = 0;
            childNode->getTaxaStringVector(childTaxa);
            
            for (size_t i = 0; i < childTaxa.size(); i++)
            {
                for (size_t ii = 0; ii < intNodeTaxa.size(); ii++)
                {
                    if (!childTaxa[i].compare(intNodeTaxa[ii]))
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


void TreeSummary::setBurnin(int b)
{
    // make sure burnin is proper
    if ( b >= static_cast<int>(trace.size()) ) 
    {
        throw RbException("Burnin size is too large for the trace.");
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



Tree* TreeSummary::map( int b )
{
    bool useMean = true;
    setBurnin(b);
    
    std::stringstream ss;
    ss << "Compiling MAP tree from " << trace.size() << " trees in tree trace, using a burnin of " << burnin << " trees.\n";
    RBOUT(ss.str());
    
    summarizeConditionalClades( b );
    summarizeTrees( b );
    
    double sampleSize = trace.size() - burnin;
    
    double meanRootAge = 0.0;
    std::vector<double> rootAgeSamples;
    for (size_t i = burnin; i < trace.size(); ++i)
    {
        // get the sampled tree
        const Tree &tree = trace.objectAt( i );
        
        // add this root age to our variable
        meanRootAge += tree.getRoot().getAge();
        
        rootAgeSamples.push_back(tree.getRoot().getAge());
        
    }
    
    std::string bestNewick = treeSamples.rbegin()->getValue();
    NewickConverter converter;
    Tree* bestTree = converter.convertFromNewick( bestNewick );
    Tree* bestTimeTree = TreeUtilities::convertTree( *bestTree );
    size_t numTaxa = bestTree->getNumberOfTips();
    
    RBOUT("Calculating mean node ages...\n");
    const std::vector<TopologyNode*> &nodes = bestTimeTree->getNodes();
    for (size_t i = numTaxa; i < nodes.size(); ++i)
    {
        TopologyNode* n = nodes[i];
        if ( !n->isTip() )
        {
            // first we compute the posterior probability of the clade
            std::vector<std::string> taxa;
            n->getTaxaStringVector(taxa);
            Clade c( taxa, 0.0 );
            
            double cladeFreq = findCladeSample( c.toString() ).getFrequency();
            double pp = cladeFreq / sampleSize;
            n->addNodeParameter("posterior",pp);
            
            // second we compute the conditional clade probability
            double ccp = 1.0;
            double age = 0.0;
            if ( !n->isRoot() )
            {
                std::vector<std::string> parentTaxa;
                n->getParent().getTaxaStringVector(parentTaxa);
                Clade parent( parentTaxa, 0.0 );
                std::map<std::string, std::vector<double> >& condCladeFreqs = conditionalCladeFrequencies[parent.toString()];
                double parentCladeFreq = findCladeSample( parent.toString() ).getFrequency();
                std::vector<double> condCladeSamples = condCladeFreqs[c.toString()];
                size_t condCladeSampleSize = condCladeSamples.size();
                ccp = condCladeSampleSize / parentCladeFreq;
                
                if (useMean) {
                    // finally, we compute the mean conditional age
                    for (size_t i = 0; i<condCladeSampleSize; ++i)
                    {
                        age += condCladeSamples[i];
                    }
                    age /= condCladeSampleSize;
                }
                else {
                    
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
                if (useMean) {
                    age = meanRootAge / sampleSize;
                }
                else {
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
            bestTimeTree->getNode( i ).setAge( age);
        }
    }
    
    const Tree &firstTree = trace.objectAt( 0 );
    const std::vector<TopologyNode*> &firstNodes = firstTree.getNodes();
    for (size_t i = 0; i < numTaxa; i++)
    {
        if (firstNodes[i]->isTip()) {
            for (size_t j = 0; j < numTaxa; j++)
            {
                if (firstNodes[i]->getName() == nodes[j]->getName())
                {
                    bestTimeTree->getNode( j ).setAge( firstNodes[i]->getAge() );
                }
            }
        }
    }
    
    return bestTimeTree;
}



Tree* TreeSummary::conTree(double cutoff, int b)
{
    
    setBurnin(b);
    
    std::stringstream ss;
    ss << "Compiling consensus tree from " << trace.size() << " trees in tree trace, using a burnin of " << burnin << " trees.\n";
    RBOUT(ss.str());
    
    //fill in clades, use all above 50% to resolve the bush with the consensus partitions
    summarizeClades(b);		//fills std::vector<Sample<std::string> > cladeSamples, sorts them by descending freq
    
    //set up variables for consensus tree assembly
    const Tree &temptree = trace.objectAt(0);
    std::vector<std::string> tipNames = temptree.getTipNames();
    std::vector<double> pp;
    std::vector<TopologyNode*> nodes;
    
    if (cutoff < 0.0 || cutoff > 1.0) cutoff = 0.5;
    
    ss << "Assembling clades into a MR consensus tree, using a " << cutoff << " cutoff...\n";
    RBOUT(ss.str());
    TopologyNode* root = assembleConsensusTopology(&nodes, tipNames, &pp, cutoff, burnin);
    
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
Clade TreeSummary::fillClades(const TopologyNode &n, std::vector<Clade> &clades)
{
    std::vector<std::string> taxa;
    n.getTaxaStringVector(taxa);
    Clade parentClade (taxa, n.getAge());
    clades.push_back(parentClade);
    
    if ( !n.isTip() )
    {
        for (size_t i = 0; i < n.getNumberOfChildren(); i++)
        {
            const TopologyNode &childNode = n.getChild(i);
            Clade ChildClade = fillClades(childNode, clades);
        }
    }
    return parentClade;
}

