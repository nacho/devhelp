/*
 * gsc-provider-devhelp.h
 * This file is part of gedit
 *
 * Copyright (C) 2009 - Jesse van den Kieboom
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


#ifndef __GSC_PROVIDER_DEVHELP_H__
#define __GSC_PROVIDER_DEVHELP_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define GSC_TYPE_PROVIDER_DEVHELP		(gsc_provider_devhelp_get_type ())
#define GSC_PROVIDER_DEVHELP(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GSC_TYPE_PROVIDER_DEVHELP, GscProviderDevhelp))
#define GSC_PROVIDER_DEVHELP_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GSC_TYPE_PROVIDER_DEVHELP, GscProviderDevhelp const))
#define GSC_PROVIDER_DEVHELP_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GSC_TYPE_PROVIDER_DEVHELP, GscProviderDevhelpClass))
#define GSC_IS_PROVIDER_DEVHELP(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSC_TYPE_PROVIDER_DEVHELP))
#define GSC_IS_PROVIDER_DEVHELP_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GSC_TYPE_PROVIDER_DEVHELP))
#define GSC_PROVIDER_DEVHELP_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GSC_TYPE_PROVIDER_DEVHELP, GscProviderDevhelpClass))

typedef struct _GscProviderDevhelp		GscProviderDevhelp;
typedef struct _GscProviderDevhelpClass		GscProviderDevhelpClass;
typedef struct _GscProviderDevhelpPrivate	GscProviderDevhelpPrivate;

struct _GscProviderDevhelp
{
	GObject parent;
	
	GscProviderDevhelpPrivate *priv;
};

struct _GscProviderDevhelpClass
{
	GObjectClass parent_class;
};

GType		 gsc_provider_devhelp_get_type		(void) G_GNUC_CONST;

GType		 gsc_provider_devhelp_register_type	(GTypeModule * module);

GscProviderDevhelp *gsc_provider_devhelp_new (void);


G_END_DECLS

#endif /* __GSC_PROVIDER_DEVHELP_H__ */
