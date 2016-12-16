//
//  JointConditionalCharacterMappingMonitor.h
//  RevBayes_development_branch
//
//  Created by will freyman on 12/13/16.
//  Copyright (c) 2016 will freyman. All rights reserved.
//


#ifndef RevBayes_development_branch_JointConditionalCharacterMappingMonitor_h
#define RevBayes_development_branch_JointConditionalCharacterMappingMonitor_h


#include "AbstractHomologousDiscreteCharacterData.h"
#include "StateDependentSpeciationExtinctionProcess.h"
#include "Monitor.h"
#include "TypedDagNode.h"
#include "StochasticNode.h"

#include <fstream>
#include <string>
#include <vector>
#include <typeinfo>


namespace RevBayesCore {
    
    /**
     * @brief Declaration and implementation of the JointConditionalCharacterMappingMonitor class.
     *
     * @file
     * Declaration and implementation of the JointConditionalCharacterMappingMonitor class which
     * monitors the ancestral states of each time slice of a character dependent birth death process
     * and prints their value into a file.
     *
     */
    template<class characterType>
    class JointConditionalCharacterMappingMonitor : public Monitor {
        
    public:
        
        // Constructors and Destructors
        JointConditionalCharacterMappingMonitor(StochasticNode<Tree>* ch, unsigned long g, const std::string &fname, const std::string &del);
        JointConditionalCharacterMappingMonitor(const JointConditionalCharacterMappingMonitor &m);
        virtual ~JointConditionalCharacterMappingMonitor(void);
        
        JointConditionalCharacterMappingMonitor*        clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                            monitor(unsigned long gen);                                         //!< Monitor at generation gen
        void                                            closeStream(void);                                                  //!< Close stream after finish writing
        void                                            openStream(bool reopen);                                            //!< Open the stream for writing
        void                                            printHeader(void);                                                  //!< Print header
        
        // getters and setters
        void                                            setAppend(bool tf);                                                 //!< Set if the monitor should append to an existing file
        void                                            swapNode(DagNode *oldN, DagNode *newN);
        
    private:
        
        // members
        std::fstream                                    outStream;
        
        // parameters
        std::string                                     filename;                                                           //!< Filename to which we print the values
        std::string                                     separator;                                                          //!< Seperator between monitored values (between columns)
        bool                                            append;                                                             //!< Flag if to append to existing file
        TypedDagNode<Tree>*                             tree;
        StochasticNode<Tree>*                           cdbdp;                                                              //!< The character dependent birth death process we are monitoring

    };
    
}


#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "RbFileManager.h"
#include "StochasticNode.h"
#include "StateDependentSpeciationExtinctionProcess.h"

using namespace RevBayesCore;


/* Constructor */
template<class characterType>
JointConditionalCharacterMappingMonitor<characterType>::JointConditionalCharacterMappingMonitor(StochasticNode<Tree>* ch, unsigned long g, const std::string &fname, const std::string &del) : Monitor(g),
outStream(),
filename( fname ),
separator( del ),
append( false ),
cdbdp( ch )
{

    // the cdbdp is both the tree and character evolution model
    addVariable( cdbdp );
    tree = static_cast< StochasticNode<Tree> *>( cdbdp );
    
}


/**
 * Copy constructor.
 */
template<class characterType>
JointConditionalCharacterMappingMonitor<characterType>::JointConditionalCharacterMappingMonitor( const JointConditionalCharacterMappingMonitor &m) : Monitor( m ),
outStream(),
filename( m.filename ),
separator( m.separator ),
append( m.append ),
tree( m.tree ),
cdbdp( m.cdbdp )
{
    
    if (m.outStream.is_open() == true )
    {
        openStream( true );
    }
    
}


/**
 * Destructor.
 */
template<class characterType>
JointConditionalCharacterMappingMonitor<characterType>::~JointConditionalCharacterMappingMonitor()
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
template<class characterType>
JointConditionalCharacterMappingMonitor<characterType>* JointConditionalCharacterMappingMonitor<characterType>::clone(void) const
{
    
    return new JointConditionalCharacterMappingMonitor<characterType>(*this);
}



/**
 * Close the stream. This means that we are finished with monitoring and we close the filestream.
 */
template<class characterType>
void JointConditionalCharacterMappingMonitor<characterType>::closeStream()
{
    
    outStream.close();
    
}


/**
 * Monitor value at given generation.
 *
 * \param[in]   gen    The current generation.
 */
template<class characterType>
void JointConditionalCharacterMappingMonitor<characterType>::monitor(unsigned long gen)
{
    if (gen % printgen == 0)
    {
        // print the iteration number first
        outStream << gen;
        
        size_t num_sites;
        size_t num_nodes;
        
        
        // get the distribution for the character
        StateDependentSpeciationExtinctionProcess       *dist_bd    = NULL;
        //dist_bd = dynamic_cast<CharacterDependentCladoBirthDeathProcess*>( &cdbdp->getDistribution() ); // this doesn't work?
        //dist_bd = dynamic_cast<CharacterDependentCladoBirthDeathProcess*>( &nodes[0]->getDistribution() ); // this does!
        dist_bd = dynamic_cast<StateDependentSpeciationExtinctionProcess*>( &nodes[0]->getDistribution() ); // this does!
        num_sites = 1;
        num_nodes = tree->getValue().getNumberOfNodes();
        
        std::vector<std::vector<characterType> > startStates(num_nodes, std::vector<characterType>(num_sites));
        std::vector<std::vector<characterType> > endStates(num_nodes, std::vector<characterType>(num_sites));
        
        // draw ancestral states
        std::vector<size_t> startStatesIndexes(num_nodes);
        std::vector<size_t> endStatesIndexes(num_nodes);
        dist_bd->drawJointConditionalAncestralStates(startStatesIndexes, endStatesIndexes);
        
        // Let us check first the type of the data and the one we excpect.
        characterType tmp = characterType();
        if ( dist_bd->getCharacterData().getTaxonData(0)[0].getDataType() != tmp.getDataType() )
        {
            throw RbException("The character type in the ancestral state monitor does not match. \" The data has type " + dist_bd->getCharacterData().getTaxonData(0)[0].getDataType() + "\" but the monitor expected \"" + tmp.getDataType() + "\".");
        }
        
        // now give as an object that we can clone.
        // this is necessary because otherwise we would not know the state labels or size of the character
        characterType *tmp_char = dynamic_cast< characterType* >( dist_bd->getCharacterData().getTaxonData(0)[0].clone() );
        
        for (size_t i = 0; i < startStatesIndexes.size(); i++)
        {
            startStates[i][0] = characterType( *tmp_char );
            startStates[i][0].setStateByIndex(startStatesIndexes[i]);
            startStates[i][0].setMissingState(false);
            endStates[i][0]   = characterType( *tmp_char );
            endStates[i][0].setStateByIndex(endStatesIndexes[i]);
            endStates[i][0].setMissingState(false);
        }
        
        delete tmp_char;
        
        
        // print ancestral states
//        const std::vector<TopologyNode*>& nds = tree->getValue().getNodes();
//        for (int i = 0; i < nds.size(); i++)
//        {
//            size_t node_index = nds[i]->getIndex();
//
//            // start states
//            if (withStartStates)
//            {
//                // add a separator before every new element
//                outStream << separator;
//                
//                // print out ancestral states....
//                for (int j = 0; j < startStates[node_index].size(); j++)
//                {
//                    if (j != 0)
//                        outStream << ",";
//                    outStream << startStates[node_index][j].getStringValue();
//                }
//            }
//            
//            // end states
//            if ( withTips || !nds[i]->isTip() )
//            {
//                outStream << separator;
//                for (int j = 0; j < endStates[node_index].size(); j++)
//                {
//                    if (j != 0)
//                        outStream << ",";
//                    outStream << endStates[node_index][j].getStringValue();
//                }
//            }
//        }
        outStream << std::endl;
    }
}


/**
 * Open the CharacterMapping stream for printing.
 */
template<class characterType>
void JointConditionalCharacterMappingMonitor<characterType>::openStream( bool reopen )
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
template<class characterType>
void JointConditionalCharacterMappingMonitor<characterType>::printHeader()
{
    // print one column for the iteration number
    outStream << "Iteration";
    
    std::vector<TopologyNode*> nodes = tree->getValue().getNodes();

    // iterate through all tree nodes and make header with node index
//    for (int i = 0; i < tree->getValue().getNumberOfNodes(); i++)
//    {
//        TopologyNode* nd = nodes[i];
//        size_t node_index = nd->getIndex();
//        if (withStartStates)
//        {
//            outStream << separator;
//            outStream << "start_" << node_index+1;
//        }
//        
//        // end states
//        if ( withTips || !nd->isTip() )
//        {
//            outStream << separator;
//            outStream << "end_" << node_index+1;
//        }
//    }
    outStream << std::endl;
}



/**
 * Set flag about whether to append to an existing file.
 *
 * \param[in]   tf   Flag if to append.
 */
template<class characterType>
void JointConditionalCharacterMappingMonitor<characterType>::setAppend(bool tf)
{
    
    append = tf;
    
}


template<class characterType>
void JointConditionalCharacterMappingMonitor<characterType>::swapNode(DagNode *oldN, DagNode* newN)
{
    
    if ( oldN == cdbdp )
    {
        cdbdp = static_cast< StochasticNode<Tree> *>( newN );
        tree = static_cast< StochasticNode<Tree> *>( newN );
    }
    
    Monitor::swapNode( oldN, newN );
    
}


#endif
