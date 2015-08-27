#include "BetaDistribution.h"
#include "BetaSimplexMove.h"
#include "Clade.h"
#include "ConstantNode.h"
#include "ConstantRateBirthDeathProcess.h"
#include "ContinuousStochasticNode.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "FixedNodeheightPruneRegraft.h"
#include "GeneralBranchHeterogeneousCharEvoModel.h"
#include "GtrRateMatrixFunction.h"
#include "LognormalDistribution.h"
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
#include "NucleotideFrequenciesFromGcContentFunction.h"
#include "RandomNumberGenerator.h"
#include "RbFileManager.h"
#include "RootTimeSlide.h"
#include "ScaleProposal.h"
#include "ScreenMonitor.h"
#include "SimplexMove.h"
#include "SlidingMove.h"
#include "SubtreeScale.h"
#include "TestUCLNRelaxedClockBHT92Model.h"
#include "TimeTree.h"
#include "TreeHeightStatistic.h"
#include "TreeScale.h"
#include "UniformDistribution.h"
#include "Tamura92RateMatrixFunction.h"
#include "VectorFunction.h"
#include "RbVectorFunction.h"

using namespace RevBayesCore;

TestUCLNRelaxedClockBHT92Model::TestUCLNRelaxedClockBHT92Model(const std::string &afn, const std::string &tFn, int gen) : alignmentFilename( afn ), treeFilename( tFn ), mcmcGenerations( gen ){
    
}

TestUCLNRelaxedClockBHT92Model::~TestUCLNRelaxedClockBHT92Model() {
    // nothing to do
}



bool TestUCLNRelaxedClockBHT92Model::run( void ) {
    
    std::vector<unsigned int> seeds;
    seeds.push_back(7);
    seeds.push_back(4);
    GLOBAL_RNG->setSeed( seeds );
    
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
    
	
    // Setting up the substitution model //
	
    //ts/tv ratio:
    ConstantNode<double > *tstv_prior = new ConstantNode<double >( "tstv_prior", new double(0.25) );
    ContinuousStochasticNode *tstv = new ContinuousStochasticNode("tstv", new ExponentialDistribution(tstv_prior) );
	
    //GC content prior:
    ConstantNode<double > *eq_gc_prior = new ConstantNode<double >( "eq_gc_prior_ab", new double(1.0) );    
	
    //Root GC frequency
    
    StochasticNode< double  > *omega = new StochasticNode< double >( "omega", new BetaDistribution(eq_gc_prior,eq_gc_prior) );
    DeterministicNode<std::vector<double> > *rf = new DeterministicNode< std::vector<double> >( "rf", new NucleotideFrequenciesFromGcContentFunction( omega ) );
	
    std::cout << "omega:\t" << omega->getValue() << std::endl;
    std::cout << "rf:\t" << rf->getValue() << std::endl;
    std::cout << "tstv:\t" << tstv->getValue() << std::endl;
    
    //Declaring a vector of matrices, one per branch
    size_t numBranches = 2*data[0]->getNumberOfTaxa() - 2;
    std::vector<ContinuousStochasticNode*> thetas;
    std::vector< const TypedDagNode < RateMatrix >* > qs;
	
	//Equilibrium GC frequency: one per branch, defined in the loop along with the T92 rate matrices.
    for (unsigned int i = 0 ; i < numBranches ; i++ ) {
        std::ostringstream eq_gc_name;
        eq_gc_name << "eq_gc(" << i << ")";
        thetas.push_back(new ContinuousStochasticNode( eq_gc_name.str(), new BetaDistribution(eq_gc_prior,eq_gc_prior) ) );
		std::ostringstream q_name;
        q_name << "q(" << i << ")";
		qs.push_back(new DeterministicNode< RateMatrix >( q_name.str(), new Tamura92RateMatrixFunction( thetas[i], tstv) ));
        //std::cout << "Matrix Q:\t"<<i<<"\t" << qs[i]->getValue() << std::endl;
    }
    
	//Build a node out of the vector of nodes
    DeterministicNode< RbVector< RateMatrix > >* qs_node = new DeterministicNode< RbVector< RateMatrix > >( "q_vector", new RbVectorFunction<RateMatrix>(qs) );
    
	
	// Setting up the relaxed clock model //

    ConstantNode<double> *a = new ConstantNode<double>("a", new double(0.5) );
    ConstantNode<double> *b = new ConstantNode<double>("b", new double(0.25) );

	
	std::vector<const TypedDagNode<double> *> branchRates;
	std::vector< ContinuousStochasticNode *> branchRates_nonConst;
	for( size_t i=0; i<numBranches; i++){
        std::ostringstream br_name;
        br_name << "br(" << i << ")";
		ContinuousStochasticNode* tmp_branch_rate = new ContinuousStochasticNode( br_name.str(), new LognormalDistribution(a, b, new ConstantNode<double>("offset", new double(0.0) )));
		branchRates.push_back( tmp_branch_rate );
		branchRates_nonConst.push_back( tmp_branch_rate );
	}
	//Build a node out of the vector of nodes
    DeterministicNode< std::vector< double > >* br_vector = new DeterministicNode< std::vector< double > >( "br_vector", new VectorFunction< double >( branchRates ) );

	
	
	// Putting it all together //

	
    std::vector<std::string> names = data[0]->getTaxonNames();
    ConstantNode<double>* origin = new ConstantNode<double>( "origin", new double( trees[0]->getRoot().getAge()*2.0 ) );
    std::vector<RevBayesCore::Taxon> taxa;
    for (size_t i = 0; i < names.size(); ++i)
    {
        taxa.push_back( Taxon( names[i] ) );
    }
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, NULL, div, turn, rho, "uniform", "survival", taxa, std::vector<Clade>()) );
    
	//If we want to get a good starting tree
	//    tau->setValue( trees[0] );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
    
    // and the character model
	//    StochasticNode<CharacterData<DnaState> > *charactermodel = new StochasticNode<CharacterData <DnaState> >("S", new SimpleGTRBranchRateTimeCharEvoModel<DnaState, TimeTree>(tau, q, br_vector, true, data[0]->getNumberOfCharacters()) );
    
    GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *phyloCTMC = new GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tau, 4, true, data[0]->getNumberOfCharacters());
    phyloCTMC->setRootFrequencies( rf );
    phyloCTMC->setRateMatrix( qs_node );
    phyloCTMC->setClockRate( br_vector );
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
    moves.push_back( new BetaSimplexMove( omega, 10.0, true, 2.0 ) );
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(tstv, 1.0), 2, true ) );
	
    for (unsigned int i = 0 ; i < numBranches ; i ++ ) {
        moves.push_back( new BetaSimplexMove( dynamic_cast<StochasticNode<double>* >(thetas[i]), 10.0, true, 2.0 ) );
        moves.push_back( new SlidingMove( thetas[i], 0.05, true, 2.0) );
        //    moves.push_back( new SimplexMove( ers[i], 100.0, 6, true, 2.0 ) );
		//        moves.push_back( new SimplexMove( pis[i], 100.0, 4, true, 2.0 ) );
    }
    
    // add some tree stats to monitor
    DeterministicNode<double> *treeHeight = new DeterministicNode<double>("TreeHeight", new TreeHeightStatistic(tau) );
    
    /* add the monitors */
    RbVector<Monitor> monitors;
    std::set<DagNode*> monitoredNodes;
    //    monitoredNodes.insert( er );
    //    monitoredNodes.insert( pi );
    monitoredNodes.insert( div );
    monitors.push_back( new FileMonitor( monitoredNodes, 10, "TestUCLNRelaxedClockBHT92Model.log", "\t" ) );
    std::set<DagNode*> monitoredNodes1;
	//    monitoredNodes1.insert( er );
    for (unsigned int i = 0 ; i < numBranches ; i ++ ) {
        monitoredNodes1.insert( thetas[i] );
    }
    monitoredNodes1.insert( rf );
    monitoredNodes1.insert( treeHeight );
    monitors.push_back( new FileMonitor( monitoredNodes1, 10, "TestUCLNRelaxedClockBHT92ModelSubstRates.log", "\t" ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes1, 10, "\t" ) );
    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( tau );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, "TestUCLNRelaxedClockBHT92Model.tree", "\t", false, false, false ) );
    
    /* instantiate the model */
    Model myModel = Model(qs[0]);
    
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
