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
        size_t                          countForAddress(const void* qPtr);
//        void                            incrementCountForAddress(void* qPtr);
        void                            incrementCountForAddress(const void* qPtr);
//        bool                            decrementCountForAddress(void* qPtr);
        bool                            decrementCountForAddress(const void* qPtr);
    
        size_t                          numberOfRegisteredObjects(void) const;
        size_t                          numberOfReferences(void) const;

    protected:
                                        RbMemoryManager(void) { }                                       //!< Prevent construction
                                        RbMemoryManager(const RbMemoryManager& x) { }                   //!< Prevent copy construction
        virtual                        ~RbMemoryManager(void) { }                                       //!< Destructor
        std::map<const void*,size_t>    refCountMap;
        RbMemoryManager&                operator=(const RbMemoryManager& m) { return (*this); }         //!< Prevent assignment
};

#endif

