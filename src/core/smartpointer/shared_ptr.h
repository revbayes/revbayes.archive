// 
// SuperSmartPointer.h 
// 
// From Chapter 25 of Professional C++ 
// by Nicholas A. Solter and Scott J. Kleper 
// (Wrox, 2005) 


#ifndef SHARED_PTR_H
#define SHARED_PTR_H

class shared_ptr { 
    
    public: 
    
    explicit shared_ptr(T* inPtr); 
    ~shared_ptr(); 
    shared_ptr(const shared_ptr& src); 
    shared_ptr& operator=(const shared_ptr& rhs); 
    const T& operator*() const; 
    const T* operator->() const; 
    T& operator*(); 
    T* operator->(); 
    operator void*() const { return mPtr; } 
    
    protected: 
    T* mPtr;  
    void finalizePointer(); 
    void initPointer(T* inPtr); 

}; 

#endif // SHARED_PTR_H