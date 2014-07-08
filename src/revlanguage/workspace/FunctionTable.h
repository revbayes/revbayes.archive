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
     * @brief FunctionTable: A multimap from function call names to functions
     *
     * FunctionTable is used to hold functions in Workspace and Environment (frame)
     * objects. It holds the functions, which it owns, in a std::multimap data
     * member variable called table. Function tables can be nested; each table defers
     * calls to its parent(s) when the task cannot be solved locally.
     *
     */
    class FunctionTable {

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
        const Function&                         getFunction(const std::string& name);                                                   //!< Get function (a copy)
        Function&                               getFunction(const std::string& name, const std::vector<Argument>& args);                //!< Get function (a copy)
        std::multimap<std::string, Function*>   getTableCopy(bool env) const;                                                           //!< Get a copy of function table
        bool                                    isDistinctFormal(const ArgumentRules& x, const ArgumentRules& y) const;                 //!< Are formals unique?
        void                                    setParentTable(const FunctionTable* ft) { parentTable = ft; }                           //!< Set parent table

    protected:
        Function&                               findFunction(const std::string&           name,
                                                             const std::vector<Argument>& args);                                        //!< Find function, process args
    
        // Member variables
        std::multimap<std::string, Function*>   table;                                                                                  //!< Table of functions
        const FunctionTable*                    parentTable;                                                                            //!< Enclosing table

};
    
}

#endif

