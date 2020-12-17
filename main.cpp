#include <iostream>
#include "img_filters.h"



int main( int argc, char *argv[] )
{
    // toolkit filter_name base_pic_name sudent_tool student_pic_name limitPix limitMSE
    // toolkit near test images!
    
    try
    {
        if (argc != 4)
            throw "Not enough arguments";

        ImgPng img; 
        if (!img.load(argv[2]))
            throw ("File" + std::string(argv[2]) + "not found").c_str();

        config cfg;
        cfg.readCfg(argv[1]);
        Filters f = Filters();
        f.setCfg(cfg); 
        f.applyTo(img);
        img.save(argv[3]);
    }
    catch (const char *str)
    {
        std::cout << "Error: " << str << std::endl;
        return 1;
    }
    return 0;
}
 