#include "sim.h"

void fill_background(int background_color) {
  for (int i = 0; i < SIM_X_SIZE; ++i) {
    for (int j = 0; j < SIM_Y_SIZE; ++j) {
      simPutPixel(i, j, background_color);
    }
  }
}

void adjust_directions(int cur_x, int cur_y, int *dir_x, int *dir_y, int RADIUS,
                       int STEP) {
  if (cur_x + 2 * RADIUS + STEP >= SIM_X_SIZE || cur_x - (STEP - 1) <= 0) {
    *dir_x = -*dir_x;
  }

  if (cur_y - (STEP - 1) <= 0 || cur_y + 2 * RADIUS + STEP >= SIM_Y_SIZE) {
    *dir_y = -*dir_y;
  }
}

void adjust_positions(int *cur_x, int *cur_y, int *dir_x, int *dir_y,
                      int RADIUS, int STEP) {
  adjust_directions(*cur_x, *cur_y, dir_x, dir_y, RADIUS, STEP);
  *cur_x += *dir_x * STEP;
  *cur_y += *dir_y * STEP;
}

void change_colors(int *background_color, int *bc_delta, int *color) {
  if (*background_color == 0xFFFFFFFF || *background_color == 0x00000000) {
    *bc_delta = -*bc_delta;
  }

  *color = simRand();
  *background_color += *bc_delta;
}

void reflect_dots(int xc, int yc, int x, int y, int color) {
  simPutPixel(xc + x, yc + y, color);
  simPutPixel(xc - x, yc + y, color);
  simPutPixel(xc + y, yc + x, color);
  simPutPixel(xc - y, yc + x, color);
}

// dir == 1, if down half, dir == -1, otherwise
void draw_half_circle(int xc, int yc, int r, int dir, int color) {
  int x = 0, y = r;
  int d = 3 - 2 * r;

  reflect_dots(xc, yc, dir * x, dir * y, color);

  while (y >= x) {

    if (d > 0) {
      y--;
      d = d + 4 * (x - y) + 10;
    } else {
      d = d + 4 * x + 6;
    }

    x++;

    reflect_dots(xc, yc, dir * x, dir * y, color);
  }
}

void draw_circle(int xc, int yc, int r, int color) {
  draw_half_circle(xc, yc, r, 1, color);
  draw_half_circle(xc, yc, r, -1, color);
}

void app() {
  // meta params
  const int RADIUS = 30;
  const int STEP = 3;

  // Current box position
  int cur_x = simRand() % (SIM_X_SIZE - 2 * RADIUS - STEP);
  int cur_y = simRand() % (SIM_Y_SIZE - 2 * RADIUS - STEP);

  // Current box direction
  int dir_x = (simRand() % 2) == 0 ? -1 : 1;
  int dir_y = (simRand() % 2) == 0 ? -1 : 1;

  int color = simRand();
  int background_color = 0x00000000;
  int bc_delta = -0x01010101;

  while (1) {
    fill_background(background_color);

    draw_circle(cur_x + RADIUS, cur_y + RADIUS, RADIUS, color);
    draw_circle(cur_x + 2 * RADIUS / 3, cur_y + 2 * RADIUS / 3, RADIUS / 5,
                color);
    draw_circle(cur_x + 4 * RADIUS / 3, cur_y + 2 * RADIUS / 3, RADIUS / 5,
                color);
    draw_half_circle(cur_x + RADIUS, cur_y + 11 * RADIUS / 10, RADIUS / 2, 1,
                     color);

    adjust_positions(&cur_x, &cur_y, &dir_x, &dir_y, RADIUS, STEP);
    change_colors(&background_color, &bc_delta, &color);

    simFlush();
  }
}
