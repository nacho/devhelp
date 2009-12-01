# Dictionary Panel plugin
plugindir = $(libdir)/gedit-2/plugins

INCLUDES = \
	-I$(top_srcdir) 				\
	$(GEDIT_CFLAGS) 				\
	$(DICT_CFLAGS)				\
	$(WARN_CFLAGS)					\
	$(DISABLE_DEPRECATED_CFLAGS)			\
	-DGEDIT_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

plugin_LTLIBRARIES = libdict.la

libdict_la_SOURCES = \
	dict-panel.h			\
	dict-panel.c			\
	gdict-sidebar.c			\
	gdict-sidebar.h			\
	gedit-dictionary-plugin.h		\
	gedit-dictionary-plugin.c
	

libdict_la_LDFLAGS = \
	$(PLUGIN_LIBTOOL_FLAGS)				\
	$(GEDIT_LIBS)					\
	$(DICT_LIBS)

# Plugin Info

plugin_in_files = dict.gedit-plugin.desktop.in

%.gedit-plugin: %.gedit-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.gedit-plugin.desktop.in=.gedit-plugin)

EXTRA_DIST = $(plugin_in_files)

CLEANFILES = $(plugin_DATA)
DISTCLEANFILES = $(plugin_DATA)
