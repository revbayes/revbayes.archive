#import "ToolAlign.h"
#import "WindowControllerAlign.h"



@implementation WindowControllerAlign


- (void)awakeFromNib {

}

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolAlign*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowAlign"]) )
        {
        myTool        = t;


		// set default values for the window
            
        // pick up Clustal values from myTool and bind
        
        }
	return self;
}

- (void)windowDidLoad {
    
}

- (IBAction)okButtonAction:(id)sender {
    NSFileManager *alignClustalFileManager;
    alignClustalFileManager = [[NSFileManager alloc] init];
    
    NSDictionary *alignClustalTemporaryDirectoryAttributes;
    alignClustalTemporaryDirectoryAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"alignClustalTemporaryDirectory"];
    
    NSString *alignClustalUserTemporaryDirectory;
    alignClustalUserTemporaryDirectory = NSTemporaryDirectory();
    
    [alignClustalFileManager createDirectoryAtPath:alignClustalUserTemporaryDirectory withIntermediateDirectories:NO attributes:alignClustalTemporaryDirectoryAttributes error:NULL];
    
    NSString *alignClustalPath = @"";
    
    NSString *alignClustalExecutable = @"clustalw2";
    
    
// Ask John where to put clustalw2--for now, use one on my hard drive
    //    NSString *alignClustalDirectory = [[NSBundle mainBundle] pathForResource:alignClustalExecutable ofType:nil];
    NSString *alignClustalDirectory = @"/Users/Edna/Documents/Cocoa_projects/Clustal/clustalw2";
    
    //       alignClustalPath = @"/Users/edna/Documents/Cocoa_projects/RB Clustal Pipe/RB Clustal Pipe/";
    alignClustalPath = alignClustalDirectory;
    
    NSLog (@"alignClustalDirectory = %@", alignClustalDirectory);
    NSLog (@"alignClustalExecutable = %@", alignClustalExecutable);
    NSLog (@"alignClustalPath = %@", alignClustalPath);
    
    NSString *alignClustalReduceConsoleOutput = @"-QUIET";
    NSString *alignClustalAlign = @"-ALIGN";
    NSString *alignClustalInfile = @"-INFILE=/Users/edna/Documents/Cocoa_projects/Clustal/nYAL001C.fas";
    NSString *alignClustalOutfile = @"-OUTFILE=";
    alignClustalOutfile = [alignClustalOutfile stringByAppendingString:alignClustalUserTemporaryDirectory];
    alignClustalOutfile = [alignClustalOutfile stringByAppendingString:@"clustaloutput.aln"];
    NSString *alignClustalGuideTree = @"-NEWTREE=";
    alignClustalGuideTree = [alignClustalGuideTree stringByAppendingString: alignClustalUserTemporaryDirectory];
    alignClustalGuideTree = [alignClustalGuideTree stringByAppendingString: @"clustaltree.dnd"];
    
    NSLog(@"alignClustalOutfile = %@", alignClustalOutfile);
    NSLog(@"alignClustalGuideTree = %@", alignClustalGuideTree);
    
    NSArray *alignClustalArguments;
    alignClustalArguments = [NSArray arrayWithObjects: alignClustalReduceConsoleOutput, alignClustalInfile, alignClustalOutfile, alignClustalGuideTree, alignClustalAlign, nil];
    
    alignClustalTask = [[NSTask alloc] init];
    [alignClustalTask setLaunchPath: alignClustalPath];
    [alignClustalTask setCurrentDirectoryPath: alignClustalPath];
    [alignClustalTask setArguments: alignClustalArguments];
    
    alignClustalFromPipe = [NSPipe pipe];
    alignClustalFromClustal = [alignClustalFromPipe fileHandleForReading];
    [alignClustalTask setStandardOutput: alignClustalFromPipe];
    
    NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];
    
    [defaultCenter addObserver: self
                      selector: @selector(receiveData:)
                          name: NSFileHandleReadCompletionNotification
                        object: alignClustalFromClustal];
    
    alignClustalToPipe = [NSPipe pipe];
    alignClustalToClustal = [alignClustalToPipe fileHandleForWriting];
    [alignClustalTask setStandardInput: alignClustalToPipe];
    
//    alignClustalErrorPipe = [NSPipe pipe];
//    alignClustalErrorData = [alignClustalErrorPipe fileHandleForReading];
//    [alignClustalTask setStandardError: alignClustalErrorPipe];
    
//    [defaultCenter addObserver: self
//                      selector: @selector(alignClustalErrorDataAvailable:)
//                          name: NSFileHandleReadCompletionNotification
//                        object: alignClustalErrorData];
    
    
    [alignClustalTask launch];
    
    [alignClustalFromClustal readInBackgroundAndNotify];
//    [alignClustalErrorData readInBackgroundAndNotify];

    [myTool closeControlPanel];
}

@end
