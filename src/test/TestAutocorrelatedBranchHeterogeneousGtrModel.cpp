#include "AutocorrelatedBranchMatrixDistribution.h"
#include "BetaDistribution.h"
#include "Clade.h"
#include "ConstantNode.h"
#include "ConstantRateBirthDeathProcess.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "FileMonitor.h"
#include "FixedNodeheightPruneRegraft.h"
#include "GammaDistribution.h"
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
#include "RandomNumberGenerator.h"
#include "RbFileManager.h"
#include "RootTimeSlide.h"
#include "ScaleProposal.h"
#include "ScreenMonitor.h"
#include "SimplexMove.h"
#include "SlidingMove.h"
#include "SubtreeScale.h"
#include "TestAutocorrelatedBranchHeterogeneousGtrModel.h"
#include "TimeTree.h"
#include "TreeHeightStatistic.h"
#include "TreeScale.h"
#include "UniformDistribution.h"

using namespace RevBayesCore;

TestAutocorrelatedBranchHeterogeneousGtrModel::TestAutocorrelatedBranchHeterogeneousGtrModel(const std::string &afn, const std::string &tFn, int gen) : alignmentFilename( afn ), treeFilename( tFn ), mcmcGenerations( gen ){
    
}

TestAutocorrelatedBranchHeterogeneousGtrModel::~TestAutocorrelatedBranchHeterogeneousGtrModel() {
    // nothing to do
}



bool TestAutocorrelatedBranchHeterogeneousGtrModel::run( void ) {
    
    // fix the rng seed
    std::vector<unsigned int> seed;
    seed.push_back(25);
    seed.push_back(42);
    GLOBAL_RNG->setSeed(seed);
    
    /* First, we read in the data */
    // the matrix
    std::vector<AbstractCharacterData*> data = NclReader().readMatrices(alignmentFilename);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    std::cout << data[0] << std::endl;
    
    std::vector<TimeTree*> trees = NclReader().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    
    /* set up the model graph */
    
    //////////////////////
    // first the priors //
    //////////////////////
    
    // birth-death process priors
    StochasticNode<double> *div = new StochasticNode<double>("diversification", new UniformDistribution(new ConstantNode<double>("div_lower", new double(0.0)), new ConstantNode<double>("div_upper", new double(100.0)) ));
    ConstantNode<double> *turn = new ConstantNode<double>("turnover", new double(0.0));
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0));
    // gtr model priors
    ConstantNode<std::vector<double> > *bf = new ConstantNode<std::vector<double> >( "bf", new std::vector<double>(4,1.0) );
    ConstantNode<std::vector<double> > *e = new ConstantNode<std::vector<double> >( "e", new std::vector<double>(6,1.0) );
    
    //Root frequencies
    StochasticNode<std::vector<double> > *rf = new StochasticNode<std::vector<double> >( "rf", new DirichletDistribution(bf) );
    
    
    StochasticNode<std::vector<double> > * er = new StochasticNode<std::vector<double> >( "er", new DirichletDistribution(e) ) ;
    
    std::cout << "bf:\t" << bf->getValue() << std::endl;
    std::cout << "e:\t" << e->getValue() << std::endl;
    
    
    std::vector<std::string> names = data[0]->getTaxonNames();
    ConstantNode<double>* origin = new ConstantNode<double>( "origin", new double( trees[0]->getRoot().getAge()*2.0 ) );
    std::vector<RevBayesCore::Taxon> taxa;
    for (size_t i = 0; i < names.size(); ++i)
    {
        taxa.push_back( Taxon( names[i] ) );
    }
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, NULL, div, turn, rho, "uniform", "survival", taxa, std::vector<Clade>()) );
    
    tau->setValue( trees[0] );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
    
    std::vector<StochasticNode < std::vector<double> >* > pis;
    //  std::vector<StochasticNode < std::vector<double> >* > ers;
    std::vector< const TypedDagNode < RateMatrix>* > qs;
    
    ConstantNode<double> *alpha_prior_shape = new ConstantNode< double >("alpha_prior_shape", new double( 5.0 ) );
    ConstantNode<double> *alpha_prior_rate = new ConstantNode< double >("alpha_prior_rtae", new double( 0.5 ) );
    StochasticNode<double> *alpha = new StochasticNode<double>("alpha", new GammaDistribution( alpha_prior_shape, alpha_prior_rate ) );
    
    ConstantNode<double> *beta_prior_shape1 = new ConstantNode< double >("beta_prior_shape1", new double( 2.0 ) );
    ConstantNode<double> *beta_prior_shape2 = new ConstantNode< double >("beta_prior_shape2", new double( 5.0 ) );
    StochasticNode<double> *beta = new StochasticNode<double>("beta", new BetaDistribution( beta_prior_shape1, beta_prior_shape2 ) );
    
    StochasticNode< RbVector<RateMatrix> > *perBranchQ = new StochasticNode< RbVector< RateMatrix > >( "autocorrBranchRate", new AutocorrelatedBranchMatrixDistribution( tau, beta, rf, er, alpha ) );

    //    StochasticNode< std::vector<RateMatrix> > *perBranchQ = new StochasticNode< std::vector< RateMatrix > >( "autocorrBranchRate", new DPP< RateMatrix >( tau, ... ) );
    
//    
//    for (unsigned int i = 0 ; i < numBranches ; i++ ) {
//        std::ostringstream pi_name;
//        pi_name << "pi(" << i << ")";
//        pis.push_back(new StochasticNode<std::vector<double> >( pi_name.str(), new DirichletDistribution(bf) ) );
//        //  ers.push_back(new StochasticNode<std::vector<double> >( "er", new DirichletDistribution(e) ) );
//        std::ostringstream q_name;
//        q_name << "q(" << i << ")";
//        qs.push_back(new DeterministicNode<RateMatrix>( q_name.str(), new GtrRateMatrixFunction(er, pis[i]) ));
//        std::cout << "Q:\t" << qs[i]->getValue() << std::endl;
//    }
    
    
    // and the character model
    GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *phyloCTMC = new GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tau, 4, true, data[0]->getNumberOfCharacters());
    phyloCTMC->setRootFrequencies( rf );
    phyloCTMC->setRateMatrix( perBranchQ );
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("S", phyloCTMC );
    charactermodel->clamp( data[0] );
    
    
    /* add the moves */
    RbVector<Move> moves;
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(div, 1.0), 2, true ) );
    moves.push_back( new NearestNeighborInterchange( tau, 5.0 ) );
    moves.push_back( new NarrowExchange( tau, 10.0 ) );
    moves.push_back( new FixedNodeheightPruneRegraft( tau, 2.0 ) );
    moves.push_back( new SubtreeScale( tau, 5.0 ) );
//    moves.push_back( new TreeScale( tau, 1.0, true, 2.0 ) );
    moves.push_back( new NodeTimeSlideUniform( tau, 30.0 ) );
    moves.push_back( new RootTimeSlide( tau, 1.0, true, 2.0 ) );
    moves.push_back( new SimplexMove( er, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( er, 100.0, 6, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( rf, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( rf, 100.0, 4, 0, true, 2.0 ) );
    
//    for (unsigned int i = 0 ; i < numBranches ; i ++ ) {
//        //     moves.push_back( new SimplexMove( ers[i], 10.0, 1, true, 2.0 ) );
//        moves.push_back( new SimplexMove( pis[i], 10.0, 1, true, 2.0 ) );
//        //    moves.push_back( new SimplexMove( ers[i], 100.0, 6, true, 2.0 ) );
//        moves.push_back( new SimplexMove( pis[i], 100.0, 4, true, 2.0 ) );
//    }
    // add some tree stats to monitor
    DeterministicNode<double> *treeHeight = new DeterministicNode<double>("TreeHeight", new TreeHeightStatistic(tau) );
    
    /* add the monitors */
    RbVector<Monitor> monitors;
    std::set<DagNode*> monitoredNodes;
    //    monitoredNodes.insert( er );
    //    monitoredNodes.insert( pi );
    monitoredNodes.insert( div );
    monitors.push_back( new FileMonitor( monitoredNodes, 10, "TestAutocorrelatedBranchHeterogeneousGtrModel.log", "\t" ) );
    std::set<DagNode*> monitoredNodes1;
    monitoredNodes1.insert( er );
    /*    for (unsigned int i = 0 ; i < numBranches ; i ++ ) {
     monitoredNodes1.insert( pis[i] );
     }*/
    monitoredNodes1.insert( rf );
    monitoredNodes1.insert( treeHeight );
    monitors.push_back( new FileMonitor( monitoredNodes1, 10, "TestAutocorrelatedBranchHeterogeneousGtrModelSubstRates.log", "\t" ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes1, 10, "\t" ) );
    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( tau );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, "TestAutocorrelatedBranchHeterogeneousGtrModel.tree", "\t", false, false, false ) );
    
    /* instantiate the model */
    Model myModel = Model( tau );
    
    std::vector<DagNode*> &nodes = myModel.getDagNodes();
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
        std::cerr << (*it)->getName() << std::endl;
    }
    
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
    
    std::cout << "Finished Autocorrelated Branch Heterogeneous GTR model test." << std::endl;
    
    return true;
}
