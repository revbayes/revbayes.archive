//
//  StochasticCharacterMappingMonitor.h
//  RevBayes_development_branch
//
//  Created by will freyman on 12/14/16.
//  Copyright (c) 2016 will freyman. All rights reserved.
//

#ifndef RevBayes_development_branch_StochasticCharacterMappingMonitor_h
#define RevBayes_development_branch_StochasticCharacterMappingMonitor_h

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
     * @brief Declaration and implementation of the StochasticCharacterMappingMonitor class.
     *
     * @file
     * Declaration and implementation of the StochasticCharacterMappingMonitor class which
     * monitors samples of character histories drawn from the state-dependent birth death process
     * and PhyloCTMC and prints their value into a file.
     *
     */
    template<class characterType>
    class StochasticCharacterMappingMonitor : public Monitor {
        
    public:
        
        // Constructors and Destructors
        StochasticCharacterMappingMonitor(StochasticNode<Tree>* ch, unsigned long g, const std::string &fname, bool is, const std::string &del);
//        StochasticCharacterMappingMonitor(TypedDagNode<Tree> *t, StochasticNode<AbstractHomologousDiscreteCharacterData>* ch, unsigned long g, const std::string &fname, bool is, const std::string &del);
        StochasticCharacterMappingMonitor(StochasticNode<AbstractHomologousDiscreteCharacterData>* ch, unsigned long g, const std::string &fname, bool is, const std::string &del);
        StochasticCharacterMappingMonitor(const StochasticCharacterMappingMonitor &m);
        virtual ~StochasticCharacterMappingMonitor(void);
        
        StochasticCharacterMappingMonitor*        clone(void) const;                                                  //!< Clone the object
        
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
        bool                                            include_simmaps;                                                    //!< Should we print out SIMMAP/phytools compatible character histories?
        std::string                                     filename;                                                           //!< Filename to which we print the values
        std::string                                     separator;                                                          //!< Seperator between monitored values (between columns)
        bool                                            append;                                                             //!< Flag if to append to existing file
        TypedDagNode<Tree>*                             tree;
        StochasticNode<Tree>*                           cdbdp;                                                              //!< The character dependent birth death process we are monitoring
        StochasticNode<AbstractHomologousDiscreteCharacterData>*            ctmc;
    };
    
}


#include "AbstractPhyloCTMCSiteHomogeneous.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "RbFileManager.h"
#include "StochasticNode.h"
#include "StateDependentSpeciationExtinctionProcess.h"

using namespace RevBayesCore;


/* Constructor for state dependent birth death process */
template<class characterType>
StochasticCharacterMappingMonitor<characterType>::StochasticCharacterMappingMonitor(StochasticNode<Tree>* ch, unsigned long g, const std::string &fname, bool is, const std::string &del) : Monitor(g),
outStream(),
filename( fname ),
separator( del ),
append( false ),
cdbdp( ch ),
include_simmaps( is )
{
    ctmc = NULL;
    
    // the cdbdp is both the tree and character evolution model
    addVariable( cdbdp );
    tree = static_cast< StochasticNode<Tree> *>( cdbdp );
    
}

//StochasticCharacterMappingMonitor<characterType>::StochasticCharacterMappingMonitor(TypedDagNode<Tree> *t, StochasticNode<AbstractHomologousDiscreteCharacterData>* ch, unsigned long g, const std::string &fname, bool is, const std::string &del) : Monitor(g),
/* Constructor for CTMC */
template<class characterType>
StochasticCharacterMappingMonitor<characterType>::StochasticCharacterMappingMonitor(StochasticNode<AbstractHomologousDiscreteCharacterData>* ch, unsigned long g, const std::string &fname, bool is, const std::string &del) : Monitor(g),
outStream(),
filename( fname ),
separator( del ),
append( false ),
ctmc( ch ),
include_simmaps( is )
{
    cdbdp = NULL;
    
    AbstractPhyloCTMCSiteHomogeneous<characterType> *ctmc_dist = NULL;
    ctmc_dist = static_cast<AbstractPhyloCTMCSiteHomogeneous<characterType>* >( &ctmc->getDistribution() );
    tree = const_cast<TypedDagNode<Tree>* >( ctmc_dist->getTree() );
    
    addVariable( tree );
    addVariable( ctmc );
}


/**
 * Copy constructor.
 */
template<class characterType>
StochasticCharacterMappingMonitor<characterType>::StochasticCharacterMappingMonitor( const StochasticCharacterMappingMonitor &m) : Monitor( m ),
outStream(),
filename( m.filename ),
separator( m.separator ),
append( m.append ),
cdbdp( m.cdbdp ),
ctmc( m.ctmc ),
tree( m.tree ),
include_simmaps( m.include_simmaps )
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
StochasticCharacterMappingMonitor<characterType>::~StochasticCharacterMappingMonitor()
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
StochasticCharacterMappingMonitor<characterType>* StochasticCharacterMappingMonitor<characterType>::clone(void) const
{
    
    return new StochasticCharacterMappingMonitor(*this);
}



/**
 * Close the stream. This means that we are finished with monitoring and we close the filestream.
 */
template<class characterType>
void StochasticCharacterMappingMonitor<characterType>::closeStream()
{
    
    outStream.close();
    
}


/**
 * Monitor value at given generation.
 *
 * \param[in]   gen    The current generation.
 */
template<class characterType>
void StochasticCharacterMappingMonitor<characterType>::monitor(unsigned long gen)
{
    
    if (gen % printgen == 0)
    {
        // print the iteration number first
        outStream << gen;
        
        size_t num_nodes;
        
        
        // get the distribution for the character
        StateDependentSpeciationExtinctionProcess *sse_process = NULL;
        AbstractPhyloCTMCSiteHomogeneous<characterType> *ctmc_dist = NULL;
        if ( ctmc != NULL )
        {
            ctmc_dist = static_cast<AbstractPhyloCTMCSiteHomogeneous<characterType>* >( &ctmc->getDistribution() );
            num_nodes = tree->getValue().getNumberOfNodes();
        }
        else
        {
            sse_process = dynamic_cast<StateDependentSpeciationExtinctionProcess*>( &nodes[0]->getDistribution() );
            num_nodes = tree->getValue().getNumberOfNodes();
        }
        
        std::vector<std::string*> character_histories( num_nodes );
        
        // draw stochastic character map
        if ( ctmc != NULL )
        {
            ctmc_dist->drawStochasticCharacterMap( character_histories, 0 );
        }
        else
        {
            sse_process->drawStochasticCharacterMap( character_histories );
        }
        
        // print to monitor file
        const std::vector<TopologyNode*>& nds = tree->getValue().getNodes();
        for (int i = 0; i < nds.size(); i++)
        {
            
            size_t node_index = nds[i]->getIndex();
            
            // add a separator before every new element
            outStream << separator;
            
            // print out this branch's character history in the format
            // used by SIMMAP and phytools
            outStream << *character_histories[ node_index ];
            
        }
        
        if ( include_simmaps == true )
        {
            // print out the SIMMAP/phytools compatible newick string as the last column of the log file
            outStream << separator;
            tree->getValue().clearNodeParameters();
            tree->getValue().addNodeParameter( "character_history", character_histories, false );
            outStream << tree->getValue().getSimmapNewickRepresentation();
        }
        
        outStream << std::endl;
    
    }
}


/**
 * Open the CharacterMapping stream for printing.
 */
template<class characterType>
void StochasticCharacterMappingMonitor<characterType>::openStream( bool reopen )
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
void StochasticCharacterMappingMonitor<characterType>::printHeader()
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

    if ( include_simmaps == true )
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
template<class characterType>
void StochasticCharacterMappingMonitor<characterType>::setAppend(bool tf)
{
    
    append = tf;
    
}


template<class characterType>
void StochasticCharacterMappingMonitor<characterType>::swapNode(DagNode *oldN, DagNode* newN)
{
    
    if ( oldN == tree )
    {
        tree = static_cast< TypedDagNode<Tree> *>( newN );
    }
    else if ( oldN == ctmc )
    {
        ctmc = static_cast< StochasticNode<AbstractHomologousDiscreteCharacterData> *>( newN );
    }
    else if ( oldN == cdbdp )
    {
        cdbdp = static_cast< StochasticNode<Tree> *>( newN );
        tree = static_cast< StochasticNode<Tree> *>( newN );
    }
    
    Monitor::swapNode( oldN, newN );
    
}

#endif
