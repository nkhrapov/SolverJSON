#ifndef JSON_SERIALIZER_HPP
#define JSON_SERIALIZER_HPP

#include <limits>
#include <vector>
//#include <bags/bfsbag.hpp>
//#include <cutfact/lbcutfact/lpzbndsupp.hpp>
#include <cutfact/lbcutfact/recordsupp.hpp>
//#include <cutfact/lbcutfact/lbcutfactory.hpp>
//#include <applycut/basic/serialcutapp.hpp>
#include <decomp/bisectdecomp.hpp>
//#include <oneobj/contboxconstr/dejong.hpp>
#include <solver/basesolver.hpp>

#include <libjson/libjson.h>
//#include "JSONSerializer.hpp"

#include <string>
template <typename FT> class JSONSerializer{
public:
    std::string JSONSerialize(void *solverPtr, void *rsPtr, void *subPtr);
};

template <typename FT>
std::string JSONSerializer<FT>::JSONSerialize(void *solverPtr, void *rsPtr, void *subPtr){

    NUC::BaseSolver<FT> &solver = *(NUC::BaseSolver<FT>*)solverPtr;
    NUC::RecordSupplier<FT> &rs = *(NUC::RecordSupplier<FT>*)rsPtr;
    NUC::Sub<FT> &sub = *(NUC::Sub<FT>*)subPtr;

    JSONNode outJSONNode(JSON_NODE);
    JSONNode recordJSONNode(JSON_NODE);
    recordJSONNode.set_name("record");

    recordJSONNode.push_back(JSONNode("x",rs.getBound(sub.mBox)));
    recordJSONNode.push_back(JSONNode("v","wah-wah"));

    JSONNode subproblemsJSONNode(JSON_ARRAY);
    subproblemsJSONNode.set_name("subproblems");

    outJSONNode.push_back(recordJSONNode);

    auto ttf = [&] (const NUC::Sub<FT>& s) {
/*        std::cout << "Sub: ";
        std::cout << "Score = " << s.mScore;
        std::cout << ", Layer = " << s.mLayer;
        std::cout << ", Box = " << snowgoose::BoxUtils::toString(s.mBox) << "\n";
*/
        JSONNode subproblemJSONNode(JSON_NODE);
        JSONNode aJSONNode(JSON_ARRAY);
        aJSONNode.set_name("a");
        JSONNode bJSONNode(JSON_ARRAY);
        bJSONNode.set_name("b");

        for (int i = 0; i < s.mBox.mDim; i++){
            aJSONNode.push_back(JSONNode("",s.mBox.mA[i]));
            bJSONNode.push_back(JSONNode("",s.mBox.mB[i]));
        }
        subproblemJSONNode.push_back(aJSONNode);
        subproblemJSONNode.push_back(bJSONNode);
        subproblemsJSONNode.push_back(subproblemJSONNode);

//        std::cout << "\nData For JSON: " << s.mBox.mA[0] << "   " << s.mBox.mB[0] << "\n";
    };

    solver.getBag().traverse(ttf);

    outJSONNode.push_back(subproblemsJSONNode);

    std::string outJSONString = outJSONNode.write_formatted();
//    std::cout <<"OUTPUT JSON:\n" << outJSONString << std::endl;
    return outJSONString;

}
#endif /*JSON_SERIALIZER_HPP*/
