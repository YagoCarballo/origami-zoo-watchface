#include <pebble.h>

static const GPathInfo BIRD_WING_B = {
	.num_points = 3,
	.points = (GPoint []) {{42, 10}, {90, 47}, {64, 62}}
};

static const GPathInfo BIRD_HEAD = {
	.num_points = 3,
	.points = (GPoint []) {{90, 31}, {107, 40}, {90, 41}}
};

static const GPathInfo BIRD_BODY = {
	.num_points = 3,
	.points = (GPoint []) {{37, 79}, {90, 31}, {90, 47}}
};

static const GPathInfo BIRD_WING_F = {
	.num_points = 3,
	.points = (GPoint []) {{90, 47}, {66, 61}, {51, 2}}
};

//// Animation

static const GPathInfo BIRD_WING_B_START = {
	.num_points = 3,
	.points = (GPoint []) {{42, 10}, {90, 47}, {64, 62}}
};

static const GPathInfo BIRD_HEAD_START = {
	.num_points = 3,
	.points = (GPoint []) {{90, 31}, {107, 40}, {90, 41}}
};

static const GPathInfo BIRD_BODY_START = {
	.num_points = 3,
	.points = (GPoint []) {{37, 79}, {90, 31}, {90, 47}}
};

static const GPathInfo BIRD_WING_F_START = {
	.num_points = 3,
	.points = (GPoint []) {{90, 47}, {66, 61}, {51, 2}}
};

static const GPathInfo BIRD_WING_B_END = {
	.num_points = 3,
	.points = (GPoint []) {{39, 52}, {92, 32}, {64, 49}}
};

static const GPathInfo BIRD_HEAD_END = {
	.num_points = 3,
	.points = (GPoint []) {{92, 16}, {106, 27}, {92, 25}}
};

static const GPathInfo BIRD_BODY_END = {
	.num_points = 3,
	.points = (GPoint []) {{36, 67}, {92, 16}, {92, 32}}
};

static const GPathInfo BIRD_WING_F_END = {
	.num_points = 4,
	.points = (GPoint []) {{92, 32}, {64, 49}, {104, 79}}
};

void update_bird_part (GPath * part, const GPathInfo* start, const GPathInfo* end, float progress);
void update_bird (float progress, GPath * wing_b, GPath * head, GPath * body, GPath * wing_f);
