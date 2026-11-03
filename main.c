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

#define USER_APPLICATION_PRIORITY				1		//0 is low priority (idle task), 6 is max and generally reserved for critical tasks.
#define USER_APPLICATION_STACK_DEPTH_BYTES		12000

/**
 * @brief A task handle for application_task.
 */
static TaskHandle_t application_task_handle = NULL;

/**
 * @brief This is a task function created in int main. User code can start in this function
 * definition.
 */
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


	/**
	 * A pointer to a void function with no params can be passed here to run at priority 6 as
	 * soon as the scheduler starts. Useful if you are loading a UI and dont want to run a new task.
	 */
	system_set_application_init_cb(NULL);


	xTaskCreate(application_task, "APP_TASK", USER_APPLICATION_STACK_DEPTH_BYTES / 4, NULL, USER_APPLICATION_PRIORITY, &application_task_handle);

	/* Starts the FreeRTOS scheduler. */
	vTaskStartScheduler();

	/* Should never reach here. */
   	while(1);
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


