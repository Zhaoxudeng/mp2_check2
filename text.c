/*
 * tab:4
 *
 * text.c - font data and text to graphics conversion utility
 *
 * "Copyright (c) 2004-2009 by Steven S. Lumetta."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:        Steve Lumetta
 * Version:        2
 * Creation Date:   Thu Sep  9 22:06:29 2004
 * Filename:        text.c
 * History:
 *    SL    1    Thu Sep  9 22:06:29 2004
 *        First written.
 *    SL    2    Sat Sep 12 13:45:33 2009
 *        Integrated original release back into main code base.
 */

#include <string.h>

#include "text.h"
#include "modex.h"

/* 
 * These font data were read out of video memory during text mode and
 * saved here.  They could be read in the same manner at the start of a
 * game, but keeping a copy allows us to run the game to fix text mode
 * if it is broken (font data missing, usually).
 *
 * Each character is 8x16 pixels and occupies two lines in the table below.
 * Each byte represents a single bitmapped line of a single character.
 */
#define BAR_SIZE  18*80*4
#define PLANE_SIZE 18*80
#define X_SIZE    80*4
#define X_WIDTH   80
/*
 * text_to_graphic
 *   DESCRIPTION: colors the status bar area with user specified color 
 *                converts a string of characters into font_data and stores
 *                the values into a buffer for printing to the screen
 *   INPUTS: s -- the string that needs to be transformed into font_data and printed to
 *                 the screen
 *   OUTPUTS: buf -- the buffer which contains the ascii image of the character
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void text_to_graphic(char* s, int level){
    unsigned char buf[BAR_SIZE];
    int size=strlen(s);
    int i,x,y;
    i=0;
//calculate the offset for centering the text in the screen
    int off_set=(X_SIZE-size*FONT_WIDTH)/2;

//make the entire status bar pink
    while(i<BAR_SIZE){
        buf[i]=13*level;
        i++;
    }
    //loop through all characters
    for(i = 0; i<size; i++){
       //find the corresponding font_data
      unsigned char* temp=font_data[(int)s[i]];
        for( x = 0 ; x < FONT_WIDTH; x++){

            for(y = 1; y < FONT_HEIGHT+1; y++){
                //find which pixel is valid for printing
                if((temp[y-1] >> (7-x))&1)
                //updates the buffer and print the pixel black
                    buf[(x%4)*PLANE_SIZE + (x+off_set+i*8)/4 + y*X_WIDTH] = 0+5*level;
            }
        }
    }
    //pass the function to print it to the status bar
    show_status(buf);
    
}
void text_to_graphic_f(char* s, int w, int h, unsigned char** floor){
   
    int size=strlen(s);
    unsigned char buf[12][12*size];
    int i,j,x,y;
    i=0;
    j=0;
//calculate the offset for centering the text in the screen


//make the entire status bar pink
    for(i=0;i<12;i++){
        for(j=0;j<12*size;j++){
            buf[i][j]=10;
        }
    }
    //loop through all characters
    for(i = 0; i<size; i++){
       //find the corresponding font_data
      unsigned char* temp=font_data[(int)s[i]];

        for( x = 0 ; x < 8; x++){
            
            for(y = 0; y < 12; y++){
                //find which pixel is valid for printing
                if((temp[y+2] >> (7-x))&1)
                //updates the buffer and print the pixel black
                    buf[y][x+i*12] = (*(*(floor+i)+y*12+x))/2;
                else    
                    buf[y][x+i*12] = 10;
            }
        }
    }
    //pass the function to print it to the status bar
    draw_full_fruit(w,h,buf,12*size,12);
    
}
unsigned char font_data[256][16] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7E, 0x81, 0xA5, 0x81, 0x81, 0xBD,
     0x99, 0x81, 0x81, 0x7E, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7E, 0xFF, 0xDB, 0xFF, 0xFF, 0xC3,
     0xE7, 0xFF, 0xFF, 0x7E, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x6C, 0xFE, 0xFE, 0xFE,
     0xFE, 0x7C, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x7C, 0xFE,
     0x7C, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x18, 0x3C, 0x3C, 0xE7, 0xE7,
     0xE7, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x18, 0x3C, 0x7E, 0xFF, 0xFF,
     0x7E, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3C,
     0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC3,
     0xC3, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x42,
     0x42, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0x99, 0xBD,
     0xBD, 0x99, 0xC3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    {0x00, 0x00, 0x1E, 0x0E, 0x1A, 0x32, 0x78, 0xCC,
     0xCC, 0xCC, 0xCC, 0x78, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x3C,
     0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x3F, 0x33, 0x3F, 0x30, 0x30, 0x30,
     0x30, 0x70, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7F, 0x63, 0x7F, 0x63, 0x63, 0x63,
     0x63, 0x67, 0xE7, 0xE6, 0xC0, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x18, 0x18, 0xDB, 0x3C, 0xE7,
     0x3C, 0xDB, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFE, 0xF8,
     0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x02, 0x06, 0x0E, 0x1E, 0x3E, 0xFE, 0x3E,
     0x1E, 0x0E, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18,
     0x7E, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
     0x66, 0x00, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7F, 0xDB, 0xDB, 0xDB, 0x7B, 0x1B,
     0x1B, 0x1B, 0x1B, 0x1B, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x7C, 0xC6, 0x60, 0x38, 0x6C, 0xC6, 0xC6,
     0x6C, 0x38, 0x0C, 0xC6, 0x7C, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0xFE, 0xFE, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18,
     0x7E, 0x3C, 0x18, 0x7E, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18,
     0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
     0x18, 0x7E, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x0C, 0xFE,
     0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x60, 0xFE,
     0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0,
     0xC0, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x6C, 0xFE,
     0x6C, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x38, 0x7C,
     0x7C, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0x7C, 0x7C,
     0x38, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x18, 0x3C, 0x3C, 0x3C, 0x18, 0x18,
     0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x66, 0x66, 0x66, 0x24, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x6C, 0x6C, 0xFE, 0x6C, 0x6C,
     0x6C, 0xFE, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00},
    {0x18, 0x18, 0x7C, 0xC6, 0xC2, 0xC0, 0x7C, 0x06,
     0x06, 0x86, 0xC6, 0x7C, 0x18, 0x18, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0xC2, 0xC6, 0x0C, 0x18,
     0x30, 0x60, 0xC6, 0x86, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x38, 0x6C, 0x6C, 0x38, 0x76, 0xDC,
     0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x30, 0x30, 0x30, 0x60, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30,
     0x30, 0x30, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x0C,
     0x0C, 0x0C, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x3C, 0xFF,
     0x3C, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7E,
     0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x18, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x0C, 0x18,
     0x30, 0x60, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0xC6,
     0xC6, 0xC6, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x18, 0x38, 0x78, 0x18, 0x18, 0x18,
     0x18, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7C, 0xC6, 0x06, 0x0C, 0x18, 0x30,
     0x60, 0xC0, 0xC6, 0xFE, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7C, 0xC6, 0x06, 0x06, 0x3C, 0x06,
     0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x0C, 0x1C, 0x3C, 0x6C, 0xCC, 0xFE,
     0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xFE, 0xC0, 0xC0, 0xC0, 0xFC, 0x06,
     0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x38, 0x60, 0xC0, 0xC0, 0xFC, 0xC6,
     0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xFE, 0xC6, 0x06, 0x06, 0x0C, 0x18,
     0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7C, 0xC6,
     0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7E, 0x06,
     0x06, 0x06, 0x0C, 0x78, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
     0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
     0x00, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x06, 0x0C, 0x18, 0x30, 0x60,
     0x30, 0x18, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00,
     0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06,
     0x0C, 0x18, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7C, 0xC6, 0xC6, 0x0C, 0x18, 0x18,
     0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xDE, 0xDE,
     0xDE, 0xDC, 0xC0, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE,
     0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x66,
     0x66, 0x66, 0x66, 0xFC, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x3C, 0x66, 0xC2, 0xC0, 0xC0, 0xC0,
     0xC0, 0xC2, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xF8, 0x6C, 0x66, 0x66, 0x66, 0x66,
     0x66, 0x66, 0x6C, 0xF8, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xFE, 0x66, 0x62, 0x68, 0x78, 0x68,
     0x60, 0x62, 0x66, 0xFE, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xFE, 0x66, 0x62, 0x68, 0x78, 0x68,
     0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x3C, 0x66, 0xC2, 0xC0, 0xC0, 0xDE,
     0xC6, 0xC6, 0x66, 0x3A, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xFE, 0xC6,
     0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18,
     0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
     0xCC, 0xCC, 0xCC, 0x78, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xE6, 0x66, 0x66, 0x6C, 0x78, 0x78,
     0x6C, 0x66, 0x66, 0xE6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xF0, 0x60, 0x60, 0x60, 0x60, 0x60,
     0x60, 0x62, 0x66, 0xFE, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6,
     0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xC6, 0xE6, 0xF6, 0xFE, 0xDE, 0xCE,
     0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6,
     0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x60,
     0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6,
     0xC6, 0xD6, 0xDE, 0x7C, 0x0C, 0x0E, 0x00, 0x00},
    {0x00, 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x6C,
     0x66, 0x66, 0x66, 0xE6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7C, 0xC6, 0xC6, 0x60, 0x38, 0x0C,
     0x06, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7E, 0x7E, 0x5A, 0x18, 0x18, 0x18,
     0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6,
     0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6,
     0xC6, 0x6C, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xD6,
     0xD6, 0xFE, 0xEE, 0x6C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xC6, 0xC6, 0x6C, 0x7C, 0x38, 0x38,
     0x7C, 0x6C, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18,
     0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xFE, 0xC6, 0x86, 0x0C, 0x18, 0x30,
     0x60, 0xC2, 0xC6, 0xFE, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30,
     0x30, 0x30, 0x30, 0x3C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0x70, 0x38,
     0x1C, 0x0E, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
     0x0C, 0x0C, 0x0C, 0x3C, 0x00, 0x00, 0x00, 0x00},
    {0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00},
    {0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0C, 0x7C,
     0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xE0, 0x60, 0x60, 0x78, 0x6C, 0x66,
     0x66, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC0,
     0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x1C, 0x0C, 0x0C, 0x3C, 0x6C, 0xCC,
     0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xFE,
     0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x38, 0x6C, 0x64, 0x60, 0xF0, 0x60,
     0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xCC, 0xCC,
     0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xCC, 0x78, 0x00},
    {0x00, 0x00, 0xE0, 0x60, 0x60, 0x6C, 0x76, 0x66,
     0x66, 0x66, 0x66, 0xE6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x18, 0x18, 0x00, 0x38, 0x18, 0x18,
     0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x06, 0x06, 0x00, 0x0E, 0x06, 0x06,
     0x06, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3C, 0x00},
    {0x00, 0x00, 0xE0, 0x60, 0x60, 0x66, 0x6C, 0x78,
     0x78, 0x6C, 0x66, 0xE6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18,
     0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xEC, 0xFE, 0xD6,
     0xD6, 0xD6, 0xD6, 0xC6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x66,
     0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6,
     0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x66,
     0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xCC, 0xCC,
     0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0x0C, 0x1E, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x76, 0x66,
     0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0x60,
     0x38, 0x0C, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x10, 0x30, 0x30, 0xFC, 0x30, 0x30,
     0x30, 0x30, 0x36, 0x1C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC,
     0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66,
     0x66, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xD6,
     0xD6, 0xD6, 0xFE, 0x6C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0x6C, 0x38,
     0x38, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6,
     0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x0C, 0xF8, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xCC, 0x18,
     0x30, 0x60, 0xC6, 0xFE, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x0E, 0x18, 0x18, 0x18, 0x70, 0x18,
     0x18, 0x18, 0x18, 0x0E, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18,
     0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x70, 0x18, 0x18, 0x18, 0x0E, 0x18,
     0x18, 0x18, 0x18, 0x70, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x76, 0xDC, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x6C, 0xC6,
     0xC6, 0xC6, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x3C, 0x66, 0xC2, 0xC0, 0xC0, 0xC0,
     0xC2, 0x66, 0x3C, 0x0C, 0x06, 0x7C, 0x00, 0x00},
    {0x00, 0x00, 0xCC, 0x00, 0x00, 0xCC, 0xCC, 0xCC,
     0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x0C, 0x18, 0x30, 0x00, 0x7C, 0xC6, 0xFE,
     0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x10, 0x38, 0x6C, 0x00, 0x78, 0x0C, 0x7C,
     0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xCC, 0x00, 0x00, 0x78, 0x0C, 0x7C,
     0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x60, 0x30, 0x18, 0x00, 0x78, 0x0C, 0x7C,
     0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x38, 0x6C, 0x38, 0x00, 0x78, 0x0C, 0x7C,
     0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x60, 0x60,
     0x66, 0x3C, 0x0C, 0x06, 0x3C, 0x00, 0x00, 0x00},
    {0x00, 0x10, 0x38, 0x6C, 0x00, 0x7C, 0xC6, 0xFE,
     0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xC6, 0x00, 0x00, 0x7C, 0xC6, 0xFE,
     0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x60, 0x30, 0x18, 0x00, 0x7C, 0xC6, 0xFE,
     0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x66, 0x00, 0x00, 0x38, 0x18, 0x18,
     0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x18, 0x3C, 0x66, 0x00, 0x38, 0x18, 0x18,
     0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x60, 0x30, 0x18, 0x00, 0x38, 0x18, 0x18,
     0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0xC6, 0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6,
     0xFE, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00},
    {0x38, 0x6C, 0x38, 0x00, 0x38, 0x6C, 0xC6, 0xC6,
     0xFE, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00},
    {0x18, 0x30, 0x60, 0x00, 0xFE, 0x66, 0x60, 0x7C,
     0x60, 0x60, 0x66, 0xFE, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xCC, 0x76, 0x36,
     0x7E, 0xD8, 0xD8, 0x6E, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x3E, 0x6C, 0xCC, 0xCC, 0xFE, 0xCC,
     0xCC, 0xCC, 0xCC, 0xCE, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x10, 0x38, 0x6C, 0x00, 0x7C, 0xC6, 0xC6,
     0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xC6, 0x00, 0x00, 0x7C, 0xC6, 0xC6,
     0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x60, 0x30, 0x18, 0x00, 0x7C, 0xC6, 0xC6,
     0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x30, 0x78, 0xCC, 0x00, 0xCC, 0xCC, 0xCC,
     0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x60, 0x30, 0x18, 0x00, 0xCC, 0xCC, 0xCC,
     0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xC6, 0x00, 0x00, 0xC6, 0xC6, 0xC6,
     0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x0C, 0x78, 0x00},
    {0x00, 0xC6, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6,
     0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0xC6, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6,
     0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x18, 0x18, 0x3C, 0x66, 0x60, 0x60, 0x60,
     0x66, 0x3C, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x38, 0x6C, 0x64, 0x60, 0xF0, 0x60, 0x60,
     0x60, 0x60, 0xE6, 0xFC, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18,
     0x7E, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0xF8, 0xCC, 0xCC, 0xF8, 0xC4, 0xCC, 0xDE,
     0xCC, 0xCC, 0xCC, 0xC6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x0E, 0x1B, 0x18, 0x18, 0x18, 0x7E, 0x18,
     0x18, 0x18, 0x18, 0x18, 0xD8, 0x70, 0x00, 0x00},
    {0x00, 0x18, 0x30, 0x60, 0x00, 0x78, 0x0C, 0x7C,
     0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x0C, 0x18, 0x30, 0x00, 0x38, 0x18, 0x18,
     0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x18, 0x30, 0x60, 0x00, 0x7C, 0xC6, 0xC6,
     0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x18, 0x30, 0x60, 0x00, 0xCC, 0xCC, 0xCC,
     0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x76, 0xDC, 0x00, 0xDC, 0x66, 0x66,
     0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00},
    {0x76, 0xDC, 0x00, 0xC6, 0xE6, 0xF6, 0xFE, 0xDE,
     0xCE, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x3C, 0x6C, 0x6C, 0x3E, 0x00, 0x7E, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x38, 0x6C, 0x6C, 0x38, 0x00, 0x7C, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x30, 0x30, 0x00, 0x30, 0x30, 0x60,
     0xC0, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xC0,
     0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x06,
     0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0xC0, 0xC0, 0xC2, 0xC6, 0xCC, 0x18, 0x30,
     0x60, 0xDC, 0x86, 0x0C, 0x18, 0x3E, 0x00, 0x00},
    {0x00, 0xC0, 0xC0, 0xC2, 0xC6, 0xCC, 0x18, 0x30,
     0x66, 0xCE, 0x9E, 0x3E, 0x06, 0x06, 0x00, 0x00},
    {0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18,
     0x3C, 0x3C, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x6C, 0xD8,
     0x6C, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xD8, 0x6C, 0x36,
     0x6C, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44,
     0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44},
    {0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA,
     0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA},
    {0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77,
     0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
     0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF8,
     0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0xF8,
     0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xF6,
     0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE,
     0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x18, 0xF8,
     0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0x36, 0x36, 0x36, 0x36, 0x36, 0xF6, 0x06, 0xF6,
     0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36},
    {0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
     0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x06, 0xF6,
     0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36},
    {0x36, 0x36, 0x36, 0x36, 0x36, 0xF6, 0x06, 0xFE,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xFE,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0xF8,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8,
     0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
     0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F,
     0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF,
     0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x1F,
     0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x37,
     0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36},
    {0x36, 0x36, 0x36, 0x36, 0x36, 0x37, 0x30, 0x3F,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x30, 0x37,
     0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36},
    {0x36, 0x36, 0x36, 0x36, 0x36, 0xF7, 0x00, 0xFF,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xF7,
     0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36},
    {0x36, 0x36, 0x36, 0x36, 0x36, 0x37, 0x30, 0x37,
     0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x36, 0x36, 0x36, 0x36, 0x36, 0xF7, 0x00, 0xF7,
     0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x00, 0xFF,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xFF,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF,
     0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
     0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36},
    {0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x3F,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x1F,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x18, 0x1F,
     0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
     0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36},
    {0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xFF,
     0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0xFF,
     0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF8,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,
     0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    {0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
     0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0},
    {0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
     0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xDC, 0xD8,
     0xD8, 0xD8, 0xDC, 0x76, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0xD8, 0xCC,
     0xC6, 0xC6, 0xC6, 0xCC, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xFE, 0xC6, 0xC6, 0xC0, 0xC0, 0xC0,
     0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0xFE, 0x6C, 0x6C, 0x6C,
     0x6C, 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0xFE, 0xC6, 0x60, 0x30, 0x18,
     0x30, 0x60, 0xC6, 0xFE, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0xD8, 0xD8,
     0xD8, 0xD8, 0xD8, 0x70, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66,
     0x66, 0x7C, 0x60, 0x60, 0xC0, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x76, 0xDC, 0x18, 0x18,
     0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x7E, 0x18, 0x3C, 0x66, 0x66,
     0x66, 0x3C, 0x18, 0x7E, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xFE,
     0xC6, 0xC6, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0x6C,
     0x6C, 0x6C, 0x6C, 0xEE, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x1E, 0x30, 0x18, 0x0C, 0x3E, 0x66,
     0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0xDB, 0xDB,
     0xDB, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x03, 0x06, 0x7E, 0xDB, 0xDB,
     0xF3, 0x7E, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x1C, 0x30, 0x60, 0x60, 0x7C, 0x60,
     0x60, 0x60, 0x30, 0x1C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6,
     0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0xFE,
     0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7E, 0x18,
     0x18, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x30, 0x18, 0x0C, 0x06, 0x0C,
     0x18, 0x30, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x0C, 0x18, 0x30, 0x60, 0x30,
     0x18, 0x0C, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x0E, 0x1B, 0x1B, 0x18, 0x18, 0x18,
     0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
     0xD8, 0xD8, 0xD8, 0x70, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x7E,
     0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xDC, 0x00,
     0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x38, 0x6C, 0x6C, 0x38, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
     0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x0F, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0xEC,
     0x6C, 0x6C, 0x3C, 0x1C, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0xD8, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x70, 0xD8, 0x30, 0x60, 0xC8, 0xF8, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x7C, 0x7C, 0x7C, 0x7C,
     0x7C, 0x7C, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};
