#ifndef CharacterMatrixDiscrete_H
#define CharacterMatrixDiscrete_H

#include <string>
#include <vector>
#include "CharacterMatrix.h"



class CharacterMatrixDiscrete : public CharacterMatrix {

    public:
									CharacterMatrixDiscrete(int nt, int nc, std::string dt);
									CharacterMatrixDiscrete(int nt, int nc, int ns, std::string dt);
		                           ~CharacterMatrixDiscrete(void);
	    void                        addAminoAcid(int taxaId, int charId, char charCode);
        void                        addNucleotide(int taxaId, int charId, char charCode);
        bool                        checkMatrix(void);
		double                      getDoubleValue(int taxaId, int charId) { return 0.0; }
		int                         getNumStates(void) { return numStates; }
		bool                        getIsAmbig(int taxaId, int charId);
		char                        getState(int taxaId, int charId);
		unsigned                    getUnsignedValue(int taxaId, int charId);
		void                        print(void);
		void                        setState(int taxaId, int charId, int stateId);
		void                        setAminoAcid(int taxaId, int charId, char charCode);
		void                        setNucleotide(int taxaId, int charId, char charCode);
		
   private:
		void                        allocateMatrix(void);
        void                        createAminoAcidMatrix(void);
        void                        createNucleotideMatrix(void);
        void                        createStandardMatrix(int ns);
		void                        freeMatrix(void);
		char                        getNucleotideChar(std::vector<bool>& x);
		int                         nucId(char nuc);
        int                         numStates;
		std::vector<bool>**         matrix;
};

#endif

