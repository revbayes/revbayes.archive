#ifndef Taxon_H
#define Taxon_H

#include "TimeAndDate.h"

#include <string>

namespace RevBayesCore {
    
    
    /**
     * @brief Taxon object representing an individual.
     *
     * This class represents a taxon.
     * A taxon simply consists of a taxon name and a date when this taxon has been observed.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-18, version 1.0
     */
    class Taxon {
        
    public:
                                            Taxon(void);                                            //!< Default constructor required by revlanguage use
                                            Taxon(const std::string &n, const std::string &sn="");  //!< Regular constructor
        virtual                             ~Taxon() {}
        
        bool                                operator==(const Taxon &t) const;           //!< Equals operators
        bool                                operator!=(const Taxon &t) const;           //!< Not-quals operators
        bool                                operator<(const Taxon &t) const;            //!< Less-than operators
        bool                                operator<=(const Taxon &t) const;           //!< Less-than operators
        
        // public methods
        const TimeAndDate&                  getDate(void) const;                        //!< Get the date info for this taxon.
        const std::string&                  getName(void) const;                        //!< Get the name for this taxon.
        const std::string&                  getSpeciesName(void) const;                 //!< Get the name of the species.
        void                                setDate(const TimeAndDate &d);              //!< Set the date info.
        void                                setName(const std::string &n);              //!< Set the name.
        void                                setSpeciesName(const std::string &n);       //!< Set the name of the species.
        
    private:
        
        // private members
        TimeAndDate                         date;
        std::string                         name;
        std::string                         speciesName;
    
    };
	
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Taxon& x);                                         //!< Overloaded output operator
}

#endif
