#pragma once

#include <pebble.h>

typedef enum {
    BUTTERFLY,
    SNAIL,
    BIRD
} Origami;

// static const GPathInfo TIMEBOX_PATH = {
// 	.num_points = 17,
// 	.points = (GPoint []) {
// 	  	{ 138, 97 }, { 138, 94 }, { 136, 93 },
// 		{ 133, 93 }, { 10, 93 }, { 7, 93 },
// 		{ 6, 94 }, { 6, 97 }, { 6, 160 },
// 		{ 6, 163 }, { 7, 165 }, { 10, 165 },
// 		{ 133, 165 }, { 136, 165 }, { 138, 163 },
// 		{ 138, 160 }, { 138, 97 }
// 	}
// };
//
// static const GPathInfo DATE_LINE_PATH = {
// 	.num_points = 2,
// 	.points = (GPoint []) {{ 11, 144 }, { 132, 144 }}
// };
//
// static const GPathInfo BATTERY_LINE_PATH = {
// 	.num_points = 2,
// 	.points = (GPoint []) {{ 90, 162 }, { 90, 144 }}
// };

static const GPathInfo BATTERY_BASE_PATH = {
	.num_points = 17,
	.points = (GPoint []) {
		{126, 149}, {126, 149}, {125, 148},
		{124, 148}, {98, 148}, {98, 148},
		{97, 149}, {97, 149}, {97, 159},
		{97, 160}, {98, 160}, {98, 160},
		{124, 160}, {125, 160}, {126, 160},
		{126, 159}, {126, 149}
	}
};

static const GPathInfo BATTERY_HEAD_PATH = {
	.num_points = 17,
	.points = (GPoint []) {
		{129, 152}, {129, 152}, {129, 151},
		{129, 151}, {127, 151}, {127, 151},
		{126, 152}, {126, 152}, {126, 157},
		{126, 157}, {127, 157}, {127, 157},
		{129, 157}, {129, 157}, {129, 157},
		{129, 157}, {129, 152}
	}
};
