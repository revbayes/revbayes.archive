#import <Foundation/Foundation.h>
#import "ToolData.h"
@class RbData;
@class WindowControllerCharacterMatrix;
@class WindowControllerReadData;




@interface ToolDataEntry : ToolData <NSCoding> {

	int                                numberOfTaxa;
	int                                numberOfCharacters;
    WindowControllerReadData*          controlWindow;
}

@property (readwrite)        int       numberOfCharacters;
@property (readwrite)        int       numberOfTaxa;

- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (BOOL)readDataFile;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;

@end
