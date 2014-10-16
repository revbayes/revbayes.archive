#ifndef RbHelpEntry_H
#define RbHelpEntry_H

#include <string>
#include <vector>

namespace RevBayesCore {
    
    
    /**
     * \brief The help entry base class.
     *
     * A help entry for a type provides all the help information available on this type, e.g.,
     * about member methods.
     *
     * \copyright (c) Copyright 2009-2013 (GPL version 3)
     * \author The RevBayes Development Core Team (Johan Dunfalk & Sebastian Hoehna)
     * \since Version 1.0, 2014-09-15
     *
     */
    class RbHelpEntry {
        
    public:
        
        virtual                                     ~RbHelpEntry() {}
        
        // getter
        const std::vector<std::string>&             getDescription(void) const;
        const std::string&                          getName(void) const;
        const std::string&                          getTitle(void) const;
        
        // setters
        void                                        setDescription(const std::vector<std::string> &d);
        void                                        setName(const std::string &n);
        void                                        setTitle(const std::string &t);
        
        
    private:
        
        std::vector<std::string>                    description;
        std::string                                 name;
        std::string                                 title;
        
    };
    
    
}

#endif

