//
//  copy.cpp
//  Copy Directory
//
//  Created by Ilya Velilyaev on 15.03.16.
//  Copyright © 2016 Ilya Velilyaev. All rights reserved.
//

#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

namespace fs = boost::filesystem;
namespace sys = boost::system;

void print_usage() {
    std::cerr << "Usage:\n\tcopy 'dir_from' 'dir_where'\n";
    exit(1);
}

int main(int argc, char* argv[])
{
    //Checking the correctness of number of arguments.
    if (argc <= 2 || argc >= 4) {
        print_usage();
    }

    
    //Initializing pathes and checking whether they are directories.
    fs::path path_from = fs::path(argv[1]);
    fs::path path_where = fs::path(argv[2]);
    
    if (!fs::is_directory(path_from) || !fs::is_directory(path_where)) {
        std::cerr << "Error! One of pathes is incorrect.\n";
        print_usage();
    }
    
    
    //If everything is ok, then copying.
    //Making pathes canonical.
    path_from = fs::canonical(path_from);
    path_where = fs::canonical(path_where);
    //Creating directory with same name in where path.
    path_where += "/";
    path_where += path_from.filename();
    std::cout << path_where << std::endl;
    if (fs::create_directory(path_where)) {
        std::cout << "Created Directory: " <<  path_where << "\n";
    } else {
        std::cout << "Error: Couldn't create dir (maybe it is already existing)\n";
        print_usage();
    }
    /*
    std::cout << path_from << std::endl << path_from.root_name() << std::endl << path_from.root_directory() << std::endl << path_from.root_path() << std::endl << path_from.relative_path() << std::endl << path_from.parent_path() << std::endl << path_from.filename() << std::endl << path_from.stem() << std::endl << path_from.extension() << std::endl;    //Iterating through directory. */
    //Creating List of paths
    
    std::vector <fs::path> vec_of_paths_to_copy;
    copy(fs::recursive_directory_iterator(path_from), fs::recursive_directory_iterator(), std::back_inserter(vec_of_paths_to_copy));
    
    for (std::vector<fs::path>::const_iterator it(vec_of_paths_to_copy.begin()); it != vec_of_paths_to_copy.end(); it++) {
        std::cout << *it << std::endl;
    }
    
    
    
    
    
    return 0;
}