/**********     INCLUDES        **********/
#include "shift_light_example.h"
#include "cangauge.h"
#include "math.h"

/**********		DEFINES		**********/
#define PRV_NUM_LEDS			9
#define PRV_SPAN_DEGREES		240.0
#define PRV_START_DEGREES		210.0
#define PRV_DIST_FROM_CENTER	190.0
#define PRV_LED_SIZE			45
#define PRV_SHIFT_POINT_RPM		6000

/**********		STATIC VARIABLES		**********/
static lv_obj_t* prv_leds[PRV_NUM_LEDS];	//An array of LEDs, at a quantity defined by a constant.
static lv_obj_t* prv_rpm_lbl;				//A label to display the RPM.

/**********		STATIC FUNCTION DECLRATIONS		**********/
static void prv_set_led_color(uint8_t led_index, lv_color_t color);
static void prv_set_led_brightness(uint8_t led_index, uint8_t brightness);
static void prv_set_led_default_colors() ;

/**********		STATIC FUNCTION DEFINITIONS		**********/
static void prv_set_led_color(uint8_t led_index, lv_color_t color)
{
	if (led_index > (PRV_NUM_LEDS - 1)) { return; }		//Make sure were not accessing outside of the LED array.
	lv_port_take_lvgl_mutex(portMAX_DELAY);
	lv_led_set_color(prv_leds[led_index], color);				//Change the color.
	lv_port_give_lvgl_mutex();
}

static void prv_set_led_brightness(uint8_t led_index, uint8_t brightness)
{
	if (led_index > (PRV_NUM_LEDS - 1)) { return; }		//Make sure were not accessing outside of the LED array.
	lv_port_take_lvgl_mutex(portMAX_DELAY);
	lv_led_set_brightness(prv_leds[led_index], brightness);		//Set the brightness.
	lv_port_give_lvgl_mutex();
}

static void prv_set_led_default_colors()
{
	/* Set the colors of the LEDs. Be careful not to exceed the number of PRV_NUM_LEDS. */
	prv_set_led_color(0, UI_COLOR_BLUE);
	prv_set_led_color(1, UI_COLOR_BLUE);
	prv_set_led_color(2, UI_COLOR_BLUE);
	prv_set_led_color(3, UI_COLOR_GREEN);
	prv_set_led_color(4, UI_COLOR_GREEN);
	prv_set_led_color(5, UI_COLOR_GREEN);
	prv_set_led_color(6, UI_COLOR_RED);
	prv_set_led_color(7, UI_COLOR_RED);
	prv_set_led_color(8, UI_COLOR_RED);
}

/**********		GLOBAL FUNCTION DEFINITIONS		**********/
void shift_light_example_init()
{
	/* Create the LVGL parent object. */
	lv_port_take_lvgl_mutex(portMAX_DELAY);
	lv_obj_t* prv_main_scr = lv_obj_create(NULL);
	lv_obj_set_size(prv_main_scr, 480, 480);
	lv_obj_set_style_bg_color(prv_main_scr, UI_COLOR_BLACK, 0);
	lv_obj_remove_flag(prv_main_scr, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_scrollbar_mode(prv_main_scr, LV_SCROLLBAR_MODE_OFF);

	/* Create and evenly space the LEDs based on how many are defined in PRV_NUM_LIGHT. */
	const float angle_increment_deg = PRV_SPAN_DEGREES / (float)(PRV_NUM_LEDS - 1);
	for (uint8_t i = 0; i < PRV_NUM_LEDS; i++)
	{
		prv_leds[i] = lv_led_create(prv_main_scr);
		lv_obj_set_size(prv_leds[i], PRV_LED_SIZE, PRV_LED_SIZE);
		float angle_pos_deg = PRV_START_DEGREES - (i * angle_increment_deg);
		float x_pos = PRV_DIST_FROM_CENTER * cos(angle_pos_deg * (M_PI / 180));
		float y_pos = PRV_DIST_FROM_CENTER * sin(angle_pos_deg * (M_PI / 180));
		y_pos *= -1.0;
		lv_obj_align(prv_leds[i], LV_ALIGN_CENTER, (int32_t)x_pos, (int32_t)y_pos);
	}

	/* Make a label to display the RPM and a label for the units. */
	prv_rpm_lbl = lv_label_create(prv_main_scr);
	lv_obj_t* units_lbl = lv_label_create(prv_main_scr);
	lv_obj_set_style_text_color(prv_rpm_lbl, UI_COLOR_WHITE, 0);
	lv_obj_set_style_text_color(units_lbl, UI_COLOR_WHITE, 0);
	lv_obj_set_style_text_font(prv_rpm_lbl, &lv_font_montserrat_36, 0);
	lv_obj_set_style_text_font(units_lbl, &lv_font_montserrat_18, 0);
	lv_obj_align(prv_rpm_lbl, LV_ALIGN_CENTER, 0, 0);
	lv_obj_align(units_lbl, LV_ALIGN_CENTER, 0, 30);
	lv_obj_set_style_text_align(prv_rpm_lbl, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_style_text_align(units_lbl, LV_TEXT_ALIGN_CENTER, 0);
	lv_label_set_text(prv_rpm_lbl, "0");
	lv_label_set_text_static(units_lbl, "RPM");

	/* Load the screen we just created. */
	lv_screen_load(prv_main_scr);

	lv_port_give_lvgl_mutex();

	/* Set the colors of the LEDs. */
	prv_set_led_color(0, UI_COLOR_BLUE);
	prv_set_led_color(1, UI_COLOR_BLUE);
	prv_set_led_color(2, UI_COLOR_BLUE);
	prv_set_led_color(3, UI_COLOR_GREEN);
	prv_set_led_color(4, UI_COLOR_GREEN);
	prv_set_led_color(5, UI_COLOR_GREEN);
	prv_set_led_color(6, UI_COLOR_RED);
	prv_set_led_color(7, UI_COLOR_RED);
	prv_set_led_color(8, UI_COLOR_RED);

	/* Setting up CAN for OBD diagnostics. */
	can_obd_run();					//Start the CAN OBD task

	/* Wait up to 10 seconds for it to initialize. */
	if (!can_obd_is_init(10000))
	{
		error_show_msgbox("Failed to initialize CAN on OBDII.");	//Show an error message if it times out.
		return;
	}


	uint32_t can_id = can_obd_get_query_can_id();	//Check if it found a UDS network.
	if (can_id == 0x00)
	{
		error_show_msgbox("No vehicle found.");		//Show an error message if it didn't
		return;
	}

	/* can_uds_current_data_t is a struct that hold information about each ISO 15031 / SAEJ1979 data PID.
	 * The name, PID, the information needed to convert the raw data, units, if it's 'nested', if it is
	 * supported by the current vehicle.
	 *
	 * We get the current data struct for RPM like this:
	 */
	can_obd_current_data_t* rpm_data = can_obd_get_current_data_by_name("Engine RPM");

	if (rpm_data == NULL)	//NULL check, make sure the name wasnt misspelled or something.
	{
		error_show_msgbox("RPM data not found.");
		return;
	}

	/* Checking if it's available will tell us if the car supports this data or not. */
	if (can_obd_get_current_data_availability(rpm_data) == false)
	{
		error_show_msgbox("RPM data not available.");
		return;
	}

	/* At this point we've connected to the car and verified the RPM data is available to us.
	 * We can set the query to continually poll the car for RPM data like so (this will ask the
	 * car for RPM data every 15ms):
	 */
	uint8_t rpm_pid = can_obd_get_current_data_pid(rpm_data);
	can_obd_set_current_data_query(rpm_pid, 0, 0, 0);
}

void shift_light_example_run()
{
	/* Get the latest RPM data. */
	can_obd_current_data_t* rpm_data = can_obd_get_current_data_by_name("Engine RPM");
	float rpm_value = can_obd_get_processed_current_data(rpm_data);

	/* Figure out how many lights to light up. */
	float frac = rpm_value / PRV_SHIFT_POINT_RPM;
	float num_leds_lit = frac * PRV_NUM_LEDS;
	if (num_leds_lit > PRV_NUM_LEDS) { num_leds_lit = PRV_NUM_LEDS; }

	if (rpm_value < PRV_SHIFT_POINT_RPM)
	{
		prv_set_led_default_colors();
		for (uint8_t i = 0; i < PRV_NUM_LEDS; i++)
		{
			lv_port_take_lvgl_mutex(portMAX_DELAY);
			if (i > num_leds_lit) { lv_led_off(prv_leds[i]); }	//Turn off LEDs above.
			else { lv_led_on(prv_leds[i]); }
			lv_port_give_lvgl_mutex();
		}
	}
	else	//Turn all the LEDs red and blink them 4 times a second.
	{
		static uint32_t last_time = 0;
		uint32_t current_time = xTaskGetTickCount();	//Get time in ms.
		if (current_time - last_time >= 125)
		{
			lv_port_take_lvgl_mutex(portMAX_DELAY);
			uint8_t brightness = lv_led_get_brightness(prv_leds[0]);
			lv_port_give_lvgl_mutex();
			brightness = (brightness == 255) ? 0 : 255;		//If brightness equals 255 set brightness to 0, else make it 255.
			for (uint8_t i = 0; i < PRV_NUM_LEDS; i++)
			{
				prv_set_led_color(i, UI_COLOR_RED);
				prv_set_led_brightness(i, brightness);
			}
			last_time = current_time;
		}
	}

	/* Set the label text. */
	lv_port_take_lvgl_mutex(portMAX_DELAY);
	lv_label_set_text_fmt(prv_rpm_lbl, "%.0f", rpm_value);
	lv_port_give_lvgl_mutex();
}
