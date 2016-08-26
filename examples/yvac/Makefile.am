bin_PROGRAMS = yvac-transcode

yvac_transcode_CPPFLAGS =	\
	$(NULL)

yvac_transcode_CXXFLAGS =	\
	$(LIBYAMI_CFLAGS)	\
	$(LIBVA_CFLAGS)		\
	$(LIBAVFORMAT_CFLAGS)	\
	$(NULL)

yvac_transcode_LDADD =		\
	$(LIBYAMI_LIBS)		\
	$(LIBVA_LIBS)		\
	$(LIBAVFORMAT_LIBS)	\
	$(NULL)

yvac_transcode_SOURCES =			\
	../../tests/decodeinput.cpp		\
	transcode.cpp				\
	vatrace.cpp				\
	$(NULL)

if ENABLE_AVFORMAT
yvac_transcode_SOURCES += ../../tests/decodeinputavformat.cpp
endif
