#include "Clade.h"
#include "CoalaFunction.h"
#include "ConstantNode.h"
#include "ConstantRateBirthDeathProcess.h"
#include "CorrespondenceAnalysis.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "DiscreteCharacterData.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "FixedNodeheightPruneRegraft.h"
#include "GeneralBranchHeterogeneousCharEvoModel.h"
#include "GtrRateMatrixFunction.h"
#include "MatrixReal.h"
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
#include "NormalDistribution.h"
#include "RbFileManager.h"
#include "RootTimeSlide.h"
#include "ScaleProposal.h"
#include "ScreenMonitor.h"
#include "SimplexMove.h"
#include "SlidingMove.h"
#include "SubtreeScale.h"
#include "TestCoala.h"
#include "TimeTree.h"
#include "TreeHeightStatistic.h"
#include "TreeScale.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"
#include "RbVectorFunction.h"

using namespace RevBayesCore;

TestCoala::TestCoala(const std::string &afn, int gen) : alignmentFilename( afn ), mcmcGenerations( gen ){
    
}

TestCoala::~TestCoala() {
    // nothing to do
}



bool TestCoala::run( void ) {
    
    /* First, we read in the data */
    // the matrix
    std::vector<AbstractCharacterData*> data = NclReader().readMatrices(alignmentFilename);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    std::cout << data[0] << std::endl;
        
    
    /* set up the model graph */
    
    //////////////////////
    // first the priors //
    //////////////////////
    
    // birth-death process priors
    StochasticNode<double> *lambda = new StochasticNode<double>("lambda", new ExponentialDistribution(new ConstantNode<double>("lambda_rate", new double(1.0))));
    ConstantNode<double> *mu = new ConstantNode<double>("mu", new double(0.0));
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0));
    // gtr model priors
    ConstantNode<std::vector<double> > *bf = new ConstantNode<std::vector<double> >( "bf", new std::vector<double>(20,1.0) );
    ConstantNode<std::vector<double> > *e = new ConstantNode<std::vector<double> >( "e", new std::vector<double>(10*19,1.0) );
    
    //Root frequencies
    StochasticNode<std::vector<double> > *rf = new StochasticNode<std::vector<double> >( "rf", new DirichletDistribution(bf) );
    
    
    std::cout << "bf:\t" << bf->getValue() << std::endl;
    std::cout << "e:\t" << e->getValue() << std::endl;
    
    //Declaring a vector of GTR matrices
    size_t numBranches = 2*data[0]->getNumberOfTaxa() - 2;
    std::vector<DeterministicNode < std::vector<double> >* > pis;
    //  std::vector<StochasticNode < std::vector<double> >* > ers;
    std::vector< const TypedDagNode< RateMatrix >* > qs;
    
    ConstantNode< double > *coordMean = new ConstantNode<double>("coordMean", new double(0.0) );
    ConstantNode< double > *coordStDevPrior = new ConstantNode<double>( "coordStDevPrior" , new double(2.0) );
    StochasticNode< double > *coordStDev = new StochasticNode<double>( "coordStDev", new ExponentialDistribution( coordStDevPrior ) );
    
    std::vector< DeterministicNode< std::vector<double> >* > coord_vecs;
    std::vector< std::vector< const TypedDagNode < double > * > > coords;
    StochasticNode<std::vector<double> > * er = new StochasticNode<std::vector<double> >( "er", new DirichletDistribution(e) ) ;
    
    MatrixReal aaFreqs = static_cast< DiscreteCharacterData<AminoAcidState>* >( data[0] )->computeStateFrequencies();
    std::cerr << aaFreqs << std::endl;
    
    unsigned int nAxes = 1;
    CorrespondenceAnalysis ca(aaFreqs, nAxes);
    MatrixReal coa = ca.getPrincipalAxes();
    std::vector<double> cw = ca.getColumnWeights();
                                                                
    for (unsigned int i = 0 ; i < numBranches ; i++ ) 
    {
        std::ostringstream coords_name;
        coords_name << "coords(" << i << ")";
        std::vector< const TypedDagNode< double >* > cs;
        for ( unsigned int j = 0; j < nAxes; ++j) 
        {
            cs.push_back( new StochasticNode<double>( "", new NormalDistribution( coordMean, coordStDev ) ) );
        }
        coords.push_back( cs );
        coord_vecs.push_back( new DeterministicNode<std::vector<double> >(coords_name.str(), new VectorFunction< double >( cs ) ) );
        
        std::ostringstream pi_name;
        pi_name << "pi(" << i << ")";
        pis.push_back(new DeterministicNode<std::vector<double> >( pi_name.str(), new CoalaFunction(coord_vecs[i], coa, cw) ) );
        //  ers.push_back(new StochasticNode<std::vector<double> >( "er", new DirichletDistribution(e) ) );
        std::ostringstream q_name;
        q_name << "q(" << i << ")";
        qs.push_back(new DeterministicNode<RateMatrix>( q_name.str(), new GtrRateMatrixFunction(er, pis[i]) ));
        std::cout << "Q:\t" << qs[i]->getValue() << std::endl;
    }
    
    DeterministicNode< RbVector< RateMatrix > >* qs_node = new DeterministicNode< RbVector< RateMatrix > >( "q_vector", new RbVectorFunction<RateMatrix>(qs) );
    
    
    std::vector<std::string> names = data[0]->getTaxonNames();
    ConstantNode<double>* origin = new ConstantNode<double>( "origin", new double( 2.0 ) );
    std::vector<RevBayesCore::Taxon> taxa;
    for (size_t i = 0; i < names.size(); ++i)
    {
        taxa.push_back( Taxon( names[i] ) );
    }
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, NULL, lambda, mu, rho, "uniform", "survival", taxa, std::vector<Clade>()) );
        
    // and the character model
    GeneralBranchHeterogeneousCharEvoModel<AminoAcidState, TimeTree> *phyloCTMC = new GeneralBranchHeterogeneousCharEvoModel<AminoAcidState, TimeTree>(tau, 20, true, data[0]->getNumberOfCharacters());
    phyloCTMC->setRootFrequencies( rf );
    phyloCTMC->setRateMatrix( qs_node );
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("S", phyloCTMC );
    charactermodel->clamp( data[0] );
    
    
    /* add the moves */
    RbVector<Move> moves;
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(lambda, 1.0), 2, true ) );
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
    
    for (unsigned int i = 0 ; i < numBranches ; i ++ ) {
        //     moves.push_back( new SimplexMove( ers[i], 10.0, 1, true, 2.0 ) );
//        moves.push_back( new SimplexMove( pis[i], 10.0, 1, 0, true, 2.0 ) );
        //    moves.push_back( new SimplexMove( ers[i], 100.0, 6, true, 2.0 ) );
//        moves.push_back( new SimplexMove( pis[i], 100.0, 4, 0, true, 2.0 ) );
    }
    // add some tree stats to monitor
    DeterministicNode<double> *treeHeight = new DeterministicNode<double>("TreeHeight", new TreeHeightStatistic(tau) );
    
    /* add the monitors */
    RbVector<Monitor> monitors;
    std::set<DagNode*> monitoredNodes;
    //    monitoredNodes.insert( er );
    //    monitoredNodes.insert( pi );
    monitoredNodes.insert( lambda );
    monitors.push_back( new FileMonitor( monitoredNodes, 10, "TestCoala.log", "\t" ) );
    std::set<DagNode*> monitoredNodes1;
    monitoredNodes1.insert( er );
    /*    for (unsigned int i = 0 ; i < numBranches ; i ++ ) {
     monitoredNodes1.insert( pis[i] );
     }*/
    monitoredNodes1.insert( rf );
    monitoredNodes1.insert( treeHeight );
    monitors.push_back( new FileMonitor( monitoredNodes1, 10, "TestCoalaSubstRates.log", "\t" ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes1, 10, "\t" ) );
    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( tau );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, "TestCoala.tree", "\t", false, false, false ) );
    
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
    delete lambda;
    //    delete sigma;
    //    delete a;
    //    delete b;
    //    delete c;
    
    std::cout << "Finished GTR model test." << std::endl;
    
    return true;
}
