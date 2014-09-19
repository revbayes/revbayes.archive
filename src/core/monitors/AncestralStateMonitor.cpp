#include "AncestralStateMonitor.h"
#include "DagNode.h"
#include "AncestralState.h"
#include "Monitor.h"
#include "RbFileManager.h"
#include "StochasticNode.h"

using namespace RevBayesCore;

/* Constructor */
AncestralStateMonitor::AncestralStateMonitor(unsigned long g, const std::string &fname, const std::string &del) : Monitor(g),
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
AncestralStateMonitor::AncestralStateMonitor( const AncestralStateMonitor &m) : Monitor( m ),
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
AncestralStateMonitor::~AncestralStateMonitor()
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
AncestralStateMonitor* AncestralStateMonitor::clone(void) const 
{
    
    return new AncestralStateMonitor(*this);
}



/**
 * Close the stream. This means that we are finished with monitoring and we close the filestream.
 */
void AncestralStateMonitor::closeStream() 
{
	
    outStream.close();
	
}


/** 
 * Monitor value at given generation.
 *
 * \param[in]   gen    The current generation.
 */
void AncestralStateMonitor::monitor(unsigned long gen) 
{
    
    if (gen % printgen == 0) 
    {
        // print the iteration number first
        outStream << gen;
		
		// call update for the marginal node likelihoods
		seq->updateMarginalNodeLikelihoods();
        
        // loop through all nodes
        for (std::vector<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i) 
        {
            // add a separator before every new element
            outStream << separator;
            
            // get the node
            DagNode *node = *i;
            
            // print the value
            node->printValue(outStream,separator);
			
			
			seq->drawAncestralStatesForNode( nodeIndex );
        }
        
        outStream << std::endl;
        
    }
}


/** 
 * Open the AncestralState stream for printing.
 */
void AncestralStateMonitor::openStream(void) 
{
    
    RbFileManager f = RbFileManager(filename);
    f.createDirectoryForFile();
    
    // open the stream to the AncestralState
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
void AncestralStateMonitor::printHeader() 
{
    
    // print one column for the iteration number
    outStream << "Iteration";
        
	
	// change this to iterate through all tree nodes and make header with node index
//    for (std::vector<DagNode *>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) 
//    {
//        // add a separator before every new element
//        outStream << separator;
//        
//        const DagNode* theNode = *it;
//        
//        // print the header
//        if (theNode->getName() != "")
//        {
//            // print the name
//			//            std::cerr << "<" << theNode << "> ";
//			//            theNode->printName( std::cerr, ", " );
//			//            std::cerr << std::endl;
//            theNode->printName(outStream,separator);
//        }
//        else
//        {
//            outStream << "Unnamed";
//        }
//    }
    
    outStream << std::endl;
}



/**
 * Reset the currently monitored DAG nodes by extracting the DAG nodes from the model again 
 * and store this in the set of DAG nodes.
 */
void AncestralStateMonitor::resetDagNodes( void )
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
            
            // only simple numeric variable can be monitored (i.e. only integer and real numbers)
            if ( (*it)->isSimpleNumeric() && !(*it)->isClamped())
            {
                if ( (!stochasticNodesOnly && !(*it)->isConstant() && (*it)->getName() != "" && !(*it)->isComposite() ) || ( (*it)->isStochastic() && !(*it)->isClamped() ) )
                {
                    if ( varNames.find( (*it)->getName() ) == varNames.end() )
                    {
                        nodes.push_back( *it );
                        varNames.insert( (*it)->getName() );
                    }
                    else
                    {
                        std::cerr << "Trying to add variable with name '" << (*it)->getName() << "' twice." << std::endl;
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
void AncestralStateMonitor::setAppend(bool tf) 
{
    
    append = tf;
    
}


/**
 * Set the model from which this monitor will extract the variables.
 * This will automatically result into a reseting of the currently monitored variables.
 *
 * \param[in]   m    The new model.
 */
void AncestralStateMonitor::setAncestralState(AncestralState *m)
{
    
    // delegate call to super class
    Monitor::setAncestralState(m);
    
    // reset the DAG nodes that should be monitored
    resetDagNodes();
    
}

