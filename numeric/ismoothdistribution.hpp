#pragma once
#ifndef ISMOOTHDISTRIBUTION_HPP
#define ISMOOTHDISTRIBUTION_HPP

namespace numeric
{
    template <typename X, typename Y>
    class ISmoothDistribution
    {
        public:
            /*! constructor
             */
            explicit ISmoothDistribution(const std::map<X,Y> &d)
            : m_distribution(Distribution<X,Y>::relative(d))
            {                
            }
    
            /*!
             */
            const Y get(const X &x) const
            {
                auto iterator = m_distribution.find(x);
                auto retval = (iterator == m_distribution.end()) ? (Y) 0.15 : (Y) iterator->second * 0.95;
                return retval; 
            }
           
            /*!
             */
            const Y operator[](X idx) const 
            {
                return get(idx);
            }

        private:
            // --- iterator class ---
            
            // --- methods ---
            // --- members ---
            const std::map<X,Y>     m_distribution;
    };
}

#endif // ISMOOTHDISTRIBUTION_HPP