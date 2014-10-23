#include "BetaDistribution.h"
#include "BetaSimplexMove.h"
#include "Clade.h"
#include "ConstantNode.h"
#include "ConstantRateBirthDeathProcess.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "FastaWriter.h"
#include "FileMonitor.h"
#include "FixedNodeheightPruneRegraft.h"
#include "PhyloCTMCSiteHomogeneous.h"
#include "PhyloCTMCSiteHomogeneousNucleotide.h"
#include "GtrRateMatrixFunction.h"
#include "Mcmc.h"
#include "MetropolisHastingsMove.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "MultispeciesCoalescent.h"
#include "NarrowExchange.h"
#include "NclReader.h"
#include "NearestNeighborInterchange.h"
#include "NodeTimeSlideBeta.h"
#include "NodeTimeSlideUniform.h"
#include "PomoStateConverter.h"
#include "PomoRootFrequenciesFunction.h"
#include "PomoRateMatrixFunction.h"
#include "RateMatrix_Pomo.h"
#include "RbFileManager.h"
#include "SymmetricDifferenceStatistic.h"
#include "RootTimeSlide.h"
#include "ScaleProposal.h"
#include "ScreenMonitor.h"
#include "SimplexMove.h"
#include "SlidingMove.h"
#include "SubtreeScale.h"
#include "TestPomoModel.h"
#include "TimeTree.h"
#include "TreeHeightStatistic.h"
#include "TreeScale.h"
#include "TreeUtilities.h"
#include "UniformDistribution.h"

using namespace RevBayesCore;

TestPomoModel::TestPomoModel(const std::string &treeFn, const unsigned int virtPopSize, int gen) : treeFilename( treeFn ), virtualPopulationSize(virtPopSize), mcmcGenerations( gen ){

}

TestPomoModel::~TestPomoModel() {
    // nothing to do
}



bool TestPomoModel::run( void ) {
    
    // fix the rng seed
    std::vector<unsigned int> seed;
    seed.push_back(25);
    seed.push_back(42);
    GLOBAL_RNG->setSeed(seed);
    
    double trueNE = 0.004; //as in Leaché et al., 2011
    double trueTreeHeight = 0.012; //as in Leaché et al., 2011
    size_t nGeneTrees = 10;
    size_t individualsPerSpecies = 10;
    
    //    #if defined (USE_LIB_OPENMP)
    //        omp_set_num_threads(numProcesses);
    //    #endif
    //    numProcesses = 4;
    //    if (nGeneTrees < numProcesses)
    //        numProcesses = nGeneTrees;
    //
    //    genesPerProcess.resize(numProcesses);
    //    for (size_t i = 0, j = 0; i < nGeneTrees; i++, j++)
    //    {
    //        if (j >= numProcesses)
    //            j = 0;
    //        genesPerProcess[j].push_back(i);
    //    }
    //
    
    
    //Folder for output of the files
    std::string folder = "/Users/boussau/sharedFolderLinux/revBayes/revbayes-code-git/examples/data/";
    
    
    
    /* First, we read in the species tree */
   // treeFilename = "data/primates.tree";
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    TimeTree *t = trees[0];
    std::cout << "True species tree:\n"<<trees[0]->getNewickRepresentation() << std::endl;
    //We rescale the subtree so that the tree height is 0.004, as in Leaché et al., 2011
    double treeHeight = t->getAge( t->getRoot().getIndex() );
    
    TreeUtilities::rescaleSubtree(t, &(t->getRoot()), trueTreeHeight/treeHeight );
    std::cout << "Rescaled species tree:\n"<<trees[0]->getNewickRepresentation() << std::endl;

    /* Then we set up the multispecies coalescent process and simulate gene trees */
    size_t nNodes = t->getNumberOfNodes();
    ConstantNode< std::vector<double> > *Ne = new ConstantNode< std::vector<double> >("N", new std::vector<double>(nNodes, trueNE) );
    std::vector<std::string> speciesNames = t->getTipNames();
    std::vector<Taxon> taxa;
    std::map<std::string, std::string> sequenceNameToSpeciesName;
    for (std::vector<std::string>::iterator s = speciesNames.begin(); s != speciesNames.end(); ++s) {
        for (size_t i = 1; i <= individualsPerSpecies; ++i)
        {
            std::stringstream o;
            o << *s << "_" << i;
            Taxon t = Taxon( o.str() );
            t.setSpeciesName( *s );
            taxa.push_back( t );
            sequenceNameToSpeciesName[o.str()] = *s;
        }
        
    }
    
    ConstantNode<TimeTree> *spTree = new ConstantNode<TimeTree>("speciesTree", t);
    MultispeciesCoalescent *m = new MultispeciesCoalescent( spTree, taxa);
    m->setNes(Ne);
    StochasticNode<TimeTree> *tauCPC = new StochasticNode<TimeTree>( "tau", m);
    
    //Simulating gene trees
    std::vector<const TimeTree*> simTrees;
    for (size_t i = 0; i < nGeneTrees; ++i) {
        simTrees.push_back(tauCPC->getValue().clone());
        // write the simulated tree
        stringstream ss; //create a stringstream
        ss << i;
        std::ofstream myfile;
        myfile.open( (folder + "primatesSimulatedTree_" + ss.str() + ".dnd").c_str() );
        myfile << simTrees[i]->getNewickRepresentation() << std::endl;
        myfile.close();
        tauCPC->redraw();
    }
    std::cout << "\t\tGene trees simulated."<<std::endl;
    
    
    
    //Now we have the gene trees.
    //We want to simulate sequences along these.
    size_t nStates= 4;
    RateMatrix_GTR gtr(nStates);
    std::vector<double> simPi;
    simPi.push_back(0.1);
    simPi.push_back(0.2);
    simPi.push_back(0.3);
    simPi.push_back(0.4);
    gtr.setStationaryFrequencies( simPi );
    
    // update rate matrix
    gtr.updateMatrix();
    ConstantNode<RateMatrix> *simQ = new ConstantNode<RateMatrix>( "Q", new RateMatrix_GTR(gtr) );
    
    ConstantNode<double> *simClockRate = new ConstantNode<double>("clockRate", new double(1.0) );
    std::vector<StochasticNode< AbstractCharacterData > *> simSeqs;
    for (size_t i = 0; i < nGeneTrees; ++i) {
        std::stringstream o;
        o << "SimulatedSequences_" << i;
        // and the character model
        NucleotideBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *phyloCTMC = new NucleotideBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(new ConstantNode<TimeTree> ("tau", new TimeTree(*(simTrees[i]))), true, 100);
        phyloCTMC->setClockRate(simClockRate);
        phyloCTMC->setRateMatrix(simQ);
        simSeqs.push_back(new StochasticNode< AbstractCharacterData >(o.str(), phyloCTMC));
        
        // write the simulated sequence
        FastaWriter writer;
        stringstream ss; //create a stringstream
        ss << i;
        writer.writeData(folder + o.str() + ".fas", simSeqs[i]->getValue());
    }
    
    //NOW THE DATA HAS BEEN SIMULATED
    std::cout << "\t\tSequence data simulated."<<std::endl;
    
    /* First, we read in the data */
    // the matrix
  /*  std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(alignmentFilename);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    std::cout << data[0] << std::endl;
    
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;*/

    
    //Now the data needs to be converted into PomoStates
    std::vector< AbstractCharacterData*> simSeqsPol;
    AbstractCharacterData* concatenatedSimSeqsPol = NULL;
    PomoStateConverter* psc = new PomoStateConverter();
    std::cout << "Number of sequence data sets: "<< simSeqs.size() <<std::endl;
    std::cout << "Each data set has length "<< simSeqs[0]->getValue().getNumberOfCharacters() <<std::endl;
    for (size_t i = 0; i<simSeqs.size(); ++i) {
        simSeqsPol.push_back(psc->convertData(simSeqs[i]->getValue(), virtualPopulationSize, sequenceNameToSpeciesName));
        if (concatenatedSimSeqsPol== NULL ) {
            concatenatedSimSeqsPol = simSeqsPol[i];
        }
        else {
        concatenatedSimSeqsPol->add(*simSeqsPol[i]); //Concatenation
        }
    }
    std::cout << "\t\tSequence data converted into Pomo format."<<std::endl;
    std::cout << "\t\tTotal number of sites: "<<concatenatedSimSeqsPol->getNumberOfCharacters() << std::endl;
    /* set up the model graph */
    
    //////////////////////
    // first the priors //
    //////////////////////
    
    // birth-death process priors
    
    StochasticNode<double> *div = new StochasticNode<double>("diversification", new UniformDistribution(new ConstantNode<double>("", new double(0.0)), new ConstantNode<double>("", new double(100.0)) ));
    ConstantNode<double> *turn = new ConstantNode<double>("turnover", new double(0.0));
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0));
    
    //////////////////////
    // POMO model priors
    
    //Size of the virtual population
    ConstantNode<int> *popSize = new ConstantNode<int>("populationSize", new int(virtualPopulationSize));
    
    // the parameters for the mutation rates
    ConstantNode<std::vector<double> > *e = new ConstantNode<std::vector<double> >( "e", new std::vector<double>(12,1.0) ); //All 12 possible mutations
    //StochasticNode<std::vector<double> > *mr = new StochasticNode<std::vector<double> >( "mutationRates", new DirichletDistribution(e) );
    std::vector<double> mu ;
    mu.push_back(0.0001);
    mu.push_back(0.0001);
    mu.push_back(0.0001);
    mu.push_back(0.0001);
    mu.push_back(0.0001);
    mu.push_back(0.0001);
    mu.push_back(0.0001);
    mu.push_back(0.0001);
    mu.push_back(0.0001);
    mu.push_back(0.0001);
    mu.push_back(0.0001);
    mu.push_back(0.0001);
    
    ConstantNode<std::vector<double> > *mr = new ConstantNode<std::vector<double> >( "mutationRates", &mu );
    
    std::cout << "Mutation rates:\t" << mr->getValue() << std::endl;

    //For the root frequencies, the proportion of polymorphic sites at the root, and the mutation rates
    ConstantNode<std::vector<double> > *bf = new ConstantNode<std::vector<double> >( "", new std::vector<double>(4,1.0) );
    ConstantNode<double>* pb = new ConstantNode<double>( "priorForBeta", new double( 0.5 ) );
    // then the parameters for the root frequencies
    StochasticNode<std::vector<double> > *fnrf = new StochasticNode<std::vector<double> >( "fixedNucleotideRootFrequencies", new DirichletDistribution(bf) );
    StochasticNode< double > *fopar = new StochasticNode< double >( "FractionOfPolymorphismsAtRoot", new BetaDistribution(pb, pb) );
    std::cout << "Root frequencies:\t" << fnrf->getValue() << std::endl;
    std::cout << "Fraction of polymorphic sites at root:\t" << fopar->getValue() << std::endl;
    //The deterministic node for root frequencies
    DeterministicNode<std::vector<double> > *rf = new DeterministicNode<std::vector<double> >( "rf", new PomoRootFrequenciesFunction(fnrf, fopar, mr, popSize) );

    //Other parameters for the Pomo model: selection coefficients for ACGT
    StochasticNode<std::vector<double> > *selco = new StochasticNode<std::vector<double> >( "selectionCoefficients", new DirichletDistribution(bf) );
    std::vector<double> temp ;
    temp.push_back(1);
    temp.push_back(1);
    temp.push_back(1);
    temp.push_back(1);

    //Temporary
    ConstantNode<std::vector<double> > *selcotemp = new ConstantNode<std::vector<double> >( "selcotemp", &temp );

    DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new PomoRateMatrixFunction(popSize, mr, selcotemp) );
    
    std::cout << "Q matrix:\t" << q->getValue() << std::endl;
    
    
    
    //////////////////////
    // Topology prior
    std::vector<std::string> names = concatenatedSimSeqsPol->getTaxonNames();
    ConstantNode<double>* origin = new ConstantNode<double>( "origin", new double( trees[0]->getRoot().getAge()*2.0 ) );
    std::vector<RevBayesCore::Taxon> taxaNames;
    for (size_t i = 0; i < names.size(); ++i)
    {
        taxaNames.push_back( Taxon( names[i] ) );
    }
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, NULL, div, turn, rho, "uniform", "survival", taxaNames, std::vector<Clade>()) );
    
    //    tau->setValue( trees[0] );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
    
    //////////////////////
    // Clock prior
    ConstantNode<double> *clockRate = new ConstantNode<double>("clockRate", new double(1.0) );
    
    
    //////////////////////////
    // Setting up the model //
    //////////////////////////
    for (size_t i = 0; i< concatenatedSimSeqsPol->getNumberOfTaxa(); ++i) {
        std::cout <<concatenatedSimSeqsPol->getTaxonNameWithIndex(i) << " : "<< concatenatedSimSeqsPol->getTaxonData(i).getCharacter(0) << " : "<< concatenatedSimSeqsPol->getTaxonData(i).getCharacter(0).getNumberOfStates() <<std::endl;
    }
        
    GeneralBranchHeterogeneousCharEvoModel<PomoState, TimeTree> *phyloCTMC = new GeneralBranchHeterogeneousCharEvoModel<PomoState, TimeTree>(tau, 58, true, concatenatedSimSeqsPol->getNumberOfCharacters());
    phyloCTMC->setClockRate( clockRate );
    phyloCTMC->setRootFrequencies( rf );
    phyloCTMC->setRateMatrix( q );
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("CharacterModel", phyloCTMC );
    charactermodel->clamp( concatenatedSimSeqsPol );
    
    /* add the moves */
    RbVector<Move> moves;
    //Move on the age
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(div, 1.0), 2, true ) );
    //Moves on the topology
    moves.push_back( new NearestNeighborInterchange( tau, 5.0 ) );
    moves.push_back( new NarrowExchange( tau, 10.0 ) );
    moves.push_back( new FixedNodeheightPruneRegraft( tau, 2.0 ) );
    moves.push_back( new SubtreeScale( tau, 5.0 ) );
//    moves.push_back( new TreeScale( tau, 1.0, true, 2.0 ) );
    moves.push_back( new NodeTimeSlideUniform( tau, 30.0 ) );
    moves.push_back( new RootTimeSlide( tau, 1.0, true, 2.0 ) );
    //Moves on the model parameters
 //   moves.push_back( new SimplexMove( mr, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( fnrf, 10.0, 1, 0, true, 2.0 ) );
  //  moves.push_back( new SimplexMove( mr, 100.0, 12, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( fnrf, 100.0, 4, 0, true, 2.0 ) );
    moves.push_back( new BetaSimplexMove( fopar, 10.0, 1, 0 ) ); //Parameters chosen arbitrarily...
    moves.push_back( new SimplexMove( selco, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( selco, 100.0, 4, 0, true, 2.0 ) );

    // add some tree stats to monitor
    DeterministicNode<double> *treeHeightStat = new DeterministicNode<double>("TreeHeight", new TreeHeightStatistic(tau) );
    // RF distance between reconstructed and true tree
    ConstantNode<TimeTree> *trueTree = new ConstantNode<TimeTree>( "trueTree", t );
    DeterministicNode<double> *spTreeRF = new DeterministicNode<double>("spTreeRF", new SymmetricDifferenceStatistic(trueTree, tau) );
    /* add the monitors */
    RbVector<Monitor> monitors;
    std::set<DagNode*> monitoredNodes;
    //    monitoredNodes.insert( er );
    //    monitoredNodes.insert( pi );
    monitoredNodes.insert( div );
    monitors.push_back( new FileMonitor( monitoredNodes, 10, folder+"TestPomoModel.log", "\t" ) );
    std::set<DagNode*> monitoredNodes1;
  /*  monitoredNodes1.insert( er );
    monitoredNodes1.insert( pi );*/
  //  monitoredNodes1.insert( q ); too large to monitor!
    monitoredNodes1.insert( treeHeightStat );
    /*  monitoredNodes1.insert( er );
     monitoredNodes1.insert( pi );*/
    //  monitoredNodes1.insert( q ); too large to monitor!
//    monitoredNodes1.insert( spTreeRF );

    monitors.push_back( new FileMonitor( monitoredNodes1, 10, folder+"TestPomoModelSubstRates.log", "\t" ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes1, 10, "\t" ) );
    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( tau );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, folder+"TestPomoModel.tree", "\t", false, false, false ) );
    
    /* instantiate the model */
    Model myModel = Model(q);
    
    /* instiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(mcmcGenerations);
    
    myMcmc.printOperatorSummary();
    
    /* clean up */
    //    for (size_t i = 0; i < 10; ++i) {
    //        delete x[i];
    //    }
    //    delete [] x;
    delete div;
    //    delete sigma;
    //    delete a;
    //    delete b;
    //    delete c;
    
    std::cout << "Finished GTR model test." << std::endl;
    
    return true;
}
