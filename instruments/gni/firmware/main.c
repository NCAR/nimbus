/*------------------------------------------------------------------------------
MAIN.C:  GNI main task.

Mike Spowart
NCAR/RAF
------------------------------------------------------------------------------*/
/*
0 - Reset.  Move all motors and switches to home position.

1 - Status.  Return status of 18 switches and 5 motors.

2 - Return count of cassettes.

3 - Move to slide number N (e.g. "3 17").

4 - Load slide, but do not expose.

5 - Expose slide, load it first if it is not already loaded.

6 - End exposure, retract slide.

7 - Control Exposure.  Just expose and extract slide with no timed exposure.
*/

#include <stdio.h>
#include <string.h>
#include "sio.h"
#include "REG952.H"
//#include "ISD51.H"

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------
#define TRUE 	1
#define FALSE	0

#define CMD       (char)0x43	 //"C"
#define STEP      (char)0x73	 //"s"
#define LF        (char)0x0A     //"\n"
#define BS        (char)0x08     //"\b"
#define SPACE     (char)0x20     //" "
#define SBUF S0BUF

#define slide_act_pickup   0x01  // I2C addr. = 010
#define slide_act_release  0x02
#define slide_act_home     0x04
#define stage_home         0x08
#define stage_pos_1        0x10
#define stage_pos_2        0x20
#define stage_pos_3        0x40
#define stage_pos_4        0x80

#define shut_home          0x01  // I2C addr. = 011
#define shut_cont_slide    0x02
#define shut_exp_slide     0x04
#define slide_pickup_home  0x08
#define slide_pickup_cas   0x20   //0x10 rev. 4
#define slide_pickup_shut  0x40   //0x20 rev. 4
#define slide_presence     0x10   //0x40 rev. 4  // reflective

#define car_home           0x01  // I2C addr. = 001 ; reflective
#define car_cas_counter    0x02
#define car_index_home     0x04
#define man_over_ride      0x08
#define man_carousel_fwd   0x40
#define man_carousel_back  0x80

#define man_slide_act_fwd   0x01 // I2C addr. = 000
#define man_slide_act_back  0x02
#define man_stage_fwd       0x04
#define man_stage_back      0x08
#define man_slide_pkup_fwd  0x10
#define man_slide_pkup_back 0x20
#define man_shuttle_fwd     0x40
#define man_shuttle_back    0x80

//Motor numbers
#define carousel         0
#define slide_ret_act    1
#define linear_stage     2
#define slide_pickup     3
#define shuttle          4 
// I2C port addresses 

#define I2C1               0x75
#define I2C2               0x77
#define I2C3               0x73
#define I2C4               0x71

#define FORWARD            1
#define BACKWARD          -1

extern data unsigned char t_in;
extern data unsigned char t_out;
unsigned char  idata serial_char[10];
unsigned char  idata drive[5][8];
//unsigned char  data active_motor;
unsigned char  data val;
unsigned char  data mul;
unsigned char  data cmd_num;
unsigned char  data slide_num;
unsigned char  data count[5];
unsigned char  data flash_data;
unsigned char  data i;
unsigned char  data j;
unsigned char  data k;
char data jj;
unsigned short kk;
static char  data direction;
static unsigned char  code *codeptr;
unsigned char  data stop_timer;
unsigned char  data nsteps;
unsigned char  data switches[4];
unsigned char  data cassette_count;
unsigned char  data slide_number;
bit escape;
bit step_wait;
bit set_switch;
bit mode;
bit home_flag;
/*
bit carousel_run;
bit slide_ret_act_run;
bit linear_stage_run;
bit slide_pickup_run;
bit shuttle_run;
*/
/*-----------------------------------------------------------------------------
	Macros
-----------------------------------------------------------------------------*/
#define MSB(word)		(BYTE)(((WORD)(word) >> 8) & 0xff)
#define LSB(word)		(BYTE)((WORD)(word) & 0xff)

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
bit write_flash(void);
void begin_RTC(void);
char putchar (char);
char _getkey (void);
void wait_buf (void);
void home_drives(void);
void status(void);
//void cas_count(void);
//void move_to_slide(char);
void advance_slide(void);   //Cmd #3
void load_slide(void);      //Cmd #4
void expose_slide(void);    //Cmd #5
void retract_slide(void);         //Cmd #6
void quick_expose(void);
void home_shuttle(void);
void home_carousel(void);
void home_slide_pickup(void);
void home_slide_retainer_actuator(void);
void home_linear_stage(void);
void advance_carousel(void);
void actuator_pin_pickup(bit);           
void actuator_slide_pickup(void);
void slide_pickup_to_cassette(void);
void slide_pickup_to_shuttle(void);
void I2C_master_receive(void);
void manual(void);
void man_car_fwd(void);
void man_car_back(void);
void man_slide_ret_fwd(void);
void man_slide_ret_back(void);
void man_linear_stage_fwd(void);
void man_linear_stage_back(void);
void man_slide_pickup_fwd(void);
void man_slide_pickup_back(void);
void man_slide_shuttle_fwd(void);
void man_slide_shuttle_back(void);
void motor_step(char, char, bit, bit);
void motor_drive(char, char, bit, bit, char, char);
void motor_run(char, char, bit, bit, char, char, char, unsigned short);
void read_switches(char);
void loop_times(char);
void inhibits_on(char);
void inhibits_off(char);

//void ISDwait();
/*------------------------------------------------------------------------------
Note that the two function above, _getkey and putchar, replace the library
functions of the same name.  These functions use the interrupt-driven serial
I/O routines in SIO.C.
------------------------------------------------------------------------------*/
//-----------------------------------------------------------------------------
// Code
//-----------------------------------------------------------------------------
// Task dispatcher
void main(void)
{
  // Initialize user device
  carousel_inhibit = 1;
  slide_ret_act_inhibit = 1;
  linear_stage_inhibit = 1;
  slide_pickup_inhibit = 1;
  shuttle_inhibit = 1;
  com_initialize ();
  set_switch = 0;
  codeptr = 0x1FC0;
  for (i = 0; i < 5; i++) {
	  drive[i][0] = 0x04;
	  drive[i][1] = 0x06;
	  drive[i][2] = 0x02;
	  drive[i][3] = 0x0A;
	  drive[i][4] = 0x08;
	  drive[i][5] = 0x09;
	  drive[i][6] = 0x01;
	  drive[i][7] = 0x05;
/*
	  drive[i][0] = 0x05;
	  drive[i][1] = 0x01;
	  drive[i][2] = 0x09;
	  drive[i][3] = 0x08;
	  drive[i][4] = 0x0A;
	  drive[i][5] = 0x02;
	  drive[i][6] = 0x06;
	  drive[i][7] = 0x04;
*/
  }
  home_flag = 1;
//  ISDwait();
  for (j = 0; j < 5; j++) {
    count[j] = 0;
    flash_data = *codeptr++;
    for (i = 0; i < 8; i++)
	  {
	    if (flash_data == drive[j][i]) 
		  {
		    count[j] = i;
			  break;
		  }
	  }	  
  }
// Delay here.
  RTCH = 0x16;    // 10 Hz
  RTCL = 0x7F;
  step_wait = 0;
  begin_RTC();
  printf ("GNI controller ready!\r\n");
  wait_buf();
  while(TRUE) {
    if(step_wait) break;
  }
  I2C_master_receive();  // Task Dispatcher

  while(TRUE){					// Main Loop

    if(set_switch) {
      set_switch = 0;
      I2C_master_receive();
    }
    k = 0;
	mode = 0;
    while((switches[2] & man_over_ride) != FALSE) {
      if(k == 0) {
        printf("GNI In Manual Mode\r\n");
        wait_buf();
        k = 1;
        mode = 0;
      }
      manual();
    }

    if(mode == 0) {
      printf("\r\n");
      wait_buf();
      printf ("GNI In Program Mode\r\n");
      wait_buf();
      mode = 1;
    }
    i = 0;
//    if(escape) {
//      printf("ESC found\r\n");
//      wait_buf();
//    }
  	serial_char[0] = 0x00;
  	serial_char[1] = 0x00;
  	serial_char[2] = 0x00;
  	serial_char[3] = 0x00;
    printf("\r\n");
    wait_buf();
	printf ("Please issue a command:\r\n");
    wait_buf();
	printf ("0 = Status\r\n");
    wait_buf();
    printf ("1 = Home Drives\r\n");
    wait_buf();
    printf ("2 = Advance Carousel\r\n");
    wait_buf();
    printf ("3 = Advance Slide\r\n");
    wait_buf();
    printf ("4 = Load Slide\r\n");
    wait_buf();
    printf ("5 = Expose Slide\r\n");
    wait_buf();
    printf ("6 = Retract Slide\r\n");
    wait_buf();
    printf ("7 = Advance, Load, Expose & Retract\r\n");
    wait_buf();
    printf ("8 = Retract Slide Pickup\r\n");
    wait_buf();
    printf ("9 = Home Shuttle\r\n");
    wait_buf();
    printf ("10 = Home Slide Actuator\r\n");
    wait_buf();

    while(TRUE) {
      serial_char[i] = getchar();
      if(mode == 0) break;
      if(serial_char[i] == BS){
        if(i != 0) i -=1;
      }
      else {
        if(serial_char[i] == SPACE){
  	      val = 0;
          mul = 1;
          cmd_num = 0;
	        for (jj = i-1; jj >= 0; jj--) {
	          val = (serial_char[jj] - 0x30) * mul;
            cmd_num += val;
  	        mul *= 10;
          }  
//        j = i;  
        }

		if(serial_char[i] == LF) {
  	      val = 0;
          mul = 1;
          slide_num = 0;
	      for (jj = i-1; jj >= 0; jj--) {
	        val = (serial_char[jj] - 0x30) * mul;
            slide_num += val;
  	        mul *= 10;
          }    
        }
      }
      if(serial_char[i] == LF) break;
      i += 1;
	}
    
// Switch Case here to check for command

    switch (val) {
      case 0:             
        status();    
        break;
      case 1:
        home_drives();
        break;
      case 2:
        advance_carousel();
        break;
      case 3:            
//        move_to_slide(slide_num);
        advance_slide();
        break;
      case 4:             
        load_slide();
        break;
      case 5:             
        expose_slide();
        break;
      case 6:
        retract_slide();
// Check for slide presence here. If no slide print error.
        read_switches(1);
        if((switches[1] & slide_presence) == TRUE) {
          printf("Slide retracted \r\n");                                
          wait_buf();               
        }
        else {
          printf("No slide detected! \r\n");
          wait_buf();   
          while((switches[1] & slide_presence) == FALSE) {
	        load_slide();
            expose_slide();
            retract_slide();
            read_switches(1);
        }
        home_slide_pickup();
        printf("Slide detected \r\n");                                
        wait_buf();                          
		    }
        break;
      case 7:
        advance_slide();
// Check for slide presence here. If no slide print error.
        read_switches(1);
        if((switches[1] & slide_presence) == FALSE) {
          printf("Slide Not Detected! \r\n");                                
          wait_buf(); 
          break;					
        }
		load_slide();
        expose_slide();
		retract_slide();
// Check for slide presence here. If no slide print error.
        read_switches(1);
        if((switches[1] & slide_presence) == TRUE) {
          printf("Slide retracted \r\n");                                
          wait_buf();               
        }
        else {
          printf("No slide detected! \r\n");
          wait_buf();   
          while((switches[1] & slide_presence) == FALSE) {
	      load_slide();
          expose_slide();
          retract_slide();
          read_switches(1);
        }
        home_slide_pickup();
        printf("Slide detected \r\n");                                
        wait_buf();                          
		}
        break;
      case 8:
        slide_pickup_to_cassette();
        break;
      case 9:
        home_shuttle();
        break;
//     case 10:
//        home_shuttle();
//        load_slide();
//        slide_pickup_to_cassette();
//        retract_slide();
//        break;
     case 10:
        home_slide_retainer_actuator();
        break;
      default:
      break;
    }

	EWDRT = 0;
    RTCCON = 0x60; //clear flag
//	  printf("Seconds = %u\r\n",(unsigned int)seconds);
//    wait_buf();
	t_in = 0;
	t_out = 0;
  }
}

//***************************************************************************
//* Reset all drive motors to home positions.
//***************************************************************************
void home_drives(void)
{
  printf("Home drives!\r\n");
  wait_buf();
  home_shuttle();
  printf("Shuttle home.\r\n");
  wait_buf();               
  home_slide_pickup();
  printf("Slide pickup home.\r\n");
  wait_buf(); 
//  motor_run(5, 1, 1, 1, 0, slide_act_pickup,0,0);
  home_linear_stage();
  printf("Linear Stage home.\r\n");
  wait_buf();             
  home_slide_retainer_actuator();
  printf("Slide retainer actuator home.\r\n");
  wait_buf();               
  home_carousel();
  printf("Carousel home.\r\n");
  wait_buf();               
  printf("GNI reset complete.\r\n");
  wait_buf();
               
}
//***************************************************************************
//* Return status.
//***************************************************************************
void status(void)
{
  printf("Status = %x %x %x\r\n",(unsigned int)switches[0],
         (unsigned int)switches[1],(unsigned int)switches[2]);
  wait_buf();
  printf("Cassette = %x \r\n", (unsigned char)cassette_count);
  wait_buf();               
  printf("Slide = %x \r\n", (unsigned char)slide_number);
  wait_buf();               
}
//***************************************************************************
//* Advance to next slide.
//***************************************************************************
void advance_slide(void)
{
  unsigned short ll, limit, limit1, limit2;         
  
  printf("Advancing to next Slide... \r\n");
  wait_buf();
  actuator_pin_pickup(0);
  i = count[2];
// If stage is in home position then go to position #1
  read_switches(0);
  
  limit = 20; 
  EA = 0;
//  IEN2 = 0x04;
  if((switches[0] & stage_home) != FALSE)
  {
    direction = BACKWARD;
    while((switches[0] & stage_pos_1)== FALSE) {
      P4 = (((drive[2][i] << 4) & 0xF0) | (P4 & 0x0F));               
      if(i == 0) i = 7;
      else i = i - 1;
      loop_times(limit);
      read_switches(0);
      if(escape) {
//        escape = 1;
        return;
      }
    }

    P4 = (((drive[2][i] << 4) & 0xF0) | (P4 & 0x0F));               
    loop_times(limit);
    read_switches(0);
    limit1 = 1;  //9 for gears
    limit2 = 100; //0
 
    count[2] = i;
    i = count[0];
    for(ll=0;ll<40;ll++) {
      motor_step(5, 0, 0, 0);
      loop_times(150);
    }

    if((i == 1) || (i == 3) || (i == 5) || (i == 7)) {
      motor_step(5, 0, 0, 0);
    }   
    count[0] = i;
    i = count[2];
    slide_number = 1;
  }
 // Else advance to the next slide.
  else if(switches[0] & stage_pos_1){
    direction = FORWARD;
    while((switches[0] & stage_pos_2) == FALSE) {
      P4 = (((drive[2][i] << 4) & 0xF0) | (P4 & 0x0F));                 
      i += 1;
      if (i > 7)  i = 0;
//      if (limit < 55) limit = 55;
      loop_times(limit);
      read_switches(0);
      if(escape) return;
      limit1 = 2; //11
      limit2 = 20; //80
    }
    slide_number = 2;
  }

  else if(switches[0] & stage_pos_2){
    direction = FORWARD;
    while((switches[0] & stage_pos_3) == FALSE) {
      P4 = (((drive[2][i] << 4) & 0xF0) | (P4 & 0x0F));                 
      i += 1;
      if (i > 7)  i = 0;
      loop_times(limit);
      read_switches(0);
      if(escape) return;
      limit1 = 2;  //14
      limit2 = 125;  //0
    }
    slide_number = 3;
  }

  else if(switches[0] & stage_pos_3){
    direction = FORWARD;
    while((switches[0] & stage_pos_4) == FALSE) {
      P4 = (((drive[2][i] << 4) & 0xF0) | (P4 & 0x0F));                 
      i += 1;
      if (i > 7)  i = 0;
//      limit -= 1;
//      if (limit < 55) limit = 55;
      loop_times(limit);
      read_switches(0);
      if(escape) return;
      limit1 = 2; //17
      limit2 = 255;  //0
    }
    count[2] = i;
    i = count[0];
    for(ll=0;ll<40;ll++) {
    motor_step(5, 0, 1, 1);
    loop_times(50);
  }
  if((i == 1) || (i == 3) || (i == 5) || (i == 7)) {
    motor_step(5, 0, 1, 0);
  }   
  count[0] = i;
  i = count[2];              
  slide_number = 4;
  }

  else if(switches[0] & stage_pos_4){
    direction = BACKWARD;
    while((switches[0] & stage_pos_1) == FALSE) {
      P4 = (((drive[2][i] << 4) & 0xF0) | (P4 & 0x0F));               
      if(i == 0) i = 7;
      else i = i - 1;
//      limit -= 1;
//      if (limit < 55) limit = 55;
      loop_times(limit);
      read_switches(0);
      if(escape) return;
      limit1 = 1;  //1
      limit2 = 0;  //0
    }
    slide_number = 1;                                   
  }

  else {
    printf("Slide Position Lost\r\n");
    wait_buf(); 
    limit1 = 0;
  }
  for(j = 0; j < limit1; j++){                                                
    for(ll = 0; ll < 250; ll++) {
      P4 = (((drive[2][i] << 4) & 0xF0) | (P4 & 0x0F));               
      loop_times(50);
      if(direction == BACKWARD) {
        if(i == 0) i = 7;
        else i = i - 1;
      }
      if(direction == FORWARD) {
        i += 1;
        if (i > 7)  i = 0;
      }
    }
  }
  for(ll = 0; ll < limit2; ll++) {
    P4 = (((drive[2][i] << 4) & 0xF0) | (P4 & 0x0F));               
    loop_times(50);
    if(direction == BACKWARD) {
      if(i == 0) i = 7;
      else i = i - 1;
     }
    if(direction == FORWARD) {
      i += 1;
      if (i > 7)  i = 0;
    }
  }
  if((i == 1) || (i == 3) || (i == 5) || (i == 7)) {
    P4 = (((drive[2][i] << 4) & 0xF0) | (P4 & 0x0F));               
    if(direction == BACKWARD) {
      if ( i== 0) i = 7;
      else i -= 1;
    }
    if(direction == FORWARD) {
      i += 1;
      if (i > 7)  i = 0;
    }
  }
  count[2] = i;
  if(set_switch) set_switch = 0;
//  IEN2 = 0x06;
  EA = 1;
  while(write_flash());
  printf("Advanced to slide #%u\r\n",(unsigned int)slide_number);
  wait_buf(); 
}
//***************************************************************************
//* Doop delay.
//***************************************************************************
void loop_times(char loop_count)
{
  unsigned char nnn, ll;
/*
  nnn = 0;
  for(ll = 0; ll < loop_count; ll++) {
    for(nnn = 0; nnn < 10; nnn++) {
      ll += 1;
      ll -= 1;
    }
*/

  nnn = 0;
  for(ll = 0; ll < loop_count; ll++) {
    if(nnn == 0) inhibits_on(2);
    else {
      inhibits_off(2);
    }
    nnn += 1;
    if(nnn == 3) nnn = 0;
  }

}
//***************************************************************************
//* Load slide.
//***************************************************************************
void load_slide(void)
{
  printf("Loading Slide \r\n");
  wait_buf();
  slide_pickup_to_cassette();
  if(escape) return;
  printf("Slide Pickup to cassette \r\n");
  wait_buf();                                                               
// Delay here.
  RTCH = 0x16;    // 10 Hz
  RTCL = 0x7F;
  step_wait = 0;
  begin_RTC();
  while(TRUE) {
    if(step_wait) break;
  }
  actuator_slide_pickup();
  printf("Actuator slide pickup \r\n");
  wait_buf();              
  if(escape) return;
  slide_pickup_to_shuttle();
  printf("Slide pickup to shuttle \r\n");
  wait_buf();               
}
//***************************************************************************
//* Expose slide.
//***************************************************************************
void expose_slide(void)
{
  printf("Exposing Slide... \r\n");
  wait_buf();
  motor_run(0, 4, 0, 0, 1, shut_exp_slide,0,50); //150
  printf("Slide exposed. \r\n");
  wait_buf();
  slide_pickup_to_cassette();
               
}
//***************************************************************************
//* Retract slide.
//***************************************************************************
void retract_slide(void)
{
  printf("Retracting...... \r\n");
  wait_buf();
  home_shuttle();
 // return;
    RTCH = 0x16;    // 1 Hz
    RTCL = 0x7F;
    step_wait = 0;
    begin_RTC();
    while(TRUE) {
      if(step_wait) break;
  }
  if(escape) return;
  slide_pickup_to_shuttle();
  if(escape) return;
  slide_pickup_to_cassette();              
  if(escape) return;
  actuator_pin_pickup(0);
  
}
//***************************************************************************
//* Carousel to home position.
//***************************************************************************
void home_carousel(void)
{
// Rewind carousel motor (forward) to the home position.
  read_switches(2);
  if((switches[2] & car_home) == FALSE) {
    motor_run(5, 0, 0, 0, 2, car_home,0,30); //Started at 55
  }
  cassette_count = 1;
  printf("Cassette # %u\r\n", (unsigned int)cassette_count);
  wait_buf();
}
//***************************************************************************
//* Slide pickup to home position.
//***************************************************************************
void home_slide_pickup(void)
{
// Rewind slide pickup motor (forwards) to the home position.
  read_switches(1);
  if((switches[1] & slide_pickup_home) == FALSE) {
    motor_run(4, 3, 1, 1, 1, slide_pickup_home,0,42);
  }
}
//***************************************************************************
//* Slide retainer actuator to home position.
//***************************************************************************
void home_slide_retainer_actuator(void)
{
// Rewind slide retainer actuator motor (forward) to the home position.
  
  read_switches(0);
  if((switches[0] & slide_act_home) == FALSE) {
    motor_run(5, 1, 1, 1, 0, slide_act_home,0,0);
  }
}
//***************************************************************************
//* Linear stage to home position.
//***************************************************************************
void home_linear_stage(void)
{
// The home position is in the middle between stage 2 and stage 3.
  unsigned short ll;
  unsigned char limit;

  EA = 0;
  limit = 20;
  i = count[2];
  nsteps = 0;
  direction = FORWARD;
  read_switches(0);
  while((switches[0] & stage_home) == FALSE) {
    home_flag = 0;
    P4 = (((drive[2][i] << 4) & 0xF0) | (P4 & 0x0F));               
    loop_times(limit);
    read_switches(0);
    if(escape) return;

    if((switches[0] & stage_pos_1) != FALSE) {
      slide_number = 1;
      direction = FORWARD;
      nsteps = 0;
    }
    else if((switches[0] & stage_pos_2) != FALSE) {
      slide_number = 2;
      direction = FORWARD;
      nsteps = 0;
    }
    else if((switches[0] & stage_pos_3) != FALSE) {
      slide_number = 3;
      direction = BACKWARD;
      nsteps = 0;
    }
    else if((switches[0] & stage_pos_4) != FALSE) {
      slide_number = 4;
      direction = BACKWARD;
      nsteps = 0;
    }
    if(direction == BACKWARD) {
      if ( i== 0) i = 7;
      else i -= 1;
    }
    if(direction == FORWARD) {
      i += 1;
      if (i > 7)  i = 0;
    }
  }
  if(direction == FORWARD)limit = 8;  //15 for gear motors
  else limit = 5;
  if(home_flag == 0) {
    for(j = 0; j < limit; j++){
      for(ll = 0; ll < 65; ll++) {
        P4 = (((drive[2][i] << 4) & 0xF0) | (P4 & 0x0F)); 
        loop_times(50);
        if(escape) return;
        if(direction == BACKWARD) {
          if ( i== 0) i = 7;
          else i -= 1;
        }
        if(direction == FORWARD) {
          i += 1;
          if (i > 7)  i = 0;
        }
      }
    }
  }
  home_flag = 1;
  if((i == 1) || (i == 3) || (i == 5) || (i == 7)) {
    P4 = (((drive[2][i] << 4) & 0xF0) | (P4 & 0x0F));               
    if(direction == BACKWARD) {
      if ( i== 0) i = 7;
      else i -= 1;
    }
    if(direction == FORWARD) {
     i += 1;
     if (i > 7)  i = 0;
    }
  }
  count[2] = i;
  if(set_switch) set_switch = 0;
  while(write_flash());
  slide_number = 0;
  EA = 1;
}                                   
//***************************************************************************
//* Shuttle to home position.
//***************************************************************************
void home_shuttle(void)
{
// Rewind shuttle motor (backwards) to the home position.
  read_switches(1);
  if (( switches[1] & shut_exp_slide) != FALSE){   //!= False
    motor_run(0, 4, 1, 0, 1, shut_home,0,35);  //2,157
//    read_switches(1);
//    while((switches[1] & shut_home) != FALSE) {
//      motor_run(0, 4, 1, 0, 1, 0, 0, 1);  //2,157
//      read_switches(1);
//    }
//    motor_run(0, 4, 1, 0, 1, 0, 0, 10);  //2,157
  }
  
  else { 
    motor_run(0, 4, 0, 0, 1, 0, 1, 0); //4,0
    motor_run(0, 4, 1, 0, 1, shut_home,0,35);  //2,157

  }
}
//***************************************************************************
//* Read switches from I2C addresses.
//*************************************************************************
void I2C_master_receive(void)
{
  unsigned short i;
// Delay here.
  RTCH = 0x16;    // 10 Hz
  RTCL = 0x7F;
  step_wait = 0;                                     
  begin_RTC(); 
  while(TRUE) {
    if(step_wait) break;
  }
  I2CON = 0x64;   //Send start bit
  while(TRUE) {
    if(I2STAT == 0x08) break; 
  }
  I2DAT = I2C1;
  I2CON = 0x44;  //Send address byte
  while(TRUE) {
    if(I2STAT == 0x40) break;
  }
  I2CON = 0x40;  //Release bus, no ACK
  while(TRUE) {
    if(I2STAT == 0x58) break;
  }
  switches[0] = I2DAT;
  I2CON = 0x54;  //Generate STOP bit
  for(i = 0; i <= 2; i++){
    I2CON = 0x54;
  }
  
  I2CON = 0x64;   //Send start bit
  while(TRUE) {
    if(I2STAT == 0x08) break; 
  }
  I2DAT = I2C2;
  I2CON = 0x44;  //Send address byte
  while(TRUE) {
    if(I2STAT == 0x40) break;
  }
  I2CON = 0x40;  //Release bus, no ACK
  while(TRUE) {
    if(I2STAT == 0x58) break;
  }
  switches[1] = I2DAT;
  I2CON = 0x54;  //Generate STOP bit

  for(i = 0; i <= 2; i++){
    I2CON = 0x54;
  }

  I2CON = 0x64;   //Send start bit
  while(TRUE) {
    if(I2STAT == 0x08) break; 
  }
  I2DAT = I2C3;
  I2CON = 0x44;  //Send address byte
  while(TRUE) {
    if(I2STAT == 0x40) break;
  }
  I2CON = 0x40;  //Release bus, no ACK
  while(TRUE) {
    if(I2STAT == 0x58) break;
  }
  switches[2] = I2DAT;
  I2CON = 0x54;  //Generate STOP bit
  for(i = 0; i <= 2; i++){
    I2CON = 0x54;
  }

  I2CON = 0x64;   //Send start bit
  while(TRUE) {
    if(I2STAT == 0x08) break; 
  }
  I2DAT = I2C4;
  I2CON = 0x44;  //Send address byte
  while(TRUE) {
    if(I2STAT == 0x40) break;
  }
  I2CON = 0x40;  //Release bus, no ACK
  while(TRUE) {
    if(I2STAT == 0x58) break;
  }
  switches[3] = I2DAT;
  I2CON = 0x54;  //Generate STOP bit
}
//***************************************************************************
//* Advance carousel to next cassette position (45 deg).
//***************************************************************************
void advance_carousel(void)
{
//  unsigned short ll;
 // Advance 900 steps.
  home_shuttle();
  printf("Shuttle home.\r\n");
  wait_buf(); 
  home_slide_pickup();
  printf("Slide pickup home.\r\n");
  wait_buf();
  home_linear_stage();
  printf("Linear Stage home.\r\n");
  wait_buf(); 
  home_slide_retainer_actuator();
  printf("Slide retainer actuator home.\r\n");
  wait_buf();  
  motor_run(5, 0, 0, 0, 0, 0,1,9);
  motor_run(5, 0, 0, 0, 2, 2,0,1);
  cassette_count = cassette_count +1; 
  if (cassette_count == 9) {
    cassette_count = 1;
    printf("\r\nYou have completed 1 revolution");
    printf(" of the carousel!\r\n\r\n");
    wait_buf();
  }
  printf("Carousel turned 45 Degrees to Cassette # %u\r\n",
        (unsigned int)cassette_count);
  wait_buf();
}
//***************************************************************************
//* Advance to actuator pin pickup.
//***************************************************************************
void actuator_pin_pickup(bit dir)
{
  read_switches(0);
  if((switches[0] & slide_act_home) != FALSE) {
    motor_run(5, 1, dir, 1, 0, slide_act_pickup,0,30);
  }

// Move from slide pickup position (backwards ~600 steps).
  else {
    motor_run(5, 1, dir, 1, 0, slide_act_pickup,0,30);
  }
}

//***************************************************************************
//* Advance to Actuator Slide Pickup.
//***************************************************************************
void actuator_slide_pickup(void)
{
 // Move from pin pickup postion to slide release position (600 steps).

  read_switches(0);
  if((switches[0] & slide_act_pickup) != FALSE) {
    motor_run(5, 1, 1, 1, 0, slide_act_release,8,0);
  }
}

//***************************************************************************
//* Slide pickup to cassette position.
//***************************************************************************
void slide_pickup_to_cassette(void)
{
// Turn the slide pickup motor to the cassette position.
  read_switches(1);
  if((switches[1] & slide_pickup_home) != FALSE) {
//    direction = BACKWARD;
    motor_run(4, 3, 0, 1, 1, slide_pickup_cas,0,20);
  }
  else {
//    direction = FORWARD;
    if((switches[1] & slide_pickup_cas) != FALSE)
      return;
    else
     motor_run(4, 3, 1, 1, 1, slide_pickup_cas,0,35); //1,120 for geared
  }
}
//***************************************************************************
//* Slide pickup to shuttle position.
//***************************************************************************
void slide_pickup_to_shuttle(void)
{
// Turn the slide pickup motor to the shuttle position.
  read_switches(1);
  if((switches[1] & slide_pickup_shut) == FALSE) {
    motor_run(4, 3, 0, 1, 1, slide_pickup_shut,0,60); //0,100 geared
  }
}
//***************************************************************************
//* Manual motor controls
//***************************************************************************
void manual(void)
{
 // Check to determine which motor/direction is being slected
//  switch (switches[2]) {   
	   
    if((switches[2] & man_carousel_fwd) == 0){
      printf("Carousel forward\r\n");
      wait_buf();
      man_car_fwd();
    }

    if((switches[2] & man_carousel_back) == 0) {
      printf("Carousel backward\r\n");
      wait_buf();
      man_car_back();
    }

    if((switches[3] & man_slide_act_fwd) == 0){
      printf("Slide Actuator forward\r\n");
      wait_buf();
      man_slide_ret_fwd();
    }

    if((switches[3] & man_slide_act_back) == 0) {
      printf("Slide Actuator backward\r\n");
      wait_buf();
      man_slide_ret_back();
    }

    if((switches[3] & man_stage_fwd) == 0){
      printf("Linear Stage forward\r\n");
      wait_buf();
      man_linear_stage_fwd();
    }

    if((switches[3] & man_stage_back) == 0) {
      printf("Linear Stage backward\r\n");
      wait_buf();
      man_linear_stage_back();
    }

    if((switches[3] & man_slide_pkup_fwd) == 0){
      printf("Slide Pickup forward\r\n");
      wait_buf();
      man_slide_pickup_fwd();
    }

    if((switches[3] & man_slide_pkup_back) == 0) {
      printf("Slide Pickup backward\r\n");
      wait_buf();
      man_slide_pickup_back();
    }

    if((switches[3] & man_shuttle_fwd) == 0){
      printf("Slide Shuttle forward\r\n");
      wait_buf();
      man_slide_shuttle_fwd();
    }

    if((switches[3] & man_shuttle_back) == 0) {
      printf("Slide Shuttle backward\r\n");
      wait_buf();
      man_slide_shuttle_back();
    }
    I2C_master_receive();

}

//***************************************************************************
//* Manual motor control
//***************************************************************************
void man_car_back(void)
{
 // Backwards 20 steps per degree.
  motor_drive(5, 0, 1, 0, 2, man_carousel_back);
}  
//***************************************************************************
//* Manual motor control
//***************************************************************************
void man_car_fwd(void)
{
 // Backwards 20 steps per degree.
  motor_drive(5, 0, 0, 0, 2, man_carousel_fwd);
}
//***************************************************************************
//* Manual motor control
//***************************************************************************
void man_slide_ret_back(void)
{
 // Backwards 20 steps per step.
  motor_drive(5, 1, 0, 1, 3, man_slide_act_back);
}
//***************************************************************************
//* Manual motor control
//***************************************************************************
void man_slide_ret_fwd(void)
{
 // Forwards 20 steps per step.
  motor_drive(5, 1, 1, 1, 3, man_slide_act_fwd);

}
//***************************************************************************
//* Manual motor control
//***************************************************************************
void man_slide_shuttle_back(void)
{
  motor_drive(0, 4, 0, 0, 3, man_shuttle_back);
}
//***************************************************************************
//* Manual motor control
//***************************************************************************
void man_slide_shuttle_fwd(void)
{
  motor_drive(0, 4, 1, 0, 3, man_shuttle_fwd);
}
//***************************************************************************
//* Manual motor control
//***************************************************************************
void man_slide_pickup_back(void)
{
  motor_drive(4, 3, 0, 1, 3, man_slide_pkup_back);
}
//***************************************************************************
//* Manual motor control
//***************************************************************************
void man_slide_pickup_fwd(void)
{
  motor_drive(4, 3, 1, 1, 3, man_slide_pkup_fwd);
}
//***************************************************************************
//* Manual motor control
//***************************************************************************
void man_linear_stage_back(void)
{
  motor_drive(4, 2, 0, 0, 3, man_stage_back);
}
//***************************************************************************
//* Manual motor control
//***************************************************************************
void man_linear_stage_fwd(void)
{
  motor_drive(4, 2, 1, 0, 3, man_stage_fwd);
}              
//***************************************************************************
//* begin_RTC
//* Input(s) : none.
//* Returns : none.
//* Description : initialization of Real Time Clock
//***************************************************************************

void begin_RTC(void)
{
//  RTCH = 0xE0;   // 1 Hz
//	RTCL = 0xFF;   
//  RTCH = 0x16;    // 10 Hz
//	RTCL = 0x7F;
	RTCCON = 0x62; //clear flag
	EWDRT = 1;
	RTCCON = 0x63; //clear flag
}

void wait_buf(void)
{
	while(t_in != 0) {
	  if(t_in == t_out){
		  t_in = 0;
			t_out = 0;
		}
	}
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
bit write_flash(void)
{
  #define LOAD 0x00
	#define EP 0x68
	unsigned char data Fm_stat;
	bit prog_fail;
//  printf("Write flash\r\n");
//  wait_buf();

  EA = 0;
  FMCON = LOAD;
	FMADRH = 0x1F;
	FMADRL = 0xC0;
  for ( i = 0; i < 5; i++) {
	  FMDATA = drive[i][count[i]];
  }
  FMCON = EP;
	Fm_stat = FMCON;
  prog_fail = 1;
  while (prog_fail) {
	if ((Fm_stat & 0x0F) != 0) prog_fail = 1;		// write failed.
	else prog_fail = 0;
  }
  EA = 1;
	return (prog_fail);
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/

char _getkey (void)
{
  do
  {
    k = com_getchar ();
    if((switches[2] & man_over_ride) != FALSE) mode = 0;
    if(mode == 0) break;
  }
  while (k == -1);
  return ((unsigned char) k);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
char putchar (char c)
{
//  volatile unsigned int data i;

  while (com_putchar (c) != 0)
  {
    for (i=0; i<1000; i++)
    {
    //DO NOTHING 
    }
  }
  return (c);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void inhibits_on (char motor_num)
{
  switch (motor_num) {

    case 0:
      P5 = (P5 & 0xF0) | 0x01;
      P4 = 0x11;
      P0 = (P0 & 0x0F) | 0x10;
      break;
    case 1:
      P5 = (P5 & 0x0F) | 0x10;
      P4 = 0x11;
      P0 = (P0 & 0x0F) | 0x10;
      break;
    case 2:
      P5 = 0x11;
      P4 = (P4 & 0xF0) | 0x01;
      P0 = (P0 & 0x0F) | 0x10;
      break;
    case 3:
      P5 = 0x11;
      P4 = (P4 & 0x0F) | 0x10;
      P0 = (P0 & 0x0F) | 0x10;
      break;
    case 4:
      P5 = 0x11;
      P4 = 0x11;
      break;
/*
    case 5:
      P5 = 0x11;
      P4 = 0x11;
      P0 = P0 | 0x10;
      break;
*/
    default:
      break;
  }
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void inhibits_off (char motor_num)
{
  switch (motor_num) {

    case 0:
      P5 = P5 & 0xF0;
      P4 = 0x00;
      P0 = P0 & 0x0F;
      break;
    case 1:
      P5 = P5 & 0x0F;
      P4 = 0x00;
      P0 = P0 & 0x0F;
      break;
    case 2:
      P5 = 0x00;
      P4 = P4 & 0xF0;
      P0 = P0 & 0x0F;
      break;
    case 3:
      P5 = 0x00;
      P4 = P4 & 0x0F;
      P0 = P0 & 0x0F;
      break;
    case 4:
      P5 = 0x00;
      P4 = 0x00;
      break;
/*
    case 5:
      P5 = 0x00;
      P4 = 0x00;
      P0 = P0 & 0x0F;
      break;
*/
    default:
      break;
  }


}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// portnum = microprocessor port 0, 4 or 5.  
// motor = motor number 0,1,2,3,4. 
// dir = 1 for forward rotation dir = 0 for backward rotation. 
// hilow = 0,1. 1 is for lower half of associated microprocessor port.
// switchnum = 0,1,2,3,4 and is equal to one of the I2C chips. 
// switchbit = hex(1,2,4,8,10,20,40,80) corresponding to one of the I2C chip pins.
// first is outer for-loop upper-limit number of first loop. 
// second = last for-loop upper-limit number.
void motor_run(char portnum, char motor, bit dir, bit hilow, char switchnum,
                 char switchbit, char first, unsigned short second)
{
  unsigned short nnn, ll, limit;

  EA = 0;
//  active_motor = motor;
  i = count[motor];
//  limit = 300;
  switch (motor) {   
    case 0:         //Carousel
    case 1:         //Actuator
      limit = 20;
      break;
    case 2:         //Linear Stage
      limit = 20;
      break;
    case 3:         //Slide Pickup
      limit = 30;
      break;
    case 4:         //Shuttle motor
      limit = 30;
      break;
    default:                                     
      limit = 50;
      break;
  }
  nnn = 0;
  read_switches(switchnum);
  if(switchbit != 0x0) {
    while((switches[switchnum] & switchbit)== FALSE) {
//      if(limit < 20) limit = 20; 
      motor_step(portnum, motor, dir, hilow);
      for(kk = 0; kk < limit; kk++) {
        if(nnn == 0) inhibits_on(motor);
        else inhibits_off(motor);
        nnn += 1;
        if(nnn == 3) nnn = 0;
      }
      read_switches(switchnum);
      if(escape) return;
    } 
  }

  if(first > 0) {
//    limit = 150;
    for(j = 0; j < first; j++){
      for(ll = 0; ll < 256; ll++) {
        motor_step(portnum, motor, dir, hilow);
//        limit -= 1;
//        if (limit < 110) limit = 110;
//        loop_times(limit);

        nnn = 0;
        for(kk = 0; kk < limit; kk++) {
          if(nnn == 0) inhibits_on(motor);
          else {
            inhibits_off(motor);
          }
          nnn += 1;
          if(nnn == 3) nnn = 0;
        }

      }
    }
  }
  if(second > 0) {
    for(ll = 0; ll < second; ll++) {
      motor_step(portnum, motor, dir, hilow);
//      loop_times(limit);

      nnn = 0;
      for(kk = 0; kk < limit; kk++) {
        if(nnn == 0) inhibits_on(motor);
        else {
          inhibits_off(motor);
        }
        nnn += 1;
        if(nnn == 3) nnn = 0;
      }

    }
  }
  if((i == 1) || (i == 3) || (i == 5) || (i == 7)) {
    motor_step(portnum, motor, dir, hilow);
  }   
//  home_flag[motor] = 0;
  count[motor] = i;
  if(set_switch) set_switch = 0;
  while(write_flash());
  inhibits_on(motor);
  EA = 1;
//  active_motor = 5;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void motor_drive(char portnum, char motor, bit dir, bit hilow, char switchnum,
                 char switchbit)
{
  unsigned char nnn;

  EA = 0;
//  active_motor = motor;
  i = count[motor];
  j = 20;

  nnn = 0;
  if((switches[switchnum] & switchbit) == FALSE) {      
    while((P2 & 0x20) == FALSE) {
      motor_step(portnum, motor, dir, hilow);
//      loop_times(j);

      for(kk = 0; kk < j; kk++) {
        if(nnn == 0) inhibits_on(motor);
        else inhibits_off(motor);
        nnn += 1;
        if(nnn == 3) nnn = 0;
      }
      read_switches(switchnum);
    }
  }
  if((i == 1) || (i == 3) || (i == 5) || (i == 7)) {
    motor_step(portnum, motor, dir, hilow);
  }
  count[motor] = i;
  while(write_flash());
  inhibits_on(motor);
  EA = 1;
//  active_motor = 5;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void motor_step(char portnum, char motor, bit dir, bit hilow)
{
  switch (portnum) {
    case 0:
      if(dir) {             //forward
        P0 = (((drive[motor][i++] << 4) & 0xF0) | (P0 & 0x0F)); //                
        if(i > 7) i = 0;  
      }
      else {                // backward
        P0 = (((drive[motor][i] << 4) & 0xF0) | (P0 & 0x0F));  //
        if(i == 0) i = 7;
        else i -= 1;
      }
      break;
    case 4:                          
      if(dir) {             //forward
        if(hilow) {
          P4 = ((drive[motor][i++] & 0x0F) | (P4 & 0xF0));  //
        }
        else {

          P4 = (((drive[motor][i++] << 4) & 0xF0) | (P4 & 0x0F)); //
        }
        if(i > 7) i = 0;  
      }
      else {                // backward
        if(hilow) {
          P4 = ((drive[motor][i] & 0x0F) | (P4 & 0xF0));        //
        }
        else {

          P4 = (((drive[motor][i] << 4) & 0xF0) | (P4 & 0x0F));  //
        }
        if(i == 0) i = 7;
        else i -= 1;
      }
      break;
    case 5:
      if(dir) {             //forward
        if(hilow) {
          P5 = ((drive[motor][i++] & 0x0F) | (P5 & 0xF0));  //linear
        }
        else {
          P5 = (((drive[motor][i++] << 4) & 0xF0) | (P5 & 0x0F)); //slide_pkup                
        }
        if(i > 7) i = 0;  
      }
      else {                // backward
        if(hilow) {
          P5 = ((drive[motor][i] & 0x0F) | (P5 & 0xF0));        //linear
        }
        else {
          P5 = (((drive[motor][i] << 4) & 0xF0) | (P5 & 0x0F));  //slide_pkup              
        }
        if(i == 0) i = 7;
        else i -= 1;
      }
      break;
      default:
      break;
  }
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void read_switches(char switch_num)
{
  unsigned char ic;
  switch (switch_num) { 
    case 0:
      ic = 0x75;
      break;
    case 1:
      ic = 0x77;
      break;
    case 2:
      ic = 0x73;
      break;
    case 3:
      ic = 0x71;
      break;
    default:
      break;
  }
  I2CON = 0x64;   //Send start bit
  while(TRUE) {
    if(I2STAT == 0x08) break; 
  }
  I2DAT = ic;
  I2CON = 0x44;  //Send address byte
  while(TRUE) {
    if(I2STAT == 0x40) break;
  }
  I2CON = 0x40;  //Release bus, no ACK
  while(TRUE) {
    if(I2STAT == 0x58) break;
  }
  switches[switch_num] = I2DAT;
  I2CON = 0x54;  //Generate STOP bit
  return;
}

