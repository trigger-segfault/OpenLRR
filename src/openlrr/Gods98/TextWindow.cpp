
#include "TextWindow.h"
#include "Memory.h"
#include "Fonts.h"



/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @004853b0>
Gods98::TextWindow* __cdecl Gods98::TextWindow_Create(Font* font, const Area2F* size, uint32 bufferSize)
{
	log_firstcall();

	TextWindow* window;

	if (window = (TextWindow *)Mem_Alloc(sizeof(TextWindow))) {
		std::memset(window, 0, sizeof(TextWindow));
		window->font = font;
		window->windowSize = *size;
		window->windowBuffer = (char*)Mem_Alloc(bufferSize);
		window->windowBufferSize = bufferSize;
		TextWindow_EnableCentering(window, true);
	}

	return window;
}

// <LegoRR.exe @00485420>
void __cdecl Gods98::TextWindow_EnableCentering(TextWindow* window, bool32 enable)
{
	log_firstcall();

	if (enable) window->flags |= TextWindowFlags::TEXTWINDOW_FLAG_CENTERED;
	else window->flags &= ~TextWindowFlags::TEXTWINDOW_FLAG_CENTERED;
}

// <LegoRR.exe @00485450>
void __cdecl Gods98::TextWindow_Remove(TextWindow* window)
{
	log_firstcall();

	Mem_Free(window->windowBuffer);
	Mem_Free(window);
}

// <LegoRR.exe @00485470>
void __cdecl Gods98::TextWindow_ChangePosition(TextWindow* window, sint32 xPos, sint32 yPos)
{
	log_firstcall();

	window->windowSize.x = (real32)xPos;
	window->windowSize.y = (real32)yPos;
}

// <LegoRR.exe @00485490>
void __cdecl Gods98::TextWindow_ChangeSize(TextWindow* window, uint32 width, uint32 height)
{
	log_firstcall();

	window->windowSize.width = (real32)width;
	window->windowSize.height = (real32)height;
}

// <LegoRR.exe @004854d0>
void __cdecl Gods98::TextWindow_PagePrintF(TextWindow* window, uint32 page, const char* msg, ...)
{
	log_firstcall();

	std::va_list args;
	va_start(args, msg);
	TextWindow_VPrintF(window, page, msg, args);
	va_end(args);
}

// <LegoRR.exe @004854f0>
void __cdecl Gods98::TextWindow_PrintF(TextWindow* window, const char* msg, ...)
{
	log_firstcall();

	std::va_list args;
	va_start(args, msg);
	TextWindow_VPrintF(window, 0, msg, args);
	va_end(args);
}

// <LegoRR.exe @00485510>
void __cdecl Gods98::TextWindow_VPrintF(TextWindow* window, uint32 page, const char* msg, std::va_list args)
{
	log_firstcall();

	char line[TEXTWINDOW_MAXSTRINGLEN];
	uint32 count, loop, onPage = 0;

	count = std::vsprintf(line, msg, args);

	for (loop=0 ; loop<count ; loop++) {
		if (onPage == page) break;
		if (line[loop] == '\a') onPage++;
	}
	if (loop == count) return;			// Couldn't find the page...

	for ( ; loop<count ; loop++){
		window->windowBuffer[window->windowBufferEnd++] = line[loop];
		if (window->windowBufferEnd == window->windowBufferSize) {
			window->windowBufferEnd = 0;
			window->flags |= TextWindowFlags::TEXTWINDOW_FLAG_WINDOWWRAPPED;
		}
	}
}

// <LegoRR.exe @004855c0>
void __cdecl Gods98::TextWindow_PrintFOverlay(TextWindow* window, bool32 oneFrame, const char* msg, ...)
{
	log_firstcall();

	char line[TEXTWINDOW_MAXSTRINGLEN];
	uint32 count;
	std::va_list args;


	va_start(args, msg);
	count = std::vsprintf(line, msg, args);
	va_end(args);

/*	if( window->secondaryBuffer )
		Mem_Free( window->secondaryBuffer );
	window->secondaryBuffer = Mem_Alloc( (sizeof(char)*count) + 1 );
*/
	std::strcpy( window->secondaryBuffer, line );

	if( oneFrame )
		window->displayDelay = 0;
	else
		window->displayDelay = TEXTWINDOW_DISPLAYDELAY;

	window->flags |= TextWindowFlags::TEXTWINDOW_FLAG_OVERLAY;

}

// <LegoRR.exe @00485650>
bool32 __cdecl Gods98::TextWindow_Update(TextWindow* window, uint32 posFromEnd, real32 time, OUT sint32* lpLowestPoint)
{
	log_firstcall();

	uint32 loop, firstLine=0, sub;
	sint32 currWidth=0, currHeight;
	sint32 over, maxOver;
	char c;
	uint32 charWidth, wordWidth = 0;
	uint32 lineWidthList[TEXTWINDOW_MAXLINES];
	bool32  caretFlag;

	if (lpLowestPoint) *lpLowestPoint = 0;

#pragma message( "  Slightly rarer crash bug? - to do with buffer wrap and going over buffer end?" )
	// This may be fixed, it was caused if the text size was EXACTLY 1024 bytes
	// this was messing up because it would slip past one of the == checks below.
	//  (the above is a note from Gods98, not from OpenLRR)

	window->maxLines = (uint32) window->windowSize.height / window->font->fontHeight;

	if (!(window->flags & TextWindowFlags::TEXTWINDOW_FLAG_OVERLAY)) {

		maxOver = window->windowBufferSize;

		if (window->flags & TextWindowFlags::TEXTWINDOW_FLAG_WINDOWWRAPPED){
			// Find the first line after the buffer end...
			for (loop=0 ; loop<window->usedLines ; loop++){
				over = window->lineList[loop] - window->windowBufferEnd;
				if (over > 0 && over < maxOver){
					firstLine = window->lineList[loop];
					maxOver = over;
				}
			}
		}

		// Rebuild the line list from the new start point...
		window->usedLines = 0;
		window->lineList[window->usedLines++] = loop = firstLine;

		while (loop != window->windowBufferEnd){
			c = window->windowBuffer[loop];
			
			if (c == '\a') {			// use \a as a page end.
				break;
			} else if (c == '\n') {
				window->lineList[window->usedLines] = loop + 1;
				lineWidthList[window->usedLines-1] = currWidth;
				window->usedLines++;
				currWidth = 0;
				wordWidth = 0;
			} else {
				if (std::isspace((uint8)c)) {
					window->lineList[window->usedLines] = loop + 1;
					lineWidthList[window->usedLines-1] = currWidth;
					wordWidth = 0;
				}
				charWidth = Font_GetCharWidth(window->font, c);
				if ((uint8)c == 203) charWidth = 0;
				currWidth += charWidth;
				wordWidth += charWidth;
				if (currWidth >= window->windowSize.width) {
					// Check to see if the word is longer than the line
					if (wordWidth >= window->windowSize.width) {
						// If so, split the word onto the next line
						window->lineList[window->usedLines] = loop;
						lineWidthList[window->usedLines-1] = currWidth-charWidth;
						window->usedLines++;
						currWidth = charWidth;
						wordWidth = charWidth;
					} else {
						currWidth = wordWidth;
						window->usedLines++;
					}
				}
			}

			if (++loop >= window->windowBufferSize) loop = 0;
		}

		window->lineList[window->usedLines] = loop;
		lineWidthList[window->usedLines-1] = currWidth;

		// Display the required lines...
		if ((window->usedLines - window->maxLines + 1) > 0) posFromEnd = posFromEnd % ( window->usedLines - window->maxLines + 1 );
		else posFromEnd = 0;

		for (loop=0 ; loop<window->usedLines ; loop++){

			currWidth = (sint32) window->windowSize.x;

			if (window->flags & TextWindowFlags::TEXTWINDOW_FLAG_CENTERED) {
				currWidth += (sint32) ((window->windowSize.width - lineWidthList[loop]) / 2);
			}

			if (window->usedLines > window->maxLines && 
				loop >= window->usedLines - window->maxLines - posFromEnd &&
				loop < window->usedLines - posFromEnd) {

				currHeight = (sint32) window->windowSize.y + ((loop - (window->usedLines - window->maxLines - posFromEnd)) * window->font->fontHeight);
			} else if (window->usedLines <= window->maxLines) {
				currHeight = (sint32) window->windowSize.y + (loop * window->font->fontHeight);
			} else currHeight = 0;

			if (lpLowestPoint) {
				if (currHeight > *lpLowestPoint) *lpLowestPoint = currHeight;
			}

			if (currHeight) {
			
				sub = window->lineList[loop];
				while (sub != window->lineList[loop+1]) {
					char c = window->windowBuffer[sub];

					// errrrkkk!  crowbar some code in here so we can ignore the caret till the next char
					if ((uchar8)c == 203) 
					{
						// if we're at the caret position flag it and carry on
						caretFlag = true;
					}
					else
					{
						// if caretFlag is TRUE then the char before was the caret, safe to draw it now
						/// FIXME: == true BAAAD
						if (caretFlag == true)
						{
							int temp;
							// reset the caretFlag
							temp = Font_OutputChar(window->font, currWidth, currHeight, c, true);
							Font_OutputChar(window->font, currWidth-4, currHeight, 203, true);
							currWidth+=temp;
							caretFlag = false;
						}
						else
						{
							// output our normal char
							currWidth += Font_OutputChar(window->font, currWidth, currHeight, c, true);
						}
					}

					if ( ++sub == (window->windowBufferSize+1) ) sub = 0;

					/// FIXME: == true BAAAAAAAD
					if ((sub == window->lineList[loop+1]) && (caretFlag == true))
					{
						Font_OutputChar(window->font, currWidth-4, currHeight, 203, true);
						caretFlag = false;
					}

				}

			}

		}

		if (lpLowestPoint) *lpLowestPoint += window->font->fontHeight;

		//A NON OVERLAY TEXT WINDOW CANNOT OVER RUN, IT JUST WRAPS. SO RETURN FALSE
		return false;

	} else return TextWindow_UpdateOverlay( window, time, lpLowestPoint );

	//return posFromEnd;
	//return lowestPoint + window->font->fontHeight;
}

// <LegoRR.exe @004859d0>
bool32 __cdecl Gods98::TextWindow_UpdateOverlay(TextWindow* window, real32 time, OUT sint32* lpLowestPoint)
{
	log_firstcall();

	uint32 sub, loop = 0, usedLines = 0;
	uint32 lineList[TEXTWINDOW_MAXLINES];
	char c;
	sint32 currWidth = 0, currHeight;
	uint32 charWidth, wordWidth = 0;
	bool32 overRun = false;
	uint32 lineWidthList[TEXTWINDOW_MAXLINES];

	lineList[usedLines++] = loop;

	while ( (c = window->secondaryBuffer[loop]) != '\0' ) {

		if (c == '\n') {
			lineWidthList[usedLines-1] = currWidth;
			lineList[usedLines] = loop + 1;
			usedLines++;
			currWidth = 0;
			wordWidth = 0;
		} else {
			if (std::isspace((uchar8)c)) {
				lineWidthList[usedLines-1] = currWidth;
				lineList[usedLines] = loop + 1;
				wordWidth = 0;
			}
			charWidth = Font_GetCharWidth(window->font, c);
			currWidth += charWidth;
			wordWidth += charWidth;
			if (currWidth > window->windowSize.width) {
				currWidth = wordWidth;
				usedLines++;
			}
		}

		loop++;

	}

	lineWidthList[usedLines-1] = currWidth;
	lineList[usedLines] = loop;

	for (loop=0 ; loop<usedLines ; loop++){

		currWidth = (sint32) window->windowSize.x;
		if (window->flags & TextWindowFlags::TEXTWINDOW_FLAG_CENTERED) {
			currWidth += (sint32) ((window->windowSize.width - lineWidthList[loop]) / 2);
		}

		if (usedLines > window->maxLines && 
			loop >= usedLines - window->maxLines &&
			loop < usedLines )
		{
			currHeight = (sint32) window->windowSize.y + ((loop - (usedLines - window->maxLines)) * window->font->fontHeight);
			overRun = true;
		}
		else if (usedLines <= window->maxLines)
			currHeight = (sint32) window->windowSize.y + (loop * window->font->fontHeight);
		else currHeight = 0;

		if( lpLowestPoint )
		{
			if( currHeight > *lpLowestPoint )
				*lpLowestPoint = currHeight;
		}

		if (currHeight) {
		
			sub = lineList[loop];
			while (sub != lineList[loop+1]) {
				c = window->secondaryBuffer[sub++];
				currWidth += Font_OutputChar(window->font, currWidth, currHeight, c, true);
			}

		}

	}

	if( (window->displayDelay -= time) < 0 )
		window->flags &= ~TextWindowFlags::TEXTWINDOW_FLAG_OVERLAY;

	return overRun;
}

// <LegoRR.exe @00485c70>
void __cdecl Gods98::TextWindow_Clear(TextWindow* window)
{
	log_firstcall();

	std::memset(window->windowBuffer, 0, window->windowBufferSize);
	std::memset(window->secondaryBuffer, 0, sizeof(window->secondaryBuffer) /*TEXTWINDOW_MAXSTRINGLEN*/);
	window->usedLines = 0;
	window->windowBufferEnd = 0;
	window->flags &= ~TextWindowFlags::TEXTWINDOW_FLAG_WINDOWWRAPPED;
}

// <LegoRR.exe @00485cc0>
void __cdecl Gods98::TextWindow_GetInfo(TextWindow* window, OUT uint32* linesUsed, OUT uint32* linesInWindow)
{
	log_firstcall();

	*linesUsed = window->usedLines;
	*linesInWindow = window->maxLines;
}


// <unused>
sint32 __cdecl Gods98::TextWindow_GetDrawPos(TextWindow* window, uint32 chrPos, uint32 line, uint32 xpos, bool32 check)
{
	uint32 firstLine=0, sub;
	sint32 currWidth=0, currHeight;
	uint32 wordWidth = 0;
	uint32 lineWidthList[TEXTWINDOW_MAXLINES];
	uint32 posFromEnd = 0;
	sint32		diff = 9999999;
	sint32		charPos = 0;

	// Display the required lines...
	if ((window->usedLines - window->maxLines + 1) > 0) posFromEnd = posFromEnd % ( window->usedLines - window->maxLines + 1 );
	else posFromEnd = 0;

	currWidth = (sint32) window->windowSize.x;

	if (window->flags & TextWindowFlags::TEXTWINDOW_FLAG_CENTERED) 
	{
		currWidth += (sint32) ((window->windowSize.width - lineWidthList[line]) / 2);
	}

	if (window->usedLines > window->maxLines && 
		line >= window->usedLines - window->maxLines - posFromEnd &&
		line < window->usedLines - posFromEnd) 
	{
		currHeight = (sint32) window->windowSize.y + ((line - (window->usedLines - window->maxLines - posFromEnd)) * window->font->fontHeight);
	} else if (window->usedLines <= window->maxLines) {
		currHeight = (sint32) window->windowSize.y + (line * window->font->fontHeight);
	} else currHeight = 0;

	if (currHeight) 
	{
		sub = window->lineList[line];
		while (sub != window->lineList[line+1]) 
		{
			char c = window->windowBuffer[sub];
			
			/// FIXME: == true... BAD BAD BAD
			if (check == true)
			{
				if (sub == chrPos)	return currWidth;
			}
			else
			{
				if (std::abs((sint32)(currWidth-xpos)) < diff)
				{
					diff = std::abs((sint32)(currWidth-xpos));
					charPos = sub;
				}
			}

			currWidth += Font_GetCharWidth(window->font, c);

			if ( ++sub == (window->windowBufferSize+1) ) sub = 0;
		}
	}
	return charPos;
}

// <unused>
sint32 __cdecl Gods98::TextWindow_GetRow(TextWindow* window, sint32 chrPos, sint32 dir)
{
	for (uint32 loop = 0 ; loop < window->maxLines-1 ; loop++)
	{
		if (((uint32)chrPos >= window->lineList[loop]) && ((uint32)chrPos < window->lineList[loop+1]))
		{
			uint32 newRow = loop+dir;

			if (newRow < 0)					return chrPos;
			if (newRow >= window->maxLines)	return chrPos;

			// I know what row we want.  Need to find out the drawPos of the 2 rows.
			sint32 xpos1 = TextWindow_GetDrawPos(window, chrPos, loop, 0, TRUE);
			sint32 xpos2 = TextWindow_GetDrawPos(window, chrPos, newRow, xpos1, FALSE);

			if (dir > 0) return xpos2-1;
			return xpos2;
		}
	}
	return 0;
}

// <unused>
void __cdecl Gods98::TextWindow_SetFont(TextWindow* window, Font* font)
{
	// will need  TextWindow_Update for it to take affect
	if (font == nullptr)	return;
	window->font = font;
}

#pragma endregion