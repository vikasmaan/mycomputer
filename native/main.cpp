#include "main.h"

#include <signal.h>
#include <execinfo.h>
#include <webkit/webkit.h>


GtkWidget *web_view=NULL;
GtkWidget *main_window=NULL;
static void window_object_cleared_cb(WebKitWebView  *web_view,WebKitWebFrame *frame,gpointer context,gpointer window_object,gpointer user_data)
{
    (void)web_view;
    (void)frame;
    (void)window_object;
    (void)user_data;
 
}



WebKitWebView *create_gtk_window_around_it(WebKitWebInspector *inspector,WebKitWebView      *webview, void *data)
{
	(void)inspector;
	(void)webview;
	(void)data;
	GtkWidget *win;
	GtkWidget *view;
	printf("create_gtk_window_around_it\n");
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), "Inspector");
	gtk_window_set_default_size(GTK_WINDOW(win), 600, 400);
	view = webkit_web_view_new();
	gtk_container_add(GTK_CONTAINER(win), view);
	g_object_set_data(G_OBJECT(webview), "inspector-window", win);
	return WEBKIT_WEB_VIEW(view);
}

static gboolean show_inspector_window(WebKitWebInspector *inspector,GtkWidget          *webview)
{
	(void)inspector;
	(void)webview;
	GtkWidget *win;
	printf("show_inspector_window\n");
	win = (GtkWidget*)g_object_get_data(G_OBJECT(webview), "inspector-window");
	gtk_widget_show_all(win);
	return TRUE;
}


int main(int argc, char* argv[])
{    
    char* t_url="file:///home/vikas/projects/mycomputer/web/root.html";
    /* Initialize the widget set */
    gtk_init (&argc, &argv);    

    //Initialization Flow: Client UI Up (3)
    /* Create the window widgets */
    main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    GdkScreen* gds = gdk_screen_get_default(); 
    //gtk_window_set_decorated(GTK_WINDOW(main_window), FALSE);
    gtk_window_move(GTK_WINDOW(main_window), 0, 0); 
    // -1, -1 offset due to gtk issue. for full size it consider as fullscreen window and fullscreen samsung0c let top xenapp window to go back
    //Treating resize request of legacy application as fullscreen request
    GdkRectangle gRect;
    gdk_screen_get_monitor_geometry(gds,0,&gRect); //0 refers to primary monitor
    //gtk_window_resize(GTK_WINDOW(main_window),gdk_screen_get_width(gds),gdk_screen_get_height(gds)-1);        
    
    gtk_window_resize(GTK_WINDOW(main_window),gRect.width/2,gRect.height/2);        

    //gtk_window_fullscreen((GtkWindow*)main_window);
    //GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);

    /* Create the WebKit Web View widget */
    web_view = webkit_web_view_new ();

    //Initialization Flow: JS Interface Up (4)
    /* Connect the window object cleared event with callback */
    g_signal_connect (G_OBJECT (web_view), "window-object-cleared", G_CALLBACK(window_object_cleared_cb), web_view);

    /* Place the WebKitWebView in the GtkScrolledWindow */  
    gtk_container_add (GTK_CONTAINER (main_window), web_view);

    /* Connect the destroy window event with destroy function */
   

    /* Load our home page*/
    webkit_web_view_load_uri (WEBKIT_WEB_VIEW (web_view), t_url);


    /* Create the main window */
    //gtk_window_set_default_size (GTK_WINDOW (main_window), 1024, 600);

    /* Show the application window */
    gtk_widget_show_all (main_window);
	
	
    //ui thread mail loop
    gtk_main ();

    //Deinitialization Round
    //memory cleanup
	
    return 0;
}

