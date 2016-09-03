#import <Cocoa/Cocoa.h>



@interface RbItem : NSWindowController {

    BOOL                      hasInspectorInfo;
    BOOL                      hasController;
    BOOL                      isSelected;
	BOOL                      isCursorOver;
	NSPoint                   itemLocation;
	NSSize                    itemSize;
	NSImage*                  itemImage[8];
    BOOL                      isResolved;
    BOOL                      showTip;
	BOOL                      isPlate;
    BOOL                      saveAsModelTemplate;
	float                     currentScaleFactor;
}

@property (nonatomic) BOOL    hasInspectorInfo;
@property (nonatomic) BOOL    hasController;
@property (nonatomic) BOOL    isSelected;
@property (nonatomic) BOOL    isCursorOver;
@property (nonatomic) NSPoint itemLocation;
@property (nonatomic) NSSize  itemSize;
@property (nonatomic) BOOL    isResolved;
@property (nonatomic) BOOL    showTip;
@property (nonatomic) BOOL    isPlate;
@property (nonatomic) BOOL    saveAsModelTemplate;
@property (nonatomic) float   currentScaleFactor;

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
