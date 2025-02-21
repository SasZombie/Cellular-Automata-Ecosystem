#include <iostream>
#include "matrix.hpp"
#include <assert.h>

int main()
{
    // Tests

    sas::Matrix<int> m(2, 2);

    m(0, 0) = 4;
    assert(m(0, 0) == 4);

    try
    {
        m(4, 4) = 4;
        assert(false && "Should catch this exception\n");
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << '\n';
    }

    const sas::Matrix<int> m1(2, 2);

    assert(m1.getCols() == 2 && m1.getRows() == 2);

    m.setCols(3);

    assert(m.getCols() == 3 && m.getRows() == 2);

    m(1, 2) = 3;

    assert(m(1, 2) == 3);

    sas::Matrix<int> m3(4, 4);

    constexpr int test[4][4] = {
        {0, 1, 2, 3},
        {1, 2, 3, 4},
        {2, 3, 4, 5},
        {3, 4, 5, 6}};

    for (size_t i = 0; i < 4; ++i)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            m3(i, j) = i + j;
        }
    }

    for (size_t i = 0; i < 4; ++i)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            assert(m3(i, j) == test[i][j]);
        }
    }

    for (const auto &elem : m3)
    {
        std::cout << elem;
    }

    std::cout << '\n';
    m3.setRows(5);

    for (const auto &elem : m3)
    {
        std::cout << elem;
    }

    std::cout << '\n';

    m = m3;

    for (const auto &elem : m)
    {
        std::cout << elem;
    }

    m3.setCols(2);

    std::cout << '\n';
    for (const auto &elem : m3)
    {
        std::cout << elem;
    }

    std::cout << '\n';

    for (const auto &elem : m)
    {
        std::cout << elem;
    }

    sas::Matrix<int> m5(5, 5);

    for (auto &elem : m)
    {
        elem = 1;
    }

    sas::Matrix<int> m6 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}};

    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            std::cout << m6(i, j) << ' ';
        }
        std::cout << '\n';
    }

    std::cout << '\n';
    // Supports both

    sas::Matrix<int> m7(5, 5);

    for(size_t i = 0; i < 5; ++i)
    {
        for(size_t j = 0; j < 5; ++j)
        {
            m7(i, j) = 1;
        }
    }
    
    for(const auto& elem : m7)
    {
        std::cout << elem;
    }

    std::cout << "M6 rows = and Cols \n";
    std::cout << m6.getRows() << ' ' << m6.getCols() <<'\n';

    const auto &[nw, n, ne, e, se, s, sw, w] = m6.getNeighbours(2, 3);
    const auto neigh = m6.getNeighbours(1, 1);

    for (const auto &elem : neigh)
    {
        std::cout << elem.value_or(-1) << ' ';
    }


    std::cout << '\n';

    std::cout << nw.value_or(-1) << ' ' << n.value_or(-1) << ' ' << ne.value_or(-1) << '\n'; 
    std::cout << e.value_or(-1) << ' ' << se.value_or(-1) << ' ' << s.value_or(-1) << '\n'; 
    std::cout << sw.value_or(-1) << ' ' << w.value_or(-1); 
}