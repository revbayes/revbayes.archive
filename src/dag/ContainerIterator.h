/**
 * @file
 * This file contains the declaration of ContainerIterator, which is
 * used to iterate through a container.
 *
 * @brief Declaration of ContainerIterator
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 * @extends RbInternal
 *
 * $Id: ContainerIterator.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef ContainerIterator_H
#define ContainerIterator_H

#include <vector>

class VectorInteger;

class ContainerIterator : public std::vector<int> {

    public:
                                ContainerIterator(const VectorInteger& index, const VectorInteger& len);     //!< Set value and length

        void                    printValue(std::ostream& o) const;                                   //!< Print value for user
        ContainerIterator&      operator++(void);                                                    //!< Increment index (prefix)
        ContainerIterator&      operator--(void);                                                    //!< Decrement index (prefix)
        ContainerIterator       operator++(int dummy);                                               //!< Increment index (postfix)
        ContainerIterator       operator--(int dummy);                                               //!< Decrement index (postfix)
        bool                    operator==(const ContainerIterator& x) const;                        //!< Equals operator
        bool                    operator<(const ContainerIterator& x) const;                         //!< Less than operator
        void                    push_front(int x) { length.insert(begin(), x); }                     //!< Add element in front

    private:
        std::vector<int>        length;                                                              //!< Length in each dimension
};

#endif


