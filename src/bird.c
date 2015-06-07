#include <pebble.h>
#include "bird.h"

void update_bird_part (GPath * part, const GPathInfo* start, const GPathInfo* end, float progress)
{
  if (progress < 0.5f)
  {
	  for (uint32_t i=0;i<end -> num_points;i++)
	  {
		  int x = start -> points[i].x + ((end -> points[i].x - start -> points[i].x) * (2 * progress));
		  int y = start -> points[i].y + ((end -> points[i].y - start -> points[i].y) * (2 * progress));
		  GPoint new_point = GPoint(x, y);
		  part -> points[i] = new_point;
	  }
  }
  else
  {
	  for (uint32_t i=0;i<start -> num_points;i++)
	  {
		  int x = end -> points[i].x + ((start -> points[i].x - end -> points[i].x) * (2 * (progress - 0.5f)));
		  int y = end -> points[i].y + ((start -> points[i].y - end -> points[i].y) * (2 * (progress - 0.5f)));
		  GPoint new_point = GPoint(x, y);
		  part -> points[i] = new_point;
	  }
  }
}

void update_bird (float progress, GPath * wing_b, GPath * head, GPath * body, GPath * wing_f)
{
  update_bird_part(wing_b,	&BIRD_WING_B_START,	&BIRD_WING_B_END,	progress);
  update_bird_part(head,	&BIRD_HEAD_START,	&BIRD_HEAD_END,		progress);
  update_bird_part(body,	&BIRD_BODY_START,	&BIRD_BODY_END,		progress);
  update_bird_part(wing_f,	&BIRD_WING_F_START,	&BIRD_WING_F_END,	progress);
}
