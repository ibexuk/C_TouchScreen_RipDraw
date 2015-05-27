

/*
Provided by IBEX UK LTD http://www.ibexuk.com
Electronic Product Design Specialists
RELEASED SOFTWARE

The MIT License (MIT)

Copyright (c) IBEX UK Ltd, http://ibexuk.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//Visit #### for more information
//
//Project Name:	RipDraw Touch Screen C Driver


//THIS DRIVER HAS BEEN WRITTEN FOR MICROCHIP XC16 WITH PIC24FJ256GB106 AND DENSITRON 84-0179-000T 7" TOUCH SCREEN
//2015-05-27 status:
//It works but in places has been hastily implemented to get the job done and only implements the rip draw commands we needed for our applicaiton.
//The densitron touch screens also need further development to fix a couple of issues we've found and to generally make development from a basic C environment easier.



/* HOW THIS DRIVER WORKS
 Some of the screen operations can take considerable time so this driver is based on a state machine which carries out the comms in the background.
 Actual UART comms can be interrupt based as implemented here or could be changed to non irq based if preffered.
 Whilst this driver uses the UART, SPI comms are identical.
 process_touch_screen_comms() is constantly called for the state machine to do its thing, starting individual command comms with the screen and retrying
 them if they fail for any reason.
 The main application can carry out one task on the screen at a time and this is based on it calling a fucntion such as touch_screen_load_image() and
 then calling touch_screen_is_last_command_complete() to wait for the command to finish and the screen to be ready for the next operation.
 This driver doesn't attempt to handle faults as if you try to sday load an image or font that isn't there then what is the driver going to do about that
 anyway. Errors can be detected from the returned ID values though if you want to detect issues in your application.

 HOW TO USE IMAGES:
	http://wiki1.globaldisplayengineering.com/clib:command_ref_image_using
*/

/*
##### INITIALISATION #####
	//----- SETUP UART 2 -----
	//Used for: Touch screen

	U2BRG = 103;			//38461.5baud with BRGH = 1
	U2MODE = 0x8008;		//8 bit, no parity, BRGH = high
	U2STA = 0x2510;			//TX IRQ when all data sent, RX IRQ on each byte
	ts_rx_no_of_bytes_to_rx = 0xffff;			//Flag that last transfer failed

	//USART 2 TX IRQ
	_U2TXIP = 3;		//Set irq priority (0-7, 7=highest, 0=disabled)
	_U2TXIF = 0;		//Reset irq
	//_U2TXIE = 0;		//Enable irq

	//USART 2 RX IRQ
	_U2RXIP = 3;		//Set irq priority (0-7, 7=highest, 0=disabled)
	_U2RXIF = 0;		//Reset irq
	_U2RXIE = 1;		//Enable irq


#### MAIN LOOP ####
	//----- PROCESS RS485 COMMS -----
	process_touch_screen_comms();

##### HEARTBEAT #####
	//----- TOUCH SCREEN TIMER -----
	if (ts_rx_1ms_timeout_timer)
		ts_rx_1ms_timeout_timer--;
	if (ts_state_1ms_timer)
		ts_state_1ms_timer--;
	if (ts_get_event_messges_1ms_timer)
		ts_get_event_messges_1ms_timer--;
	if (ts_delay_before_new_command_allowed_ms_timer)
		ts_delay_before_new_command_allowed_ms_timer--;
*/


/* ##### HOW TO USE EXAMPLES #####
//Use ripdraw to add all of the images and text using fonts you want to your design, and to get coordinates of how things will be placed.  Actual placement
 //is carried out in this code however so until ripdraw supports generating standard C code without a taret OS you don't actually get any benefit from
 //laying things out correctly in ripdraw. You just use to to get everthing included in the exported package which is uploaded to the screen and then
 //you manually show everthing using code like this


char ts_img_background_main[] = "bg_main";
uint16_t ts_img_background_main_id;

//Look in the "\fonts\" folder of the exported ripdraw project to find the names of the fonts that have been included
char ts_font_1[] = "VDS_18";
uint16_t ts_font_1_id;

uint16_t ts_image_1_write_id;
uint16_t ts_string_1_write_id;

 //The following are shown as switch case's as used like this your application to do other things and just periodically call this - i.e. not be stalled.
 //However you could just change each of these to while loops with a call to process_touch_screen_comms(); within each while instead if you wanted to
 //wait as each completed.
		switch (ts_io_sub_state)
		{
		//----------------------------
		//----- DO INITIAL SETUP -----
		//----------------------------

		//----- LOAD IMAGES -----
		case 0:
			if (touch_screen_load_image(ts_img_background_main))
				ts_io_sub_state++;
			break;
		case 1:
			if (touch_screen_is_last_command_complete(&ts_img_background_main_id))
				ts_io_sub_state++;
			break;
		//<<Add more images to load here

		//----- LOAD FONTS -----
		case 2:
			if (touch_screen_load_font(ts_font_1))
				ts_io_sub_state++;
			break;
		case 3:
			if (touch_screen_is_last_command_complete(&ts_font_1_id))
				ts_io_sub_state++;
			break;
		//<<Add more fonts to load here

		//----- SET TOUCH ZONES -----
		case 4:
			if (touch_screen_touch_map_rectangle ((char*)"a", 45, 389, 191, 86))				//Rhythm Generator 1
				ts_io_sub_state++;
			break;
		case 5:
			if (touch_screen_is_last_command_complete(&command_response))
				ts_io_sub_state++;
			break;
		//<<Add more touch zones here


 		//-------------------------
		//----- DISPLAY STUFF -----
		//-------------------------

		//----- ENABLE LAYER 1 -----
		case 6:
			if (touch_screen_layer_enable(TS_LAYER_1, 1))
				ts_io_sub_state++;
			break;
		case 7:
			if (touch_screen_is_last_command_complete(&command_response))
				ts_io_sub_state++;
			break;

		//----- IMAGE WRITE -----
		case 8:
			if (touch_screen_image_write(TS_LAYER_1, ts_img_background_main_id, 0, 0))
				ts_io_sub_state++;
			break;
		case 9:
			if (touch_screen_is_last_command_complete(&ts_image_1_write_id))
				ts_io_sub_state++;
			break;


		//----- ENABLE LAYER 2 -----
		case 10:
			if (touch_screen_layer_enable(TS_LAYER_2, 1))
				ts_io_sub_state++;
			break;
		case 11:
			if (touch_screen_is_last_command_complete(&command_response))
				ts_io_sub_state++;
			break;


		//----- WRITE STRING -----
		case 12:
			if (touch_screen_string_write ((char*)"Hello", TS_LAYER_2, 50, 100, ts_font_3_id, 0, TS_COLOR_WHITE))		//<<<<Possible string length bug - if it fails write a very short or blank string and then string_replace it
				ts_io_sub_state++;
			break;
		case 13:
			if (touch_screen_is_last_command_complete(&ts_string_1_write_id))
				ts_io_sub_state++;
			break;


		//----- COMPOSE LAYERS TO PAGE -----
		case 14:
			if (touch_screen_compose_layers_to_a_page(1))		//Page ID
				ts_io_sub_state++;
			break;
		case 15:
			if (touch_screen_is_last_command_complete(&command_response))
				ts_io_sub_state++;
			break;

		//------ PAGE TO SCREEN -----
		case 16:
			if (touch_screen_page_to_screen(1))
				ts_io_sub_state++;
			break;
		case 17:
			if (touch_screen_is_last_command_complete(&command_response))
				ts_io_sub_state++;
			break;



		//----- STRING REPLACE -----
		case 18:
			if (touch_screen_string_replace((char*) "New string", ts_string_1_write_id))
				ts_io_sub_state++;
			break;
		case 19:
			if (touch_screen_is_last_command_complete(&command_response))
				ts_io_sub_state++;
			break;

		//COMPOSE LAYERS TO PAGE
		case 20:
			if (touch_screen_compose_layers_to_a_page(1))		//Page ID
				ts_io_sub_state++;
			break;
		case 21:
			if (touch_screen_is_last_command_complete(&command_response))
				ts_io_sub_state++;
			break;

		//PAGE TO SCREEN
		case 22:
			if (touch_screen_page_to_screen(1))
				ts_io_sub_state++;
			break;
		case 23:
			if (touch_screen_is_last_command_complete(&command_response))
				ts_io_sub_state++;
			break;



		//----- STRING DELETE -----
		case 24:
			if (touch_screen_string_delete(ts_string_1_write_id))
				ts_io_sub_state++;
			break;
		case 25:
			if (touch_screen_is_last_command_complete(&command_response))
				ts_io_sub_state++;
			break;

		//COMPOSE LAYERS TO PAGE
		case 26:
			if (touch_screen_compose_layers_to_a_page(1))
				ts_io_sub_state++;
			break;
		case 27:
			if (touch_screen_is_last_command_complete(&command_response))
				ts_io_sub_state++;
			break;

		//PAGE TO SCREEN
		case 28:
			if (touch_screen_page_to_screen(1))
				ts_io_sub_state++;
			break;
		case 29:
			if (touch_screen_is_last_command_complete(&command_response))
				ts_io_sub_state++;
			break;



		//----- IMAGE DELETE -----
		case 30:
			if (touch_screen_image_delete(ts_image_1_write_id))
				ts_io_sub_state++;
			break;
		case 31:
			if (touch_screen_is_last_command_complete(&command_response))
				ts_io_sub_state++;
			break;

		//COMPOSE LAYERS TO PAGE
		case 32:
			if (touch_screen_compose_layers_to_a_page(1))
				ts_io_sub_state++;
			break;
		case 33:
			if (touch_screen_is_last_command_complete(&command_response))
				ts_io_sub_state++;
			break;

		//PAGE TO SCREEN
		case 34:
			if (touch_screen_page_to_screen(1))
				ts_io_sub_state++;
			break;
		case 35:
			if (touch_screen_is_last_command_complete(&command_response))
				ts_io_sub_state++;
			break;



//Detecting touch events in your application:
 	if (touch_event_received)
	{
		//----- THERE HAS BEEN A TOUCH EVENT -----
		//ts_touch_label_buffer[]  has the label

		touch_event_received = 0;				//Reset the flag
	}


*/




//********************************
//********************************
//********** DO DEFINES **********
//********************************
//********************************
#ifndef TOUCH_SCREEN_C_INIT		//Do only once the first time this file is used
#define TOUCH_SCREEN_C_INIT

#define	TS_RESET_APPLY			_TRISE3 = 0; _LATE3 = 0
#define	TS_RESET_RELEASE		_LATE3 = 1

#define	TS_BUFFER_LENGTH					250				//SET TX/RX BUFFER LENGTH
#define	TS_COMMAND_NAME_BUFFER_MAX_LENGTH	100				//Max length of screen file names and strings to be written
#define	TS_TOUCH_LABEL_BUFFER_MAX_LENGTH	32


#define	TS_ENABLE_TX_IRQ			_U2TXIE

#define	TS_TX_REG					U2TXREG
#define	TS_RX_REG					U2RXREG
#define	TS_STATUS_REG				U2STAbits		//<<####This code has special workaround for not being able to read/write U2STA - search for it and check for your ap####
#define	TS_TX_IRQ_FLAG				_U2TXIF
#define	TS_RX_IRQ_FLAG				_U2RXIF


//----- RIP DRAW -----
#ifdef TOUCH_SCREEN_C
	#define SEQUENCE_HIGH_BYTE 0x00
	#define SEQUENCE_LOW_BYTE 0x01
	#define CMD_RESET 0x3831
	#define CMD_IMAGE_LOAD 0x3231
	#define CMD_IMAGE_RELEASE 0x3232
	#define CMD_IMAGE_WRITE 0x3233
	#define CMD_IMAGE_DELETE 0x3234
	#define CMD_IMAGE_MOVE 0x3235
	#define CMD_IMAGE_LIST_LOAD 0x3236
	#define CMD_IMAGE_LIST_RELEASE 0x3237
	#define CMD_IMAGE_LIST_WRITE 0x3238
	#define CMD_IMAGE_LIST_REPLACE 0x3239
	#define CMD_IMAGE_LIST_DELETE 0x3240
	#define CMD_SET_LAYER_ENABLE 0x3131
	#define CMD_COMPOSE_LAYERS_TO_PAGE 0x3138
	#define CMD_PAGE_TO_SCREEN 0x3139
	#define CMD_SET_LAYER_BACK_COLOR 0x3133
	#define CMD_ANIMATION_PLAY 0x3241
	#define CMD_ANIMATION_STOP 0x3242
	#define CMD_SET_LAYER_TRANSPARENCY 0x3134
	#define CMD_LAYER_CLEAR 0x3135
	#define CMD_PARTIAL_COMPOSE_LAYER_TO_SCREEN 0x3141
	#define CMD_TOUCH_MAP_RECTANGLE 0x3531
	#define CMD_EVENT_MESSAGE 0x3832
	#define CMD_FONT_LOAD 0x3331
	#define CMD_STRING_WRITE 0x3334
	#define CMD_STRING_REPLACE 0x3335
	#define CMD_STRING_DELETE 0x3336
#endif

#define TS_LAYER_1		1			//<<Furthest back
#define TS_LAYER_2		2
#define TS_LAYER_3		3
#define TS_LAYER_4		4
#define TS_LAYER_5		5
#define TS_LAYER_6		6
#define TS_LAYER_7		7

#define TS_COLOR_WHITE		0xFFFFFFFF		// Red|Green|Blue|Alpha
#define TS_COLOR_YELLOW		0xFFFF00FF		// Red|Green|Blue|Alpha


typedef enum _TS_COMMS_STATE
{
    TS_POWERUP,
	TS_LAYER_ENABLE,
	TS_IMAGE_LOAD,
	TS_IMAGE_WRITE,
	TS_IMAGE_DELETE,
	TS_COMPOSE_LAYERS_TO_A_PAGE,
	TS_PAGE_TO_SCREEN,
	TS_TOUCH_MAP_RECTANGLE,
	TS_FONT_LOAD,
	TS_STRING_WRITE,
	TS_STRING_REPLACE,
	TS_STRING_DELETE,

	TS_GET_EVENT_MESSAGE,
	TS_IDLE
} TS_COMMS_STATE;



#endif



//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef TOUCH_SCREEN_C
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------
void ts_start_tx (BYTE* p_buffer, uint16_t timeout_time_ms);
void ts_start_tx_setup_done (void);
BYTE ts_get_string_length (char *string);


//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)
BYTE touch_screen_is_last_command_complete (uint16_t *command_response);
BYTE touch_screen_layer_enable (uint16_t layer_id, BYTE enable);
BYTE touch_screen_load_image (char *image_name);
BYTE touch_screen_image_write (uint16_t layer_id, uint16_t image_id, uint16_t x_position, uint16_t y_position);
BYTE touch_screen_image_delete (uint16_t image_write_id);
BYTE touch_screen_compose_layers_to_a_page (uint16_t page_id);
BYTE touch_screen_page_to_screen (uint16_t page_id);
BYTE touch_screen_touch_map_rectangle (char *label_name, uint16_t x_position, uint16_t y_position, uint16_t width, uint16_t height);
BYTE touch_screen_load_font (char *font_name);
BYTE touch_screen_string_write (char *string_to_write, uint16_t layer_id, uint16_t x_position, uint16_t y_position, uint16_t font_id, BYTE h_direction, uint32_t color_rgba);
BYTE touch_screen_string_replace (char *string_to_write, uint16_t string_write_id);
BYTE touch_screen_string_delete (uint16_t string_write_id);

void ts_transmit(void);
void ts_timer_irq (void);
void ts_receive (void);
void process_touch_screen_comms (void);


#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------
extern BYTE touch_screen_is_last_command_complete (uint16_t *command_response);
extern BYTE touch_screen_layer_enable (uint16_t layer_id, BYTE enable);
extern BYTE touch_screen_load_image (char *image_name);
extern BYTE touch_screen_image_write (uint16_t layer_id, uint16_t image_id, uint16_t x_position, uint16_t y_position);
extern BYTE touch_screen_image_delete (uint16_t image_write_id);
extern BYTE touch_screen_compose_layers_to_a_page (uint16_t page_id);
extern BYTE touch_screen_page_to_screen (uint16_t page_id);
extern BYTE touch_screen_touch_map_rectangle (char *label_name, uint16_t x_position, uint16_t y_position, uint16_t width, uint16_t height);
extern BYTE touch_screen_load_font (char *font_name);
extern BYTE touch_screen_string_write (char *string_to_write, uint16_t layer_id, uint16_t x_position, uint16_t y_position, uint16_t font_id, BYTE h_direction, uint32_t color_rgba);
extern BYTE touch_screen_string_replace (char *string_to_write, uint16_t string_write_id);
extern BYTE touch_screen_string_delete (uint16_t string_write_id);

extern void ts_transmit(void);
extern void ts_timer_irq (void);
extern void ts_receive (void);
extern void process_touch_screen_comms (void);


#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef TOUCH_SCREEN_C
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------
BYTE ts_current_mode;
BYTE ts_do_command = 0x00;
uint16_t ts_tx_rx_chksum;
uint16_t ts_tx_byte;
uint16_t ts_rx_byte;
uint16_t ts_tx_no_of_bytes_to_tx;
WORD ts_rx_1ms_timeout_time;
BYTE ts_transfer_in_progress = 0;
BYTE ts_comms_no_of_tries;
uint16_t ts_command_response;
uint16_t ts_rx_payload_length;

char ts_command_name_buffer[TS_COMMAND_NAME_BUFFER_MAX_LENGTH];
uint16_t ts_command_paramter1;
uint16_t ts_command_paramter2;
uint16_t ts_command_paramter3;
uint16_t ts_command_paramter4;
uint16_t ts_command_paramter5;
uint32_t ts_command_paramter_32bit_1;



//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)
BYTE ts_rx_failed;
uint16_t ts_rx_1ms_timeout_timer;
BYTE ts_tx_rx_buffer[TS_BUFFER_LENGTH];
uint16_t ts_rx_no_of_bytes_to_rx = 0;
BYTE ts_tx_no_of_bytes_to_tx_copy;
BYTE ts_tx_buffer_copy[TS_BUFFER_LENGTH];
volatile uint16_t ts_comms_good_10ms_timeout_timer = 0;
volatile uint16_t ts_state_1ms_timer = 0;
volatile BYTE ts_get_event_messges_1ms_timer = 0;
volatile uint16_t ts_delay_before_new_command_allowed_ms_timer = 0;
BYTE touch_event_received = 0;
BYTE ts_touch_label_buffer[TS_TOUCH_LABEL_BUFFER_MAX_LENGTH];

#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------
extern BYTE ts_rx_failed;
extern uint16_t ts_rx_1ms_timeout_timer;
extern BYTE ts_tx_rx_buffer[TS_BUFFER_LENGTH];
extern uint16_t ts_rx_no_of_bytes_to_rx;
extern BYTE ts_tx_no_of_bytes_to_tx_copy;
extern BYTE ts_tx_buffer_copy[TS_BUFFER_LENGTH];
extern volatile uint16_t ts_comms_good_10ms_timeout_timer;
extern volatile uint16_t ts_state_1ms_timer;
extern volatile BYTE ts_get_event_messges_1ms_timer;
extern volatile uint16_t ts_delay_before_new_command_allowed_ms_timer;
extern BYTE touch_event_received;
extern BYTE ts_touch_label_buffer[TS_TOUCH_LABEL_BUFFER_MAX_LENGTH];


#endif



