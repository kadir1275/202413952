#include "matrix.hpp"

int main()
{
    std::vector<std::vector<int>> berke;
    std::vector<int> tmp;

    tmp.push_back(1);
    tmp.push_back(2);

    berke.push_back(tmp);
    tmp.clear();

    tmp.push_back(3);
    tmp.push_back(4);

    berke.push_back(tmp);
    tmp.clear();

    
    std::vector<std::vector<int>> enes;

    tmp.push_back(5);
    tmp.push_back(6);

    enes.push_back(tmp);
    tmp.clear();

    tmp.push_back(7);
    tmp.push_back(8);

    enes.push_back(tmp);
    tmp.clear();
    
    std::vector<std::vector<int>> toplam;
    toplam = ytr::matrixSum(enes,berke);
    std::cout<<toplam[0][0]<< std::endl;
    std::cout<<toplam[0][1]<< std::endl;
    std::cout<<toplam[1][0]<< std::endl;
    std::cout<<toplam[1][1]<< std::endl;


}