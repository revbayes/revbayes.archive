#include "BetaDistribution.h"
#include "Clade.h"
#include "ConstantNode.h"
#include "ConstantRateBirthDeathProcess.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "FileMonitor.h"
#include "FixedNodeheightPruneRegraft.h"
#include "GeneralBranchHeterogeneousCharEvoModel.h"
#include "GtrRateMatrixFunction.h"
#include "Mcmc.h"
#include "MetropolisHastingsMove.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NarrowExchange.h"
#include "NclReader.h"
#include "NearestNeighborInterchange.h"
#include "NodeTimeSlideBeta.h"
#include "NodeTimeSlideUniform.h"
#include "PomoRootFrequenciesFunction.h"
#include "PomoRateMatrixFunction.h"
#include "RateMatrix_Pomo.h"
#include "RbFileManager.h"
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
#include "UniformDistribution.h"

using namespace RevBayesCore;

TestPomoModel::TestPomoModel(const std::string &afn, const std::string &tFn, int gen) : alignmentFilename( afn ), treeFilename( tFn ), mcmcGenerations( gen ){
    
}

TestPomoModel::~TestPomoModel() {
    // nothing to do
}



bool TestPomoModel::run( void ) {
    
    /* First, we read in the data */
    // the matrix
    std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(alignmentFilename);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    std::cout << data[0] << std::endl;
    
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    
    /* set up the model graph */
    
    //Size of the virtual population
    ConstantNode<int> *popSize = new ConstantNode<int>("populationSize", new int(10));
    
    //////////////////////
    // first the priors //
    //////////////////////
    
    // birth-death process priors
    
    StochasticNode<double> *div = new StochasticNode<double>("diversification", new UniformDistribution(new ConstantNode<double>("", new double(0.0)), new ConstantNode<double>("", new double(100.0)) ));
    ConstantNode<double> *turn = new ConstantNode<double>("turnover", new double(0.0));
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0));
    
    
    // POMO model priors
    
    // the parameters for the mutation rates
    ConstantNode<std::vector<double> > *e = new ConstantNode<std::vector<double> >( "e", new std::vector<double>(12,1.0) ); //All 12 possible mutations
    StochasticNode<std::vector<double> > *mr = new StochasticNode<std::vector<double> >( "mutationRates", new DirichletDistribution(e) );
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

    DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new PomoRateMatrixFunction(popSize, mr, selco) );
    
    std::cout << "Q:\t" << q->getValue() << std::endl;
    
    /////I stopped here. From there on, need to adapt the code to the Pomo model.
    
    
    std::vector<std::string> names = data[0]->getTaxonNames();
    ConstantNode<double>* origin = new ConstantNode<double>( "origin", new double( trees[0]->getRoot().getAge()*2.0 ) );
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, div, turn, rho, "uniform", "survival", int(names.size()), names, std::vector<Clade>()) );
    
    //    tau->setValue( trees[0] );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
    
    ConstantNode<double> *clockRate = new ConstantNode<double>("clockRate", new double(1.0) );
    
    // and the character model
    //    StochasticNode<CharacterData<DnaState> > *charactermodel = new StochasticNode<CharacterData <DnaState> >("S", new CharacterEvolutionAlongTree<DnaState, TimeTree>(tau, q, data[0]->getNumberOfCharacters()) );
    //    StochasticNode<CharacterData<DnaState> > *charactermodel = new StochasticNode<CharacterData <DnaState> >("S", new SimpleCharEvoModel<DnaState, TimeTree>(tau, q, data[0]->getNumberOfCharacters()) );
    GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *phyloCTMC = new GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tau, 4, true, data[0]->getNumberOfCharacters());
    phyloCTMC->setClockRate( clockRate );
    phyloCTMC->setRateMatrix( q );
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("S", phyloCTMC );
    charactermodel->clamp( data[0] );
    
    /* add the moves */
    RbVector<Move> moves;
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(div, 1.0), 2, true ) );
    moves.push_back( new NearestNeighborInterchange( tau, 5.0 ) );
    moves.push_back( new NarrowExchange( tau, 10.0 ) );
    moves.push_back( new FixedNodeheightPruneRegraft( tau, 2.0 ) );
    moves.push_back( new SubtreeScale( tau, 5.0 ) );
    moves.push_back( new TreeScale( tau, 1.0, true, 2.0 ) );
    moves.push_back( new NodeTimeSlideUniform( tau, 30.0 ) );
    moves.push_back( new RootTimeSlide( tau, 1.0, true, 2.0 ) );
 /*   moves.push_back( new SimplexMove( er, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( pi, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( er, 100.0, 6, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( pi, 100.0, 4, 0, true, 2.0 ) );
    */
    // add some tree stats to monitor
    DeterministicNode<double> *treeHeight = new DeterministicNode<double>("TreeHeight", new TreeHeightStatistic(tau) );
    
    /* add the monitors */
    RbVector<Monitor> monitors;
    std::set<DagNode*> monitoredNodes;
    //    monitoredNodes.insert( er );
    //    monitoredNodes.insert( pi );
    monitoredNodes.insert( div );
    monitors.push_back( new FileMonitor( monitoredNodes, 10, "TestPomoModel.log", "\t" ) );
    std::set<DagNode*> monitoredNodes1;
  /*  monitoredNodes1.insert( er );
    monitoredNodes1.insert( pi );*/
    monitoredNodes1.insert( q );
    monitoredNodes1.insert( treeHeight );
    monitors.push_back( new FileMonitor( monitoredNodes1, 10, "TestPomoModelSubstRates.log", "\t" ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes1, 10, "\t" ) );
    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( tau );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, "TestPomoModel.tree", "\t", false, false, false ) );
    
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
