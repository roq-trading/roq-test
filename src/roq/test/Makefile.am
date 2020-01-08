bin_PROGRAMS = roq-test
roq_test_LDADD = \
	$(PTHREAD_LIBS) \
	$(GFLAGS_LIBS) \
	$(FMT_LIBS) \
	$(ROQ_API_LIBS) \
	$(ROQ_CLIENT_LIBS)
roq_test_CPPFLAGS = \
	$(PTHREAD_CFLAGS) \
	$(FMT_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(ROQ_API_CFLAGS) \
	$(ROQ_CLIENT_CFLAGS) \
	-I$(top_srcdir)/src
roq_test_SOURCES = \
	application.cpp \
	config.cpp \
       	main.cpp \
       	options.cpp \
       	strategy.cpp
