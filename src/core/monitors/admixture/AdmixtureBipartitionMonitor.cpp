//
//  AdmixtureBipartitionMonitor.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 5/27/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureBipartitionMonitor.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "RbException.h"
#include <iomanip>

using namespace RevBayesCore;

/* Constructor */
AdmixtureBipartitionMonitor::AdmixtureBipartitionMonitor(TypedDagNode<AdmixtureTree> *t,  TypedDagNode< std::vector< double > >* br, TypedDagNode<int>* dt, int ntr, int nar, int g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap) : Monitor(g,t), outStream(), tree( t ), branchRates(br), delayTimer(dt), filename( fname ), separator( del ), posterior( pp ), likelihood( l ), prior( pr ), append(ap), numSamples(0), numTreeResults(ntr), numAdmixtureResults(nar), numTaxa(0) {
    
    nodes.insert(branchRates);
    nodes.insert(delayTimer);
    
    initializeTaxonBipartition();
}

AdmixtureBipartitionMonitor::AdmixtureBipartitionMonitor(const AdmixtureBipartitionMonitor &m) : Monitor( m ), outStream( ), tree( m.tree ), branchRates( m.branchRates), delayTimer(m.delayTimer), nodeVariables( m.nodeVariables ), numSamples(m.numSamples), numTreeResults(m.numTreeResults), numAdmixtureResults(m.numAdmixtureResults), numTaxa(m.numTaxa) {
    
    filename    = m.filename;
    separator   = m.separator;
    prior       = m.prior;
    posterior   = m.posterior;
    likelihood  = m.likelihood;
    append      = m.append;
}


/* Clone the object */
AdmixtureBipartitionMonitor* AdmixtureBipartitionMonitor::clone(void) const {
    
    return new AdmixtureBipartitionMonitor(*this);
}


void AdmixtureBipartitionMonitor::closeStream() {
    outStream.close();
}

void AdmixtureBipartitionMonitor::initializeTaxonBipartition(void)
{
    numTaxa = tree->getValue().getNumberOfTips();
}

void AdmixtureBipartitionMonitor::flipTaxonBipartitionToMinor(std::vector<bool>& b)
{
    int m = 0;
    for (size_t i = 0; i < b.size(); i++)
        if (b[i])
            m++;
    if (m > b.size()/2)
        b.flip();
}

void AdmixtureBipartitionMonitor::findTaxonBipartition(std::vector<bool>& b, AdmixtureNode* p)
{
    size_t numChildren = p->getNumberOfChildren();
    for (size_t i = 0; i < numChildren; i++)
        findTaxonBipartition(b, &p->getChild(i));
    
    if (numChildren == 0)
        b[p->getIndex()] = 1;
}

std::string AdmixtureBipartitionMonitor::buildBipartitionString(void)
{
    // return buffer
    std::stringstream ss;
    
    // get tree object
    AdmixtureTree* tau = &tree->getValue();
    std::vector<AdmixtureNode*> nodes;
    for (int i = 0; i < tree->getValue().getNumberOfNodes(); i++)
        nodes.push_back(&tau->getNode(i));
    
    bool firstRootHit = false; // necessary?
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
        
        std::vector<bool> pbp = std::vector<bool>(numTaxa,false);
        findTaxonBipartition(pbp, p);
        
        if (&p->getAdmixtureParent() != NULL)
        {
            // admixture children are handled by the admixture parent case
            continue;
        }
        else if (&p->getAdmixtureChild() == NULL && &p->getAdmixtureParent() == NULL)
        {
            // skip tips for divergence partitions
            if (p->getNumberOfChildren() == 0)
                continue;
            
            // print bitstring for divergence partition
            for (size_t i = 0; i < pbp.size(); i++)
                ss << std::noboolalpha << pbp[i];
            ss << separator;
        }
        
        else if (&p->getAdmixtureChild() != NULL)
        {
            AdmixtureNode* c = &p->getAdmixtureChild();
            std::vector<bool> cbp = std::vector<bool>(numTaxa,false);
            findTaxonBipartition(cbp, c);

            for (size_t i = 0; i < pbp.size(); i++)
                ss << std::noboolalpha << pbp[i];
            ss << ">";
            for (size_t i = 0; i < cbp.size(); i++)
                ss << std::noboolalpha << cbp[i];
            ss << separator;
        }
    }
    return ss.str();
}

void AdmixtureBipartitionMonitor::monitor(long gen) {
    
    // get the printing frequency
    int samplingFrequency = printgen;
    
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
        
        std::string tp = buildBipartitionString();
        
        outStream << tp << std::endl;
        
    }
}


/** open the file stream for printing */
void AdmixtureBipartitionMonitor::openStream(void) {
    
    // open the stream to the file
    if (append)
        outStream.open( filename.c_str(), std::fstream::out | std::fstream::app);
    else
        outStream.open( filename.c_str(), std::fstream::out);
    
}

/** Print header for monitored values */
void AdmixtureBipartitionMonitor::printHeader() {
    
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


void AdmixtureBipartitionMonitor::swapNode(DagNode *oldN, DagNode *newN) {
    
    if ( oldN == tree ) {
        tree = static_cast< TypedDagNode< AdmixtureTree > *>( newN );
    }
    else if (oldN == branchRates)
    {
        branchRates = static_cast<TypedDagNode<std::vector<double> >* >(newN);
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
