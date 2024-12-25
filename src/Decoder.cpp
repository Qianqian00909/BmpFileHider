#include "bmp.h"

int main()
{
    Bmp BMP;
    std::string pathl;
    std::cout<<"Please enter the load path:";
    std::cin>>pathl;
    pathl.erase(std::remove(pathl.begin(), pathl.end(), '\''), pathl.end());
    if(!BMP.Load(pathl)) return 0;
    if(!BMP.extract()) return 0;
    return 0;
}