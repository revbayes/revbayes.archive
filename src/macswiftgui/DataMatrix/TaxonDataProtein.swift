//
//  TaxonDataProtein.swift
//
//  Created by John Huelsenbeck on 7/25/19.
//  Copyright © 2019 John Huelsenbeck. All rights reserved.
//
// This class holds protein sequence information for a taxon. The amino acid sequences are stored
// in a bitvector, which this class ultimately inherits from. 20 bits are used to
// store the amino acid information. A 21st bit acts as a flag indicating whether the
// amino acid is a gap.

import Foundation



class TaxonDataProtein : TaxonData, NSCopying {

    // default initializer
    init() {
    
        super.init(numStates: 20, numFlags: 1)

        // initialize to type Protein
        dataType = DataType.Protein
    }
    
    // initialize from the taxon name and a string of nucleotides characters
    init(name: String, aminoAcidString: String) throws {
    
        // initialize the object (the bitvector will be empty)
        super.init(numStates: 20, numFlags: 1)

        // initialize to type Protein
        dataType = DataType.Protein

        super.taxonName = name
        
        // process the string, removing spaces and making certain the characters are uppercase
        var potentialAminoAcidString : String = aminoAcidString.uppercased()
        potentialAminoAcidString = potentialAminoAcidString.replacingOccurrences(of: " ", with: "")
        
        // check that the nucleotide sequence is valid
        guard isValidCharacterStateString(potentialCharacterStateString:potentialAminoAcidString) == true else {
            throw TaxonDataError.incorrectDataFormat
        }
        
        // the string containing the amino acid sequence is good, so we can now initialize it
        numCharacters = potentialAminoAcidString.count
        for c in potentialAminoAcidString {
            addCharacter(characterState: c)
        }
    }

    // initialize from serialized data
    required init(from decoder: Decoder) throws {
    
        do {
            try super.init(from: decoder)
        }
        catch {
            throw TaxonDataError.decodingError
        }
    }

    func copy(with zone: NSZone? = nil) -> Any {
    
        do {
            let copy = try TaxonDataProtein(name: taxonName, aminoAcidString: self.characterDataString())
            return copy
        }
        catch {
            return TaxonDataProtein()
        }
    }

    // MARK: -

    override func addCharacter(characterState: Character) {

        let aaCode: Int = encodeAminoAcid(aaChar: characterState)
        var aas: [Bool] = [false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false]
        encodeAminoAcidVector(aaCode: aaCode, aaVector: &aas)
        let isGapChar: Bool = isGap(dataChar: characterState)
        for i in 0..<20 {
            self += aas[i]
            }
        self += isGapChar
    }
    
    // return a String holding the amino acid sequence for this taxon
    override func characterDataString() -> String {
    
        var str: String = ""
        for i in 0..<numCharacters {
            let offset = i * 21
            let isGapChar: Bool = self[offset + 20]
            if isGapChar == true {
                str += "-"
            }
            else {
                var aas: [Bool] = [false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false]
                for j in 0..<20 {
                    aas[j] = self[offset+j]
                }
                str += String(decodeAminoAcidVector(aaVector:aas))
            }
        }
        return str
    }

    // decode a vector of 20 Bool variables, returning the amino acid using the IUPAC standard
    func decodeAminoAcidVector(aaVector: [Bool]) -> Character {
    
        var numOn: Int = 0
        for i in 0..<20 {
            if aaVector[i] == true {
                numOn += 1
            }
        }
        
        if numOn == 20 {
            return "?"
        }
        else if numOn == 1 {
            if aaVector[0] == true {
                return "G"
            }
            else if aaVector[1] == true {
                return "A"
            }
            else if aaVector[2] == true {
                return "L"
            }
            else if aaVector[3] == true {
                return "M"
            }
            else if aaVector[4] == true {
                return "F"
            }
            else if aaVector[5] == true {
                return "W"
            }
            else if aaVector[6] == true {
                return "K"
            }
            else if aaVector[7] == true {
                return "Q"
            }
            else if aaVector[8] == true {
                return "E"
            }
            else if aaVector[9] == true {
                return "S"
            }
            else if aaVector[10] == true {
                return "P"
            }
            else if aaVector[11] == true {
                return "V"
            }
            else if aaVector[12] == true {
                return "I"
            }
            else if aaVector[13] == true {
                return "C"
            }
            else if aaVector[14] == true {
                return "Y"
            }
            else if aaVector[15] == true {
                return "H"
            }
            else if aaVector[16] == true {
                return "R"
            }
            else if aaVector[17] == true {
                return "N"
            }
            else if aaVector[18] == true {
                return "D"
            }
            else {
                return "T"
            }
        }
        else {
            return " "
        }
    }

    // return the  bit vector for the IUPAC nucleotide character
    func encodeAminoAcid(aaChar: Character) -> Int {

        switch aaChar {
            case "G":
                return 1
            case "A":
                return 2
            case "L":
                return 4
            case "M":
                return 8
            case "F":
                return 16
            case "W":
                return 32
            case "K":
                return 64
            case "Q":
                return 128
            case "E":
                return 256
            case "S":
                return 512
            case "P":
                return 1024
            case "V":
                return 2048
            case "I":
                return 4096
            case "C":
                return 8192
            case "Y":
                return 16384
            case "H":
                return 32768
            case "R":
                return 65536
            case "N":
                return 131072
            case "D":
                return 262144
            case "T":
                return 524288
            case "?":
                return 1048575
            case "-":
                return 1048575
           default:
                return 0
        }
    }

    // fill out an array of 20 Bools that represent the IUPAC amino acid code
    func encodeAminoAcidVector(aaCode: Int, aaVector: inout [Bool]) {

        switch aaCode {
            case 1:
                aaVector[0] = true
            case 2:
                aaVector[1] = true
            case 4:
                aaVector[2] = true
            case 8:
                aaVector[3] = true
            case 16:
                aaVector[4] = true
            case 32:
                aaVector[5] = true
            case 64:
                aaVector[6] = true
            case 128:
                aaVector[7] = true
            case 256:
                aaVector[8] = true
            case 512:
                aaVector[9] = true
            case 1024:
                aaVector[10] = true
            case 2048:
                aaVector[11] = true
            case 4096:
                aaVector[12] = true
            case 8192:
                aaVector[13] = true
            case 16384:
                aaVector[14] = true
            case 32768:
                aaVector[15] = true
            case 65536:
                aaVector[16] = true
            case 131072:
                aaVector[17] = true
            case 262144:
                aaVector[18] = true
            case 524288:
                aaVector[19] = true
            case 1048575:
                for i in 0..<20 {
                    aaVector[i] = true
                }
            default:
                for i in 0..<20 {
                    aaVector[i] = false
                }
            }
        }

    // check that the state is valid
    override func isValidCharacterState(potentialCharacterState: Character) -> Bool {

        let validAaCharacters = "GALMFWKQESPVICYHRNDT?-"
        if validAaCharacters.contains(potentialCharacterState) == false {
            return false
        }
        return true
    }

    override func setCharacterState(characterIdx: Int, characterState: Character) {

        let aaCode: Int = encodeAminoAcid(aaChar:characterState)
        var aas: [Bool] = [false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false]
        encodeAminoAcidVector(aaCode: aaCode, aaVector: &aas)
        let isGapChar: Bool = isGap(dataChar:characterState)

        let offset : Int = characterIdx * 21
        for i in 0..<20 {
            self[offset + i] = aas[i]
            }
        self[offset + 20] = isGapChar
    }

}
