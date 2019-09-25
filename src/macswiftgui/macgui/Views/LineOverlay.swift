// Created by Rob Mayoff on 4/28/17.

import AppKit

class LineOverlay {
    
    init(startScreenPoint: CGPoint, endScreenPoint: CGPoint) {
        self.startScreenPoint = startScreenPoint
        self.endScreenPoint = endScreenPoint
        
        NotificationCenter.default.addObserver(self, selector: #selector(LineOverlay.screenLayoutDidChange(_:)), name: NSApplication.didChangeScreenParametersNotification, object: nil)
        synchronizeWindowsToScreens()
    }
    
    var startScreenPoint: CGPoint { didSet { setViewPoints() } }
    
    var endScreenPoint: CGPoint { didSet { setViewPoints() } }
    
    func removeFromScreen() {
        windows.forEach { $0.close() }
        windows.removeAll()
    }
    
    private var windows = [NSWindow]()
    
    deinit {
        NotificationCenter.default.removeObserver(self)
        removeFromScreen()
    }
    
    @objc private func screenLayoutDidChange(_ note: Notification) {
        synchronizeWindowsToScreens()
    }
    
    private func synchronizeWindowsToScreens() {
        var spareWindows = windows
        windows.removeAll()
        for screen in NSScreen.screens {
            let window: NSWindow
            if let index = spareWindows.firstIndex(where: { $0.screen === screen}) {
                window = spareWindows.remove(at: index)
            } else {
                let styleMask = NSWindow.StyleMask.borderless
                window = NSWindow(contentRect: .zero, styleMask: styleMask, backing: .buffered, defer: true, screen: screen)
                window.contentView = ConnectionView()
                window.isReleasedWhenClosed = false
                window.ignoresMouseEvents = true
            }
            windows.append(window)
            window.setFrame(screen.frame, display: true)
            
            // Make the view's geometry match the screen geometry for simplicity.
            let view = window.contentView!
            var rect = view.bounds
            rect = view.convert(rect, to: nil)
            rect = window.convertToScreen(rect)
            view.bounds = rect
            
            window.backgroundColor = .clear
            window.isOpaque = false
            window.hasShadow = false
            window.isOneShot = true
            window.level = NSWindow.Level(rawValue: 1)
            
            window.contentView?.needsLayout = true
            window.orderFront(nil)
        }
        
        spareWindows.forEach { $0.close() }
    }
    
    private func setViewPoints() {
        for window in windows {
            let view = window.contentView! as! ConnectionView
            view.parameters.startPoint = startScreenPoint
            view.parameters.endPoint = endScreenPoint
        }
    }
    
}

private class ConnectionView: NSView {
    struct Parameters {
        var startPoint = CGPoint.zero
        var endPoint = CGPoint.zero
        var barThickness = CGFloat(2)
        var ballRadius = CGFloat(3)
    }
    
    var parameters = Parameters() { didSet { needsLayout = true } }
    
    init() {
        super.init(frame: .zero)
        
        wantsLayer = true
        
        shapeLayer.lineJoin = CAShapeLayerLineJoin.miter
        shapeLayer.lineWidth = 0.75
        shapeLayer.strokeColor = NSColor.white.cgColor
        shapeLayer.fillColor = NSColor(calibratedHue: 209/360, saturation: 0.83, brightness: 1, alpha: 1).cgColor
        shapeLayer.shadowColor = NSColor.selectedControlColor.blended(withFraction: 0.2, of: .black)?.withAlphaComponent(0.85).cgColor
        shapeLayer.shadowRadius = 3
        shapeLayer.shadowOpacity = 1
        shapeLayer.shadowOffset = .zero
    }
    
    required init?(coder decoder: NSCoder) { fatalError() }
    
    let shapeLayer = CAShapeLayer()
    override func makeBackingLayer() -> CALayer { return shapeLayer }
    
    override func layout() {
        super.layout()
        
        shapeLayer.path = CGPath.barbell(from: parameters.startPoint, to: parameters.endPoint, barThickness: parameters.barThickness, bellRadius: parameters.ballRadius)
        shapeLayer.shadowPath = CGPath.barbell(from: parameters.startPoint, to: parameters.endPoint, barThickness: parameters.barThickness + shapeLayer.lineWidth / 2, bellRadius: parameters.ballRadius + shapeLayer.lineWidth / 2)
    }
    
    public override func draggingEntered(_ sender: NSDraggingInfo) -> NSDragOperation {
        Swift.print("draggingEntered \(self) \(sender)")
        return []
    }
    
    
}

private extension CGPath {
    class func barbell(from start: CGPoint, to end: CGPoint, barThickness proposedBarThickness: CGFloat, bellRadius proposedBellRadius: CGFloat) -> CGPath {
        let barThickness = max(0, proposedBarThickness)
        let bellRadius = max(barThickness / 2, proposedBellRadius)
        
        let vector = CGPoint(x: end.x - start.x, y: end.y - start.y)
        let length = hypot(vector.x, vector.y)
        
        if length == 0 {
            return CGPath(ellipseIn: CGRect(origin: start, size: .zero).insetBy(dx: -bellRadius, dy: -bellRadius), transform: nil)
        }
        
        var yOffset = barThickness / 2
        var xOffset = sqrt(bellRadius * bellRadius - yOffset * yOffset)
        let halfLength = length / 2
        if xOffset > halfLength {
            xOffset = halfLength
            yOffset = sqrt(bellRadius * bellRadius - xOffset * xOffset)
        }
        
        let jointRadians = asin(yOffset / bellRadius)
        let path = CGMutablePath()
        path.addArc(center: .zero, radius: bellRadius, startAngle: jointRadians, endAngle: 2 * .pi - jointRadians, clockwise: false)
        path.addArc(center: CGPoint(x: length, y: 0), radius: bellRadius, startAngle: .pi + jointRadians, endAngle: .pi - jointRadians, clockwise: false)
        path.closeSubpath()
        
        let unitVector = CGPoint(x: vector.x / length, y: vector.y / length)
        var transform = CGAffineTransform(a: unitVector.x, b: unitVector.y, c: -unitVector.y, d: unitVector.x, tx: start.x, ty: start.y)
        return path.copy(using: &transform)!
    }
}
