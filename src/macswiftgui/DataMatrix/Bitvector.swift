//
//  Bitvector.swift
//
//  Created by John Huelsenbeck on 7/9/19.
//  Copyright © 2019 John Huelsenbeck. All rights reserved.
//
// This class holds an array of bits, packed into the minumum number of
// unsigned integers (UInt). One can access a particular bit using the []
// operator, e.g. "bv[3] = true" sets the fourth bit to true. You can add
// bits to the bit vector using the += operator. Logical And, Or, and Xor
// are all overloaded so one can perform operations on multiple bit vectors.
// Bitvector objects can serialize themselves using the Codable protocol.
//

import Foundation



class Bitvector : CustomStringConvertible, Codable {

    // variables
    private static let bitsPerUint: Int = MemoryLayout<UInt>.size * 8         // the number of bits per Uint
    private static let maxUInt: UInt = UInt.max                               // the maximum value of a Uint
    private static let highBit: UInt = (UInt(1) << (Bitvector.bitsPerUint-1)) // a Uint with only the highest bit on
    private var numElements: Int = 0                                          // the number of bits
    private var numInts: Int = 0                                              // the number of Uints needed to store the bits
    private var v: [UInt] = []                                                // the array of Uints that store the bits
    private var mask: UInt = 0                                                // a mask for the last Uint in the array

    // MARK: -

    // keys indicating serialized elements
    private enum CodingKeys: String, CodingKey {
    
        case numElements
        case numInts
        case v
        case mask
    }

    enum BitvectorError: Error {
    
        case encodingError
        case decodingError
    }

    // MARK: -

    // initialize with an empty vector
    init () {
    
        self.numElements = 0
        self.numInts = 0
        self.mask = 0
    }
    
    // initialize with the number of elements
    init(numElements: Int) {

        // calculate size of vector and allocate memory for bitfield
        self.numElements = numElements
        self.numInts = (numElements / Bitvector.bitsPerUint)
        if self.numElements % Bitvector.bitsPerUint != 0 {
            numInts += 1
        }
        v = Array(repeating:UInt(0), count:numInts)
        
        // initialize the mask
        if numElements % Bitvector.bitsPerUint != 0 {
            mask = (Bitvector.highBit >> (numElements % Bitvector.bitsPerUint))
            mask -= 1
            mask ^= Bitvector.maxUInt
        }
        else {
            mask = Bitvector.maxUInt
        }
    }

    // initialize from serialized data
    required init(from decoder: Decoder) throws {

        print("Bitvector decoder")

        do {
            let values = try decoder.container(keyedBy: CodingKeys.self)
            self.numElements = try values.decode(Int.self,    forKey: .numElements)
            self.numInts     = try values.decode(Int.self,    forKey: .numInts)
            self.v           = try values.decode([UInt].self, forKey: .v)
            self.mask        = try values.decode(UInt.self,   forKey: .mask)
        }
        catch {
            throw BitvectorError.decodingError
        }
    }

    // encode the object for serialization
    func encode(to encoder: Encoder) throws {

        do {
            var container = encoder.container(keyedBy: CodingKeys.self)
            try container.encode(numElements, forKey: .numElements)
            try container.encode(numInts,     forKey: .numInts)
            try container.encode(v,           forKey: .v)
            try container.encode(mask,        forKey: .mask)
        }
        catch {
            throw BitvectorError.encodingError
        }
    }

    // overload the [] operator
    subscript(index: Int) -> Bool {
    
        get {
            if index >= 0 && index < self.numElements {
                return (( v[index/Bitvector.bitsPerUint] & (Bitvector.highBit >> (index % Bitvector.bitsPerUint))) != 0)
            }
            else {
                return false
            }
        }
        
        set(newValue) {
            if index < 0 {
                print("Index cannot be negative (\(index)")
                return
            }
            else if index >= self.numElements {
                print("Index is too large (\(index)")
                return
            }
            if newValue == true {
                v[index/Bitvector.bitsPerUint] |= (Bitvector.highBit >> (index % Bitvector.bitsPerUint))
            }
            else {
                v[index/Bitvector.bitsPerUint] &= ((Bitvector.highBit >> (index % Bitvector.bitsPerUint)) ^ Bitvector.maxUInt)
            }
        }
    }

    // MARK: -

    // sensibly print the object's information
    var description: String {
    
        var str: String = ""
        str += "Bitvector\n"
        str += "   Number of elements = \(numElements)\n"
        str += "   Number of available bits = \(numInts*Bitvector.bitsPerUint)\n   "
        str += bitString()
        return str
    }
    
    // overload the & operator, allowing statements such as "bv1 = bv2 & bv3"
    static func &(lhs: Bitvector, rhs: Bitvector) -> Bitvector {
    
        if lhs.numElements != rhs.numElements {
            print("Attempting & on bit vectors of unequal size (\(lhs.numElements) on left-hand side is unequal to \(rhs.numElements) on the right-hand side)")
            return Bitvector(numElements:0)
        }
        else {
        let ret = Bitvector(numElements:lhs.numElements)
        for i in 0..<lhs.numInts {
            ret.v[i] = (lhs.v[i] & rhs.v[i])
            }
        return ret
        }
    }

    // overload the | operator, allowing statements such as "bv1 = bv2 | bv3"
    static func |(lhs: Bitvector, rhs: Bitvector) -> Bitvector {
    
        if lhs.numElements != rhs.numElements {
            print("Attempting | on bit vectors of unequal size (\(lhs.numElements) on left-hand side is unequal to \(rhs.numElements) on the right-hand side)")
            return Bitvector(numElements:0)
        }
        else {
        let ret = Bitvector(numElements:lhs.numElements)
        for i in 0..<lhs.numInts {
            ret.v[i] = (lhs.v[i] | rhs.v[i])
            }
        return ret
        }
    }
    
    // overload the ^ operator, allowing statements such as "bv1 = bv2 ^ bv3"
    static func ^(lhs: Bitvector, rhs: Bitvector) -> Bitvector {
    
        if lhs.numElements != rhs.numElements {
            print("Attempting ^ on bit vectors of unequal size (\(lhs.numElements) on left-hand side is unequal to \(rhs.numElements) on the right-hand side)")
            return Bitvector(numElements:0)
        }
        else {
            let ret = Bitvector(numElements:lhs.numElements)
            for i in 0..<lhs.numInts {
                ret.v[i] = (lhs.v[i] ^ rhs.v[i])
            }
            return ret
        }
    }
    
    // overload the &= operator, allowing statements such as "bv1 &= bv2"
    static func &=(lhs: Bitvector, rhs: Bitvector) {

        if lhs.numElements == rhs.numElements {
            for i in 0..<lhs.numInts {
                lhs.v[i] &= rhs.v[i]
            }
        }
        else {
            print("Attempting &= on bit vectors of unequal size (\(lhs.numElements) on left-hand side is unequal to \(rhs.numElements) on the right-hand side)")
        }
    }
    
    // overload the |= operator, allowing statements such as "bv1 |= bv2"
    static func |=(lhs: Bitvector, rhs: Bitvector) {

        if lhs.numElements == rhs.numElements {
            for i in 0..<lhs.numInts {
                lhs.v[i] |= rhs.v[i]
            }
        }
        else {
            print("Attempting |= on bit vectors of unequal size (\(lhs.numElements) on left-hand side is unequal to \(rhs.numElements) on the right-hand side)")
        }
    }

    // overload the += operator, allowing statements such as "bv += true"
    static func +=(lhs: Bitvector, newVal: Bool) {

        lhs.numElements += 1;
        
        var newNumInts = (lhs.numElements / Bitvector.bitsPerUint)
        if lhs.numElements % Bitvector.bitsPerUint != 0 {
            newNumInts += 1
        }
        
        if newNumInts > lhs.numInts {
            lhs.v.append(0)
            lhs.numInts += 1
        }
        
        lhs[lhs.numElements-1] = newVal

        // re-initialize the mask
        if lhs.numElements % Bitvector.bitsPerUint != 0 {
            lhs.mask = (Bitvector.highBit >> (lhs.numElements % Bitvector.bitsPerUint))
            lhs.mask -= 1
            lhs.mask ^= Bitvector.maxUInt
        }
        else {
            lhs.mask = Bitvector.maxUInt
        }
    }

    static func +=(lhs: Bitvector, rhs: Bitvector) {

        for i in 0..<rhs.size() {
        
            let newVal : Bool = rhs[i]
            
            lhs.numElements += 1;
            
            var newNumInts = (lhs.numElements / Bitvector.bitsPerUint)
            if lhs.numElements % Bitvector.bitsPerUint != 0 {
                newNumInts += 1
            }
            
            if newNumInts > lhs.numInts {
                lhs.v.append(0)
                lhs.numInts += 1
            }
            
            lhs[lhs.numElements-1] = newVal

            // re-initialize the mask
            if lhs.numElements % Bitvector.bitsPerUint != 0 {
                lhs.mask = (Bitvector.highBit >> (lhs.numElements % Bitvector.bitsPerUint))
                lhs.mask -= 1
                lhs.mask ^= Bitvector.maxUInt
            }
            else {
                lhs.mask = Bitvector.maxUInt
            }
        }
    }

    // MARK: -

    // return a string holding the on and off bits
    func bitString() -> String {
    
        var str: String = ""
        for i in 0..<numElements {
            let tf = self[i]
            if tf == true {
                str += "1"
            }
            else {
                str += "0"
            }
        }
        return str
    }
    
    // set all of the bits to false
    func clearBits() {
    
        for i in 0..<numInts {
            v[i] = 0
        }
    }
    
    // change all true bits to false and all false bits to true
    func flipBits() {
    
        for i in 0..<numInts {
            v[i] = v[i] ^ Bitvector.maxUInt
        }
        v[numInts-1] &= self.mask
    }
    
    // flip a specific bit
    func flipBit(index: Int) {
    
        if index < 0 {
            print("Index cannot be negative (\(index)")
            return
        }
        else if index >= self.numElements {
            print("Index is too large (\(index)")
            return
        }
        v[index/Bitvector.bitsPerUint] ^= ((Bitvector.highBit >> (index % Bitvector.bitsPerUint)))
    }
    
    // return the number of true bits
    func numOnBits() -> Int {
    
        var cnt: Int = 0
        for i in 0..<numElements {
            if (v[i/Bitvector.bitsPerUint] & (Bitvector.highBit >> (i % Bitvector.bitsPerUint))) != 0 {
                cnt += 1
            }
        }
        return cnt
    }
    
    // return the number of bits in the bit vector
    func size() -> Int {
    
        return numElements
    }
}
