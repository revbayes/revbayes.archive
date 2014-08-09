#ifndef FunctionTable_H
#define FunctionTable_H

#include "RevObject.h"
#include "RevPtr.h"

#include <map>
#include <ostream>
#include <string>
#include <vector>

namespace RevLanguage {
    
    class Argument;
    class ArgumentRule;
    class Function;

    /**
     * @brief FunctionTable: A multimap from function names to functions
     *
     * FunctionTable is used to hold functions in Workspace and Environment (frame)
     * objects. It holds the functions, which it owns, in a std::multimap, which it
     * is derived from. Function tables can be nested; each table defers
     * calls to its parent(s) when the task cannot be solved locally.
     *
     */
    class FunctionTable : public std::multimap<std::string, Function*> {
        
    public:

        FunctionTable(FunctionTable* parent = NULL);                                                                                    //!< Empty table
        FunctionTable(const FunctionTable& x);                                                                                          //!< Copy constructor
        virtual                                 ~FunctionTable();                                                                       //!< Delete functions

        // Assignment operator
        FunctionTable&                          operator=(const FunctionTable& x);                                                      //!< Assignment operator 

        // Basic utility functions
        virtual FunctionTable*                  clone(void) const;                                                                      //!< Clone object
        void                                    printValue(std::ostream& o, bool env) const;                                            //!< Print table for user

        // FunctionTable functions
        virtual void                            addFunction(const std::string name, Function *func);                                    //!< Add function
        void                                    clear(void);                                                                            //!< Clear table
        RevPtr<Variable>                        executeFunction(const std::string&           name,
                                                                const std::vector<Argument>& args);                                     //!< Evaluate function (once)
        bool                                    existsFunction(const std::string &name) const;                                          //!< Does this table contain a function with given name?
        void                                    eraseFunction(const std::string& name);                                                 //!< Erase a function (all versions)
        std::vector<Function*>                  findFunctions(const std::string& name) const;                                           //!< Return functions matching name
        const Function&                         getFirstFunction(const std::string& name);                                              //!< Get function
        const Function&                         getFunction(const std::string& name);                                                   //!< Get function
        Function&                               getFunction(const std::string& name, const std::vector<Argument>& args);                //!< Get function
        std::multimap<std::string, Function*>   getTableCopy(bool env) const;                                                           //!< Get a copy of function table
        bool                                    isDistinctFormal(const ArgumentRules& x, const ArgumentRules& y) const;                 //!< Are formals unique?
        bool                                    isProcedure(const std::string& fxnName) const;                                          //!< Is 'fxnName' a procedure?
        void                                    setParentTable(const FunctionTable* ft) { parentTable = ft; }                           //!< Set parent table

    protected:
        Function&                               findFunction(const std::string&           name,
                                                             const std::vector<Argument>& args);                                        //!< Find function, process args
    
        // Member variables
        const FunctionTable*                    parentTable;                                                                            //!< Enclosing table

};
    
}

#endif

