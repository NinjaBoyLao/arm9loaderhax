// Copyright 2013 Normmatt
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "font.h"
#include "draw.h"

#define STD_COLOR_BG   COLOR_BLACK
#define STD_COLOR_FONT COLOR_WHITE

#define DBG_COLOR_BG   COLOR_BLACK
#define DBG_COLOR_FONT COLOR_WHITE

#define DBG_START_Y 10
#define DBG_END_Y   (SCREEN_HEIGHT - 10)
#define DBG_START_X 10
#define DBG_END_X   (SCREEN_WIDTH_TOP - 10)
#define DBG_STEP_Y  10

#define DBG_N_CHARS_Y   ((DBG_END_Y - DBG_START_Y) / DBG_STEP_Y)
#define DBG_N_CHARS_X   (((DBG_END_X - DBG_START_X) / 8) + 1)

static char debugstr[DBG_N_CHARS_X * DBG_N_CHARS_Y] = { 0 };

void ClearScreen(u8* screen, int width, int color)
{
    if (color == COLOR_TRANSPARENT) color = COLOR_BLACK;
    for (int i = 0; i < (width * SCREEN_HEIGHT); i++) {
        *(screen++) = color >> 16;  // B
        *(screen++) = color >> 8;   // G
        *(screen++) = color & 0xFF; // R
    }
}

void ClearScreenFull(bool clear_top, bool clear_bottom)
{
    if (clear_top) {
        ClearScreen(TOP_SCREEN0, SCREEN_WIDTH_TOP, STD_COLOR_BG);
        ClearScreen(TOP_SCREEN1, SCREEN_WIDTH_TOP, STD_COLOR_BG);
    }
    if (clear_bottom) {
        ClearScreen(BOT_SCREEN0, SCREEN_WIDTH_BOT, STD_COLOR_BG);
        ClearScreen(BOT_SCREEN1, SCREEN_WIDTH_BOT, STD_COLOR_BG);
    }
}

void DrawCharacter(u8* screen, int character, int x, int y, int color, int bgcolor)
{
    for (int yy = 0; yy < 8; yy++) {
        int xDisplacement = (x * BYTES_PER_PIXEL * SCREEN_HEIGHT);
        int yDisplacement = ((SCREEN_HEIGHT - (y + yy) - 1) * BYTES_PER_PIXEL);
        u8* screenPos = screen + xDisplacement + yDisplacement;

        u8 charPos = font[character * 8 + yy];
        for (int xx = 7; xx >= 0; xx--) {
            if ((charPos >> xx) & 1) {
                *(screenPos + 0) = color >> 16;  // B
                *(screenPos + 1) = color >> 8;   // G
                *(screenPos + 2) = color & 0xFF; // R
            } else if (bgcolor != COLOR_TRANSPARENT) {
                *(screenPos + 0) = bgcolor >> 16;  // B
                *(screenPos + 1) = bgcolor >> 8;   // G
                *(screenPos + 2) = bgcolor & 0xFF; // R
            }
            screenPos += BYTES_PER_PIXEL * SCREEN_HEIGHT;
        }
    }
}

void DrawString(u8* screen, const char *str, int x, int y, int color, int bgcolor)
{
    for (int i = 0; i < strlen(str); i++)
        DrawCharacter(screen, str[i], x + i * 8, y, color, bgcolor);
}

void DrawStringF(int x, int y, bool use_top, const char *format, ...)
{
    char str[256] = {};
    va_list va;

    va_start(va, format);
    vsnprintf(str, 256, format, va);
    va_end(va);

    if (use_top) {
        DrawString(TOP_SCREEN0, str, x, y, STD_COLOR_FONT, STD_COLOR_BG);
        DrawString(TOP_SCREEN1, str, x, y, STD_COLOR_FONT, STD_COLOR_BG);
    } else {
        DrawString(BOT_SCREEN0, str, x, y, STD_COLOR_FONT, STD_COLOR_BG);
        DrawString(BOT_SCREEN1, str, x, y, STD_COLOR_FONT, STD_COLOR_BG);
    }
}

void DebugClear()
{
    memset(debugstr, 0x00, DBG_N_CHARS_X * DBG_N_CHARS_Y);
    ClearScreen(TOP_SCREEN0, SCREEN_WIDTH_TOP, DBG_COLOR_BG);
    ClearScreen(TOP_SCREEN1, SCREEN_WIDTH_TOP, DBG_COLOR_BG);
}

int firstTime = 0;

void Debug(const char *format, ...)
{
	if(!firstTime)
	{
		firstTime = 1;
		DebugClear();
	}
    char tempstr[DBG_N_CHARS_X] = { 0 };
    va_list va;
    
    va_start(va, format);
    vsnprintf(tempstr, DBG_N_CHARS_X, format, va);
    va_end(va);
    
    memmove(debugstr + DBG_N_CHARS_X, debugstr, DBG_N_CHARS_X * (DBG_N_CHARS_Y - 1));
    snprintf(debugstr, DBG_N_CHARS_X, "%-*.*s", DBG_N_CHARS_X - 1, DBG_N_CHARS_X - 1, tempstr);
    
    int pos_y = DBG_START_Y;
    for (char* str = debugstr + (DBG_N_CHARS_X * (DBG_N_CHARS_Y - 1)); str >= debugstr; str -= DBG_N_CHARS_X) {
        if (str[0] != '\0') {
            DrawString(TOP_SCREEN0, str, DBG_START_X, pos_y, DBG_COLOR_FONT, DBG_COLOR_BG);
            DrawString(TOP_SCREEN1, str, DBG_START_X, pos_y, DBG_COLOR_FONT, DBG_COLOR_BG);
            pos_y += DBG_STEP_Y;
        }
    }
}

void ShowProgress(u64 current, u64 total)
{
    const u32 progX = SCREEN_WIDTH_TOP - 40;
    const u32 progY = SCREEN_HEIGHT - 20;
    
    if (total > 0) {
        char progStr[8];
        snprintf(progStr, 8, "%3llu%%", (current * 100) / total);
        DrawString(TOP_SCREEN0, progStr, progX, progY, DBG_COLOR_FONT, DBG_COLOR_BG);
        DrawString(TOP_SCREEN1, progStr, progX, progY, DBG_COLOR_FONT, DBG_COLOR_BG);
    } else {
        DrawString(TOP_SCREEN0, "    ", progX, progY, DBG_COLOR_FONT, DBG_COLOR_BG);
        DrawString(TOP_SCREEN1, "    ", progX, progY, DBG_COLOR_FONT, DBG_COLOR_BG);
    }
}
