/**
 * @file
 * This file contains the declaration of RbMemoryManager, which is
 * a singleton class that manages the memory of objects allocated by
 * wrapping the pointer around an RbPtr (templated class object).
 *
 * @brief Declaration of RbMemoryManager
 *
 */

#ifndef RbMemoryManager_H
#define RbMemoryManager_H

#include <map>



class RbMemoryManager {

    public:
        static RbMemoryManager&     rbMemoryManager(void)                                           //!< Get the memory manager
		                               {
		                               static RbMemoryManager theMemoryManager = RbMemoryManager();
		                               return theMemoryManager;
		                               }
        int                         countForAddress(void* qPtr);
        void                        incrementCountForAddress(void* qPtr);
        void                        decrementCountForAddress(void* qPtr);

    protected:
                                    RbMemoryManager(void) { }                                       //!< Prevent construction
                                    RbMemoryManager(const RbMemoryManager& x) { }                   //!< Prevent copy construction
        virtual                    ~RbMemoryManager(void) { }                                       //!< Destructor
        std::map<void*,int>         refCountMap;
        RbMemoryManager&            operator=(const RbMemoryManager& m) { return (*this); }         //!< Prevent assignment
};

#endif

