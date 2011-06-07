#import <Cocoa/Cocoa.h>



@interface RbItem : NSWindowController {

    BOOL                 hasInspectorInfo;
    BOOL                 isSelected;
	BOOL                 isCursorOver;
	NSPoint              itemLocation;
	NSSize               itemSize;
	NSImage*             itemImage[8];
    BOOL                 isResolved;
    BOOL                 showTip;
	BOOL                 isPlate;
    BOOL                 saveAsModelTemplate;
	float                currentScaleFactor;
}

@property (readwrite) BOOL hasInspectorInfo;
@property (readwrite) BOOL isSelected;
@property (readwrite) BOOL isCursorOver;
@property (readwrite) NSPoint itemLocation;
@property (readwrite) NSSize itemSize;
@property (readwrite) BOOL isResolved;
@property (readwrite) BOOL showTip;
@property (readwrite) BOOL isPlate;
@property (readwrite) BOOL saveAsModelTemplate;
@property (readwrite) float currentScaleFactor;

- (void)flipSelected;
- (void)initializeImage;
- (id)initWithScaleFactor:(float)sf;
- (id)initWithScaleFactor:(float)sf andWindowNibName:(NSString*)wNibName;
- (NSPoint)itemCenter;
- (NSImage*)itemImageWithIndex:(int)idx;
- (void)removeAllConnections;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;
- (void)showInspectorPanel;
- (NSString*)xibName;

@end
