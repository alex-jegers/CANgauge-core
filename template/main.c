/**
 *
 * @file main.c
 * @author Jags Electronics
 * @brief Program entry point.
 *
 */

#include "cangauge.h"
#include "demos/benchmark/lv_demo_benchmark.h"
#include "demos/music/lv_demo_music.h"

/**
 * @brief A task handle for application_task.
 */
static TaskHandle_t application_task_handle = NULL;

/**
 * @brief Example initialization function. Gets passed to system_set_ui_init_cb and
 * creates a FreeRTOS task with the application_task function.
 */
static void application_init();
static void application_task(void* args);

int main(void)
{
	/* Configures the clocks to run at full speed in full power mode. */
	rcc_main_clock_config();

	/* Initializes the external SDRAM. */
	fmc_init_sdram();

	/* Enable the hardware floating point unit (ensure this is enabled in project settings too). */
	system_init_fpu();

	system_init();

	/* Creates a task to finish the rest of the system initialization. */
   	xTaskCreate(system_task_init, "SYS_INIT", 650, NULL, 5, NULL);


	/**************************************************************************************
	 * @attention A function pointer passed here will run during this initialization
	 * task at max FreeRTOS priority. Use this to load UIs, start application tasks, etc.
	 **************************************************************************************/
	system_set_application_init_cb(application_init);


	/* Starts the FreeRTOS scheduler. */
	vTaskStartScheduler();

	/* Should never reach here. */
   	while(1);
}

void application_init()
{
	/******************************************
	 *** USER APPLICATION SYSTEM
	 *** INITIALIZATION CODE GOES HERE.
	 *** THIS WILL RUN AT MAX PRIORITY (6).
	 ******************************************/

	lv_demo_music();
	//lv_demo_benchmark();


	/******************************************
	 *** USER APPLICATION INITIALIZATION	***
	 *** CODE END.							***
	 ******************************************/
	const uint32_t application_task_stack_depth_bytes = 12000;
	xTaskCreate(application_task, "APP_TASK", application_task_stack_depth_bytes / 4, NULL, 1, &application_task_handle);
}

void application_task(void* args)
{
	/******************************************
	 *** USER APPLICATION TASK INITIALIZATION
	 *** CODE GOES HERE. THIS WILL RUN
	 *** AT THIS TASKS PRIORITY (1).
	 ******************************************/




	while (1)
	{
		/***************************************
		 *** USER APPLICATION CODE GOES HERE ***
		 ***************************************/



		const uint32_t delay_time_ms = 25;		//Some amount of delay time is recommended to let the FreeRTOS idle task (priority 0) run.
		vTaskDelay(pdMS_TO_TICKS(delay_time_ms));
		/*********************************
		 *** END USER APPLICATION CODE ***
		 *********************************/
	}

}


