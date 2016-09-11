#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class GuiTree;
@class TreeTaxonBipartitions;
@class WindowControllerTreeConsensus;
@class WindowControllerTreeConsensusViewer;




@interface ToolTreeConsensus : Tool <NSCoding> {

    TreeTaxonBipartitions*                  myParts;
    WindowControllerTreeConsensus*          controlWindow;
    WindowControllerTreeConsensusViewer*    treeInspector;
    GuiTree*                                consensusTree;
    BOOL                                    isConsensusTreeWindowOpen;
    NSString*                               outgroupName;
    float                                   partitionFrequencies;
    BOOL                                    showAllCompatiblePartitions;
}

@property (nonatomic)        BOOL           isConsensusTreeWindowOpen;
@property (nonatomic,strong) GuiTree*       consensusTree;
@property (nonatomic,strong) NSString*      outgroupName;
@property (nonatomic)        float          partitionFrequencies;
@property (nonatomic)        BOOL           showAllCompatiblePartitions;

- (void)addTree:(GuiTree*)t withWeight:(float)w ;
- (void)closeControlPanel;
- (void)closeInspectorPanel;

@end
