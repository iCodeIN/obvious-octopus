#pragma once
#ifndef NUMERIC_DFT_HPP
#define NUMERIC_DFT_HPP

#include <algorithm>
#include <complex>
#include <vector>

namespace numeric
{
    /*! The discrete Fourier transform (DFT) converts a finite sequence of equally
      	 spaced samples of a function into the list of coefficients of a finite
    	 combination of complex sinusoids, ordered by their frequencies, that has those
     	 same sample values. It can be said to convert the sampled function from its
     	 original domain (often time or position along a line) to the frequency domain.
     */
    class DFT final
    {
        public:
            /*! Prohibit construction of DFT. This class offers only static methods.
             */
            explicit DFT() = delete;

            /*! Apply the discrete Fourier transform.
             */
            static std::vector<std::complex<double>> forwardDFT(const std::vector<double> &xs)
            {
                // note: this code is not optimised at all, written for clarity not speed.
                int N = xs.size();
                std::vector<std::complex<double>>  ys;
                for (int k = 0; k <= N/2; ++k)
                {
                    double re = 0;
                    double im = 0;
                    for (int x = 0; x < N; ++x)
                    {
                        re += xs[x] * cos(2 * M_PI / N * k * x);
                        im += xs[x] * sin(2 * M_PI / N * k * x);
                    }
                    // normalization
                    re *= 2. / N;
                    im *= 2. / N;
                    // insert
                    ys.push_back(std::complex<double>(re,im));
                }
                return ys;
            }

            /*! Apply the inverse discrete Fourier transform.
                The inverse Fourier transformation, also called Fourier synthesis, of a
                frequency domain representation combines the contributions of all the different
                frequencies to recover the original function of time.
             */
            static std::vector<double> inverseDFT(const std::vector<std::complex<double>> &xs)
            {
                // note: this code is not optimised at all, written for clarity not speed.
                int N = (xs.size() - 1) * 2;
                std::vector<double> ys;
                for (int x = 0; x < N; ++x)
                {
                    double y = real(xs[0]);
                    for (int k = 1; k <= N / 2; ++k)
                    {
                        y += real(xs[k]) * cos(2 * M_PI / N * k * x) + imag(xs[k]) * sin(2 * M_PI / N * k * x);
                    }
                    ys.push_back(y);
                }
                return ys;
            }

            /*! Apply the discrete Fourier transform.
                Furthermore, this method also calculates the frequency (in terms of the original
                original input-domain), and sorts the frequencies based on the modulus of their
                corresponding Fourier-coefficients.
             */
            static std::vector<double> sortedDFT(const std::vector<double> &xs)
            {
                // perform DFT
                auto frq = forwardDFT(xs);

                // calculate corresponding amplitudes
                std::vector<std::pair<double,double>> amp;
                for(int i=0; i<frq.size(); i++)
                {
                    double a = real(frq[i]) * real(frq[i]) + imag(frq[i]) * imag(frq[i]);
                    double f = i == 0 ? 0.0 : (xs.size() / (i + 0.0));
                    amp.push_back(std::make_pair(f,a));
                }

                // sort
                std::sort(amp.begin(), amp.end(), [](const std::pair<double,double> &left, const std::pair<double,double> &right)
                {
                    return left.second < right.second;
                });

                // return
                std::vector<double> ys;
                for(int i=amp.size()-1; i>=0; i--)
                {
                    ys.push_back(amp[i].first);
                }
                return ys;
            }

    };
}

#endif // NUMERIC_DFT_HPP
