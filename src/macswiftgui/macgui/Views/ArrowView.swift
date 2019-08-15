//
//  ArrowView.swift
//  macgui
//
//  Created by Svetlana Krasikova on 5/24/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class ArrowView: CanvasObjectView {
    
    weak var arrowViewDelegate: ArrowViewDelegate?
    var clickArea: CGPath?

    
    override func mouseDown(with event: NSEvent) {
        let point = self.convert(event.locationInWindow, from: nil)
        if clickAreaContains(point: point) {
            super.mouseDown(with: event)
        } else {
            if let canvasView = self.superview as? CanvasView {
                if canvasView.delegate?.isMouseDownOnArrowView(event: event, point: point) == false {
                    canvasView.mouseDown(with: event)
                }
            }
        }
    }
    
    func clickAreaContains(point: NSPoint) -> Bool {
        if let clickArea = clickArea {
            let contains = clickArea.contains(point)
            return contains
            
        }
        else { return false }
    }
    
    
    override func updateLayer() {
        arrowViewDelegate?.setClickArea()
        arrowViewDelegate?.updateArrowInLayer(selected: isSelected)
    }

    
}

protocol ArrowViewDelegate: class {
    func updateArrowInLayer(selected: Bool)
    func setClickArea()
}

