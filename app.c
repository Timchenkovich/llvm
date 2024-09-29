#include "sim.h"

// Current box position
int CUR_X;
int CUR_Y;

// Current box direction
int DIR_X;
int DIR_Y;

int COLOR;
int BACKGROUND_COLOR;
int BC_DELTA;

void init()
{
    CUR_X = simRand() % (SIM_X_SIZE - 11);
    CUR_Y = simRand() % (SIM_Y_SIZE - 11);
    DIR_X = (simRand() % 2) == 0 ? -1 : 1;
    DIR_Y = (simRand() % 2) == 0 ? -1 : 1;

    COLOR = simRand();
    BACKGROUND_COLOR = 0x00000000;
    BC_DELTA = -0x01010101;
}

void fill_background()
{
    for (int i = 0; i < SIM_X_SIZE; ++i)
    {
        for (int j = 0; j < SIM_Y_SIZE; ++j)
        {
            simPutPixel(i, j, BACKGROUND_COLOR);
        }
    }
}

void adjust_directions()
{
    if (CUR_X + 11 >= SIM_X_SIZE || CUR_X <= 0)
    {
        DIR_X = -DIR_X;
    }

    if (CUR_Y <= 0 || CUR_Y + 11 >= SIM_Y_SIZE)
    {
        DIR_Y = -DIR_Y;
    }
}

void adjust_positions()
{
    adjust_directions();
    CUR_X += DIR_X;
    CUR_Y += DIR_Y;
}

void change_colors()
{
    if (BACKGROUND_COLOR == 0xFFFFFFFF || BACKGROUND_COLOR == 0x00000000)
    {
        BC_DELTA = -BC_DELTA;
    }

    COLOR = simRand();
    BACKGROUND_COLOR += BC_DELTA;
}

void draw_happy()
{
    for (int i = 3; i >= 0; --i)
    {
        for (int j = i; j < i + 2; ++j)
        {
            simPutPixel(CUR_X + j, CUR_Y + (3 - i), COLOR);
        }

        for (int j = 10 - i; j > 8 - i; --j)
        {
            simPutPixel(CUR_X + j, CUR_Y + (3 - i), COLOR);
        }
    }

    for (int i = 4; i < 7; ++i)
    {
        simPutPixel(CUR_X, CUR_Y + i, COLOR);
        simPutPixel(CUR_X + 10, CUR_Y + i, COLOR);
    }

    for (int i = 3; i >= 0; --i)
    {
        for (int j = i; j < i + 2; ++j)
        {
            simPutPixel(CUR_X + j, CUR_Y + 10 - (3 - i), COLOR);
        }

        for (int j = 10 - i; j > 8 - i; --j)
        {
            simPutPixel(CUR_X + j, CUR_Y + 10 - (3 - i), COLOR);
        }
    }

    simPutPixel(CUR_X + 5, CUR_Y, COLOR);
    simPutPixel(CUR_X + 5, CUR_Y + 10, COLOR);

    simPutPixel(CUR_X + 4, CUR_Y + 3, COLOR);
    simPutPixel(CUR_X + 6, CUR_Y + 3, COLOR);

    simPutPixel(CUR_X + 3, CUR_Y + 6, COLOR);
    simPutPixel(CUR_X + 4, CUR_Y + 7, COLOR);
    simPutPixel(CUR_X + 5, CUR_Y + 7, COLOR);
    simPutPixel(CUR_X + 6, CUR_Y + 7, COLOR);
    simPutPixel(CUR_X + 7, CUR_Y + 6, COLOR);
}

void app()
{
    init();

    while (1)
    {
        fill_background();
        draw_happy();
        adjust_positions();
        change_colors();

        simFlush();
    }
}
