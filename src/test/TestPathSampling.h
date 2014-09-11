//
//  TestPathSampling.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__TestPathSampling__
#define __rb_mlandis__TestPathSampling__


#include "RbFileManager.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    template<class T>
    class TreeTrace;
    
    class BranchLengthTree;
    class Tree;
    class AbstractCharacterData;
    
    class TestPathSampling {
        
    public:
        TestPathSampling(void);
        virtual                                ~TestPathSampling(void);                                                            //!< Virtual destructor
        bool                                    run(void);
        bool                                    run_aa(void);
        bool                                    run_binary(void);
        
    private:
        
    };
    
}


#endif /* defined(__rb_mlandis__TestPathSampling__) */
