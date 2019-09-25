//
//  ToolObject.swift
//  macgui
//
//  Created by Svetlana Krasikova on 2/11/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

//Generic class for tools
class ToolObject: NSObject{
    
    var image: NSImage
    @objc dynamic var frameOnCanvas: NSRect
    var name: String?
    
    
    init(image: NSImage, frameOnCanvas: NSRect){
        self.frameOnCanvas = frameOnCanvas
        self.image = image
    }
}
