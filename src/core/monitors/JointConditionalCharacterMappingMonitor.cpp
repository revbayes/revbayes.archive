//
//  JointConditionalCharacterMappingMonitor.cpp
//  RevBayes_development_branch
//
//  Created by will freyman on 12/14/16.
//  Copyright (c) 2016 will freyman. All rights reserved.
//

#include "JointConditionalCharacterMappingMonitor.h"

#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "RbFileManager.h"
#include "StochasticNode.h"
#include "StateDependentSpeciationExtinctionProcess.h"

using namespace RevBayesCore;


/* Constructor */
JointConditionalCharacterMappingMonitor::JointConditionalCharacterMappingMonitor(StochasticNode<Tree>* ch, unsigned long g, const std::string &fname, bool is, const std::string &del) : Monitor(g),
outStream(),
filename( fname ),
separator( del ),
append( false ),
cdbdp( ch ),
include_simmaps( is )
{
    
    // the cdbdp is both the tree and character evolution model
    addVariable( cdbdp );
    tree = static_cast< StochasticNode<Tree> *>( cdbdp );
    
}


/**
 * Copy constructor.
 */
JointConditionalCharacterMappingMonitor::JointConditionalCharacterMappingMonitor( const JointConditionalCharacterMappingMonitor &m) : Monitor( m ),
outStream(),
filename( m.filename ),
separator( m.separator ),
append( m.append ),
cdbdp( m.cdbdp ),
tree( m.tree )
{
    
    if (m.outStream.is_open() == true )
    {
        openStream( true );
    }
    
}


/**
 * Destructor.
 */
JointConditionalCharacterMappingMonitor::~JointConditionalCharacterMappingMonitor()
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
JointConditionalCharacterMappingMonitor* JointConditionalCharacterMappingMonitor::clone(void) const
{
    
    return new JointConditionalCharacterMappingMonitor(*this);
}



/**
 * Close the stream. This means that we are finished with monitoring and we close the filestream.
 */
void JointConditionalCharacterMappingMonitor::closeStream()
{
    
    outStream.close();
    
}


/**
 * Monitor value at given generation.
 *
 * \param[in]   gen    The current generation.
 */
void JointConditionalCharacterMappingMonitor::monitor(unsigned long gen)
{
    if (gen % printgen == 0)
    {
        // print the iteration number first
        outStream << gen;
        
        size_t num_nodes;
        
        // get the distribution for the character
        StateDependentSpeciationExtinctionProcess *sse_process = NULL;
        sse_process = dynamic_cast<StateDependentSpeciationExtinctionProcess*>( &nodes[0]->getDistribution() );
        num_nodes = tree->getValue().getNumberOfNodes();
        
        // draw character map from the joint distribution
        std::vector<std::string*> character_histories(num_nodes);
//        sse_process->drawJointConditionalCharacterMap(character_histories);
        
        const std::vector<TopologyNode*>& nds = tree->getValue().getNodes();
        for (int i = 0; i < nds.size(); i++)
        {
            
            size_t node_index = nds[i]->getIndex();
            
            // add a separator before every new element
            outStream << separator;
            
            // print out this branch's character history in the format
            // used by SIMMAP and phytools
            outStream << character_histories[ node_index ];
            
        }
        
        if ( include_simmaps = true )
        {
            // print out the SIMMAP/phytools compatible newick string in the last column of the log file
            outStream << separator;
            tree->getValue().addNodeParameter("character_history", character_histories, false);
            outStream << tree->getValue().getSimmapNewickRepresentation();
        }
        
        outStream << std::endl;
    }
}


/**
 * Open the CharacterMapping stream for printing.
 */
void JointConditionalCharacterMappingMonitor::openStream( bool reopen )
{
    
    RbFileManager f = RbFileManager(filename);
    f.createDirectoryForFile();
    
    // open the stream to the CharacterMapping
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
void JointConditionalCharacterMappingMonitor::printHeader()
{
    // print one column for the iteration number
    outStream << "Iteration";
    
    std::vector<TopologyNode*> nodes = tree->getValue().getNodes();
    
    // iterate through all tree nodes and make header with node index
    for (int i = 0; i < tree->getValue().getNumberOfNodes(); i++)
    {
        TopologyNode* nd = nodes[i];
        size_t node_index = nd->getIndex();
        
        outStream << separator;
        outStream << node_index + 1;
        
    }
    
    if ( include_simmaps = true )
    {
        outStream << separator;
        outStream << "simmap";
    }
    
    outStream << std::endl;
}



/**
 * Set flag about whether to append to an existing file.
 *
 * \param[in]   tf   Flag if to append.
 */
void JointConditionalCharacterMappingMonitor::setAppend(bool tf)
{
    
    append = tf;
    
}


void JointConditionalCharacterMappingMonitor::swapNode(DagNode *oldN, DagNode* newN)
{
    
    if ( oldN == cdbdp )
    {
        cdbdp = static_cast< StochasticNode<Tree> *>( newN );
        tree = static_cast< StochasticNode<Tree> *>( newN );
    }
    
    Monitor::swapNode( oldN, newN );
    
}
