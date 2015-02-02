#ifndef TimeAndDate_H
#define TimeAndDate_H

#include <iostream>

namespace RevBayesCore {
    
    /**
     * @brief Time and date object for dates of taxon samples.
     *
     * This class provides the time and/or date information for a taxon.
     * Some convenience methods are provided.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-18, version 1.0
     */
    class TimeAndDate {
        
    public:
        TimeAndDate();                                                          //!< Default constructor
        
        bool                                operator==(const TimeAndDate &t) const;           //!< Equals operator
        bool                                operator!=(const TimeAndDate &t) const;           //!< Not-equals operator

        // public functions
        double                              getDaysAD(void) const;              //!< Get the number of days since counting.
        double                              getMya(void) const;                 //!< Get the number of million years ago.
        double                              getYearsAD(void) const;             //!< Get the number of years since counting.
        
    private:
        
    };
    
    std::ostream& operator<<(std::ostream& o, const TimeAndDate& x) ;

}

#endif
