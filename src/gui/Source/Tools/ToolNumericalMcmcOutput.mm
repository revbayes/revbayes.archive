//
//  ToolNumericalMcmcOutput.m
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 3/22/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "ToolNumericalMcmcOutput.h"


#import "InOutlet.h"
#import "RevBayes.h"
#import "RbPtr.h"
#import "Trace.h"
#import "WindowControllerNumberMatrix.h"
#import "XmlParser.h"



@implementation ToolNumericalMcmcOutput

@synthesize source;
@synthesize data;

- (void)awakeFromNib {
    [sourceTypeTab selectTabViewItemWithIdentifier:source];
	[self setControlsEnabledState];
	[self setControlWindowSize];
    
}

- (IBAction)cancelAction:(id)sender {
	
	[self closeControlPanel];
}

- (void)closeControlPanel {
    
    [NSApp stopModal];
	[self close];
    [self setInletsAndOutlets];
}

- (void)dealloc {
    
	[source release];
//    [data release];
    data->clear();
    delete data;
    
	[super dealloc];
}

- (void)tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem {
    
    // getting the new selected source (either File or MCMC)
    source = [NSString stringWithString:[[sourceTypeTab selectedTabViewItem] label]];
    if ( [source isEqualToString:@"MCMC"] == YES )
    {
        [okButton setTitle:(@"OK")];
        NSString* msg = @"MCMC output:\nNo further information available yet. Might contain later info about mcmc and model.";
        [mcmcTextField setStringValue:msg];
    }
    else if ( [source isEqualToString:@"Import"] == YES )
    {
        [okButton setTitle:(@"Import")];
        
    }
    else if ( [source isEqualToString:@"Export"] == YES )
    {
        [okButton setTitle:(@"Export")];
        
    }
    
    // updating the outlets and inlets
    [self setInletsAndOutlets];
    
    // resetting the window size
    [self setControlWindowSize];
}


- (void)encodeWithCoder:(NSCoder *)aCoder {
    
	[super encodeWithCoder:aCoder];
    
    [aCoder encodeInt:(int)data->size()           forKey:@"nTraces"];
    for (int i=0; i<(int)data->size(); i++) {
        XmlDocument* doc = new XmlDocument();
        NSString* traceName = [NSString stringWithFormat:@"Trace%lu",i];
        const RbPtr<XmlElement> element = data->at(i)->encode(doc,[traceName cStringUsingEncoding:NSUTF8StringEncoding]);
        doc->addXmlElement(element);
        NSString* tmp_str = [NSString stringWithUTF8String:doc->print().c_str()];
        NSString* key = [NSString stringWithFormat:@"Trace%lu", i];
        [aCoder encodeObject:tmp_str forKey:key];
    }
}

- (id)init {
    
    self = [self initWithScaleFactor:1.0];
    return self;
}

- (id)initWithScaleFactor:(float)sf {
    
    if ( (self = [super initWithScaleFactor:sf andWindowNibName:@"ControlWindowNumericalMcmcOutput"]) ) 
    {
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
		// initialize the inlet/outlet information
		[self addInletOfColor:[NSColor orangeColor]];
		[self addOutletOfColor:[NSColor yellowColor]];
        
        // initialize the settings
		source = [[NSString alloc] initWithString:@"MCMC"];
        
        // initialize the data
        data = new std::vector<RbPtr<Trace> >;
        
        // note that the state of this tool is, by default, resolved
        [self setIsResolved:YES];
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {
    
    if ( (self = [super initWithCoder:aDecoder]) ) 
    {
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
        // initialize the settings
        
        // initialize the data
        data = new std::vector<RbPtr<Trace> >;
        int traces = [aDecoder decodeIntForKey:@"nTraces"];
        for (int i=0; i<traces; i++) {
            NSString* key = [NSString stringWithFormat:@"Trace%lu", i];
            NSString* xmlString = [aDecoder decodeObjectForKey:key];
            
            RbPtr<XmlParser> parser( new XmlParser() );
            RbPtr<const XmlDocument> doc( parser->parse([xmlString cStringUsingEncoding:NSUTF8StringEncoding]) );
            
            RbPtr<const XmlElementAttributed> element( static_cast<const XmlElementAttributed*>( (const XmlElement*)doc->getFirstXmlElement() ) );
            RbPtr<Trace> t(new Trace( doc, element ) );
            data->push_back(t);
        }
    
    }
	return self;
}

- (void)initializeImage {
    
    itemImage[0] = [NSImage imageNamed:@"ToolNumberSet25.icns"];
    itemImage[1] = [NSImage imageNamed:@"ToolNumberSet50.icns"];
    itemImage[2] = [NSImage imageNamed:@"ToolNumberSet75.icns"];
    itemImage[3] = [NSImage imageNamed:@"ToolNumberSet100.icns"];
    itemImage[4] = [NSImage imageNamed:@"ToolNumberSet125.icns"];
    itemImage[5] = [NSImage imageNamed:@"ToolNumberSet150.icns"];
    itemImage[6] = [NSImage imageNamed:@"ToolNumberSet200.icns"];
    itemImage[7] = [NSImage imageNamed:@"ToolNumberSet400.icns"];
    
	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (IBAction)okButtonAction:(id)sender {
    
    // set the tool state to unresolved
    [self setIsResolved:NO];
    
	// remember the state of the control panel
    
	// perform the action
    
    source = [NSString stringWithString:[[sourceTypeTab selectedTabViewItem] label]];
    if ( [source isEqualToString:@"MCMC"] == YES )
    {
        [self closeControlPanel];
        [self updateForConnectionChange];
        [self setIsResolved:YES];
    }
    else if ( [source isEqualToString:@"Import"] == YES )
    {
        // user selected "OK" for a number set to be written to the memory from the mcmc output)
        //		[myTool removeAllDataMatrices];
		BOOL isSuccessful = [self readDataFile];
		
		if (isSuccessful == YES)
        {
            [self setHasInspectorInfo:YES];
			[self closeControlPanel];
			[self updateForConnectionChange];
            [self setIsResolved:YES];
        }
		else 
        {
            NSLog(@"Oh oh. Could not read the MCMC output from a file.");
            
        }
        
    }
    else if ( [source isEqualToString:@"Export"] == YES )
    {
        // user selected "OK" for a number set to be written to the memory from the mcmc output)
        //		[myTool removeAllDataMatrices];
        BOOL isSuccessful = [self writeDataFile];
        
        if (isSuccessful == YES)
        {
            [self closeControlPanel];
            [self updateForConnectionChange];
            [self setIsResolved:YES];
        }
        else 
        {
            NSLog(@"Oh oh. Could not write the MCMC output to a file.");
            
        }
    } 
}

- (BOOL)readDataFile {
    // start the file dialog and get the file name
    
    // make an array containing the valid file types that can be chosen
	NSArray* fileTypes = [NSArray arrayWithObjects: @"csv", @"txt", @"log", @"p", NSFileTypeForHFSTypeCode( 'TEXT' ), nil];
    
    // get the open panel
    NSOpenPanel* oPanel = [NSOpenPanel openPanel];
    [oPanel setAllowsMultipleSelection:NO];
    [oPanel setCanChooseDirectories:NO];
    
    // open the panel
    NSString* fileToOpen;
    int result = (int)[oPanel runModalForDirectory:nil file:nil types:fileTypes];
    if ( result == NSOKButton ) 
    {
        // get the file path/name to the file    
        fileToOpen = [oPanel filename];
        
        // set the encoding type
        NSStringEncoding enc = NSUTF8StringEncoding;
        
        // reading the file content into a string
        NSString *fileString = [NSString stringWithContentsOfFile: fileToOpen encoding:enc error:nil];
        
        // dividing the file content into lines
        NSArray *lines = [fileString componentsSeparatedByString:@"\n"]; 
        
        // the data
//        NSMutableArray* data;
        // release the old data
        data->clear();
//        [data release];
        
        bool hasHeaderBeenRead = NO;
        
        for (int i = 0; i < [lines count]; i++) {
            NSString* line = [lines objectAtIndex:i];
            
            // skip empty lines
            line = [line stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
            if ([line length] == 0) 
            {
                continue;
            }
            
            
            // removing comments
            if ([line characterAtIndex:0] == '#') {
                continue;
            }
            
            // splitting every line into its columns
            NSArray* columns;
            // first, getting the file delimmiter
            NSString* delimitter = [NSString stringWithString:[importFileTypeButton titleOfSelectedItem]];
            if ([delimitter isEqualToString:@"Tab Delimitted (txt)"] == YES) 
            {
                columns = [line componentsSeparatedByString:@"\t"]; 
            }
            else if ([delimitter isEqualToString:@"Comma Delimmited (csv)"] == YES) 
            {
                columns = [line componentsSeparatedByString:@","]; 
            }
            
            // we assume a header at the first line of the file
            if (!hasHeaderBeenRead) {
//                data = [NSMutableArray arrayWithCapacity:[columns count]];
                
                for (int j=0; j<[columns count]; j++) {
//                    Trace* t = [[Trace alloc] init];
                    Trace* t = new Trace();
                    
                    NSString* parmName = [columns objectAtIndex:j];
                    t->setParameterName([parmName cStringUsingEncoding:NSUTF8StringEncoding]);
                    t->setFileName([fileToOpen cStringUsingEncoding:NSUTF8StringEncoding]);
                    
                    data->push_back(t);
                }
                
                hasHeaderBeenRead = YES;
                
                continue;
            }
            
            // adding values to the traces
            for (int j=0; j<[columns count]; j++) {
//                Trace* t = [data objectAtIndex:j];
                Trace* t = data->at(j);
                NSNumber* tmp = [columns objectAtIndex:j];
                double d = [tmp doubleValue];
                t->addObject(d);
                //                [[columns objectAtIndex:j] retain];
            }
        }
        
        NSLog(@"Created %lu traces.", data->size());
//        NSLog(@"Every trace containes %lu elements.", [[data objectAtIndex:0] count]);
        
//        [self setData:data];
//        [data retain];
        
        return YES;
        
    }
    else if (result == NSCancelButton) 
    {
        NSLog(@"doSaveAs we have a Cancel button");
        return NO;
    }
    else 
    {
        NSLog(@"doSaveAs tvarInt not equal 1 or zero = %3d", result);
        return NO;
    }   
    
    
    
	return NO;
}



- (NSMutableAttributedString*)sendTip {
    
    NSString* myTip = [NSString stringWithString:@" Numerical MCMC output Tool "];
    if ([self isResolved] == YES)
        myTip = [myTip stringByAppendingString:@"\n Status: Resolved "];
    else 
        myTip = [myTip stringByAppendingString:@"\n Status: Unresolved "];
    if ([self isFullyConnected] == YES)
        myTip = [myTip stringByAppendingString:@"\n Fully Connected "];
    else 
        myTip = [myTip stringByAppendingString:@"\n Missing Connections "];
    
    NSDictionary *attr = [NSDictionary 
                          dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Lucida Grande Bold" size:14.0], [[NSColor whiteColor] colorWithAlphaComponent:1.0], nil] 
                          forKeys: [NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
    
    NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:myTip attributes:attr];
    
    return attrString;
}

- (void)setControlWindowSize {
    
	NSRect oldFrame = [[self window] frame];
    NSRect newFrame = oldFrame;
    
    // setting the window size to a default value which fits nicely all elements into the window. The window itself remains not-resizeable.
    if ( [source isEqualToString:@"MCMC"] == YES )
    {
        newFrame.size.height = 146.0;
    }
    else if ( [source isEqualToString:@"Import"] == YES )
    {
        newFrame.size.height = 146.0;
    }
    else if ( [source isEqualToString:@"Export"] == YES )
    {
        newFrame.size.height = 146.0;
    }
    
    newFrame.origin.y += (oldFrame.size.height - newFrame.size.height);
    [[self window] setFrame:newFrame display:YES animate:YES];
}

- (void)setControlsEnabledState {
    
}


- (void)setInletsAndOutlets {
    
    NSLog(@"ToolNumericalMcmcOutput setInletsAndOutlets  = %@", [self source]);
    
    if ( [[self source] isEqualToString:@"MCMC"] == YES )
    {
        [self removeAllInletsAndOutlets];
        [self addInletOfColor:[NSColor orangeColor]];
        [self addOutletOfColor:[NSColor yellowColor]];
    }
    else if ( [[self source] isEqualToString:@"Import"] == YES )
    {
        [self removeAllInletsAndOutlets];
        [self addOutletOfColor:[NSColor yellowColor]];
    }
}

- (void)showControlPanel {
    
    NSPoint p = [self originForControlWindow:[self window]];
    [[self window] setFrameOrigin:p];
	[self showWindow:self];    
	[[self window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[self window]];
}

- (void)showInspectorPanel {
    
    if (dataInspector != nil)
        [dataInspector release];
    dataInspector = [[WindowControllerNumberMatrix alloc] initWithTool:self];
    [[dataInspector window] center];
    [dataInspector showWindow:self];
}

- (void)windowDidLoad {
    
}

- (BOOL)writeDataFile {
    
    NSSavePanel *sp = [NSSavePanel savePanel];
    [sp setTitle:@"Save output file as"];
    NSArray *fileTypes = [NSArray arrayWithObjects: @"txt", @"text", NSFileTypeForHFSTypeCode( 'TEXT' ), nil];
    [sp setAllowedFileTypes:fileTypes];
    
    int spInt = (int)[sp runModalForDirectory:nil file:nil];
    if (spInt == NSOKButton)
    {
        NSLog(@"doSaveAs we have an OK button");	
        
        return YES;
    }
    else if (spInt == NSCancelButton) 
    {
        NSLog(@"doSaveAs we have a Cancel button");
        return NO;
    }
    else 
    {
        NSLog(@"doSaveAs tvarInt not equal 1 or zero = %3d", spInt);
        return NO;
    }     
    
	
    
	return NO;
}


@end
