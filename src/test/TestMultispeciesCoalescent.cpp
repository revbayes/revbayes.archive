#include "BinaryDivision.h"
#include "Clade.h"
#include "ConstantRateBirthDeathProcess.h"
#include "DeterministicNode.h"
#include "ConstantNode.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "FixedNodeheightPruneRegraft.h"
#include "GammaDistribution.h"
#include "Mcmc.h"
#include "MetropolisHastingsMove.h"
#include "Model.h"
#include "ModelMonitor.h"
#include "MultispeciesCoalescent.h"
#include "NarrowExchange.h"
#include "NclReader.h"
#include "NearestNeighborInterchange.h"
#include "NodeTimeSlideUniform.h"
#include "OneOverXDistribution.h"
#include "RandomNumberGenerator.h"
#include "RandomNumberFactory.h"
#include "RbFileManager.h"
#include "RootTimeSlide.h"
#include "ScaleProposal.h"
#include "ScreenMonitor.h"
#include "StochasticNode.h"
#include "SubtreeScale.h"
#include "TestMultispeciesCoalescent.h"
#include "TimeTree.h"
#include "TreeScale.h"
#include "TreeSummary.h"
#include "TreeTrace.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"

#include <sstream>

using namespace RevBayesCore;

TestMultispeciesCoalescent::TestMultispeciesCoalescent(const std::string &tfn) : treeFilename( tfn ) {
    
}

TestMultispeciesCoalescent::~TestMultispeciesCoalescent() {
    // nothing to do
}





TreeTrace<TimeTree> TestMultispeciesCoalescent::readTreeTrace(const std::string &fname) {
//    RevLanguage::Func_readTreeTrace reader;
//    std::vector<RevLanguage::Argument> args;
//    RevLanguage::RevPtr<RevLanguage::Variable> var = new RevLanguage::Variable(new RevLanguage::RlString(fname) );
//    args.push_back( RevLanguage::Argument(var,"filename") );
//    RevLanguage::RevPtr<RevLanguage::Variable> var2 = new RevLanguage::Variable(new RevLanguage::RlString("clock") );
//    args.push_back( RevLanguage::Argument(var2,"treetype") );
//    reader.processArguments(args);
//    
//    RevLanguage::RevObject* trace = reader.execute();
//    
//    TreeTrace<TimeTree> rv = static_cast<RevLanguage::TreeTrace<RevLanguage::TimeTree>* >( trace )->getValue();
//    
//    delete trace;
//    

    // This is temporary to allow compilation...
    TreeTrace<TimeTree> rv;
    return rv;
}


bool TestMultispeciesCoalescent::run( void ) {
    
    // fix the rng seed
    std::vector<unsigned int> seed;
    seed.push_back(25);
    seed.push_back(42);
    GLOBAL_RNG->setSeed(seed);
    
    double trueNE = 0.001;
    size_t nGeneTrees = 50;
    size_t individualsPerSpecies = 10;
    
    /* First, we read in the data */
    std::vector<TimeTree*> trees = NclReader().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    TimeTree *t = trees[0];
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    
    /* testing the likelihood implementation */
    // first the priors
    size_t nNodes = t->getNumberOfNodes();
    ConstantNode< std::vector<double> > *Ne = new ConstantNode< std::vector<double> >("N", new std::vector<double>(nNodes, trueNE) );
    std::vector<std::string> speciesNames = t->getTipNames();
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
    
    ConstantNode<TimeTree> *spTree = new ConstantNode<TimeTree>("speciesTree", t);
    MultispeciesCoalescent *m = new MultispeciesCoalescent( spTree, taxa);
    m->setNes(Ne);
    StochasticNode<TimeTree> *tauCPC = new StochasticNode<TimeTree>( "tau", m);

    //    std::cout << "tau:\t" << tauCBD->getValue() << std::endl;
    std::vector<const TimeTree*> simTrees;
    for (size_t i = 0; i < nGeneTrees; ++i) 
    {
        simTrees.push_back( &tauCPC->getValue() );
        tauCPC->redraw();
	//	std::cerr << *simTrees[i]  << std::endl;

    }
    
//    std::cerr << tauCPC->getValue() << std::endl;
//    std::cout << tauCPC->getValue().getRoot().getAge() << std::endl;
    std::cerr << *simTrees[0] << "\n\n\n" << std::endl;
    

    RbVector<Move> moves;

    // construct the parameters for the gamma prior of the population sizes
    // shape of the gamma (we fix it to 2.0)
    ConstantNode<double> *shape = new ConstantNode<double>("shape", new double(2.0));
    // rate of the gamma distribution. we use rate = 1.0/scale and a hyperprior on the P(scale) = 1.0 / scale, which is the infamous OneOverX prior distribution
    ConstantNode<double> *min = new ConstantNode<double>("min", new double(1E6));
    ConstantNode<double> *max = new ConstantNode<double>("max", new double(1E-10));
    StochasticNode<double> *scale = new StochasticNode<double>("scale", new OneOverXDistribution(min,max));
    ConstantNode<double> *one = new ConstantNode<double>("one", new double(1.0) );
    DeterministicNode<double> *rate = new DeterministicNode<double>("rate", new BinaryDivision<double, double, double>(one,scale));
    
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(scale, 1.0), 2, true ) );

    std::vector<const TypedDagNode<double> *> ne_nodes;
    for (size_t i = 0; i < nNodes; ++i) 
    {
        std::stringstream o;
        o << "Ne_" << i;
        StochasticNode<double> *NePerNode = new StochasticNode<double>(o.str(), new GammaDistribution(shape,rate) );
        ne_nodes.push_back( NePerNode );
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(NePerNode, 1.0), 2, true ) );
    }
    DeterministicNode< std::vector<double> > *Ne_inf = new DeterministicNode< std::vector<double > >("Ne", new VectorFunction<double>( ne_nodes ) );
//    ConstantNode< std::vector<double> > *Ne_inf = new ConstantNode< std::vector<double> >("N", new std::vector<double>(nNodes, trueNE) );

    
    ConstantNode<double> *speciationRate = new ConstantNode<double>("speciationRate", new double(10.0) );
    ConstantNode<double> *extinctionRate = new ConstantNode<double>("extinctionRate", new double(2.5) );
    ConstantNode<double> *sampling = new ConstantNode<double>("rho", new double(1.0) );
    ConstantNode<double>* origin = new ConstantNode<double>( "origin", new double( t->getRoot().getAge()*2.0 ) );
    std::vector<std::string> names = t->getTipNames();
    std::vector<RevBayesCore::Taxon> taxaNames;
    for (size_t i = 0; i < names.size(); ++i)
    {
        taxaNames.push_back( Taxon( names[i] ) );
    }
    StochasticNode<TimeTree> *spTree_inf = new StochasticNode<TimeTree>( "S", new ConstantRateBirthDeathProcess( origin, NULL, speciationRate, extinctionRate, sampling, "uniform", "survival", taxaNames, std::vector<Clade>()) );
	
    // If we want to initialize the species tree to the true tree
    TimeTree *startingTree = spTree_inf->getValue().clone();
    TreeUtilities::rescaleTree(startingTree, &startingTree->getRoot(), 0.01);
    spTree_inf->setValue( startingTree );
    
    std::cerr << spTree_inf->getValue() << std::endl;

	
    //If we want to explore the species tree topology, use these 3 moves:
    moves.push_back( new NearestNeighborInterchange( spTree_inf, 10.0 ) );
    moves.push_back( new NarrowExchange( spTree_inf, 10.0 ) );
    moves.push_back( new FixedNodeheightPruneRegraft( spTree_inf, 30.0 ) );

	//Moves for node heights only
    moves.push_back( new SubtreeScale( spTree_inf, 10.0 ) );
//    moves.push_back( new TreeScale( spTree_inf, 1.0, true, 10.0 ) );
    moves.push_back( new NodeTimeSlideUniform( spTree_inf, 10.0 ) );
    moves.push_back( new RootTimeSlide( spTree_inf, 1.0, false, 5.0 ) );
    
    
    
    std::vector< StochasticNode<TimeTree> *> geneTrees_inf;
    for (size_t i = 0; i < nGeneTrees; ++i) {
        std::stringstream o;
        o << "G_" << i;
        MultispeciesCoalescent* m = new MultispeciesCoalescent( spTree_inf, taxa) ;
        m->setNes(Ne_inf);
        geneTrees_inf.push_back( new StochasticNode<TimeTree>( o.str(), m ) );
        geneTrees_inf[i]->clamp( const_cast<TimeTree*>(simTrees[i]) );
    }
    
    Model myModel = Model(spTree_inf);
    
    RbVector<Monitor> monitors;
    monitors.push_back( new ModelMonitor( 10, "TestMultispeciesCoalescent.p", "\t" ) );
    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( spTree_inf );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, "TestMultispeciesCoalescent.trees", "\t", false, false, false ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes2, 10, false, false, false ) );

    
    /* instantiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
//    myMcmc.run(10000);
    myMcmc.run(10000);

    
    // read in the tree trace
    TreeTrace<TimeTree> trace = readTreeTrace("TestMultispeciesCoalescent.trees");
    
    TreeSummary<TimeTree> summary = TreeSummary<TimeTree>(trace);
    summary.summarize();
    summary.printTreeSummary(std::cerr);
    
    return true;
}
