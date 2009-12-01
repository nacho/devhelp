# Dictionary Panel plugin
plugindir = $(libdir)/gedit-2/plugins

INCLUDES = \
	-I$(top_srcdir) 				\
	$(GEDIT_DEVHELP_PLUGIN_CFLAGS)			\
	$(WARN_CFLAGS)					\
	$(DISABLE_DEPRECATED_CFLAGS)			\
	-DGEDIT_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

plugin_LTLIBRARIES = libdevhelp.la

libdevhelp_la_SOURCES = \
	gsc-provider-devhelp.h	\
	gsc-provider-devhelp.c	\
	gedit-devhelp-plugin.h	\
	gedit-devhelp-plugin.c
	

libdevhelp_la_LDFLAGS = \
	$(PLUGIN_LIBTOOL_FLAGS)				\
	$(GEDIT_DEVHELP_LIBS)

# Plugin Info

plugin_in_files = devhelp.gedit-plugin.desktop.in

%.gedit-plugin: %.gedit-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.gedit-plugin.desktop.in=.gedit-plugin)

EXTRA_DIST = $(plugin_in_files)

CLEANFILES = $(plugin_DATA)
DISTCLEANFILES = $(plugin_DATA)
