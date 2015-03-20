#include "AbstractFileMonitor.h"
#include "DagNode.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "RbFileManager.h"

using namespace RevBayesCore;

/* Constructor */
AbstractFileMonitor::AbstractFileMonitor(DagNode *n, unsigned long g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap) : Monitor(g,n),
    outStream(),
    filename( fname ),
    workingFileName( fname ),
    replicateIndex( 0 ),
    separator( del ),
    posterior( pp ),
    prior( pr ),
    likelihood( l ),
    append(ap)
{
    
}


AbstractFileMonitor::AbstractFileMonitor(const std::vector<DagNode *> &n, unsigned long g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap) : Monitor(g,n),
    outStream(),
    filename( fname ),
    workingFileName( fname ),
    replicateIndex( 0 ),
    separator( del ),
    posterior( pp ),
    prior( pr ),
    likelihood( l ),
    append(ap)
{
    
}


AbstractFileMonitor::AbstractFileMonitor(const AbstractFileMonitor &f) : Monitor( f ),
    outStream()
{
    
    filename        = f.filename;
    workingFileName = f.workingFileName;
    replicateIndex  = f.replicateIndex;
    separator       = f.separator;
    prior           = f.prior;
    posterior       = f.posterior;
    likelihood      = f.likelihood;
    append          = f.append;
    
    //    if (f.outStream.is_open())
    //    {
    //        openStream();
    //    }
    
}


void AbstractFileMonitor::closeStream()
{
    //    outStream.close();
}


/** 
 * Monitor value at generation gen 
 */
void AbstractFileMonitor::monitorVariables(unsigned long gen)
{
        
    for (std::vector<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
    {
        // add a separator before every new element
        outStream << separator;
            
        // get the node
        DagNode *node = *i;
            
        // print the value
        node->printValueElements(outStream, separator);
    }
    
}


/**
 * Monitor 
 */
void AbstractFileMonitor::monitor(unsigned long gen)
{
    
    outStream.open( workingFileName.c_str(), std::fstream::out | std::fstream::app);
    
//    if ( nodes[0]->getName() != "pi" && nodes[0]->getName() != "tau" )
//    {
//        std::cerr << "Strange first node:\t\t" << nodes[0]->getName() << std::endl;
//    }
    
    // get the printing frequency
    unsigned long samplingFrequency = printgen;
    
    if (gen % samplingFrequency == 0)
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
        
        if ( likelihood ) {
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
        
        if ( prior ) {
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
        
        monitorVariables( gen );
        
        outStream << std::endl;
        
    }
    
    outStream.close();
    
}


/** open the file stream for printing */
void AbstractFileMonitor::openStream(void)
{
    
    RbFileManager f = RbFileManager(workingFileName);
    f.createDirectoryForFile();
    
    // open the stream to the file
    if (append)
    {
        outStream.open( workingFileName.c_str(), std::fstream::out | std::fstream::app);
    }
    else
    {
        outStream.open( workingFileName.c_str(), std::fstream::out);
    }
    
    outStream.close();
    
}

/** 
 * Print header for monitored values 
 */
void AbstractFileMonitor::printHeader( void )
{
    
    outStream.open( workingFileName.c_str(), std::fstream::out | std::fstream::app);
    
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
    
    // print the headers for the variables
    printFileHeader();
    
    outStream << std::endl;
    
    outStream.close();
    
}


/**
 * Print additional header for monitored values
 */
void AbstractFileMonitor::printFileHeader( void )
{
    
    for (std::vector<DagNode *>::const_iterator it=nodes.begin(); it!=nodes.end(); it++)
    {
        // add a separator before every new element
        outStream << separator;
        
        const DagNode* theNode = *it;
        
        // print the header
        if (theNode->getName() != "")
        {
            theNode->printName(outStream,separator);
        }
        else
        {
            outStream << "Unnamed";
        }
        
    }
    
}


/**
 * Set flag about whether to append to an existing file.
 *
 * \param[in]   tf   Flag if to append.
 */
void AbstractFileMonitor::setAppend(bool tf)
{
    
    append = tf;
    
}


/**
 * Set flag about whether to print the likelihood.
 *
 * \param[in]   tf   Flag if the likelihood should be printed.
 */
void AbstractFileMonitor::setPrintLikelihood(bool tf)
{
    
    likelihood = tf;
    
}


/**
 * Set flag about whether to print the posterior probability.
 *
 * \param[in]   tf   Flag if the posterior probability should be printed.
 */
void AbstractFileMonitor::setPrintPosterior(bool tf)
{
    
    posterior = tf;
    
}


/**
 * Set flag about whether to print the prior probability.
 *
 * \param[in]   tf   Flag if the prior probability should be printed.
 */
void AbstractFileMonitor::setPrintPrior(bool tf)
{
    
    prior = tf;
    
}



/**
 * Set the replicate index.
 * If the index is larger than 0, then we add it to the filename.
 */
void AbstractFileMonitor::setReplicateIndex(size_t idx)
{
    
    // store the index for possible later uses
    replicateIndex = idx;
    
    // compute the working filename
    if ( replicateIndex > 0 )
    {
        RbFileManager fm = RbFileManager(filename);
        workingFileName = fm.getFilePath() + fm.getPathSeparator() + fm.getFileNameWithoutExtension() + "_run_" + idx + "." + fm.getFileExtension();
    }
    
}
/**
 * Set the replicate index.
 * If the index is larger than 0, then we add it to the filename.
 */
void AbstractFileMonitor::setStoneIndex(size_t idx)
{
    
    // store the index for possible later uses
    replicateIndex = idx;
    
    // compute the working filename
    if ( replicateIndex > 0 )
    {
        RbFileManager fm = RbFileManager(filename);
        workingFileName = fm.getFilePath() + fm.getPathSeparator() + fm.getFileNameWithoutExtension() + "_stone_" + idx + "." + fm.getFileExtension();
    }
    
}


