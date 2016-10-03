#import "AnalysisTools.h"
#import "RevBayes.h"
#import "Tool.h"
#import "ToolLoop.h"
#import "WindowControllerNoteBook.h"



@implementation AnalysisTools

@synthesize tools;

- (NSString*)analysisName { 

	return analysisName; 
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

	[aCoder encodeObject:analysisName     forKey:@"analysisName"];
    [aCoder encodeObject:tools            forKey:@"tools"];
	[aCoder encodeObject:notebookContents forKey:@"notebookContents"];
}

- (id)init {

     if ( (self = [super init]) ) 
		{
        [self setAnalysisName:@"My Analysis"];
		tools            = [[NSMutableArray alloc] init];
		notebookContents = [[NSMutableAttributedString alloc] initWithString:@""];

		// allocate the note book
		noteBook = [[WindowControllerNoteBook alloc] initWithAnalysis:self];
		}
    return self;
}
 
- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super init]) ) 
		{
		// unarchive the core information for the analysis
		tools            = [aDecoder decodeObjectForKey:@"tools"];
        analysisName     = [aDecoder decodeObjectForKey:@"analysisName"];
		notebookContents = [aDecoder decodeObjectForKey:@"notebookContents"];

		// allocate the note book
		noteBook = [[WindowControllerNoteBook alloc] initWithAnalysis:self];
		[noteBook setString:notebookContents];
		}
	return self;
}

- (void)scaleToolsByScale:(float)s andFactor:(float)f {

    NSSize sz = NSMakeSize(ITEM_IMAGE_SIZE * s, ITEM_IMAGE_SIZE * s);

	NSEnumerator* toolEnumerator = [tools objectEnumerator];
	id element;
	while ( (element = [toolEnumerator nextObject]) )
		{
		// set the current scale factor for the tool
		[element setCurrentScaleFactor:s];
		
		// change the size of the tool's image
        NSPoint p = [element itemLocation];
        p.x *= f;
        p.y *= f;
        [element setItemLocation:p];
        if ( [element isLoop] == YES )
            {
            NSSize s = [element itemSize];
            s.height *= f;
            s.width *= f;
            [element setItemSize:s];
            //[element setImageWithSize:[(ToolLoop*)element loopRect].size];
            }
        else
            {
            [element setImageWithSize:sz];
            }
        }
}

- (void)setAnalysisView:(AnalysisView*)av {

	NSEnumerator* enumerator = [tools objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
        [element setAnalysisView:av];
		}
}

- (void)setAnalysisName:(NSString*)aName {

    if (aName != analysisName) 
		{
        analysisName = aName;
		
		if ( [noteBook isWindowLoaded] == YES )
			{
			NSString* nbTitle = @"RevBayes Notebook for \"";
			nbTitle = [nbTitle stringByAppendingString:[self analysisName]];
			nbTitle = [nbTitle stringByAppendingString:@"\""];
			[[noteBook window] setTitle:nbTitle];;
			}
		}
}

- (void)setNotebookString:(NSAttributedString*)newValue {

	if (notebookContents != newValue) 
		{
		notebookContents = [newValue copy];
		}
}

- (void)showNoteBook {

	[noteBook showWindow:self];  
	[[noteBook window] makeKeyAndOrderFront:nil];
}

- (void)touchAllTools {

	NSEnumerator* enumerator = [tools objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
		{
		if ( [element touchOnRevival] == YES )
			[element updateForChangeInParent];
		}
}

@end
