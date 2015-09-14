//
//  JointConditionalJointConditionalAncestralStateMonitor.h
//  revbayes-proj
//
//  Created by Michael Landis on 2/28/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__JointConditionalJointConditionalAncestralStateMonitor__
#define __revbayes_proj__JointConditionalJointConditionalAncestralStateMonitor__

#include "AbstractDiscreteCharacterData.h"
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
     * @brief Declaration and implementation of the JointConditionalAncestralStateMonitor class.
     *
     * @file
	 * Declaration and implementation of the JointConditionalAncestralStateMonitor class which
	 * monitors the ancestral states of each internal node and
	 * prints their value into a file.
     *
     * @copyright Copyright 2014-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @version 1.0
     *
     */
    template<class characterType, class treeType>
	class JointConditionalAncestralStateMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
		JointConditionalAncestralStateMonitor(TypedDagNode<treeType> *t, StochasticNode<AbstractDiscreteCharacterData>* ch, unsigned long g, const std::string &fname, const std::string &del, bool wt, bool wss);                                  //!< Constructor
		
        JointConditionalAncestralStateMonitor(const JointConditionalAncestralStateMonitor &m);
        virtual ~JointConditionalAncestralStateMonitor(void);
        
        JointConditionalAncestralStateMonitor*              clone(void) const;                                                  //!< Clone the object
        
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
		StochasticNode<AbstractDiscreteCharacterData>*                            ctmc;
		bool                                stochasticNodesOnly;
        bool                                withTips;
        bool                                withStartStates;
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
JointConditionalAncestralStateMonitor<characterType, treeType>::JointConditionalAncestralStateMonitor(TypedDagNode<treeType> *t, StochasticNode<AbstractDiscreteCharacterData>* ch, unsigned long g, const std::string &fname, const std::string &del, bool wt, bool wss) : Monitor(g),
outStream(),
filename( fname ),
separator( del ),
append( false ),
tree( t ),
ctmc( ch ),
stochasticNodesOnly( false ),
withTips( wt ),
withStartStates( wss )
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
JointConditionalAncestralStateMonitor<characterType, treeType>::JointConditionalAncestralStateMonitor( const JointConditionalAncestralStateMonitor &m) : Monitor( m ),
outStream(),
filename( m.filename ),
separator( m.separator ),
append( m.append ),
tree( m.tree ),
ctmc( m.ctmc ),
stochasticNodesOnly( m.stochasticNodesOnly ),
withTips( m.withTips ),
withStartStates( m.withStartStates )
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
JointConditionalAncestralStateMonitor<characterType, treeType>::~JointConditionalAncestralStateMonitor()
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
JointConditionalAncestralStateMonitor<characterType, treeType>* JointConditionalAncestralStateMonitor<characterType, treeType>::clone(void) const
{
    
    return new JointConditionalAncestralStateMonitor<characterType, treeType>(*this);
}



/**
 * Close the stream. This means that we are finished with monitoring and we close the filestream.
 */
template<class characterType, class treeType>
void JointConditionalAncestralStateMonitor<characterType, treeType>::closeStream()
{
	
    outStream.close();
	
}


/**
 * Monitor value at given generation.
 *
 * \param[in]   gen    The current generation.
 */
template<class characterType, class treeType>
void JointConditionalAncestralStateMonitor<characterType, treeType>::monitor(unsigned long gen)
{
    if (gen % printgen == 0)
    {
        // print the iteration number first
        outStream << gen;        
        
		AbstractPhyloCTMCSiteHomogeneous<characterType, treeType> *dist = static_cast<AbstractPhyloCTMCSiteHomogeneous<characterType, treeType>* >( &ctmc->getDistribution() );
        size_t numSites = dist->getValue().getNumberOfCharacters();
        size_t numNodes = tree->getValue().getNumberOfNodes();

        // draw characters
        std::vector<std::vector<characterType> > startStates(numNodes,std::vector<characterType>(numSites));
        std::vector<std::vector<characterType> > endStates(numNodes,std::vector<characterType>(numSites));
        dist->drawJointConditionalAncestralStates(startStates, endStates);
        
        
        const std::vector<TopologyNode*>& nds = tree->getValue().getNodes();
		for (int i = 0; i < nds.size(); i++)
		{
            size_t nodeIndex = nds[i]->getIndex();
            
            // start states
            if (withStartStates)
            {
                // add a separator before every new element
                outStream << separator;

                // print out ancestral states....
                for (int j = 0; j < startStates[nodeIndex].size(); j++)
                {
                    if (j != 0)
                        outStream << ",";
                    outStream << startStates[nodeIndex][j].getStringValue();
                }
            }
            
            // end states
            if ( withTips || !nds[i]->isTip() )
            {
                outStream << separator;
                for (int j = 0; j < endStates[nodeIndex].size(); j++)
                {
                    if (j != 0)
                        outStream << ",";
                    outStream << endStates[nodeIndex][j].getStringValue();
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
void JointConditionalAncestralStateMonitor<characterType, treeType>::openStream(void)
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
void JointConditionalAncestralStateMonitor<characterType, treeType>::printHeader()
{
    // print one column for the iteration number
    outStream << "Iteration";
	
	std::vector<TopologyNode*> nodes = tree->getValue().getNodes();
	
	// iterate through all tree nodes and make header with node index
	for (int i = 0; i < tree->getValue().getNumberOfNodes(); i++)
    {
		TopologyNode* nd = nodes[i];
        size_t nodeIndex = nd->getIndex();
		if (withStartStates)
        {
            outStream << separator;
            outStream << "start_" << nodeIndex;
        }
        
        // end states
        if ( withTips || !nd->isTip() )
        {
			outStream << separator;
			outStream << "end_" << nodeIndex;
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
void JointConditionalAncestralStateMonitor<characterType, treeType>::setAppend(bool tf)
{
    
    append = tf;
    
}


template<class characterType, class treeType>
void JointConditionalAncestralStateMonitor<characterType, treeType>::swapNode(DagNode *oldN, DagNode* newN)
{
	
	if ( oldN == tree )
	{
		tree = static_cast< TypedDagNode<treeType> *>( newN );
	}
	else if ( oldN == ctmc )
	{
		ctmc = static_cast< StochasticNode<AbstractDiscreteCharacterData> *>( newN );
	}
    
    Monitor::swapNode( oldN, newN );
	
}

#endif /* defined(__revbayes_proj__JointConditionalJointConditionalAncestralStateMonitor__) */
