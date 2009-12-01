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

#ifndef __GEDIT_Dict_PLUGIN_H__
#define __GEDIT_Dict_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <gedit/gedit-plugin.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define GEDIT_TYPE_DICT_PLUGIN		(gedit_dict_plugin_get_type ())
#define GEDIT_DICT_PLUGIN(o)			(G_TYPE_CHECK_INSTANCE_CAST ((o), GEDIT_TYPE_DICT_PLUGIN, GeditDictPlugin))
#define GEDIT_DICT_PLUGIN_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), GEDIT_TYPE_Dict_PLUGIN, GeditDictPluginClass))
#define GEDIT_IS_DICT_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), GEDIT_TYPE_Dict_PLUGIN))
#define GEDIT_IS_DICT_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), GEDIT_TYPE_Dict_PLUGIN))
#define GEDIT_Dict_PLUGIN_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), GEDIT_TYPE_Dict_PLUGIN, GeditDictPluginClass))

/* Private structure type */
typedef struct _GeditDictPluginPrivate	GeditDictPluginPrivate;

/*
 * Main object structure
 */
typedef struct _GeditDictPlugin		GeditDictPlugin;

struct _GeditDictPlugin
{
	GeditPlugin parent_instance;
};

/*
 * Class definition
 */
typedef struct _GeditDictPluginClass	GeditDictPluginClass;

struct _GeditDictPluginClass
{
	GeditPluginClass parent_class;
};

/*
 * Public methods
 */
GType	gedit_Dict_plugin_get_type	(void) G_GNUC_CONST;

/* All the plugins must implement this function */
G_MODULE_EXPORT GType register_gedit_plugin (GTypeModule *module);

G_END_DECLS

#endif /* __GEDIT_Dict_PLUGIN_H__ */
