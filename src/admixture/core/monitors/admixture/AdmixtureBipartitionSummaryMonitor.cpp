//
//  AdmixtureBipartitionSummaryMonitor.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 3/10/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureBipartitionSummaryMonitor.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "RbException.h"
#include <iomanip>

using namespace RevBayesCore;

/* Constructor */
AdmixtureBipartitionSummaryMonitor::AdmixtureBipartitionSummaryMonitor(TypedDagNode<AdmixtureTree> *t,  TypedDagNode< RbVector< double > >* br, TypedDagNode<int>* dt, int ntr, int nar, unsigned long g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap) : Monitor(g,t), outStream(), tree( t ), branchRates(br), delayTimer(dt), filename( fname ), separator( del ), posterior( pp ), prior( pr ), likelihood( l ), append(ap), numTaxa(0), numSamples(0), numTreeResults(ntr), numAdmixtureResults(nar) {
    
    nodes.push_back(branchRates);
    nodes.push_back(delayTimer); // ????
    
    initializeTaxonBipartition();
}

AdmixtureBipartitionSummaryMonitor::AdmixtureBipartitionSummaryMonitor(const AdmixtureBipartitionSummaryMonitor &m) : Monitor( m ), outStream( ), tree( m.tree ), branchRates( m.branchRates), delayTimer(m.delayTimer), nodeVariables( m.nodeVariables ), numTaxa(m.numTaxa), numSamples(m.numSamples), numTreeResults(m.numTreeResults), numAdmixtureResults(m.numAdmixtureResults) {
    
    filename    = m.filename;
    separator   = m.separator;
    prior       = m.prior;
    posterior   = m.posterior;
    likelihood  = m.likelihood;
    append      = m.append;
}


/* Clone the object */
AdmixtureBipartitionSummaryMonitor* AdmixtureBipartitionSummaryMonitor::clone(void) const {
    
    return new AdmixtureBipartitionSummaryMonitor(*this);
}


void AdmixtureBipartitionSummaryMonitor::closeStream() {
    outStream.close();
}

void AdmixtureBipartitionSummaryMonitor::initializeTaxonBipartition(void)
{
    numTaxa = tree->getValue().getNumberOfTips();
}

void AdmixtureBipartitionSummaryMonitor::flipTaxonBipartitionToMinor(std::vector<bool>& b)
{
    unsigned m = 0;
    for (size_t i = 0; i < b.size(); i++)
        if (b[i])
            m++;
    if (m > b.size()/2)
        b.flip();
}

void AdmixtureBipartitionSummaryMonitor::findTaxonBipartition(std::vector<bool>& b, AdmixtureNode* p)
{
    size_t numChildren = p->getNumberOfChildren();
    for (size_t i = 0; i < numChildren; i++)
        findTaxonBipartition(b, &p->getChild(i));
    
    if (numChildren == 0)
        b[p->getIndex()] = 1;
}

void AdmixtureBipartitionSummaryMonitor::updateBipartitions(void)
{
    
    if (delayTimer->getValue() <= 0)
    {
        
        // get tree object
        AdmixtureTree* tau = &tree->getValue();
        std::vector<AdmixtureNode*> nodes;
        for (unsigned i = 0; i < tree->getValue().getNumberOfNodes(); i++)
            nodes.push_back(&tau->getNode(i));
        
        // map key iterator used for find()
        std::map<std::vector<bool>, std::vector<AdmixtureEdgeRecord> >::iterator it;
    
        // get partitions for all nodes
        bool firstRootHit = false;
        for (size_t i = 0; i < nodes.size(); i++)
        {
            AdmixtureNode* p = nodes[i];
            if (p->isRoot())
                continue;
            else if (&p->getParent() == &tau->getRoot() && firstRootHit)
            {
                firstRootHit = false;
                continue;
            }
            
            std::vector<bool> p_key = std::vector<bool>(numTaxa,false);
            std::vector<bool> c_key;

            AdmixtureEdgeRecord er(0,false,0,0,0,0);
            findTaxonBipartition(p_key, p);
         
            if (&p->getAdmixtureParent() != NULL)
            {
                // admixture children are handled by the admixture parent case
                continue;
            }
            else if (&p->getAdmixtureChild() == NULL && &p->getAdmixtureParent() == NULL)
            {
                // skip tips for tree partitions
                if (p->getNumberOfChildren() == 0)
                    continue;
                
                // topology nodes have complementary disjoint bipartitions
                //flipTaxonBipartitionToMinor(p_key);

                er = AdmixtureEdgeRecord((int)numSamples,false,p->getWeight(),p->getAge(),p->getBranchLength(),branchRates->getValue()[p->getIndex()]);
                
                // if key exists, push on to existing value as copy of original node
                if (treeBipartitions.find(p_key) != treeBipartitions.end())
                {
                    treeBipartitions[p_key].push_back(er);
                    //std::cout << "\tpush\n";
                }
                
                // otherwise, create new vector
                else
                {
                    std::vector<AdmixtureEdgeRecord> tmp(1,er);
                    treeBipartitions[p_key] = tmp;
                    //std::cout << "\tinsert\n";
                }
            }
            
            else if (&p->getAdmixtureChild() != NULL)
            {
                // admixture parent-child pairs have disjoint bipartitions
                AdmixtureNode* c = &p->getAdmixtureChild();
                c_key = std::vector<bool>(numTaxa,false);
                findTaxonBipartition(c_key, c);
                er = AdmixtureEdgeRecord((int)numSamples,true,c->getWeight(),c->getAge(),c->getBranchLength(),branchRates->getValue()[c->getTopologyChild(0).getIndex()]);
                
                // if key exists, push on to existing value as copy of original node
                if (AdmixtureBipartitionSummarys.find(p_key) != AdmixtureBipartitionSummarys.end() && AdmixtureBipartitionSummarys.at(p_key).find(c_key) != AdmixtureBipartitionSummarys.at(p_key).end())
                {
                    AdmixtureBipartitionSummarys[p_key][c_key].push_back(er);
                }
                
                // otherwise, create new vector
                else
                {
                    std::vector<AdmixtureEdgeRecord> tmp(1,er);
                    AdmixtureBipartitionSummarys[p_key][c_key] = tmp;
                }
            }
        }
        numSamples++;
    }
}

std::string AdmixtureBipartitionSummaryMonitor::buildTreeBipartitionString(void)
{
    if (delayTimer->getValue() > 0) return "";
    
    // insert all map elements into set sorted by container sizes in decreasing order
    std::multiset< std::pair< std::vector<bool>, std::vector<AdmixtureEdgeRecord> >, TreeBipartitionCompare> s;
    std::map< std::vector<bool>, std::vector<AdmixtureEdgeRecord> >::const_iterator it1;
    
    for (it1 = treeBipartitions.begin(); it1 != treeBipartitions.end(); it1++)
    {
        // create (bitfield1,edge)
        std::pair< std::vector<bool>, std::vector<AdmixtureEdgeRecord> > v(it1->first, it1->second);
        s.insert(v);
    }
    
    
    // store the taxon bipartition pairs with the greatest frequency
    size_t curResults = 0;
    std::multiset<std::pair<std::vector<bool>, std::vector<AdmixtureEdgeRecord> >, TreeBipartitionCompare>::iterator its;
    std::vector<std::vector<bool> > bestPairs;
    
    for (its = s.begin(); its != s.end(); its++)
    {
        if (curResults >= numTreeResults)
            break;
        
        bestPairs.push_back(its->first);
        curResults++;
    }
    
    // query the map using bestPairs[i] to get the ith set of admixture edges given the two partitions exist
    // ...
    std::vector<TopologyNode*> tips = tree->getValue().getNodes();
    std::stringstream ss("");

    ss << "Top " << numTreeResults << " tree bipartitions\n";
    for (size_t i = 0; i < bestPairs.size(); i++)
    {
        ss << i;
        
        std::vector<AdmixtureEdgeRecord>* v = &(treeBipartitions[bestPairs[i]]);
        ss << "\tpp: " << std::setprecision(8) << std::fixed << (double)v->size() / numSamples << "\tn:" << std::setw(8) << v->size() << "\tN: " << std::setw(8) << numSamples;
        
        double w = 0.0;
        double t = 0.0;
        for (std::vector<AdmixtureEdgeRecord>::iterator it = v->begin(); it != v->end(); it++)
        {
            w += (*it).weight;
            t += (*it).age;
            /*
             ss << "\t";
             ss << "p: " << &(*it)->getAdmixtureParent();
             ss << "c: " << &(*it)->getAdmixtureChild();
             ss << "w: " << (*it)->getWeight();
             ss << "\n";
             */
        }
        ss << "\tw: " << w / v->size();
        // ss << "\tt: " << t / v->size(); // ... not sure how meaningful this is...
        
        ss << "\t{";
        //        for (size_t j = 0; j < bestPairs[i].second.size(); j++)
        //            ss << (bestPairs[i].second[j] ? "*" : ".");
        bool firstfirst = true;
        for (size_t j = 0; j < bestPairs[i].size(); j++)
        {
            if (bestPairs[i][j] == true)
            {
                if (firstfirst)
                    firstfirst = false;
                else
                    ss << ",";
                ss << tips[j]->getName();
            }
        }
        
        ss << "}";
        /*
        ss << " | {";
        //        for (size_t j = 0; j < bestPairs[i].second.size(); j++)
        //            ss << (bestPairs[i].second[j] ? "*" : ".");
        std::vector<bool> complementPartition = bestPairs[i];
        complementPartition.flip();
        
        bool firstsecond = true;
        for (size_t j = 0; j < complementPartition.size(); j++)
        {
            if (complementPartition[j] == true)
            {
                if (firstsecond)
                    firstsecond = false;
                else
                    ss << ",";
                ss << tips[j]->getName();
            }
        }
        ss << "}";
        */
        ss << "\n";
    }
    ss << "\n";
    return ss.str();
}

std::string AdmixtureBipartitionSummaryMonitor::buildAdmixtureBipartitionSummaryString(void)
{
    if (delayTimer->getValue() > 0) return "";
    
    // insert all map elements into set sorted by container sizes in decreasing order
    std::multiset< std::pair< std::pair< std::vector<bool>, std::vector<bool> >, std::vector<AdmixtureEdgeRecord> >, AdmixtureBipartitionSummaryCompare> s;
    std::map< std::vector<bool>, std::map< std::vector<bool>, std::vector<AdmixtureEdgeRecord> > >::const_iterator it1;
    std::map< std::vector<bool>, std::vector<AdmixtureEdgeRecord> >::const_iterator it2;
    
    for (it1 = AdmixtureBipartitionSummarys.begin(); it1 != AdmixtureBipartitionSummarys.end(); it1++)
    {
        for (it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
        {
            
            // create ((bitfield1,bitfield2),edge)
            std::pair<std::pair<std::vector<bool>, std::vector<bool> >, std::vector<AdmixtureEdgeRecord> > v(std::pair<std::vector<bool>, std::vector<bool> >(it1->first, it2->first), it2->second);
            s.insert(v);
        }
    }
    
    // store the taxon bipartition pairs with the greatest frequency of admixture events
    size_t curResults = 0;
    std::multiset< std::pair< std::pair< std::vector<bool>, std::vector<bool> >, std::vector<AdmixtureEdgeRecord> >, AdmixtureBipartitionSummaryCompare>::iterator its;
    std::vector<std::pair<std::vector<bool>, std::vector<bool> > > bestPairs;
    
    for (its = s.begin(); its != s.end(); its++)
    {
        if (curResults >= numAdmixtureResults)
            break;
        
        bestPairs.push_back(std::pair<std::vector<bool>, std::vector<bool> >(its->first.first, its->first.second));
        curResults++;
    }
    
    // query the map using bestPairs[i] to get the ith set of admixture edges given the two partitions exist
    // ...
    std::vector<TopologyNode*> tips = tree->getValue().getNodes();
    std::stringstream ss("");
    ss << "Top " << numAdmixtureResults << " admixture bipartitions\n";
    for (size_t i = 0; i < bestPairs.size(); i++)
    {
        ss << i;
        
        std::vector<AdmixtureEdgeRecord>* v = &(AdmixtureBipartitionSummarys[bestPairs[i].first][bestPairs[i].second]);
        ss << "\tpp: " << std::setprecision(8) << std::fixed << (double)v->size() / numSamples << "\tn:" << std::setw(8) << v->size() << "\tN: " << std::setw(8) << numSamples;
    
        double w = 0.0;
        double a = 0.0;
        for (std::vector<AdmixtureEdgeRecord>::iterator it = v->begin(); it != v->end(); it++)
        {
            w += (*it).weight;
            a += (*it).age;
            /*
             ss << "\t";
             ss << "p: " << &(*it)->getAdmixtureParent();
             ss << "c: " << &(*it)->getAdmixtureChild();
             ss << "w: " << (*it)->getWeight();
             ss << "\n";
             */
        }
        ss << "\tw: " << w / v->size();
        ss << "\ta: " << a / v->size(); // ... not sure how meaningful this is...
        ss << "\t{";
        //        for (size_t j = 0; j < bestPairs[i].second.size(); j++)
        //            ss << (bestPairs[i].second[j] ? "*" : ".");
        bool firstfirst = true;
        for (size_t j = 0; j < bestPairs[i].first.size(); j++)
        {
            if (bestPairs[i].first[j] == true)
            {
                if (firstfirst)
                    firstfirst = false;
                else
                    ss << ",";
                ss << tips[j]->getName();
            }
        }
        
        ss << "} -> {";
        //        for (size_t j = 0; j < bestPairs[i].second.size(); j++)
        //            ss << (bestPairs[i].second[j] ? "*" : ".");
        bool firstsecond = true;
        for (size_t j = 0; j < bestPairs[i].first.size(); j++)
        {
            if (bestPairs[i].second[j] == true)
            {
                if (firstsecond)
                    firstsecond = false;
                else
                    ss << ",";
                ss << tips[j]->getName();
            }
        }
        ss << "}";
        ss << "\n";
    }
    ss << "\n";
    return ss.str();
}


void AdmixtureBipartitionSummaryMonitor::monitor(unsigned long gen) {
    
    // get the printing frequency
    unsigned long samplingFrequency = printgen;
    
    if (gen % samplingFrequency == 0) {
        // print the iteration number first
        outStream << gen;
        
        if ( posterior ) {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
                pp += (*it)->getLnProbability();
            }
            outStream << pp;
        }
        
        if ( likelihood ) {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
                if ( (*it)->isClamped() ) {
                    pp += (*it)->getLnProbability();
                }
            }
            outStream << pp;
        }
        
        if ( prior ) {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
                if ( !(*it)->isClamped() ) {
                    pp += (*it)->getLnProbability();
                }
            }
            outStream << pp;
        }
        
        // add a separator before the tree
        outStream << separator;
        
        updateBipartitions();
        
        std::string tp = buildTreeBipartitionString();
        //std::cout << "\n" << tp;
        
        std::string ap = buildAdmixtureBipartitionSummaryString();
        //std::cout << "\n" << ap;
        
        outStream << std::endl;
        outStream << tp << std::endl;
        outStream << ap << std::endl;
        
        outStream << std::endl;
        
    }
}


/** open the file stream for printing */
void AdmixtureBipartitionSummaryMonitor::openStream(bool reopen) {
    
    // open the stream to the file
    if (append)
        outStream.open( filename.c_str(), std::fstream::out | std::fstream::app);
    else
        outStream.open( filename.c_str(), std::fstream::out);
    
}

/** Print header for monitored values */
void AdmixtureBipartitionSummaryMonitor::printHeader() {
    
    // print one column for the iteration number
    outStream << "Iteration";
    
    if ( posterior ) {
        // add a separator before every new element
        outStream << separator;
        outStream << "Posterior";
    }
    
    if ( likelihood ) {
        // add a separator before every new element
        outStream << separator;
        outStream << "Likelihood";
    }
    
    if ( prior ) {
        // add a separator before every new element
        outStream << separator;
        outStream << "Prior";
    }
    
    // add a separator tree
    outStream << separator << "Tree";
    
    // end line of stream
    outStream << std::endl;
}


void AdmixtureBipartitionSummaryMonitor::swapNode(DagNode *oldN, DagNode *newN) {
    
    if ( oldN == tree ) {
        tree = static_cast< TypedDagNode< AdmixtureTree > *>( newN );
    }
    else if (oldN == branchRates)
    {
        branchRates = static_cast<TypedDagNode< RbVector<double> >* >(newN);
    }
    else if (oldN == delayTimer)
    {
        delayTimer = static_cast<TypedDagNode<int>* >(newN);
    }
    
    else {
        // error catching
        if ( nodeVariables.find(oldN) == nodeVariables.end() ) {
            throw RbException("Cannot replace DAG node in this monitor because the monitor doesn't hold this DAG node.");
        }
        
        nodeVariables.erase( oldN );
        nodeVariables.insert( newN );
    }
    
    // delegate to base class
    Monitor::swapNode(oldN, newN);
}
