#ifndef RbRbBitSetGeneral_H
#define RbRbBitSetGeneral_H

#include <ostream>
#include <vector>

#include "RbBitSet.h"

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
    class RbBitSetGeneral : public RbBitSet {
        
    public:
        RbBitSetGeneral(void);                                                                                         //!< Constructor requires character type
        RbBitSetGeneral(size_t n, bool set=false);                                                                     //!< Constructor requires character type
//        RbBitSetGeneral(const RbBitSetGeneral &bs);                                                                           //!< Constructor requires character type
//        virtual                        ~RbBitSetGeneral(void);
//
//        RbBitSetGeneral&                operator=(const RbBitSetGeneral &bs);

        bool                            operator[](size_t i) const;

        bool                            operator==(const RbBitSet &bs) const;
        bool                            operator!=(const RbBitSet &bs) const;
        bool                            operator<(const RbBitSet &bs) const;
        
        RbBitSetGeneral                 operator&(const RbBitSet &bs) const;
        RbBitSetGeneral                 operator|(const RbBitSet &bs) const;
        RbBitSetGeneral                 operator^(const RbBitSet &bs) const;
        RbBitSetGeneral&                operator~();
        RbBitSetGeneral&                operator&=(const RbBitSet &bs);
        RbBitSetGeneral&                operator|=(const RbBitSet &bs);


//        void                            clear(void);
        void                            clearBits(void);
        bool                            empty(void) const;
        void                            flip(size_t i);
        size_t                          getNumberSetBits(void) const;                                           //!< Get the number of bits set.
        size_t                          getFirstSetBit(void) const;                                             //!< Get the number of bits set.
        bool                            isSet(size_t i) const;
        void                            resize(size_t size);
        void                            set(size_t i);
        size_t                          size(void) const;
        void                            unset(size_t i);
        
        virtual size_t memorySize() const {
            size_t size = 0;
            size += sizeof(num_set_bits);
            size += sizeof(value);
            size += sizeof(bool) * value.size();
            size += sizeof(value) + sizeof(decltype(value)::value_type)*value.capacity();
            
            return size; }
        
    private:

        std::vector<bool>               value;
        size_t                          num_bits;
        size_t                          num_set_bits;
        

        
    };
    
    // Global functions using the class
    std::ostream&                               operator<<(std::ostream& o, const RbBitSetGeneral& x);                    //!< Overloaded output operator
    
}

#endif
