#import <Cocoa/Cocoa.h>
#import "RbItem.h"
@class ParmPlate;
@class ParmPlateTree;
@class ToolModel;



@interface Variable : RbItem <NSCoding> {

    NSMutableArray*                       parents;           // the parents of this parameter
    NSMutableArray*                       children;          // the children of this parameter
    NSString*                             parmName;          // the parameter name
    int                                   parmType;          // the parameter type
    int                                   domain;            // the domain of the parameter (e.g., real numbers, integers, etc.)
	ToolModel*                            myModel;           // a pointer to the model tool
	NSMutableArray*                       containingPlates;  // the plates that this parameter sits on
	NSWindow*                             parmsWindow;       // a pointer to the window the parameter lives on
    int                                   drawingIndex;
    BOOL                                  isConstant;        // is the parameter a constant
    BOOL                                  isVector;
    int                                   dimensions;

	IBOutlet NSButton*                    cancelButton;
	IBOutlet NSButton*                    helpButton;
	IBOutlet NSTextField*                 nameField;
	IBOutlet NSTextField*                 nameLabel;
	IBOutlet NSButton*                    okButton;
}

@property (nonatomic)        int          domain;
@property (nonatomic)        int          drawingIndex;
@property (nonatomic,strong) NSString*    parmName;
@property (nonatomic)        int          parmType;
@property (nonatomic,strong) NSWindow*    parmsWindow;
@property (nonatomic,strong) ToolModel*   myModel;
@property (nonatomic)        BOOL         isConstant;
@property (nonatomic,strong) NSTextField* nameField;
@property (nonatomic,strong) NSTextField* nameLabel;
@property (nonatomic)        BOOL         isVector;
@property (nonatomic)        int          dimensions;

- (void)addChild:(Variable*)p;
- (void)addParent:(Variable*)p;
- (void)addToPlate:(ParmPlate*)p;
- (ParmPlateTree*)assignedTreePlate;
- (IBAction)cancelAction:(id)sender;
- (IBAction)changeName:(id)sender;
- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (Variable*)getChildIndexed:(int)x;
- (Variable*)getParentIndexed:(int)x;
- (Variable*)getParentWithName:(NSString*)str;
- (ParmPlate*)getPlateIndexed:(int)x;
- (IBAction)helpAction:(id)sender;
- (void)initializeImage;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;
- (BOOL)isAssignedToPlate:(ParmPlate*)p;
- (ParmPlate*)isOnPlateWithIndexSource:(int)src;
- (int)numAssignedPlates;
- (int)numChildren;
- (int)numParents;
- (int)numAttachedMatrices;
- (int)numCharactersForMatrix:(int)idx;
- (int)numExcludedCharactersForMatrix:(int)idx;
- (int)numExcludedTaxaForMatrix:(int)idx;
- (int)numTaxaForMatrix:(int)idx;
- (IBAction)okAction:(id)sender;
- (NSPoint)originForControlWindow:(NSWindow*)win;
- (void)refreshWindow;
- (void)removeAllConnections;
- (void)removeChild:(Variable*)c;
- (void)removeFromAllPlates;
- (void)removeFromPlate:(ParmPlate*)p;
- (void)removeParent:(Variable*)p;
- (void)resizeWindowTo:(float)h;
- (void)setImageWithSize:(NSSize)s;
- (void)showControlPanel;
- (void)updateChildren;

@end
