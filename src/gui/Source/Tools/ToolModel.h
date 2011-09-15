#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class DistributionListGui;
@class WindowControllerModel;
@class WindowControllerModelBrowser;
@class WindowControllerModelSubmission;



@interface ToolModel : Tool <NSCoding> {

    WindowControllerModel*             controlWindow;         // a pointer to the control window
    WindowControllerModelBrowser*      modelBrowser;
	WindowControllerModelSubmission*   modelSubmission;
	NSMutableArray*                    parms;                 // an array holding the pointers to the parameters
	NSMutableArray*                    dataMatrices;
    BOOL                               possibleInlets[4];
    NSModalSession                     mySession;
    DistributionListGui*               distributionList;
}

@property (readwrite,assign) NSMutableArray* dataMatrices;
@property (retain) DistributionListGui* distributionList;
@property (readwrite,assign) NSMutableArray* parms;

- (NSMutableArray*)allocateParms;
- (int)calculateNumTaxa;
- (void)closeControlPanel;
- (void)deleteParms;
- (void)encodeWithCoder:(NSCoder *)aCoder;
- (void)exportModel;
- (void)importModel;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
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

@end
