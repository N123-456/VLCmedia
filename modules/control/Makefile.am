controldir = $(pluginsdir)/control

libdummy_plugin_la_SOURCES = control/dummy.c control/intromsg.h
libgestures_plugin_la_SOURCES = control/gestures.c
libhotkeys_plugin_la_SOURCES = control/hotkeys.c
libhotkeys_plugin_la_LIBADD = $(LIBM)
# XXX: netsync disabled, move current code to new playlist/player and add a
# way to control the output clock from the player
#libnetsync_plugin_la_SOURCES = control/netsync.c
#libnetsync_plugin_la_LIBADD = $(SOCKET_LIBS)
librc_plugin_la_SOURCES = \
	control/intromsg.h \
	control/cli/player.c control/cli/playlist.c \
	control/cli/cli.c control/cli/cli.h
librc_plugin_la_LIBADD = $(SOCKET_LIBS) $(LIBM)

control_LTLIBRARIES = \
	libdummy_plugin.la \
	libgestures_plugin.la \
	libhotkeys_plugin.la \
	librc_plugin.la

liblirc_plugin_la_SOURCES = control/lirc.c
liblirc_plugin_la_LIBADD = -llirc_client
if HAVE_LIRC
control_LTLIBRARIES += liblirc_plugin.la
endif

libvlc_motion_la_SOURCES = control/motionlib.c control/motionlib.h
libvlc_motion_la_LDFLAGS = -static
noinst_LTLIBRARIES += libvlc_motion.la

libdbus_plugin_la_SOURCES = \
	control/dbus/dbus_introspect.h control/dbus/dbus_common.h \
	control/dbus/dbus_root.c control/dbus/dbus_root.h \
	control/dbus/dbus_player.c control/dbus/dbus_player.h \
	control/dbus/dbus_tracklist.c control/dbus/dbus_tracklist.h \
	control/dbus/dbus.c
libdbus_plugin_la_CFLAGS = $(AM_CFLAGS) $(DBUS_CFLAGS)
libdbus_plugin_la_LIBADD = $(DBUS_LIBS) $(LIBM)
if HAVE_DBUS
control_LTLIBRARIES += libdbus_plugin.la
endif

libxcb_hotkeys_plugin_la_SOURCES = control/globalhotkeys/xcb.c
libxcb_hotkeys_plugin_la_CFLAGS = $(AM_CFLAGS) \
	$(XCB_KEYSYMS_CFLAGS) $(XCB_CFLAGS)
libxcb_hotkeys_plugin_la_LIBADD = $(XCB_KEYSYMS_LIBS) $(XCB_LIBS)
if HAVE_XCB_KEYSYMS
control_LTLIBRARIES += libxcb_hotkeys_plugin.la
endif

libntservice_plugin_la_SOURCES = control/ntservice.c
libwin_hotkeys_plugin_la_SOURCES = control/globalhotkeys/win32.c
libwin_msg_plugin_la_SOURCES = control/win_msg.c
if HAVE_WIN32_DESKTOP
control_LTLIBRARIES += libntservice_plugin.la
control_LTLIBRARIES += libwin_hotkeys_plugin.la libwin_msg_plugin.la
endif
