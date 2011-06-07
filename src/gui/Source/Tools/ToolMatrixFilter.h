#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class RbData;
@class WindowControllerCharacterMatrix;
@class WindowControllerMatrixFilter;



@interface ToolMatrixFilter : Tool <NSCoding> {

    WindowControllerMatrixFilter*      controlWindow;
    WindowControllerCharacterMatrix*   dataInspector;	
	NSMutableArray*                    dataMatrices;
}

@property (readwrite,assign) NSMutableArray* dataMatrices;

- (void)addMatrix:(RbData*)m;
- (void)closeControlPanel;
- (RbData*)dataMatrixIndexed:(int)i;
- (void)encodeWithCoder:(NSCoder *)aCoder;
- (id)initWithScaleFactor:(float)sf;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (int)numDataMatrices;
- (void)removeAllDataMatrices;
- (NSMutableAttributedString*)sendTip;
- (void)setInletsAndOutlets;
- (void)setOutlets;
- (void)showControlPanel;
- (void)showInspectorPanel;
- (void)updateForConnectionChange;

@end
