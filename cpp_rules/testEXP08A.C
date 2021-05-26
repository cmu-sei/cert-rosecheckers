/*
EXP08-A. A switch statement should have a default clause unless every enumeration value is tested
Added by Fred Long, last edited by Justin Pincar on Sep 12, 2007  (view change) show comment hide comment
Comment: Edited by sciSpider (sch jbop) (X_X)@==(Q_Q)@
View page history
Labels: 
(None)


Add Labels
	
Wait Image 
Enter labels to add to this page:
Tip: Looking for a label? Just start typing.
	

If there is no default clause in a switch statement, then the compiler can issue a warning when not all of the enumeration values are tested. So, the lack of a default clause should indicate that every enumeration value is tested. If all values are not tested, then a default clause should be included for completeness (see [MSC01-A. Strive for Logical Completeness]).
*/

enum WidgetEnum { WE_W, WE_X, WE_Y, WE_Z };

void f( WidgetEnum  widget_type ) {
	switch (widget_type) { // diagnostic
	case WE_X:
		break;
	case WE_Y:
		break;
	case WE_Z:
		break;
	}

	switch (widget_type) { // no diagnostic
	case WE_X:
		break;
	case WE_Y:
	case WE_W:
		break;
	case WE_Z:
		break;
	}

	switch (widget_type) { // no diagnostic
	default:
	case WE_X:
		break;
	case WE_Y:
		break;
	case WE_Z:
		break;
	}
}
