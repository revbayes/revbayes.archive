#import <Cocoa/Cocoa.h>



@interface ToolView : NSView {

    NSEvent*                      mouseDownEvent;
	int                           whichToolSelected;
    float                         scaleFactor;
    int                           numTools;
    float                         toolSize;
    NSTimer*                      toolTipTimer;
    BOOL                          isToolTipTimerActive;
	int                           potentialToolForToolTip;
    int                           toolForToolTip;
    int                           fireNum;
	NSMutableArray*               myTrackingAreas;
}

@property (readwrite) int whichToolSelected;

- (void)scaleFactorChanged:(NSNotification*)notification;
- (void)startToolTipTimer;
- (void)stopToolTipTimer;
- (void)updateScrollBars;

@end
