//
//  AdmixtureBipartitionMonitor.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 5/27/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureBipartitionMonitor.h"
#include "DagNode.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "RbException.h"
#include <iomanip>

using namespace RevBayesCore;

/* Constructor */
//AdmixtureBipartitionMonitor::AdmixtureBipartitionMonitor(TypedDagNode<AdmixtureTree> *t,  TypedDagNode< RbVector< double > >* br, TypedDagNode<int>* dt, int ntr, int nar, int g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap) : Monitor(g,t), outStream(), tree( t ), branchRates(br), delayTimer(dt), filename( fname ), separator( del ), posterior( pp ), likelihood( l ), prior( pr ), append(ap), numSamples(0), numTreeResults(ntr), numAdmixtureResults(nar), numTaxa(0) {

AdmixtureBipartitionMonitor::AdmixtureBipartitionMonitor(TypedDagNode<AdmixtureTree> *t, TypedDagNode<double>* dr, TypedDagNode< RbVector< double > >* br, size_t ntr, size_t nar, unsigned long g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap, bool ci, bool ch) : Monitor(g,t), outStream(), tree( t ), branchRates(br), driftRate(dr), filename( fname ), separator( del ), posterior( pp ), prior( pr ), likelihood( l ), append(ap), chainIdx(ci), chainHeat(ch), numTaxa(0), numSamples(0), numTreeResults(ntr), numAdmixtureResults(nar) {
    
    nodes.push_back(branchRates);
    nodes.push_back(driftRate);
  //  nodes.insert(delayTimer);
    
    initializeTaxonBipartition();
}

AdmixtureBipartitionMonitor::AdmixtureBipartitionMonitor(const AdmixtureBipartitionMonitor &m) : Monitor( m ), outStream( ), tree( m.tree ), branchRates(m.branchRates), driftRate(m.driftRate), nodeVariables( m.nodeVariables ), numTaxa(m.numTaxa), numSamples(m.numSamples), numTreeResults(m.numTreeResults), numAdmixtureResults(m.numAdmixtureResults) {
    
    filename    = m.filename;
    separator   = m.separator;
    prior       = m.prior;
    posterior   = m.posterior;
    likelihood  = m.likelihood;
    append      = m.append;
    chainIdx    = m.chainIdx;
    chainHeat   = m.chainHeat;
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
    unsigned m = 0;
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
    for (unsigned i = 0; i < tree->getValue().getNumberOfNodes(); i++)
        nodes.push_back(&tau->getNode(i));
    
   // bool firstRootHit = false; // necessary?
    for (size_t i = 0; i < nodes.size(); i++)
    {

        AdmixtureNode* p = nodes[i];
                /*
        if (p->isRoot())
            continue;
        else if (&p->getParent() == &tau->getRoot() && firstRootHit)
        {
            firstRootHit = false;
            continue;
        }
         */
        
        std::vector<bool> pbp = std::vector<bool>(numTaxa,false);
        findTaxonBipartition(pbp, p);
        
        // look like s_0,s_1,r_0,r_1,t,w
        
        if (&p->getAdmixtureParent() != NULL)
        {
            // admixture children are handled by the admixture parent case
            continue;
        }
        else if (p->isRoot())
        {
            for (size_t i = 0; i < pbp.size(); i++)
            {
                //std::cout << std::noboolalpha << pbp[i];
                ss << std::noboolalpha << pbp[i];
            }
            //std::cout << "\n";
            ss << ",";
            ss << "," << p->getAge();
            ss << ",";
            ss << ",";
            ss << ",1";
            ss << separator;
        }
        else if (&p->getAdmixtureChild() == NULL && &p->getAdmixtureParent() == NULL)
        {
            // skip tips for divergence partitions
            // actually, keep 'em for pop-size params...
            //if (p->getNumberOfChildren() == 0)
            //    continue;

            //ss << "D";
            // print bitstring for divergence partition
            for (size_t i = 0; i < pbp.size(); i++)
                ss << std::noboolalpha << pbp[i];
            ss << ",";
            ss << "," << p->getAge();
            ss << "," << branchRates->getValue()[p->getIndex()];
            ss << ",";
            ss << ",1";
            ss << separator;
        }
        
        else if (&p->getAdmixtureChild() != NULL)
        {
            AdmixtureNode* c = &p->getAdmixtureChild();
            std::vector<bool> cbp = std::vector<bool>(numTaxa,false);
            findTaxonBipartition(cbp, c);
            
            for (size_t i = 0; i < pbp.size(); i++)
                ss << std::noboolalpha << pbp[i];
            ss << ",";
            for (size_t i = 0; i < cbp.size(); i++)
                ss << std::noboolalpha << cbp[i];
            ss << "," << p->getAge();
            ss << "," << branchRates->getValue()[p->getTopologyChild(0).getIndex()];
            ss << "," << branchRates->getValue()[c->getTopologyChild(0).getIndex()];
            ss << "," << c->getWeight();
            ss << separator;
        }
    }
    return ss.str();
}

void AdmixtureBipartitionMonitor::monitor(unsigned long gen) {
    
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
        
        if (chainIdx)
        {
            outStream << separator;
            outStream << mcmc->getChainIndex();
        }

        if (chainHeat)
        {
            outStream << separator;
            outStream << mcmc->getChainHeat();
        }
        
        outStream << separator;
        outStream << driftRate->getValue();
        
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
    
    if (chainIdx)
    {
        outStream << separator;
        outStream << "ChainIndex";
    }
    
    if (chainHeat)
    {
        outStream << separator;
        outStream << "ChainHeat";
    }
    
    outStream << separator;
    outStream << "DriftRate";
    
    // add a separator tree
    outStream << separator;
  
    // get tree object names
    AdmixtureTree* tau = &tree->getValue();
    std::vector<AdmixtureNode*> nodes;
    for (size_t i = 0; i < tree->getValue().getNumberOfNodes(); i++)
        nodes.push_back(&tau->getNode(i));
    
    for (size_t i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->getNumberOfChildren() == 0)
        {
            if (i != 0)
                outStream << ",";
            outStream << nodes[i]->getName();
        }
    }
    
    // end line of stream
    outStream << std::endl;
}


void AdmixtureBipartitionMonitor::swapNode(DagNode *oldN, DagNode *newN) {
    
    if ( oldN == tree ) {
        tree = static_cast< TypedDagNode< AdmixtureTree > *>( newN );
    }
    else if (oldN == branchRates)
    {
        branchRates = static_cast<TypedDagNode<RbVector<double> >* >(newN);
    }
    else if (oldN == driftRate)
    {
        driftRate = static_cast<TypedDagNode<double>* >(newN);
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
