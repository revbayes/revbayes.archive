#ifndef AncestralStateMonitor_H
#define AncestralStateMonitor_H

#include "Monitor.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "StochasticNode.h"
#include "DnaState.h"

#include <fstream>
#include <string>
#include <vector>
#include <typeinfo>

#define FAILED 0
#define PASSED 1
#define NOT_CHECKED 2

namespace RevBayesCore {
    
    /**
     * @brief Declaration and implementation of the AncestralStateMonitor class.
     *
     * @file 
	 * Declaration and implementation of the AncestralStateMonitor class which
	 * monitors the ancestral states of each internal node and
	 * prints their value into a file.
     *
     * @copyright Copyright 2014-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @version 1.0
     *
     */
    template<class characterType, class treeType> 
	class AncestralStateMonitor : public Monitor {
	
    public:
        // Constructors and Destructors
		AncestralStateMonitor(TypedDagNode<treeType> *t, DagNode* &ch, unsigned long g, const std::string &fname, const std::string &del);                                  //!< Constructor
		
        AncestralStateMonitor(const AncestralStateMonitor &m);
        virtual ~AncestralStateMonitor(void);
        
        AncestralStateMonitor*              clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                monitor(unsigned long gen);                                         //!< Monitor at generation gen
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(void);                                                   //!< Open the stream for writing
        void                                printHeader(void);                                                  //!< Print header
        
        // getters and setters
        void                                setAppend(bool tf);                                                 //!< Set if the monitor should append to an existing file
		void								swapNode(DagNode *oldN, DagNode *newN);
		
    private:
        
        // members
        std::fstream                        outStream;
        
        // parameters
        std::string                         filename;                                                           //!< Filename to which we print the values
        std::string                         separator;                                                          //!< Seperator between monitored values (between columns)
		bool                                append;                                                             //!< Flag if to append to existing file
		TypedDagNode<treeType>*             tree;
		DagNode*                            ctmc;
		bool                                stochasticNodesOnly;
    };
    
}


#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "RbFileManager.h"
#include "StochasticNode.h"
#include "PhyloCTMCSiteHomogeneous.h"
#include "AbstractPhyloCTMCSiteHomogeneous.h"

using namespace RevBayesCore;


/* Constructor */
template<class characterType, class treeType>
AncestralStateMonitor<characterType, treeType>::AncestralStateMonitor(TypedDagNode<treeType> *t, DagNode* &ch, unsigned long g, const std::string &fname, const std::string &del) : Monitor(g),
    outStream(),
    filename( fname ),
    separator( del ),
    append( false ),
    tree( t ),
    ctmc( ch ),
    stochasticNodesOnly( false )
{
	
	nodes.push_back( tree );
	nodes.push_back( ctmc );
    
    // tell the nodes that we have a reference to it (avoids deletion)
    tree->incrementReferenceCount();
    ctmc->incrementReferenceCount();
}


/**
 * Copy constructor.
 */
template<class characterType, class treeType>
AncestralStateMonitor<characterType, treeType>::AncestralStateMonitor( const AncestralStateMonitor &m) : Monitor( m ),
    outStream(),
    filename( m.filename ),
    separator( m.separator ),
    append( m.append ),
    tree( m.tree ),
    ctmc( m.ctmc ),
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
template<class characterType, class treeType>
AncestralStateMonitor<characterType, treeType>::~AncestralStateMonitor()
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
template<class characterType, class treeType>
AncestralStateMonitor<characterType, treeType>* AncestralStateMonitor<characterType, treeType>::clone(void) const 
{
    
    return new AncestralStateMonitor<characterType, treeType>(*this);
}



/**
 * Close the stream. This means that we are finished with monitoring and we close the filestream.
 */
template<class characterType, class treeType>
void AncestralStateMonitor<characterType, treeType>::closeStream() 
{
	
    outStream.close();
	
}


/** 
 * Monitor value at given generation.
 *
 * \param[in]   gen    The current generation.
 */
template<class characterType, class treeType>
void AncestralStateMonitor<characterType, treeType>::monitor(unsigned long gen) 
{
    if (gen % printgen == 0) 
    {
        // print the iteration number first
        outStream << gen;
		
		// convert 'ctmc' from a DagNode to a StochasticNode so that we can call ctmc->getDistribution()
		StochasticNode<PhyloCTMCSiteHomogeneous<characterType, treeType> > *char_stoch = (StochasticNode<PhyloCTMCSiteHomogeneous<characterType, treeType> >*) ctmc;
		
		// now  get the TypedDistribution and cast it as an AbstractSiteHomogeneousMixtureCharEvoModel distribution
		PhyloCTMCSiteHomogeneous<characterType, treeType> *dist = (PhyloCTMCSiteHomogeneous<characterType, treeType>*) &char_stoch->getDistribution();
		
		// update the marginal node likelihoods
		dist->updateMarginalNodeLikelihoods();
        
		std::vector<TopologyNode*> nodes = tree->getValue().getNodes();
		
        // loop through all tree nodes
		for (int i = 0; i < tree->getValue().getNumberOfNodes(); i++)
		{		
			
			// we need to print values for all internal nodes
			// we assume that tip nodes always precede internal nodes
			TopologyNode* the_node = nodes[i];
			
			if ( !the_node->isTip() )
            {
				
				// add a separator before every new element
				outStream << separator;
				
				// for each node print
				// site1,site2,site3
				
				std::vector<characterType> ancestralStates = dist->drawAncestralStatesForNode( *the_node );
                
                bool hasCladogenicStates = false; // dist->hasCladogenicStates();
                std::vector<characterType> cladogenicStates;
                if (hasCladogenicStates)
                {
                    ; // cladogenicStates = dist->drawCladogenicStatesForNode( *the_node );
                }
				
				// print out ancestral states....
				for (int j = 0; j < ancestralStates.size(); j++)
				{
					outStream << ancestralStates[j].getStringValue();
					if (j != ancestralStates.size()-1 && typeid(ancestralStates[j]) != typeid(DnaState) )
                    {
						outStream << ",";
					}
				}
			} 
        }
        outStream << std::endl;
        
    }
}


/** 
 * Open the AncestralState stream for printing.
 */
template<class characterType, class treeType>
void AncestralStateMonitor<characterType, treeType>::openStream(void) 
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
template<class characterType, class treeType>
void AncestralStateMonitor<characterType, treeType>::printHeader() 
{
    // print one column for the iteration number
    outStream << "Iteration";
	
	std::vector<TopologyNode*> nodes = tree->getValue().getNodes();
	
	// iterate through all tree nodes and make header with node index
	for (int i = 0; i < tree->getValue().getNumberOfNodes(); i++)
    {
		TopologyNode* the_node = nodes[i];
		if ( !the_node->isTip() )
        {
			// add a separator before every new element
			outStream << separator;
			
			// print the node index
			outStream << the_node->getIndex();
		}
    }
    outStream << std::endl;
}



/**
 * Set flag about whether to append to an existing file. 
 *
 * \param[in]   tf   Flag if to append.
 */
template<class characterType, class treeType>
void AncestralStateMonitor<characterType, treeType>::setAppend(bool tf) 
{
    
    append = tf;
    
}


template<class characterType, class treeType>
void AncestralStateMonitor<characterType, treeType>::swapNode(DagNode *oldN, DagNode* newN)
{
	
	if ( oldN == tree ) 
	{
		tree = static_cast< TypedDagNode<treeType> *>( newN );
	}
	else if ( oldN == ctmc )
	{
		ctmc = newN;
	}

    Monitor::swapNode( oldN, newN );
	
}




#endif

