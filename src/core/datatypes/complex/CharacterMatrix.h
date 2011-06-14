#ifndef CharacterMatrix_H
#define CharacterMatrix_H

#include <string>
#include <vector>


class CharacterMatrix {

    public:
	                                CharacterMatrix(void);
	                                CharacterMatrix(int nt, int nc);
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
		virtual void                print(void) = 0;
		void                        setNumCharacters(int x) { numCharacters = x; }
		void                        setNumTaxa(int x) { numTaxa = x; }
		void                        setName(std::string s) { name = s; }

   protected:
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

