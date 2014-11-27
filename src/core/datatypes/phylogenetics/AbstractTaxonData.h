#ifndef AbstractTaxonData_H
#define AbstractTaxonData_H

#include <string>

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
    class AbstractTaxonData {
    
    public:
        virtual                                ~AbstractTaxonData(void) {}
        
        // AbstractTaxonData functions
        virtual AbstractTaxonData&              add(const AbstractTaxonData &d) = 0;                                //!< Addition operator used for example in '+=' statements
        virtual size_t                          getNumberOfCharacters(void) const = 0;                              //!< How many characters
        virtual const std::string&              getTaxonName(void) const = 0;                                       //!< Return the name of the character vector
        virtual void                            setTaxonName(const std::string &tn) = 0;                            //!< Set the taxon name
    
    protected:
        AbstractTaxonData() {}                                                                                      //!< Default constructor

    };

}

#endif
