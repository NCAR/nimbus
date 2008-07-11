/* vectors.h -- operations involving vectors 
*/
#define VECTOR_ARROW_LENGTH 		5.0
#define VECTOR_TICK_LENGTH		5
#define VECTOR_SCALE_TICKS       	6
#define VECTOR_SCALE_AREA_HEIGHT(q)   	(rbottom[q]-rtop[q])/2
#define VECTOR_SCALE_AREA_YTOP(q)   	rtop[q]
#define VECTOR_SCALE_AREA_YBOT(q)   	rtop[q]+VECTOR_SCALE_AREA_HEIGHT(q)
#define VECTOR_SCALE_AREA_XMIN(q)   	(rright[q]+1)
#define VECTOR_SCALE_RECTANGLE(q) text_left_x,rtop[q], \
                           window_width[q]-text_left_x, \
                           VECTOR_SCALE_AREA_HEIGHT(q)+font_height/2

#define NUM_AVAILABLE_VECTOR_INTRVLS	4
#define VECTOR_DEFAULT_INTRVL		30
#define VECTOR_DEFAULT_SCALE_MAX	VECTOR_SCALE_TICKS-1

#define SAME_AS_TRACE			0
#define CONTRAST_TRACE			1
