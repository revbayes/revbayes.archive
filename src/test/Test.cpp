//
//  Test.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/11/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "BranchLengthTree.h"
#include "ConstantNode.h"
#include "NewickConverter.h"
#include "RbException.h"
#include "RbOptions.h"
#include "Test.h"
#include "TestDPPRelClock.h"
#include "TestFilteredStandardLikelihood.h"
#include "Tree.h"
#include "TreeTrace.h"

#ifdef USE_LIB_ARMADILLO
#include "TestAdmixtureGraph.h"
#endif

#include <time.h>
#include <vector>

using namespace RevBayesCore;

Test::Test(void) {
    
}


Test::Test(int c, const char* v[]) : argc(c), argv(v)
{
    
}


Test::~Test() {
    
}



bool Test::performTests(int argc, const char * argv[]) {
    time_t start,end;
    time (&start);
    int numPassed = 0;
    int numAttempted = 0;
    
    ////////////////
    // Newer tests
    ////////////////
    try {
        numAttempted += 1;
        TestFilteredStandardLikelihood testFSL = TestFilteredStandardLikelihood(
#                                                                               if defined(READ_MORPHO_AS_DNA)
                                                                                    "data/morpho-as-dna.nex",
#                                                                               else
                                                                                    "data/morpho.nex",
#                                                                               endif
                                                                                "data/morpho.tre");
        if (testFSL.run()) {
            numPassed += 1;
        } else {
            std::cerr << "TestFilteredStandardLikelihood failed!" << std::endl;
        }
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
	// #######
    // TAH: working on relaxed-clock models, setting up consistent test files
	
    /* A DPP relaxed model test */
    try {
        //        TestDPPRelClock testDPPRC = TestDPPRelClock("data/Primates.nex", "data/primates.tree", 100);
        TestDPPRelClock testDPPRC = TestDPPRelClock("data/test_data_clock_gtr.nex", "data/true_calib_clk.tre", 100000);

//      numAttempted += 1;
        //if (testDPPRC.run()) {
        //    numPassed += 1;
        //} else {
        //    std::cerr << "TestDPPRelClock failed!" << std::endl;
        //}
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
	   
	// #######
	
		    
    
#ifdef USE_LIB_ARMADILLO
    // Admixture graph
    try {
        //TestAdmixtureGraph testAG = TestAdmixtureGraph("canmap2_snps_removedMissingData.txt", 10000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph("moo_snps.no_999111.removedMissingData.txt", 10000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph("humans.071713.txt", 10000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph("modern.clovis.130723.txt", 10000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph("modern.saqqaq.130723.txt", 10000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph("modern.malta.130723.txt", 10000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph("modern.939.130723.txt", 10000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph(argc, argv, "small.Modern_Saqqaq_Clovis.genotypes.polymorphic.txt.noMayanAleutsTsimshianAltai.TreeMixInput.txt", 2000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph(argc, argv, "Modern_Saqqaq_Clovis_Malta.fix_haploid.txt", 2000, "/Users/mlandis/data/admix/input/", "");
        TestAdmixtureGraph testAG = TestAdmixtureGraph(argc, argv, "hgdp_humans.txt", 2000, "/Users/mlandis/data/admix/input/", "");
        //testAG.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
#endif
    
    
    time (&end);
    double dif = difftime(end,start);
    std::cout << "The tests ran in " << dif << " seconds." << std::endl;
    if (numPassed == numAttempted) {
        std::cout << "Finished Tests!!!" << std::endl;
        return true;
    }
    std::cout << "Unfortunately, only " << numPassed << " out of " << numAttempted << " tests passed." << std::endl;
    return false;

}
