all: 
	CPLUS_INCLUDE_PATH=/usr/include:/usr/local/include LIBRARY_PATH=/usr/local/lib clang++ -lboost_filesystem-mt -lboost_system-mt -lboost_thread-mt copy.cpp -o copy
