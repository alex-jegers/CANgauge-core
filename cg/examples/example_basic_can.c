/**********     INCLUDES        **********/
#include "example_basic_can.h"
#include "cangauge.h"

/**********     TYPEDEFS         **********/

/**********		DEFINES		**********/

/**********		EXTERNAL VARIABLE DEFINITIONS		**********/

/**********		STATIC VARIABLES		**********/
static TaskHandle_t prv_task_to_notify;

/**********		STATIC FUNCTION DECLRATIONS		**********/
static void prv_can2_rx_handler()
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	vTaskNotifyGiveFromISR(prv_task_to_notify, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
/**********		STATIC FUNCTION DEFINITIONS		**********/

/**********		GLOBAL FUNCTION DEFINITIONS		**********/
void example_basic_can_init(TaskHandle_t task_to_notify)
{
	prv_task_to_notify = task_to_notify;

	can_init();
	can_set_baud_rate(FDCAN1, CAN_BAUD_500K);
	can_set_baud_rate(FDCAN2, CAN_BAUD_500K);
	can_run(FDCAN1);
	can_run(FDCAN2);
	can_transmit_run(FDCAN1, 15);
	can_enable_rx_rf0n_interrupt(FDCAN2);
	can_assign_rx_rf0n_cb(FDCAN2, prv_can2_rx_handler);

	/* Create a message to transmit. */
	can_tx_buffer_entry_t tx;
	memset(&tx, 0, sizeof(can_tx_buffer_entry_t));
	uint8_t tx_data[8] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0 };
	can_set_can_id(&tx, 0x1, CAN_ID_STD);
	can_set_data(&tx, tx_data, 8);

	/* Create a message to transmit. */
	can_tx_buffer_entry_t tx2;
	memset(&tx2, 0, sizeof(can_tx_buffer_entry_t));
	uint8_t tx2_data[5] = {0xaa, 0x55, 0x10, 0x00, 0xf2 };
	can_set_can_id(&tx2, 0x1, CAN_ID_STD);
	can_set_data(&tx2, tx2_data, 5);

	/* Create a transmit handle. */
	can_transmit_handle_t* tx_hndl = can_transmit_create_msg();
	assert( tx_hndl != NULL );
	can_transmit_set_msg_data(tx_hndl, &tx);		//Set the data to be transmitted.
	can_transmit_set_period(tx_hndl, 100);			//Attempt to send it every 100ms.
	can_transmit_set_active(tx_hndl);				//Activate it.

	/* Create a transmit handle. */
	can_transmit_handle_t* tx2_hndl = can_transmit_create_msg();
	assert( tx2_hndl != NULL );
	can_transmit_set_msg_data(tx2_hndl, &tx2);		//Set the data to be transmitted.
	can_transmit_set_period(tx2_hndl, 500);			//Attempt to send it every 500ms.
	can_transmit_set_active(tx2_hndl);				//Activate it.

}

void example_basic_can_run()
{
	if (ulTaskNotifyTake(pdFALSE, portMAX_DELAY))
	{
		/* Read a message from FIFO0. */
		can_rx_buffer_entry_t rx;
		memset(&rx, 0, sizeof(can_rx_buffer_entry_t));
		can_read_from_fifo0(FDCAN2, &rx);
		uint8_t rx_data[8];
		uint32_t dlc = can_get_data(&rx, rx_data);

		/* Using the driver level access, echo it on CAN2. */
		can_tx_buffer_entry_t tx;
		memset(&tx, 0, sizeof(can_tx_buffer_entry_t));
		can_set_can_id(&tx, 2, CAN_ID_STD);
		can_set_data(&tx, rx_data, dlc);
		can_tx_buffer_entry_t* tx_buf = can_get_tx_buffer(FDCAN2, 0);
		memcpy(tx_buf, &tx, sizeof(can_tx_buffer_entry_t));
		can_tx(FDCAN2, 0);
	}
}
