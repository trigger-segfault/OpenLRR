// Credits.cpp : 
//

#include "../../engine/core/Files.h"
#include "../../engine/core/Memory.h"
#include "../../engine/core/Utils.h"
#include "../../engine/drawing/Fonts.h"
#include "../../engine/input/Input.h"
#include "../../engine/input/Keys.h"
#include "../../engine/video/Animation.h"
#include "../../engine/Main.h"

#include "../audio/SFX.h"
#include "Credits.h"


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00409ff0>
void __cdecl LegoRR::Credits_Play(const char* textFile, Gods98::Font* font, const char* aviFile)
{
	char cconvBuff[1024];
	uint32 fileSize;

	// First try loading the filename with an added ".cconv" extension.
	std::sprintf(cconvBuff, "%s.cconv", textFile);
	char* text = (char*)Gods98::File_LoadBinary(cconvBuff, &fileSize);
	// Otherwise, try loading the filename as-is.
	if (!text) text = (char*)Gods98::File_LoadBinary(textFile, &fileSize);


	if (text != nullptr) {
		// Resize text to make room for a null terminator.
		text = (char*)Gods98::Mem_ReAlloc(text, fileSize + 4);
		text[fileSize] = '\0';

		Gods98::Animation_t* anim = Gods98::Animation_Load(aviFile);


		const Rect2I animDestRect = {
			0,
			0,
			Gods98::appWidth(),
			Gods98::appHeight(),
		};

		const sint32 appCenterX = (sint32)Gods98::appWidth() / 2;
		const uint32 lineHeight = Gods98::Font_GetHeight(font);
		const sint32 linesPerScreen = (Gods98::appHeight() / lineHeight) + 1; // +1 extra for intermediate scrolling


		// Count number of lines in the file with CRLF endings.
		// We need to count so that we can then allocate an array
		// to store them in before calling Util_Tokenise.
		/// FIXME: Consider not relying on CRLF line endings, since LRR doesn't care almost everywhere else.
		sint32 lineCount = 1; // Always start with 1 line (because we're counting separators between lines).
		for (uint32 i = 0; i < (fileSize - 1); i++) {
			if (text[i] == '\r' && text[i+1] == '\n') {
				lineCount++;
				i++; // skip second character
			}
		}

		// Split file lines.
		char** lines = (char**)Gods98::Mem_Alloc(lineCount * 4);
		Gods98::Util_Tokenise(text, lines, "\r\n");


		// Index of line for the first row at the top of the screen.
		// Start at -linesPerScreen so that all lines start just below the screen before moving up.
		sint32 lineStart = -linesPerScreen;
		// Intermediate distance from 0 to lineHeight, after which the value is reset to 0, and
		//  lineStart is incremented.
		uint32 scrollY = 0;

		uint32 timeLast = Gods98::Main_GetTime();

		bool32 mslbLast = false;
		bool32 finished = false;
		while (!finished) {

			/// CUSTOM: Debug key for faster credits testing (same key as 300% game speed).
			bool32 debugSpeed = (Gods98::Main_ProgrammerMode() > 0) && Input_KeyIsDown(Gods98::KEYPAD_7);
			// Is key pressed, or is left mouse button released?
			if ((!debugSpeed && Gods98::Input_AnyKeyPressed()) || (mslbLast && !Gods98::mslb())) {
				finished = true;
			}
			mslbLast = Gods98::mslb();

			// Update and render animation.
			if (anim && Gods98::Animation_IsOk(anim)) {
				Gods98::Animation_Update(anim);

				// Loop animation after reaching the end.
				if (Gods98::Animation_GetTime(anim) >= Gods98::Animation_GetLength(anim)) {
					/// TODO: Consider wrapping animation time instead of resetting to exactly zero?
					Gods98::Animation_SetTime(anim, 0);
				}
				Gods98::Animation_BlitToBackBuffer(anim, &animDestRect);
			}

			// Update line scrolling.
			// Scroll Y up by 1px every 33 milliseconds.
			const uint32 INTERVAL = (!debugSpeed ? 33 : 0);
			if (debugSpeed || (Gods98::Main_GetTime() - timeLast) > INTERVAL) {

				if (++scrollY >= lineHeight) {
					// At the end of the credits, start over from the beginning (user can only exit via input).
					if (lineStart >= lineCount) lineStart = -linesPerScreen;
					else                        lineStart++;
					scrollY = 0;
				}
				// Handling to keep a consistent speed.
				// We need to assign to Main_GetTime if higher, otherwise the comparison
				//  at the top of this block may cause integer underflow.
				timeLast = std::max((timeLast + INTERVAL), Gods98::Main_GetTime());
			}

			/// CUSTOM: When debugging, state our line position.
			if (Gods98::Main_ProgrammerMode() > 0) {
				Gods98::Font_PrintF(font, 0, 0, "line: %i/%i", lineStart, lineCount);
				if (!debugSpeed) {
					Gods98::Font_PrintF(font, 0, lineHeight, "scroll: %i/%i", scrollY, lineHeight);
				}
			}

			// Render lines.
			const sint32 startLineY = lineStart * lineHeight;
			sint32 lineY = startLineY;
			for (sint32 i = lineStart; i < lineCount; i++) {

				// If we've rendered past the last visible row, end the loop.
				if ((i - lineStart) > linesPerScreen)
					break;

				/// SPEED: Add check for non-empty lines.
				if (i >= 0 && lines[i][0] != '\0') {
					// Draw all lines centered.
					const uint32 lineWidth = Gods98::Font_GetStringWidth(font, "%s", lines[i]);
					sint32 x = (appCenterX - ((sint32)lineWidth / 2));
					sint32 y = ((lineY - startLineY) - scrollY);
					Gods98::Font_PrintF(font, x, y, "%s", lines[i]);
				}

				lineY += lineHeight;
			}

			SFX_Update(1.0f);

			// Update main input and render to screen.
			if (anim && Gods98::Animation_IsOk(anim)) {
				Gods98::Main_LoopUpdate(false);
			}
			else {
				// When not using animations, clear the background and wait 10ms,
				// since time isn't being spent on Animation drawing functions(?)
				Gods98::Main_LoopUpdate(true);
				/// QOL: Remove Sleep call, since the most significant impact to speed is Font drawing.
				//if (!debugSpeed) Gods98::Main_Sleep(10); // Replaces direct call to `::Sleep(10)`
			}
		}

		if (anim) Gods98::Animation_Free(anim);

		Gods98::Mem_Free(lines);
		Gods98::Mem_Free(text);
	}
}

#pragma endregion
