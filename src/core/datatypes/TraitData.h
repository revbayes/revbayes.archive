/**
 * @file
 * This file contains the declaration of 
 *
 * @brief Declaration of the TraitData
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: TimeTree.h 1651 2012-07-05 14:47:08Z hoehna $
 */

#ifndef TraitData_H
#define TraitData_H


#include <vector>
#include <string>

namespace RevBayesCore {
    
    class TraitData {
        
    public:
        TraitData(void);                                                                                                     //!< Default constructor
//        TraitData(const TraitData& t);                                                                                        //!< Copy constructor
        virtual                                    ~TraitData(void);                                                                                                        //!< Destructor
        
//        TraitData&                                   operator=(const TraitData& t);
        
        // Basic utility functions
        TraitData*                                   clone(void) const;                                                      //!< Clone object
        
        // TraitData functions
        void                                        addParameterName(const std::string &n);
        void                                        addParameterValue(const std::string &n, const std::string &p, double v);
        void                                        addTaxon(const std::string &n);

    protected:
        
    private:
        
        std::vector<std::string>                    taxonNames;
        std::vector<std::string>                    paramNames;
        std::vector<std::vector<double> >           values;
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const TraitData& x);                                         //!< Overloaded output operator
    
}

#endif

