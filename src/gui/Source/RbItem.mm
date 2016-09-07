#import "RbItem.h"
#import "RevBayes.h"



@implementation RbItem

@synthesize hasInspectorInfo;
@synthesize hasController;
@synthesize isSelected;
@synthesize isCursorOver;
@synthesize itemLocation;
@synthesize itemSize;
@synthesize showTip;
@synthesize isPlate;
@synthesize currentScaleFactor;
@synthesize saveAsModelTemplate;

- (void)encodeWithCoder:(NSCoder*)aCoder {

    // readjust the item's location and size to account for scale changes. We 
    // always save at the size/location for 100% scaling
	float r = 1.0 / [self currentScaleFactor];
	NSSize tempItemSize = [self itemSize];
	tempItemSize.width  *= r;
	tempItemSize.height *= r;
	if ( [self isPlate] == NO )
		tempItemSize = NSMakeSize(ITEM_IMAGE_SIZE, ITEM_IMAGE_SIZE);
	NSPoint tempItemLocation = [self itemLocation];
    tempItemLocation.x  *= r;
    tempItemLocation.y  *= r;

    // archive the information
    [aCoder encodeBool:hasInspectorInfo    forKey:@"hasInspectorInfo"];
    [aCoder encodeBool:hasController       forKey:@"hasController"];
	[aCoder encodeBool:isSelected          forKey:@"isSelected"];
	[aCoder encodePoint:tempItemLocation   forKey:@"itemLocation"];
	[aCoder encodeSize:tempItemSize        forKey:@"itemSize"];
	[aCoder encodeBool:isPlate             forKey:@"isPlate"];
    [aCoder encodeBool:saveAsModelTemplate forKey:@"saveAsModelTemplate"];
}

- (void)flipSelected {

	if (isSelected == YES)
		isSelected = NO;
	else 
		isSelected = YES;
}

- (id)init {

    self = [self initWithScaleFactor:1.0];
    return self;
}

- (void)initializeImage {

}

- (id)initWithScaleFactor:(float)sf {

	if ( (self = [super initWithWindowNibName:[self xibName]]) )
		{
        hasInspectorInfo    = NO;
        hasController       = YES;
        isSelected          = NO;
        itemLocation        = NSMakePoint(0.0, 0.0);
		itemSize            = NSMakeSize(ITEM_IMAGE_SIZE * sf, ITEM_IMAGE_SIZE * sf);
		isCursorOver        = NO;
        showTip             = NO;
		isPlate             = NO;
        saveAsModelTemplate = NO;
		currentScaleFactor  = sf;
		}
    return self;
}

- (id)initWithScaleFactor:(float)sf andWindowNibName:(NSString*)wNibName {
    
	if ( (self = [super initWithWindowNibName:wNibName]) )
        {
        hasInspectorInfo    = NO;
        hasController       = YES;
        isSelected          = NO;
        itemLocation        = NSMakePoint(0.0, 0.0);
		itemSize            = NSMakeSize(ITEM_IMAGE_SIZE * sf, ITEM_IMAGE_SIZE * sf);
		isCursorOver        = NO;
        showTip             = NO;
		isPlate             = NO;
        saveAsModelTemplate = NO;
		currentScaleFactor  = sf;
        }
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

	if ( (self = [super initWithWindowNibName:[self xibName]]) )
		{
        hasInspectorInfo    = [aDecoder decodeBoolForKey:@"hasInspectorInfo"];
        hasController       = [aDecoder decodeBoolForKey:@"hasController"];
		isSelected          = [aDecoder decodeBoolForKey:@"isSelected"];
		itemLocation        = [aDecoder decodePointForKey:@"itemLocation"];
		itemSize            = [aDecoder decodeSizeForKey:@"itemSize"];
		isPlate             = [aDecoder decodeBoolForKey:@"isPlate"];
        saveAsModelTemplate = [aDecoder decodeBoolForKey:@"saveAsModelTemplate"];
		isCursorOver        = NO;
        showTip             = NO;
		currentScaleFactor  = 1.0; // we always revive tools or parameters in a window that is at 100% scale size
		}
	return self;
}

- (NSPoint)itemCenter {

	// return the center of the image, remembering that the item's
	// image has (0,0) coordinates in the top-left corner of the view
	NSPoint c = itemLocation;
	c.x += 0.5 * itemSize.width;
	c.y -= 0.5 * itemSize.height;
	return c;
}

- (NSImage*)itemImageWithIndex:(int)idx {

	return itemImage[idx];
}

- (void)removeAllConnections {

}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" This is a RB interface object ";

    NSDictionary *attr = [NSDictionary 
                 dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Lucida Grande Bold" size:14.0], [[NSColor whiteColor] colorWithAlphaComponent:1.0], nil] 
                               forKeys: [NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];
    
    NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:myTip attributes:attr];

    return attrString;
}

- (void)showControlPanel {

}

- (void)showInspectorPanel {

}

- (NSString*)xibName {

	NSString* xn = @"";
	return xn;
}

@end
