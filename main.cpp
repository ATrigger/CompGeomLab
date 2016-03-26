#include <stddef.h>
#include <fstream>
#include "gmpxx.h"
#include <boost/numeric/interval.hpp>
#include <vector>
#include "tests.h"

#define x first
#define y second
typedef std::pair<double, double> point;
using namespace boost::numeric::interval_lib::compare::certain;
typedef boost::numeric::interval<double> intd_t;

inline bool bounding_box(double a, double b, double c, double d) {
    if (a > b) swap(a, b);
    if (c > d) swap(c, d);
    return max(a, c) <= min(b, d);
}

struct line {
    line(std::vector<double> coords)
            : start(coords[0], coords[1]), ending{coords[2], coords[3]} { }

    point start, ending;
};

int PredicateOfTurn(line l, point c) {
    intd_t bxmax = l.ending.x;
    intd_t bymay = l.ending.y;
    intd_t cxmax = c.x;
    intd_t cymay = c.y;
    bxmax -= l.start.x;
    bymay -= l.start.y;
    cxmax -= l.start.x;
    cymay -= l.start.y;

    intd_t intervalTurn = bxmax * cymay - bymay * cxmax;


    if (intervalTurn > 0.) return 1;
    if (intervalTurn < 0.) return -1;

    mpq_class bx{l.ending.x};
    mpq_class ax{l.start.x};
    mpq_class by{l.ending.y};
    mpq_class ay{l.start.y};
    mpq_class cx{c.x};
    mpq_class cy{c.y};
    bx -= ax;
    by -= ay;
    cx -= ax;
    cy -= ay;
    mpq_class gmpTurn{bx * cy - by * cx};
    return mpq_sgn(gmpTurn.__get_mp());
}

int main() {
    auto fin = new std::fstream("input.txt", std::fstream::in);
    auto fout = new std::fstream("output.txt", std::fstream::out | std::fstream::trunc);
    int test_id;
    fin->operator>>(test_id);
    vector<double> in = genTest(test_id);
    for (int i = 0; i < in.size() / 8; i++) {

        line AB{{in.begin() + i * 8, in.begin() + i * 8 + 4}};
        line CD{{in.begin() + i * 8 + 4, in.begin() + i * 8 + 8}};

        int t1 = PredicateOfTurn(AB, CD.start);
        int t2 = PredicateOfTurn(AB, CD.ending);
        int t3 = PredicateOfTurn(CD, AB.start);
        int t4 = PredicateOfTurn(CD, AB.ending);

        if (t1 * t2 > 0) {
            *fout << "N";
            continue;
        }

        if (t3 * t4 > 0) {
            *fout << "N";
            continue;
        }

        if (t1 == 0 && t2 == 0 && t3 == 0 && t4 == 0) {
            if ((bounding_box(AB.start.x, AB.ending.x, CD.start.x, CD.ending.x))
                && (bounding_box(AB.start.y, AB.ending.y, CD.start.y, CD.ending.y))) {
                *fout << "Y";
                continue;
            } else {
                *fout << "N";
                continue;
            }
        }

        *fout << "Y";
    }
    delete fin;
    delete fout;
    return 0;
}