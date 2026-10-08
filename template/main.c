/**
 ******************************************************************************
 * @file           : main.c
 * @author         : A. Jegers
 * @brief          : Main program body
 ******************************************************************************
 *
 ******************************************************************************
 */
#include "cangauge.h"
#include "demos/benchmark/lv_demo_benchmark.h"

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

	/* Load the menu screen. */
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
	 *** THIS WILL RUN BEFORE THE SCHEDULER
	 *** STARTS.
	 ******************************************/




	/******************************************
	 *** USER APPLICATION INITIALIZATION	***
	 *** CODE END.							***
	 ******************************************/
	xTaskCreate(application_task, "APP_TASK", 8000 / 4, NULL, 1, &application_task);
}

void application_task(void* args)
{
	/******************************************
	 *** USER APPLICATION TASK INITIALIZATION
	 *** CODE GOES HERE. THIS WILL RUN
	 *** AFTER THE SCHEDULER STARTS.
	 ******************************************/



	while (1)
	{
		/***************************************
		 *** USER APPLICATION CODE GOES HERE ***
		 ***************************************/



		//Some idle time to allow the FreeRTOS idle task to clean up memory is recommended.
		const uint32_t delay_time_ms = 50;
		vTaskDelay(pdMS_TO_TICKS(delay_time_ms));
		/*********************************
		 *** END USER APPLICATION CODE ***
		 *********************************/
	}

}


