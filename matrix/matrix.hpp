#pragma once 

#include <iostream>
#include <vector>
 
namespace ytr
{
    template <typename T>
    static std::vector<std::vector<T> > matrixSum(std::vector<std::vector<T> > matrix1, std::vector<std::vector<T> > matrix2)
    {
        std::vector<std::vector<T> > newMatrix;
 
        for (size_t i = 0; i < matrix1.size(); i++)
        {
            std::vector<T> tmp;
            for (size_t j = 0; j < matrix1[i].size(); j++)
            {
                tmp.push_back(matrix1[i][j] + matrix2[i][j]);
            }
            newMatrix.push_back(tmp);
            tmp.clear();
        }
        return (newMatrix);
    }
}
