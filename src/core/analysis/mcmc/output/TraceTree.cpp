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
#include "TraceTree.h"
#include "TreeUtilities.h"

#include <boost/math/special_functions/factorials.hpp>

using namespace RevBayesCore;

TraceTree::TraceTree( bool c ) : Trace<Tree>(),
    clock( c ),
    rooted( true )
{
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */

TraceTree* TraceTree::clone(void) const
{
    
    return new TraceTree(*this);
}


// annotate the MAP node/branch parameters
void TraceTree::mapParameters( Tree &tree ) const
{
    
    const Tree& sample_tree = objectAt( 0 );
    
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
void TraceTree::mapDiscrete(Tree &tree, const std::string &n, size_t paramIndex, size_t num, bool isNodeParameter ) const
{
    
    // 2-d vectors to keep the data (posteriors and states) of the inputTree nodes: [node][data]
    const std::vector<TopologyNode*> &summary_nodes = tree.getNodes();
    std::vector<std::map<std::string, Sample<std::string> > > stateAbsencePresence(summary_nodes.size(), std::map<std::string, Sample<std::string> >());
    
    bool interiorOnly = true;
    bool tipsChecked = false;
    //    bool useRoot = true;
    
    // loop through all trees in tree trace
    for (size_t iteration = burnin; iteration < size(); ++iteration)
    {
        const Tree &sample_tree = objectAt( iteration );
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
                        stateSample.addNonObservations(iteration - burnin);
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
                
                double pp = stateSamples[j].getFrequency() / double(stateSamples[j].size());
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
void TraceTree::mapContinuous(Tree &tree, const std::string &n, size_t paramIndex, double hpd, bool isNodeParameter ) const
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
    for (size_t i = burnin; i < size(); i++)
    {
        const Tree &sample_tree = objectAt( i );
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


void TraceTree::annotateTree( Tree &tree, AnnotationReport report, bool verbose )
{
    summarize( verbose );
    
    RBOUT("Annotating tree ...");
    
    std::string newick;
    
    if ( report.tree_ages )
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
        
        if ( tmp_tree->isRooted() == false && rooted == false )
        {
            std::vector<std::string> tip_names = objectAt(0).getTipNames();
            std::sort(tip_names.begin(),tip_names.end());
            std::string outgroup = tip_names[0];
            tmp_tree->reroot( outgroup, true );
        }
        else if ( tmp_tree->isRooted() != rooted )
        {
            throw(RbException("Rooting of input tree differs from the tree sample"));
        }
        
        newick = TreeUtilities::uniqueNewickTopology( *tmp_tree );
        
        delete tmp_tree;
        
        if ( treeCladeAges.find(newick) == treeCladeAges.end() )
        {
            throw(RbException("Could not find input tree in tree sample"));
        }
    }
    
    const std::vector<TopologyNode*> &nodes = tree.getNodes();
    
    double sampleSize = size() - burnin;
    
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
        if ( sampledAncestorSamples.empty() == false )
        {
            double saFreq = sampledAncestorSamples[n->getTaxon()].getFrequency();
            
            // annotate sampled ancestor prob
            if ( ((n->isTip() && n->isFossil()) || saFreq > 0) && report.sa )
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
            if ( !n->isTip() && report.ccp )
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
        if ( report.ages )
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
        if ( report.hpd )
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
            
            if ( clock )
            {
                if ( !n->isTip() || ( ( n->isFossil() || upper != lower) && !n->isSampledAncestor() ) )
                {
                    std::string label = "age_" + StringUtilities::toString( (int)(report.hpd * 100) ) + "%_HPD";
                    n->addNodeParameter(label, interval);
                }
            }
            else if ( !n->isRoot() )
            {
                std::string label = "brlen_" + StringUtilities::toString( (int)(report.hpd * 100) ) + "%_HPD";
                n->addBranchParameter(label, interval);
            }
        }
        
    }
    
    /*if ( !report.tree_ages && clock )
     {
     enforceNonnegativeBranchLengths( tree.getRoot() );
     }*/
    
    if ( report.map_parameters )
    {
        mapParameters( tree );
    }
    
}



double TraceTree::cladeProbability(const RevBayesCore::Clade &c, bool verbose )
{
    summarize(verbose);
    
    double f = 0.0;
    Clade tmp = c;
    tmp.resetTaxonBitset( objectAt(0).getTaxonBitSetMap() );
    
    try
    {
        const Sample<Clade> &s = findCladeSample( tmp );
        f = double(s.getFrequency()) / s.size();
    }
    catch (RbException& e)
    {
        // do nothing
    }
    
    return f;
}


double TraceTree::computeEntropy( double credible_interval_size, int num_taxa, bool verbose )
{
    summarize( verbose );
    NewickConverter converter;
    double total_prob = 0;
    double total_samples = size();
    double entropy = 0.0;
    /*double tree_count = 0.0;*/
    for (std::vector<Sample<std::string> >::const_reverse_iterator it = treeSamples.rbegin(); it != treeSamples.rend(); ++it)
    {
        double freq = it->getFrequency();
        double p = freq/(total_samples-burnin);
        /*double p = freq/(total_samples);*/
        total_prob += p;
        entropy += (p * log(p));
        if ( total_prob >= credible_interval_size )
        {
            break;
        }
        
    }

    /*This calculation is directly from AMP / Jeremy's paper.*/
    double ntopologies = (boost::math::factorial<double>(((2*num_taxa)-5))) / (boost::math::factorial<double>((num_taxa-3)) * (pow(2, num_taxa-3)));
    entropy += log( ntopologies );
    /*std::cout << ntopologies << '\n';*/
    
    return entropy;
}


std::vector<double> TraceTree::computePairwiseRFDistance( double credible_interval_size, bool verbose )
{
    summarize( verbose );
    std::vector<Tree> unique_trees;
    std::vector<size_t> sample_count;
    NewickConverter converter;
    double total_prob = 0;
    double total_samples = size();
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


std::vector<double> TraceTree::computeTreeLengths( void )
{
    
    std::vector<double> tree_lengths;
    NewickConverter converter;
    
    for (size_t i = burnin; i < size(); ++i)
    {
        
        const Tree &tree = objectAt(i);
        tree_lengths.push_back( tree.getTreeLength() );
        
    }
    
    return tree_lengths;
}


void TraceTree::enforceNonnegativeBranchLengths(TopologyNode& node) const
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
Clade TraceTree::fillConditionalClades(const TopologyNode &n, std::map<Clade, std::set<Clade, CladeComparator>, CladeComparator> &cond_clades)
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


const Sample<Clade>& TraceTree::findCladeSample(const Clade &n) const
{
    
    std::vector<Sample<Clade> >::const_iterator it = find_if (cladeSamples.begin(), cladeSamples.end(), CladeComparator(rooted, n) );
    
    if ( it != cladeSamples.end() )
    {
        return *it;
    }
    
    throw RbException("Couldn't find a clade with name '" + n.toString() + "'.");
}


TopologyNode* TraceTree::findParentNode(TopologyNode& n, const Clade& tmp, std::vector<TopologyNode*>& children, RbBitSet& child_b ) const
{
    RbBitSet node = n.getClade().getBitRepresentation();
    
    Clade c = tmp;
    RbBitSet clade  = c.getBitRepresentation();
    
    RbBitSet mask  = node | clade;
    
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
        RbBitSet child_mask(clade.size());
        
        for (size_t i = 0; i < x.size(); i++)
        {
            RbBitSet child_b(clade.size());
            
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
    else if (child)
    {
        child_b = node;
        children.push_back(&n);
    }
    
    return parent;
}


int TraceTree::getBurnin( void ) const
{
    
    return burnin;
}


int TraceTree::getTopologyFrequency(const RevBayesCore::Tree &tree, bool verbose)
{
    summarize( verbose );
    
    std::vector<std::string> tip_names = objectAt(0).getTipNames();
    std::sort(tip_names.begin(),tip_names.end());
    std::string outgroup = tip_names[0];
    
    Tree t = tree;
    
    if ( t.isRooted() == false && rooted == false )
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


std::vector<Tree> TraceTree::getUniqueTrees( double credible_interval_size, bool verbose )
{
    summarize( verbose );
    
    std::vector<Tree> unique_trees;
    NewickConverter converter;
    double total_prob = 0;
    double total_samples = size();
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



bool TraceTree::isCoveredInInterval(const std::string &v, double ci_size, bool verbose)
{
    
    summarize( verbose );
    

    RandomNumberGenerator *rng = GLOBAL_RNG;
    
    std::vector<std::string> tip_names = objectAt(0).getTipNames();
    std::sort(tip_names.begin(),tip_names.end());
    std::string outgroup = tip_names[0];
    
    Tree tree;
    tree.initFromString(v);

    if ( tree.isRooted() == false && rooted == false )
    {
        tree.reroot( outgroup, true );
    }
    
    std::string newick = TreeUtilities::uniqueNewickTopology(tree);
    
    double totalSamples = size();
    double totalProb = 0.0;
    for (std::vector<Sample<std::string> >::reverse_iterator it = treeSamples.rbegin(); it != treeSamples.rend(); ++it)
    {
        
        double p = it->getFrequency()/(totalSamples-burnin);
//        double include_prob = p / (1.0-totalProb) * (ci_size - totalProb) / (1.0-totalProb);
        double include_prob = (ci_size-totalProb)/p;
//        double include_prob = p * ci_size;
        
        if ( include_prob > rng->uniform01() )
        {
            const std::string &current_sample = it->getValue();
            if ( newick == current_sample )
            {
                return true;
            }
            
        }
        
        totalProb += p;
        
        
        if ( totalProb >= ci_size )
        {
            break;
        }
        
    }
    
    return false;
}


bool TraceTree::isClock(void)
{
    return clock;
}


Tree* TraceTree::mapTree( AnnotationReport report, bool verbose )
{
    std::stringstream ss;
    ss << "Compiling maximum a posteriori tree from " << size() << " trees in tree trace, using a burnin of " << burnin << " trees.\n";
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
    
    TaxonMap tm = TaxonMap( objectAt(0) );
    tmp_tree->setTaxonIndices( tm );
    
    report.ages            = true;
    report.map_parameters  = true;
    annotateTree(*tmp_tree, report, verbose );
    
    return tmp_tree;
}


Tree* TraceTree::mccTree( AnnotationReport report, bool verbose )
{
    std::stringstream ss;
    ss << "Compiling maximum clade credibility tree from " << size() << " trees in tree trace, using a burnin of " << burnin << " trees.\n";
    RBOUT(ss.str());
    
    summarize( verbose );
    
    Tree* best_tree = NULL;
    double max_cc = 0;
    
    // find the clade credibility score for each tree
    for (size_t t = 0; t < treeSamples.size(); t++)
    {
        std::string newick = treeSamples[t].getValue();
        
        // now we summarize the clades for the best tree
        std::map<Clade, std::vector<double>, CladeComparator> cladeAges = treeCladeAges[newick];
        
        double cc = 0;
        
        // find the product of the clade frequencies
        for (std::map<Clade, std::vector<double>, CladeComparator>::iterator clade = cladeAges.begin(); clade != cladeAges.end(); clade++)
        {
            cc += log( findCladeSample( clade->first ).getFrequency() );
        }
        
        if (cc > max_cc)
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
            
            TaxonMap tm = TaxonMap( objectAt(0) );
            best_tree->setTaxonIndices( tm );
            
            delete tmp_tree;
        }
    }
    
    report.ages = true;
    annotateTree(*best_tree, report, verbose );
    
    return best_tree;
}


Tree* TraceTree::mrTree(AnnotationReport report, double cutoff, bool verbose)
{
    if (cutoff < 0.0 || cutoff > 1.0) cutoff = 0.5;
    
    std::stringstream ss;
    ss << "Compiling majority rule consensus tree (cutoff = " << cutoff << ") from " << size() << " trees in tree trace, using a burnin of " << burnin << " trees.\n";
    RBOUT(ss.str());
    
    //fill in clades, use all above 50% to resolve the bush with the consensus partitions
    summarize( verbose );        //fills std::vector<Sample<std::string> > cladeSamples, sorts them by descending freq
    
    //set up variables for consensus tree assembly
    std::vector<std::string> tipNames = objectAt(0).getTipNames();
    
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
        float cladeFreq = cladeSamples[rIndex].getFrequency() / (float)(size() - burnin);
        if (cladeFreq < cutoff)  break;
        
        Clade clade = cladeSamples[rIndex].getValue();
        
        //make sure we have an internal node
        if (clade.size() == 1 || clade.size() == tipNames.size())  continue;
        
        //find parent node
        std::vector<TopologyNode*> children;
        RbBitSet tmp;
        TopologyNode* parentNode = findParentNode(*root, clade, children, tmp );
        
        if (parentNode != NULL )
        {
            // skip this node if we've already found a clade compatible with it
            if ( children.size() == parentNode->getNumberOfChildren() ) continue;
            
            std::vector<TopologyNode*> mrca;
            
            // find the mrca child if it exists
            if ( !clade.getMrca().empty() )
            {
                for (size_t i = 0; i < children.size(); i++)
                {
                    if ( children[i]->isTip() && std::find(clade.getMrca().begin(), clade.getMrca().end(), children[i]->getTaxon() ) != clade.getMrca().end() )
                    {
                        mrca.push_back(children[i]);
                    }
                }
                
                // if we couldn't find the mrca, then this clade is not compatible
                if ( mrca.size() != clade.getMrca().size() )
                {
                    continue;
                }
                else
                {
                    for (size_t i = 0; i < mrca.size(); i++)
                    {
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
            if ( !mrca.empty() && children.size() > 2 )
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


void TraceTree::printCladeSummary(std::ostream &o, double minCladeProbability, bool verbose)
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
    
    double totalSamples = size();
    
    for (std::vector<Sample<Clade> >::reverse_iterator it = cladeSamples.rbegin(); it != cladeSamples.rend(); ++it)
    {
        size_t num_taxa = it->getValue().size();
        
        if ( num_taxa == 1 ) continue;
        
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



void TraceTree::printTreeSummary(std::ostream &o, double credibleIntervalSize, bool verbose)
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
    double totalSamples = size();
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


void TraceTree::summarize( bool verbose )
{
    if ( isDirty() == false ) return;
    
    std::vector<std::string> tip_names = objectAt(0).getTipNames();
    std::sort(tip_names.begin(),tip_names.end());
    std::string outgroup = tip_names[0];

    if( rooted != objectAt(0).isRooted() )
    {
        rooted = objectAt(0).isRooted();
    }

    cladeAges = std::map<Clade, std::vector<double>, CladeComparator >( (CladeComparator(rooted)) );
    //conditionalCladeAges = std::map<Clade, std::map<Clade, std::vector<double>, CladeComparator >, CladeComparator >( CladeComparator(rooted), std::map<Clade, std::vector<double>, CladeComparator >( CladeComparator(rooted) ) );

    sampledAncestorSamples.clear();
    treeCladeAges.clear();
    
    std::map<Clade, Sample<Clade>, CladeComparator > cladeSampleMap( (CladeComparator(rooted)) );
    std::map<std::string, Sample<std::string> >      treeSampleMap;
    
    ProgressBar progress = ProgressBar(size(), burnin);
    if ( verbose )
    {
        RBOUT("Summarizing clades ...\n");
        progress.start();
    }
    
    for (size_t i = burnin; i < size(); ++i)
    {
        
        if ( verbose )
        {
            progress.update(i);
        }
        
        Tree tree = objectAt(i);
        
        if ( rooted == false )
        {
            tree.reroot( outgroup, true );
        }
        
        std::string newick = TreeUtilities::uniqueNewickTopology( tree );
        
        Sample<std::string> treeSample(newick, 0);
        treeSample.addNonObservations(i - burnin);
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
            cladeSample.addNonObservations(i - burnin);
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
            taxonSample.addNonObservations(i - burnin);
            
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
    
    if ( using_sampled_ancestors == false ) sampledAncestorSamples.clear();
    
    dirty = false;
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
    if ( rooted || ab.size() != bb.size() )
    {
        if ( ab.size() != bb.size() || a.getMrca() == b.getMrca() )
        {
            return a < b;
        }
        else
        {
            return a.getMrca() < b.getMrca();
        }
    }
    
    if ( ab.empty() || bb.empty() )
    {
        throw(RbException("Cannot compare unrooted clades (splits) with empty bitsets"));
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
    if ( rooted || ab.size() != bb.size() )
    {
        return a == clade && a.getMrca() == clade.getMrca();
    }
    
    if ( ab.empty() || bb.empty() )
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
