#ifndef AbstractTaxonData_H
#define AbstractTaxonData_H

#include "Cloneable.h"

#include <string>
#include <set>

namespace RevBayesCore {
    
    class CharacterState;

    /**
     * Abstract class for all taxon objects.
     *
     * This abstract class provides the base class for all taxon data objects.
     * A taxon data object contains a vector of character objects and defines additional
     * convenience functions to access the data.
     * Note that this class is a pure interface and thus contains only pure virtual functions!
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2013-04-15, version 1.0
     */
    class AbstractTaxonData : public Cloneable {
    
    public:
        virtual                                ~AbstractTaxonData(void) {}
        
        // methods of the Cloneable interface
        virtual AbstractTaxonData*              clone(void) const = 0;
        
        // AbstractTaxonData functions
        virtual AbstractTaxonData&              concatenate(const AbstractTaxonData &d) = 0;                        //!< Concatenate sequences
        virtual size_t                          getNumberOfCharacters(void) const = 0;                              //!< How many characters
        virtual double                          getPercentageMissing(void) const = 0;                               //!< Returns the percentage of missing data for this sequence
        virtual const std::string&              getTaxonName(void) const = 0;                                       //!< Return the name of the character vector
        virtual bool                            isCharacterResolved(size_t idx) const = 0;                          //!< Returns whether the character is fully resolved (e.g., "A" or "1.32") or not (e.g., "AC" or "?")
        virtual bool                            isSequenceMissing(void) const = 0;                                  //!< Returns whether the contains only missing data or has some actual observations
        virtual void                            removeCharacters(const std::set<size_t> &i) = 0;                    //!< Remove all the characters with a given index
        virtual void                            setTaxonName(const std::string &tn) = 0;                            //!< Set the taxon name
        virtual std::string                     getStringRepresentation(size_t idx) const = 0;
    
    protected:
        AbstractTaxonData() {}                                                                                      //!< Default constructor

    };

}

#endif
