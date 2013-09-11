//
//  TestAdmixtureGraph.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__TestAdmixtureGraph__
#define __revbayes_mlandis__TestAdmixtureGraph__

#include "RbFileManager.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    class TestAdmixtureGraph {
        
    public:
//        TestAdmixtureGraph(const std::string &sfn, int gen, const std::string &tfn="");
        TestAdmixtureGraph(const std::string &sfn, int gen, const std::string &sfp="", const std::string &tfn="");
        TestAdmixtureGraph(int ac, const char* av[], const std::string &sfn, int gen, const std::string &sfp="", const std::string &tfn="");
        virtual                                ~TestAdmixtureGraph(void);                                                            //!< Virtual destructor
        
        bool                                    run();
        
    private:
        
        // members
        std::string                             snpFilename;
        std::string                             snpFilepath;
        std::string                             treeFilename;
        int                                     mcmcGenerations;
        int argc;
        const char** argv;
        std::vector<std::string>                argTokens;
        
    };
    
}

#endif /* defined(__revbayes_mlandis__TestAdmixtureGraph__) */
