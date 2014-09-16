#ifndef RbHelpSystem_H
#define RbHelpSystem_H

#include "RbHelpFunction.h"

#include <string>
#include <map>

namespace RevBayesCore {
    
    
    /**
     * \brief The class managing our help system.
     *
     * Our help system consists of several xml-files. Here we load in the files.
     * The help system will provide access to other classes to the help documentation.
     *
     * \copyright (c) Copyright 2009-2013 (GPL version 3)
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2014-09-15
     *
     */
    class RbHelpSystem {
        
    public:
        
        std::string                                 getHelp(const std::string &qs, size_t columnWidth);                     //!< Format the help information for printing to the terminal
        bool                                        isHelpAvailableForQuery(const std::string&query);
        
        static RbHelpSystem&                        getHelpSystem(void)                                                     //!< Return a reference to the singleton help
        {
            static RbHelpSystem globalHelpInstance = RbHelpSystem();
            return globalHelpInstance;
        }
        
    private:
        RbHelpSystem(void);                                                                     //!< Default constructor
        //    Help(const Help&); //!< Copy constructor (hidden away as this is a singleton class)
        //    Help& operator=(const Help&); //!< Assignment operator (hidden away as this is a singleton class)
        
        void                                        initializeHelp(const std::string &helpDir);                             //!< Initialize the help from an XML file
        
        
        std::map<std::string, RbHelpFunction>       helpForFunctions;
        
    };
    
    
    
}

#endif

