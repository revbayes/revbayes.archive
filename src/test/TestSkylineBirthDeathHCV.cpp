
#include "Clade.h"
#include "ConstantRateBirthDeathProcess.h"
#include "ConstantNode.h"
#include "ContinuousStochasticNode.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "Mcmc.h"
#include "MetropolisHastingsMove.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NclReader.h"
#include "RbFileManager.h"
#include "ScaleProposal.h"
#include "SlidingMove.h"
#include "TestSkylineBirthDeathHCV.h"
#include "TimeTree.h"
#include "TreeUtilities.h"
#include "UniformDistribution.h"
#include "StochasticNode.h"
#include "PiecewiseConstantSerialSampledBirthDeathProcess.h"
#include "DeterministicNode.h"
#include "BinaryMultiplication.h"
#include "VectorFunction.h"
#include "Taxon.h"

using namespace RevBayesCore;

TestSkylineBirthDeathHCV::TestSkylineBirthDeathHCV(const std::string &aFn, const std::string &tFn, int gen) : alignmentFilename( aFn ), treeFilename( tFn ), mcmcGenerations( gen ){
    
}

TestSkylineBirthDeathHCV::~TestSkylineBirthDeathHCV() {
    // nothing to do
}


bool TestSkylineBirthDeathHCV::run( void ) {
    
    alignmentFilename = "/Users/tracyh/Code/RevBayes/virus/data/HCV_data.nex";
    treeFilename = "/Users/tracyh/Code/RevBayes/virus/data/HCV_data_start.tre";

    /* First, we read in the data */
	std::vector<AbstractCharacterData*> data = NclReader().readMatrices(alignmentFilename);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
//    std::cout << data[0] << std::endl;
	
	// First, we read in the data 
    std::vector<TimeTree*> trees = NclReader().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
//    std::cout << trees[0]->getNewickRepresentation() << std::endl;
	
	
	// Origin //
	ConstantNode<double> *origin  = new ConstantNode<double>( "origin", new double( 257.2439155 ) );

	
	size_t nInterval = 10;
	size_t nChanges = nInterval - 1;
	
//	std::vector<double> *relChangeTimes;
	std::vector<const TypedDagNode<double> *> absChangeTimes;
	for( size_t i=0; i<nChanges; i++){
		ConstantNode<double> *v = new ConstantNode<double>("", new double(1.0 - (i * (1.0 / (double)nInterval))));
		std::ostringstream brName;
        brName << "ct(" << i << ")";
//		relChangeTimes.push_back(v);
		DeterministicNode<double> *tmpT = new DeterministicNode<double>(brName.str(), new BinaryMultiplication<double,double,double>(origin, v) );
		absChangeTimes.push_back(tmpT);
	}
	DeterministicNode< std::vector< double > >* vAbsChangeTimes = new DeterministicNode< std::vector< double > >( "absol_changes", new VectorFunction< double >( absChangeTimes ) );
    
    std::vector<const TypedDagNode<double> *> R0V;
	ConstantNode<double> *r1 = new ConstantNode<double>("r1", new double(1.355944061));
    R0V.push_back(r1);
	ConstantNode<double> *r2 = new ConstantNode<double>("r2", new double(1.103042864));
    R0V.push_back(r2);
	ConstantNode<double> *r3 = new ConstantNode<double>("r3", new double(0.4261830512));
    R0V.push_back(r3);
	ConstantNode<double> *r4 = new ConstantNode<double>("r4", new double(1.196423027));
    R0V.push_back(r4);
	ConstantNode<double> *r5 = new ConstantNode<double>("r5", new double(1.088663642));
    R0V.push_back(r5);
	ConstantNode<double> *r6 = new ConstantNode<double>("r6", new double(0.6275632772));
    R0V.push_back(r6);
	ConstantNode<double> *r7 = new ConstantNode<double>("r7", new double(0.3322857361));
    R0V.push_back(r7);
	ConstantNode<double> *r8 = new ConstantNode<double>("r8", new double(1.274769002));
    R0V.push_back(r8);
	ConstantNode<double> *r9 = new ConstantNode<double>("r9", new double(1.199315078));
    R0V.push_back(r9);
	ConstantNode<double> *r10 = new ConstantNode<double>("r10", new double(1.010521913));
    R0V.push_back(r10);
	DeterministicNode< std::vector< double > >* vR0 = new DeterministicNode< std::vector< double > >( "R0", new VectorFunction< double >( R0V ) );

	std::vector<const TypedDagNode<double> *> dv;
	ConstantNode<double> *d1 = new ConstantNode<double>("d1", new double(0.78798691));
    dv.push_back(d1);
	ConstantNode<double> *d2 = new ConstantNode<double>("d2", new double(1.170685969));
    dv.push_back(d2);
	ConstantNode<double> *d3 = new ConstantNode<double>("d3", new double(0.236631795));
    dv.push_back(d3);
	ConstantNode<double> *d4 = new ConstantNode<double>("d4", new double(0.2605686551));
    dv.push_back(d4);
	ConstantNode<double> *d5 = new ConstantNode<double>("d5", new double(4.735510115));
    dv.push_back(d5);
	ConstantNode<double> *d6 = new ConstantNode<double>("d6", new double(0.8664180496));
    dv.push_back(d6);
	ConstantNode<double> *d7 = new ConstantNode<double>("d7", new double(0.4407274484));
    dv.push_back(d7);
	ConstantNode<double> *d8 = new ConstantNode<double>("d8", new double(0.4372837011));
    dv.push_back(d8);
	ConstantNode<double> *d9 = new ConstantNode<double>("d9", new double(0.8993880512));
    dv.push_back(d9);
	ConstantNode<double> *d10 = new ConstantNode<double>("d10", new double(3.992530229));
    dv.push_back(d10);
	DeterministicNode< std::vector< double > >* vDelta = new DeterministicNode< std::vector< double > >( "delta", new VectorFunction< double >( dv ) );

	std::vector<const TypedDagNode<double> *> lv;
	for( size_t i=0; i<nInterval; i++){
		std::ostringstream brName;
        brName << "lambda(" << i << ")";
//		relChangeTimes.push_back(v);
		DeterministicNode<double> *tmpL = new DeterministicNode<double>(brName.str(), new BinaryMultiplication<double,double,double>(R0V[i], dv[i]) );
		lv.push_back(tmpL);
	}
	DeterministicNode< std::vector< double > >* vLambda = new DeterministicNode< std::vector< double > >( "lambda", new VectorFunction< double >( lv ) );

	std::vector<const TypedDagNode<double> *> rhov;
	ConstantNode<double> *rho1 = new ConstantNode<double>("rho1", new double(3.94e-04));
    rhov.push_back(rho1);
	DeterministicNode< std::vector< double > >* vRho = new DeterministicNode< std::vector< double > >( "rho", new VectorFunction< double >( rhov ) );

	std::vector<const TypedDagNode<double> *> sv;
	ConstantNode<double> *s1 = new ConstantNode<double>("s1", new double(0.0));
    sv.push_back(s1);
	DeterministicNode< std::vector< double > >* vS = new DeterministicNode< std::vector< double > >( "s", new VectorFunction< double >( sv ) );

	std::vector<const TypedDagNode<double> *> nullTs;
	ConstantNode<double> *nt1 = new ConstantNode<double>("nt1", new double(0.0));
    nullTs.push_back(nt1);
	DeterministicNode< std::vector< double > >* vNT = new DeterministicNode< std::vector< double > >( "nts", new VectorFunction< double >( nullTs ) );
//	TypedDagNode<std::vector<double> > *vNT;

	std::vector<std::string> names = data[0]->getTaxonNames();
    std::vector<Taxon> taxa;
    for (size_t i = 0; i < names.size(); ++i) 
    {
        taxa.push_back( Taxon( names[i] ) );
    }
//																								(const TypedDagNode<double> *o,
//                                                                                                 const TypedDagNode<std::vector<double> > *s, const TypedDagNode<std::vector<double> > *st,
//                                                                                                 const TypedDagNode<std::vector<double> > *e, const TypedDagNode<std::vector<double> > *et,
//                                                                                                 const TypedDagNode<std::vector<double> > *p, const TypedDagNode<std::vector<double> > *pt,
//                                                                                                 const TypedDagNode<std::vector<double> > *r, const TypedDagNode<std::vector<double> > *rt,
//                                                                                                 double tLastSample, const std::string &cdt, 
//                                                                                                 const std::vector<Taxon> &tn, const std::vector<Clade> &c)
//    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new PiecewiseConstantSerialSampledBirthDeathProcess(origin,
//																															vLambda, vAbsChangeTimes,
//																															vDelta, vAbsChangeTimes,
//																															vS, vNT,
//																															vRho, vNT,
//																															0.0, "survival",
//																															taxa, std::vector<Clade>()) );
	
//	tau->clamp(trees[0]);
//	
//	double lnp = tau->getLnProbability();
//	
//	std::cout << " *** " << lnp << std::endl;
   
    return true;
}
