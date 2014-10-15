//
//  AdmixtureResidualsMonitor.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 3/10/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureResidualsMonitor.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "RbException.h"
#include <iomanip>

using namespace RevBayesCore;

/* Constructor */
AdmixtureResidualsMonitor::AdmixtureResidualsMonitor(TypedDagNode< RbVector< double > >* r, std::vector<std::string> tn, int g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap) : Monitor(g,r), outStream(), residuals(r), taxonNames(tn), filename( fname ), separator( del ), posterior( pp ),  prior( pr ), likelihood( l ), append(ap)
{

}

AdmixtureResidualsMonitor::AdmixtureResidualsMonitor(const AdmixtureResidualsMonitor &m) : Monitor( m ), outStream( ), residuals( m.residuals), nodeVariables( m.nodeVariables ), taxonNames(m.taxonNames) {
    
    filename    = m.filename;
    separator   = m.separator;
    prior       = m.prior;
    posterior   = m.posterior;
    likelihood  = m.likelihood;
    append      = m.append;
}


/* Clone the object */
AdmixtureResidualsMonitor* AdmixtureResidualsMonitor::clone(void) const {
    
    return new AdmixtureResidualsMonitor(*this);
}


void AdmixtureResidualsMonitor::closeStream() {
    outStream.close();
}

void AdmixtureResidualsMonitor::monitor(unsigned long gen) {
    
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
        
        // add residuals
        size_t numTaxa = taxonNames.size();
        const std::vector<double>& rv = residuals->getValue();
        
        double sumNegative = 0.0;
        double sumPositive = 0.0;
        double sum = 0.0;
        for (size_t i = 0; i < numTaxa; i++)
        {
            for (size_t j = i; j < numTaxa; j++)
            {
                double v = rv[i*numTaxa+j];
                if (v > 0.0)
                    sumPositive += v;
                else if (v < 0.0)
                    sumNegative += v;
            }
        }
        sum = sumPositive + sumNegative;

        outStream << separator << sum;
        outStream << separator << sumPositive;
        outStream << separator << sumNegative;
        
        for (size_t i = 0; i < numTaxa; i++)
        {
            for (size_t j = i; j < numTaxa; j++)
            {
                outStream << separator << std::setprecision(12) << std::fixed << rv[i * numTaxa + j];
            }
        }
        
        
        outStream << std::endl;
        
    }
}


/** open the file stream for printing */
void AdmixtureResidualsMonitor::openStream(void) {
    
    // open the stream to the file
    if (append)
        outStream.open( filename.c_str(), std::fstream::out | std::fstream::app);
    else
        outStream.open( filename.c_str(), std::fstream::out);
}

/** Print header for monitored values */
void AdmixtureResidualsMonitor::printHeader() {
    
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

    outStream << separator << "sum";
    outStream << separator << "sum_positive";
    outStream << separator << "sum_negative";
    
    // add residual pair names
    size_t numTaxa = taxonNames.size();
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = i; j < numTaxa; j++)
        {
            outStream << separator << taxonNames[i] << "." << taxonNames[j];
        }
    }

    // end line of stream
    outStream << std::endl;
}


void AdmixtureResidualsMonitor::swapNode(DagNode *oldN, DagNode *newN) {
    
    if (oldN == residuals)
    {
        residuals = static_cast<TypedDagNode< RbVector<double> >* >(newN);
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
