#import <Cocoa/Cocoa.h>
@class ModelView;
@class Parm;
@class ParmDraw;
@class PaletteDrawer;
@class Variable;
@class WindowControllerModel;



@interface PaletteView : NSView {

    NSEvent*                           mouseDownEvent;
    IBOutlet PaletteDrawer*            drawerPtr;
    int                                numVariables;
    int                                numConstants;
    int                                numPlates;
    int                                numFunctions;
    int                                numDisplayColumns;
    int                                parmIdOffsets[4];
    float                              parmSize;
    float                              parmSpacing;
    NSTimer*                           toolTipTimer;
    BOOL                               isToolTipTimerActive;
	Variable*                          potentialVariable;
    Variable*                          variableForTip;
    int                                fireNum;
	NSMutableArray*                    myTrackingAreas;
    NSMutableArray*                    drawElements;
    BOOL                               isOverParameterImage;
	IBOutlet WindowControllerModel*    myController;
}

- (void)drawParameterIndexed:(int)idx andWithAddress:(Parm*)pPtr inRect:(NSRect)r;
- (ParmDraw*)getParmElementIndexed:(int)idx;
- (int)indexOfParameterWithName:(NSString*)pName;
- (void)initializeUnicode;
- (int)numColumns;
- (int)numParms;
- (int)numRows;
- (int)parmId;
- (void)removeAllTrackingAreas;
- (void)setViewBounds;
- (void)startToolTipTimer;
- (void)stopToolTipTimer;
- (void)transformToBottomLeftCoordinates:(NSPoint*)forPoint;
- (void)transformToTopLeftCoordinates:(NSPoint*)forPoint;
- (void)updateViewFrame;
- (NSString*)whichParmSelected;

@end
