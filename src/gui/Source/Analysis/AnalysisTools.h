#import <Cocoa/Cocoa.h>
@class AnalysisView;
@class WindowControllerNoteBook;



@interface AnalysisTools : NSObject <NSCoding> {

	NSString*                             analysisName;
	NSMutableArray*                       tools;
	WindowControllerNoteBook*             noteBook;
	NSMutableAttributedString*            notebookContents;
}

@property (nonatomic,strong) NSMutableArray* tools;

- (NSString*)analysisName;
- (void)scaleToolsByScale:(float)s andFactor:(float)f;
- (void)setAnalysisName:(NSString*)aName;
- (void)setAnalysisView:(AnalysisView*)av;
- (void)setNotebookString:(NSAttributedString*)newValue;
- (void)showNoteBook;
- (void)touchAllTools;

@end
