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


#include "main.h"
#define TOUCH_SCREEN_C
#include "touch-screen.h"





//**********************************************
//**********************************************
//********** IS LAST COMMAND COMPLETE **********
//**********************************************
//**********************************************
BYTE touch_screen_is_last_command_complete (uint16_t *command_response)
{
	*command_response = ts_command_response;
	if (ts_do_command == 0x00)
		return(1);
	else
		return(0);
}





//**********************************
//**********************************
//********** LAYER ENABLE **********
//**********************************
//**********************************
//Returns
//	0 = not possible currently (another action is in progress)
//	1 = command started
//		Call touch_screen_is_last_command_complete() to find out when it has been completed
//		command_response will be the status returned by the screen
BYTE touch_screen_layer_enable (uint16_t layer_id, BYTE enable)
{
	if (ts_do_command != 0x00)
		return(0);

	ts_command_paramter1 = layer_id;			//ts_command_paramter1 is layer ID
	ts_command_paramter2 = enable;				//ts_command_paramter2 is enable / disable

	ts_do_command = TS_LAYER_ENABLE;
	return(1);
}




//********************************
//********************************
//********** LOAD IMAGE **********
//********************************
//********************************
//Returns
//	0 = not possible currently (another action is in progress)
//	1 = command started
//		Call touch_screen_is_last_command_complete() to find out when it has been completed
//		command_response will be the imageLoadId returned by the screen, or 0xffff if operation failed
//BUG FOUND 2015-05 - IMAGE NAMES > 15 CHARACTERS MAY NOT WORK - LOOKS LIKE SCREEN HAS AN INTERNAL BUFFER OVERRUN PROBLEM THAT NEEDS FIXING
BYTE touch_screen_load_image (char *image_name)
{
	BYTE count;
	if (ts_do_command != 0x00)
		return(0);

	//Copy the name
	for (count = 0; count < TS_COMMAND_NAME_BUFFER_MAX_LENGTH; count++)
	{
		ts_command_name_buffer[count] = image_name[count];
		if (image_name[count] == 0x00)
			break;
	}

	ts_do_command = TS_IMAGE_LOAD;
	return(1);
}


//*********************************
//*********************************
//********** IMAGE WRITE **********
//*********************************
//*********************************
//Returns
//	0 = not possible currently (another action is in progress)
//	1 = command started
//		Call touch_screen_is_last_command_complete() to find out when it has been completed
//		command_response will be the ImageWriteId returned by the screen, or 0xffff if operation failed
BYTE touch_screen_image_write (uint16_t layer_id, uint16_t image_id, uint16_t x_position, uint16_t y_position)
{
	if (ts_do_command != 0x00)
		return(0);

	ts_command_paramter1 = layer_id;			//ts_command_paramter1 is Layer ID
	ts_command_paramter2 = image_id;			//ts_command_paramter2 is Image ID
	ts_command_paramter3 = x_position;			//ts_command_paramter3 is X Position
	ts_command_paramter4 = y_position;			//ts_command_paramter4 is Y Position


	ts_do_command = TS_IMAGE_WRITE;
	return(1);
}


//**********************************
//**********************************
//********** DELETE IMAGE **********
//**********************************
//**********************************
//Returns
//	0 = not possible currently (another action is in progress)
//	1 = command started
//		Call touch_screen_is_last_command_complete() to find out when it has been completed
//		command_response will be the status returned by the screen, or 0xffff if operation failed
BYTE touch_screen_image_delete (uint16_t image_write_id)
{
	if (ts_do_command != 0x00)
		return(0);

	ts_command_paramter1 = image_write_id;				//ts_command_paramter1 is Image Write ID

	ts_do_command = TS_IMAGE_DELETE;
	return(1);
}




//**********************************************
//**********************************************
//********** COMPOSE LAYERS TO A PAGE **********
//**********************************************
//**********************************************
//page_id		A user set value
//Returns
//	0 = not possible currently (another action is in progress)
//	1 = command started
//		Call touch_screen_is_last_command_complete() to find out when it has been completed
//		command_response will be the status returned by the screen
BYTE touch_screen_compose_layers_to_a_page (uint16_t page_id)
{
	if (ts_do_command != 0x00)
		return(0);

	ts_command_paramter1 = page_id;			//ts_command_paramter1 is page ID

	ts_do_command = TS_COMPOSE_LAYERS_TO_A_PAGE;
	return(1);
}




//**********************************************
//**********************************************
//********** COMPOSE LAYERS TO A PAGE **********
//**********************************************
//**********************************************
//Returns
//	0 = not possible currently (another action is in progress)
//	1 = command started
//		Call touch_screen_is_last_command_complete() to find out when it has been completed
//		command_response will be the status returned by the screen
BYTE touch_screen_page_to_screen (uint16_t page_id)
{
	if (ts_do_command != 0x00)
		return(0);

	ts_command_paramter1 = page_id;			//ts_command_paramter1 is page ID

	ts_do_command = TS_PAGE_TO_SCREEN;
	return(1);
}





//*****************************************
//*****************************************
//********** TOUCH MAP RECTANGLE **********
//*****************************************
//*****************************************
//Returns
//	0 = not possible currently (another action is in progress)
//	1 = command started
//		Call touch_screen_is_last_command_complete() to find out when it has been completed
BYTE touch_screen_touch_map_rectangle (char *label_name, uint16_t x_position, uint16_t y_position, uint16_t width, uint16_t height)
{
	BYTE count;
	if (ts_do_command != 0x00)
		return(0);

	//Copy the name
	for (count = 0; count < TS_COMMAND_NAME_BUFFER_MAX_LENGTH; count++)
	{
		ts_command_name_buffer[count] = label_name[count];
		if (label_name[count] == 0x00)
			break;
	}

	ts_command_paramter1 = x_position;			//ts_command_paramter1 is X Position
	ts_command_paramter2 = y_position;			//ts_command_paramter2 is Y Position
	ts_command_paramter3 = width;				//ts_command_paramter3 is Width
	ts_command_paramter4 = height;				//ts_command_paramter4 is Height


	ts_do_command = TS_TOUCH_MAP_RECTANGLE;
	return(1);
}






//*******************************
//*******************************
//********** LOAD FONT **********
//*******************************
//*******************************
//Returns
//	0 = not possible currently (another action is in progress)
//	1 = command started
//		Call touch_screen_is_last_command_complete() to find out when it has been completed
//		command_response will be the FontId returned by the screen, or 0xffff if operation failed
BYTE touch_screen_load_font (char *font_name)
{
	BYTE count;
	if (ts_do_command != 0x00)
		return(0);

	//Copy the name
	for (count = 0; count < TS_COMMAND_NAME_BUFFER_MAX_LENGTH; count++)
	{
		ts_command_name_buffer[count] = font_name[count];
		if (font_name[count] == 0x00)
			break;
	}

	ts_do_command = TS_FONT_LOAD;
	return(1);
}



//**********************************
//**********************************
//********** WRITE STRING **********
//**********************************
//**********************************
//Returns
//	0 = not possible currently (another action is in progress)
//	1 = command started
//		Call touch_screen_is_last_command_complete() to find out when it has been completed
//		command_response will be the WriteStringId returned by the screen, or 0xffff if operation failed
BYTE touch_screen_string_write (char *string_to_write, uint16_t layer_id, uint16_t x_position, uint16_t y_position, uint16_t font_id, BYTE h_direction, uint32_t color_rgba)
{
	BYTE count;
	if (ts_do_command != 0x00)
		return(0);

	//Copy the string
	for (count = 0; count < TS_COMMAND_NAME_BUFFER_MAX_LENGTH; count++)
	{
		ts_command_name_buffer[count] = string_to_write[count];
		if (string_to_write[count] == 0x00)
			break;
	}

	ts_command_paramter1 = layer_id;				//ts_command_paramter1 is Layer ID
	ts_command_paramter2 = x_position;				//ts_command_paramter2 is X Position
	ts_command_paramter3 = y_position;				//ts_command_paramter3 is Y Position
	ts_command_paramter4 = font_id;					//ts_command_paramter4 is Font ID
	ts_command_paramter5 = h_direction;				//ts_command_paramter5 is H Direction
	ts_command_paramter_32bit_1 = color_rgba;		//ts_command_paramter_32bit_1 is: Red | Green | Blue | Alpha

	ts_do_command = TS_STRING_WRITE;
	return(1);
}


//************************************
//************************************
//********** REPLACE STRING **********
//************************************
//************************************
//Returns
//	0 = not possible currently (another action is in progress)
//	1 = command started
//		Call touch_screen_is_last_command_complete() to find out when it has been completed
//		command_response will be the status returned by the screen, or 0xffff if operation failed
BYTE touch_screen_string_replace (char *string_to_write, uint16_t string_write_id)
{
	BYTE count;
	if (ts_do_command != 0x00)
		return(0);

	//Copy the string
	for (count = 0; count < TS_COMMAND_NAME_BUFFER_MAX_LENGTH; count++)
	{
		ts_command_name_buffer[count] = string_to_write[count];
		if (string_to_write[count] == 0x00)
			break;
	}

	ts_command_paramter1 = string_write_id;				//ts_command_paramter1 is String Write ID

	ts_do_command = TS_STRING_REPLACE;
	return(1);
}


//***********************************
//***********************************
//********** DELETE STRING **********
//***********************************
//***********************************
//Returns
//	0 = not possible currently (another action is in progress)
//	1 = command started
//		Call touch_screen_is_last_command_complete() to find out when it has been completed
//		command_response will be the status returned by the screen, or 0xffff if operation failed
BYTE touch_screen_string_delete (uint16_t string_write_id)
{
	if (ts_do_command != 0x00)
		return(0);

	ts_command_paramter1 = string_write_id;				//ts_command_paramter1 is String Write ID

	ts_do_command = TS_STRING_DELETE;
	return(1);
}





























//************************************************************************************************************************************************************
//************************************************************************************************************************************************************
//************************************************************************************************************************************************************
//************************************************************************************************************************************************************
//************************************************************************************************************************************************************
//************************************************************************************************************************************************************
//************************************************************************************************************************************************************
//
//************************************************
//************************************************
//********** PROCESS TOUCH SCREEN COMMS **********
//************************************************
//************************************************
void process_touch_screen_comms (void)
{
	static BYTE ts_current_mode_last = 0xff;
	static BYTE power_up_waiting_for_screen = 1;
	BYTE ts_just_entered_new_mode = 0;
	BYTE b_temp;
	BYTE* p_buffer;
	uint16_t length;
	uint16_t count;
	uint16_t event_data_length;
	uint16_t no_of_events;
	uint16_t event_length;
	BYTE event_type;




	if (ts_transfer_in_progress)
	{
		//Transfer complete?
		if ((ts_rx_no_of_bytes_to_rx != 0) && (ts_rx_no_of_bytes_to_rx != 0xffff) && (ts_rx_1ms_timeout_timer != 0))
			return;

		if (ts_rx_1ms_timeout_timer == 0)
			ts_rx_failed = 1;
		if (ts_rx_no_of_bytes_to_rx == 0xffff)
			ts_rx_failed = 1;

		//--------------------------
		//----- COMMS COMPLETE -----
		//--------------------------
		ts_transfer_in_progress = 0;
		if (!ts_rx_failed)
		{
			//-------------------------
			//-------------------------
			//----- COMMS SUCCESS -----
			//-------------------------
			//-------------------------
			ts_comms_good_10ms_timeout_timer = 200;


		}
		else
		{
			//---------------------------
			//----- TRANSFER FAILED -----
			//---------------------------
			ts_comms_no_of_tries++;
			if (ts_comms_no_of_tries < 3)		//<Set number of attempts if comms fail for any reason
			{
				//---------------------
				//----- TRY AGAIN -----
				//---------------------
				//Copy the original tx packet
				for (b_temp = 0; b_temp < TS_BUFFER_LENGTH; b_temp++)
					ts_tx_rx_buffer[b_temp] = ts_tx_buffer_copy[b_temp];
				ts_tx_no_of_bytes_to_tx = ts_tx_no_of_bytes_to_tx_copy;

				ts_start_tx_setup_done();
				return;
			}
			else
			{
				//-------------------
				//----- GIVE UP -----
				//-------------------
				ts_tx_rx_buffer[0] = 0;
				ts_tx_rx_buffer[1] = 0;
				ts_tx_rx_buffer[2] = 0;
				ts_tx_rx_buffer[3] = 0;
				ts_tx_rx_buffer[4] = 0;
				ts_tx_rx_buffer[5] = 0;
				ts_tx_rx_buffer[6] = 0;
				ts_tx_rx_buffer[7] = 0;

			}
		}
	} //if (ts_transfer_in_progress)



	//-------------------------------
	//-------------------------------
	//----- START NEXT TRANSFER -----
	//-------------------------------
	//-------------------------------





	//------------------------------
	//----- CHECK FOR NEW MODE -----
	//------------------------------
	ts_just_entered_new_mode = 0;
	if (ts_current_mode != ts_current_mode_last)
	{
		//Flag that we've just entered a new mode
		ts_just_entered_new_mode = 1;
		ts_current_mode_last = ts_current_mode;
	}


	//------------------------------------
	//------------------------------------
	//----- PROCESS THE CURRENT MODE -----
	//------------------------------------
	//------------------------------------
	switch (ts_current_mode)
	{
	case TS_POWERUP:
		//-------------------
		//-------------------
		//----- POWERUP -----
		//-------------------
		//-------------------
		if (ts_just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			//#ifdef __DEBUG
			//	ts_state_1ms_timer = 500;
			//#else
				TS_RESET_APPLY;
				Nop();
				Nop();
				Nop();
				Nop();
				Nop();
				Nop();
				Nop();
				Nop();
				Nop();
				Nop();
				TS_RESET_RELEASE;
				ts_state_1ms_timer = 8000;		//Wait for the screen to nearly be ready (we will poll for comms success before starting with setup commands)
			//#endif
		}
		//------------------------
		//----- PROCESS MODE -----
		//------------------------

		if (ts_state_1ms_timer == 0)
			ts_current_mode = TS_IDLE;

		break;



	case TS_LAYER_ENABLE:
		//------------------------
		//------------------------
		//----- LAYER ENABLE -----
		//------------------------
		//------------------------
		if (ts_just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			p_buffer = &ts_tx_rx_buffer[0];

			//Command
			*p_buffer++ = (BYTE)(CMD_SET_LAYER_ENABLE & 0x00ff);
			*p_buffer++ = (BYTE)(CMD_SET_LAYER_ENABLE >> 8);

			//Sequence Number
			*p_buffer++ = 0x01;
			*p_buffer++ = 0x00;

			//Payload Length
			*p_buffer++ = 0;			//Will be set by tx function
			*p_buffer++ = 0;

			//THE PAYLOAD
			*p_buffer++ = (BYTE)(ts_command_paramter1 & 0x00ff);		//ts_command_paramter1 is layer ID
			*p_buffer++ = (BYTE)(ts_command_paramter1 >> 8);
			*p_buffer++ = (BYTE)ts_command_paramter2;					//ts_command_paramter2 is enable / disable

			//----- START TX -----
			ts_start_tx(p_buffer, 150);			//Layer enable takes 15mS (measured)

			break;
		}
		//------------------------
		//----- PROCESS MODE -----
		//------------------------

		//ts_command_response is the status returned by the screen
		ts_command_response = 0;
		if (ts_rx_payload_length >= 2)
			ts_command_response = ((uint16_t)ts_tx_rx_buffer[7] << 8) + ts_tx_rx_buffer[6];

		ts_do_command = 0x00;		//Flag that command has completed
		ts_current_mode = TS_IDLE;

		break;




	case TS_IMAGE_LOAD:
		//----------------------
		//----------------------
		//----- IMAGE LOAD -----
		//----------------------
		//----------------------
		if (ts_just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			p_buffer = &ts_tx_rx_buffer[0];

			length = ts_get_string_length(&ts_command_name_buffer[0]);
			if (length > TS_COMMAND_NAME_BUFFER_MAX_LENGTH)
				length = TS_COMMAND_NAME_BUFFER_MAX_LENGTH;

			//Command
			*p_buffer++ = (BYTE)(CMD_IMAGE_LOAD & 0x00ff);
			*p_buffer++ = (BYTE)(CMD_IMAGE_LOAD >> 8);

			//Sequence Number
			*p_buffer++ = 0x01;
			*p_buffer++ = 0x00;

			//Payload Length
			*p_buffer++ = 0;			//Will be set by tx function
			*p_buffer++ = 0;

			//THE PAYLOAD
			//Data Length
			*p_buffer++ = (BYTE)(length & 0x00ff);
			*p_buffer++ = (BYTE)(length >> 8);

			//Payload (Image_label)
			for (b_temp = 0; b_temp < length; b_temp++)
				*p_buffer++ = ts_command_name_buffer[b_temp];

			//----- START TX -----
			ts_command_response = 0xffff;
			ts_start_tx(p_buffer, 600);			//Loading a 1024x600 image takes 300mS (measured)

			break;
		}
		//------------------------
		//----- PROCESS MODE -----
		//------------------------

		//ts_command_response is the imageLoadId returned by the screen
		ts_command_response = 0xffff;
		if (ts_rx_payload_length >= 4)
			ts_command_response = ((uint16_t)ts_tx_rx_buffer[9] << 8) + ts_tx_rx_buffer[8];

		ts_do_command = 0x00;		//Flag that command has completed
		ts_current_mode = TS_IDLE;

		ts_delay_before_new_command_allowed_ms_timer = 80;		//We need to wait before we hit the screen with anything else

		break;




	case TS_IMAGE_WRITE:
		//-----------------------
		//-----------------------
		//----- IMAGE WRITE -----
		//-----------------------
		//-----------------------
		if (ts_just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			p_buffer = &ts_tx_rx_buffer[0];

			//Command
			*p_buffer++ = (BYTE)(CMD_IMAGE_WRITE & 0x00ff);
			*p_buffer++ = (BYTE)(CMD_IMAGE_WRITE >> 8);

			//Sequence Number
			*p_buffer++ = 0x01;
			*p_buffer++ = 0x00;

			//Payload Length
			*p_buffer++ = 0;			//Will be set by tx function
			*p_buffer++ = 0;

			//THE PAYLOAD
			*p_buffer++ = (BYTE)(ts_command_paramter1 & 0x00ff);		//ts_command_paramter1 is Layer ID
			*p_buffer++ = (BYTE)(ts_command_paramter1 >> 8);
			*p_buffer++ = (BYTE)(ts_command_paramter2 & 0x00ff);		//ts_command_paramter2 is Image ID
			*p_buffer++ = (BYTE)(ts_command_paramter2 >> 8);
			*p_buffer++ = (BYTE)(ts_command_paramter3 & 0x00ff);		//ts_command_paramter3 is X Position
			*p_buffer++ = (BYTE)(ts_command_paramter3 >> 8);
			*p_buffer++ = (BYTE)(ts_command_paramter4 & 0x00ff);		//ts_command_paramter4 is Y Position
			*p_buffer++ = (BYTE)(ts_command_paramter4 >> 8);

			//----- START TX -----
			ts_command_response = 0xffff;
			ts_start_tx(p_buffer, 500);			//Image write takes 30mS for a 1024x600 image (measured)

			break;
		}
		//------------------------
		//----- PROCESS MODE -----
		//------------------------

		//ts_command_response is the ImageWriteId returned by the screen
		ts_command_response = 0xffff;
		if (ts_rx_payload_length >= 4)
			ts_command_response = ((uint16_t)ts_tx_rx_buffer[9] << 8) + ts_tx_rx_buffer[8];

		ts_do_command = 0x00;		//Flag that command has completed
		ts_current_mode = TS_IDLE;

		break;





	case TS_IMAGE_DELETE:
		//-----------------------
		//-----------------------
		//----- IMAGE DELETE -----
		//-----------------------
		//-----------------------
		if (ts_just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			p_buffer = &ts_tx_rx_buffer[0];

			//Command
			*p_buffer++ = (BYTE)(CMD_IMAGE_DELETE & 0x00ff);
			*p_buffer++ = (BYTE)(CMD_IMAGE_DELETE >> 8);

			//Sequence Number
			*p_buffer++ = 0x01;
			*p_buffer++ = 0x00;

			//Payload Length
			*p_buffer++ = 0;			//Will be set by tx function
			*p_buffer++ = 0;

			//THE PAYLOAD
			*p_buffer++ = (BYTE)(ts_command_paramter1 & 0x00ff);		//ts_command_paramter1 is Image Write ID
			*p_buffer++ = (BYTE)(ts_command_paramter1 >> 8);

			//----- START TX -----
			ts_command_response = 0xffff;
			ts_start_tx(p_buffer, 300);			//Image delete takes 30mS for a 1024x600 image (measured)

			break;
		}
		//------------------------
		//----- PROCESS MODE -----
		//------------------------

		//ts_command_response is the ImageWriteId returned by the screen
		ts_command_response = 0xffff;
		if (ts_rx_payload_length >= 4)
			ts_command_response = ((uint16_t)ts_tx_rx_buffer[9] << 8) + ts_tx_rx_buffer[8];

		ts_do_command = 0x00;		//Flag that command has completed
		ts_current_mode = TS_IDLE;

		break;




	case TS_COMPOSE_LAYERS_TO_A_PAGE:
		//------------------------------------
		//------------------------------------
		//----- COMPOSE LAYERS TO A PAGE -----
		//------------------------------------
		//------------------------------------
		if (ts_just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			p_buffer = &ts_tx_rx_buffer[0];

			//Command
			*p_buffer++ = (BYTE)(CMD_COMPOSE_LAYERS_TO_PAGE & 0x00ff);
			*p_buffer++ = (BYTE)(CMD_COMPOSE_LAYERS_TO_PAGE >> 8);

			//Sequence Number
			*p_buffer++ = 0x01;
			*p_buffer++ = 0x00;

			//Payload Length
			*p_buffer++ = 0;			//Will be set by tx function
			*p_buffer++ = 0;

			//THE PAYLOAD
			*p_buffer++ = (BYTE)(ts_command_paramter1 & 0x00ff);		//ts_command_paramter1 is Page ID
			*p_buffer++ = (BYTE)(ts_command_paramter1 >> 8);

			//----- START TX -----
			ts_command_response = 0xffff;
			ts_start_tx(p_buffer, 150);			//Compose page to screen takes 15mS (measured)

			break;
		}
		//------------------------
		//----- PROCESS MODE -----
		//------------------------

		//ts_command_response is the status returned by the screen
		ts_command_response = 0xffff;
		if (ts_rx_payload_length >= 2)
			ts_command_response = ((uint16_t)ts_tx_rx_buffer[7] << 8) + ts_tx_rx_buffer[6];

		ts_do_command = 0x00;		//Flag that command has completed
		ts_current_mode = TS_IDLE;

		break;





	case TS_PAGE_TO_SCREEN:
		//--------------------------
		//--------------------------
		//----- PAGE TO SCREEN -----
		//--------------------------
		//--------------------------
		if (ts_just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			p_buffer = &ts_tx_rx_buffer[0];

			//Command
			*p_buffer++ = (BYTE)(CMD_PAGE_TO_SCREEN & 0x00ff);
			*p_buffer++ = (BYTE)(CMD_PAGE_TO_SCREEN >> 8);

			//Sequence Number
			*p_buffer++ = 0x01;
			*p_buffer++ = 0x00;

			//Payload Length
			*p_buffer++ = 0;			//Will be set by tx function
			*p_buffer++ = 0;

			//THE PAYLOAD
			*p_buffer++ = (BYTE)(ts_command_paramter1 & 0x00ff);		//ts_command_paramter1 is Page ID
			*p_buffer++ = (BYTE)(ts_command_paramter1 >> 8);

			//----- START TX -----
			ts_command_response = 0xffff;
			ts_start_tx(p_buffer, 150);			//Page to screen takes 5mS for a 1024x600 image (measured)

			break;
		}
		//------------------------
		//----- PROCESS MODE -----
		//------------------------

		//ts_command_response is the status returned by the screen
		ts_command_response = 0xffff;
		if (ts_rx_payload_length >= 2)
			ts_command_response = ((uint16_t)ts_tx_rx_buffer[7] << 8) + ts_tx_rx_buffer[6];

		ts_do_command = 0x00;		//Flag that command has completed
		ts_current_mode = TS_IDLE;

		break;






	case TS_TOUCH_MAP_RECTANGLE:
		//-------------------------------
		//-------------------------------
		//----- TOUCH MAP RECTANGLE -----
		//-------------------------------
		//-------------------------------
		if (ts_just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			p_buffer = &ts_tx_rx_buffer[0];

			length = ts_get_string_length(&ts_command_name_buffer[0]);
			if (length > TS_COMMAND_NAME_BUFFER_MAX_LENGTH)
				length = TS_COMMAND_NAME_BUFFER_MAX_LENGTH;
			
			//Command
			*p_buffer++ = (BYTE)(CMD_TOUCH_MAP_RECTANGLE & 0x00ff);
			*p_buffer++ = (BYTE)(CMD_TOUCH_MAP_RECTANGLE >> 8);

			//Sequence Number
			*p_buffer++ = 0x01;
			*p_buffer++ = 0x00;

			//Payload Length
			*p_buffer++ = 0;			//Will be set by tx function
			*p_buffer++ = 0;

			//THE PAYLOAD
			*p_buffer++ = (BYTE)(ts_command_paramter1 & 0x00ff);		//ts_command_paramter1 is X Position
			*p_buffer++ = (BYTE)(ts_command_paramter1 >> 8);
			*p_buffer++ = (BYTE)(ts_command_paramter2 & 0x00ff);		//ts_command_paramter2 is Y Position
			*p_buffer++ = (BYTE)(ts_command_paramter2 >> 8);
			*p_buffer++ = (BYTE)(ts_command_paramter3 & 0x00ff);		//ts_command_paramter3 is Width
			*p_buffer++ = (BYTE)(ts_command_paramter3 >> 8);
			*p_buffer++ = (BYTE)(ts_command_paramter4 & 0x00ff);		//ts_command_paramter4 is Height
			*p_buffer++ = (BYTE)(ts_command_paramter4 >> 8);

			//Data Length
			*p_buffer++ = (BYTE)(length & 0x00ff);
			*p_buffer++ = (BYTE)(length >> 8);

			//Payload (Image_label)
			for (b_temp = 0; b_temp < length; b_temp++)
				*p_buffer++ = ts_command_name_buffer[b_temp];		
			
			//----- START TX -----
			ts_command_response = 0xffff;
			ts_start_tx(p_buffer, 500);			//Image write takes 30mS for a 1024x600 image (measured)

			break;
		}
		//------------------------
		//----- PROCESS MODE -----
		//------------------------

		//ts_command_response is the TouchMapId returned by the screen
		ts_command_response = 0xffff;
		if (ts_rx_payload_length >= 4)
			ts_command_response = ((uint16_t)ts_tx_rx_buffer[9] << 8) + ts_tx_rx_buffer[8];

		ts_do_command = 0x00;		//Flag that command has completed
		ts_current_mode = TS_IDLE;

		break;




	case TS_FONT_LOAD:
		//---------------------
		//---------------------
		//----- FONT LOAD -----
		//---------------------
		//---------------------
		if (ts_just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			p_buffer = &ts_tx_rx_buffer[0];

			length = ts_get_string_length(&ts_command_name_buffer[0]);
			if (length > TS_COMMAND_NAME_BUFFER_MAX_LENGTH)
				length = TS_COMMAND_NAME_BUFFER_MAX_LENGTH;

			//Command
			*p_buffer++ = (BYTE)(CMD_FONT_LOAD & 0x00ff);
			*p_buffer++ = (BYTE)(CMD_FONT_LOAD >> 8);

			//Sequence Number
			*p_buffer++ = 0x01;
			*p_buffer++ = 0x00;

			//Payload Length
			*p_buffer++ = 0;			//Will be set by tx function
			*p_buffer++ = 0;

			//THE PAYLOAD
			//Data Length
			*p_buffer++ = (BYTE)(length & 0x00ff);
			*p_buffer++ = (BYTE)(length >> 8);

			//Payload (Image_label)
			for (b_temp = 0; b_temp < length; b_temp++)
				*p_buffer++ = ts_command_name_buffer[b_temp];

			//----- START TX -----
			ts_command_response = 0xffff;
			ts_start_tx(p_buffer, 300);			//Loading a example font takes 35mS (measured)

			break;
		}
		//------------------------
		//----- PROCESS MODE -----
		//------------------------

		//ts_command_response is the imageLoadId returned by the screen
		ts_command_response = 0xffff;
		if (ts_rx_payload_length >= 4)
			ts_command_response = ((uint16_t)ts_tx_rx_buffer[9] << 8) + ts_tx_rx_buffer[8];

		ts_do_command = 0x00;		//Flag that command has completed
		ts_current_mode = TS_IDLE;

		ts_delay_before_new_command_allowed_ms_timer = 80;		//We need to wait before we hit the screen with anything else

		break;




	case TS_STRING_WRITE:
		//------------------------
		//------------------------
		//----- STRING WRITE -----
		//------------------------
		//------------------------
		if (ts_just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			p_buffer = &ts_tx_rx_buffer[0];

			length = ts_get_string_length(&ts_command_name_buffer[0]);
			if (length > TS_COMMAND_NAME_BUFFER_MAX_LENGTH)
				length = TS_COMMAND_NAME_BUFFER_MAX_LENGTH;

			//Command
			*p_buffer++ = (BYTE)(CMD_STRING_WRITE & 0x00ff);
			*p_buffer++ = (BYTE)(CMD_STRING_WRITE >> 8);

			//Sequence Number
			*p_buffer++ = 0x01;
			*p_buffer++ = 0x00;

			//Payload Length
			*p_buffer++ = 0;			//Will be set by tx function
			*p_buffer++ = 0;

			//THE PAYLOAD
			*p_buffer++ = (BYTE)(ts_command_paramter1 & 0x00ff);					//ts_command_paramter1 is Layer ID
			*p_buffer++ = (BYTE)(ts_command_paramter1 >> 8);
			*p_buffer++ = (BYTE)(ts_command_paramter2 & 0x00ff);					//ts_command_paramter2 is X Position
			*p_buffer++ = (BYTE)(ts_command_paramter2 >> 8);
			*p_buffer++ = (BYTE)(ts_command_paramter3 & 0x00ff);					//ts_command_paramter3 is Y Position
			*p_buffer++ = (BYTE)(ts_command_paramter3 >> 8);
			*p_buffer++ = (BYTE)(ts_command_paramter4 & 0x00ff);					//ts_command_paramter4 is Font ID
			*p_buffer++ = (BYTE)(ts_command_paramter4 >> 8);
			*p_buffer++ = (BYTE)((ts_command_paramter_32bit_1 & 0xff000000) >> 24);	//ts_command_paramter_32bit_1 is: Red | Green | Blue | Alpha
			*p_buffer++ = (BYTE)((ts_command_paramter_32bit_1 & 0x00ff0000) >> 16);
			*p_buffer++ = (BYTE)((ts_command_paramter_32bit_1 & 0x0000ff00) >> 8);
			*p_buffer++ = (BYTE)(ts_command_paramter_32bit_1 & 0x000000ff);
			*p_buffer++ = (BYTE)(ts_command_paramter5 & 0x00ff);					//ts_command_paramter5 is H Direction

			//Data Length
			*p_buffer++ = (BYTE)(length & 0x00ff);
			*p_buffer++ = (BYTE)(length >> 8);

			//String to write
			for (b_temp = 0; b_temp < length; b_temp++)
				*p_buffer++ = ts_command_name_buffer[b_temp];

			//----- START TX -----
			ts_command_response = 0xffff;
			ts_start_tx(p_buffer, 300);			//Loading a example font takes 35mS (measured)

			break;
		}
		//------------------------
		//----- PROCESS MODE -----
		//------------------------

		//ts_command_response is the WriteStringId returned by the screen
		ts_command_response = 0xffff;
		if (ts_rx_payload_length >= 4)
			ts_command_response = ((uint16_t)ts_tx_rx_buffer[9] << 8) + ts_tx_rx_buffer[8];

		ts_do_command = 0x00;		//Flag that command has completed
		ts_current_mode = TS_IDLE;

		ts_delay_before_new_command_allowed_ms_timer = 80;		//We need to wait before we hit the screen with anything else

		break;




	case TS_STRING_REPLACE:
		//--------------------------
		//--------------------------
		//----- STRING REPLACE -----
		//--------------------------
		//--------------------------
		if (ts_just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			p_buffer = &ts_tx_rx_buffer[0];

			length = ts_get_string_length(&ts_command_name_buffer[0]);
			if (length > TS_COMMAND_NAME_BUFFER_MAX_LENGTH)
				length = TS_COMMAND_NAME_BUFFER_MAX_LENGTH;

			//Command
			*p_buffer++ = (BYTE)(CMD_STRING_REPLACE & 0x00ff);
			*p_buffer++ = (BYTE)(CMD_STRING_REPLACE >> 8);

			//Sequence Number
			*p_buffer++ = 0x01;
			*p_buffer++ = 0x00;

			//Payload Length
			*p_buffer++ = 0;			//Will be set by tx function
			*p_buffer++ = 0;

			//THE PAYLOAD
			*p_buffer++ = (BYTE)(ts_command_paramter1 & 0x00ff);					//ts_command_paramter1 is String Write ID
			*p_buffer++ = (BYTE)(ts_command_paramter1 >> 8);

			//Data Length
			*p_buffer++ = (BYTE)(length & 0x00ff);
			*p_buffer++ = (BYTE)(length >> 8);

			//String to write
			for (b_temp = 0; b_temp < length; b_temp++)
				*p_buffer++ = ts_command_name_buffer[b_temp];

			//----- START TX -----
			ts_command_response = 0xffff;
			ts_start_tx(p_buffer, 300);			//Loading a example font takes 35mS (measured)

			break;
		}
		//------------------------
		//----- PROCESS MODE -----
		//------------------------

		//ts_command_response is the status returned by the screen
		ts_command_response = 0xffff;
		if (ts_rx_payload_length >= 2)
			ts_command_response = ((uint16_t)ts_tx_rx_buffer[9] << 8) + ts_tx_rx_buffer[8];

		ts_do_command = 0x00;		//Flag that command has completed
		ts_current_mode = TS_IDLE;

		ts_delay_before_new_command_allowed_ms_timer = 80;		//We need to wait before we hit the screen with anything else

		break;
		




	case TS_STRING_DELETE:
		//-------------------------
		//-------------------------
		//----- STRING DELETE -----
		//-------------------------
		//-------------------------
		if (ts_just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			p_buffer = &ts_tx_rx_buffer[0];

			length = ts_get_string_length(&ts_command_name_buffer[0]);
			if (length > TS_COMMAND_NAME_BUFFER_MAX_LENGTH)
				length = TS_COMMAND_NAME_BUFFER_MAX_LENGTH;

			//Command
			*p_buffer++ = (BYTE)(CMD_STRING_DELETE & 0x00ff);
			*p_buffer++ = (BYTE)(CMD_STRING_DELETE >> 8);

			//Sequence Number
			*p_buffer++ = 0x01;
			*p_buffer++ = 0x00;

			//Payload Length
			*p_buffer++ = 0;			//Will be set by tx function
			*p_buffer++ = 0;

			//THE PAYLOAD
			*p_buffer++ = (BYTE)(ts_command_paramter1 & 0x00ff);					//ts_command_paramter1 is String Write ID
			*p_buffer++ = (BYTE)(ts_command_paramter1 >> 8);

			//----- START TX -----
			ts_command_response = 0xffff;
			ts_start_tx(p_buffer, 300);			//Loading a example font takes 35mS (measured)

			break;
		}
		//------------------------
		//----- PROCESS MODE -----
		//------------------------

		//ts_command_response is the status returned by the screen
		ts_command_response = 0xffff;
		if (ts_rx_payload_length >= 2)
			ts_command_response = ((uint16_t)ts_tx_rx_buffer[9] << 8) + ts_tx_rx_buffer[8];

		ts_do_command = 0x00;		//Flag that command has completed
		ts_current_mode = TS_IDLE;

		ts_delay_before_new_command_allowed_ms_timer = 80;		//We need to wait before we hit the screen with anything else

		break;






	case TS_GET_EVENT_MESSAGE:
		//-----------------------------
		//-----------------------------
		//----- GET EVENT MESSAGE -----
		//-----------------------------
		//-----------------------------
		if (ts_just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			p_buffer = &ts_tx_rx_buffer[0];

			//Command
			*p_buffer++ = (BYTE)(CMD_EVENT_MESSAGE & 0x00ff);
			*p_buffer++ = (BYTE)(CMD_EVENT_MESSAGE >> 8);

			//Sequence Number
			*p_buffer++ = 0x01;
			*p_buffer++ = 0x00;

			//Payload Length
			*p_buffer++ = 0;			//Will be set by tx function
			*p_buffer++ = 0;

			//THE PAYLOAD

			//----- START TX -----
			ts_command_response = 0xffff;
			ts_start_tx(p_buffer, 150);			//Page to screen takes 5mS for a 1024x600 image (measured)

			break;
		}
		//------------------------
		//----- PROCESS MODE -----
		//------------------------

		//ts_command_response is the status returned by the screen
		ts_command_response = 0;
		if (ts_rx_payload_length >= 8)
		{
			ts_command_response = 0xffff;

			//RX Packet format
			//	Command L
			//	Command H
			//	Sequence L
			//	Sequence H
			//	Payload Length L
			//	Payload Length H
			//	Unused L
			//	Unused H
			//	Size of event data L
			//	Size of event data H
			//	No of events L
			//	No of events H
			//	Unknown L
			//	Unknown H

			//	For each event:
			//		Length L
			//		Length H
			//		Type (1=touch)
			//		DataLength L
			//		DataLength H
			//		Gesture ID
			//		No of touch points
			//		Touchpoint X L
			//		Touchpoint X H
			//		Touchpoint Y L
			//		Touchpoint Y H
			//		Label length
			//		Label[]


			if (
				(ts_tx_rx_buffer[0] == (BYTE)(CMD_EVENT_MESSAGE & 0x00ff)) &&
				(ts_tx_rx_buffer[1] == (BYTE)(CMD_EVENT_MESSAGE >> 8))
			)
			{
				//	Command L
				//	Command H
				//	Sequence L
				//	Sequence H
				//	Payload Length L
				//	Payload Length H
				//	Unused L
				//	Unused H
				//	Size of event data L
				//	Size of event data H
				//	No of events L
				//	No of events H
				//	Unknown L
				//	Unknown H
				p_buffer = &ts_tx_rx_buffer[8];


				//Size of event data
				event_data_length = *p_buffer++;
				event_data_length |= ((uint16_t)(*p_buffer++) << 8);

				if (ts_rx_payload_length >= (event_data_length + 4))
				{
					//No of events
					no_of_events = *p_buffer++;
					no_of_events |= ((uint16_t)(*p_buffer++) << 8);
					//Unknown
					p_buffer++;
					p_buffer++;
					//while (no_of_events--)
					if (no_of_events)
					{
						//--------------------------
						//----- GET NEXT EVENT -----
						//--------------------------
						//	For each event:
						//		Length L
						//		Length H
						//		Type (1=touch)
						//		DataLength L
						//		DataLength H
						//		Gesture ID
						//		No of touch points
						//		Touchpoint X L
						//		Touchpoint X H
						//		Touchpoint Y L
						//		Touchpoint Y H
						//		Label length
						//		Label[]

						//Length
						event_length = *p_buffer++;
						event_length |= ((uint16_t)(*p_buffer++) << 8);

						//Type
						event_type = *p_buffer++;
						if (event_type != 1)
							Nop();

						//DataLength
						length = *p_buffer++;
						length |= ((uint16_t)(*p_buffer++) << 8);

						p_buffer += length;

						//Label length
						length = *p_buffer++;
						length |= ((uint16_t)(*p_buffer++) << 8);

						//Label[]
						for (count = 0; count < length; count++)
						{
							if (p_buffer >=  & ts_tx_rx_buffer[TS_BUFFER_LENGTH])
								break;

							if (count < TS_TOUCH_LABEL_BUFFER_MAX_LENGTH)
								ts_touch_label_buffer[count] = *p_buffer;
							p_buffer++;
							touch_event_received = 1;
						}
						if (count < TS_TOUCH_LABEL_BUFFER_MAX_LENGTH)
							ts_touch_label_buffer[count] = 0x00;
						





					} //while (no_of_events--)
				} //if (ts_rx_payload_length >= (event_data_length + 8))




				if (power_up_waiting_for_screen)
				{
					power_up_waiting_for_screen = 0;			//Flag that we have had comms with screen so it is ready
					ts_delay_before_new_command_allowed_ms_timer = 80;		//We need to wait before we hit the screen with anything else
				}
			}
		}

		ts_get_event_messges_1ms_timer = 50;		//<<<Get touch event messages every #mS

		//ts_do_command = 0x00;		//Flag that command has completed
		ts_current_mode = TS_IDLE;

		break;








		

















	case TS_IDLE:
		//-------------------------------
		//-------------------------------
		//----- IDLE - DEFAULT MODE -----
		//-------------------------------
		//-------------------------------
		if (ts_just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----


			if (ts_get_event_messges_1ms_timer == 0)	//Don't do immediately in case there's a new command about to be sent by the application
				ts_get_event_messges_1ms_timer = 2;

		}
		//------------------------
		//----- PROCESS MODE -----
		//------------------------

		if (ts_delay_before_new_command_allowed_ms_timer)
			break;

		if (power_up_waiting_for_screen)
		{
			//----- WE ARE WAITING FOR SCREEN TO BE READY AFTER FIRST POWER UP -----
			ts_current_mode = TS_GET_EVENT_MESSAGE;
			break;
		}

		//----- CHECK FOR A PENDING COMMAND TO BE STARTED -----
		switch (ts_do_command)
		{
		case TS_IMAGE_LOAD:
			ts_current_mode = TS_IMAGE_LOAD;
			break;
		case TS_LAYER_ENABLE:
			ts_current_mode = TS_LAYER_ENABLE;
			break;
		case TS_IMAGE_WRITE:
			ts_current_mode = TS_IMAGE_WRITE;
			break;
		case TS_IMAGE_DELETE:
			ts_current_mode = TS_IMAGE_DELETE;
			break;
		case TS_COMPOSE_LAYERS_TO_A_PAGE:
			ts_current_mode = TS_COMPOSE_LAYERS_TO_A_PAGE;
			break;
		case TS_PAGE_TO_SCREEN:
			ts_current_mode = TS_PAGE_TO_SCREEN;
			break;
		case TS_TOUCH_MAP_RECTANGLE:
			ts_current_mode = TS_TOUCH_MAP_RECTANGLE;
			break;
		case TS_FONT_LOAD:
			ts_current_mode = TS_FONT_LOAD;
			break;
		case TS_STRING_WRITE:
			ts_current_mode = TS_STRING_WRITE;
			break;
		case TS_STRING_REPLACE:
			ts_current_mode = TS_STRING_REPLACE;
			break;
		case TS_STRING_DELETE:
			ts_current_mode = TS_STRING_DELETE;
			break;

			
		default:
			//Nothing pending - periodically go and get touch event messages
			if (ts_get_event_messges_1ms_timer == 0)
			{
				ts_current_mode = TS_GET_EVENT_MESSAGE;
			}
			break;
		} //switch (ts_do_command)


		break;





//<<<<< ADD OTHER MODES HERE



	} //switch (ts_current_mode)



}






//******************************
//******************************
//********** START TX **********
//******************************
//******************************
void ts_start_tx (BYTE* p_buffer, uint16_t timeout_time_ms)
{
	uint16_t count;

	ts_tx_no_of_bytes_to_tx = (BYTE)(p_buffer - &ts_tx_rx_buffer[0]);		//no of bytes to tx (excluding checksum), byte 0 will be sent with bit 9 high

	//Set the payload length
	ts_tx_rx_buffer[4] = (BYTE)((ts_tx_no_of_bytes_to_tx - 6) & 0x00ff);
	ts_tx_rx_buffer[5] = (BYTE)((ts_tx_no_of_bytes_to_tx - 6) >> 8);

	ts_comms_no_of_tries = 0;
	ts_rx_1ms_timeout_time = timeout_time_ms;

	for (count = 0; count < TS_BUFFER_LENGTH; count++)						//Copy the tx packet in case we need to re-try
		ts_tx_buffer_copy[count] = ts_tx_rx_buffer[count];

	ts_tx_no_of_bytes_to_tx_copy = ts_tx_no_of_bytes_to_tx;

	ts_start_tx_setup_done();
}

void ts_start_tx_setup_done (void)
{
	ts_rx_no_of_bytes_to_rx = 0xfffe;
	ts_rx_1ms_timeout_timer = 0xffff;
	ts_tx_byte = 0;

	//TX the first byte
	DISABLE_INT;
	ts_tx_rx_chksum = ts_tx_rx_buffer[0];
	TS_TX_REG = (WORD)ts_tx_rx_buffer[0];
	ts_tx_byte++;

	ts_transfer_in_progress = 1;
	TS_ENABLE_TX_IRQ = 1;
	ENABLE_INT;
}




//*****************************************
//*****************************************
//********** TX BUFFER EMPTY IRQ **********
//*****************************************
//*****************************************
void __attribute__((__interrupt__,__auto_psv__)) _U2TXInterrupt(void)
{
	TS_TX_IRQ_FLAG = 0;				//Reset irq flag


	if (ts_tx_byte < ts_tx_no_of_bytes_to_tx)
	{
		//----- SEND NEXT DATA BYTE -----
		ts_tx_rx_chksum += ts_tx_rx_buffer[ts_tx_byte];			//Add to checksum
		
		TS_TX_REG = (WORD)ts_tx_rx_buffer[ts_tx_byte];
		ts_tx_byte++;
	}
	else if (ts_tx_byte == ts_tx_no_of_bytes_to_tx)
	{	
		//----- SEND CHECKSUM LOW -----
		TS_TX_REG = (WORD)(ts_tx_rx_chksum & 0x00ff);
		ts_tx_byte++;
	}
	else if (ts_tx_byte == (ts_tx_no_of_bytes_to_tx + 1))
	{	
		//----- SEND CHECKSUM HIGH -----
		TS_TX_REG = (WORD)(ts_tx_rx_chksum >> 8);
		ts_tx_byte++;
	}
	else
	{
		//----- ALL SENT (TX SHIFT REGISTER EMPTY) -----
		
		TS_ENABLE_TX_IRQ = 0;						//disable tx irq

		ts_tx_byte = 0;					//Reset tx byte counter
		ts_rx_byte = 0;
		ts_tx_rx_chksum = 0;
		ts_rx_1ms_timeout_timer = ts_rx_1ms_timeout_time;
		ts_rx_failed = 0;
	}

}






//****************************************
//****************************************
//********** RX BUFFER FULL IRQ **********
//****************************************
//****************************************
void __attribute__((__interrupt__,__auto_psv__)) _U2RXInterrupt(void)
{
	static BYTE rx_data;
	static uint16_t rx_checksum;
	static uint16_t w_temp;

	TS_RX_IRQ_FLAG = 0;				//Reset irq flag


	if ((TS_STATUS_REG.FERR == 0) && (TS_STATUS_REG.PERR == 0) && (TS_STATUS_REG.OERR == 0) && (ts_rx_no_of_bytes_to_rx != 0xffff))
	{
		//--------------------
		//----- RX IS OK -----
		//--------------------
		rx_data = TS_RX_REG;
		
		if (TS_ENABLE_TX_IRQ)
		{
			//We're not receiving right no so ignore
		}
		else if (ts_rx_byte < ts_rx_no_of_bytes_to_rx)
		{
			//----- DATA BYTE -----
			if (ts_rx_byte < TS_BUFFER_LENGTH)
				ts_tx_rx_buffer[ts_rx_byte] = rx_data;

			ts_tx_rx_chksum += rx_data;

			//RX Packet format
			//	Command L
			//	Command H
			//	Sequence L
			//	Sequence H
			//	Payload Length L
			//	Payload Length H
			//	Status L (always part of the response)
			//	Status H


			if (ts_rx_byte == 5)
			{
				//We have the payload length
				ts_rx_payload_length = ((int)ts_tx_rx_buffer[5] << 8) + ts_tx_rx_buffer[4];
				ts_rx_no_of_bytes_to_rx = ts_rx_payload_length + 6;
			}
			ts_rx_byte++;
		}
		else if (ts_rx_byte == ts_rx_no_of_bytes_to_rx)
		{
			//----- CHECKSUM LOW -----
			rx_checksum = rx_data;
			ts_rx_byte++;
		}
		else if (ts_rx_byte == (ts_rx_no_of_bytes_to_rx + 1))
		{
			//----- CHECKSUM HIGH -----
			rx_checksum |= ((uint16_t)rx_data << 8);
			ts_rx_byte++;

			if (rx_checksum == ts_tx_rx_chksum)
			{
				//CHECKSUM IS GOOD
				ts_rx_no_of_bytes_to_rx = 0;		//Clear reg to flag rx is done
			}
			else
			{
				//CHECKSUM IS BAD
				ts_rx_no_of_bytes_to_rx = 0xffff;	//Flag that rx failed and rx is done
			}
		}
	}
	else
	{
		//--------------------
		//----- RX ERROR -----
		//--------------------
		rx_data = TS_RX_REG;
		rx_data = TS_RX_REG;
		rx_data = TS_RX_REG;
		rx_data = TS_RX_REG;
		
		//ts_STATUS_REG.OERR = 0;		//Can't read write UxSTA due to silicon bug
		w_temp = U2STA;
		w_temp &= 0xfffd;
		w_temp |= 0x2000;					//This bit gets cleared by a read in this device so need to set it again
		U2STA = w_temp;

		ts_rx_failed = 1;
	}
}



//***************************************
//***************************************
//********** GET STRING LENGTH **********
//***************************************
//***************************************
BYTE ts_get_string_length (char *string)
{
	BYTE count = 0;

	while (*string++ != 0x00)
		count++;
	return(count);
}




