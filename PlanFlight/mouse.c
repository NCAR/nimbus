
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <stdio.h>

gboolean
on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data); 

float  axx, axy, ayx, ayy, det;
float  xl, xh, yl, yh;
int    type_X, type_Y;
int    ixx = 0, iyy = 0, ioo = 0;
int    ix0, iy0, ix1, ix2, iy1, iy2;
FILE  *log_file;

int 
main(int argc, char* argv[])
{
  GtkWidget *window; 

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* connect signal callbacks */
    g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (gtk_main_quit), NULL);
    g_signal_connect (G_OBJECT (window), "key_press_event",
                    G_CALLBACK (on_key_press), NULL);
                   
//     get axis information:
    printf(" Minimum coordinate for abscissa: ");
    scanf("%f", &xl);
    printf(" Maximum coordinate for abscissa: ");
    scanf("%f", &xh);
    printf(" Abscissa axis type (0 for linear, 1 for logarithmic): ");
    scanf("%d", &type_X);
    printf(" Abscissa limits are %f, %f, type is %d\n", xl, xh, type_X);
    printf(" Minimum coordinate for ordinate: ");
    scanf("%f", &yl);
    printf(" Maximum coordinate for ordinate: ");
    scanf("%f", &yh);
    printf(" Ordinate axis type (0 for linear, 1 for logarithmic): ");
    scanf("%d", &type_Y);
    printf(" Ordinate limits are %f, %f, type is %d\n", yl, yh, type_Y);;
    log_file = fopen("./mouse.output", "a");
    fprintf(log_file, " Abscissa limits are %f, %f, type is %d\n", xl, xh, type_X);
    fprintf(log_file, " Ordinate limits are %f, %f, type is %d\n", yl, yh, type_Y);;

    if (type_X) { 
	xl = log10(xl); 
	xh = log10(xh); 
    }
    if (type_Y) { 
	yl = log10(yl); 
	yh = log10(yh); 
    }
    printf(" Next locate cursor on origin of plot and press \"o\"\n");
    gtk_widget_show_all (window);
   
    gtk_main ();
 
    return 0;
} 

gboolean
on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
   
    gint x=0, y=0, xadd=0, yadd=0;
    GdkDisplay *display = NULL;
    GdkScreen *screen = NULL;
    float xf, yf;

    printf(" key pressed = %d, ", event->keyval);
    /* get default display and screen */
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_widget_grab_focus ((GtkWidget*) screen);
   
    /* get cursor position */
    gdk_display_get_pointer (display, NULL, &x, &y, NULL);
    printf(" cursor location is %d, %d\n", x, y);
    if (ioo == 0 && event->keyval == 111) {
	ioo = 1;
	ix0 = x;
	iy0 = y;
        printf(" Next locate cursor at end of abscissa and press \"x\"\n");
    } else if (ixx == 0 && event->keyval == 120) {
	ixx = 1;
	ix2 = x;
	iy1 = y;
        printf(" Next locate cursor at end of ordinate and press \"y\"\n");
    } else if (iyy == 0 && event->keyval == 121) {
	iyy = 1;
	ix1 = x;
	iy2 = y;
	printf(" Now enter any symbol except o,x,y,q to get coordinates of the cursor\n");
	printf(" Use \"Q\" to quit.\n");
    } else if (event->keyval == 113) {
        fclose(log_file);
	exit(0);
    } else {

        if (ioo && ixx && iyy) {
            ioo = ixx = iyy = 0;
            axx = (ix2 - ix0) / (xh - xl);
            axy = (ix1 - ix0) / (yh - yl);
            ayx = (iy1 - iy0) / (xh - xl);
            ayy = (iy2 - iy0) / (yh - yl);
            det = axx * ayy - axy * ayx;
	    printf(" det = %lf, \n, axx, axy, ayx, ayy = %lf, %lf, %lf, %lf\n",
			det, axx, axy, ayx, ayy);
	    printf(" xl, yl = %f, %f\n", xl, yl);
        }
//     transformation is:
	xf = xl + (ayy * (x - ix0) - axy * (y - iy0)) / det;
	yf = yl - (ayx * (x - ix0) - axx * (y - iy0)) / det;
	if (type_X) { xf = pow(10., xf); }
	if (type_Y) { yf = pow(10., yf); }
	printf(" %c,%f,%f\n", event->keyval, xf, yf);
	fprintf(log_file, "%c,%f,%f\n", event->keyval, xf, yf);
    }
    return FALSE; /* propogate event */
}
