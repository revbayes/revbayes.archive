//
//  ConnectorItem.swift
//  macgui
//
//  Created by Svetlana Krasikova on 4/21/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class ConnectorItem: NSCollectionViewItem, ConnectorItemViewDelegate {

    
    weak var parentTool: Connectable?
    
    var connector: Connector? {
        didSet{
            guard isViewLoaded else { return }
            if let type = connector {
                let fillColor = type.getColor()
                (view as! ConnectorItemView).drawArrow(color: fillColor)
            }
        }
    }
    

    override func viewDidLoad() {
        super.viewDidLoad()
        (view as! ConnectorItemView).delegate = self
    }
    
    override func mouseDown(with mouseDownEvent: NSEvent) {
        guard
            let window = view.window,
            let source = window.dragEndpoint(at: mouseDownEvent.locationInWindow)
            else { return super.mouseDown(with: mouseDownEvent) }
//        begin of a dragging session
        let controller = ConnectionDragController()
        controller.trackDrag(forMouseDownEvent: mouseDownEvent, in: source)
    }
    
    
    func getTool() -> Any? {
        if let parentTool = parentTool {
            return parentTool
        } else {return nil}
    }
    
    func getConnector() -> Any? {
        if let connector = connector {
            return connector
        } else {return nil}
    }
    
    func isOutlet() -> Bool {
        if let parentTool = parentTool, let connector = connector {
           return parentTool.outlets.contains(connector)
        }
        return false
    }
    
    
}

private extension NSWindow {
    
    func dragEndpoint(at point: CGPoint) -> ConnectorItemView? {
        var view = contentView?.hitTest(point)
        while let candidate = view {
            if let endpoint = candidate as? ConnectorItemView { return endpoint }
            view = candidate.superview
        }
        return nil
    }
    
}
