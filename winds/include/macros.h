/* macros.h -- define macros for general use
*/

#define get_type(type)         (type & 0x0fffffff)
#define FRAME_EVENT_TRUE       0xf0000000
#define over_frame(type)       (type & FRAME_EVENT_TRUE)

#define within_boundaries(x,y,x0,y0,x1,y1) (x>=x0 && x<=x1 && y>=y0 && y<=y1)
