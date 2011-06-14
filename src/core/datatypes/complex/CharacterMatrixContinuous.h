#ifndef CharacterMatrixContinuous_H
#define CharacterMatrixContinuous_H

#include <string>
#include <vector>
#include "CharacterMatrix.h"



class CharacterMatrixContinuous : public CharacterMatrix {
    
public:
    CharacterMatrixContinuous(void);
    CharacterMatrixContinuous(int nt, int nc);
    ~CharacterMatrixContinuous(void);
    void                        createMatrix(void);
    double                      getDoubleValue(int taxaId, int charId) { return matrix[taxaId][charId]; }
    int                         getNumStates(void) { return 0; }
    bool                        getIsAmbig(int taxaId, int charId);
    char                        getState(int taxaId, int charId) { return '*'; }
    unsigned                    getUnsignedValue(int taxaId, int charId) { return 0; }
    void                        print(void);
    void                        setState(int taxaId, int charId, double x);
    
private:
    void                        allocateMatrix(void);
    void                        freeMatrix(void);
    double**                    matrix;
};

#endif

