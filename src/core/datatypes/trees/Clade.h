#ifndef Clade_H
#define Clade_H

#include <vector>
#include <string>

namespace RevBayesCore {
    
    /**
     * Object describing clades.
     *
     * A clade is simply a container of the taxon names.
     * Hence, this class just provides some convenience methods but could be considered as
     * a string-vector.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2013-03-10, version 1.0
     */
    class Clade  {
        
    public:
                                                    Clade(void);                                            //! Default constructor: empty clade of age 0.0
                                                    Clade(const std::vector<std::string> &n, double a);     //!< Default constructor with optional index
        
        std::vector<std::string>::const_iterator    begin(void) const;
        std::vector<std::string>::iterator          begin(void);
        std::vector<std::string>::const_iterator    end(void) const;
        std::vector<std::string>::iterator          end(void);
        // overloaded operators
        bool                                        operator==(const Clade &t) const;
        bool                                        operator!=(const Clade &t) const;
        bool                                        operator<(const Clade &t) const;
        bool                                        operator<=(const Clade &t) const;

        
        // Basic utility functions
        Clade*                                      clone(void) const;                                                      //!< Clone object
        
        // public methods
        double                                      getAge(void) const;                                                     //!< Get the age of this clade.
        const std::string&                          getTaxonName(size_t i) const;                                           //!< Get a single taxon name.
        const std::vector<std::string>&             getTaxonNames(void) const;                                              //!< Get the taxon names.
        size_t                                      size(void) const;                                                       //!< Get the number of taxa.
        std::string                                 toString(void) const;                                                   //!< Convert this value into a string.
        
        // public TopologyNode functions
        
    private: 
        
        // members
        double                                      age;
        std::vector<std::string>                    taxonNames;
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Clade& x);                                         //!< Overloaded output operator

}

#endif
