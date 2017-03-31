#include "JointConditionalContinuousAncestralStateMonitor.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "RbFileManager.h"
#include "StochasticNode.h"
#include "AbstractPhyloBrownianProcess.h"

using namespace RevBayesCore;

/* Constructor for CTMC */
JointConditionalContinuousAncestralStateMonitor::JointConditionalContinuousAncestralStateMonitor(TypedDagNode<Tree> *t, StochasticNode<ContinuousCharacterData>* ch, unsigned long g, const std::string &fname, const std::string &del, bool wt, bool wss) : Monitor(g),
    outStream(),
    filename( fname ),
    separator( del ),
    append( false ),
    tree( t ),
    ccpm( ch ),
    stochasticNodesOnly( false ),
    withTips( wt ),
    withStartStates( wss )
{
    
    addVariable( tree );
    addVariable( ccpm );
    
}


/**
 * Copy constructor.
 */
JointConditionalContinuousAncestralStateMonitor::JointConditionalContinuousAncestralStateMonitor( const JointConditionalContinuousAncestralStateMonitor &m) : Monitor( m ),
    outStream(),
    filename( m.filename ),
    separator( m.separator ),
    append( m.append ),
    tree( m.tree ),
    ccpm( m.ccpm ),
    stochasticNodesOnly( m.stochasticNodesOnly ),
    withTips( m.withTips ),
    withStartStates( m.withStartStates )
{
    if (m.outStream.is_open() == true )
    {
        openStream( true );
    }
    
}


/**
 * Destructor.
 */
JointConditionalContinuousAncestralStateMonitor::~JointConditionalContinuousAncestralStateMonitor()
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
JointConditionalContinuousAncestralStateMonitor* JointConditionalContinuousAncestralStateMonitor::clone(void) const
{
    
    return new JointConditionalContinuousAncestralStateMonitor(*this);
}



/**
 * Close the stream. This means that we are finished with monitoring and we close the filestream.
 */
void JointConditionalContinuousAncestralStateMonitor::closeStream()
{
	
    outStream.close();
	
}


/**
 * Monitor value at given generation.
 *
 * \param[in]   gen    The current generation.
 */
void JointConditionalContinuousAncestralStateMonitor::monitor(unsigned long gen)
{
    if (gen % printgen == 0)
    {
        // print the iteration number first
        outStream << gen;        
        
        // get the distribution for the character
        AbstractPhyloContinuousCharacterProcess *dist_ccpm = static_cast<AbstractPhyloContinuousCharacterProcess* >( &ccpm->getDistribution() );
        size_t num_nodes = tree->getValue().getNumberOfNodes();
//        size_t num_sites = dist_ccpm->getValue().getNumberOfCharacters();

        std::vector< ContinuousTaxonData > startStates( num_nodes, ContinuousTaxonData( Taxon("") ) );
        std::vector< ContinuousTaxonData >   endStates( num_nodes, ContinuousTaxonData( Taxon("") ) );
        
        // draw ancestral states
        dist_ccpm->drawJointConditionalAncestralStates(startStates, endStates);
        
        // print ancestral states
        const std::vector<TopologyNode*>& nds = tree->getValue().getNodes();
		for (int i = 0; i < nds.size(); i++)
		{
            size_t node_index = nds[i]->getIndex();
            
            // start states
            if (withStartStates)
            {
                // add a separator before every new element
                outStream << separator;

                // print out ancestral states....
                for (int j = 0; j < startStates[node_index].getNumberOfCharacters(); j++)
                {
                    if (j != 0)
                        outStream << ",";
                    outStream << startStates[node_index].getStringRepresentation(j);
                }
            }
            
            // end states
            if ( withTips || !nds[i]->isTip() )
            {
                outStream << separator;
                for (int j = 0; j < endStates[node_index].getNumberOfCharacters(); j++)
                {
                    if (j != 0)
                        outStream << ",";
                    outStream << endStates[node_index].getStringRepresentation(j);
                }
            }
        }
        outStream << std::endl;
        
    }
    
}


/**
 * Open the AncestralState stream for printing.
 */
void JointConditionalContinuousAncestralStateMonitor::openStream( bool reopen )
{
    
    RbFileManager f = RbFileManager(filename);
    f.createDirectoryForFile();
    
    // open the stream to the AncestralState
    if ( append == true || reopen == true )
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
void JointConditionalContinuousAncestralStateMonitor::printHeader()
{
    // print one column for the iteration number
    outStream << "Iteration";
	
	std::vector<TopologyNode*> nodes = tree->getValue().getNodes();
	
	// iterate through all tree nodes and make header with node index
	for (int i = 0; i < tree->getValue().getNumberOfNodes(); i++)
    {
		TopologyNode* nd = nodes[i];
        size_t node_index = nd->getIndex();
		if (withStartStates)
        {
            outStream << separator;
            outStream << "start_" << node_index+1;
        }
        
        // end states
        if ( withTips || !nd->isTip() )
        {
			outStream << separator;
			outStream << "end_" << node_index+1;
		}
    }
    outStream << std::endl;
}



/**
 * Set flag about whether to append to an existing file.
 *
 * \param[in]   tf   Flag if to append.
 */
void JointConditionalContinuousAncestralStateMonitor::setAppend(bool tf)
{
    
    append = tf;
    
}


void JointConditionalContinuousAncestralStateMonitor::swapNode(DagNode *oldN, DagNode* newN)
{
	
	if ( oldN == tree )
	{
		tree = static_cast< TypedDagNode<Tree> *>( newN );
	}
	else if ( oldN == ccpm )
	{
		ccpm = static_cast< StochasticNode<ContinuousCharacterData> *>( newN );
	}
    
    Monitor::swapNode( oldN, newN );
	
}
