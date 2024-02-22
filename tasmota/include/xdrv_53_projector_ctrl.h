#ifndef _XDRV53_PROJECTOR_CTRL_H_
#define _XDRV53_PROJECTOR_CTRL_H_

//define RELAY that is forced ON while projector is running
#ifndef PROJECTOR_CTRL_PWR_BY_RELAY
#define PROJECTOR_CTRL_PWR_BY_RELAY 1
#endif //PROJECTOR_CTRL_PWR_BY_RELAY

#ifdef USE_PROJECTOR_CTRL_EPSON
	#define PROJECTOR_CTRL_SERIAL_TIMEOUT 30 //up to 254 seconds
#else
	#define PROJECTOR_CTRL_SERIAL_TIMEOUT 10 //up to 254 seconds
#endif

#ifndef PROJECTOR_CTRL_SERIAL_BAUDRATE
	#define PROJECTOR_CTRL_SERIAL_BAUDRATE 9600
#endif //PROJECTOR_CTRL_SERIAL_BAUDRATE

#ifdef USE_PROJECTOR_CTRL_NEC
/* see the serial codes in
 * https://www.sharpnecdisplays.eu/p/download/v/5e14a015e26cacae3ae64a422f7f8af4/cp/Products/Projectors/Shared/CommandLists/PDF-ExternalControlManual-english.pdf#page=5
 * tested with NEC V300W
 * final cksum byte is appended automatically
 */
#define PROJECTOR_CTRL_LOGNAME	"DLP[NEC]"
static const uint8_t projector_ctrl_msg_qry_typ[] = { 0x00, 0xBF, 0x00, 0x00, 0x01, 0x00 }; //page81
static const uint8_t projector_ctrl_msg_qry_pwr[] = { 0x00, 0xBF, 0x00, 0x00, 0x01, 0x02 }; //page83
static const uint8_t projector_ctrl_msg_qry_lmp[] = {0x03, 0x96, 0x00, 0x00, 0x02, 0x00, 0x01}; // page 34
static const uint8_t projector_ctrl_msg_pwr_on[] = { 0x02, 0x00, 0x00, 0x00, 0x00 };        //page15
static const uint8_t projector_ctrl_msg_pwr_off[] = { 0x02, 0x01, 0x00, 0x00, 0x00 };       //page16
static const struct projector_ctrl_command_info_s projector_ctrl_commands[] = {
	{PROJECTOR_CTRL_S_QRY_TYPE, &projector_ctrl_msg_qry_typ[0], sizeof(projector_ctrl_msg_qry_typ),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 0x20, 22, 8, 4, 0xA0, 8, 5, 2},
	{PROJECTOR_CTRL_S_QRY_PWR,  &projector_ctrl_msg_qry_pwr[0], sizeof(projector_ctrl_msg_qry_pwr),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 0x20, 22, 6, 1, 0xA0, 8, 5, 2},
	{PROJECTOR_CTRL_S_QRY_LAMP,  &projector_ctrl_msg_qry_lmp[0], sizeof(projector_ctrl_msg_qry_lmp),// Expected response: 0x23, 0x96, 0x00, 0x00, 0x06, 0x00, 0x01, 0x50, 0x46, 0x00, 0x00 <Checksum>
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 0x23, 12, 7, 3, 0xA3, 8, 5, 2},		// Error resp.: 0xA3, 0x96, 0x00, 0x00, 0x02, 0x00, 0x00 <Checksum>
	{PROJECTOR_CTRL_S_PWR_ON,   &projector_ctrl_msg_pwr_on[0], sizeof(projector_ctrl_msg_pwr_on),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 0x22, 6, 0, 1, 0xA2, 8, 5, 2},
	{PROJECTOR_CTRL_S_PWR_OFF,  &projector_ctrl_msg_pwr_off[0], sizeof(projector_ctrl_msg_pwr_off),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 0x22, 6, 0, 1, 0xA2, 8, 5, 2}
};
#define PROJECTOR_CTRL_QRYPWR_ON      0x04
#define PROJECTOR_CTRL_QRYPWR_COOLING 0x05
#define PROJECTOR_CTRL_QRYPWR_STARTING 0x01 //undocumented state, seen on V300W
#define PROJECTOR_CTRL_QRYPWR_WARMING 0x02  //undocumented state, seen on V300W


#elif defined(USE_PROJECTOR_CTRL_OPTOMA)
/* see the serial codes in
 * https://www.optoma.co.uk/uploads/manuals/hd36-m-en-gb.pdf#page=56
 * not really tested with OPTOMA devices
 */
#define PROJECTOR_CTRL_LOGNAME	"DLP[OPTOMA]"
static const uint8_t projector_ctrl_msg_qry_typ[] = { 0x7e, 0x30, 0x30, 0x31, 0x32, 0x32, 0x20, 0x31, 0x0d }; //page59
static const uint8_t projector_ctrl_msg_qry_pwr[] = { 0x7e, 0x30, 0x30, 0x31, 0x32, 0x34, 0x20, 0x31, 0x0d }; //page59
static const uint8_t projector_ctrl_msg_qry_lmp[] = {0x7e, 0x30, 0x30, 0x31, 0x30, 0x38, 0x20, 0x32, 0x0d}; // page 59, 0x7E, 0x30, 0x30, 0x31, 0x30, 0x38, 0x20, 0x32, 0x0D (Error in PDF between Lamp Hours and Cumulative Lamp Hours, ASCII differs, HEX Code does not)
static const uint8_t projector_ctrl_msg_pwr_on[] = { 0x7e, 0x30, 0x30, 0x30, 0x30, 0x20, 0x31, 0x0d };        //page56
static const uint8_t projector_ctrl_msg_pwr_off[] = { 0x7e, 0x30, 0x30, 0x30, 0x30, 0x20, 0x30, 0x0d };       //page56
static const struct projector_ctrl_command_info_s projector_ctrl_commands[] = {
	{PROJECTOR_CTRL_S_QRY_TYPE, &projector_ctrl_msg_qry_typ[0], sizeof(projector_ctrl_msg_qry_typ),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 'O', 6, 2, 4, 'I', 5, 4, 1},
	{PROJECTOR_CTRL_S_QRY_PWR,  &projector_ctrl_msg_qry_pwr[0], sizeof(projector_ctrl_msg_qry_pwr),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 'O', 3, 2, 1, 'I', 5, 4, 1},
	{PROJECTOR_CTRL_S_QRY_LAMP,  &projector_ctrl_msg_qry_lmp[0], sizeof(projector_ctrl_msg_qry_lmp),// Expected response: 'O', 'K', 0x00, 0x00, 0x00, 0x06, 0x00
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 'O', 7, 4, 4, 'I', 5, 4, 1},		// Error resp. not found in PDF, expecting same as other queries
	{PROJECTOR_CTRL_S_PWR_ON,   &projector_ctrl_msg_pwr_on[0], sizeof(projector_ctrl_msg_pwr_on),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 'P', 1, 0, 1, 'F', 1, 0, 1},
	{PROJECTOR_CTRL_S_PWR_OFF,  &projector_ctrl_msg_pwr_off[0], sizeof(projector_ctrl_msg_pwr_off),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 'P', 1, 0, 1, 'F', 1, 0, 1}
};
#define PROJECTOR_CTRL_QRYPWR_ON      0x31
#define PROJECTOR_CTRL_QRYPWR_COOLING 0x31 //placebo
#define PROJECTOR_CTRL_QRYPWR_STARTING 0x31 //placebo
#define PROJECTOR_CTRL_QRYPWR_WARMING 0x31 //placebo


#elif defined(USE_PROJECTOR_CTRL_ACER)
/* see the serial codes in
 * http://global-download.acer.com/GDFiles/Document/RS232%20Command%20Table/RS232%20Command%20Table_Acer_1.0_A_A.zip?acerid=636791605984811687
 * tested with ACER P1500
 * every command is first acknowledged by "*000\r" (success) or "*001\r" (failure) followed by an optional response
 * most commands fail in stand-by mode
 */
#define PROJECTOR_CTRL_LOGNAME	"DLP[ACER]"
//static const uint8_t projector_ctrl_msg_qry_typ[] = { '*',' ','0',' ','I','R',' ','0','3','5', 0x0d }; //line50 - responds *000\rModel Model P1500E\r [fails in stand-by mode]
static const uint8_t projector_ctrl_msg_qry_typ[] = { '*',' ','0',' ','L','a','m','p',' ','?', 0x0d }; //cannot query type, so query Lamp status instead
static const uint8_t projector_ctrl_msg_qry_pwr[] = { '*',' ','0',' ','L','a','m','p',' ','?', 0x0d }; //line97 - responds *000\rLamp 0\r
static const uint8_t projector_ctrl_msg_qry_lmp[] = { '*',' ','0',' ','L','a','m','p', 0x0d}; //line105 - responds XXXX\r
static const uint8_t projector_ctrl_msg_pwr_on[]  = { '*',' ','0',' ','I','R',' ','0','0','1', 0x0d }; //line18 - responds *000\r
static const uint8_t projector_ctrl_msg_pwr_off[] = { '*',' ','0',' ','I','R',' ','0','0','2', 0x0d }; //line19 - responds *000\r
static const struct projector_ctrl_command_info_s projector_ctrl_commands[] = {
	{PROJECTOR_CTRL_S_QRY_TYPE, &projector_ctrl_msg_qry_typ[0], sizeof(projector_ctrl_msg_qry_typ),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, '*', 12, 10, 1, '?', 1, 0, 1},
	{PROJECTOR_CTRL_S_QRY_PWR,  &projector_ctrl_msg_qry_pwr[0], sizeof(projector_ctrl_msg_qry_pwr),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, '*', 12, 10, 1, '?', 1, 0, 1},
	{PROJECTOR_CTRL_S_QRY_LAMP,  &projector_ctrl_msg_qry_lmp[0], sizeof(projector_ctrl_msg_qry_lmp),// Expected response: XXXX\r
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 0x30, 5, 0, 4, '?', 1, 0, 1},		// Only working with 0-999 lamp hours
	{PROJECTOR_CTRL_S_PWR_ON,   &projector_ctrl_msg_pwr_on[0], sizeof(projector_ctrl_msg_pwr_on),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, '*', 5, 0, 1, '?', 1, 0, 1},
	{PROJECTOR_CTRL_S_PWR_OFF,  &projector_ctrl_msg_pwr_off[0], sizeof(projector_ctrl_msg_pwr_off),
								PROJECTOR_CTRL_SERIAL_TIMEOUT, '*', 5, 0, 1, '?', 1, 0, 1}
};
#define PROJECTOR_CTRL_QRYPWR_ON      '1'
#define PROJECTOR_CTRL_QRYPWR_COOLING '1'  //placebo
#define PROJECTOR_CTRL_QRYPWR_STARTING '1' //placebo
#define PROJECTOR_CTRL_QRYPWR_WARMING '1'  //placebo

#elif defined(USE_PROJECTOR_CTRL_EPSON)
/* see the serial codes in ESC/VP21 Command User’s Guide for Home Projectors
 * https://files.support.epson.com/pdf/pltw1_/pltw1_cm.pdf
 * and ESC/VP21 Command User's Guide for Business Projectors
 * https://download.epson-europe.com/pub/download/3222/epson322269eu.pdf
 */
#define PROJECTOR_CTRL_LOGNAME	"LCD[EPSON]"
static const uint8_t projector_ctrl_msg_qry_typ[] = { 0x0d }; // <CR>
static const uint8_t projector_ctrl_msg_qry_pwr[] = { 'P', 'W', 'R', '?', 0x0d }; // PWR? 
static const uint8_t projector_ctrl_msg_qry_lmp[] = { 'L', 'A', 'M', 'P', '?', 0x0d }; // LAMP? 
static const uint8_t projector_ctrl_msg_pwr_on[] = { 'P', 'W', 'R', ' ', 'O', 'N', 0x0d }; // PWR ON
static const uint8_t projector_ctrl_msg_pwr_off[] = { 'P', 'W', 'R', ' ', 'O', 'F', 'F', 0x0d }; //PWR OFF
static const struct projector_ctrl_command_info_s projector_ctrl_commands[] = {
	{PROJECTOR_CTRL_S_QRY_TYPE, &projector_ctrl_msg_qry_typ[0], sizeof(projector_ctrl_msg_qry_typ), // Expected response: ':'
								PROJECTOR_CTRL_SERIAL_TIMEOUT, ':', 1, 0, 1, 'E', 5, 0, 0}, 		// Error resp.: 'E','R','R',0x0D,':'
	{PROJECTOR_CTRL_S_QRY_PWR,  &projector_ctrl_msg_qry_pwr[0], sizeof(projector_ctrl_msg_qry_pwr), // Expected response: 'P','W','R','=','0','0',0x0D,':'
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 'P', 8, 4, 1, 'E', 5, 0, 0}, 		// Error resp.: 'E','R','R',0x0D,':'
	{PROJECTOR_CTRL_S_QRY_LAMP,  &projector_ctrl_msg_qry_lmp[0], sizeof(projector_ctrl_msg_qry_lmp),// Expected response: 'L','A','M','P','=','0','0','0','0','0',0x0D,':'
								PROJECTOR_CTRL_SERIAL_TIMEOUT, 'L', 11, 5, 4, 'E', 5, 0, 0},		// Error resp.: 'E','R','R',0x0D,':'
	{PROJECTOR_CTRL_S_PWR_ON,   &projector_ctrl_msg_pwr_on[0], sizeof(projector_ctrl_msg_pwr_on),	// Expected response: ':'
								PROJECTOR_CTRL_SERIAL_TIMEOUT, ':', 1, 0, 1, 'E', 5, 0, 0}, 		// Error resp.: 'E','R','R',0x0D,':'
	{PROJECTOR_CTRL_S_PWR_OFF,  &projector_ctrl_msg_pwr_off[0], sizeof(projector_ctrl_msg_pwr_off), // Expected response: ':'
								PROJECTOR_CTRL_SERIAL_TIMEOUT, ':', 1, 0, 1, 'E', 5, 0, 0}			// Error resp.: 'E','R','R',0x0D,':'
};
#define PROJECTOR_CTRL_QRYPWR_ON      0x3031
#define PROJECTOR_CTRL_QRYPWR_COOLING 0x3033 //placebo
#define PROJECTOR_CTRL_QRYPWR_STARTING 0x3032
#define PROJECTOR_CTRL_QRYPWR_WARMING 0x3034 //placebo

#else
#error USE_PROJECTOR_CTRL: No projector type defined
#endif


#endif //_XDRV53_PROJECTOR_CTRL_H_