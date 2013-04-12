/**
 * @file
 * This file contains the declaration of Alignment, which is
 * class that holds a character matrix in RevBayes.
 *
 * @brief Declaration of Alignment
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-07-10 14:09:12 +0200 (Tue, 10 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: CharacterData.h 1660 2012-07-10 12:09:12Z hoehna $
 */

#ifndef DistanceMatrix_H
#define DistanceMatrix_H

#include "MatrixReal.h"

#include <map>
#include <set>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    class DistanceMatrix : public MatrixReal {
        
    public:    
        DistanceMatrix(const size_t nTaxa);                                                                             //!< Constructor requires character type
        virtual                            ~DistanceMatrix(void) {}
        
        // CharacterData functions
        void                                addTaxonWithName(std::string s);                                            //!< Add a taxon name to the matrix
        void                                excludeTaxon(size_t i);                                                     //!< Exclude taxon
        void                                excludeTaxon(std::string& s);                                               //!< Exclude taxon
        double                              getDistance(size_t row, size_t col) const;                                  //!< Get distance from matrix
        size_t                              getNumberOfTaxa(void) const;                                                //!< Number of taxa
        const std::string&                  getTaxonNameWithIndex(size_t idx) const;                                    //!< Returns the idx-th taxon name
        bool                                isTaxonExcluded(size_t i) const;                                            //!< Is the taxon excluded
        bool                                isTaxonExcluded(std::string& s) const;                                      //!< Is the taxon excluded
        void                                restoreTaxon(size_t i);                                                     //!< Restore taxon
        void                                restoreTaxon(std::string& s);                                               //!< Restore taxon
        void                                showData(void);                                                             //!< Show the data in the character matrix
        
    private:
        // Utility functions
        size_t                              indexOfTaxonWithName(const std::string& s) const;                           //!< Get the index of the taxon
        
        // Member variables
        std::set<size_t>                    deletedTaxa;                                                                //!< Set of deleted taxa
        std::vector<std::string>            sequenceNames;                                                              //!< names of the sequences
        
    };
    
}

#endif
