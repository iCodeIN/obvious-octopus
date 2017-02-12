#pragma once
#ifndef SVG_PATH_HPP
#define SVG_PATH_HPP

#include "svg/ielement.hpp"

#include <assert.h>

namespace SVG
{
    /*! Defines a path.
     */
    class Path final : public IElement
    {
        public:

            /*! define a tuple containing a single command that belongs in an SVG path
                - M moveto
                - L lineto
                - H horizontal lineto
                - V vertical lineto
                - C curveto
                - S smooth curveto
                - Q quadratic bézier curveto
                - T smooth quadratic bézier curveto
                - A elliptical arc
                - Z closepath
             */
            using PathCommand = std::tuple<char, std::vector<double>>;

            /*! construct a Path from an std::vector of PathCommands
             */
            explicit Path(std::vector<PathCommand> cmds)
            {
                for(int i=0; i<cmds.size(); i++)
                {
                    auto type = std::get<0>(cmds[i]);
                    auto args = std::get<1>(cmds[i]);
                    // check correct number of arguments
                    assert(((type=='m' || type=='M') && args.size()==2) ||   // moveto
                           ((type=='l' || type=='L') && args.size()==2) ||   // lineto

                           ((type=='h' || type=='H') && args.size()==1) ||   // horizontal lineto
                           ((type=='v' || type=='V') && args.size()==1) ||   // vertical lineto

                           ((type=='c' || type=='C') && args.size()==2) ||   // curveto
                           ((type=='s' || type=='S') && args.size()==2) ||   // smooth curveto

                           ((type=='q' || type=='Q') && args.size()==2) ||   // quadratic bézier curveto
                           ((type=='t' || type=='T') && args.size()==2) ||   // smooth quadratic bézier curveto

                           ((type=='a' || type=='A') && args.size()==2) ||   // elliptical arc

                           ((type=='z' || type=='Z') && args.size()==0)      // closepath
                          );
                    // check whether arguments make sense
                    for(int j=0; j<args.size(); j++)
                    {
                        assert(args[j] >= 0);
                    }
                }
            }

            /*! Default destructor
             */
            virtual ~Path() = default;

            /*! Prohibit const copy constructor
             */
            Path(const Path&) = delete;

            /*! Prohibit copy constructor
             */
            Path(Path&) = delete;

            /*! Prohibit const assignment operator
             */
            void operator=(const Path&) = delete;

            /*! Prohibit assignment operator
             */
            void operator=(Path&) = delete;

        protected:
            // -- IElement ---
            virtual std::unique_ptr<XML::IElement> toXML() const
            {
                auto node = IElement::toXML();
                node.get()->setName("path");

                auto s = "";
                for(auto cmd : m_pathCommands)
                {
                    s += std::get<0>(cmd);
                    for(auto coord : std::get<1>(cmd))
                    {
                        s += toString(coord) + " ";
                    }
                }

                node.get()->setAttribute("d", s);
                return node;
            }
        private:
            // --- methods ---
            std::string toString(int a)
            {
                std::ostringstream os;
                os << a;
                return os.str();
            }

            // --- members ---
            std::vector<PathCommand> m_pathCommands;
    };
}

#endif // SVG_PATH_HPP
