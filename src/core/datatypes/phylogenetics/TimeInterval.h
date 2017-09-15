#ifndef TimeRange_H
#define TimeRange_H

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
    class TimeInterval {
        
    public:
        TimeInterval();                                                          //!< Default constructor
        
        bool                                operator==(const TimeInterval &t) const;           //!< Equals operator
        bool                                operator!=(const TimeInterval &t) const;           //!< Not-equals operator

        // public functions
        double                              getStart(void) const;           //!< Get the number of days since counting.
        double                              getEnd(void) const;             //!< Get the number of million years ago.
        void                                setStart(double);               //!< Get the number of days since counting.
        void                                setEnd(double);                 //!< Get the number of million years ago.
        
    private:
        double  start;
        double  end;
        
    };
    
    std::ostream& operator<<(std::ostream& o, const TimeInterval& x) ;

}

#endif
