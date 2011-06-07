#import <Cocoa/Cocoa.h>
@class WindowControllerNoteBook;



@interface AnalysisTools : NSObject <NSCoding> {

	NSString*                             analysisName;
	NSMutableArray*                       tools;
	WindowControllerNoteBook*             noteBook;
	NSMutableAttributedString*            notebookContents;
}

@property (readonly,retain) NSMutableArray* tools;

- (NSString*)analysisName;
- (void)scaleToolsByScale:(float)s andFactor:(float)f;
- (void)setAnalysisName:(NSString*)aName;
- (void)setNotebookString:(NSAttributedString*)newValue;
- (void)showNoteBook;
- (void)touchAllTools;

@end
