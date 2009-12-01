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

#ifndef __DICT_PANEL_H__
#define __DICT_PANEL_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define GEDIT_TYPE_DICT_PANEL		(gedit_dict_panel_get_type ())
#define GEDIT_DICT_PANEL(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), GEDIT_TYPE_DICT_PANEL, GeditDictPanel))
#define GEDIT_DICT_PANEL_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), GEDIT_TYPE_DICT_PANEL, GeditDictPanelClass))
#define GEDIT_IS_DICT_PANEL(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), GEDIT_TYPE_DICT_PANEL))
#define GEDIT_IS_DICT_PANEL_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), GEDIT_TYPE_DICT_PANEL))
#define GEDIT_DICT_PANEL_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), GEDIT_TYPE_DICT_PANEL, GeditDictPanelClass))

/* Private structure type */
typedef struct _GeditDictPanelPrivate	GeditDictPanelPrivate;

/*
 * Main object structure
 */
typedef struct _GeditDictPanel		GeditDictPanel;

struct _GeditDictPanel
{
	GtkVBox parent_instance;
	
	/*< private > */
	GeditDictPanelPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _GeditDictPanelClass	GeditDictPanelClass;

struct _GeditDictPanelClass
{
	GtkVBoxClass parent_class;
};

/*
 * Public methods
 */
GType		 gedit_dict_panel_get_type	   (void) G_GNUC_CONST;

GType		 gedit_dict_panel_register_type    (GTypeModule * module);

GtkWidget *	 gedit_dict_panel_new 		   (void);

void             gedit_dict_panel_set_position     (GeditDictPanel *panel,
						    gint pos);

G_END_DECLS

#endif /* __DICT_PANEL_H__ */
