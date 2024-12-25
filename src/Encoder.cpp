#include "bmp.h"

int main()
{
    Bmp BMP;
    std::string pathl, paths;
    std::cout << "Please enter the load path:";
    std::cin >> pathl;
    std::cout << "Please enter the save path:";
    std::cin >> paths;
    pathl.erase(std::remove(pathl.begin(), pathl.end(), '\''), pathl.end());
    paths.erase(std::remove(paths.begin(), paths.end(), '\''), paths.end());
    if (!BMP.Load(pathl))
        return 0;
    if (!BMP.GetMessage())
        return 0;
    if (!BMP.process())
        return 0;
    if (!BMP.Save(paths))
        return 0;
    return 0;
}