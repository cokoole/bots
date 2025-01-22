#pragma once

struct Coordinates {
  int x;
  int y;
};

struct Mantis_coord {
  int offset_x;
  int offset_y;
  int width;
  int height;
  Coordinates coord_click;
};

enum Type_res {
  food =      0,
  leaves =    1,
  water =     2,
  mushrooms = 3
};
