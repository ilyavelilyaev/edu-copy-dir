all: 
	CPLUS_INCLUDE_PATH=/usr/include:/usr/local/include LIBRARY_PATH=/usr/local/lib clang++ -lboost_filesystem-mt -lboost_system-mt copy.cpp -o copy
