//
//  TaxonData.swift
//
//  Created by John Huelsenbeck on 7/11/19.
//  Copyright © 2019 John Huelsenbeck. All rights reserved.
//
// This class holds DNA sequence information for a taxon. The DNA sequences are stored
// in a bitvector, which this class ultimately inherits from. Four bits are used to
// store the nucleotide information. A fifth bit acts as a flag indicating whether the
// nucleotide is a gap.

import Foundation



class TaxonDataDna : TaxonData, NSCopying {

    // default initializer
    init () {
    
        super.init(numStates: 4, numFlags: 1)

        // initialize to type DNA
        dataType = DataType.DNA
    }
    
    // initialize from the taxon name and a string of nucleotides characters
    init(name: String, nucleotideString: String) throws {
    
        // initialize the object (the bitvector will be empty)
        super.init(numStates: 4, numFlags: 1)

        // initialize to type DNA
        dataType = DataType.DNA

        // add the name
        super.taxonName = name
        
        // process the string, removing spaces and making certain the characters are uppercase
        var potentialNucleotideString : String = nucleotideString.uppercased()
        potentialNucleotideString = potentialNucleotideString.replacingOccurrences(of: " ", with: "")
        
        // check that the nucleotide sequence is valid
        guard isValidCharacterStateString(potentialCharacterStateString:potentialNucleotideString) == true else {
            throw TaxonDataError.incorrectDataFormat
        }
        
        // the string containing the nucleotide sequence is good, so we can now initialize it
        numCharacters = potentialNucleotideString.count
        for c in potentialNucleotideString {
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
            let copy = try TaxonDataDna(name: taxonName, nucleotideString: self.characterDataString())
            return copy
        }
        catch {
            return TaxonDataDna()
        }
    }

    // MARK: -

    override func addCharacter(characterState: Character) {

        let nucCode: Int = encodeNucleotide(nucChar: characterState)
        var nucs: [Bool] = [false,false,false,false]
        encodeNucleotideVector(nucCode: nucCode, nucVector: &nucs)
        let isGapChar: Bool = isGap(dataChar: characterState)
        for i in 0..<4 {
            self += nucs[i]
            }
        self += isGapChar
    }
    
    // return a String holding the DNA sequences for this taxon
    override func characterDataString() -> String {
    
        var str: String = ""
        for i in 0..<numCharacters {
            let offset = i * 5
            let isGapChar: Bool = self[offset + 4]
            if isGapChar == true {
                str += "-"
            }
            else {
                var nucs: [Bool] = [false,false,false,false]
                nucs[0] = self[offset    ]
                nucs[1] = self[offset + 1]
                nucs[2] = self[offset + 2]
                nucs[3] = self[offset + 3]
                str += String(decodeNucleotideVector(nucVector:nucs))
            }
        }
        return str
    }

    // decode a vector of four Bool variables, returning the nucleotide using the IUPAC standard
    func decodeNucleotideVector(nucVector: [Bool]) -> Character {
    
        if nucVector[0] == true && nucVector[1] == false && nucVector[2] == false && nucVector[3] == false {
            return "A"
        }
        else if nucVector[0] == false && nucVector[1] == true && nucVector[2] == false && nucVector[3] == false {
            return "C"
        }
        else if nucVector[0] == true && nucVector[1] == true && nucVector[2] == false && nucVector[3] == false {
            return "M"
        }
        else if nucVector[0] == false && nucVector[1] == false && nucVector[2] == true && nucVector[3] == false {
            return "G"
        }
        else if nucVector[0] == true && nucVector[1] == false && nucVector[2] == true && nucVector[3] == false {
            return "R"
        }
        else if nucVector[0] == false && nucVector[1] == true && nucVector[2] == true && nucVector[3] == false {
            return "S"
        }
        else if nucVector[0] == true && nucVector[1] == true && nucVector[2] == true && nucVector[3] == false {
            return "V"
        }
        else if nucVector[0] == false && nucVector[1] == false && nucVector[2] == false && nucVector[3] == true {
            return "T"
        }
        else if nucVector[0] == true && nucVector[1] == false && nucVector[2] == false && nucVector[3] == true {
            return "W"
        }
        else if nucVector[0] == false && nucVector[1] == true && nucVector[2] == false && nucVector[3] == true {
            return "Y"
        }
        else if nucVector[0] == true && nucVector[1] == true && nucVector[2] == false && nucVector[3] == true {
            return "H"
        }
        else if nucVector[0] == false && nucVector[1] == false && nucVector[2] == true && nucVector[3] == true {
            return "K"
        }
        else if nucVector[0] == true && nucVector[1] == false && nucVector[2] == true && nucVector[3] == true {
            return "D"
        }
        else if nucVector[0] == false && nucVector[1] == true && nucVector[2] == true && nucVector[3] == true {
            return "B"
        }
        else {
            return "N"
        }
    }

    // return the IUPAC nucleotide character
    func encodeNucleotide(nucChar: Character) -> Int {
    
        switch nucChar {
            case "A":
                return 1
            case "C":
                return 2
             case "G":
                return 4
            case "T":
                return 8
            case "R":
                return 5
            case "Y":
                return 10
            case "M":
                return 3
            case "K":
                return 12
            case "S":
                return 6
            case "W":
                return 9
            case "H":
                return 11
            case "B":
                return 14
            case "V":
                return 7
            case "D":
                return 13
            case "N":
                return 15
            case "?":
                return 15
            case "-":
                return 15
           default:
                return 15
        }
    }

    // fill out an array of four Bools that represent the IUPAC nucleotide code
    func encodeNucleotideVector(nucCode: Int, nucVector: inout [Bool]) {

        switch nucCode {
            case 1:
                nucVector[0] = true
                nucVector[1] = false
                nucVector[2] = false
                nucVector[3] = false
            case 2:
                nucVector[0] = false
                nucVector[1] = true
                nucVector[2] = false
                nucVector[3] = false
            case 3:
                nucVector[0] = true
                nucVector[1] = true
                nucVector[2] = false
                nucVector[3] = false
            case 4:
                nucVector[0] = false
                nucVector[1] = false
                nucVector[2] = true
                nucVector[3] = false
            case 5:
                nucVector[0] = true
                nucVector[1] = false
                nucVector[2] = true
                nucVector[3] = false
            case 6:
                nucVector[0] = false
                nucVector[1] = true
                nucVector[2] = true
                nucVector[3] = false
            case 7:
                nucVector[0] = true
                nucVector[1] = true
                nucVector[2] = true
                nucVector[3] = false
            case 8:
                nucVector[0] = false
                nucVector[1] = false
                nucVector[2] = false
                nucVector[3] = true
            case 9:
                nucVector[0] = true
                nucVector[1] = false
                nucVector[2] = false
                nucVector[3] = true
            case 10:
                nucVector[0] = false
                nucVector[1] = true
                nucVector[2] = false
                nucVector[3] = true
            case 11:
                nucVector[0] = true
                nucVector[1] = true
                nucVector[2] = false
                nucVector[3] = true
            case 12:
                nucVector[0] = false
                nucVector[1] = false
                nucVector[2] = true
                nucVector[3] = true
            case 13:
                nucVector[0] = true
                nucVector[1] = false
                nucVector[2] = true
                nucVector[3] = true
            case 14:
                nucVector[0] = false
                nucVector[1] = true
                nucVector[2] = true
                nucVector[3] = true
            case 15:
                nucVector[0] = true
                nucVector[1] = true
                nucVector[2] = true
                nucVector[3] = true
            default:
                nucVector[0] = true
                nucVector[1] = true
                nucVector[2] = true
                nucVector[3] = true
        }
    }
    
    override func getCharacterState(index: Int) -> Character {
    
        var nucs: [Bool] = [false,false,false,false]
        let offset : Int = index * 5
        nucs[0] = self[offset    ]
        nucs[1] = self[offset + 1]
        nucs[2] = self[offset + 2]
        nucs[3] = self[offset + 3]
        let c : Character = decodeNucleotideVector(nucVector:nucs)
        return c
    }
    
    // check that the state is valid
    override func isValidCharacterState(potentialCharacterState: Character) -> Bool {

        let validDnaCharacters = "ACGTRYMKSWHBVDN?-"
        if validDnaCharacters.contains(potentialCharacterState) == false {
            return false
        }
        return true
    }
    
    override func setCharacterState(characterIdx: Int, characterState: Character) {
    
        let nucCode: Int = encodeNucleotide(nucChar:characterState)
        var nucs: [Bool] = [false,false,false,false]
        encodeNucleotideVector(nucCode: nucCode, nucVector: &nucs)
        let isGapChar: Bool = isGap(dataChar:characterState)
        
        let offset : Int = characterIdx * 5
        self[offset    ] = nucs[0]
        self[offset + 1] = nucs[1]
        self[offset + 2] = nucs[2]
        self[offset + 3] = nucs[3]
        self[offset + 4] = isGapChar
    }

}
