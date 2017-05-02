#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class DistributionListGui;
@class WindowControllerModel;
@class WindowControllerModelBrowser;
@class WindowControllerModelSubmission;



@interface ToolModel : Tool <NSCoding> {

    WindowControllerModel*                        controlWindow;         // a pointer to the control window
    WindowControllerModelBrowser*                 modelBrowser;
	WindowControllerModelSubmission*              modelSubmission;
	NSMutableArray*                               parms;                 // an array holding the pointers to the parameters
	NSMutableArray*                               dataMatrices;
    BOOL                                          possibleInlets[4];
    NSModalSession                                mySession;
    DistributionListGui*                          distributionList;
    NSMutableArray*                               variableList;
}

@property (nonatomic,strong) NSMutableArray*      dataMatrices;
@property (nonatomic,strong) DistributionListGui* distributionList;
@property (nonatomic,strong) NSMutableArray*      parms;

- (NSMutableArray*)allocateParms;
- (int)calculateNumTaxa;
- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder *)aCoder;
- (BOOL)execute;
- (void)exportModel;
- (void)importModel;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (void)initializePallet;
- (BOOL)isPlateIndexAvailableForChar:(char)c;
- (BOOL)isInletActiveWithIndex:(int)idx;
- (int)numAttachedMatrices;
- (int)numCharactersForMatrix:(int)idx;
- (int)numExcludedCharactersForMatrix:(int)idx;
- (int)numExcludedTaxaForMatrix:(int)idx;
- (int)numTaxaForMatrix:(int)idx;
- (NSArray*)numTaxaForMatrices;
- (NSMutableAttributedString*)sendTip;
- (void)setInletsAndOutlets;
- (void)setPossibleInletTo:(BOOL)tf forPossibility:(int)idx;
- (void)showControlPanel;
- (void)touchAllParameters;
- (void)updateInlets;
- (NSMutableArray*)variables;

@end
