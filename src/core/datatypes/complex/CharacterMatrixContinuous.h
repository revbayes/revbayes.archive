#ifndef CharacterMatrixContinuous_H
#define CharacterMatrixContinuous_H

#include <string>
#include <vector>

#include "Real.h"
#include "CharacterMatrix.h"



class CharacterMatrixContinuous : public CharacterMatrix {
    
public:
    CharacterMatrixContinuous(void);
    CharacterMatrixContinuous(const CharacterMatrixContinuous& cmc);
    CharacterMatrixContinuous(int nt, int nc);
    ~CharacterMatrixContinuous(void);
    
    
    // Basic utility functions you have to override
    CharacterMatrixContinuous*      clone(void) const;                                                  //!< Clone object
    const VectorString&             getClass(void) const;                                               //!< Get class
    void                            printValue(std::ostream& o) const;                                  //!< Print value for user
    std::string                     richInfo(void) const;                                               //!< Complete info about object
    
    // Container functions you have to override to allow vector of vectors representation
    void                            clear(void);                                                        //!< Clear
    void                            resize(const std::vector<size_t>& len);                             //!< Resize to new length vector
    void                            setLength(const std::vector<size_t>& len);                          //!< Reorganize container
    size_t                          size(void) const;                                                   //!< Get total number of elements
    
    void                            createMatrix(void);
    double                          getDoubleValue(int taxaId, int charId) { return matrix[taxaId][charId]; }
    int                             getNumStates(void) { return 0; }
    bool                            getIsAmbig(int taxaId, int charId);
    char                            getState(int taxaId, int charId) { return '*'; }
    unsigned                        getUnsignedValue(int taxaId, int charId) { return 0; }
    void                            setState(int taxaId, int charId, double x);
    
    void                            transpose(void);
    
private:
    
    RbObject*                       getDefaultElement(void) { return new Real(); }                              //!< Get default element for empty slots
    bool                            numFmt(int& numToLft, int& numToRht, std::string s) const;                  //!< Calculates the number of digits to the left and right of the decimal
    
    // Parser help functions you have to override
    DAGNode*                        getElement(const VectorInteger& index);                         //!< Get element or subcontainer for parser
    void                            setElement(const VectorNatural& index, DAGNode* var);           //!< Allow parser to set element
    
    void                            allocateMatrix(void);
    void                            freeMatrix(void);
    double**                        matrix;
};

#endif

