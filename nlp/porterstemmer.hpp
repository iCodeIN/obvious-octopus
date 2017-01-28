#ifndef PORTERSTEMMER_HPP
#define PORTERSTEMMER_HPP

#include "nlp/istemmer.hpp"
#include "util/stringutils.hpp"

namespace nlp
{
    /*! A later stemmer written by Martin Porter and published in the July 1980 issue of the journal Program.
        This stemmer was very widely used and became the de facto standard algorithm used for English stemming.
        Dr. Porter received the Tony Kent Strix award in 2000 for his work on stemming and information retrieval.
     */
    class PorterStemmer : public IStemmer
    {
        public:

            /*! default constructor
             */
            explicit PorterStemmer() = default;

            /*! default destructor
             */
            virtual ~PorterStemmer() = default;

            //! --- IStemmer ---
            virtual std::string stem(const std::string& s) const
            {
                auto retval = std::string(s);
                if(this->isSet(IGNORE_CASE))
                {
                    retval = util::string::toUpper(retval);
                }

                // steps of the porter stemmer
                step1a(retval);
                step1b(retval);
                step1c(retval);

                step2(retval);

                step3(retval);

                step4(retval);

                // return
                return retval;
            }

        private:
            // --- methods ---

            int measure(const std::string& s) const
            {
                std::vector<bool> pattern;
                for(int i=0; i<s.length(); i++)
                {
                    bool c = consonant(s, i);
                    if(pattern.size() == 0 || pattern[pattern.size()-1] != c)
                    {
                        pattern.push_back(c);
                    }
                }
                int m = 0;
                int p = 0;
                while(p < s.length() && pattern[p])
                {
                    p++;
                }
                while(p + 1 < s.length() && !pattern[p] && pattern[p])
                {
                    p+=2;
                    m++;
                }
                return m;
            }

            bool consonant(const std::string& s, int p) const
            {
                if(s[p] == 'Y' || s[p]=='y')
                {
                    return p == 0 ? true : !consonant(s, p - 1);
                }
                else
                {
                    return  s[p] != 'A' && s[p] != 'a' &&
                            s[p] != 'E' && s[p] != 'e' &&
                            s[p] != 'I' && s[p] != 'i' &&
                            s[p] != 'O' && s[p] != 'o' &&
                            s[p] != 'U' && s[p] != 'u';
                }
            }

            bool hasVowel(const std::string& s) const
            {
                for(int i=0; i<s.length(); i++)
                {
                    if(!consonant(s, i))
                    {
                        return true;
                    }
                }
                return false;
            }

            void step1a(std::string& s) const
            {
                // SSES -> SS
                if(util::string::endsWith(s, "SSES"))
                {
                    s = s.substr(0, s.length() - 4) + "SS";
                }
                // IES -> I
                else if(util::string::endsWith(s, "IES"))
                {
                    s = s.substr(0, s.length() - 3) + "I";
                }
                // SS -> 'SS'
                else if(util::string::endsWith(s, "SS"))
                {
                    // nothing
                }
                // S -> ''
                else if(util::string::endsWith(s, "S"))
                {
                    s = s.substr(0, s.length() - 1);
                }
            }

            void step1b(std::string& s) const
            {
                // (m>0) EED -> EE
                if(measure(s) > 0 && util::string::endsWith(s, "EED"))
                {
                    s = s.substr(0, s.length() - 3) + "EE";
                }
                // (*v*) ED  -> ''
                // (*v*) ING -> ''
                if(
                    (util::string::endsWith(s, "ED") && hasVowel(s.substr(0, s.length() - 2))) ||
                    (util::string::endsWith(s, "ING") && hasVowel(s.substr(0, s.length() - 3)))
                )
                {
                    if(util::string::endsWith(s, "ED"))
                    {
                        s = s.substr(0, s.length() - 2);
                    }
                    else
                    {
                        s = s.substr(0, s.length() - 3);
                    }

                    // AT -> ATE
                    // BL -> BLE
                    // IZ -> IZE
                    if(util::string::endsWith(s, "AT") || util::string::endsWith(s, "BL") || util::string::endsWith(s, "IZ"))
                    {
                        s += "E";
                    }

                    // (*d) AND NOT (*L or *S or *Z) -> single letter

                    // (m==1) AND *O -> E
                }
            }

            void step1c(std::string& s) const
            {
                // (*v*) Y -> I
                if(util::string::endsWith(s, "Y") && hasVowel(s.substr(0, s.length() - 1)))
                {
                    s = s.substr(0, s.length() - 1) + "I";
                }
            }

            void step2(std::string& s) const
            {
                if(measure(s) == 0)
                {
                    return;
                }
                // ATIONAL  -> ATE
                if(util::string::endsWith(s, "ATIONAL"))
                {
                    s = s.substr(0, s.length() - 7) + "ATE";
                }
                // TIONAL   -> TION
                if(util::string::endsWith(s, "TIONAL"))
                {
                    s = s.substr(0, s.length() - 6) + "TION";
                }
                // ENCI     -> ENCE
                if(util::string::endsWith(s, "ENCI"))
                {
                    s = s.substr(0, s.length() - 4) + "ENCE";
                }
                // ANCI     -> ANCE
                if(util::string::endsWith(s, "ANCI"))
                {
                    s = s.substr(0, s.length() - 4) + "ANCE";
                }
                // IZER     -> IZE
                if(util::string::endsWith(s, "IZER"))
                {
                    s = s.substr(0, s.length() - 4) + "IZE";
                }
                // ABLI     -> ABLE
                if(util::string::endsWith(s, "ABLI"))
                {
                    s = s.substr(0, s.length() - 4) + "ABLE";
                }
                // ALLI     -> AL
                if(util::string::endsWith(s, "ALLI"))
                {
                    s = s.substr(0, s.length() - 4) + "AL";
                }
                // ENTLI    -> ENT
                if(util::string::endsWith(s, "ENTLI"))
                {
                    s = s.substr(0, s.length() - 5) + "ENT";
                }
                // ELI      -> E
                if(util::string::endsWith(s, "ELI"))
                {
                    s = s.substr(0, s.length() - 3) + "E";
                }
                // OUSLI    -> OUS
                if(util::string::endsWith(s, "OUSLI"))
                {
                    s = s.substr(0, s.length() - 5) + "OUS";
                }
                // IZATION  -> IZE
                if(util::string::endsWith(s, "IZATION"))
                {
                    s = s.substr(0, s.length() - 7) + "IZE";
                }
                // ATION    -> ATE
                if(util::string::endsWith(s, "ATION"))
                {
                    s = s.substr(0, s.length() - 5) + "ATE";
                }
                // ATOR     -> ATE
                if(util::string::endsWith(s, "ATOR"))
                {
                    s = s.substr(0, s.length() - 4) + "ATE";
                }
                // ALISM    -> AL
                if(util::string::endsWith(s, "ALISM"))
                {
                    s = s.substr(0, s.length() - 5) + "AL";
                }
                // IVENESS  -> IVE
                if(util::string::endsWith(s, "IVENESS"))
                {
                    s = s.substr(0, s.length() - 7) + "IVE";
                }
                // FULNESS  -> FUL
                if(util::string::endsWith(s, "FULNESS"))
                {
                    s = s.substr(0, s.length() - 7) + "FUL";
                }
                // OUSNESS  -> OUS
                if(util::string::endsWith(s, "OUSNESS"))
                {
                    s = s.substr(0, s.length() - 7) + "OUS";
                }
                // ALITI    -> AL
                if(util::string::endsWith(s, "ALITI"))
                {
                    s = s.substr(0, s.length() - 5) + "AL";
                }
                // IVITI    -> IVE
                if(util::string::endsWith(s, "IVITI"))
                {
                    s = s.substr(0, s.length() - 5) + "IVE";
                }
                // BILITI   -> BLE
                if(util::string::endsWith(s, "BILITI"))
                {
                    s = s.substr(0, s.length() - 6) + "BLE";
                }
            }

            void step3(std::string& s) const
            {
                if(measure(s) == 0)
                {
                    return;
                }
                // ICATE    -> IC
                if(util::string::endsWith(s, "ICATE"))
                {
                    s = s.substr(0, s.length() - 5) + "IC";
                }
                // ATIVE    -> ''
                if(util::string::endsWith(s, "ATIVE"))
                {
                    s = s.substr(0, s.length() - 5);
                }
                // ALIZE    -> AL
                if(util::string::endsWith(s, "ALIZE"))
                {
                    s = s.substr(0, s.length() - 5) + "AL";
                }
                // ICITI    -> IC
                if(util::string::endsWith(s, "ICITI"))
                {
                    s = s.substr(0, s.length() - 5) + "IC";
                }
                // ICAL     -> IC
                if(util::string::endsWith(s, "ICAL"))
                {
                    s = s.substr(0, s.length() - 4) + "IC";
                }
                // FUL      -> ''
                if(util::string::endsWith(s, "FUL"))
                {
                    s = s.substr(0, s.length() - 3);
                }
                // NESS     -> ''
                if(util::string::endsWith(s, "NESS"))
                {
                    s = s.substr(0, s.length() - 4);
                }
            }

            void step4(std::string& s) const
            {
                if(measure(s) <= 1)
                {
                    return;
                }
                // AL
                if(removeSuffix(s, "AL"))
                {
                    return;
                }
                // ANCE
                if(removeSuffix(s, "ANCE"))
                {
                    return;
                }
                // ENCE
                if(removeSuffix(s, "ENCE"))
                {
                    return;
                }
                // ER
                if(removeSuffix(s, "ER"))
                {
                    return;
                }
                // IC
                if(removeSuffix(s, "IC"))
                {
                    return;
                }
                // ABLE
                if(removeSuffix(s, "ABLE"))
                {
                    return;
                }
                // IBLE
                if(removeSuffix(s, "IBLE"))
                {
                    return;
                }
                // ANT
                if(removeSuffix(s, "ANT"))
                {
                    return;
                }
                // EMENT
                if(removeSuffix(s, "EMENT"))
                {
                    return;
                }
                // MENT
                if(removeSuffix(s, "MENT"))
                {
                    return;
                }
                // ENT
                if(removeSuffix(s, "ENT"))
                {
                    return;
                }
                // TION
                if(removeSuffix(s, "TION"))
                {
                    return;
                }
                // SION
                if(removeSuffix(s, "SION"))
                {
                    return;
                }
                // OU
                if(removeSuffix(s, "OU"))
                {
                    return;
                }
                // ISM
                if(removeSuffix(s, "ISM"))
                {
                    return;
                }
                // ATE
                if(removeSuffix(s, "ATE"))
                {
                    return;
                }
                // ITI
                if(removeSuffix(s, "ITI"))
                {
                    return;
                }
                // OUS
                if(removeSuffix(s, "OUS"))
                {
                    return;
                }
                // IVE
                if(removeSuffix(s, "IVE"))
                {
                    return;
                }
                // IZE
                if(removeSuffix(s, "IZE"))
                {
                    return;
                }
            }

            void step5(std::string& s) const
            {

            }

            bool removeSuffix(std::string& s, const std::string& suffix) const
            {
                if(util::string::endsWith(s, suffix))
                {
                    s = s.substr(0, s.length() - suffix.length());
                    return true;
                }
                return false;
            }
            // --- members ---
    };
}

#endif // PORTERSTEMMER_HPP