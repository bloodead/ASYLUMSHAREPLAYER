#include <string.h>

#include <gtk/gtk.h>
#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>

#include <gdk/gdk.h>
#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk/gdkquartz.h>
#endif
#include <stdio.h>

typedef struct _CustomData {
  GstElement *playbin2;

} CustomData;

static void realize_cb (GtkWidget *widget, CustomData *data) {
  GdkWindow *window = gtk_widget_get_window (widget);
  guintptr window_handle;
   
  if (!gdk_window_ensure_native (window))
    g_error ("Couldn't create native window needed for GstXOverlay!"); 
  /* Retrieve window handler from GDK */
#if defined (GDK_WINDOWING_WIN32)
  window_handle = (guintptr)GDK_WINDOW_HWND (window);
#elif defined (GDK_WINDOWING_QUARTZ)
  window_handle = gdk_quartz_window_get_nsview (window);
#elif defined (GDK_WINDOWING_X11)
  window_handle = GDK_WINDOW_XID (window);
#endif
  /* Pass it to playbin2, which implements XOverlay and will forward it to the video sink */
  gst_x_overlay_set_window_handle (GST_X_OVERLAY (data->playbin2), window_handle);
}



int	main(int argc, char *argv[]) 
{
CustomData data;
GtkWidget *main_window;
GtkWidget *video_window;
GtkWidget *main_box;
GtkWidget *title;
GtkWidget *video;
GtkWidget *label;


  /* Initialize 

GStreamer */
	
  gst_init (&argc, &argv);
gtk_init (&argc, &argv);
memset (&data, 0, sizeof (data));
  /* Build the pipeline */
data.playbin2 = gst_parse_launch ("playbin2 uri=http://stream.section-asylum.com/youtube/Oi2dxjiA6nA.webm", NULL);

  /* Start playing */
        //sleep(5);
main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
video_window = gtk_drawing_area_new ();
  gtk_widget_set_double_buffered (video_window, FALSE);
 g_signal_connect (video_window, "realize", G_CALLBACK (realize_cb), &data);
label = gtk_label_new("ASYLUMSHARE PLAYER");

main_box = gtk_vbox_new (FALSE, 0);
title = gtk_hbox_new (FALSE, 0);
video = gtk_hbox_new (FALSE, 0);

  /* Wait until error or EOS */
gtk_box_pack_start (GTK_BOX (video), video_window, TRUE, TRUE, 0);
gtk_box_pack_start (GTK_BOX (title), label, TRUE, TRUE, 0);
gtk_box_pack_start (GTK_BOX (main_box), title, TRUE, TRUE, 0);
gtk_box_pack_start (GTK_BOX (main_box), video, TRUE, TRUE, 0);

gtk_container_add (GTK_CONTAINER (main_window), main_box);
gtk_window_set_default_size (GTK_WINDOW (main_window), 640, 480);
gtk_widget_show_all(main_window);
gst_element_set_state (data.playbin2, GST_STATE_PLAYING);
gtk_main ();
return 0;
}
