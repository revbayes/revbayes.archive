//
//  RbDagNodeMemoryManager.h
//  revbayes
//
//  Created by Sebastian Hoehna on 11/5/14.
//  Copyright (c) 2014 Sebastian Hoehna. All rights reserved.
//

#ifndef revbayes_RbDagNodeMemoryManager_h
#define revbayes_RbDagNodeMemoryManager_h


namespace RevBayesCore {
    
    class RbDagNodeMemoryManager {
        
    public:
        virtual                            ~RbDagNodeMemoryManager(void) {}                                                                      //!< Virtual destructor
        
        static RbDagNodeMemoryManager&     getInstance(void)                                                                                     //!< Get the global instance
        {
            static RbDagNodeMemoryManager theInstance = RbDagNodeMemoryManager();
            return theInstance;
        }
        
        
        int         decreaseCounter() { --counter; return counter; }
        int         increaseCounter() { ++counter; return counter; }
        int         getCounter() { return counter; }
    private:
        RbDagNodeMemoryManager(void) { counter = 0; }

        int counter;
        
    };
}

#endif
