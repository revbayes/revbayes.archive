//
//  ArrowViewController.swift
//  macgui
//
//  Created by Svetlana Krasikova on 5/24/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa
import Darwin

class ArrowViewController: CanvasObjectViewController, ArrowViewDelegate, NSWindowDelegate {
    
    private var observers = [NSKeyValueObservation]()
    

    weak var targetTool: Connectable?
    weak var sourceTool: Connectable?
    weak var connection: Connection?
    
    var frame: NSRect
    var color: NSColor
    
    var endPoint: NSPoint {
        get{
            return (targetTool?.frameOnCanvas.center())!
        }
    }
    
    var beginPoint: NSPoint {
        get {
            return (sourceTool?.frameOnCanvas.center())!
        }
    }
    
    private var targetToolFrame: NSRect {
        get {
            return (targetTool?.frameOnCanvas)!
        }
    }
    

    var arrowHeadParams: [String: CGFloat] {
        get {
            let angle = findLineAngle()
            let end = findEdgePoint(angle: angle, dimension: targetToolFrame.size.width)
            let begin = findEdgePoint(angle: angle, dimension: targetToolFrame.size.width + 5)
            return [
                "angle": angle,
                "endX": end.x,
                "endY": end.y,
                "beginX": begin.x,
                "beginY": begin.y,
            ]
        }
    }
    
    
    
    init(frame: NSRect, color: NSColor, sourceTool: Connectable, targetTool: Connectable, connection: Connection){
        self.targetTool = targetTool
        self.sourceTool = sourceTool
        self.color = color
        self.frame = frame
        self.connection = connection
        super.init(nibName: nil, bundle: nil)
    }
    
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    func ownedBy(tool: ToolObject) -> Bool{
        if self.targetTool  === tool || self.sourceTool === tool {
            return true
        }
        return false
    }
    
    func willDeleteView(){
        targetTool?.removeNeighbor(neighbor: sourceTool!, linkType: LinkType.inlet)
        sourceTool?.removeNeighbor(neighbor: targetTool!, linkType: LinkType.outlet)
    }

    
    func clearSublayers(){
        if let sublayers = view.layer?.sublayers {
            for sublayer in sublayers {
                sublayer.removeFromSuperlayer()
            }
        }
    }
    
    func drawArrow(width: CGFloat, highlight: Bool){
        clearSublayers()
        let arrowLayer = CAShapeLayer()
        arrowLayer.strokeColor = color.cgColor
        arrowLayer.lineWidth = width
        if highlight {
            arrowLayer.shadowOpacity = 0.7
            arrowLayer.shadowRadius = 10.0
        }
        
        let combined = CGMutablePath()
        combined.addPath(createLinePath())
        combined.addPath(createHeadPath(width: width))
        arrowLayer.path = combined
        view.layer?.addSublayer(arrowLayer)
        
    }
    
    
    func updateArrowInLayer(selected: Bool){
        if selected {
           drawArrow(width: 3.0, highlight: true)
        } else {
            drawArrow(width: 2.0, highlight: false)
        }
    }
    
    
    func setClickArea(){
        let path = CGMutablePath()
        path.addLines(between: [beginPoint, endPoint])
        (self.view as! ArrowView).clickArea = path.copy(strokingWithWidth: 15, lineCap: CGLineCap.round, lineJoin: CGLineJoin.round, miterLimit: 1)    }
    
    override func loadView() {
        self.view = ArrowView(frame: frame)     
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
       NotificationCenter.default.addObserver(self, selector: #selector(NSWindowDelegate.windowDidResize(_:)), name: NSWindow.didResizeNotification, object: nil)
        (self.view as! ArrowView).arrowViewDelegate = self
        view.wantsLayer = true
        drawArrow(width: 2.0, highlight: false)
        setClickArea()
        observeEndPointChanges()
    }
    
    
    func windowDidResize(_ notification: Notification) {
        if let canvasView = view.superview as? CanvasView {
            view.setFrameSize(canvasView.frame.size)
        }
    }
    
    func observeEndPointChanges(){
        observers = [
            sourceTool?.observe(\Connectable.frameOnCanvas, options: [.old, .new]) {tool, change in
                self.view.needsDisplay = true},
            
            targetTool?.observe(\Connectable.frameOnCanvas, options: [.old, .new]) {tool, change in
                self.view.needsDisplay = true}] as! [NSKeyValueObservation]
    }
    
}

extension ArrowViewController {
    
    func createLinePath() -> CGMutablePath {
        let path = CGMutablePath()
        let endOfLine = CGPoint(x: arrowHeadParams["beginX"]!, y: arrowHeadParams["beginY"]!)
        path.addLines(between: [beginPoint, endOfLine])
        return path
    }
    
    func createHeadPath(width: CGFloat)-> CGPath {
        let end = CGPoint(x: arrowHeadParams["endX"]!, y: arrowHeadParams["endY"]!)
        let begin = CGPoint(x: arrowHeadParams["beginX"]!, y: arrowHeadParams["beginY"]!)
        let path = arrow(from: begin, to: end, tailWidth: 0.0, headWidth: width + 1.5, headLength: 3.0)
        return path
    }
    
    func arrow(from start: CGPoint, to end: CGPoint, tailWidth: CGFloat, headWidth: CGFloat, headLength: CGFloat) -> CGMutablePath {
        let length = hypot(end.x - start.x, end.y - start.y)
        let tailLength = length - headLength
        
        func p(_ x: CGFloat, _ y: CGFloat) -> CGPoint { return CGPoint(x: x, y: y) }
        let points: [CGPoint] = [
            p(0, -tailWidth / 2),
            p(tailLength, -tailWidth / 2),
            p(tailLength, -headWidth / 2),
            p(length, 0),
            p(tailLength, headWidth / 2),
            p(tailLength, tailWidth / 2),
            p(0, tailWidth / 2)
        ]
        
        let cosine = (end.x - start.x) / length
        let sine = (end.y - start.y) / length
        let transform = CGAffineTransform(a: cosine, b: sine, c: -sine, d: cosine, tx: start.x, ty: start.y)
        
        let path = CGMutablePath()
        path.addLines(between: points, transform: transform)
        path.closeSubpath()
    
        return path
    }
    
    func findEdgePoint(angle: CGFloat, dimension: CGFloat) -> CGPoint {
        let intersection: CGPoint
        
        let xRad = dimension/2
        let yRad = dimension/2
        
        let tangent = tan(angle)
        let y = xRad * CGFloat(tangent)
        
        if abs(y) <= yRad {
        
            if angle < CGFloat.pi / 2 || angle > 3 * CGFloat.pi / 2 {
                intersection = CGPoint(x: xRad, y: y)
            } else {
                intersection = CGPoint(x: -xRad, y: -y)
            }
        } else {

            let x = yRad / CGFloat(tangent)
            
            if angle < CGFloat.pi {
                intersection = CGPoint(x: x, y: yRad)
            } else {
                intersection = CGPoint(x: -x, y: -yRad)
            }
        }
        let convertedIntersection = CGPoint(x: endPoint.x+intersection.x, y: endPoint.y+intersection.y)
        return convertedIntersection
    }
    
    
    func findLineAngle() -> CGFloat {
        let fromPoint = endPoint
        let toPoint = beginPoint
        let dx: CGFloat = toPoint.x - fromPoint.x
        let dy: CGFloat = toPoint.y - fromPoint.y
        let twoPi = 2 * CGFloat.pi
        let radians = (atan2(dy, dx) + twoPi).truncatingRemainder(dividingBy: twoPi)
        return radians
    }
    
   
    
}
