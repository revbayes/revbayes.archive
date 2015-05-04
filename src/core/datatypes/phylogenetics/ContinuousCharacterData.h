#ifndef ContinuousCharacterData_H
#define ContinuousCharacterData_H

#include "AbstractCharacterData.h"
#include "ContinuousTaxonData.h"

#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

namespace RevBayesCore {
    
    /**
     * Class for all Continuous character data objects.
     *
     * The continuous character data class is class for all continuous character data objects.
     * Here the data is stored via a set of ContinuousTaxonData objects.
     * Several convenience functions are provided to access, store and delete the data.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-02-16, version 1.0
     */
    class ContinuousCharacterData : public AbstractCharacterData {
        
    public:
                                                        ContinuousCharacterData();                                                                                        //!< Default constructor
        
        // Overloaded operators
        const ContinuousTaxonData&                      operator[](size_t i) const;                                                 //!< Subscript operator (const)
        
        // implemented methods of the Cloneable interface
        ContinuousCharacterData*                        clone(void) const;
        
        // Container functions
        void                                            clear(void);
        
        // CharacterData functions
        void                                            addTaxonData(const AbstractTaxonData &obs);                                 //!< Add taxon data
        void                                            addTaxonData(const ContinuousTaxonData &obs);                               //!< Add Continuous taxon data
        ContinuousCharacterData&                        concatenate(const ContinuousCharacterData &d);                              //!< Concatenate data matrices
        ContinuousCharacterData&                        concatenate(const AbstractCharacterData &d);                                //!< Concatenate data matrices
        void                                            excludeAllCharacters(void);                                                 //!< Exclude all characters
        void                                            excludeCharacter(size_t i);                                                 //!< Exclude character
        void                                            excludeTaxon(size_t i);                                                     //!< Exclude taxon
        void                                            excludeTaxon(const std::string& s);                                         //!< Exclude taxon
        const double&                                   getCharacter(size_t tn, size_t cn) const;                                   //!< Return a reference to a character element in the character matrix
        std::string                                     getDatatype(void) const;
        const std::string&                              getFileName(void) const;                                                    //!< Returns the name of the file the data came from
        const std::string&                              getFilePath(void) const;                                                    //!< Returns the name of the file path the data came from
        size_t                                          getIndexOfTaxon(const std::string &n) const;                                //!< Get the index of the taxon with name 'n'.
        size_t                                          getNumberOfCharacters(void) const;                                          //!< Number of characters
        size_t                                          getNumberOfIncludedCharacters(void) const;                                  //!< Number of characters
        size_t                                          getNumberOfTaxa(void) const;                                                //!< Number of taxa
        size_t                                          getNumberOfIncludedTaxa(void) const;                                        //!< Number of included taxa
        ContinuousTaxonData&                            getTaxonData(size_t tn);                                                    //!< Return a reference to a sequence in the character matrix
        const ContinuousTaxonData&                      getTaxonData(size_t tn) const;                                              //!< Return a reference to a sequence in the character matrix
        ContinuousTaxonData&                            getTaxonData(const std::string &tn);                                        //!< Return a reference to a sequence in the character matrix
        const ContinuousTaxonData&                      getTaxonData(const std::string &tn) const;                                  //!< Return a reference to a sequence in the character matrix
        const std::vector<std::string>&                 getTaxonNames(void) const;                                                  //!< Get the names of the taxa
        const std::string&                              getTaxonNameWithIndex(size_t idx) const;                                    //!< Returns the idx-th taxon name
        void                                            includeCharacter(size_t i);                                                 //!< Include character
        bool                                            isCharacterExcluded(size_t i) const;                                        //!< Is the character excluded
        bool                                            isCharacterResolved(size_t txIdx, size_t chIdx) const;                      //!< Returns whether the character is fully resolved (e.g., "A" or "1.32") or not (e.g., "AC" or "?")
        bool                                            isCharacterResolved(const std::string &tn, size_t chIdx) const;             //!< Returns whether the character is fully resolved (e.g., "A" or "1.32") or not (e.g., "AC" or "?")
        bool                                            isHomologyEstablished(void) const;                                          //!< Returns whether the homology of the characters has been established
        bool                                            isTaxonExcluded(size_t i) const;                                            //!< Is the taxon excluded
        bool                                            isTaxonExcluded(const std::string& s) const;                                //!< Is the taxon excluded
        void                                            removeExludedCharacters(void);                                              //!< Remove all the excluded characters
        void                                            restoreCharacter(size_t i);                                                 //!< Restore character
        void                                            restoreTaxon(size_t i);                                                     //!< Restore taxon
        void                                            restoreTaxon(const std::string& s);                                         //!< Restore taxon
        void                                            setFileName(const std::string &fn);                                         //!< Set the file name
        void                                            setFilePath(const std::string &fn);                                         //!< Set the file path
        void                                            setHomologyEstablished(bool tf);                                            //!< Set whether the homology of the characters has been established
        void                                            setTaxonName(const std::string& currentName, const std::string& newName);   //!< Change the name of a taxon
        void                                            show(std::ostream &out);                                                    //!< Show the entire content

        
    protected:
        // Utility functions
        size_t                                          indexOfTaxonWithName(const std::string& s) const;                                 //!< Get the index of the taxon
        
        // Member variables
        std::set<size_t>                                deletedTaxa;                                                                //!< Set of deleted taxa
        std::set<size_t>                                deletedCharacters;                                                          //!< Set of deleted characters
        std::string                                     fileName;                                                                   //!< The path/filename from where this matrix originated
        std::string                                     filePath;                                                                   //!< The path from where this matrix originated
        std::vector<std::string>                        sequenceNames;                                                              //!< names of the sequences
        size_t                                          sequenceLength;                                                             //!< The length of each sequence
        bool                                            homologyEstablished;                                                        //!< Whether the homology of the characters has been established
        
        std::map<std::string, ContinuousTaxonData >     taxonMap;
    };
    
    
}

#endif

