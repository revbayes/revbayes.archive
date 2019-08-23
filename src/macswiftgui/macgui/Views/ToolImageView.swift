//
//  ToolImageView.swift
//  macgui
//
//  Created by Svetlana Krasikova on 5/15/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class ToolImageView: NSImageView {
    
    override var needsPanelToBecomeKey: Bool {return true}
    override var acceptsFirstResponder: Bool {return true }
    override func becomeFirstResponder() -> Bool { return true }
    override func resignFirstResponder() -> Bool { return true }
   
    override func keyDown(with event: NSEvent) {
        super.keyDown(with: event)
    }
    
    
}
