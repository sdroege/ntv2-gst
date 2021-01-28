plugin_LTLIBRARIES = libgstaja.la

libgstaja_la_CPPFLAGS = \
	-std=c++11 \
	$(GST_PLUGINS_BAD_CFLAGS) \
	$(GST_PLUGINS_BASE_CFLAGS) \
	$(GST_BASE_CFLAGS) \
	$(GST_CFLAGS) \
	-DAJALinux=1 \
	-DAJA_LINUX=1 \
	-I$(srcdir)/../../../ajalibraries/ajantv2/includes \
	-I$(srcdir)/../../../ajalibraries/ajantv2/src \
	-I$(srcdir)/../../../ajalibraries/ajantv2/src/lin \
	-I$(srcdir)/../../../ajalibraries/ajantv2/codecs/hevc/m31 \
	-I$(srcdir)/../../../ajalibraries/ajabase \
	-I$(srcdir)/../../../ajalibraries

libgstaja_la_LIBADD = \
	-L$(srcdir)/../../../lib \
	$(GST_PLUGINS_BASE_LIBS) \
	-lgstaudio-1.0 \
	-lgstvideo-1.0 \
	-lajantv2 \
	-lpthread \
	-lrt \
	$(GST_BASE_LIBS) \
	$(GST_LIBS) \
	$(LIBM)

libgstaja_la_LDFLAGS = -std=c++11 $(GST_PLUGIN_LDFLAGS)
libgstaja_la_LIBTOOLFLAGS = $(GST_PLUGIN_LIBTOOLFLAGS)

# sources used to compile this plug-in
libgstaja_la_SOURCES = \
	gstaja.cpp \
	gstajavideosrc.cpp \
	gstajaaudiosrc.cpp \
	gstajasink.cpp \
	gstajadeviceprovider.cpp \
	gstntv2.cpp
#	gstajavideosink.cpp
#	gstajaaudiosink.cpp
#	gstajahevcsrc.cpp

# headers we need but don't want installed
noinst_HEADERS = \
	gstaja.h \
	gstajavideosrc.h \
	gstajaaudiosrc.h \
	gstajasink.h \
	gstajadeviceprovider.h \
	gstntv2.h \
	../../../ajalibraries/ajantv2/includes/ntv2enums.h \
	../../../ajalibraries/ajantv2/includes/ntv2m31enums.h
#	gstajahevcsrc.h
#	gstajavideosink.cpp
#	gstajaaudiosink.cpp
