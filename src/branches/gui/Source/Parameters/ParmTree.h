#import <Cocoa/Cocoa.h>
#import "ParmRandomVariable.h"
@class TaxonBipartition;

#define NO_SOURCE    0
#define FIRST_MATRIX 1
#define ITH_MATRIX   2



@interface ParmTree : ParmRandomVariable <NSCoding> {

    int                       rootednessType;
    NSMutableArray*           bipartitions;
    int                       treeSizeSource;
    int                       originalRootednessType;
    int                       unrootedDistType;
    int                       rootedDistType;
    
    IBOutlet NSPopUpButton*   sourceSelector;
    IBOutlet NSPopUpButton*   fixedTopologySelector;
    IBOutlet NSPopUpButton*   rootednessTypeSelector;
	IBOutlet NSTextField*     treeSizeField;
    IBOutlet NSTextField*     fixedValueField;
    IBOutlet NSPopUpButton*   partitionsList;
    IBOutlet NSButton*        partitionAddButton;
    IBOutlet NSBox*           clampDivider;
}

@property (readwrite) int rootednessType;
@property (readwrite) int treeSizeSource;
@property (readwrite) int originalRootednessType;

- (IBAction)addBipartitionAction:(id)sender;
- (void)addUserBipartition:(TaxonBipartition*)bp;
- (IBAction)changeRootednessTypeAction:(id)sender;
- (IBAction)changeTreeSizeSourceAction:(id)sender;
- (BOOL)doesTreeOrderPlate;
- (TaxonBipartition*)getBipartitionIndexed:(int)idx;
- (NSString*)getNextDefaultPartitionName;
- (void)initializeImage;
- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;
- (BOOL)isBipartitionNameTaken:(NSString*)nme;
- (int)numBipartitions;
- (int)numTaxa;

@end
