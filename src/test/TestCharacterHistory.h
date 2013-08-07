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
        TestCharacterHistory(int gen);
        virtual                                ~TestCharacterHistory(void);                                                            //!< Virtual destructor
        
        bool                                    run();
        
    private:
        
        int                                     mcmcGenerations;
        
    };
    
}


#endif /* defined(__rb_mlandis__TestCharacterHistory__) */
