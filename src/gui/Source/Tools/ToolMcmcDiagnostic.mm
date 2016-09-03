//
//  ToolMcmcDiagnostic.mm
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 3/28/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "ToolMcmcDiagnostic.h"


#import "EssMax.h"
#import "GelmanRubinTest.h"
#import "GewekeTest.h"
#import "InOutlet.h"
#import "RevBayes.h"
#import "StationarityTest.h"
#import "SemMin.h"
#import "Trace.h"
#import "TraceAnalysisContinuous.h"
#import "ToolNumericalMcmcOutput.h"

#if 1

@implementation ToolMcmcDiagnostic

@synthesize data;
@synthesize included;

@synthesize useGeweke;
@synthesize useHeidelbergerWelch;
@synthesize useRafteryLewis;
@synthesize useEssThreshold;
@synthesize useSemThreshold;
@synthesize useStationarity;
@synthesize useIidBetweenChains;
@synthesize useGelmanRubin;
@synthesize useFixPercentage;
@synthesize useOptimalBurninEstimation;

@synthesize pGeweke;
@synthesize kEss;
@synthesize kSem;
@synthesize pStationarity;
@synthesize pIidBetween;
@synthesize rGelman;
@synthesize fixPercentage;

- (void)awakeFromNib {
	
    [viewTab selectTabViewItemWithIdentifier:tab];
	[self setControlsEnabledState];
	[self setControlWindowSize];
}

- (NSString*)checkString:(int)index cell:(NSTextFieldCell*)aCell
{
    if (index == FAILED)
    {
        [aCell setTextColor:[NSColor redColor]];
//        [aCell setAlignment:NSCenterTextAlignment];
		unichar uc = 0x2717;
        NSString* content = [NSString stringWithFormat:@"%C", uc];
        return content;
    }
    else if (index == PASSED)
    {
        [aCell setTextColor:[NSColor greenColor]];
        [aCell setAlignment:NSCenterTextAlignment];
		unichar uc = 0x2713;
        NSString* content = [NSString stringWithFormat:@"%C", uc];
        return content;
    }
    else if (index == NOT_CHECKED)
    {
        [aCell setTextColor:[NSColor blackColor]];
        [aCell setAlignment:NSCenterTextAlignment];
		unichar uc = 0x2751;
        NSString* content = [NSString stringWithFormat:@"%C", uc];
        return content;
    }
    
    return @"";
}

- (IBAction)closeAction:(id)sender {
	
    [self closeControlPanel];
}

- (void)closeControlPanel {
    
    [NSApp stopModal];
	[self close];
    [self setInletsAndOutlets];
}

- (void)tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem {
    
    // getting the new selected source (either File or MCMC)
    tab = [NSString stringWithString:[[viewTab selectedTabViewItem] label]];
    if ( [tab isEqualToString:@"Results"] == YES )
    {
        
    }
    else if ( [tab isEqualToString:@"Settings"] == YES )
    {
        
    }
    
    
    // updating the outlets and inlets
    [self setInletsAndOutlets];
    
    // resetting the window size
    [self setControlWindowSize];
}


- (void)encodeWithCoder:(NSCoder *)aCoder {
    
	[super encodeWithCoder:aCoder];
}

- (BOOL)execute {



    return [super execute];
}

- (IBAction)helpButtonAction:(id)sender {
    /** 
     * /TODO need some help implementation
     *
     */
}

- (id)init {
    
    self = [self initWithScaleFactor:1.0 andWindowNibName:@"ControlWindowMcmcDiagnostic"];
    return self;
}

- (id)initWithScaleFactor:(float)sf {
    
    if ( (self = [super initWithScaleFactor:sf andWindowNibName:@"ControlWindowMcmcDiagnostic"]) ) 
    {
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
		// initialize the inlet/outlet information
		[self addInletOfColor:[NSColor purpleColor]];
        [self setInletLocations];
        [self setOutletLocations];
        
        // initialize the settings
        [progressBar setIndeterminate:NO];
		
		// initialize the control window
//		controlWindow = [[WindowControllerMcmcDiagnostic alloc] initWithTool:self];
        
        // initialize the data
        data                        = new std::vector<RevBayesCore::Trace* >;
        included                    = [[NSMutableArray alloc] init];
        // within chain convergence gui objects
        pStationarity               = 0.01;
        pGeweke                     = 0.01;
        kEss                        = 200;
        kSem                        = 0.1;
        
        // between chain convergence gui objects
        pIidBetween                 = 0.01;
        rGelman                     = 1.001;
        
        // burnin gui objects
        fixPercentage               = 0.1;
        
        // within chain convergence settings
        useStationarity             = YES;
        useGeweke                   = NO;
        useHeidelbergerWelch        = NO;
        useRafteryLewis             = NO;
        useEssThreshold             = YES;
        useSemThreshold             = NO;
        
        // between chain convergence settings
        useIidBetweenChains         = YES;
        useGelmanRubin              = NO;
        
        // burnin estimation settings
        useFixPercentage            = NO;
        useOptimalBurninEstimation  = YES;
        burninSelection             = OPTIMAL_BURNIN;
        
        // allocate objects
        tab                         = @"Settings";
        
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
        
		// initialize the control window
//		controlWindow = [[WindowControllerMcmcDiagnostic alloc] initWithTool:self];
    }
	return self;
}

- (void)initializeImage {
    
    itemImage[0] = [NSImage imageNamed:@"Tool_NumberSetDiagnosis25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_NumberSetDiagnosis50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_NumberSetDiagnosis75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_NumberSetDiagnosis100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_NumberSetDiagnosis125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_NumberSetDiagnosis150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_NumberSetDiagnosis200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_NumberSetDiagnosis400.icns"];
    
	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (int)numberOfRowsInTableView:(NSTableView *)tableView
{
    // HACK: check if the number of columns is correct; if not, reset columns
    if ([[table tableColumns] count] != data->size()) 
    {
        [self setTableColumns];
    }
    
    return (int)data->size();
}


- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(int)row
{
    // HACK: check if the number of columns is correct; if not, reset columns
    //if ([[table tableColumns] count] != [[myTool data] count]) 
    //{
    //    [self setTableColumns];
    //}
    
    NSTextFieldCell* aCell = [tableColumn dataCell];
    
    // get the trace for this row
    RevBayesCore::Trace* t = data->at(row);
    
    // get the name of the column
    NSString* header = [[tableColumn headerCell] objectValue];
    if ([header isEqualToString:@"Include"] == YES)
    {
        return [included objectAtIndex:row];
    }
    
    if ([header isEqualToString:@"File name"] == YES)
    {
        std::string str = t->getFileName();
        NSString* cellContent = [[NSString alloc] initWithCString:str.c_str() encoding:NSUTF8StringEncoding];
        return cellContent;
    }
    
    if ([header isEqualToString:@"Parameter name"] == YES)
    {
        std::string str = t->getParameterName();
        NSString* cellContent = [[NSString alloc] initWithCString:str.c_str() encoding:NSUTF8StringEncoding];
        return cellContent;
    }
    
    if ([header isEqualToString:@"Burnin"] == YES)
    {
#if 0
        int n = (int)t->getBurnin();
        NSNumber* cellContent = [[NSNumber alloc] initWithInt:n];
        return cellContent;
#else
        return 0;
#endif
    }
    
    if ([header isEqualToString:@"Mean"] == YES)
    {
#if 0
        double n = t->getMean();
        NSNumber* cellContent = [[NSNumber alloc] initWithDouble:n];
        return cellContent;
#else
        return 0;
#endif
    }
    
    if ([header isEqualToString:@"SEM"] == YES)
    {
#if 0
        double n = t->getSem();
        NSNumber* cellContent = [[NSNumber alloc] initWithDouble:n];
        return cellContent;
#else
        return 0;
#endif
    }
    
    if ([header isEqualToString:@"ESS"] == YES)
    {
#if 0
        double n = t->getEss();
        NSNumber* cellContent = [[NSNumber alloc] initWithDouble:n];
        return cellContent;
#else
        return 0;
#endif
    }
    
    if ([header isEqualToString:@"Stationarity test"] == YES)
    {
#if 0
        return [self checkString:t->hasPassedStationarityTest() cell:aCell];
#else
        return 0;
#endif
    }
    
    if ([header isEqualToString:@"Geweke test"] == YES)
    {
#if 0
        return [self checkString:t->hasPassedGewekeTest() cell:aCell];
#else
        return 0;
#endif
    }
    
//    if ([header isEqualToString:@"Heidelberger-Welch test"] == YES)
//    {
//        return [self checkString:[t passedHeidelbergerWelchStatistic] cell:aCell];
//    }
    
//    if ([header isEqualToString:@"Raftery-Lewis test"] == YES)
//    {
//        return [self checkString:[t passedRafteryLewisStatistic] cell:aCell];
//    }
    
    if ([header isEqualToString:@"ESS > k"] == YES)
    {
#if 0
        return [self checkString:t->hasPassedEssThreshold() cell:aCell];
#else
        return 0;
#endif
    }
    
    if ([header isEqualToString:@"SEM < k"] == YES)
    {
#if 0
        return [self checkString:t->hasPassedSemThreshold() cell:aCell];
#else
        return 0;
#endif
    }
    
    if ([header isEqualToString:@"IID Sampling Theory between chains"] == YES)
    {
#if 0
        return [self checkString:t->hasPassedIidBetweenChainsStatistic() cell:aCell];
#else
        return 0;
#endif
    }
    
    if ([header isEqualToString:@"Gelman-Rubin test"] == YES)
    {
#if 0
        return [self checkString:t->hasPassedGelmanRubinTest() cell:aCell];
#else
        return 0;
#endif
    }
    
    
    return nil;
}


- (void)removeAllTraces 
{
    while ([included count] > 0) {
        [included removeLastObject];
    }
    
    data = nil;
}

- (IBAction)runButtonAction:(id)sender {
    // settings for the progress dialog
    [progressBar setIndeterminate:FALSE];
    [progressBar setMinValue:0.0];
    [progressBar setMaxValue:(int)data->size()];
    [progressBar setDoubleValue:0.0];
    
    [progressBar setUsesThreadedAnimation:YES];
    [progressBar startAnimation:self];
    
    // show the progress dialog
    [progressPanel makeKeyAndOrderFront:self];
    //    [[self window] orderOut:self];
    [progressPanel setViewsNeedDisplay:YES];
    [progressPanel center];
    
    // set the tool state to unresolved
    [self setIsResolved:NO];
    
	// remember the state of the control panel
//	[self setToolValues];
    
	// perform the action
    // iterate over each trace
    for (int i=0; i<data->size(); i++) {
        [progressTextField setEditable:YES];
        [progressTextField setStringValue:[NSString stringWithFormat:@"Anaylising trace %d",i+1]];
 //       [progressPanel abortEditing];
        [progressPanel setViewsNeedDisplay:YES];
        [progressPanel update];
        
        // check if that trace is include
        if ([[included objectAtIndex:i] boolValue]) {
            RevBayesCore::Trace* t = data->at(i);
#if 0
            
            // calculate the burnin
            if (useFixPercentage) {
                int burnin = (int)t->size();
                burnin *= t->getStepSize();
                burnin *= fixPercentage;
//                [t setBurnin:(((int)[t count]) * fixPercentage * [t stepSize])];
                t->setBurnin(burnin);
                
                RevBayesCore::TraceAnalysisContinuous* analysis = new RevBayesCore::TraceAnalysisContinuous();
                analysis->analyseCorrelation(t->getValues(), burnin);
                t->setEss(analysis->getEss());
                t->setMean(analysis->getMean());
                t->setSem(analysis->getStdErrorOfMean());
                
                delete analysis;
            }
            else {
                if ([[[optimalBurninPuButton selectedItem] title] isEqualToString:@"ESS"] ) {
                    // estimate the burnin
                    RevBayesCore::EssMax* estimator = new RevBayesCore::EssMax();
                    int burnin = (int)estimator->estimateBurnin(t->getValues());
                    delete estimator;
                    // calculate the trace statistics
                    RevBayesCore::TraceAnalysisContinuous* analysis = new RevBayesCore::TraceAnalysisContinuous();
                    analysis->analyseCorrelation(t->getValues(), burnin);
                    t->setBurnin(burnin);
                    t->setEss(analysis->getEss());
                    t->setMean(analysis->getMean());
                    t->setSem(analysis->getStdErrorOfMean());
                    delete analysis;
                }
                else {
                    // estimate the burnin
                    RevBayesCore::SemMin* estimator = new RevBayesCore::SemMin();
                    int burnin = (int)estimator->estimateBurnin(t->getValues());
                    delete estimator;
                    // calculate the trace statistics
                    RevBayesCore::TraceAnalysisContinuous* analysis = new RevBayesCore::TraceAnalysisContinuous();
                    analysis->analyseCorrelation(t->getValues(), burnin);
                    t->setBurnin(burnin);
                    t->setEss(analysis->getEss());
                    t->setMean(analysis->getMean());
                    t->setSem(analysis->getStdErrorOfMean());
                    delete analysis;
                }
            }
#endif
    
#if 0
            // convergence assessment methods
            
            // ESS > k statistic
            if (useEssThreshold) {
                t->setPassedEssThreshold(t->getEss() > kEss);
            }
            
            // SEM < k statistic
            if (useEssThreshold) {
                t->setPassedSemThreshold(t->getSem()/abs(t->getMean()) < kSem);
            }
            
            // test stationarity within chain
            if (useStationarity) {
                int nBlocks = 10;
                RevBayesCore::StationarityTest* test = new RevBayesCore::StationarityTest(nBlocks, pStationarity);
                bool passed = test->assessConvergenceSingleChain(t->getValues(), t->getBurnin());
                t->setPassedStationarityTest(passed);
                
                delete test;
            }
            
            // Geweke's test for convergence within a chain
            if (useGeweke) {
                RevBayesCore::GewekeTest* test = new RevBayesCore::GewekeTest(pGeweke);
                bool passed = test->assessConvergenceSingleChain(t->getValues(), t->getBurnin());
                t->setPassedGewekeTest(passed);
                
                delete test;
            }
#endif
        }
        
        // incrment the progress
        [progressBar incrementBy:1.0];
        [progressBar displayIfNeeded];
    }
    
    [table reloadData];
    [progressBar stopAnimation:self];
    [progressPanel orderOut:self];
    [[self window] makeKeyAndOrderFront:self];
}


- (NSMutableAttributedString*)sendTip {
    
    NSString* myTip = @" MCMC Diagnostic Tool ";
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

- (IBAction)settingChangedAction:(id)sender {
    
    useFixPercentage            = burninSelection == FIXED_BURNIN;
    useOptimalBurninEstimation  = burninSelection == OPTIMAL_BURNIN;
    
    // update the view
    [self setControlsEnabledState];
    
    // update the table view
    [self setTableColumns];
    
}

- (void)setControlWindowSize {
    
    NSLog(@"Setting window size for tab %@", tab);
    
	NSRect oldFrame = [[self window] frame];
    NSRect newFrame = oldFrame;
    
    // setting the window size to a default value which fits nicely all elements into the window. 
    if ( [tab isEqualToString:@"Results"] == YES )
    {
        newFrame.size.height = 465.0;
        
    }
    else if ( [tab isEqualToString:@"Settings"] == YES )
    {
        newFrame.size.height = 465.0;
    }
    newFrame.origin.y += (oldFrame.size.height - newFrame.size.height);
    [[self window] setFrame:newFrame display:YES animate:YES];
}

- (void)setControlsEnabledState {
    // do the stuff here with hiding and showing options in the gui
    // burnin
    [fixPercentageTextField setHidden:!useFixPercentage];
    [fixPercentageLabel setHidden:!useFixPercentage];
    [optimalBurninPuButton setHidden:!useOptimalBurninEstimation];
    // within chain
    [pStationarityTextField setHidden:!useStationarity];
    [pStationarityLabel setHidden:!useStationarity];
    [pGewekeTextField setHidden:!useGeweke];
    [pGewekeLabel setHidden:!useGeweke];
    //    [pIidWithinTextField setHidden:isHeidelbergerWelchActive];
    //    [pIidWithinTextField setHidden:isRafteryLewisActive];
    [kEssTextField setHidden:!useEssThreshold];
    [kEssLabel setHidden:!useEssThreshold];
    [kSemTextField setHidden:!useSemThreshold];
    [kSemLabel setHidden:!useSemThreshold];
    // between chains
    [pIidBetweenTextField setHidden:!useIidBetweenChains];
    [pIidBetweenLabel setHidden:!useIidBetweenChains];
    [rGelmanTextField setHidden:!useGelmanRubin];
    [rGelmanLabel setHidden:!useGelmanRubin];
    
    
}

- (void)setInletsAndOutlets {
    // this function does nothing because so far we do not have the case that inlets or outlets change.
    
}

- (void)tableView:(NSTableView *)tableView 
   setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)rowIndex
{
    
    // get the name of the column
    NSString* header = [[tableColumn headerCell] objectValue];
    if ([header isEqualToString:@"Include"] == YES)
    {
        BOOL tmp = [[included objectAtIndex:rowIndex] boolValue] == NO;
        [included replaceObjectAtIndex:rowIndex withObject:[[NSNumber alloc] initWithBool:tmp]];
    }
    
}

- (void)setTableColumns {
    // first, removing all old columns
    NSArray* tc = [table tableColumns];
    
    while ([tc count] > 0)
    {
        NSTableColumn* col = [tc objectAtIndex:0]; 
        [table removeTableColumn:col];
    }
    
    // adding columns
    float width = 100;
    float minWidth = 100;
    
    // adding a column with the the check box 
    NSString* identifier = @"Include";
    // create a table column
    NSTableColumn* col = [[NSTableColumn alloc] initWithIdentifier:identifier];
    [col setWidth:width];
    [col setMinWidth:minWidth];
    // set the value for the table header
    NSTableHeaderCell* header = [col headerCell];
    [header setObjectValue:identifier];
    // set the default cell for the column
    NSButtonCell* bCell = [[NSButtonCell alloc] init];
    [bCell setTitle:@""];
    [bCell setEditable:YES];
    [bCell setButtonType:NSSwitchButton];
    [bCell setControlSize:NSSmallControlSize];
    [bCell setAllowsMixedState:YES];
    [bCell setState:YES];
    [bCell setImagePosition:NSImageOnly];
//    [bCell setAlignment:NSCenterTextAlignment];
    [col setDataCell:bCell];
    // set this column to be editable
    [col setEditable:YES];
    // add the table column to the table view
    [table addTableColumn:col];

    
    // adding a column with the file name
    identifier = @"File name";
    // create a table column
    col = [[NSTableColumn alloc] initWithIdentifier:identifier];
    [col setWidth:width];
    [col setMinWidth:minWidth];
    // set the value for the table header
    header = [col headerCell];
    [header setObjectValue:identifier];
    // set the default cell for the column
    NSTextFieldCell* tCell = [[NSTextFieldCell alloc] init];
    [tCell setAlignment:NSLeftTextAlignment];
    [col setDataCell:tCell];
    // add the table column to the table view
    [table addTableColumn:col];
    
    // adding a column with the parameter name
    identifier = @"Parameter name";
    // create a table column
    col = [[NSTableColumn alloc] initWithIdentifier:identifier];
    [col setWidth:width];
    [col setMinWidth:minWidth];
    // set the value for the table header
    header = [col headerCell];
    [header setObjectValue:identifier];
    tCell = [[NSTextFieldCell alloc] init];
    [tCell setAlignment:NSLeftTextAlignment];
    [col setDataCell:tCell];
    // add the table column to the table view
    [table addTableColumn:col];
    
    
    // adding a column with the burnin
    identifier = @"Burnin";
    // create a table column
    col = [[NSTableColumn alloc] initWithIdentifier:identifier];
    [col setWidth:width];
    [col setMinWidth:minWidth];
    // set the value for the table header
    header = [col headerCell];
    [header setObjectValue:identifier];
    // set the default cell for the column
    tCell = [[NSTextFieldCell alloc] init];
    [tCell setAlignment:NSRightTextAlignment];
    [col setDataCell:tCell];
    // add the table column to the table view
    [table addTableColumn:col];
    
    // adding a column with the mean
    identifier = @"Mean";
    // create a table column
    col = [[NSTableColumn alloc] initWithIdentifier:identifier];
    [col setWidth:width];
    [col setMinWidth:minWidth];
    // set the value for the table header
    header = [col headerCell];
    [header setObjectValue:identifier];
    // set the default cell for the column
    tCell = [[NSTextFieldCell alloc] init];
    [tCell setAlignment:NSRightTextAlignment];
    [col setDataCell:tCell];
    // add the table column to the table view
    [table addTableColumn:col];
    
    // adding a column with the SEM
    identifier = @"SEM";
    // create a table column
    col = [[NSTableColumn alloc] initWithIdentifier:identifier];
    [col setWidth:width];
    [col setMinWidth:minWidth];
    // set the value for the table header
    header = [col headerCell];
    [header setObjectValue:identifier];
    // set the default cell for the column
    tCell = [[NSTextFieldCell alloc] init];
    [tCell setAlignment:NSRightTextAlignment];
    [col setDataCell:tCell];
    // add the table column to the table view
    [table addTableColumn:col];    
    
    // adding a column with the ESS
    identifier = @"ESS";
    // create a table column
    col = [[NSTableColumn alloc] initWithIdentifier:identifier];
    [col setWidth:width];
    [col setMinWidth:minWidth];
    // set the value for the table header
    header = [col headerCell];
    [header setObjectValue:identifier];
    // set the default cell for the column
    tCell = [[NSTextFieldCell alloc] init];
    [tCell setAlignment:NSRightTextAlignment];
    [col setDataCell:tCell];
    // add the table column to the table view
    [table addTableColumn:col];
    
    if (useStationarity)
    {
        // adding a column with the column name
        identifier = @"Stationarity test";
        // create a table column
        col = [[NSTableColumn alloc] initWithIdentifier:identifier];
        [col setWidth:width];
        [col setMinWidth:minWidth];
        // set the value for the table header
        header = [col headerCell];
        [header setObjectValue:identifier];
        // set the default cell for the column
        tCell = [[NSTextFieldCell alloc] init];
        [tCell setAlignment:NSCenterTextAlignment];
        [col setDataCell:tCell];
        // add the table column to the table view
        [table addTableColumn:col];
    }
    
    if (useGeweke)
    {
        // adding a column with the column name
        identifier = @"Geweke test";
        // create a table column
        col = [[NSTableColumn alloc] initWithIdentifier:identifier];
        [col setWidth:width];
        [col setMinWidth:minWidth];
        // set the value for the table header
        header = [col headerCell];
        [header setObjectValue:identifier];
        // set the default cell for the column
        tCell = [[NSTextFieldCell alloc] init];
        [tCell setAlignment:NSCenterTextAlignment];
        [col setDataCell:tCell];
        // add the table column to the table view
        [table addTableColumn:col];
    }
    
    if (useHeidelbergerWelch)
    {
        // adding a column with the column name
        identifier = @"Heidelberger-Welch test";
        // create a table column
        col = [[NSTableColumn alloc] initWithIdentifier:identifier];
        [col setWidth:width];
        [col setMinWidth:minWidth];
        // set the value for the table header
        header = [col headerCell];
        [header setObjectValue:identifier];
        // set the default cell for the column
        tCell = [[NSTextFieldCell alloc] init];
        [tCell setAlignment:NSCenterTextAlignment];
        [col setDataCell:tCell];
        // add the table column to the table view
        [table addTableColumn:col];
    }
    
    if (useRafteryLewis)
    {
        // adding a column with the column name
        identifier = @"Raftery-Lewis test";
        // create a table column
        col = [[NSTableColumn alloc] initWithIdentifier:identifier];
        [col setWidth:width];
        [col setMinWidth:minWidth];
        // set the value for the table header
        header = [col headerCell];
        [header setObjectValue:identifier];
        // set the default cell for the column
        tCell = [[NSTextFieldCell alloc] init];
        [tCell setAlignment:NSCenterTextAlignment];
        [col setDataCell:tCell];
        // add the table column to the table view
        [table addTableColumn:col];
    }
    
    if (useEssThreshold)
    {
        // adding a column with the column name
        identifier = @"ESS > k";
        // create a table column
        col = [[NSTableColumn alloc] initWithIdentifier:identifier];
        [col setWidth:width];
        [col setMinWidth:minWidth];
        // set the value for the table header
        header = [col headerCell];
        [header setObjectValue:identifier];
        // set the default cell for the column
        tCell = [[NSTextFieldCell alloc] init];
        [tCell setAlignment:NSCenterTextAlignment];
        [col setDataCell:tCell];
        // add the table column to the table view
        [table addTableColumn:col];
    }
    
    if (useSemThreshold)
    {
        // adding a column with the column name
        identifier = @"SEM < k";
        // create a table column
        col = [[NSTableColumn alloc] initWithIdentifier:identifier];
        [col setWidth:width];
        [col setMinWidth:minWidth];
        // set the value for the table header
        header = [col headerCell];
        [header setObjectValue:identifier];
        // set the default cell for the column
        tCell = [[NSTextFieldCell alloc] init];
        [tCell setAlignment:NSCenterTextAlignment];
        [col setDataCell:tCell];
        // add the table column to the table view
        [table addTableColumn:col];
    }
    
    if (useIidBetweenChains)
    {
        // adding a column with the column name
        identifier = @"IID Sampling Theory between chains";
        // create a table column
        col = [[NSTableColumn alloc] initWithIdentifier:identifier];
        [col setWidth:width];
        [col setMinWidth:minWidth];
        // set the value for the table header
        header = [col headerCell];
        [header setObjectValue:identifier];
        // set the default cell for the column
        tCell = [[NSTextFieldCell alloc] init];
        [tCell setAlignment:NSCenterTextAlignment];
        [col setDataCell:tCell];
        // add the table column to the table view
        [table addTableColumn:col];
    }
    
    if (useGelmanRubin)
    {
        // adding a column with the column name
        identifier = @"Gelman-Rubin test";
        // create a table column
        col = [[NSTableColumn alloc] initWithIdentifier:identifier];
        [col setWidth:width];
        [col setMinWidth:minWidth];
        // set the value for the table header
        header = [col headerCell];
        [header setObjectValue:identifier];
        // set the default cell for the column
        tCell = [[NSTextFieldCell alloc] init];
        [tCell setAlignment:NSCenterTextAlignment];
        [col setDataCell:tCell];
        // add the table column to the table view
        [table addTableColumn:col];
    }
}


- (void)showControlPanel {
    
    NSPoint p = [self originForControlWindow:[self window]];
    [[self window] setFrameOrigin:p];
	[self showWindow:self];
	[[self window] makeKeyAndOrderFront:self];
}

- (void)showInspectorPanel {
    // we might want to add a data inspector which tells if and which chains/parameters failed to converge.
    
//    if (dataInspector != nil)
//        [dataInspector release];
//    dataInspector = [[WindowControllerNumberMatrix alloc] initWithTool:self];
//    [[dataInspector window] center];
//    [dataInspector showWindow:self];
}

- (NSString*)toolName {

    return @"MCMC Diagnostics";
}

- (void)updateForChangeInParent {
    
    // set the tool state to unresolved
    [self setIsResolved:NO];
    
	// attempt to get a pointer to the parent tool
    ToolNumericalMcmcOutput* t = (ToolNumericalMcmcOutput*)[self getParentToolOfInletIndexed:0];
	
	// update the state of this tool depending upon the state/presence of the parent tool
	if (t == nil)
    {
		// we don't have a parent tool
		[self removeAllTraces];
    }
	else 
    {
		// there is a parent tool 
		
		// check that the tool is a NumericalMcmcOutput tool...the only connection possible should be to a NumericalMcmcOutput tool
		NSString* className = NSStringFromClass([t class]); 
		if ( [className isEqualToString:@"ToolNumericalMcmcOutput"] == NO )
			return;
        
		// remove any traces the diagnostic may contain
		[self removeAllTraces];
        
//        NSLog(@"Copying %lu traces into the MCMC diagnostic tool.", [[t data] count]);
		
		// copy the traces to this control window's tool
//        data = [data initWithArray:[t data]];
//        data = [data initWithArray:[t data] copyItems:YES];
        
//        NSLog(@"Diagnostic tool has now %lu traces.", [data count]);

        [self setIsResolved:YES];
    }
    
}


- (void)windowDidLoad {
    [progressPanel orderOut:self];
}



@end

#endif