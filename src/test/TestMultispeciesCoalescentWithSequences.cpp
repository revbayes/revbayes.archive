#include "BinaryDivision.h"
#include "Clade.h"
#include "ConstantNode.h"
#include "ConstantRateBirthDeathProcess.h"
#include "ContinuousStochasticNode.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "ExponentialDistribution.h"
#include "FastaWriter.h"
#include "FileMonitor.h"
#include "FixedNodeheightPruneRegraft.h"
#include "GammaDistribution.h"
#include "NucleotideBranchHeterogeneousCharEvoModel.h"
#include "GtrRateMatrixFunction.h"
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
#include "SimplexSingleElementScale.h"
#include "StochasticNode.h"
#include "StringUtilities.h"
#include "SubtreeScale.h"
#include "TestMultispeciesCoalescentWithSequences.h"
#include "TimeTree.h"
#include "TreeScale.h"
#include "TreeSummary.h"
#include "TreeTrace.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"

#include <sstream>

//#if defined (USE_LIB_OPENMP)
//    #include <omp.h>
//#endif



using namespace RevBayesCore;

TestMultispeciesCoalescentWithSequences::TestMultispeciesCoalescentWithSequences(const std::string &tfn) : treeFilename( tfn ) {
    
}

TestMultispeciesCoalescentWithSequences::~TestMultispeciesCoalescentWithSequences() {
    // nothing to do
}





TreeTrace<TimeTree> TestMultispeciesCoalescentWithSequences::readTreeTrace(const std::string &fname) {
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


bool TestMultispeciesCoalescentWithSequences::run( void ) {
    
    // fix the rng seed
    std::vector<unsigned int> seed;
    seed.push_back(25);
    seed.push_back(42);
    GLOBAL_RNG->setSeed(seed);
    
    double trueNE = 100;
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
    std::vector<TimeTree*> trees = NclReader().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    TimeTree *t = trees[0];
    std::cout << "True species tree:\n"<<trees[0]->getNewickRepresentation() << std::endl;
   /* TreeUtilities::rescaleSubtree(t, &(t->getRoot()), 100 );
    std::cout << "Rescaled species tree:\n"<<trees[0]->getNewickRepresentation() << std::endl;*/

    
    /* Then we set up the multispecies coalescent process and simulate gene trees */
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

    //Simulating gene trees
    std::vector<const TimeTree*> simTrees;
    for (size_t i = 0; i < nGeneTrees; ++i) {
        simTrees.push_back(tauCPC->getValue().clone());
        // write the simulated tree
        stringstream ss; //create a stringstream
        ss << i;
        std::ofstream myfile;
        myfile.open( (folder+"primatesSimulatedTree_" + ss.str() + ".dnd").c_str() );
        myfile << simTrees[i]->getNewickRepresentation() << std::endl;
        myfile.close();
        tauCPC->redraw();
        //	std::cerr << *simTrees[i]  << std::endl;
    }
        std::cout << "\t\tGene trees simulated."<<std::endl;

    
//    std::cerr << tauCPC->getValue() << std::endl;
//    std::cout << tauCPC->getValue().getRoot().getAge() << std::endl;
 //   std::cerr << *simTrees[0] << "\n\n\n" << std::endl;
    
    //Now we have the gene trees.
    //We want to simulate sequences along these.
    size_t nStates= 4;
   // ConstantNode<RateMatrix> *q = new ConstantNode<RateMatrix>( "Q", new RateMatrix_JC(4) );
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
        //StochasticNode< AbstractCharacterData > *charactermodel = 
        simSeqs.push_back(new StochasticNode< AbstractCharacterData >(o.str(), phyloCTMC));

        // write the simulated sequence
        FastaWriter writer;
        stringstream ss; //create a stringstream
        ss << i;
        //  writer.writeData( "primatesSimulated_" + ss.str() + ".fas", charactermodel->getValue() );
        writer.writeData(folder+o.str() + ".fas", simSeqs[i]->getValue());
    }
    
    //NOW THE DATA HAS BEEN SIMULATED
    std::cout << "\t\tSequence data simulated."<<std::endl;
    //We set up the model for inference
    
    // construct the parameters for the gamma prior of the population sizes
    // shape of the gamma (we fix it to 2.0)
    ConstantNode<double> *shape = new ConstantNode<double>("shapeGamma", new double(2.0));
    // rate of the gamma distribution. we use rate = 1.0/scale and a hyperprior on the P(scale) = 1.0 / scale, which is the infamous OneOverX prior distribution 
    ConstantNode<double> *min = new ConstantNode<double>("minScaleGamma", new double(1));
    //ConstantNode<double> *max = new ConstantNode<double>("maxScaleGamma", new double(1E-10));
    StochasticNode<double> *scale = new StochasticNode<double>("scaleGamma", new ExponentialDistribution(min));
    ConstantNode<double> *one = new ConstantNode<double>("one", new double(1.0) );
    DeterministicNode<double> *rate = new DeterministicNode<double>("rate", new BinaryDivision<double, double, double>(one,scale));
 
    RbVector<Move> moves;
    //Move on population size prior distribution scale
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(scale, 1.0), 2, true ) );

    std::cout << "\t\tParameters for the gamma prior on population sizes set."<<std::endl;

    
    //Setting branch-wise Nes
    std::vector< const TypedDagNode<double> *> ne_nodes;
    for (size_t i = 0; i < nNodes; ++i) 
    {
        std::stringstream o;
        o << "Ne_" << i;
        StochasticNode<double> *NePerNode = new StochasticNode<double>(o.str(), new GammaDistribution(shape,rate) );
        ne_nodes.push_back( NePerNode );
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(NePerNode, 1.0), 2, true ) );
    }
    DeterministicNode< std::vector<double> > *Ne_inf = new DeterministicNode< std::vector<double > >("Nes", new VectorFunction<double>( ne_nodes ) );
//    ConstantNode< std::vector<double> > *Ne_inf = new ConstantNode< std::vector<double> >("N", new std::vector<double>(nNodes, trueNE) );

    
    ConstantNode<double> *speciationRate = new ConstantNode<double>("speciationRate", new double(10.0) );
    ConstantNode<double> *extinctionRate = new ConstantNode<double>("extinctionRate", new double(2.5) );
    ConstantNode<double> *sampling = new ConstantNode<double>("rho", new double(1.0) );
    ConstantNode<double>* origin = new ConstantNode<double>( "origin", new double( t->getRoot().getAge()*2.0 ) );
    std::vector<std::string> names = t->getTipNames();
    std::vector<Taxon> taxaNames;
    for (size_t i = 0; i < names.size(); ++i)
    {
        taxaNames.push_back( Taxon( names[i] ) );
    }
    StochasticNode<TimeTree> *spTree_inf = new StochasticNode<TimeTree>( "SpeciesTree", new ConstantRateBirthDeathProcess( origin, NULL, speciationRate, extinctionRate, sampling, "uniform", "survival", taxaNames, std::vector<Clade>()) );
	
    // If we want to initialize the species tree to the starting tree
    TimeTree *startingTree = spTree_inf->getValue().clone();
    TreeUtilities::rescaleTree(startingTree, &startingTree->getRoot(), 0.01);
    spTree_inf->setValue( startingTree );
    
    std::cerr << "Starting species tree for the MCMC:\n"<< spTree_inf->getValue() << std::endl;

    //Now we set up the substitution model for inference
    //This model is shared over all gene trees
    // gtr model priors
    ConstantNode<std::vector<double> > *bf = new ConstantNode<std::vector<double> >( "bf", new std::vector<double>(4,1.0) );
    ConstantNode<std::vector<double> > *e = new ConstantNode<std::vector<double> >( "e", new std::vector<double>(6,1.0) );

    StochasticNode< std::vector<double> >* pi = new StochasticNode<std::vector<double> >( "pi", new DirichletDistribution(bf) );
    StochasticNode< std::vector<double> >* er = new StochasticNode<std::vector<double> >( "er", new DirichletDistribution(e) ) ;

    DeterministicNode<RateMatrix>* q = new DeterministicNode<RateMatrix>( "q", new GtrRateMatrixFunction(er, pi) ) ;

    //Clock rate:
    ConstantNode<double> *a = new ConstantNode<double>("a", new double(1.0) );
    ContinuousStochasticNode* clockRate = new ContinuousStochasticNode( "rateOfEvolution", new ExponentialDistribution( a ));
    
    std::vector< StochasticNode<TimeTree> *> geneTrees_inf;
    std::vector < NucleotideBranchHeterogeneousCharEvoModel<DnaState, TimeTree> * > charModels; 
    std::vector < StochasticNode< AbstractCharacterData > * > charactermodels;
    std::vector < MultispeciesCoalescent* > multiCoals ;
    for (size_t i = 0; i < nGeneTrees; ++i) {
        std::stringstream o;
        o << "GeneTree_" << i;
        multiCoals.push_back( new MultispeciesCoalescent(spTree_inf, taxa) );
        multiCoals[i]->setNes(Ne_inf);
        geneTrees_inf.push_back( new StochasticNode<TimeTree>(o.str(), multiCoals[i]) );
        // the following line is useful if we want to use the simulated gene trees as data
        // geneTrees_inf[i]->clamp( const_cast<TimeTree*>(simTrees[i]) );

        // Build the character model
        // GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *charModel =
        charModels.push_back(new NucleotideBranchHeterogeneousCharEvoModel<DnaState, TimeTree>( geneTrees_inf[i], true, simSeqs[i]->getValue().getNumberOfCharacters() ) );
        // set the branch heterogeneous substitution matrices
        // if you set instead of a vector a single matrix, then you get a homogeneous model
        charModels[i]->setRateMatrix(q);
        charModels[i]->setRootFrequencies(pi);
        // set the per branch clock rates
        // if you instead specify a single rate, you get a strict clock model
        charModels[i]->setClockRate(clockRate);
        // specify the rate variation across sites
        // if you skip this then you get the model without rate variation across sites.
        // charModels[i]->setSiteRates( site_rates_norm );

        //StochasticNode< AbstractCharacterData > *charactermodel = 
        std::stringstream o2;
        o2 << "characterModel_" << i;
        charactermodels.push_back( new StochasticNode< AbstractCharacterData >(o2.str(), charModels[i]) );
        charactermodels[i]->clamp(simSeqs[i]->getValue().clone() );
    }
    
    Model myModel = Model(spTree_inf);
    
    
    //Now, we define the remaining moves
    //If we want to explore the species tree topology, use these 3 moves:
    moves.push_back( new NearestNeighborInterchange( spTree_inf, 10.0 ) );
    moves.push_back( new NarrowExchange( spTree_inf, 10.0 ) );
    moves.push_back( new FixedNodeheightPruneRegraft( spTree_inf, 30.0 ) );
    
    //Moves for node heights of the species tree only
    moves.push_back( new SubtreeScale( spTree_inf, 10.0 ) );
//    moves.push_back( new TreeScale( spTree_inf, 1.0, true, 10.0 ) );
    moves.push_back( new NodeTimeSlideUniform( spTree_inf, 10.0 ) );
    moves.push_back( new RootTimeSlide( spTree_inf, 1.0, false, 5.0 ) );
    
    //Moves on the substitution model
    moves.push_back( new SimplexSingleElementScale( er, 10.0, true, 2.0 ) );
    moves.push_back( new SimplexSingleElementScale( pi, 10.0, true, 2.0 ) );

    //Moves on the gene trees 
    for (unsigned int i = 0; i < nGeneTrees; i++) {
        moves.push_back(new NearestNeighborInterchange(geneTrees_inf[i], 10.0));
        moves.push_back(new NarrowExchange(geneTrees_inf[i], 10.0));
        moves.push_back(new FixedNodeheightPruneRegraft(geneTrees_inf[i], 30.0));

        //Moves for node heights of the species tree only
        moves.push_back(new SubtreeScale(geneTrees_inf[i], 10.0));
//        moves.push_back(new TreeScale(geneTrees_inf[i], 1.0, true, 10.0));
        moves.push_back(new NodeTimeSlideUniform(geneTrees_inf[i], 10.0));
        moves.push_back(new RootTimeSlide(geneTrees_inf[i], 1.0, false, 5.0));
    }

    
    
    RbVector<Monitor> monitors;
    monitors.push_back( new ModelMonitor( 10, folder+"TestMultispeciesCoalescentWithSequences.p", "\t" ) );
    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( spTree_inf );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, folder+"TestMultispeciesCoalescentWithSequences.trees", "\t", false, false, false ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes2, 10, false, false, false ) );
    for (unsigned int i = 0; i < nGeneTrees; i++) {
        std::stringstream o;
        o << "estimatedGeneTree_" << i << ".trees";

        std::set<DagNode*> monitoredNodesi;
        monitoredNodesi.insert( geneTrees_inf[i] );
        monitors.push_back( new FileMonitor( monitoredNodesi, 10, o.str(), "\t", false, false, false ) );

    }
    
    /* instantiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
//    myMcmc.run(10000);
    myMcmc.run(10000);

    
    // read in the tree trace
    TreeTrace<TimeTree> trace = readTreeTrace(folder+"TestMultispeciesCoalescentWithSequences.trees");
    
    TreeSummary<TimeTree> summary = TreeSummary<TimeTree>(trace);
    summary.summarize();
    summary.printTreeSummary(std::cerr);
    
    return true;
}

