#include "AncestralStateMonitor.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "RbFileManager.h"
#include "StochasticNode.h"
#include "GeneralBranchHeterogeneousCharEvoModel.h"
#include "AbstractSiteHomogeneousMixtureCharEvoModel.h"
#include "ChromosomesState.h"

using namespace RevBayesCore;


/* Constructor */
AncestralStateMonitor::AncestralStateMonitor(TypedDagNode<Tree> *t, RevBayesCore::DagNode* &ch, unsigned long g, const std::string &fname, const std::string &del) : Monitor(g),
outStream(), 
filename( fname ), 
separator( del ), 
tree( t ),
character( ch ),
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
tree( m.tree ), 
character( m.character ), 
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
		
		// TODO!! make this work for CharacterStates other than ChromosomesState
		
		// convert 'character' which is DagNode to a StochasticNode
		// so that we can call character->getDistribution()
		StochasticNode<AbstractSiteHomogeneousMixtureCharEvoModel<ChromosomesState, BranchLengthTree> > *char_stoch 
			= (StochasticNode<AbstractSiteHomogeneousMixtureCharEvoModel<ChromosomesState, BranchLengthTree> >*) character;		
				
		// now we get the TypedDistribution and need to cast it  
		// into an AbstractSiteHomogeneousMixtureCharEvoModel distribution
		AbstractSiteHomogeneousMixtureCharEvoModel<ChromosomesState, BranchLengthTree> *dist
			= (AbstractSiteHomogeneousMixtureCharEvoModel<ChromosomesState, BranchLengthTree>*) &char_stoch->getDistribution();
		
		// call update for the marginal node likelihoods
		dist->updateMarginalNodeLikelihoods();
        
		std::vector<TopologyNode*> nodes = tree->getValue().getNodes();
		
        // loop through all tree nodes
		for (int i = 0; i < tree->getValue().getNumberOfNodes(); i++)
		{		
			// add a separator before every new element
			outStream << separator;
			
			// for each node print
			// site1,site2,site3
			
			TopologyNode* the_node = nodes[i];
			
			// get the node index
//			int nodeIndex = the_node->getIndex();
			
			// TODO: make this function a template so as to accept other CharacterState objects
//			std::vector<ChromosomesState> ancestralStates = dist->drawAncestralStatesForNode( nodeIndex );
			std::vector<ChromosomesState> ancestralStates = dist->drawAncestralStatesForNode( *the_node );
				
			// print out ancestral states....
			for (int j = 0; j < ancestralStates.size(); j++)
			{
				outStream << ancestralStates[j].getStringValue();
				if (j != ancestralStates.size()-1) {
					outStream << ",";
				}
			}
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
        
	std::vector<TopologyNode*> nodes = tree->getValue().getNodes();
	
	// iterate through all tree nodes and make header with node index
	for (int i = 0; i < tree->getValue().getNumberOfNodes(); i++)
    {
		TopologyNode* the_node = nodes[i];
        // add a separator before every new element
        outStream << separator;
        
		// print the node index
		outStream << the_node->getIndex();
    }
    
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



