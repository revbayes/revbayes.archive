//
//  CanvasObjectViewController.swift
//  macgui
//
//  Created by Svetlana Krasikova on 6/4/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class CanvasObjectViewController: NSViewController, CanvasObjectViewDelegate {
    
    var shiftKeyPressed: Bool = false
    
    var viewSelected: Bool = false {
        didSet {
            (view as! CanvasObjectView).isSelected = viewSelected
            if viewSelected && !shiftKeyPressed {
                NotificationCenter.default.post(name: .didSelectCanvasObjectController, object: self)
            }
        }
    }
    func setObjectViewSelected(flag: Bool) {
        shiftKeyPressed = flag
        viewSelected = true
    }
    
    
    override func keyDown(with event: NSEvent) {
        if event.charactersIgnoringModifiers == String(Character(UnicodeScalar(NSDeleteCharacter)!)) {
            NotificationCenter.default.post(name: .didSelectDeleteKey, object: self)
        }
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        (self.view as! CanvasObjectView).delegate = self
    }
    
    
    
}
