/**
 * @file
 * This file contains the implementation of ContainerIterator, which is
 * used to iterate through a container.
 *
 * @brief Implementation of ContainerIterator
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id: ContainerIterator.h 63 2009-11-18 00:05:57Z ronquist $
 */

#include "ContainerIterator.h"
#include "IntVector.h"
#include "RbException.h"


/** Constructor from vector<int> */
ContainerIterator::ContainerIterator(const std::vector<int> len) {

    if (len.size() == 0)
        throw (RbException("Container index with missing length"));

    for (std::vector<int>::const_iterator i=len.begin(); i!=len.end(); i++) {
        if ((*i) <= 0)
            throw (RbException("Container index with nonpositive length"));
        push_back(0);
        length.push_back(*i);
    }
}


/** Constructor from IntVector*/
ContainerIterator::ContainerIterator(const IntVector& len) {

    if (len.size() == 0)
        throw (RbException("Container index with missing length"));

    for (size_t i=0; i<len.size(); i++) {
        if (len[i] <= 0)
            throw (RbException("Container index with nonpositive length"));
        push_back(0);
        length.push_back(len[i]);
    }
}


/** Overloaded operator++ (prefix) (We add ints with base 10 so we can go on to infinity) */
ContainerIterator& ContainerIterator::operator++() {

    std::vector<int>::reverse_iterator i, j;
    for (i=rbegin(), j=length.rbegin(); i!=rend(); i++, j++) {
        (*i)++;
        if ((*i) < (*j))
            break;
        (*i) = 0;
    }
    /* Add in numbers ad infinitum */
    if (i == rend()) {
        insert(begin(), 1);
        if (j == length.rend())
            length.insert(length.begin(), 10);
    }
    return (*this);
}


/** Overloaded operator++ (postfix) */
ContainerIterator ContainerIterator::operator++(int dummy) {

    ContainerIterator result = (*this);
    ++(*this);
    return result;
}


/** Overloaded operator-- (prefix) */
ContainerIterator& ContainerIterator::operator--() {

    std::vector<int>::reverse_iterator i, j;
    for (i=rbegin(), j=length.rbegin(); i!=rend(); i++, j++) {
        (*i)--;
        if ((*i) >= 0)
            break;
        else
            (*i) = (*j) - 1;
    }
    /* Take away numbers until empty */
    if (i == rend() && size() > 0) {
        pop_back();
        for (i=rbegin(); i!=rend(); i++)
            (*i) = 0;
    }
    return (*this);
}


/** Overloaded operator-- (postfix) */
ContainerIterator ContainerIterator::operator--(int dummy) {

    ContainerIterator result = (*this);
    --(*this);
    return result;
}


/** Overloaded operator== */
bool ContainerIterator::operator==(const ContainerIterator& x) const {

    if (size() != x.size())
        return false;

    std::vector<int>::const_iterator i, j;
    for (i=begin(), j=length.begin(); i!=end(); i++, j++) {
        if ((*i) != (*j))
            return false;
    }
    return true;
}


/** Overloaded operator< (smaller length is always smaller if unequal) */
bool ContainerIterator::operator<(const ContainerIterator& x) const {

    if (size() < x.size())
        return true;
    else if (size() > x.size())
        return false;
    
    std::vector<int>::const_iterator i, j;
    for (i=begin(), j=length.begin(); i!=end(); i++, j++) {
        if ((*i) < (*j))
            return true;
    }
    return false;
}


