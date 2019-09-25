//
//  ConnectionDragController.swift
//  macgui
//
//  Created by Svetlana Krasikova on 5/21/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class ConnectionDragController: NSObject, NSDraggingSource {

    var sourceEndpoint: ConnectorItemView?
    private var lineOverlay: LineOverlay?
    
    func connect(to target: ConnectorItemView) {
        print("Connect \(sourceEndpoint!) to \(target)")
        NotificationCenter.default.post(name: .didConnectTools,
                                        object: self,
                                        userInfo: ["source": sourceEndpoint!, "target": target])
    }
    
    func trackDrag(forMouseDownEvent mouseDownEvent: NSEvent, in sourceEndpoint: ConnectorItemView) {
        self.sourceEndpoint = sourceEndpoint
        let item = NSDraggingItem(pasteboardWriter: NSPasteboardItem(pasteboardPropertyList: "view", ofType: NSPasteboard.PasteboardType(rawValue: kUTTypeData as String as String))!)
        let session = sourceEndpoint.beginDraggingSession(with: [item], event: mouseDownEvent, source: self)
        session.animatesToStartingPositionsOnCancelOrFail = false
    }
    
    func draggingSession(_ session: NSDraggingSession, sourceOperationMaskFor context: NSDraggingContext) -> NSDragOperation {
        switch context {
        case .withinApplication: return .generic
        case .outsideApplication: return []
        @unknown default:
            fatalError()
        }
    }
    
    func draggingSession(_ session: NSDraggingSession, willBeginAt screenPoint: NSPoint) {
        sourceEndpoint?.state = .source
        lineOverlay = LineOverlay(startScreenPoint: screenPoint, endScreenPoint: screenPoint)
    }
    
    func draggingSession(_ session: NSDraggingSession, movedTo screenPoint: NSPoint) {
        lineOverlay?.endScreenPoint = screenPoint
    }
    
    func draggingSession(_ session: NSDraggingSession, endedAt screenPoint: NSPoint, operation: NSDragOperation) {
        lineOverlay?.removeFromScreen()
        sourceEndpoint?.state = .idle
    }
    
    func ignoreModifierKeys(for session: NSDraggingSession) -> Bool { return true }

}
