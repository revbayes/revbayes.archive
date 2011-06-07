#import "RbAccessoryViewController.h"



@implementation RbAccessoryViewController

#pragma mark FACTORY METHOD

+ (RbAccessoryViewController *)sharedController  {

	static RbAccessoryViewController *sharedController = nil;
	if (sharedController == nil) 
		{
		sharedController = [[self alloc] initWithNibName:@"MyAccessoryView" bundle:nil];
		}
	return sharedController;
}

- (void)awakeFromNib {

	[self localizedSummaryItems];
	[self keyPathsForValuesAffectingPreview];
	rbUnitConversionConstant = 72.0; // currently set to convert points to inches
}

#pragma mark ACTION METHODS

- (IBAction) changeFitHorizontal:(id)sender {

	[self setFitHorizontal:[sender state] ? YES : NO];
}

- (IBAction) changeFitVertical:(id)sender {

	[self setFitVertical:[sender state] ? YES : NO];
}

- (IBAction) changeTMargin:(id)sender {

	[self setTMargin:([sender floatValue] * rbUnitConversionConstant)];
}

- (IBAction) changeBMargin:(id)sender {

	[self setBMargin: ([sender floatValue] * rbUnitConversionConstant)];
}

- (IBAction) changeLMargin:(id)sender {

	[self setLMargin:([sender floatValue] * rbUnitConversionConstant)];
}

- (IBAction) changeRMargin:(id)sender {

	[self setRMargin:([sender floatValue] * rbUnitConversionConstant)];
}

#pragma mark ACCESSOR METHODS

- (NSButton *)fitHorizontalCheckbox {

	return fitHorizontalCheckbox;
}

- (NSButton *)fitVerticalCheckbox { 

	return fitVerticalCheckbox;
}

- (NSTextField *)tMarginTextField {

	return tMarginTextField;
}

- (NSTextField *)bMarginTextField {

	return bMarginTextField;
}

- (NSTextField *)lMarginTextField {

	return lMarginTextField;
}

- (NSTextField *)rMarginTextField {

	return rMarginTextField;
}

- (void)setFitHorizontal:(int)h {

	[[[self representedObject] dictionary] setObject:[NSNumber numberWithInt:h] forKey:NSPrintHorizontalPagination];
}

- (int)fitHorizontal {

	return [[[[self representedObject] dictionary] objectForKey:NSPrintHorizontalPagination] intValue];	
}

- (void)setFitVertical:(int)v {

	[[[self representedObject] dictionary] setObject:[NSNumber numberWithInt:v] forKey:NSPrintVerticalPagination];
}

- (int)fitVertical {

	return [[[[self representedObject] dictionary] objectForKey:NSPrintVerticalPagination] intValue];	
}

- (void)setTMargin:(float)t {

	[[[self representedObject] dictionary] setObject:[NSNumber numberWithFloat:t] forKey:NSPrintTopMargin];
}

- (float)tMargin {

	return [[[[self representedObject] dictionary] objectForKey:NSPrintTopMargin] floatValue];
}

- (void)setBMargin: (float)b {

	[[[self representedObject] dictionary] setObject:[NSNumber numberWithFloat:b] forKey:NSPrintBottomMargin];	
}

- (float)bMargin {

	return [[[[self representedObject] dictionary] objectForKey:NSPrintBottomMargin] floatValue];
}

- (void)setLMargin:(float)l {

	[[[self representedObject] dictionary] setObject:[NSNumber numberWithFloat:l] forKey:NSPrintLeftMargin];	
}

- (float)lMargin {

	return [[[[self representedObject] dictionary] objectForKey:NSPrintLeftMargin] floatValue];
}

- (void)setRMargin:(float)r {

	[[[self representedObject] dictionary] setObject:[NSNumber numberWithFloat:r] forKey:NSPrintRightMargin];
}

- (float)rMargin {

	return [[[[self representedObject] dictionary] objectForKey:NSPrintRightMargin] floatValue];
}

- (float)tMarginConverted:(float)tc {

	tMarginConverted = (tc / rbUnitConversionConstant);	
	return tMarginConverted;
}

- (float)bMarginConverted:(float)bc {

	bMarginConverted = (bc / rbUnitConversionConstant);
	return bMarginConverted;
}

- (float)lMarginConverted:(float)lc {

	lMarginConverted = (lc / rbUnitConversionConstant);
	return lMarginConverted;
}

- (float)rMarginConverted:(float)rc {

	rMarginConverted = (rc / rbUnitConversionConstant);
	return rMarginConverted;
}

#pragma mark PROTOCOL METHODS

- (NSArray *)localizedSummaryItems {
 
	NSString *itemNameForFitHorizontal = @"horizontalPagination";
	NSString *itemDescriptionForFitHorizontal = @"";
	
	NSString *itemNameForFitVertical = @"verticalPagination";
	NSString *itemDescriptionForFitVertical = @"";
	
	NSString *itemNameForTopMargin = @"topMargin";
	NSString *itemDescriptionForTopMargin = [[NSNumber numberWithFloat:[self tMargin]]stringValue];
	
	NSString *itemNameForBottomMargin = @"bottomMargin";
	NSString *itemDescriptionForBottomMargin = [[NSNumber numberWithFloat:[self bMargin]]stringValue];
	
	NSString *itemNameForLeftMargin = @"leftMargin";
	NSString *itemDescriptionForLeftMargin = [[NSNumber numberWithFloat:[self lMargin]]stringValue];
	
	NSString *itemNameForRightMargin = @"rightMargin";
	NSString *itemDescriptionForRightMargin = [[NSNumber numberWithFloat:[self rMargin]]stringValue];
	
	return [NSArray arrayWithObjects:
			  [NSDictionary dictionaryWithObjectsAndKeys:itemNameForFitHorizontal,NSPrintPanelAccessorySummaryItemNameKey, itemDescriptionForFitHorizontal, NSPrintPanelAccessorySummaryItemDescriptionKey, nil],
			  [NSDictionary dictionaryWithObjectsAndKeys:itemNameForFitVertical,NSPrintPanelAccessorySummaryItemNameKey, itemDescriptionForFitVertical, NSPrintPanelAccessorySummaryItemDescriptionKey, nil],
			  [NSDictionary dictionaryWithObjectsAndKeys:itemNameForTopMargin,NSPrintPanelAccessorySummaryItemNameKey, itemDescriptionForTopMargin, NSPrintPanelAccessorySummaryItemDescriptionKey, nil],
			  [NSDictionary dictionaryWithObjectsAndKeys:itemNameForBottomMargin,NSPrintPanelAccessorySummaryItemNameKey, itemDescriptionForBottomMargin, NSPrintPanelAccessorySummaryItemDescriptionKey, nil],
			  [NSDictionary dictionaryWithObjectsAndKeys:itemNameForLeftMargin,NSPrintPanelAccessorySummaryItemNameKey, itemDescriptionForLeftMargin, NSPrintPanelAccessorySummaryItemDescriptionKey, nil],
			  [NSDictionary dictionaryWithObjectsAndKeys:itemNameForRightMargin,NSPrintPanelAccessorySummaryItemNameKey, itemDescriptionForRightMargin, NSPrintPanelAccessorySummaryItemDescriptionKey, nil],
			  nil];
}

- (NSSet *)keyPathsForValuesAffectingPreview {

	return [NSSet setWithObjects: @"fitHorizontal",
			  @"fitVertical", 
			  @"bMargin", 
			  @"tMargin",
			  @"lMargin",
			  @"rMargin", nil];
}

#pragma mark OVERRIDE METHODS

- (void)setRepresentedObject:(id)printInfo  {
 
	[super setRepresentedObject:printInfo];
	[self updateView];
}

#pragma mark VIEW MANAGEMENT

- (void)updateView {

	[[self fitHorizontalCheckbox] setState: ([self fitHorizontal]) ? NSOnState : NSOffState];
	[[self fitVerticalCheckbox] setState: ([self fitVertical]) ? NSOnState : NSOffState];
	[[self tMarginTextField] setFloatValue: [self tMarginConverted: [self tMargin]]];
	[[self bMarginTextField] setFloatValue: [self bMarginConverted: [self bMargin]]];
	[[self lMarginTextField] setFloatValue: [self lMarginConverted: [self lMargin]]];
	[[self rMarginTextField] setFloatValue: [self rMarginConverted: [self rMargin]]];
}

@end
