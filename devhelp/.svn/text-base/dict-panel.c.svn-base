/*
 * Copyright (C) 2007  Ignacio Casal Quinteiro <nacho.resa@gmail.com>
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "dict-panel.h"
#include "gdict-sidebar.h"

#include <glib.h>
#include <glib-object.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <gdict/gdict.h>
#include <gconf/gconf-client.h>
#include <gedit/gedit-plugin.h>

#define GEDIT_DICT_PANEL_GET_PRIVATE(object)	(G_TYPE_INSTANCE_GET_PRIVATE ( \
						 (object),		       \
						 GEDIT_TYPE_DICT_PANEL,     \
						 GeditDictPanelPrivate))

#define GDICT_GCONF_DIR                     "/apps/gnome-dictionary"
#define GDICT_GCONF_DATABASE_KEY            GDICT_GCONF_DIR "/database"
#define GDICT_GCONF_STRATEGY_KEY            GDICT_GCONF_DIR "/strategy"
#define GDICT_DEFAULT_SOURCE_NAME           "Default"
#define GDICT_GCONF_SOURCE_KEY              GDICT_GCONF_DIR "/source-name"

#define PANEL_KEY "/apps/gedit-2/plugins/dictionary"

/* sidebar pages logical ids */
#define GDICT_SIDEBAR_SPELLER_PAGE      "speller"
#define GDICT_SIDEBAR_DATABASES_PAGE    "db-chooser"
#define GDICT_SIDEBAR_STRATEGIES_PAGE   "strat-chooser"

GEDIT_PLUGIN_DEFINE_TYPE(GeditDictPanel, gedit_dict_panel, GTK_TYPE_VBOX)

struct _GeditDictPanelPrivate
{
	GtkPaned   *paned;

	GtkTooltips *tooltips;
  
	GConfClient *gconf_client;
	guint notify_id;
	
	gchar *database;
	gchar *strategy;
	gchar *source_name;  

	gchar *word;  
	GdictContext *context;
	guint lookup_start_id;
	guint lookup_end_id;
	guint error_id;

	GdictSourceLoader *loader;

	GtkWidget *speller;
	GtkWidget *db_chooser;
	GtkWidget *strat_chooser;
	GtkWidget *entry;
	GtkWidget *button;
	GtkWidget *defbox;
	GtkWidget *sidebar;
};


static gchar *
gdict_get_data_dir (void)
{
	gchar *retval;
	
	retval = g_build_filename (g_get_home_dir (),
				   ".gnome2",
				   "gnome-dictionary",
				   NULL);

	return retval;
}

static gchar *
gdict_gconf_get_string_with_default (GConfClient *client,
				     const gchar *key,
				     const gchar *def)
{
	gchar *val;
	
	val = gconf_client_get_string (client, key, NULL);
	return val ? val : g_strdup (def);
}

static void
clear_cb (GtkWidget   *widget,
	  GeditDictPanel *panel)
{
	GeditDictPanelPrivate *priv = panel->priv;

	gtk_entry_set_text (GTK_ENTRY (priv->entry), "");

	if (!priv->defbox)
		return;
  
	gdict_defbox_clear (GDICT_DEFBOX (priv->defbox));
}

static void
gedit_dict_panel_entry_activate_cb (GtkWidget   *widget,
					  GeditDictPanel *panel)
{
	GeditDictPanelPrivate *priv = panel->priv;
	const gchar *text;
	
	text = gtk_entry_get_text (GTK_ENTRY(priv->entry));
	
	if (!text)
		return;
  
	g_free (priv->word);
	priv->word = g_strdup(text);

	gdict_defbox_lookup (GDICT_DEFBOX (priv->defbox), priv->word);
}

static void
gedit_dict_panel_set_database (GeditDictPanel *panel,
				     const gchar *database)
{
	GeditDictPanelPrivate *priv = panel->priv;
  
	g_free (priv->database);

	if (database)
		priv->database = g_strdup (database);
	else
		priv->database = gdict_gconf_get_string_with_default (priv->gconf_client,
								      GDICT_GCONF_DATABASE_KEY,
								      GDICT_DEFAULT_DATABASE);
	if (priv->defbox)
		gdict_defbox_set_database (GDICT_DEFBOX (priv->defbox),
					   priv->database);
}

static void
gedit_dict_panel_set_strategy (GeditDictPanel *panel,
				     const gchar *strategy)
{
	GeditDictPanelPrivate *priv = panel->priv;
	
	g_free (priv->strategy);
	
	if (strategy)
		priv->strategy = g_strdup (strategy);
	else
		priv->strategy = gdict_gconf_get_string_with_default (priv->gconf_client,
								      GDICT_GCONF_STRATEGY_KEY,
								      GDICT_DEFAULT_STRATEGY);
}

static GdictContext *
get_context_from_loader (GeditDictPanel *panel)
{
	GeditDictPanelPrivate *priv = panel->priv;
	GdictSource *source;
	GdictContext *retval;

	if (!priv->source_name)
		priv->source_name = g_strdup (GDICT_DEFAULT_SOURCE_NAME);

	source = gdict_source_loader_get_source (priv->loader,
						 priv->source_name);
	if (!source)
	{
		gchar *detail;
		
		detail = g_strdup_printf (_("No dictionary source available with name '%s'"),
					  priv->source_name);

		g_warning(_("Unable to find dictionary source"));
		g_free (detail);

		return NULL;
	}

	gedit_dict_panel_set_database (panel, gdict_source_get_database (source));
	gedit_dict_panel_set_strategy (panel, gdict_source_get_strategy (source));

	retval = gdict_source_get_context (source);
	if (!retval)
	{
		gchar *detail;

		detail = g_strdup_printf (_("No context available for source '%s'"),
					  gdict_source_get_description (source));
      				
      /*gdict_show_error_dialog (NULL,
                               _("Unable to create a context"),
                               detail);*/
		g_warning(_("Unable to create a context"));

		g_free (detail);
		g_object_unref (source);

		return NULL;
	}

	g_object_unref (source);
	
	return retval;
}

static void
gedit_dict_panel_set_context (GeditDictPanel *panel,
				    GdictContext *context)
{
	GeditDictPanelPrivate *priv = panel->priv;

	if (priv->context)
	{
		g_signal_handler_disconnect (priv->context, priv->lookup_start_id);
		g_signal_handler_disconnect (priv->context, priv->lookup_end_id);
		g_signal_handler_disconnect (priv->context, priv->error_id);

		priv->lookup_start_id = 0;
		priv->lookup_end_id = 0;
		priv->error_id = 0;
		
		g_object_unref (priv->context);
		priv->context = NULL;
	}
	
	if (priv->defbox)
		gdict_defbox_set_context (GDICT_DEFBOX (priv->defbox), context);
	
	if (!context)
		return;
	
	/* attach our callbacks */
	/* priv->lookup_start_id = g_signal_connect (context, "lookup-start",
					    G_CALLBACK (gdict_applet_lookup_start_cb),
					    panel);
  priv->lookup_end_id   = g_signal_connect (context, "lookup-end",
					    G_CALLBACK (gdict_applet_lookup_end_cb),
					    panel);*/
  /*priv->error_id        = g_signal_connect (context, "error",
		  			    G_CALLBACK (gdict_applet_error_cb),
					    panel);*/

	priv->context = context;
}

static void
gedit_dict_panel_set_source_name (GeditDictPanel *panel,
					const gchar *source_name)
{
	GeditDictPanelPrivate *priv = panel->priv;
	GdictContext *context;
	
	g_free (priv->source_name);
	
	if (source_name)
		priv->source_name = g_strdup (source_name);
	else
		priv->source_name = gdict_gconf_get_string_with_default (priv->gconf_client,
									 GDICT_GCONF_SOURCE_KEY,
									 GDICT_DEFAULT_SOURCE_NAME);

	context = get_context_from_loader (panel);
	gedit_dict_panel_set_context (panel, context);
}

static void
strategy_activated_cb (GdictStrategyChooser *chooser,
                       const gchar          *strat_name,
                       const gchar          *strat_desc,
                       GeditDictPanel *panel)
{
	GeditDictPanelPrivate *priv = panel->priv;
	gedit_dict_panel_set_strategy (panel, strat_name);
/*
  if (window->status)
    {
      gchar *message;

      message = g_strdup_printf (_("Strategy `%s' selected"), strat_desc);
      gtk_statusbar_push (GTK_STATUSBAR (window->status), 0, message);
      g_free (message);
    }*/
}

static void
database_activated_cb (GdictDatabaseChooser *chooser,
		       const gchar          *db_name,
		       const gchar          *db_desc,
		       GeditDictPanel *panel)
{
	GeditDictPanelPrivate *priv = panel->priv;
	gedit_dict_panel_set_database (panel, db_name);
	/*
  if (window->status)
    {
      gchar *message;

      message = g_strdup_printf (_("Database `%s' selected"), db_desc);
      gtk_statusbar_push (GTK_STATUSBAR (window->status), 0, message);
      g_free (message);
    }*/
}

static void
gedit_dict_panel_set_word (GeditDictPanel *panel,
				 const gchar *word,
				 const gchar *database)
{
	GeditDictPanelPrivate *priv = panel->priv;
	gchar *title;
	
	g_free (priv->word);
	priv->word = NULL;
	
	if (word && word[0] != '\0')
		priv->word = g_strdup (word);
	else
		return;
	
	if (!database || database[0] == '\0')
		database = priv->database;
	
	if (priv->defbox)
	{
		gdict_defbox_set_database (GDICT_DEFBOX (priv->defbox), database);
		gdict_defbox_lookup (GDICT_DEFBOX (priv->defbox), word);
	}
}

static void
speller_word_activated_cb (GdictSpeller *speller,
			   const gchar  *word,
			   const gchar  *db_name,
			   GeditDictPanel *panel)
{
	GeditDictPanelPrivate *priv = panel->priv;
	gtk_entry_set_text (GTK_ENTRY (priv->entry), word);
	
	gedit_dict_panel_set_word (panel, word, db_name);
	/*
  if (window->status)
    {
      gchar *message;

      message = g_strdup_printf (_("Word `%s' selected"), word);
      gtk_statusbar_push (GTK_STATUSBAR (window->status), 0, message);
      g_free (message);
    }*/
}



static void
sidebar_page_changed_cb (GdictSidebar *sidebar,
			 GeditDictPanel *panel)
{
	GeditDictPanelPrivate *priv = panel->priv;
	const gchar *page_id;
	const gchar *message;
	
	page_id = gdict_sidebar_current_page (sidebar);
	
	switch (page_id[0])
	{
		case 's':
		{
			switch (page_id[1])
			{
				case 'p': /* speller */
					message = _("Double-click on the word to look up");
				if (priv->word)
					gdict_speller_match (GDICT_SPELLER (priv->speller),
							     priv->word);
				break;
				case 't': /* strat-chooser */
					message = _("Double-click on the matching strategy to use");
				
					gdict_strategy_chooser_refresh (GDICT_STRATEGY_CHOOSER (priv->strat_chooser));
				break;
				default:
					message = NULL;
			}
		}
		break;
		case 'd': /* db-chooser */
			message = _("Double-click on the database to use");
		
		gdict_database_chooser_refresh (GDICT_DATABASE_CHOOSER (priv->db_chooser));
		break;
		default:
			message = NULL;
		break;
	}
	
	/*if (message && window->status)
	gtk_statusbar_push (GTK_STATUSBAR (window->status), 0, message);*/
}

static void
store_position(GObject    *gobject,
               GParamSpec *arg1,
               gpointer    user_data)
{
	GtkPaned *paned = GTK_PANED(gobject);
	GConfClient *client;
	gint position;
	
	client = gconf_client_get_default();
	position = gtk_paned_get_position(paned);
	gconf_client_set_int(client, PANEL_KEY "/panel_position", position, NULL);
	
	g_object_unref(client);
}

static void
gedit_dict_panel_link_clicked(GtkWidget *defbox,
			      const gchar *link_text,
			      GeditDictPanel *panel)
{
	if (!link_text)
		return;
  
	g_free (panel->priv->word);
	panel->priv->word = g_strdup(link_text);
	
	gtk_entry_set_text(GTK_ENTRY(panel->priv->entry), link_text);

	gdict_defbox_lookup (GDICT_DEFBOX (defbox), panel->priv->word);
}

static void
gedit_dict_panel_draw (GeditDictPanel *panel)
{
	GtkWidget  *vbox;
	GtkWidget  *hbox;
	
	vbox = gtk_vbox_new (FALSE, 6);
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 6);
	gtk_widget_show (vbox);
 
	hbox = gtk_hbox_new (FALSE, 12);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	gtk_widget_show (hbox);
	
	/*
	 * Look up Button
	 */
	panel->priv->button = gtk_button_new_with_mnemonic (_("Look _up:"));
	g_signal_connect(panel->priv->button, "clicked",
				 G_CALLBACK (gedit_dict_panel_entry_activate_cb),
				 panel);
	gtk_button_set_relief (GTK_BUTTON (panel->priv->button), GTK_RELIEF_NONE);
	gtk_box_pack_start (GTK_BOX (hbox), panel->priv->button, FALSE, FALSE, 0);
 	gtk_widget_show(panel->priv->button);
	
	/*
	 * Entry
	 */
	panel->priv->entry = gtk_entry_new ();
	if (panel->priv->word)
		gtk_entry_set_text (GTK_ENTRY (panel->priv->entry), panel->priv->word);
  
	g_signal_connect (panel->priv->entry, "activate",
				  G_CALLBACK (gedit_dict_panel_entry_activate_cb),
				  panel);
	gtk_box_pack_start (GTK_BOX (hbox), panel->priv->entry, TRUE, TRUE, 0);
	gtk_widget_show (panel->priv->entry);


	/*
	 * Defbox
	 */
	panel->priv->defbox = gdict_defbox_new ();
	if (panel->priv->context)
		gdict_defbox_set_context (GDICT_DEFBOX (panel->priv->defbox),
					  panel->priv->context);
	g_signal_connect(panel->priv->defbox, "link-clicked",
			 G_CALLBACK(gedit_dict_panel_link_clicked), panel);
	
	gtk_container_add (GTK_CONTAINER (vbox), panel->priv->defbox);
	gtk_widget_show (panel->priv->defbox);
	
	panel->priv->sidebar = gdict_sidebar_new ();
	g_signal_connect (panel->priv->sidebar, "page-changed",
			  G_CALLBACK (sidebar_page_changed_cb),
			  panel);

	/*
	 * Paned
	 */
	panel->priv->paned = GTK_PANED(gtk_vpaned_new());
	gtk_box_pack_start (GTK_BOX (panel), GTK_WIDGET(panel->priv->paned), TRUE, TRUE, 0);
	gtk_paned_pack1 (panel->priv->paned, vbox, FALSE, TRUE);
	gtk_paned_pack2 (panel->priv->paned, panel->priv->sidebar, TRUE, TRUE);
	gtk_widget_show (GTK_WIDGET(panel->priv->paned));
	
	g_signal_connect(panel->priv->paned, "notify::position",
			 G_CALLBACK(store_position), NULL);
	
	
	/*
	 * Speller
	 */
	panel->priv->speller = gdict_speller_new ();
	if (panel->priv->context)
		gdict_speller_set_context (GDICT_SPELLER (panel->priv->speller),
					   panel->priv->context);
	g_signal_connect (panel->priv->speller, "word-activated",
			  G_CALLBACK (speller_word_activated_cb),
			  panel);
  
	gdict_sidebar_add_page (GDICT_SIDEBAR (panel->priv->sidebar),
				GDICT_SIDEBAR_SPELLER_PAGE,
				_("Similar words"),
				panel->priv->speller);
	gtk_widget_show (panel->priv->speller);

	/*
	 * db chooser
	 */
  	panel->priv->db_chooser = gdict_database_chooser_new ();
	if (panel->priv->context)
		gdict_database_chooser_set_context (GDICT_DATABASE_CHOOSER (panel->priv->db_chooser),
						    panel->priv->context);
	g_signal_connect (panel->priv->db_chooser, "database-activated",
			  G_CALLBACK (database_activated_cb),
			  panel);
	gdict_sidebar_add_page (GDICT_SIDEBAR (panel->priv->sidebar),
				GDICT_SIDEBAR_DATABASES_PAGE,
				_("Available dictionaries"),
				panel->priv->db_chooser);
	gtk_widget_show (panel->priv->db_chooser);

	/*
	 * Strategy chooser
	 */
  	panel->priv->strat_chooser = gdict_strategy_chooser_new ();
	if (panel->priv->context)
		gdict_strategy_chooser_set_context (GDICT_STRATEGY_CHOOSER (panel->priv->strat_chooser),
						    panel->priv->context);
	g_signal_connect (panel->priv->strat_chooser, "strategy-activated",
			  G_CALLBACK (strategy_activated_cb),
			  panel);
	gdict_sidebar_add_page (GDICT_SIDEBAR (panel->priv->sidebar),
				GDICT_SIDEBAR_STRATEGIES_PAGE,
				_("Available strategies"),
				panel->priv->strat_chooser);
	gtk_widget_show (panel->priv->strat_chooser);

	gtk_widget_show (panel->priv->sidebar);
	
	
}

static void
gedit_dict_panel_gconf_notify_cb (GConfClient *client,
			      guint        cnxn_id,
			      GConfEntry  *entry,
			      gpointer     user_data)
{
	GeditDictPanel *panel = GEDIT_DICT_PANEL (user_data);
	GeditDictPanelPrivate *priv = panel->priv;

	if (strcmp (entry->key, GDICT_GCONF_SOURCE_KEY) == 0)
	{
		if (entry->value && (entry->value->type == GCONF_VALUE_STRING))
			gedit_dict_panel_set_source_name (panel, gconf_value_get_string (entry->value));
		else
			gedit_dict_panel_set_source_name (panel, GDICT_DEFAULT_SOURCE_NAME);
	}
	else if (strcmp (entry->key, GDICT_GCONF_DATABASE_KEY) == 0)
	{
		if (entry->value && (entry->value->type == GCONF_VALUE_STRING))
			gedit_dict_panel_set_database (panel, gconf_value_get_string (entry->value));
		else
			gedit_dict_panel_set_database (panel, GDICT_DEFAULT_DATABASE);
	}
	else if (strcmp (entry->key, GDICT_GCONF_STRATEGY_KEY) == 0)
	{
		if (entry->value && (entry->value->type == GCONF_VALUE_STRING))
			gedit_dict_panel_set_strategy (panel, gconf_value_get_string (entry->value));
		else
			gedit_dict_panel_set_strategy (panel, GDICT_DEFAULT_STRATEGY);
	}
}

static void
gedit_dict_panel_init(GeditDictPanel *panel)
{
	gchar * data_dir;
	GeditDictPanelPrivate *priv;
	GError *gconf_error;
	
	panel->priv = GEDIT_DICT_PANEL_GET_PRIVATE (panel);
	priv = panel->priv;
	
	if (!priv->loader)
		panel->priv->loader = gdict_source_loader_new ();
	
	/* add our data dir inside $HOME to the loader's search paths */
	data_dir = gdict_get_data_dir ();
	gdict_source_loader_add_search_path (priv->loader, data_dir);
	g_free (data_dir);
	
	
	
	/* get the default gconf client */
	if (!priv->gconf_client)
		priv->gconf_client = gconf_client_get_default ();

	gconf_error = NULL;
	gconf_client_add_dir (priv->gconf_client,
			      GDICT_GCONF_DIR,
			      GCONF_CLIENT_PRELOAD_ONELEVEL,
			      &gconf_error);
	if (gconf_error)
	{
		/*gdict_show_gerror_dialog (NULL,
					  _("Unable to connect to GConf"),
					  gconf_error);*/
		gconf_error = NULL;
	}
	
	priv->notify_id = gconf_client_notify_add (priv->gconf_client,
						   GDICT_GCONF_DIR,
						   gedit_dict_panel_gconf_notify_cb,
						   panel, NULL,
						   &gconf_error);
	if (gconf_error)
	{
		/*gdict_show_gerror_dialog (NULL,
					  _("Unable to get notification for preferences"),
					  gconf_error);*/
			
		gconf_error = NULL;
	}
	
	/* force retrieval of the configuration from GConf */
	gedit_dict_panel_set_source_name (panel, NULL);
	
	/* Draw widgets */
	gedit_dict_panel_draw(panel);
}

static void
gedit_dict_panel_finalize (GObject *object)
{
	GeditDictPanel *panel = GEDIT_DICT_PANEL (object);

	g_free (panel->priv->source_name);
	g_free (panel->priv->word);
	g_free (panel->priv->database);
	g_free (panel->priv->strategy);
	
	G_OBJECT_CLASS (gedit_dict_panel_parent_class)->finalize (object);
}

static void
gedit_dict_panel_class_init (GeditDictPanelClass *klass)
{	
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	
	g_type_class_add_private (klass, sizeof (GeditDictPanelPrivate));
	
	gobject_class->finalize = gedit_dict_panel_finalize;  
	
}

GtkWidget *
gedit_dict_panel_new (void)
{
	return GTK_WIDGET (g_object_new (GEDIT_TYPE_DICT_PANEL, NULL));
}

void
gedit_dict_panel_set_position(GeditDictPanel *panel,
			      gint pos)
{
	gtk_paned_set_position(panel->priv->paned, pos);
}

