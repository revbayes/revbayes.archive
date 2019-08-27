//
//  CanvasToolView.swift
//  macgui
//
//  Created by Svetlana Krasikova on 2/27/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class CanvasToolView: CanvasObjectView {
   
    enum Appearance {
         static let borderWidth: CGFloat = 1.8
        static let selectionCornerRadius: CGFloat = 5.0
        
        static let selectionColor: CGColor = NSColor.systemGray.cgColor
        static let defaultColor: CGColor = NSColor.clear.cgColor
        
        static let selectionShadowOpacity: Float = 0.7
        static let defaultShadowOpacity: Float = 0.0
       
        static let selectionShadowRadius: CGFloat = 10.0
        static let defaultShadowRadius: CGFloat = 3.0
    }
    

    var firstMouseDownPoint: NSPoint?
    weak var canvasViewToolDelegate: CanvasToolViewDelegate?
    
    override init(frame: NSRect) {
        super.init(frame: frame)
        commonInit()
    }
    
    required init?(coder decoder: NSCoder) {
        super.init(coder: decoder)
        commonInit()
    }
    
    private func commonInit() {
        let trackingArea = NSTrackingArea(rect: self.bounds, options:NSTrackingArea.Options.init(rawValue: 129), owner: self, userInfo: nil)
        self.addTrackingArea(trackingArea)

        registerForDraggedTypes([NSPasteboard.PasteboardType(rawValue: kUTTypeData as String)])
    }
    
    public override func draggingEntered(_ sender: NSDraggingInfo) -> NSDragOperation {
        return canvasViewToolDelegate?.getConnectorItem(sender)?.draggingEntered(sender) ??  sender.draggingSourceOperationMask
    }
    
    public override func draggingExited(_ sender: NSDraggingInfo?) {
        if let sender = sender {
            canvasViewToolDelegate?.getConnectorItem(sender)?.draggingExited(sender)
        }
    }
    
    public override func draggingEnded(_ sender: NSDraggingInfo?) {
        if let sender = sender {
            canvasViewToolDelegate?.getConnectorItem(sender)?.draggingEnded(sender)
        }
    }
    
    public override func performDragOperation(_ sender: NSDraggingInfo) -> Bool {
        if let connector = canvasViewToolDelegate?.getConnectorItem(sender) {
            return connector.performDragOperation(sender)
        } else { return false }
    }
    
    
    override func mouseDown(with event: NSEvent) {
        super.mouseDown(with: event)
        firstMouseDownPoint = (self.window?.contentView?.convert(event.locationInWindow, to: self))!
    }

    
    override func mouseDragged(with event: NSEvent) {
        if let newPoint = self.window?.contentView?.convert(event.locationInWindow, to: self), let firstMouseDownPoint = firstMouseDownPoint {
            let offset = NSPoint(x: newPoint.x - firstMouseDownPoint.x, y: newPoint.y - firstMouseDownPoint.y)
            let origin = self.frame.origin
            let newOrigin = NSPoint(x: origin.x + offset.x, y: origin.y + offset.y)
            canvasViewToolDelegate?.updateFrame()
            self.setFrameOrigin(newOrigin)
        }
    }
    
    override func mouseUp(with event: NSEvent) {
        canvasViewToolDelegate?.updateFrame()
    }
    
    override func viewDidEndLiveResize() {
        super.viewDidEndLiveResize()
        canvasViewToolDelegate?.updateFrame()
    }
    
    override func updateLayer() {
        layer?.cornerRadius = Appearance.selectionCornerRadius
        layer?.borderWidth = Appearance.borderWidth
        layer?.masksToBounds =  false
        if isSelected {
            layer?.shadowOpacity = Appearance.selectionShadowOpacity
            layer?.shadowRadius = Appearance.selectionShadowRadius
            layer?.borderColor = Appearance.selectionColor
        } else {
            layer?.shadowOpacity = Appearance.defaultShadowOpacity
            layer?.shadowRadius = Appearance.defaultShadowRadius
            layer?.borderColor = Appearance.defaultColor
        }
    }
}

protocol CanvasToolViewDelegate: class {
    func updateFrame()
    func getConnectorItem(_ sender: NSDraggingInfo) -> ConnectorItemView?
}
