//
//  Analysis.swift
//  macgui
//
//  Created by Svetlana Krasikova on 2/14/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class Analysis: NSObject, NSCopying {
    
    @objc dynamic var name: String = ""
    @objc dynamic var tools: [ToolObject] = []
    @objc dynamic var arrows: [Connection] = []
    
    override init(){
        super.init()
    }
    init(name: String){
        self.name = name
        super.init()
    }
    
    func isEmpty() -> Bool {
        return arrows.isEmpty && tools.isEmpty
    }
    
    
    func copy(with zone: NSZone? = nil) -> Any {
        let copy = Analysis()
        copy.tools = tools
        copy.arrows = arrows
        return copy
    }

}


