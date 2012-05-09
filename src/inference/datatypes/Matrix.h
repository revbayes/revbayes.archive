/**
 * @file
 * This file contains the declaration of Matrix,
 * a container type used to hold value matrices for the inference machinery.
 *
 * @brief Declaration of Matrix
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-10 12:55:11 +0100 (Sat, 10 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-05-08, version 1.0
 *
 * $Id: Matrix.h 1327 2012-03-10 11:55:11Z hoehna $
 */

#ifndef Matrix_H
#define Matrix_H

#include <vector>

template <typename valueType>
class Matrix {
public:
    Matrix(size_t n, size_t k);
    Matrix(size_t n, size_t k, valueType v);
    
    
    // overloaded operators
    std::vector<valueType>&                 operator[](size_t index);
    const std::vector<valueType>&           operator[](size_t index) const;
    
    // utility funcions
    void                                    clear(void);
    size_t                                  getNumberOfColumns(void) const;
    size_t                                  getNumberOfRows(void) const;
    size_t                                  size(void) const;
    
private:
    std::vector<std::vector<valueType> >    elements;

};

#endif