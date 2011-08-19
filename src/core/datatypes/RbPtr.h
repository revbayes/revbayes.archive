#include <iostream>
#include <map>
#include "RbException.h"
#include "RbMemoryManager.h"



template <typename T>
class RbPtr {

    public:
        explicit                    RbPtr(T* inPtr);
                                   ~RbPtr(void);
                                    RbPtr(const RbPtr<T>& src);
        RbPtr<T>&                   operator=(const RbPtr<T>& rhs);
        const T&                    operator*(void) const;
        const T*                    operator->(void) const;
        T&                          operator*(void);
        T*                          operator->(void);
        operator                    void*(void) const { return mPtr; }
        
    protected:
        void                        finalizePointer(void);
        void                        initializePointer(T* inPtr);
        T*                          mPtr;
};

template <typename T>
RbPtr<T>::RbPtr(T* inPtr) {

    initializePointer(inPtr);
}

template <typename T>
RbPtr<T>::RbPtr(const RbPtr<T>& src) {

    initializePointer(src.mPtr);
}

template <typename T>
RbPtr<T>& RbPtr<T>::operator=(const RbPtr<T>& rhs) {

    if (this == &rhs) 
        {
        return (*this);
        }
    finalizePointer();
    initializePointer(rhs.mPtr);
    return (*this);
}

template <typename T>
void RbPtr<T>::initializePointer(T* inPtr) {

    mPtr = inPtr;
    RbMemoryManager& myMemoryManager = RbMemoryManager::rbMemoryManager();
    myMemoryManager.incrementCountForAddress(mPtr);
}

template <typename T>
void RbPtr<T>::finalizePointer(void) {

    RbMemoryManager& myMemoryManager = RbMemoryManager::rbMemoryManager();
    myMemoryManager.decrementCountForAddress(mPtr);
}

template <typename T>
const T* RbPtr<T>::operator->(void) const {

    return (mPtr);
}

template <typename T>
const T& RbPtr<T>::operator*(void) const {

    return (*mPtr);
}

template <typename T>
T* RbPtr<T>::operator->(void) {

    return (mPtr);
}

template <typename T>
T& RbPtr<T>::operator*(void) {

    return (*mPtr);
}