#ifndef RbRbBitSet_H
#define RbRbBitSet_H

#include <ostream>
#include <vector>

namespace RevBayesCore {
    
    /**
     * RevBayes class for bit sets.
     *
     * This class is essentially a wrapper of a std::vector<bool> with some additional functional.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2016-09-06, version 1.0
     */
    class RbBitSet {
        
    public:
        RbBitSet(void) {}                                                                                         //!< Constructor requires character type
        virtual                        ~RbBitSet(void) {}
        
//        
        virtual bool                            operator[](size_t i) const = 0;
        
        virtual bool                            operator==(const RbBitSet &bs) const = 0;
        virtual bool                            operator!=(const RbBitSet &bs) const = 0;
        virtual bool                            operator<(const RbBitSet &bs) const = 0;
        
//        virtual RbBitSet                        operator&(const RbBitSet &bs) const = 0;
//        virtual RbBitSet                        operator|(const RbBitSet &bs) const = 0;
//        virtual RbBitSet                        operator^(const RbBitSet &bs) const = 0;
        virtual RbBitSet&                       operator~() = 0;
        virtual RbBitSet&                       operator&=(const RbBitSet &bs) = 0;
        virtual RbBitSet&                       operator|=(const RbBitSet &bs) = 0;
        
        
        //        void                            clear(void);
        virtual void                            clearBits(void) = 0;
        virtual bool                            empty(void) const = 0;
        virtual void                            flip(size_t i) = 0;
        virtual size_t                          getNumberSetBits(void) const = 0;                                           //!< Get the number of bits set.
        virtual size_t                          getFirstSetBit(void) const = 0;                                             //!< Get the number of bits set.
        virtual bool                            isSet(size_t i) const = 0;
        virtual void                            resize(size_t size) = 0;
        virtual void                            set(size_t i) = 0;
        virtual size_t                          size(void) const = 0;
        virtual void                            unset(size_t i) = 0;
        
        virtual size_t memorySize() const {
            size_t size = 0;
            
            return size; }
        
        
        
        
    };
    
    // Global functions using the class
    std::ostream&                               operator<<(std::ostream& o, const RbBitSet& x);                    //!< Overloaded output operator
    
}

#endif
