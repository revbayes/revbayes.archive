#import "WindowControllerModelSubmission.h"


@implementation WindowControllerModelSubmission

@synthesize submitModelToPublic;
@synthesize modelName;
@synthesize creatorName;
@synthesize notes;

- (void)awakeFromNib {

}

- (IBAction)cancelAction:(id)sender {

    [NSApp stopModal];
    [self close];
}

- (IBAction)changedModelName:(id)sender {

	if ( [self doesModelExist:[modelNameField stringValue]] == YES )
        {
        NSString* eStr = [NSString stringWithFormat:@"The file %@ already exists. Please choose another name.", [modelNameField stringValue]];
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Error"];
        [alert setInformativeText:eStr];
        [alert runModal];
		//NSRunAlertPanel(@"Error", @"The file %@ already exists. Please choose another name.", @"OK", nil, nil, [modelNameField stringValue]);
        }
}

- (void)cleanWindow {

	// get a default name for the model
	BOOL goodName = NO;
	int i = 1;
	while (goodName == NO)
		{
		char temp[100];
		sprintf(temp, "Untitled Model %d", i);
		NSString* fn = [NSString stringWithCString:temp encoding:[NSString defaultCStringEncoding]];
		i++;
		NSString* possibleFileName = @"~/Library/Application Support/RevBayes/User Models/";
		possibleFileName = [possibleFileName stringByAppendingString:fn];
		possibleFileName = [possibleFileName stringByExpandingTildeInPath];
		
		NSFileManager* fileManager = [NSFileManager defaultManager];
		if ([fileManager fileExistsAtPath:possibleFileName] == NO)
			{
			goodName = YES;
			modelName = [NSString stringWithString:[possibleFileName lastPathComponent]];
			[modelNameField setStringValue:[possibleFileName lastPathComponent]];
			}
		}

	// get the name of the user
	NSString* userName = NSFullUserName();
	creatorName = [NSString stringWithString:userName];
	[creatorNameField setStringValue:userName];
}

- (BOOL)doesModelExist:(NSString*)mn {

	NSString* newModelName = @"~/Library/Application Support/RevBayes/User Models/";
	newModelName = [newModelName stringByAppendingString:mn];
	newModelName = [newModelName stringByExpandingTildeInPath];
	NSFileManager* fileManager = [NSFileManager defaultManager];
	if ([fileManager fileExistsAtPath:newModelName] == YES)
		return YES;
	return NO;
}

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

	if ( (self = [super initWithWindowNibName:@"ModelSubmission"]) )
        {
		submitModelToPublic = NO;
        modelName   = @"";
        creatorName = @"";
        notes       = @"";
        }
	return self;
}

- (IBAction)okAction:(id)sender {

	if ( [self doesModelExist:[modelNameField stringValue]] == YES )
		{
        NSString* eStr = [NSString stringWithFormat:@"The file %@ already exists. Please choose another name.", [modelNameField stringValue]];
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Error"];
        [alert setInformativeText:eStr];
        [alert runModal];
		//NSRunAlertPanel(@"Error", @"The file %@ already exists. Please choose another name.", @"OK", nil, nil, [modelNameField stringValue]);
		return;
		}
    [NSApp stopModal];
    [self close];
}

- (void)windowDidLoad {

}

@end
