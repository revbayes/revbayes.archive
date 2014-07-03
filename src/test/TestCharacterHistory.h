//
//  TestCharacterHistory.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__TestCharacterHistory__
#define __rb_mlandis__TestCharacterHistory__


#include "RbFileManager.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    class Tree;
    class AbstractCharacterData;
    
    class TestCharacterHistory {
        
    public:
        TestCharacterHistory(const std::string &afn,  const std::string &tfn, const std::string &gfn, int gen, const std::string &fp="");
        TestCharacterHistory(int ac, const char* av[], const std::string &afn, const std::string &tfn, const std::string &gfn, int gen, const std::string &fp="");
        virtual                                ~TestCharacterHistory(void);                                                            //!< Virtual destructor

        bool                                    run(void);
        bool                                    run_old(void);
        bool                                    run_exp(void);
        bool                                    run_dollo(void);
        
    private:
        void tokenizeArgv(void);
        
        // members
        std::string                             filepath;
        std::string                             areaFilename;
        std::string                             treeFilename;
        std::string                             geoFilename;
        std::string                             areaNodeFilename;
        std::string                             areaEdgeFilename;
        std::string                             timeatlasFilename;
        
        int                                     mcmcGenerations;
        int                                     argc;
        const char**                            argv;
        std::vector<std::string>                argTokens;
        
    };
    
}


#endif /* defined(__rb_mlandis__TestCharacterHistory__) */
