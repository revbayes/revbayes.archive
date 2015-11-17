#ifndef Trace_H
#define Trace_H

#include <string>
#include <vector>

namespace RevBayesCore {
    
#define FAILED 0
#define PASSED 1
#define NOT_CHECKED 2
    
    class Trace {
        
    public:
        
        Trace(void);
        
        virtual                     ~Trace(void) {}
        
        bool                        operator==(const Trace &t) const                { return this == &t; }
        bool                        operator!=(const Trace &t) const                { return !this->operator==( t ); }
        bool                        operator<(const Trace &t) const                 { return this < &t; }
        
        // pure virtual methods
        virtual void                addValueFromString(const std::string &s) = 0;
        virtual Trace*              clone(void) const = 0;                                              //!< Clone object
        virtual size_t              size() const = 0;
        
        
        // getters and setters
        std::string                 getFileName()                                   { return fileName; }
        const std::string&          getParameterName() const                        { return parmName; }
        
        void                        setFileName(std::string fn)                     { fileName = fn; }
        void                        setParameterName(std::string pm)                { parmName = pm; }
        
    protected:
        
        std::string                 fileName;
        std::string                 parmName;
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Trace& x);                                //!< Overloaded output operator
    
}

#endif
