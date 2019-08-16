//
//  ControllerExtensions.swift
//  macgui
//
//  Created by Svetlana Krasikova on 2/20/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

extension NSArrayController {
    
    /**
     Delete selected Analysis objects. Show a control panel to confirm the operation.
     
     - parameter toRemove: array of selected Analysis objects
    
     */
    func removeSelectedAnalyses(toRemove: [Analysis]){
        let alert = NSAlert()
        alert.messageText = "Warning: Delete Analysis"
        if toRemove.count > 1 {
            alert.informativeText = "You are about to delete \(toRemove.count) analyses, with all their associated information. Do you wish to continue with this operation?"
        } else {
            alert.informativeText = "You are about to delete an analysis, with all of its associated information. Do you wish to continue with this operation?"
        }
        
        alert.addButton(withTitle: "Delete")
        alert.addButton(withTitle: "Cancel")
        
        let result = alert.runModal()
        switch result {
            case NSApplication.ModalResponse.alertFirstButtonReturn:
                self.remove(contentsOf: toRemove)
            default: break
        }
    }
}

extension NavigatorViewController {
    
    /**
     Create a unique default analysis name starting with prefix followed by the next available numerical identifier
     - parameter prefix: the initial substring of the returned name
     - returns: A unique name for an analysis
     */
    
    
    func getUniqueAnalysisName(prefix: String, isCopy: Bool) -> String {
        var prefixUntilCopy = prefix
        var defaultNameIndices = [Int](repeating: 0, count: analyses.count)
        if let index = prefix.range(of: " copy")?.lowerBound{
            prefixUntilCopy = String(prefix.prefix(upTo: index))
        }
        for analysis in analyses{
            if analysis.name.starts(with: prefixUntilCopy){
                if !isCopy {
                    if analysis.name == prefix {
                        defaultNameIndices[0] = 1
                    } else {
                        if  analysis.name.range(of: "copy") == nil {
                            let numericalIdentifierIndex = (analysis.name.components(separatedBy: " ").count)-1
                            if let index = Int(analysis.name.components(separatedBy: " ")[numericalIdentifierIndex]){
                                if index >= defaultNameIndices.count {
                                    defaultNameIndices.insert(1, at: defaultNameIndices.count)
                                } else {
                                    defaultNameIndices[index] = 1
                                }
                            }
                        }
                    }
                } else {
                    if analysis.name.hasSuffix("copy") ||  analysis.name.range(of: "copy") == nil {
                        defaultNameIndices[0] = 1
                    } else {
                        if  analysis.name.range(of: "copy") != nil {
                            let numericalIdentifierIndex = (analysis.name.components(separatedBy: " ").count)-1
                            if let index = Int(analysis.name.components(separatedBy: " ")[numericalIdentifierIndex]){
                                if index >= defaultNameIndices.count {
                                    defaultNameIndices.insert(1, at: defaultNameIndices.count)
                                } else {
                                    defaultNameIndices[index] = 1
                                }
                            }
                        }
                    }
                }
            }
        }
        if defaultNameIndices.isEmpty || defaultNameIndices[0] == 0 {
            return isCopy ? "\(prefixUntilCopy) copy" : prefix
        }
        for (i,v) in defaultNameIndices.enumerated(){
            if v == 0 {
                return isCopy ? "\(prefixUntilCopy) copy \(i)": "\(prefix) \(i)"
            }
        }
        return isCopy ? "\(prefixUntilCopy) copy \(defaultNameIndices.count)": "\(prefix) \(defaultNameIndices.count)"
    }
    
}


extension CanvasViewController {
    
    func initToolObjectWithName(_ name: String, image: NSImage, frame: NSRect) -> ToolObject {
        let index = name.firstIndex(of: ".") ?? name.endIndex
        let toolType = String(name[..<index])
        switch toolType {
        case "bootstrap":
            return Bootstrap(image: image, frameOnCanvas: frame)
        case "align":
            return Align(image: image, frameOnCanvas: frame)
        case "readdata":
            return ReadData(image: image, frameOnCanvas: frame)
        default:
            return ToolObject(image: image, frameOnCanvas: frame)
            
        }
    }
}



