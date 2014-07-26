#import <Cocoa/Cocoa.h>
#import "ToolData.h"
@class WindowControllerConcatenate;

#define MINIMAL_OVERLAP_METHOD  0
#define MAXIMAL_OVERLAP_METHOD  1
#define MERGE_BY_DATA_TYPE      0
#define MERGE_UNCONDITIONALLY   1


@interface ToolConcatenate : ToolData <NSCoding> {

    WindowControllerConcatenate*   controlWindow;
    BOOL                           useMinimalSet;
    BOOL                           matchUsingNames;
    int                            mergeMethod;
}

@property (nonatomic) BOOL         useMinimalSet;
@property (nonatomic) BOOL         matchUsingNames;
@property (nonatomic) int          mergeMethod;

- (void)assembleNames:(NSMutableArray*)names usingMethod:(int)overlapMethod fromMatrices:(NSMutableArray*)alignedData;
- (void)closeControlPanel;
- (BOOL)concatenateWithOverlap:(int)overlapMethod andMergeMethod:(int)mergeMethod;
- (NSMutableArray*)concatenateMatrices:alignedData forTaxa:(NSMutableArray*)names usingMergeMethod:(int)mergeMethod;
- (int)numberOfIncomingAlignments;
- (int)mininumNumberOfIncomingSequences;
- (int)maximumNumberOfIncomingSequences;
- (void)showControlPanel;

@end
