#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
@class Node;
@class WindowControllerSimulate;



@interface SimulateView : NSView {

    IBOutlet WindowControllerSimulate*      myWindowController;
    NSSize                                  handleSize;
    Node*                                   selectedNode;
    BOOL                                    isBiggestRectInitialized;
    NSPoint                                 startingPoint;
    NSPoint                                 currentPoint;
    BOOL                                    activeBranchDrag;
    NSRect                                  biggestNameRect;
    double                                  factor;
    double                                  upperLimitY;
    double                                  lowerLimitY;
}

- (void)findBiggestTextRectForTaxonNames;
- (void)keyDown:(NSEvent*)event;

@end
