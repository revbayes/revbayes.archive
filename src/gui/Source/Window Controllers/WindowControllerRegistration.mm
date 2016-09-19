#import "ServerComm.h"
#import "WindowControllerPreferences.h"
#import "WindowControllerRegistration.h"



@implementation WindowControllerRegistration

- (void)awakeFromNib {

	id country;
	NSEnumerator* countryEnumerator = [countryList objectEnumerator];
	while ( (country = [countryEnumerator nextObject]) )
		[countrySelector addItemWithTitle:country];

    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [firstNameField setStringValue:[defaults objectForKey:RB_UserFirstNameKey]];
    [lastNameField setStringValue:[defaults objectForKey:RB_UserLastNameKey]];
    [emailAddressField setStringValue:[defaults objectForKey:RB_UserEmailKey]];
    [institutionField setStringValue:[defaults objectForKey:RB_UserInstitutionKey]];
    [countrySelector selectItemWithTitle:[defaults objectForKey:RB_UserCountryKey]];
}

- (id)init {

	if ( (self = [super initWithWindowNibName:@"RegistrationPanel"]) )
        {
		countryList = [[NSArray alloc] initWithObjects:@"United States",@"Sweden",@"Germany",@"Canada",@"Afghanistan",
		              @"Albania",@"Algeria",@"Andorra",@"Angola",
		              @"Antigua",@"Argentina",@"Armenia",@"Australia",@"Austria",@"Azerbaijan",@"Bahamas",@"Bahrain",
					  @"Bangladesh",@"Barbados",@"Bavaria",@"Belarus",@"Belgium",@"Belize",@"Benin",@"Bhutan",@"Bolivia",
					  @"Bosnia Herzegovina",@"Botswana",@"Brazil",@"Brunei",@"Bulgaria",@"Burkina",@"Burundi",
					  @"Cambodia",@"Cameroon",@"Cape Verde",@"Central African Rep.",@"Chad",@"Chile",
					  @"China",@"Colombia",@"Comoros",@"Congo",@"Costa Rica",@"Croatia",@"Cuba",@"Cyprus",@"Czech Republic",
					  @"Denmark",@"Djibouti",@"Dominica",@"Dominican Republic",@"East Timor",@"Ecuador",@"Egypt",
					  @"El Salvador",@"Equatorial Guinea",@"Eritrea",@"Estonia",@"Ethiopia",@"Fiji",@"Finland",
					  @"France",@"Gabon",@"Gambia",@"Georgia",@"Ghana",@"Greece",@"Grenada",@"Guatemala",
					  @"Guinea",@"Guinea-Bissau",@"Guyana",@"Haiti",@"Honduras",@"Hungary",@"Iceland",@"India",
					  @"Indonesia",@"Iran",@"Iraq",@"Ireland",@"Israel",@"Italy",@"Ivory Coast",@"Jamaica",@"Japan",
					  @"Jordan",@"Kazakhstan",@"Kenya",@"Kiribati",@"Korea North",@"Korea South",@"Kosovo",@"Kuwait",
					  @"Kyrgyzstan",@"Laos",@"Latvia",@"Lebanon",@"Lesotho",@"Liberia",@"Libya",@"Liechtenstein",
					  @"Lithuania",@"Luxembourg",@"Macedonia",@"Madagascar",@"Malawi",@"Malaysia",@"Maldives",
					  @"Mali",@"Malta",@"Marshall Islands",@"Mauritania",@"Mauritius",@"Mexico",@"Micronesia",
					  @"Moldova",@"Monaco",@"Mongolia",@"Montenegro",@"Morocco",@"Mozambique",@"Myanmar",@"Namibia",
					  @"Nauru",@"Nepal",@"Netherlands",@"New Zealand",@"Nicaragua",@"Niger",@"Nigeria",@"Norway",
					  @"Oman",@"Pakistan",@"Palau",@"Panama",@"Papua New Guinea",@"Paraguay",@"Peru",@"Philippines",
					  @"Poland",@"Portugal",@"Qatar",@"Romania",@"Russian Federation",@"Rwanda",@"St Kitts & Nevis",
					  @"St Lucia",@"Saint Vincent & the Grenadines",@"Samoa",@"San Marino",@"Sao Tome & Principe",
					  @"Saudi Arabia",@"Senegal",@"Serbia",@"Seychelles",@"Sierra Leone",@"Singapore",@"Slovakia",
					  @"Slovenia",@"Solomon Islands",@"Somalia",@"South Africa",@"Spain",@"Sri Lanka",@"Sudan",
					  @"Suriname",@"Swaziland",@"Switzerland",@"Syria",@"Taiwan",@"Tajikistan",@"Tanzania",@"Texas",
					  @"Thailand",@"Togo",@"Tonga",@"Trinidad & Tobago",@"Tunisia",@"Turkey",@"Turkmenistan",@"Tuvalu",
					  @"Uganda",@"Ukraine",@"United Arab Emirates",@"United Kingdom",@"Uruguay",
					  @"Uzbekistan",@"Vanuatu",@"Vatican City",@"Venezuela",@"Vietnam",@"Yemen",@"Zambia",@"Zimbabwe",nil];
        hasRegistered = NO;
		}
	return self;
}

- (void)openWindow {

    [[self window] center];
	[self showWindow:self];
}

- (void)registerUserEntriesInDefaults {

    // get a pointer to the defaults
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];

    // register default values for analysis window background color
	[defaults setObject:[NSNumber          numberWithBool:hasRegistered] forKey:RB_UserHasRegisteredKey];
	[defaults setObject:[firstNameField    stringValue]                  forKey:RB_UserFirstNameKey    ];
	[defaults setObject:[lastNameField     stringValue]                  forKey:RB_UserLastNameKey     ];
	[defaults setObject:[emailAddressField stringValue]                  forKey:RB_UserEmailKey        ];
	[defaults setObject:[institutionField  stringValue]                  forKey:RB_UserInstitutionKey  ];
	[defaults setObject:[countrySelector   titleOfSelectedItem]          forKey:RB_UserCountryKey      ];
}

- (IBAction)registerNowAction:(id)sender {

	// register the user entries to the defaults (this time with hasRegistered = NO)
    [self registerUserEntriesInDefaults];

	// attempt to connect to the server
	ServerComm* server = [[ServerComm alloc] init];
	if ([server connectToServer] == NO)
		{
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Connection Failure"];
        [alert setInformativeText:@"Your registration could not be submitted because the RevBayes server could not be reached. Please register at a later time."];
        [alert runModal];
		//NSRunAlertPanel(@"Connection Failure", @"Your registration could not be submitted because the RevBayes server could not be reached. Please register at a later time.", @"OK", nil, nil);
		return;
		}

	// if the connection is successful, communicate the information
	NSArray* ui = [NSArray arrayWithObjects:[firstNameField stringValue], [lastNameField stringValue], [emailAddressField stringValue], [institutionField  stringValue], [countrySelector titleOfSelectedItem], nil];
	[server submitRegistrationInfo:ui];
	[server disconnectFromServer];

	// register the user entries to the defaults (again, but this time with hasRegistered = YES)
    hasRegistered = YES;
    [self registerUserEntriesInDefaults];
	
	// close the window
    [self close];
}

- (IBAction)registerLaterAction:(id)sender {

    hasRegistered = NO;
    [self registerUserEntriesInDefaults];
    [self close];
}

@end
