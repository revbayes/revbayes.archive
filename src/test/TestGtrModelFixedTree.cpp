#include "Clade.h"
#include "ConstantNode.h"
#include "ConstantBirthDeathProcess.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "FileMonitor.h"
#include "GeneralBranchHeterogeneousCharEvoModel.h"
#include "GtrRateMatrixFunction.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NclReader.h"
#include "NodeTimeSlideBeta.h"
#include "NodeTimeSlideUniform.h"
#include "RbFileManager.h"
#include "RootTimeSlide.h"
#include "ScaleMove.h"
#include "SimplexMove.h"
#include "SlidingMove.h"
#include "SubtreeScale.h"
#include "TestGtrModelFixedTree.h"
#include "TimeTree.h"
#include "TreeScale.h"
#include "UniformDistribution.h"

using namespace RevBayesCore;

TestGtrModelFixedTree::TestGtrModelFixedTree(const std::string &afn, const std::string &tfn, int gen) : alignmentFilename( afn ), treeFilename( tfn ), mcmcGenerations( gen ){
    
}

TestGtrModelFixedTree::~TestGtrModelFixedTree() {
    // nothing to do
}



bool TestGtrModelFixedTree::run( void ) {
    
    /* First, we read in the data */
    // the matrix
    std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(alignmentFilename);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    
    // and the tree
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    
    /* set up the model graph */
    
    // first the priors
    // birth-death process priors
    StochasticNode<double> *div = new StochasticNode<double>("diversification", new UniformDistribution(new ConstantNode<double>("", new double(0.0)), new ConstantNode<double>("", new double(100.0)) ));
    ConstantNode<double> *turn = new ConstantNode<double>("turnover", new double(0.0));
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0));
    std::vector<std::string> names = data[0]->getTaxonNames();
    ConstantNode<std::vector<double> > *met = new ConstantNode<std::vector<double> >("MET",new std::vector<double>() );
    ConstantNode<std::vector<double> > *mep = new ConstantNode<std::vector<double> >("MESP",new std::vector<double>() );
    StochasticNode<TimeTree> *tree = new StochasticNode<TimeTree>( "tau", new ConstantBirthDeathProcess(div, turn, met, mep, rho, "uniform", "survival", int(names.size()), names, std::vector<Clade>()) );
    
    tree->setValue( trees[0] );

    ConstantNode<std::vector<double> > *bf = new ConstantNode<std::vector<double> >( "bf", new std::vector<double>(4,1.0/4.0) );
    ConstantNode<std::vector<double> > *e = new ConstantNode<std::vector<double> >( "e", new std::vector<double>(6,1.0) );
    
    std::cout << "bf:\t" << bf->getValue() << std::endl;
    std::cout << "e:\t" << e->getValue() << std::endl;

    // then the parameters
    StochasticNode<std::vector<double> > *pi = new StochasticNode<std::vector<double> >( "pi", new DirichletDistribution(bf) );
    StochasticNode<std::vector<double> > *er = new StochasticNode<std::vector<double> >( "er", new DirichletDistribution(e) );
    
    std::cout << "pi:\t" << pi->getValue() << std::endl;
    std::cout << "er:\t" << er->getValue() << std::endl;
    
    
//    std::vector<double> *pi_emp = new std::vector<double>();
//    pi_emp->push_back(0.295100);
//    pi_emp->push_back(0.400767);
//    pi_emp->push_back(0.145596);
//    pi_emp->push_back(0.158537);
//    pi->setValue( pi_emp );
//    std::vector<double> *er_emp = new std::vector<double>();
//    er_emp->push_back(0.126168);
//    er_emp->push_back(0.459902);
//    er_emp->push_back(0.125375);
//    er_emp->push_back(0.123063);
//    er_emp->push_back(0.056665);
//    er_emp->push_back(0.108827);
    std::vector<double> *pi_emp = new std::vector<double>();
    pi_emp->push_back(0.282074);
    pi_emp->push_back(0.213660);
    pi_emp->push_back(0.094146);
    pi_emp->push_back(0.410120);
    pi->setValue( pi_emp );
    std::vector<double> *er_emp = new std::vector<double>();
    er_emp->push_back(0.387769);
    er_emp->push_back(0.028213);
    er_emp->push_back(0.134871);
    er_emp->push_back(0.041994);
    er_emp->push_back(0.147997);
    er_emp->push_back(0.259156);
    er->setValue( er_emp );
//    pi->setValue( new std::vector<double>(4,1.0/4.0) );
//    er->setValue( new std::vector<double>(6,1.0/6.0) );
    
    std::cout << "pi:\t" << pi->getValue() << std::endl;
    std::cout << "er:\t" << er->getValue() << std::endl;

    DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new GtrRateMatrixFunction(er, pi) );
    
    std::cout << "Q:\t" << q->getValue() << std::endl;
    
    ConstantNode<double> *clockRate = new ConstantNode<double>("clockRate", new double(1.0) );
    
    // and the character model
//    SimpleSiteHomogeneousCharEvoModel< DnaState, TimeTree > *charModel = new SimpleSiteHomogeneousCharEvoModel<DnaState, TimeTree>(tree, q, clockRate, true, data[0]->getNumberOfCharacters() );
    GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *charModel = new GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tree, 4, true, data[0]->getNumberOfCharacters() ); 
    charModel->setRateMatrix( q );
    charModel->setClockRate( clockRate );
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("S", charModel );
    charactermodel->clamp( data[0] );
    
    std::cout << "Expected log-L:\t\t" << "-6741.647819812251" << std::endl; // obtained from BEAST
    std::cout << "Observed log-L:\t\t" << charactermodel->getLnProbability() << std::endl;
    
    /* add the moves */
    std::vector<Move*> moves;
    moves.push_back( new ScaleMove(div, 1.0, true, 1.0) );
    moves.push_back( new SimplexMove( er, 10.0, 1, 0, true, 5.0 ) );
    moves.push_back( new SimplexMove( pi, 10.0, 1, 0, true, 5.0 ) );
    moves.push_back( new SimplexMove( er, 10.0, 6, 0, true, 5.0 ) );
    moves.push_back( new SimplexMove( pi, 10.0, 4, 0, true, 5.0 ) );
//    moves.push_back( new SubtreeScale( tree, 5.0 ) );
//    moves.push_back( new TreeScale( tree, 1.0, true, 1.0 ) );
    moves.push_back( new NodeTimeSlideUniform( tree, 20.0 ) );
    moves.push_back( new RootTimeSlide( tree, 1.0, true, 5.0 ) );
    
    /* add the monitors */
    std::vector<Monitor*> monitors;
    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert( div );
    monitors.push_back( new FileMonitor( monitoredNodes, 1, "TestGtrModelFixedTree.log", "\t" ) );
    std::set<DagNode*> monitoredNodes1;
    monitoredNodes1.insert( er );
    monitoredNodes1.insert( pi );
    monitors.push_back( new FileMonitor( monitoredNodes1, 10, "TestGtrModelFixedTreeSubstRates.log", "\t" ) );
    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( tree );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, "TestGtrModelFixedTree.tree", "\t", false, false, false ) );
    
    /* instantiate the model */
    Model myModel = Model(q);
    
    /* instiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(mcmcGenerations);
    
    
    /* clean up */
//    for (size_t i = 0; i < 10; ++i) {
//        delete x[i];
//    }
//    delete [] x;
//    delete mu;
//    delete sigma;
//    delete a;
//    delete b;
//    delete c;
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
        const Move *theMove = *it;
        delete theMove;
    }
    for (std::vector<Monitor*>::iterator it = monitors.begin(); it != monitors.end(); ++it) {
        const Monitor *theMonitor = *it;
        delete theMonitor;
    }
    
    return true;
}
