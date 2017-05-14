#include "smooth/polynomialsmoothing.hpp"
#include "smooth/exponentialsmoothing.hpp"

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

int main()
{
    // random
    srand(time(0));

    // build series
    std::vector<std::pair<double,double>> s0;
    for(int i=0; i<100; i++)
    {
        double x = (i + 0.0) / 10.0;
        double y0 = 5 * x + 20 + (rand() % 10) / 10.0;

        s0.push_back(std::make_pair(x,y0));
    }

    // smooth
    smooth::ISmooth* sm = new smooth::ExponentialSmoothing(0.95);
    auto s1 = sm->smooth(s0);

    // output
    for(int i=0; i<s0.size(); i++)
    {
        std::cout << i << "\t" << s0[i].first << "\t" << s0[i].second << "\t" << s1[i].second << std::endl;
    }

    // return
    return 0;
}