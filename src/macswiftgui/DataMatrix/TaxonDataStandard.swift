//
//  TaxonDataStandard.swift
//
//  Created by John Huelsenbeck on 8/5/19.
//  Copyright © 2019 John Huelsenbeck. All rights reserved.
//
// This class holds Standard (i.e., morphological) character data for a taxon. The
// characters are stored as a bitvector, which this class ultimately inherits from. The
// number of bits for each character, however, is flexible and determined by the number
// of possible states.

import Foundation


class TaxonDataStandard : TaxonData, NSCopying {

    internal var possibleCharacterStates : String
    internal var missingCharacter : String
    internal var validCharacters : String

    // MARK: -
    
    private enum CodingKeys: String, CodingKey {
    
        case possibleCharacterStates
        case missingCharacter
        case validCharacters
    }

    // MARK: -
    
    // initialize from the taxon name and a string of nucleotides characters
    init(name: String, charcterDataString: String, possibleStates: String, missingCharacter: String) throws {
        
        self.possibleCharacterStates = possibleStates
        self.missingCharacter = missingCharacter
        self.validCharacters = possibleStates + missingCharacter

        super.init(numStates: validCharacters.count, numFlags: 1)

        // check that the states are valid
        guard checkStandardStates(possibleStates: possibleStates, missingCharacter: missingCharacter) == true else {
            throw TaxonDataError.incorrectStateInformation
        }

        // initialize to type Standard
        dataType = DataType.Standard

        super.taxonName = name
        
        // process the string, removing spaces. Note the states are case-sensitive
        let potentialACharacterStateString : String = charcterDataString.replacingOccurrences(of: " ", with: "")
        
        // check that the character data are valid
        guard isValidCharacterStateString(potentialCharacterStateString:potentialACharacterStateString) == true else {
            throw TaxonDataError.incorrectDataFormat
        }
        
        // the string containing the character data is good, so we can now initialize it
        numCharacters = potentialACharacterStateString.count
        for c in potentialACharacterStateString {
            addCharacter(characterState: c)
        }
    }

    // initialize from serialized data
    required init(from decoder: Decoder) throws {
    
        do {
            let container = try decoder.container(keyedBy: CodingKeys.self)
            self.possibleCharacterStates = try container.decode(String.self, forKey: .possibleCharacterStates)
            self.missingCharacter        = try container.decode(String.self, forKey: .missingCharacter)
            self.validCharacters         = try container.decode(String.self, forKey: .validCharacters)
            try super.init(from: container.superDecoder())
        }
        catch {
            throw TaxonDataError.decodingError
        }
    }

    override func encode(to encoder: Encoder) throws {

        do {
            var container = encoder.container(keyedBy: CodingKeys.self)
            try container.encode(possibleCharacterStates, forKey: .possibleCharacterStates)
            try container.encode(missingCharacter,        forKey: .missingCharacter)
            try container.encode(validCharacters,         forKey: .validCharacters)
            try super.encode(to:container.superEncoder())
        }
        catch {
            throw TaxonDataError.encodingError
        }
    }

    func copy(with zone: NSZone? = nil) -> Any {
    
        do {
            let copy = try TaxonDataStandard(name: taxonName, charcterDataString: self.characterDataString(), possibleStates: self.possibleCharacterStates, missingCharacter: self.missingCharacter)
            return copy
        }
        catch {
            return TaxonDataProtein()
        }
    }

    // MARK: -

    override func addCharacter(characterState: Character) {
    
        // set up an array indicating which state is observed
        var st : [Bool] = Array(repeating: false, count: numStates)
        
        // check if the character is represents missing data
        let isMissing : Bool = missingCharacter.contains(characterState);
        
        // fill in the vector of bools for the states
        var i : Int = 0
        for c in possibleCharacterStates {
            if c == characterState || isMissing == true {
                st[i] = true
            }
            i += 1
        }
        for c in missingCharacter {
            if c == characterState {
                st[i] = true
            }
            i += 1
        }
        
        // initialize the bitvector appropriately
        for i in 0..<numStates {
            self += st[i]
            }

        // fill in the last bit, which flags whether the state is missing
        self += isMissing
    }

    // return a String holding the character data for this taxon
    override func characterDataString() -> String {
    
        var st : [Bool] = Array(repeating: false, count: numStates+1)
        var str: String = ""
        for c in 0..<numCharacters {
        
            let offset = c * (numStates + 1)
            for i in 0..<(numStates+1) {
                st[i] = self[offset + i]
            }
            
            // check if the character is missing data
            let isMissing : Bool = st[numStates]
            
            // figure out the character
            if isMissing == false {
                var i : Int = 0
                for s in possibleCharacterStates {
                    if st[i] == true {
                        str += String(s)
                        break
                    }
                    i += 1
                }
            }
            else {
                var i : Int = possibleCharacterStates.count
                for s in missingCharacter {
                    if st[i] == true {
                        str += String(s)
                        break
                    }
                    i += 1
                }
            }

        }
        return str
    }

    func checkStandardStates(possibleStates: String, missingCharacter: String) -> Bool {
    
        // check that there is only one missing character
        if missingCharacter.count != 1 {
            return false
        }
        
        let checkedStates = possibleStates + missingCharacter
        
        // check for duplicates
        for c in checkedStates {
            if c == " " {
                return false
            }
            var n : Int = 0
            for d in checkedStates {
                if c == d {
                    n += 1
                }
            }
            if n != 1 {
                return false
            }
        }
        return true
    }

    func decodeCharacterStateVector(stateVector: [Bool]) {
    
        // count the number of on bits
        var numOn : Int = 0
        for b in stateVector {
            if b == true {
                numOn += 1
            }
        }
        
        if numOn == 1 {
        
        }
        else
        {
        
        }
    }

    override func isSameStateSpace(taxonData: TaxonData) -> Bool {
    
        if taxonData is TaxonDataStandard {
            let td = taxonData as! TaxonDataStandard
            if td.possibleCharacterStates != self.possibleCharacterStates {
                return false
            }
            if td.missingCharacter != self.missingCharacter {
                return false
            }
            return true
        }
        else {
            return false
        }
    }

    // check that the state is valid
    override func isValidCharacterState(potentialCharacterState: Character) -> Bool {

        if validCharacters.contains(potentialCharacterState) == false {
            return false
        }
        return true
    }

}
