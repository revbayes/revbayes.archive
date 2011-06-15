#ifndef CharacterMatrixDiscrete_H
#define CharacterMatrixDiscrete_H

#include <string>
#include <vector>
#include "CharacterMatrix.h"
#include "Real.h"



class CharacterMatrixDiscrete : public CharacterMatrix {

public:
    CharacterMatrixDiscrete(const CharacterMatrixDiscrete& cmc);
									CharacterMatrixDiscrete(int nt, int nc, std::string dt);
									CharacterMatrixDiscrete(int nt, int nc, int ns, std::string dt);
		                           ~CharacterMatrixDiscrete(void);
    
    
    // Basic utility functions you have to override
    CharacterMatrixDiscrete*        clone(void) const;                                                  //!< Clone object
    const VectorString&             getClass(void) const;                                               //!< Get class
    void                            printValue(std::ostream& o) const;                                  //!< Print value for user
    std::string                     richInfo(void) const;                                               //!< Complete info about object
    
    // Container functions you have to override to allow vector of vectors representation
    void                            clear(void);                                                        //!< Clear
    void                            resize(const std::vector<size_t>& len);                             //!< Resize to new length vector
    void                            setLength(const std::vector<size_t>& len);                          //!< Reorganize container
    size_t                          size(void) const;                                                   //!< Get total number of elements
    
    // Matrix function you have to override
    void                            transpose(void);                                                //!< Transpose matrix
    
	    void                        addAminoAcid(int taxaId, int charId, char charCode);
        void                        addNucleotide(int taxaId, int charId, char charCode);
        bool                        checkMatrix(void);
		double                      getDoubleValue(int taxaId, int charId) { return 0.0; }
		int                         getNumStates(void) { return numStates; }
		bool                        getIsAmbig(int taxaId, int charId);
		char                        getState(int taxaId, int charId);
		unsigned                    getUnsignedValue(int taxaId, int charId);
		void                        setState(int taxaId, int charId, int stateId);
		void                        setAminoAcid(int taxaId, int charId, char charCode);
		void                        setNucleotide(int taxaId, int charId, char charCode);
		
   private:
    
    RbObject*                       getDefaultElement(void) { return new Real(); }                              //!< Get default element for empty slots
    bool                            numFmt(int& numToLft, int& numToRht, std::string s) const;                  //!< Calculates the number of digits to the left and right of the decimal
    
    // Parser help functions you have to override
    DAGNode*                        getElement(const VectorInteger& index);                         //!< Get element or subcontainer for parser
    void                            setElement(const VectorNatural& index, DAGNode* var);           //!< Allow parser to set element
    
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

