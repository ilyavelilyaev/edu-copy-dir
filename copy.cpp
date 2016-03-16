//
//  copy.cpp
//  Copy Directory
//
//  Created by Ilya Velilyaev on 15.03.16.
//  Copyright © 2016 Ilya Velilyaev. All rights reserved.
//

#include <iostream>
#include <queue>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

#define MAX_OPENED_FILES 20


namespace fs = boost::filesystem;

void print_usage() {
    std::cerr << "Usage:\n\tcopy 'dir_from' 'dir_where'\n";
    exit(1);
}

void copy_file(fs::path &from, fs::path &full_path, fs::path &current_dir_to_create) {
    try {
        /* Check if directory exists, if not, then create it */
        if (!fs::exists(current_dir_to_create)) {
            if (fs::create_directories(current_dir_to_create)) {
                
#ifdef DEBUG
                std::cout << "Created Directory: " << current_dir_to_create << "\n";
#endif //DEBUG
                
            } else {
                std::cerr << "Error: Couldn't create dir \'" << full_path << "\' (maybe it is already existing)\n";
            }
        }
        
        /* Copy file */
        fs::copy_file(from, full_path, fs::copy_option::overwrite_if_exists);
        
    } catch (fs::filesystem_error const &e) {
        std::cerr << e.what() << "\n";
        exit(2);
        return;
    }
    
    
}

int main(int argc, char* argv[])
{
    /* Checking the correctness of number of arguments. */
    if (argc <= 2 || argc >= 4) {
        print_usage();
    }
    
    
    /* Initializing pathes and checking whether they are directories. */
    fs::path path_from = fs::path(argv[1]);
    fs::path path_where = fs::path(argv[2]);
    
    try {
        
        if (!fs::exists(path_from) || !fs::exists(path_where) ||
            !fs::is_directory(path_from) || !fs::is_directory(path_where)) {
            std::cerr << "Error! One of pathes is incorrect.\n";
            print_usage();
        }
        
        path_from = fs::canonical(path_from);
        path_where = fs::canonical(path_where);
        
        /* Creating directory with same name in 'where' path. */
        
        if (fs::create_directory(path_where / path_from.filename())) {
            
#ifdef DEBUG
            std::cout << "Created Directory: " <<  path_where / path_from.filename() << "\n";
#endif //DEBUG
            
        } else {
            std::cerr << "Error: Couldn't create dir \'" << path_where / path_from.filename() << "\' (maybe it is already existing)\n";
            print_usage();
        }
        
    } catch (fs::filesystem_error const &e) {
        std::cerr << e.what() << "\n";
        print_usage();
    }
    
    
    /* Creating Queue to track threads */
    std::queue<boost::thread*> threads;
    
    /* Recursively walk through all inner directories and copy files using different threads. */
    for (fs::recursive_directory_iterator it(path_from), end; it != end; it++) {
        
        try {
            fs::path current(*it);
            fs::path relative_path = path_from.filename() / fs::relative(current, path_from);
            fs::path full_path = path_where / relative_path;
            fs::path current_dir_to_create = full_path.parent_path();
            
            if (fs::is_regular_file(current)) {
                boost::thread *t = new boost::thread(copy_file, current, full_path, current_dir_to_create);
                threads.push(t);
            }
            
        } catch (fs::filesystem_error const &e) {
            std::cerr << e.what() << "\n";
            print_usage();
        }
        
        /* Check if amount of open threads is less then maximum allowed */
        if (threads.size() >= MAX_OPENED_FILES) {
            (threads.front())->join();
            threads.pop();
        }
        
        
    }
    
    /* Joining all threads */
    
    unsigned int size = threads.size();
    for (unsigned int i = 0; i < size; i++) {
        (threads.front())->join();
        threads.pop();
    }
    
    
    
    return 0;
}