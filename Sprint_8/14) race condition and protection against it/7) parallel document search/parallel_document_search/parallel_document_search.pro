TEMPLATE = app
CONFIG += console c++1z
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -ltbb
LIBS += -lpthread

SOURCES += \
        document.cpp \
        main.cpp \
        process_queries.cpp \
        read_input_functions.cpp \
        remove_duplicates.cpp \
        request_queue.cpp \
        search_server.cpp \
        string_processing.cpp

HEADERS += \
    concurrent_map.h \
    document.h \
    log_duration.h \
    paginator.h \
    process_queries.h \
    read_input_functions.h \
    remove_duplicates.h \
    request_queue.h \
    search_server.h \
    string_processing.h
