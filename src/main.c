#include <pebble.h>
#include "origami_resources.h"
#include "bird.h"

static struct OrigamiUI
{
	Window		* window;
	Layer		* background;
	Layer		* battery;
	Layer		* animal;
	TextLayer	* hour;
	TextLayer	* date;
	GDrawCommandImage *ui_timebox;
} ui;

static struct UIElementsPaths
{
	GPath *timebox;
	GPath *date_line;
	GPath *battery_line;
	GPath *battery_base;
	GPath *battery_head;
} paths;

static struct Battery
{
	GColor8 color;
	int level : 28;
} battery;

static struct AnimalPaths
{
	GPath * wing_b;
	GPath * head;
	GPath * body;
	GPath * wing_f;
} animal_paths;

static struct AnimalInfo
{
	bool animationCreated;
	Origami currentAnimal;
	Animation * animation;
	GColor8 animalColor;
	GColor8 hudColor;
	GColor8 backgroundColor;
} animal_info;

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// ANIMATION ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void animal_animation_update (Animation *animation, const AnimationProgress progress)
{
  float percentage = ((float) progress / ANIMATION_NORMALIZED_MAX);

  update_bird(
	  percentage,
	  animal_paths.wing_b,
	  animal_paths.head,
	  animal_paths.body,
	  animal_paths.wing_f
  );

  layer_mark_dirty(ui.animal);
}

static const AnimationImplementation animal_animation_implementation = {
  .update = (AnimationUpdateImplementation) animal_animation_update
};

static void createAnimation ()
{
	if (animal_info.animationCreated == false)
	{
		animal_info.animationCreated = true;
		animal_info.animation = animation_create();
		animation_set_duration(animal_info.animation, 1200);
		animation_set_implementation(animal_info.animation, &animal_animation_implementation);
		animation_set_curve(animal_info.animation, AnimationCurveEaseInOut);
		animation_set_play_count(animal_info.animation, 5);
	}
}

static void startAnimation ()
{
	animation_schedule(animal_info.animation);
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// ANIMALS ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Bird

void create_bird ()
{
	animal_info.animalColor		= GColorRed;
	animal_info.backgroundColor	= GColorWhite;
	animal_info.hudColor		= GColorRajah;
	animal_paths.wing_b			= gpath_create(&BIRD_WING_B);
	animal_paths.head			= gpath_create(&BIRD_HEAD);
	animal_paths.body			= gpath_create(&BIRD_BODY);
	animal_paths.wing_f			= gpath_create(&BIRD_WING_F);
}

void destroy_bird ()
{
	gpath_destroy(animal_paths.wing_b);
	gpath_destroy(animal_paths.head);
	gpath_destroy(animal_paths.body);
	gpath_destroy(animal_paths.wing_f);
}


void change_animal (Origami animal)
{
	// if (animal_info.currentAnimal != animal)
	// {
	// 	switch (animal_info.currentAnimal)
	// 	{
	// 		case BUTTERFLY:
	// 		case SNAIL:
	// 			return;
	// 		case BIRD:
	// 			destroy_bird();
	// 	}
	// }

	switch (animal)
	{
		default:
		case BUTTERFLY:
		case SNAIL:
		case BIRD:
			create_bird();
	}

	window_set_background_color(ui.window, animal_info.backgroundColor);
}

void draw_animal_part (GContext* ctx, GPath * part, GColor8 color)
{
  // Fill the path
  graphics_context_set_fill_color(ctx, color);
  gpath_draw_filled(ctx, part);

  // Stroke the path
  graphics_context_set_stroke_color(ctx, GColorBlack);
  gpath_draw_outline(ctx, part);
}


////////////////////////////////////////////////////////////////////////////////
///////////////////////////// BATTERY //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static void update_battery_info (BatteryChargeState charge_state)
{
	float value = (charge_state.charge_percent / 100.0f);
	battery.level = 28 * value;

	if (value < 0.3f)
	{
		battery.color = GColorRed;
	}
	else if (value < 0.7f)
	{
		battery.color = GColorChromeYellow;
	}
	else
	{
		battery.color = GColorKellyGreen;
	}
}

static void battery_handler(BatteryChargeState charge_state) {
	update_battery_info(charge_state);
	layer_mark_dirty(ui.battery);
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// CLOCK /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  static char date_buffer[] = "Sat Jan 01";
  strftime(date_buffer, sizeof("Mon Jan 01"), "%a %b %d", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(ui.hour, buffer);
  text_layer_set_text(ui.date, date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// LAYERS //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static void background_update_proc(Layer *layer, GContext *ctx)
{
	// If the image was loaded successfully...
	if (ui.ui_timebox) {
		// Set the image's position
		GPoint origin = GPoint(0, -3);

		// Draw it
		gdraw_command_image_draw(ctx, ui.ui_timebox, origin);
	}

	// Stroke the Battery Line path
	gpath_draw_outline(ctx, paths.battery_head);

	// Stroke the Battery Line path
	gpath_draw_outline(ctx, paths.battery_base);
}

static void battery_update_proc(Layer *layer, GContext *ctx)
{
	// Fill the Battery Fill path
	graphics_context_set_fill_color(ctx, battery.color);
	GRect fill_bounds = GRect(98, 144, battery.level, 11);
	graphics_fill_rect(ctx, fill_bounds, 0, GCornerNone);
}

static void animal_update_proc(Layer *layer, GContext *ctx)
{
	draw_animal_part(ctx, animal_paths.wing_b,	animal_info.animalColor);
	draw_animal_part(ctx, animal_paths.head,	animal_info.animalColor);
	draw_animal_part(ctx, animal_paths.body,	animal_info.animalColor);
	draw_animal_part(ctx, animal_paths.wing_f,	animal_info.animalColor);
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// GUI //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setup_paths(void) {
	// Sets up the Battery Base Path
	paths.battery_base = gpath_create(&BATTERY_BASE_PATH);
	gpath_move_to(paths.battery_base, GPoint(0, -5));

	// Sets up the Battery Head Path
	paths.battery_head = gpath_create(&BATTERY_HEAD_PATH);
	gpath_move_to(paths.battery_head, GPoint(0, -5));
}

static void text_layer_prepare (TextLayer* layer, GFont font)
{
	text_layer_set_text(layer, "");
	text_layer_set_text_alignment(layer, GTextAlignmentCenter);
	text_layer_set_background_color(layer, GColorClear);
	text_layer_set_text_color(layer, GColorWhite);
	text_layer_set_font(layer, font);
}

static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	// Create animal Layer and set up the update procedure
	ui.animal = layer_create(bounds);
	layer_set_update_proc(ui.animal, animal_update_proc);
	layer_add_child(window_layer, ui.animal);

	// Create canvas Layer and set up the update procedure
	ui.background = layer_create(bounds);
	layer_set_update_proc(ui.background, background_update_proc);
	layer_add_child(window_layer, ui.background);

	// Creates the Hour Layer
	GRect hours_layer_bounds = GRect(0, 90, 144, 52);
	ui.hour = text_layer_create(hours_layer_bounds);

	GFont open_sans_light_40 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_OPEN_SANS_LIGHT_40));
	text_layer_prepare(ui.hour, open_sans_light_40);
	layer_add_child(window_layer, text_layer_get_layer(ui.hour));

	// Creates the Date Layer
	GRect date_layer_bounds = GRect(11, 142, 75, 17);
	ui.date = text_layer_create(date_layer_bounds);

	GFont open_sans_light_12 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_OPEN_SANS_LIGHT_12));
	text_layer_prepare(ui.date, open_sans_light_12);
	layer_add_child(window_layer, text_layer_get_layer(ui.date));

	// Make sure the time is displayed from the start
	update_time();

	// Checks the initial battery info
	BatteryChargeState charge_state = battery_state_service_peek();
	update_battery_info(charge_state);

	// Creates the path
	setup_paths();

	// Create battery Layer and set up the update procedure
	ui.battery = layer_create(bounds);
	layer_set_update_proc(ui.battery, battery_update_proc);
	layer_add_child(window_layer, ui.battery);

	// Load the background and check it was succcessful
	ui.ui_timebox = gdraw_command_image_create_with_resource(RESOURCE_ID_UI_TIMEBOX);
	if (!ui.ui_timebox) {
		APP_LOG(APP_LOG_LEVEL_ERROR, "Image is NULL!");
	}

	// Register with TickTimerService
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

	// Register Battery Service
	battery_state_service_subscribe(battery_handler);

	change_animal(BIRD);

	layer_mark_dirty(ui.animal);
	createAnimation();
	startAnimation();
}

static void window_unload(Window *window) {
	// Destroy canvas Layer
	layer_destroy(ui.background);
	layer_destroy(ui.battery);
	layer_destroy(ui.animal);

	// Destroy Text Layers
	text_layer_destroy(ui.hour);
	text_layer_destroy(ui.date);

	// Destroy the image
	gdraw_command_image_destroy(ui.ui_timebox);
}

static void init() {
  // Set up main Window
  ui.window = window_create();
  window_set_background_color(ui.window, GColorCadetBlue);
  window_set_window_handlers(ui.window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
  });

  window_stack_push(ui.window, true);
}

static void deinit() {
  // Destroy main Window
  window_destroy(ui.window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
