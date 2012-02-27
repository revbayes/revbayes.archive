#import <Cocoa/Cocoa.h>
#import "RbItem.h"
@class AnalysisView;
@class Inlet;
@class InOutlet;
@class Outlet;
@class YRKSpinningProgressIndicator;


@interface Tool : RbItem <NSCoding> {

    AnalysisView*                 myAnalysisView;
    NSMutableArray*               inlets;
    NSMutableArray*               outlets;
	int                           flagCount;
	BOOL                          touchOnRevival;
    BOOL                          isVisited;
    BOOL                          isCurrentlyExecuting;
    BOOL                          isLoop;
    YRKSpinningProgressIndicator* progressIndicator;
    NSString*                     workspaceName;
}

@property (readwrite)        int       flagCount;
@property (readwrite)        BOOL      touchOnRevival;
@property (readwrite)        BOOL      isCurrentlyExecuting;
@property (readwrite)        BOOL      isLoop;
@property (readwrite)        BOOL      isVisited;
@property (readwrite,retain) NSString* workspaceName;

- (void)addInletOfColor:(NSColor*)c;
- (void)addOutletOfColor:(NSColor*)c;
- (NSColor*)colorOfInletIndexed:(int)idx;
- (NSColor*)colorOfOutletIndexed:(int)idx;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (void)execute;
- (InOutlet*)findInOutletWithColor:(NSColor*)c;
- (Tool*)getParentToolOfInlet:(Inlet*)inlt;
- (Tool*)getParentToolOfInletIndexed:(int)idx;
- (BOOL)hasParents;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (id)initWithScaleFactor:(float)sf andWindowNibName:(NSString*)wNibName;
- (Inlet*)inletIndexed:(int)idx;
- (void)initializeImage;
- (BOOL)isFullyConnected;
- (BOOL)isSomeParentVisited;
- (int)numInlets;
- (int)numInletsOfColor:(NSColor*)c;
- (int)numOutlets;
- (int)numOutletsOfColor:(NSColor*)c;
- (NSPoint)originForControlWindow:(NSWindow*)win;
- (Outlet*)outletIndexed:(int)idx;
- (void)removeAllConnections;
- (void)removeAllConnectionsToInlets;
- (void)removeAllConnectionsToOutlets;
- (void)removeAllInletsAndOutlets;
- (void)removeAllInlets;
- (void)removeAllOutlets;
- (void)removeFilesFromTemporaryDirectory;
- (void)removeInletOfColor:(NSColor*)c;
- (NSMutableAttributedString*)sendTip;
- (void)setAnalysisView:(AnalysisView*)av;
- (void)setImageWithSize:(NSSize)s;
- (void)setInletLocations;
- (void)setOutletLocations;
- (void)showControlPanel;
- (void)showInspectorPanel;
- (void)startProgressIndicator;
- (void)stopProgressIndicator;
- (void)updateForChangeInState;
- (void)updateDownstreamTools;

@end
