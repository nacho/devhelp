/*
 * gedit-devhelp-plugin.h
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


#ifndef __GEDIT_DEVHELP_PLUGIN_H__
#define __GEDIT_DEVHELP_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <gedit/gedit-plugin.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define GEDIT_TYPE_DEVHELP_PLUGIN		(gedit_devhelp_plugin_get_type ())
#define GEDIT_DEVHELP_PLUGIN(o)			(G_TYPE_CHECK_INSTANCE_CAST ((o), GEDIT_TYPE_DEVHELP_PLUGIN, GeditDevhelpPlugin))
#define GEDIT_DEVHELP_PLUGIN_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), GEDIT_TYPE_DEVHELP_PLUGIN, GeditDevhelpPluginClass))
#define GEDIT_IS_DEVHELP_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), GEDIT_TYPE_DEVHELP_PLUGIN))
#define GEDIT_IS_DEVHELP_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), GEDIT_TYPE_DEVHELP_PLUGIN))
#define GEDIT_DEVHELP_PLUGIN_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), GEDIT_TYPE_DEVHELP_PLUGIN, GeditDevhelpPluginClass))

/* Private structure type */
typedef struct _GeditDevhelpPluginPrivate	GeditDevhelpPluginPrivate;

/*
 * Main object structure
 */
typedef struct _GeditDevhelpPlugin		GeditDevhelpPlugin;

struct _GeditDevhelpPlugin
{
	GeditPlugin parent_instance;

	GeditDevhelpPluginPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _GeditDevhelpPluginClass	GeditDevhelpPluginClass;

struct _GeditDevhelpPluginClass
{
	GeditPluginClass parent_class;
};

/*
 * Public methods
 */
GType	gedit_devhelp_plugin_get_type		(void) G_GNUC_CONST;

/* All the plugins must implement this function */
G_MODULE_EXPORT GType register_gedit_plugin 	(GTypeModule *module);

G_END_DECLS

#endif /* __GEDIT_Devhelp_PLUGIN_H__ */
