/**
 * @file
 * This file contains a test of the multispecies coalescent process.
 * The test simulates a species tree, then gene trees according to the multispecies coalescent, 
 * then gene sequences along the gene trees. 
 *
 * @brief Test of the multispecies coalescent model with sequences
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-05
 *
 * $Id:$
 */

#ifndef TESTMULTISPECIESCOALESCENTWITHSEQUENCES_H
#define	TESTMULTISPECIESCOALESCENTWITHSEQUENCES_H


#include "RbFileManager.h"
#include "TreeTrace.h"
#include "TimeTree.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    class TestMultispeciesCoalescentWithSequences {
        
    public:
        TestMultispeciesCoalescentWithSequences(const std::string &treeFile);
        virtual                         ~TestMultispeciesCoalescentWithSequences(void);                                                            //!< Virtual destructor
        
        TreeTrace<TimeTree>              readTreeTrace(const std::string &fname);
        bool                            run();
        
    private:        
        // members
        std::string                     treeFilename;
//        std::int                        numProcesses;
//        std::vector<std::vector<size_t> >                   genesPerProcess;

    };
    
}


#endif	/* TESTMULTISPECIESCOALESCENTWITHSEQUENCES_H */
