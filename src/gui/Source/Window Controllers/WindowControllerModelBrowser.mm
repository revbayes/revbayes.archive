#import "ModelList.h"
#import "WindowControllerModelBrowser.h"
#import "WindowControllerModel.h"



@implementation WindowControllerModelBrowser

@synthesize curatedModels;
@synthesize userDidCancel;
@synthesize userModels;
@synthesize userSelectedCuratedModel;

- (void)awakeFromNib {

	[self readCuratedModels];
	[self readUserModels];
    NSTabViewItem* item = [tabView selectedTabViewItem];
    NSString* itemLabel = [item label];
    if ( [itemLabel isEqualToString:@"Curated"] == YES )
        [removeUserModelButton setHidden:YES];
    else 
        [removeUserModelButton setHidden:NO];
}

- (IBAction)cancelButtonAction:(id)sender {

    [self setUserDidCancel:YES];
    [NSApp stopModal];
    [self close];
}

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithModelWindow:nil];
}

- (id)initWithModelWindow:(WindowControllerModel*)mc {

	if ( (self = [super initWithWindowNibName:@"ModelBrowser"]) )
        {
        modelWindowPtr = mc;
        curatedModels = [[NSMutableArray alloc] init];
        userModels = [[NSMutableArray alloc] init];
        }
	return self;
}

- (IBAction)okButtonAction:(id)sender {

    NSTabViewItem* item = [tabView selectedTabViewItem];
    NSString* itemLabel = [item label];
    userSelectedCuratedModel = NO;
    if ( [itemLabel isEqualToString:@"Curated"] == YES )
        userSelectedCuratedModel = YES;

    [self setUserDidCancel:NO];
    [NSApp stopModal];
    [self close];
}

- (void)readCuratedModels {

    // get a path to the RevBayes support folder
    NSFileManager* fileManager = [NSFileManager defaultManager];
    NSString* folder = @"~/Library/Application Support/RevBayes/Curated Models/";
    folder = [folder stringByExpandingTildeInPath];
    if ([fileManager fileExistsAtPath:folder] == NO)
        {
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Error"];
        [alert setInformativeText:@"Cannot find model folder."];
        [alert runModal];
        //NSRunAlertPanel(@"Error", @"Cannot find model folder", @"OK", nil, nil);
        return;
        }

    // fill in an array containing the files in that directory
    NSArray* modelFiles = [fileManager contentsOfDirectoryAtPath:folder error:nil];

    // enumerate the files
    [curatedModels removeAllObjects];
	id element;
	NSEnumerator* modelEnumerator = [modelFiles objectEnumerator];
	while ( (element = [modelEnumerator nextObject]) )
		{
        if ( [element characterAtIndex:0] != '.' )
            {
            ModelList* m = [[ModelList alloc] init];
            [m setModelName:element];
            [curatedModels addObject:m];
            }
		}
        
    if ([curatedModels count] > 0)
        [curatedModelController setSelectionIndex:0];
        
#   if 0
    modelEnumerator = [curatedModels objectEnumerator];
	while (element = [modelEnumerator nextObject])
        NSLog(@"Read curated model %@", [element modelName]);
#   endif
}

- (void)readUserModels {

    // get a path to the RevBayes support folder
    NSFileManager* fileManager = [NSFileManager defaultManager];
    NSString* folder = @"~/Library/Application Support/RevBayes/User Models/";
    folder = [folder stringByExpandingTildeInPath];
    if ([fileManager fileExistsAtPath:folder] == NO)
        {
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Error"];
        [alert setInformativeText:@"Cannot find user model folder."];
        [alert runModal];
        //NSRunAlertPanel(@"Error", @"Cannot find user model folder", @"OK", nil, nil);
        return;
        }

    // fill in an array containing the files in that directory
    NSArray* modelFiles = [fileManager contentsOfDirectoryAtPath:folder error:nil];

    // enumerate the files
    [userModels removeAllObjects];
	id element;
	NSEnumerator* modelEnumerator = [modelFiles objectEnumerator];
	while ( (element = [modelEnumerator nextObject]) )
		{
        if ( [element characterAtIndex:0] != '.' )
            {
            ModelList* m = [[ModelList alloc] init];
            [m setModelName:element];
            [userModels addObject:m];
            }
		}

    if ([userModels count] > 0)
        [userModelController setSelectionIndex:0];

#   if 0
    modelEnumerator = [userModels objectEnumerator];
	while (element = [modelEnumerator nextObject])
        NSLog(@"Read user model %@", [element modelName]);
#   endif
}

- (IBAction)removeUserModelAction:(id)sender {

    // get the name of the model to remove
    NSString* nameOfModelToRemove = [[[userModelController selectedObjects] objectAtIndex:0] modelName];
    
    // remove the file
	NSString* selectedModelName = @"~/Library/Application Support/RevBayes/User Models/";
	selectedModelName = [selectedModelName stringByAppendingString:nameOfModelToRemove];
	selectedModelName = [selectedModelName stringByExpandingTildeInPath];
    
    // remove the model file
    NSFileManager* fm = [NSFileManager defaultManager];
    NSError* error = nil;
    [fm removeItemAtPath:selectedModelName error:&error];
    if (error != nil)
        {
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Error"];
        [alert setInformativeText:@"Could not remove user model."];
        [alert runModal];
        //NSRunAlertPanel(@"Error", @"Could not remove user model", @"OK", nil, nil);
        return;
        }

    // remove the object from the array controller
    [userModelController removeObject:[[userModelController selectedObjects] objectAtIndex:0]];
}

- (NSString*)selectedModel {

	if (userSelectedCuratedModel == YES)
		return [[[curatedModelController selectedObjects] objectAtIndex:0] modelName];
	else 
		return [[[userModelController selectedObjects] objectAtIndex:0] modelName];
}

- (void)tabView:(NSTabView*)tv didSelectTabViewItem:(NSTabViewItem*)tabViewItem {

    NSTabViewItem* item = [tv selectedTabViewItem];
    NSString* itemLabel = [item label];
    if ( [itemLabel isEqualToString:@"Curated"] == YES )
        [removeUserModelButton setHidden:YES];
    else 
        [removeUserModelButton setHidden:NO];
}

@end
