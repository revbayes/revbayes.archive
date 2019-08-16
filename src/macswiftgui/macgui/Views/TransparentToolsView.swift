//
//  TransparentToolsView.swift
//  macgui
//
//  Created by Svetlana Krasikova on 5/30/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class TransparentToolsView: NSView {

    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)

    }
    
    override func hitTest(_ point: NSPoint) -> NSView? {
        return nil
    }
    
}
