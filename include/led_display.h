/*
 * led_display.h
 *
 *  Version: 2.0.0
 *
 *  Created on: 01.11.2012.
 *      Author: srkos
 */

#ifndef LED_DISPLAY_H_
#define LED_DISPLAY_H_

// API Status Codes Type:
typedef enum E_ERROR_CODES
{
	DL_OK = 0,

	DL_EVENT_BUSY,

	DL_TIMEOUT_ERROR,

	DL_RESOURCE_NOT_INIT,
	DL_RESOURCE_BUSY,

	DL_NULL_POINTER,
	DL_CRC_ERROR,
	DL_BUFFER_OVERFLOW,
	DL_TIMESTAMP_INVALID,
	DL_WRONG_INPUT_ARG,

	DL_CMD_RESPONSE_UNSUCCESS = 20,
	DL_COMMUNICATION_ERROR,
	DL_ERR_CMD_NOT_SENT,
	DL_ERR_ACK_NOT_RX,
	DL_UNIT_ACK_ERR,

	// RF over UART
	DL_RF_CHANNEL_NOT_CHANGED = 30,
	DL_RF_NO_ACK,

	// Hamming
	DL_HAMMING_ERROR = 50,
	DL_HAMMING_ERROR_NO_ACK,
	DL_HAMMING_ERROR_WRONG_ACK,
	DL_HAMMING_ERROR_NO_RESPONSE,
	DL_HAMMING_ERROR_WRONG_RESPONSE,

	// File
	DL_FILE_NOT_FOUND = 90,
	DL_FILE_EMPTY,
	// FTDI
	DL_FTDI_NO_DEVICE = 100,
	DL_FTDI_ERROR,
	DL_FTDI_CLOSE_ERROR,
	DL_FTDI_READ_ERROR,
	DL_FTDI_READ_LESS_DATA, // less data readed from UART
	DL_FTDI_WRITE_ERROR,
	DL_FTDI_WRITE_LESS_DATA,
	// Flashing
	DL_FLASHING_WRONG_ANSWER_1 = 300,
	DL_FLASHING_CMD_NOT_SENT,
	DL_FLASHING_CMD_NO_ANSWER,
	DL_FLASHING_NO_0x03,
	DL_FLASHING_BLOCK_NO_TX,
	DL_FLASHING_BLOCK_NO_ACK,
	DL_FLASHING_PROG_BRAKE,
	DL_FLASHING_WRONG_DATA_ACK,
	DL_FILE_OPEN_ERROR,
	DL_FLASHING_CRC_MISS,
	DL_FLASHING_CRC_WRONG_ACK,
	//---------------------

	DL_ERROR_NO_DATA = 40, // as = "velicina je 0";

	HAMMING_ERROR = 50,

	HAMMING_ERROR_NOT_SENT_CMD1, // as = "nije poslata komanda"
	HAMMING_ERROR_NO_ACK_CMD1, // as = "nema potvrde komande 1";
	HAMMING_ERROR_WRONG_ACK_CMD1, // as = "poogresna potvrda komande 1";
	HAMMING_ERROR_NO_RESPONSE_CMD1, // as = "nema odgovora 1";
	HAMMING_ERROR_WRONG_RESPONSE_CMD1, // as = "pogresan odgovor na komandu 1";

	HAMMING_ERROR_NOT_SENT_CMD2,
	HAMMING_ERROR_NO_ACK_CMD2, // as = "nema potvrde komande 2";
	HAMMING_ERROR_WRONG_ACK_CMD2, // as = "poogresna potvrda komande 2";
	HAMMING_ERROR_NO_RESPONSE_CMD2, // as = "nema odgovora 2";
	HAMMING_ERROR_WRONG_RESPONSE_CMD2, // as = "pogresan odgovor na komandu 2";

//	FLASHING_WRONG_ANSWER_1 = 300,
//	FLASHING_CMD_NOT_SENT,
//	FLASHING_CMD_NO_ANSWER,
	FLASHING_NO_0x03,
	FLASHING_BLOCK_NO_TX, // as = "nije poslat blok " + AnsiString(i);
	FLASHING_BLOCK_NO_ACK, // as = "nema odgovora " + AnsiString(i);
	FLASHING_PROG_BRAKE, // as = "prekid programiranja";
	FLASHING_WRONG_DATA_ACK, // as = "greska dobijeno " + AnsiString(FrameR[0]);
//	FILE_OPEN_ERROR,
	FLASHING_FINISH_NO_ACK, //		as = "nije stigla potvrda upisa";
	FLASHING_FINISH_UNSUCCESSFUL, //		as = "nisu upisani parametri";
	FLASHING_CRC_MISS, //		as = "CRC error";
	FLASHING_CRC_WRONG_ACK, //		as = "Pogresna potvrda";

	DL_LAST_ERROR = 0xFFFFFFFF
} DL_STATUS;

//#############################################################################

#include <stdint.h>
#include <stddef.h>
#include <time.h>

typedef const char * c_string;

//#############################################################################

#ifdef _WIN32
#	ifdef DL_API_STATIC_LIB
#		define DL_API
#	else
#		ifndef DL_API_EXPORTS
#			define DL_API /* __declspec(dllimport) */ __stdcall
#		else
#			define DL_API __declspec(dllexport) __stdcall
#		endif
#	endif
#else // __linux__ || __APPLE__
#	define DL_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif

//#############################################################################

// XXX: only one FTDI device attached to computer
DL_API
DL_STATUS DL_OpenPort(void);

DL_API
DL_STATUS DL_ClosePort(void);

// global reset service / DLL
DL_API
DL_STATUS DL_RestartPort(void);

DL_API
uint32_t DL_GetDLLVersion(void);

DL_API
c_string DL_GetLibraryVersionStr(void);

//#############################################################################

DL_API
DL_STATUS DL_GetDisplayVersion(int display_id, uint32_t * version);

DL_API
DL_STATUS DL_GetDisplaySerial(int display_id, char *strSN, int *intID);

//#############################################################################

DL_API
DL_STATUS DL_DisplayReset(int display_id);

//#############################################################################
// get and set display time
DL_API
DL_STATUS DL_DisplayTimeGet(int display_id, uint32_t *timestamp);

DL_API
DL_STATUS DL_DisplayTimeSet(int display_id, uint32_t timestamp);

//#############################################################################
// prepare data for BUS Display
//-----------------------------------------------------------------------------
// config display
DL_API
DL_STATUS DL_DisplaySendConfig(int display_id, int number_of_panels, int font, int brightness,
		int scroll_start_ms, int scroll_speed_ms, int screen_display_time);

//-----------------------------------------------------------------------------
DL_API
DL_STATUS DL_DisplaySendText(int display_id, char * chDisplayText, int DisplayText_Len);

// additional functions for formating text
/**
 * extra functions for help creation active content (like clock, date, count-down timer) in Text for Display
 *
 * this function will concatenate data about active content to buffer and increase its size
 * if timestamp is NULL Display will show actual time from RTC
 * function modify text_buffer_size with appended data size
 * (first text_buffer_size must be initialized with size of the available text_buffer bytes)
 */
DL_API
DL_STATUS DL_Insert_DateTimeFormated(char *text_buffer, int *text_buffer_size, time_t timestamp,
		int datetime_format);

DL_API
DL_STATUS DL_Insert_FixBeforeAndAlignFollow(char *text_buffer, int *text_buffer_size, int format);

DL_API
DL_STATUS DL_Insert_ArrowUp(char *text_buffer, int *text_buffer_size);

DL_API
DL_STATUS DL_Insert_ArrowDown(char *text_buffer, int *text_buffer_size);

DL_API
DL_STATUS DL_Insert_NewLine(char *text_buffer, int *text_buffer_size);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

DL_API
DL_STATUS DL_DisplaySetDefault(
		int display_id, //
		int number_of_panels, int font, int brightness, int scroll_start_ms, int scroll_speed_ms,
		int screen_display_time, //
		char *strDefaultMsg, int DefaultMsg_Len);

DL_API
DL_STATUS DL_DisplaySetDefaultRgb(
		int display_id, //
		int number_of_panels, int number_of_rows, int font, int brightness, int scroll_start_ms, int scroll_speed_ms,
		int screen_display_time, //
		char *strDefaultMsg, int DefaultMsg_Len, int red, int green, int blue);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//#############################################################################
// prepare data for Small Display
DL_API
DL_STATUS DL_DisplaySmallNumbers(int display_id, char * chLine1Display, char * chLine2Display);

DL_API
DL_STATUS DL_DisplayCommon(int display_id, int number_of_panels, int font, int brightness,
		int scroll_start_ms, int scroll_speed_ms, int screen_display_time, char * chLine1Display,
		int Line1Display_Len);

DL_API
DL_STATUS DL_DisplayCommonRgb(int display_id, int number_of_panels, int number_of_rows, int font, int brightness,
		int scroll_start_ms, int scroll_speed_ms, int screen_display_time, char * chLine1Display,
		int Line1Display_Len, int red, int green, int blue);

//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################
// private
// not documented functions
//#############################################################################
#define CONFIG_TABLE_COLS	14

typedef union U_config_row
{
	int cell[CONFIG_TABLE_COLS];
	struct
	{
		long num;
		long display_id;
		long panel_x;
		long panel_y;
		long pix_x;
		long pix_y;
		long brightness;
		long font;
		long scroll_start;
		long scroll_speed;
		long screen_display_time;
          long rgb[3];
	};
} t_config_row;

typedef union U_VERSION
{
	uint32_t val;
	unsigned short w[2];
	unsigned char v[4];
	struct
	{
		uint32_t major :8;
		uint32_t minor :8;
		uint32_t release :8;
		uint32_t build :8;
	};
	struct
	{
		uint32_t fw_ver :16;
		uint32_t hw_ver :8;
		uint32_t hw_type :8;
	};
} t_version;
//#############################################################################
// undocumented, functions with structures
// send display settings
DL_API
DL_STATUS DL_DisplaySendConfigStruct(int device_id, void * display_config);
// send display data
DL_API
DL_STATUS DL_DisplaySendData(int device_id, void * display_config, char * chDisplayText,
		int DisplayText_Len);
//#############################################################################

//#############################################################################
// FIXME: tempirana bomba za vise displeja na magistrali
DL_API
DL_STATUS DL_DisplayInitSN(int display_id, char * new_sn, int new_id);

//#############################################################################
DL_API
DL_STATUS DL_DisplayFlashFirmwareBOOT_Init(int device_id, void * firmware_data, int firmware_size);

DL_API
DL_STATUS DL_DisplayFlashFirmware_Init(int device_id, void * firmware_data, int firmware_size);

DL_API
DL_STATUS DL_DisplayTransferData_DoEvent(void);

DL_API
int DL_DisplayTransferData_GetPercent(void);
//#############################################################################
DL_API
DL_STATUS DL_DisplayFlashFirmwareFromFile(const char * bin_path_name);

DL_API
DL_STATUS DL_DisplayFlashFirmware(int device_id, void * firmware_data, int firmware_size);
DL_API
DL_STATUS DL_DisplayFlashFirmwareBOOT(int device_id, void * firmware_data, int firmware_size);

DL_API
c_string DL_Debug_ErrorStr(DL_STATUS DL_status);

DL_API
DL_STATUS DL_TestMain(void * in_param);

#ifdef __cplusplus
}
#endif

#endif /* LED_DISPLAY_H_ */
