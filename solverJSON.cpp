#include <limits>
#include <vector>
#include <bags/bfsbag.hpp>
#include <cutfact/lbcutfact/lpzbndsupp.hpp>
#include <cutfact/lbcutfact/recordsupp.hpp>
#include <cutfact/lbcutfact/lbcutfactory.hpp>
#include <applycut/basic/serialcutapp.hpp>
#include <decomp/bisectdecomp.hpp>
#include <oneobj/contboxconstr/dejong.hpp>


#include <solver/basesolver.hpp>
#include <libjson/libjson.h>
#include "JSONSerializer.hpp"

bool stopper(const NUC::BaseSolver<double>& solver) {
    static int cnt = 0;
    const int maxCnt = 5;
    if (cnt++ > maxCnt) {
        return true;
    } else {
        return false;
    }
}

int main() {
    const int n = 2;
    const double L = 1;
    const double eps = 0.001;

    // Setup problem
    OPTITEST::DejongProblemFactory fact(n, -2, 4);
    COMPI::MPProblem<double> *mpp = fact.getProblem();

    //Setup bag of sub problems
    NUC::Sub<double> sub(0, std::numeric_limits<double>::max(), *(mpp->mBox));
    NUC::BFSBag<double> bag;
    bag.putSub(sub);

    //Setup Cut Factory
    NUC::RecordSupplier<double> rs(std::numeric_limits<double>::max());
    COMPI::Functor<double>* pf = mpp->mObjectives.at(0);
    NUC::LpzBoundSupplier<double> lplb(n, L, *pf);
    NUC::LBCutFactory<double> cf(eps, rs, lplb);

    // Setup decomposer
    NUC::BisectDecomp<double> bisdec;
    // Setup cut applicator 
    NUC::SerialCutApplicator<double> cutapp;
    // Setup solver
    NUC::BaseSolver<double> solver(bag, bisdec, cf, cutapp);

    // Set stopper for solver
    solver.setStopper(stopper);
    //Setup step watchers
    auto tf = [](const NUC::Sub<double>& s,
            const std::vector<std::shared_ptr <NUC::Cut <double> > >&,
            const std::vector< snowgoose::Box<double> >&,
            const NUC::BaseSolver<double>& slv) {
        std::cout << "Sub: \n";
        std::cout << "Score = " << s.mScore;
        std::cout << "\n Layer = " << s.mLayer;
        std::cout << "\n Box = " << snowgoose::BoxUtils::toString(s.mBox) << "\n";
    };
    solver.addStepWatcher(tf);

    double x[n];
    //Setup sub evaluators
    auto sf = [&](NUC::Sub<double>& s) {
        snowgoose::BoxUtils::getCenter(s.mBox, x);
        double v = pf->func(x);
        rs.updateRv(v);
        double r = snowgoose::BoxUtils::radius(s.mBox);
        s.mScore = v - r * L;
    };
    solver.addSubEval(sf);

    // Run solver
    solver.solve();

    std::cout << "Best value found: " << rs.getBound(sub.mBox) << "\n";

    std::cout << "\n\n**************\n*************\n************\nJSON OUTPUT:\n";
    JSONSerializer jsonser;
    std::string js = jsonser.JSONSerialize(&solver, &rs, &sub);
    std::cout << js << "\n";

    return 0;
}


