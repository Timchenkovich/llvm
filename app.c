#include "sim.h"

// -------META PARAMS-------
const int RADIUS = 30;
const int STEP = 3;

// -------GLOBALS-------

// Current box position
int CUR_X;
int CUR_Y;

// Current box direction
int DIR_X;
int DIR_Y;

int COLOR;
int BACKGROUND_COLOR;
int BC_DELTA;

// -------CODE-------

void init()
{
    CUR_X = simRand() % (SIM_X_SIZE - 2 * RADIUS - STEP);
    CUR_Y = simRand() % (SIM_Y_SIZE - 2 * RADIUS - STEP);
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
    if (CUR_X + 2 * RADIUS + STEP >= SIM_X_SIZE || CUR_X - (STEP - 1) <= 0)
    {
        DIR_X = -DIR_X;
    }

    if (CUR_Y - (STEP - 1) <= 0 || CUR_Y + 2 * RADIUS + STEP >= SIM_Y_SIZE)
    {
        DIR_Y = -DIR_Y;
    }
}

void adjust_positions()
{
    adjust_directions();
    CUR_X += DIR_X * STEP;
    CUR_Y += DIR_Y * STEP;
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

void reflect_dots(int xc, int yc, int x, int y)
{
    simPutPixel(xc + x, yc + y, COLOR);
    simPutPixel(xc - x, yc + y, COLOR);
    simPutPixel(xc + y, yc + x, COLOR);
    simPutPixel(xc - y, yc + x, COLOR);
}

// dir == 1, if down half, dir == -1, otherwise
void draw_half_circle(int xc, int yc, int r, int dir)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;

    reflect_dots(xc, yc, dir * x, dir * y);

    while (y >= x)
    {

        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }

        x++;

        reflect_dots(xc, yc, dir * x, dir * y);
    }
}

void draw_circle(int xc, int yc, int r)
{
    draw_half_circle(xc, yc, r, 1);
    draw_half_circle(xc, yc, r, -1);
}

void app()
{
    init();

    while (1)
    {
        fill_background();

        draw_circle(CUR_X + RADIUS, CUR_Y + RADIUS, RADIUS);
        draw_circle(CUR_X + 2 * RADIUS / 3, CUR_Y + 2 * RADIUS / 3, RADIUS / 5);
        draw_circle(CUR_X + 4 * RADIUS / 3, CUR_Y + 2 * RADIUS / 3, RADIUS / 5);
        draw_half_circle(CUR_X + RADIUS, CUR_Y + 11 * RADIUS / 10, RADIUS / 2, 1);

        adjust_positions();
        change_colors();

        simFlush();
    }
}
