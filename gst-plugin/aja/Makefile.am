plugin_LTLIBRARIES = libgstaja.la

libgstaja_la_CPPFLAGS = \
	$(GST_PLUGINS_BAD_CFLAGS) \
	$(GST_PLUGINS_BASE_CFLAGS) \
	$(GST_BASE_CFLAGS) \
	$(GST_CFLAGS) \
	-DAJALinux=1 \
	-DAJA_LINUX=1 \
	-I$(GST_NTV2)/ajalibraries/ajantv2/includes \
	-I$(GST_NTV2)/ajalibraries/ajantv2/src \
	-I$(GST_NTV2)/ajalibraries/ajantv2/src/lin \
	-I$(GST_NTV2)/ajalibraries/ajantv2/codecs/hevc/m31 \
	-I$(GST_NTV2)/ajalibraries/ajabase \
	-I$(GST_NTV2)/ajalibraries

libgstaja_la_LIBADD = \
	-L$(GST_NTV2)/lib \
	$(GST_PLUGINS_BASE_LIBS) \
	-lgstaudio-1.0 \
	-lgstvideo-1.0 \
	-lajantv2 \
	-lajabase \
	-lpthread \
	-lrt \
	$(GST_BASE_LIBS) \
	$(GST_LIBS) \
	$(LIBM)

libgstaja_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstaja_la_LIBTOOLFLAGS = $(GST_PLUGIN_LIBTOOLFLAGS)

# sources used to compile this plug-in
libgstaja_la_SOURCES = \
	gstaja.cpp \
	gstajavideosrc.cpp \
	gstajahevcsrc.cpp \
	gstajaaudiosrc.cpp \
	gstajavideosink.cpp \
	gstajaaudiosink.cpp \
	gstntv2.cpp

# headers we need but don't want installed
noinst_HEADERS = \
	gstaja.h \
	gstajavideosrc.h \
	gstajahevcsrc.h \
	gstntv2.h
