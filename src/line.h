#pragma once

typedef struct line
{
    int x, y;
} Line;

void printLine(Line* val);
void tPrintLine(Line* val);
Line* makeLine(int x, int y);
