#include "BranchLengthTree.h"
#include "ConstantNode.h"
#include "MultispeciesCoalescent.h"
#include "NclReader.h"
#include "NewickConverter.h"
#include "RandomNumberGenerator.h"
#include "RandomNumberFactory.h"
#include "RbFileManager.h"
#include "RbSettings.h"
#include "StochasticNode.h"
#include "TestMultispeciesCoalescentCombinatorics.h"
#include "TimeTree.h"
#include "Topology.h"
#include "TreeUtilities.h"

#include <sstream>
#include <string>
#include <vector>

using namespace RevBayesCore;

TestMultispeciesCoalescentCombinatorics::TestMultispeciesCoalescentCombinatorics() {
    
}

TestMultispeciesCoalescentCombinatorics::~TestMultispeciesCoalescentCombinatorics() {
    // nothing to do
}


bool TestMultispeciesCoalescentCombinatorics::run( void ) {
    
    // let us surpress the index of the nodes in the print outs
    RbSettings::userSettings().setPrintNodeIndex( false );
    
    // fix the rng seed
    std::vector<unsigned int> seed;
    seed.push_back(25);
    seed.push_back(42);
    GLOBAL_RNG->setSeed(seed);
    
//    double trueNE = 1000.0;
    size_t individualsPerSpecies = 2;
    size_t nGeneTrees = 100000;
    
    /* First, we read in the data */
    NewickConverter nc;
    BranchLengthTree *blTree = nc.convertFromNewick("(A:693.1472,B:693.1472);");
    TimeTree *speciesTree = TreeUtilities::convertTree( *blTree );
    
    std::cout << "Species Tree:\n" << *speciesTree << "\n\n";
    
    
    /* testing the likelihood implementation */
    // first the priors
  //  size_t nNodes = speciesTree->getNumberOfNodes();
//    ConstantNode< std::vector<double> > *Ne = new ConstantNode< std::vector<double> >("N", new std::vector<double>(nNodes, trueNE) );
    std::vector<std::string> speciesNames = speciesTree->getTipNames();
    std::vector<Taxon> taxa;
    for (std::vector<std::string>::iterator s = speciesNames.begin(); s != speciesNames.end(); ++s) {
        for (size_t i = 1; i <= individualsPerSpecies; ++i)
        {
            std::stringstream o;
            o << *s << "_" << i;
            Taxon t = Taxon( o.str() );
            t.setSpeciesName( *s );
            taxa.push_back( t );
        }
        
    }
    
    ConstantNode<TimeTree> *spTree = new ConstantNode<TimeTree>("speciesTree", speciesTree);
    StochasticNode<TimeTree> *tauCPC = new StochasticNode<TimeTree>( "tau", new MultispeciesCoalescent( spTree, taxa) );
    //    std::cout << "tau:\t" << tauCBD->getValue() << std::endl;
    std::vector<const TimeTree*> simTrees;
    for (size_t i = 0; i < nGeneTrees; ++i) 
    {
        simTrees.push_back( &tauCPC->getValue() );
        tauCPC->redraw();
    }
    
    std::cerr << *simTrees[0] << "\n\n" << std::endl;
    
    // here are all possible topologies
    std::vector<Topology> topologies;
    topologies.push_back( nc.convertFromNewick("((A_1,A_2),(B_1,B_2));")->getTopology() );
    topologies.push_back( nc.convertFromNewick("(((A_1,A_2),B_1),B_2);")->getTopology() );
    topologies.push_back( nc.convertFromNewick("(((A_1,A_2),B_2),B_1);")->getTopology() );
    topologies.push_back( nc.convertFromNewick("(((B_1,B_2),A_1),A_2);")->getTopology() );
    topologies.push_back( nc.convertFromNewick("(((B_1,B_2),A_2),A_1);")->getTopology() );
    topologies.push_back( nc.convertFromNewick("((A_1,B_2),(B_1,A_2));")->getTopology() );
    topologies.push_back( nc.convertFromNewick("((A_1,B_1),(A_2,B_2));")->getTopology() );
    topologies.push_back( nc.convertFromNewick("(((A_1,B_1),A_2),B_2);")->getTopology() );
    topologies.push_back( nc.convertFromNewick("(((A_1,B_1),B_2),A_2);")->getTopology() );
    topologies.push_back( nc.convertFromNewick("(((A_1,B_2),A_2),B_1);")->getTopology() );
    topologies.push_back( nc.convertFromNewick("(((A_1,B_2),B_1),A_2);")->getTopology() );
    topologies.push_back( nc.convertFromNewick("(((A_2,B_1),A_1),B_2);")->getTopology() );
    topologies.push_back( nc.convertFromNewick("(((A_2,B_1),B_2),A_1);")->getTopology() );
    topologies.push_back( nc.convertFromNewick("(((A_2,B_2),A_1),B_1);")->getTopology() );
    topologies.push_back( nc.convertFromNewick("(((A_2,B_2),B_1),A_1);")->getTopology() );
    
    std::vector<int> observedTopologyFrequencies(topologies.size(),0);
    
    // now let's count how often we saw each topology
    for ( size_t i = 0; i < simTrees.size() ; i++ ) 
    {
        const Tree *t = simTrees[i];
        for ( size_t j = 0; j < topologies.size(); ++j) 
        {
            if ( t->hasSameTopology( topologies[j] ) == true ) 
            {
                observedTopologyFrequencies[j]++;
                break;
            }
        }
    }
    
    
    // print out the result
    for ( size_t j = 0; j < topologies.size(); ++j) 
    {
        std::cout << topologies[j].getPlainNewickRepresentation() << "\t\t" << observedTopologyFrequencies[j]/double(nGeneTrees) << std::endl;
    }
    
    
    return true;
}
