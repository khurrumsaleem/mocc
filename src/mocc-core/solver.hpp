#pragma once

#include <memory>

#include "transport_sweeper.hpp"
#include "output_interface.hpp"

namespace mocc {
    /**
    * This provides a virtual base type, which shall provide a solve() and
    * step() method. At the highest level of the heirarchy, the driver calls
    * solve() and that should invoke everything that is necessary.
    */
    class Solver: public HasOutput {
    public:
        virtual ~Solver() { };

        /**
        * Perform a full solution to the class of problem that the most-derived
        * Solver type is designed to solve. This is usually called upon the top-
        * level Solver by the driver.
        */
        virtual void solve()=0;

        /**
        * Perfom some sort of intermediate step in solving the problem of
        * interest, typically as part of another solver. What specifically is
        * done is quite solver specific, so check the derived class to see what
        * it does for a specific case.
        */
        virtual void step()=0;

        /**
        * Return a pointer to a transport sweeper object. If the Solver does
        * not actually have a sweeper, return nullptr.
        */
        virtual const TransportSweeper* sweeper() const {
            return nullptr;
        }
    private:
    };

    typedef std::shared_ptr<Solver> SP_Solver_t;
}
