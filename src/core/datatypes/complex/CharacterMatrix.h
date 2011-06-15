#ifndef CharacterMatrix_H
#define CharacterMatrix_H

#include "Matrix.h"

#include <string>
#include <vector>


class CharacterMatrix : public Matrix {

public:
	                                CharacterMatrix(void);
	                                CharacterMatrix(int nt, int nc);
    
    
    // Basic utility functions you have to override
    virtual CharacterMatrix*        clone(void) const = 0;                                              //!< Clone object
    virtual const VectorString&     getClass(void) const;                                               //!< Get class
    virtual void                    printValue(std::ostream& o) const = 0;                              //!< Print value for user
    virtual std::string             richInfo(void) const = 0;                                           //!< Complete info about object
    
    // Container functions you have to override to allow vector of vectors representation
    virtual void                    clear(void) = 0;                                                    //!< Clear
    virtual void                    resize(const std::vector<size_t>& len) = 0;                         //!< Resize to new length vector
    virtual void                    setLength(const std::vector<size_t>& len) = 0;                      //!< Reorganize container
    virtual size_t                  size(void) const = 0;                                               //!< Get total number of elements
    
    // Matrix function you have to override
    virtual void                    transpose(void) = 0;                                                //!< Transpose matrix
    
        void                        addTaxonName(std::string tn) { taxonNames.push_back(tn); }
        void                        excludeCharacter(int i) { isCharacterExcluded[i] = true; }
        void                        excludeTaxon(int i) { isTaxonExcluded[i] = true; }
		std::string                 getDataType(void) { return dataType; }
        int                         getIndexOfTaxonNamed(std::string s);
        bool                        getIsCharacterExcluded(int i) { return isCharacterExcluded[i]; }
		bool                        getIsDiscrete(void) { return isDiscrete; }
        bool                        getIsTaxonExcluded(int i) { return isTaxonExcluded[i]; }
		virtual int                 getNumStates(void) = 0;
		virtual double              getDoubleValue(int taxaId, int charId) = 0;
		virtual bool                getIsAmbig(int taxaId, int charId) = 0;
		virtual char                getState(int taxaId, int charId) = 0;
		virtual unsigned            getUnsignedValue(int taxaId, int charId) = 0;
		int                         getNumCharacters(void) { return numCharacters; }
		int                         getNumTaxa(void) { return numTaxa; }
		std::string                 getName(void) { return name; }
        std::vector<std::string>&   getTaxa(void) { return taxonNames; }
        std::string                 getTaxonIndexed(int i) { return taxonNames[i]; }
		void                        setNumCharacters(int x) { numCharacters = x; }
		void                        setNumTaxa(int x) { numTaxa = x; }
		void                        setName(std::string s) { name = s; }

protected:
    
    // Parser help functions you have to override
    virtual DAGNode*                getElement(const VectorInteger& index) = 0;                         //!< Get element or subcontainer for parser
    virtual void                    setElement(const VectorNatural& index, DAGNode* var) = 0;           //!< Allow parser to set element
    
		int                         numTaxa;
		int                         numCharacters;
		bool                        isDiscrete;
		std::string                 dataType;
		std::string                 name;
		std::string                 filePathName;
        std::vector<std::string>    taxonNames;
        std::vector<bool>           isTaxonExcluded;
        std::vector<bool>           isCharacterExcluded;
};

#endif

