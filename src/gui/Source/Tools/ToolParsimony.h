#import <Cocoa/Cocoa.h>
#import "Tool.h"
#include <vector>
@class Tree;
@class WindowControllerParsimony;




@interface ToolParsimony : Tool <NSCoding> {

    WindowControllerParsimony*          controlWindow;
    int                                 numTreesVisited;
    unsigned**                          stateSets;
    unsigned**                          stateSetsPtr;
    int                                 numCharacters;
    int                                 scoreOfBestTree;
    NSMutableArray*                     bestTrees;
}

- (void)addTaxonFromList:(NSMutableArray*)availableTaxa toTree:(NSMutableArray*)nodes usingSpareNodes:(NSMutableArray*)spares treeObject:(Tree*)t;
- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (void)execute;
- (void)exhaustiveSearch;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (void)showControlPanel;

@end
