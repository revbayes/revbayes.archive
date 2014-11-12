#include "ModelMonitor.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "RbFileManager.h"
#include "StochasticNode.h"

using namespace RevBayesCore;

/* Constructor */
ModelMonitor::ModelMonitor(unsigned long g, const std::string &fname, const std::string &del) : Monitor(g),
    outStream(), 
    filename( fname ), 
    separator( del ), 
    posterior( true ), 
    likelihood( true ), 
    prior( true ), 
    append( false ),
    stochasticNodesOnly( false )
{
    
}


/**
 * Copy constructor.
 */
ModelMonitor::ModelMonitor( const ModelMonitor &m) : Monitor( m ),
    outStream(), 
    filename( m.filename ), 
    separator( m.separator ), 
    posterior( m.posterior ), 
    likelihood( m.likelihood ), 
    prior( m.prior ), 
    append( m.append ),
    stochasticNodesOnly( m.stochasticNodesOnly )
{
    if (m.outStream.is_open())
    {
        openStream();
    }
    
}


/**
 * Destructor.
 */
ModelMonitor::~ModelMonitor()
{
    
    if ( outStream.is_open() ) 
    {
        closeStream();
    }
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
ModelMonitor* ModelMonitor::clone(void) const 
{
    
    return new ModelMonitor(*this);
}



/**
 * Close the stream. This means that we are finished with monitoring and we close the filestream.
 */
void ModelMonitor::closeStream() 
{

    outStream.close();

}


/** 
 * Monitor value at given generation.
 *
 * \param[in]   gen    The current generation.
 */
void ModelMonitor::monitor(unsigned long gen) 
{
    
    if (gen % printgen == 0) 
    {
        // print the iteration number first
        outStream << gen;
        
        if ( posterior ) 
        {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) 
            {
                pp += (*it)->getLnProbability();
            }
            outStream << pp;
        }
        
        if ( likelihood ) 
        {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) 
            {
                if ( (*it)->isClamped() ) 
                {
                    pp += (*it)->getLnProbability();
                }
            }
            outStream << pp;
        }
        
        if ( prior ) 
        {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) 
            {
                if ( !(*it)->isClamped() ) 
                {
                    pp += (*it)->getLnProbability();
                }
            }
            outStream << pp;
        }
        
        for (std::vector<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i) 
        {
            // add a separator before every new element
            outStream << separator;
            
            // get the node
            DagNode *theNode = *i;
            
            if ( theNode->getName() != "" && theNode->isHidden() == false )
            {
                // print the value
                theNode->printValueElements(outStream,separator);
            }
        }
        
        outStream << std::endl;
        
    }
}


/** 
 * Open the Model stream for printing.
 */
void ModelMonitor::openStream(void) 
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
void ModelMonitor::printHeader() 
{
    
    // print one column for the iteration number
    outStream << "Iteration";
    
    if ( posterior ) 
    {
        // add a separator before every new element
        outStream << separator;
        outStream << "Posterior";
    }
    
    if ( likelihood ) 
    {
        // add a separator before every new element
        outStream << separator;
        outStream << "Likelihood";
    }
    
    if ( prior ) 
    {
        // add a separator before every new element
        outStream << separator;
        outStream << "Prior";
    }
    
    for (std::vector<DagNode *>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) 
    {
        // add a separator before every new element
        outStream << separator;
        
        const DagNode* theNode = *it;
        
        // print the header
        if (theNode->getName() != "" && theNode->isHidden() == false)
        {
            // print the name
            theNode->printName(outStream,separator);
        }
        else
        {
            outStream << "Unnamed";
        }
    }
    
    outStream << std::endl;
}



/**
 * Reset the currently monitored DAG nodes by extracting the DAG nodes from the model again 
 * and store this in the set of DAG nodes.
 */
void ModelMonitor::resetDagNodes( void )
{
    
    // for savety we empty our dag nodes
    nodes.clear();
    
    if ( model != NULL )
    {
        // we only want to have each nodes once
        // this should by default happen by here we check again
        std::set<std::string> varNames;
        
        const std::vector<DagNode*> &n = model->getDagNodes();
        for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) 
        {
            
            DagNode *theNode = *it;
            
            // only simple numeric variable can be monitored (i.e. only integer and real numbers)
            if ( theNode->isSimpleNumeric() && !theNode->isClamped())
            {
                if ( (!stochasticNodesOnly && !theNode->isConstant() && theNode->getName() != "" && !theNode->isHidden() ) || ( theNode->isStochastic() && !theNode->isClamped() && theNode->isHidden() == false ) )
                {
                    if ( varNames.find( theNode->getName() ) == varNames.end() )
                    {
                        nodes.push_back( theNode );
                        varNames.insert( theNode->getName() );
                    }
                    else
                    {
                        std::cerr << "Trying to add variable with name '" << theNode->getName() << "' twice." << std::endl;
                    }
                }
            }
        
        }
    }
    
}


/**
 * Set flag about whether to append to an existing file. 
 *
 * \param[in]   tf   Flag if to append.
 */
void ModelMonitor::setAppend(bool tf) 
{
    
    append = tf;
    
}


/**
 * Set the model from which this monitor will extract the variables.
 * This will automatically result into a reseting of the currently monitored variables.
 *
 * \param[in]   m    The new model.
 */
void ModelMonitor::setModel(Model *m)
{
    
    // delegate call to super class
    Monitor::setModel(m);
    
    // reset the DAG nodes that should be monitored
    resetDagNodes();
    
}


/**
 * Set flag about whether to print the likelihood. 
 *
 * \param[in]   tf   Flag if the likelihood should be printed.
 */
void ModelMonitor::setPrintLikelihood(bool tf) 
{
    
    likelihood = tf;
    
}


/**
 * Set flag about whether to print the posterior probability. 
 *
 * \param[in]   tf   Flag if the posterior probability should be printed.
 */
void ModelMonitor::setPrintPosterior(bool tf) 
{
    
    posterior = tf;
    
}


/**
 * Set flag about whether to print the prior probability. 
 *
 * \param[in]   tf   Flag if the prior probability should be printed.
 */
void ModelMonitor::setPrintPrior(bool tf) 
{
    
    prior = tf;
    
}


/**
 * Set flag about whether to monitor only stochastic DAG nodes or deterministic ones as well. 
 *
 * \param[in]   tf   Flag if only stochastic nodes should be monitored.
 */
void ModelMonitor::setStochasticNodesOnly(bool tf) 
{
    
    stochasticNodesOnly = tf;
    
    // reset the DAG nodes that should be monitored
    resetDagNodes();
    
}

