/**
 * @file
 *
 * @brief Declaration of ReferenceCount
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#ifndef ReferenceCount_H
#define ReferenceCount_H


#include <map>

class ReferenceCount {
    
public:
    static ReferenceCount&          getInstance(void)                                                      //!< Return a reference to the singleton factory
    {
        static ReferenceCount singleReferenceCount;
        return singleReferenceCount;
    }
    
    void                            decreaseReferenceCount(void* ptr);
    void                            erase(void* ptr);
    size_t                          getNumberReferences(void* ptr);
    void                            increaseReferenceCount(void* ptr);
    
private:
    ReferenceCount(void);                                                                                   //!< Default constructor
    ReferenceCount(const ReferenceCount&);                                                                  //!< Copy constructor
    ReferenceCount&                 operator=(const ReferenceCount&);                                       //!< Assignment operator
    ~ReferenceCount(void);                                                                                  //!< Destructor
    
    std::map<void*, size_t> refCountMap;
};

#endif


