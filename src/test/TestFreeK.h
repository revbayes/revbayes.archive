//
//  TestFreeK.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__TestFreeK__
#define __rb_mlandis__TestFreeK__


#include <string>
#include <vector>

namespace RevBayesCore {
    
    template<class T>
    class TreeTrace;
    
    class BranchLengthTree;
    class Tree;
    class AbstractCharacterData;
    
    class TestFreeK {
        
    public:
        TestFreeK(void);
        virtual                                ~TestFreeK(void);                                                            //!< Virtual destructor
        bool                                    run(void);
        
    private:
        
    };
    
}


#endif /* defined(__rb_mlandis__TestFreeK__) */
