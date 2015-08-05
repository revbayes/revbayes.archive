#include "PosteriorPredictiveMonitor.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "RbFileManager.h"
#include "StochasticNode.h"

using namespace RevBayesCore;

/* Constructor */
PosteriorPredictiveMonitor::PosteriorPredictiveMonitor(unsigned long g, const std::string &fname, const std::string &del) : Monitor(g),
    outStream(),
    filename( fname ),
    separator( del ),
    append( false ),
    predictions()
{
    
}


/**
 * Copy constructor.
 */
PosteriorPredictiveMonitor::PosteriorPredictiveMonitor( const PosteriorPredictiveMonitor &m) : Monitor( m ),
    outStream(),
    filename( m.filename ),
    separator( m.separator ),
    append( m.append ),
    predictions()
{
    
    if (m.outStream.is_open())
    {
        openStream();
    }
    
    for ( std::vector<Distribution*>::const_iterator it = m.predictions.begin(); it != m.predictions.end(); ++it)
    {
        Distribution *d = (*it)->clone();
        predictions.push_back( d );
    }
    
}


/**
 * Destructor.
 */
PosteriorPredictiveMonitor::~PosteriorPredictiveMonitor()
{
    
    if ( outStream.is_open() )
    {
        closeStream();
    }
    
    clear();
    
}


/**
 * Free the memory of the current distribution objects and empty the vector.
 */
void PosteriorPredictiveMonitor::clear( void )
{
    
    for ( std::vector<Distribution*>::const_iterator it = predictions.begin(); it != predictions.end(); ++it)
    {
        Distribution *obs = (*it);
        delete obs;

    }
    
    predictions.clear();
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
PosteriorPredictiveMonitor* PosteriorPredictiveMonitor::clone(void) const
{
    
    return new PosteriorPredictiveMonitor(*this);
}



/**
 * Close the stream. This means that we are finished with monitoring and we close the filestream.
 */
void PosteriorPredictiveMonitor::closeStream()
{
    
    outStream.close();
    
}


/**
 * Monitor value at given generation.
 *
 * \param[in]   gen    The current generation.
 */
void PosteriorPredictiveMonitor::monitor(unsigned long gen)
{
    
    if (gen % printgen == 0)
    {
        // print the iteration number first
        outStream << gen;
        
        double lnl = 0.0;
        if ( true )
        {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it)
            {
                if ( (*it)->isClamped() )
                {
                    lnl += (*it)->getLnProbability();
                }
            }
            outStream << lnl;
        }
        
        double predlnl = 0.0;

        
        for (std::vector<Distribution*>::iterator i = predictions.begin(); i != predictions.end(); ++i)
        {
            
            // get the node
            Distribution *theNode = *i;
            
            theNode->redrawValue();
            predlnl += theNode->computeLnProbability();

        }

        // add a separator before every new element
        outStream << separator;
        outStream << predlnl;
        
        // add a separator before every new element
        outStream << separator;
        outStream << lnl-predlnl;
        
        outStream << std::endl;
        
    }
}


/**
 * Open the Model stream for printing.
 */
void PosteriorPredictiveMonitor::openStream(void)
{
    
    RbFileManager f = RbFileManager(filename);
    f.createDirectoryForFile();
    
    // open the stream to the Model
    if ( append )
    {
        outStream.open( filename.c_str(), std::fstream::out | std::fstream::app);
    }
    else
    {
        outStream.open( filename.c_str(), std::fstream::out);
    }
    
}

/**
 * Print header for monitored values
 */
void PosteriorPredictiveMonitor::printHeader()
{
    
    // print one column for the iteration number
    outStream << "Iteration";
    
    if ( true )
    {
        // add a separator before every new element
        outStream << separator;
        outStream << "Likelihood";
    }
    
    // add a separator before every new element
    outStream << separator;
    outStream << "PredictiveLikelihood";
    
    // add a separator before every new element
    outStream << separator;
    outStream << "PredictiveObservedLikelihoodDifference";
    
    
    outStream << std::endl;
}



/**
 * Reset the distributions objects of the clamped variables.
 */
void PosteriorPredictiveMonitor::resetDistribution( void )
{
    
    // empty and free memory of old distributions
    clear();
    
    if ( model != NULL )
    {
        // we only want to have each nodes once
        // this should by default happen by here we check again
        std::set<std::string> varNames;
        
        const std::vector<DagNode*> &n = model->getDagNodes();
        for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it)
        {
            
            DagNode *theNode = *it;
            
            // we need the clamped nodes
            if ( theNode->isClamped() )
            {
                
                Distribution *obs = theNode->getDistribution().clone();
                predictions.push_back( obs );
                
            }
            
        }
    }
    
}


/**
 * Set flag about whether to append to an existing file.
 *
 * \param[in]   tf   Flag if to append.
 */
void PosteriorPredictiveMonitor::setAppend(bool tf)
{
    
    append = tf;
    
}


/**
 * Set the model from which this monitor will extract the variables.
 * This will automatically result into a reseting of the currently monitored variables.
 *
 * \param[in]   m    The new model.
 */
void PosteriorPredictiveMonitor::setModel(Model *m)
{
    
    // delegate call to super class
    Monitor::setModel(m);
    
    // reset the DAG nodes that should be monitored
    resetDistribution();
    
}




