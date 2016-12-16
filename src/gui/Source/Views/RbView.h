#import <Cocoa/Cocoa.h>
#import "RevBayes.h"
@class AnalysisDocument;
@class RbItem;

#define min(x,y)   ((x) < (y) ? (x) : (y))	
#define max(x,y)   ((x) > (y) ? (x) : (y))	
extern NSString* const ToolDataPboardType;



@interface RbView : NSView {

	AnalysisDocument*             analysisDocumentPtr;  // a pointer to the AnalysisDocument instance that owns this view
    NSColor*                      bkgrndColor;          // the color of the background
	NSPoint                       bottomRightCorner;    // the position of the item(s) that are most bottom and most left
    NSMutableArray*               copiedItems;          // an array that keeps track of the objects to be copied
	NSPoint                       cursorLocation;       // location of cursor, for drawing connections
    NSSize                        cursorOffset;         // the offset for dragging an item
    BOOL                          draggedItems;         // did the user just drag items around
    int                           fireNum;
    NSColor*                      gridColor;            // the color of the background
    float                         gridSize;             // size of the grid
    BOOL                          isItemTipTimerActive;
	ItemSelector                  itemArea;
    RbItem*                       itemForTip;
    NSTimer*                      itemTipTimer;
	NSMutableArray*               itemsPtr;             // a pointer to the array holding the pointers to the items
	IBOutlet NSScrollView*        myScrollView;         // the pointer to the scrollview that contains this view
	int                           pasteNum;             // keeps track of the number of pastes
    BOOL                          optionClicked;        // did the user option-click on mouse down
    float                         scaleFactor;          // the zoom factor for drawing
	int                           scaleIdx;             // an index for the scale that helps selecting the correct image for an item
    NSMutableSet*                 selectedItems;        // keeps track of items originally selected for sweep selections
	ItemSelector                  selection;            // the selected item with information on the selection
    BOOL                          shiftClicked;         // did the user shift click on mouse down
	BOOL                          showGrid;             // whether or not the grid should be displayed
    BOOL                          snapToGrid;           // whether or not the items should be drawn to the grid
	BOOL                          sweepAction;          // a bool indicating that a sweep action took place
	NSPoint                       sweepCurrentLocation; // a point to keep track of a sweep action's end
	NSPoint                       sweepStartLocation;   // a point to keep track of a sweep action's beginning
	NSPoint                       topLeftCorner;        // the position of the item(s) that are most to the top and left
	NSMutableDictionary*          trackingAreas;        // keeps tracking areas for roll overs
	NSRect                        informationRect[8];
	NSPoint                       iOffset[8];
	NSPoint                       rOffset[8];
	NSPoint                       mOffset[8];
    NSPoint                       lOffset[8];
	NSAttributedString*           selectedAttributedString[8];
	NSAttributedString*           unselectedAttributedString[8];

}

@property (nonatomic) float       scaleFactor;
@property (nonatomic) int         scaleIdx;
@property (nonatomic) BOOL        snapToGrid;

- (BOOL)acceptsFirstResponder;
- (void)addTrackingForItem:(RbItem*)itm;
- (IBAction)copy:(id)sender;
- (IBAction)cut:(id)sender;
- (void)concludeDragOperation:(id <NSDraggingInfo>)sender;
- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender;
- (void)draggingExited:(id <NSDraggingInfo>)sender;
- (NSPoint)findNearestGridIntersectionToPoint:(NSPoint)p;
- (BOOL)hasItemCollided:(RbItem*)itm;
- (BOOL)hasRectCollided:(NSRect)r;
- (BOOL)hasRectCollidedWithEdge:(NSRect)r;
- (void)initializeDrawingInfo;
- (void)itemTipFire;
- (void)keyDown:(NSEvent*)event;
- (void)mouseDown:(NSEvent*)event;
- (void)mouseEntered:(NSEvent*)event;
- (void)mouseExited:(NSEvent*)event;
- (IBAction)moveToBack:(NSEvent*)event;
- (IBAction)moveBackward:(NSEvent*)event;
- (IBAction)moveToFront:(NSEvent*)event;
- (IBAction)moveForward:(NSEvent*)event;
- (BOOL)movePlateBackward:(RbItem*)plt;
- (BOOL)movePlateForward:(RbItem*)plt;
- (int)numPlates;
- (int)numSelectedItems;
- (int)numSelectedPlates;
- (IBAction)paste:(id)sender;
- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender;
- (BOOL)prepareForDragOperation:(id <NSDraggingInfo>)sender;
- (BOOL)readFromPasteboard:(NSPasteboard*)pb;
- (void)removeAllFocusRings;
- (void)removeAllTrackingAreas;
- (void)removeSelectedConnections;
- (void)removeSelectedItems;
- (void)removeTrackingForItem:(RbItem*)itm;
- (void)scaleFactorChanged:(NSNotification*)notification;
- (IBAction)selectAll:(id)sender;
- (void)selectAllItems;
- (void)selectCursorOverForItemsAtPoint:(NSPoint)p;
- (ItemSelector)selectItem:(NSPoint)forPoint;
- (void)selectItemsInSweepArea;
- (ItemSelector)selectItemOnly:(NSPoint)forPoint;
- (ItemSelector)selectNonPlateItemOnly:(NSPoint)forPoint;
- (void)setAnalysisDocumentPtr:(AnalysisDocument*)dp;
- (void)setCorners;
- (void)setItems:(NSMutableArray*)itms;
- (void)setShowGrid:(BOOL)x;
- (BOOL)showGrid;
- (IBAction)showParameterPalette:(id)sender;
- (void)showTrackingAreas;
- (IBAction)showGridAction:(id)sender;
- (IBAction)snapToGridAction:(id)sender;
- (void)startItemTipTimerForItem:(RbItem*)itm;
- (void)stopItemTipTimer;
- (void)transformToBottomLeftCoordinates:(NSPoint*)forPoint;
- (void)transformToTopLeftCoordinates:(NSPoint*)forPoint;
- (void)updateAllTrackingAreas;
- (void)updateBackgroundColor:(NSNotification*)notification;
- (void)updateScrollBars;
- (BOOL)validateMenuItem:(NSMenuItem*)item;
- (void)windowDidResize:(NSNotification*)notification;
- (void)writeToPasteboard:(NSPasteboard*)pb;

@end
