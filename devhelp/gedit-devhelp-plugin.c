/*
 * gedit-devhelp-plugin.c
 * This file is part of gedit
 *
 * Copyright (C) 2009 - Ignacio Casal Quinteiro
 *
 * gedit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * gedit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gedit; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "gedit-devhelp-plugin.h"
#include "gsc-provider-devhelp.h"

#include <gedit/gedit-debug.h>
#include <gedit/gedit-window.h>

#define WINDOW_DATA_KEY "GeditDevhelpPluginDataKey"

#define GEDIT_DEVHELP_PLUGIN_GET_PRIVATE(object) \
				(G_TYPE_INSTANCE_GET_PRIVATE ((object),	\
				GEDIT_TYPE_DEVHELP_PLUGIN,		\
				GeditDevhelpPluginPrivate))

typedef struct
{
	GscProviderDevhelp *provider;
	gulong tab_added_id;
	gulong tab_removed_id;
} WindowData;

GEDIT_PLUGIN_REGISTER_TYPE_WITH_CODE (GeditDevhelpPlugin, gedit_devhelp_plugin,
		gedit_devhelp_panel_register_type (module);
)

static void
gedit_devhelp_plugin_init (GeditDevhelpPlugin *plugin)
{
	gedit_debug_message (DEBUG_PLUGINS, "GeditDevhelpPlugin initializing");
}

static void
gedit_devhelp_plugin_finalize (GObject *object)
{
	gedit_debug_message (DEBUG_PLUGINS, "GeditDevhelpPlugin finalizing");

	G_OBJECT_CLASS (gedit_devhelp_plugin_parent_class)->finalize (object);
}

static void
free_window_data (WindowData *data)
{
	g_return_if_fail (data != NULL);

	g_object_unref (data->provider);
	g_slice_free (WindowData, data);
}

static void
add_view (WindowData    *data,
	  GtkSourceView *view)
{
	GtkSourceCompletion *completion;
	
	completion = gtk_source_view_get_completion (view);
	
	gtk_source_completion_add_provider (completion,
					    GTK_SOURCE_COMPLETION_PROVIDER (data->provider),
					    NULL);
}

static void
remove_view (WindowData    *data,
	     GtkSourceView *view)
{
	GtkSourceCompletion *completion;
	
	completion = gtk_source_view_get_completion (view);
	
	gtk_source_completion_remove_provider (completion,
					       GTK_SOURCE_COMPLETION_PROVIDER (data->provider),
					       NULL);
}

static void
tab_added_cb (GeditWindow *window,
	      GeditTab    *tab,
	      gpointer     useless)
{
	GeditView *view;
	WindowData *data;
	
	data = (WindowData *) g_object_get_data (G_OBJECT (window),
						 WINDOW_DATA_KEY);
	g_return_if_fail (data != NULL);
	
	view = gedit_tab_get_view (tab);

	add_view (data, GTK_SOURCE_VIEW (view));
}

static void
tab_removed_cb (GeditWindow *window,
		GeditTab    *tab,
		gpointer     useless)
{
	GeditView *view;
	WindowData *data;
	
	data = (WindowData *) g_object_get_data (G_OBJECT (window),
						 WINDOW_DATA_KEY);
	g_return_if_fail (data != NULL);
	
	view = gedit_tab_get_view (tab);

	remove_view (data, GTK_SOURCE_VIEW (view));
}

static void
impl_activate (GeditPlugin *plugin,
	       GeditWindow *window)
{
	WindowData *data;
	GList *views, *l;

	gedit_debug (DEBUG_PLUGINS);

	data = g_slice_new (WindowData);
	data->provider = gsc_provider_devhelp_new ();

	views = gedit_window_get_views (window);
	for (l = views; l != NULL; l = g_list_next (l))
	{
		add_view (data, GTK_SOURCE_VIEW (l->data));
	}

	g_object_set_data_full (G_OBJECT (window),
				WINDOW_DATA_KEY,
				data,
				(GDestroyNotify) free_window_data);

	data->tab_added_id =
		g_signal_connect (window, "tab-added",
				  G_CALLBACK (tab_added_cb),
				  NULL);
	data->tab_removed_id =
		g_signal_connect (window, "tab-removed",
				  G_CALLBACK (tab_removed_cb),
				  NULL);
}

static void
impl_deactivate (GeditPlugin *plugin,
		 GeditWindow *window)
{
	WindowData *data;
	GList *views, *l;

	gedit_debug (DEBUG_PLUGINS);
	
	data = (WindowData *) g_object_get_data (G_OBJECT (window),
						 WINDOW_DATA_KEY);
	g_return_if_fail (data != NULL);

	views = gedit_window_get_views (window);
	for (l = views; l != NULL; l = g_list_next (l))
	{
		remove_view (data, GTK_SOURCE_VIEW (l->data));
	}

	g_signal_handler_disconnect (window, data->tab_added_id);
	g_signal_handler_disconnect (window, data->tab_removed_id);
	
	g_object_set_data (G_OBJECT (window), WINDOW_DATA_KEY, NULL);
}

static void
gedit_devhelp_plugin_class_init (GeditDevhelpPluginClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GeditPluginClass *plugin_class = GEDIT_PLUGIN_CLASS (klass);

	object_class->finalize = gedit_devhelp_plugin_finalize;

	plugin_class->activate = impl_activate;
	plugin_class->deactivate = impl_deactivate;
}
