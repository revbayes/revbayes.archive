#import <Cocoa/Cocoa.h>
#import "RbView.h"
@class PaletteView;
@class Parm;
@class ParmPlate;
@class ParmPlateTree;
@class ParmTree;
@class ToolModel;



@interface ModelView : RbView {

    float             parmNameHeight;     // height of parameter names for deterimining the bottom-left corner
    int               parmCount;          // keeps track of the number of parameters created
    NSMutableArray*   selectedLinks;
	BOOL              showParmNames;      // whether the parameter names should be displayed
	ToolModel*        myTool;
	char              availableIndices[27];
    PaletteView*      parameterDrawObj;
}

@property (nonatomic)        int        parmCount;
@property (nonatomic)        BOOL       showParmNames;
@property (nonatomic,strong) ToolModel* myTool;

- (void)addToSelectedLinkListParent:(Parm*)x andChild:(Parm*)y;
- (void)checkPlateMembership;
- (float)distanceFromPoint:(NSPoint)a toPoint:(NSPoint)b;
- (float)distanceOfPoint:(NSPoint)a0 fromLineDefinedByPoint:(NSPoint)a1 andPoint:(NSPoint)a2;
- (BOOL)doesLineWithEndsA:(CGPoint)a andB:(CGPoint)b intersectRect:(CGRect)r;
- (NSPoint)findClosestPointToTreeForPlate:(Parm*)tp;
- (NSPoint)findClosestPointBetweenTree:(ParmTree*)t andTreePlate:(ParmPlateTree*)tp;
- (NSString*)getNextParameterName;
- (BOOL)hasItemCollided:(RbItem*)itm;
- (Parm*)instantiateParm:(NSString*)pName;
- (BOOL)isLinkSelectedBetweenParent:(Parm*)x andChild:(Parm*)y;
- (BOOL)isNameTaken:(NSString*)nme;
- (BOOL)isCursorAtPoint:(NSPoint)p overTreePlate:(ParmPlate*)plt;
- (BOOL)isCursorOverTreePlate:(NSPoint)p ;
- (void)mouseDown:(NSEvent*)event;
- (NSPoint)newPointDistance:(float)d betweenPoint:(NSPoint)p1 andPoint:(NSPoint)p2;
- (BOOL)readFromPasteboard:(NSPasteboard*)pb;
- (void)removeAllFocusRings;
- (void)removeSelectedConnections;
- (void)removeLinkSelectedBetweenParent:(Parm*)x andChild:(Parm*)y;
- (void)scaleFactorChanged:(NSNotification*)notification;
- (void)selectAllConnections;
- (void)selectAllItems;
- (ItemSelector)selectItem:(NSPoint)forPoint;
- (void)selectItemsInSweepArea;
- (ItemSelector)selectPlateItemOnly:(NSPoint)forPoint;
- (void)setCorners;
- (void)setIndices;
- (void)setItems:(NSMutableArray*)itm;
- (void)showParameters:(NSMutableArray*)a;
- (void)showSelectedLinks;
- (void)updateBackgroundColor:(NSNotification*)notification;
- (void)writeToPasteboard:(NSPasteboard*)pb;

@end
