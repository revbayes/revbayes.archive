#ifndef JointConditionalJointConditionalAncestralStateMonitor_H
#define JointConditionalJointConditionalAncestralStateMonitor_H

#include "AbstractHomologousDiscreteCharacterData.h"
#include "CharacterDependentCladoBirthDeathProcess.h"
#include "StateDependentSpeciationExtinctionProcess.h"
#include "Monitor.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "StochasticNode.h"
#include "DnaState.h"

#include <fstream>
#include <string>
#include <vector>
#include <typeinfo>

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
    template<class characterType>
	class JointConditionalAncestralStateMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
		JointConditionalAncestralStateMonitor(TypedDagNode<Tree> *t, StochasticNode<AbstractHomologousDiscreteCharacterData>* ch, unsigned long g, const std::string &fname, const std::string &del, bool wt, bool wss);                                  //!< Constructor
		JointConditionalAncestralStateMonitor(StochasticNode<Tree>* ch, unsigned long g, const std::string &fname, const std::string &del, bool wt, bool wss);
        JointConditionalAncestralStateMonitor(const JointConditionalAncestralStateMonitor &m);
        virtual ~JointConditionalAncestralStateMonitor(void);
        
        JointConditionalAncestralStateMonitor*          clone(void) const;                                                  //!< Clone the object
        
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
        std::fstream                        outStream;
        
        // parameters
        std::string                         filename;                                                           //!< Filename to which we print the values
        std::string                         separator;                                                          //!< Seperator between monitored values (between columns)
		bool                                append;                                                             //!< Flag if to append to existing file
		TypedDagNode<Tree>*                 tree;
		StochasticNode<AbstractHomologousDiscreteCharacterData>*            ctmc;
        StochasticNode<Tree>*                                               cdbdp;
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
#include "CharacterDependentCladoBirthDeathProcess.h"
#include "StateDependentSpeciationExtinctionProcess.h"

using namespace RevBayesCore;


/* Constructor for state dependent birth death process */
template<class characterType>
JointConditionalAncestralStateMonitor<characterType>::JointConditionalAncestralStateMonitor(StochasticNode<Tree>* ch, unsigned long g, const std::string &fname, const std::string &del, bool wt, bool wss) : Monitor(g),
    outStream(),
    filename( fname ),
    separator( del ),
    append( false ),
    cdbdp( ch ),
    stochasticNodesOnly( false ),
    withTips( wt ),
    withStartStates( wss )
{
    ctmc = NULL;
    
    // the cdbdp is both the tree and character evolution model
    addVariable( cdbdp );
    tree = cdbdp;
}


/* Constructor for CTMC */
template<class characterType>
JointConditionalAncestralStateMonitor<characterType>::JointConditionalAncestralStateMonitor(TypedDagNode<Tree> *t, StochasticNode<AbstractHomologousDiscreteCharacterData>* ch, unsigned long g, const std::string &fname, const std::string &del, bool wt, bool wss) : Monitor(g),
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
    cdbdp = NULL;
    
    addVariable( tree );
    addVariable( ctmc );
    
}



/**
 * Copy constructor.
 */
template<class characterType>
JointConditionalAncestralStateMonitor<characterType>::JointConditionalAncestralStateMonitor( const JointConditionalAncestralStateMonitor &m) : Monitor( m ),
    outStream(),
    filename( m.filename ),
    separator( m.separator ),
    append( m.append ),
    tree( m.tree ),
    ctmc( m.ctmc ),
    cdbdp( m.cdbdp ),
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
template<class characterType>
JointConditionalAncestralStateMonitor<characterType>::~JointConditionalAncestralStateMonitor()
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
JointConditionalAncestralStateMonitor<characterType>* JointConditionalAncestralStateMonitor<characterType>::clone(void) const
{
    
    return new JointConditionalAncestralStateMonitor<characterType>(*this);
}



/**
 * Close the stream. This means that we are finished with monitoring and we close the filestream.
 */
template<class characterType>
void JointConditionalAncestralStateMonitor<characterType>::closeStream()
{
	
    outStream.close();
	
}


/**
 * Monitor value at given generation.
 *
 * \param[in]   gen    The current generation.
 */
template<class characterType>
void JointConditionalAncestralStateMonitor<characterType>::monitor(unsigned long gen)
{
    if (gen % printgen == 0)
    {
        // print the iteration number first
        outStream << gen;        
        
        size_t num_sites;
        size_t num_nodes;
        
        
        // get the distribution for the character
        AbstractPhyloCTMCSiteHomogeneous<characterType> *dist_ctmc  = NULL;
        StateDependentSpeciationExtinctionProcess       *dist_bd    = NULL;
        if ( ctmc != NULL )
        {
            dist_ctmc = static_cast<AbstractPhyloCTMCSiteHomogeneous<characterType>* >( &ctmc->getDistribution() );
            num_sites = dist_ctmc->getValue().getNumberOfCharacters();
            num_nodes = tree->getValue().getNumberOfNodes();
        }
        else
        {
            //dist_bd = dynamic_cast<CharacterDependentCladoBirthDeathProcess*>( &cdbdp->getDistribution() ); // this doesn't work?
            //dist_bd = dynamic_cast<CharacterDependentCladoBirthDeathProcess*>( &nodes[0]->getDistribution() ); // this does!
            dist_bd = dynamic_cast<StateDependentSpeciationExtinctionProcess*>( &nodes[0]->getDistribution() ); // this does!
            num_sites = 1;
            num_nodes = tree->getValue().getNumberOfNodes();
        }
        
        std::vector<std::vector<characterType> > startStates(num_nodes,std::vector<characterType>(num_sites));
        std::vector<std::vector<characterType> > endStates(num_nodes,std::vector<characterType>(num_sites));
        
        // draw ancestral states
        if ( ctmc != NULL )
        {
            dist_ctmc->drawJointConditionalAncestralStates(startStates, endStates);
        }
        else
        {
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
            
        }
        
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
                for (int j = 0; j < startStates[node_index].size(); j++)
                {
                    if (j != 0)
                        outStream << ",";
                    outStream << startStates[node_index][j].getStringValue();
                }
            }
            
            // end states
            if ( withTips || !nds[i]->isTip() )
            {
                outStream << separator;
                for (int j = 0; j < endStates[node_index].size(); j++)
                {
                    if (j != 0)
                        outStream << ",";
                    outStream << endStates[node_index][j].getStringValue();
                }
            }
        }
        outStream << std::endl;
    }
}


/**
 * Open the AncestralState stream for printing.
 */
template<class characterType>
void JointConditionalAncestralStateMonitor<characterType>::openStream( bool reopen )
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
template<class characterType>
void JointConditionalAncestralStateMonitor<characterType>::printHeader()
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
template<class characterType>
void JointConditionalAncestralStateMonitor<characterType>::setAppend(bool tf)
{
    
    append = tf;
    
}


template<class characterType>
void JointConditionalAncestralStateMonitor<characterType>::swapNode(DagNode *oldN, DagNode* newN)
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

#endif /* defined(__revbayes_proj__JointConditionalJointConditionalAncestralStateMonitor__) */
