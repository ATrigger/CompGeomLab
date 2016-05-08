//
// Created by petrovich on 08.05.16.
//

#include <algorithm>
#include <set>
#include <vector>
#include <stdio.h>
#include <cmath>
#include <functional>
#include <ios>

struct point {
    double x=0,y=0;
    point(){}
    point(double x, double y):x(x),y(y){}
};
struct segment {
    point start,notstart;
    int num;
    double get_y(double x) const {
        if(std::fabs(start.x - notstart.x) < 1e-9) return start.y;
        return start.y + (notstart.y- start.y) * (x - start.x)/(notstart.x-start.x);
    }
    bool less(const segment& b)const{
        double x = std::max(std::min(start.x,notstart.x),std::min(b.start.x,b.notstart.x));
        return get_y(x) < b.get_y(x) - 1e-9;
    }
};
template <typename T> int sgn(T val) {
    return (T(1e-9) < val) - (val < T(-1e-9));
}
int turn(const point & a, const point & b, const point & c) {
    return sgn((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
}
inline bool bounding_box(double a, double b, double c, double d) {
    if (a > b) std::swap(a, b);
    if (c > d) std::swap(c, d);
    return std::max(a, c) <= std::min(b, d) + 1e-9;
}
bool simpleintersex(const segment&a, const segment&b){
    return bounding_box(a.start.x, a.notstart.x, b.start.x, b.notstart.x)
    && bounding_box(a.start.y, a.notstart.y, b.start.y, b.notstart.y)
    && turn(a.start, a.notstart, b.start) * turn(a.start, a.notstart, b.notstart) <= 0
    && turn(b.start, b.notstart, a.start) * turn(b.start, b.notstart, a.notstart) <= 0;
}
bool operator<(const segment&a ,const segment&b){
    return a.less(b);
}
std::set<segment> segments;
std::vector<decltype(segments.begin())> where;
struct event{
    double x;
    int tp, id;
    event(){}
    event(double x, int tp, int id):x(x),tp(tp),id(id){}

    bool operator<(const event &e){
        if(std::fabs(x - e.x) > 1e-9) return x < e.x;
        return tp> e.tp;
    }
};
std::pair<int,int> answer(const std::vector<segment> &a){
    //http://neerc.ifmo.ru/wiki/index.php?title=%D0%9F%D0%B5%D1%80%D0%B5%D1%81%D0%B5%D1%87%D0%B5%D0%BD%D0%B8%D0%B5_%D0%BC%D0%BD%D0%BE%D0%B6%D0%B5%D1%81%D1%82%D0%B2%D0%B0_%D0%BE%D1%82%D1%80%D0%B5%D0%B7%D0%BA%D0%BE%D0%B2
    int n = a.size();
    auto myprev = [&segments](decltype(segments.begin()) iter) -> decltype(segments.begin()) {
      if(iter == segments.begin()) return segments.end();
        else return --iter;
    };
    auto dupiter = [](decltype(segments.begin()) iter) -> decltype(segments.begin()) {
        return ++iter;
    };
    std::vector<event> e;
    for (int i = 0; i < n; i++) {
        e.emplace_back(std::min(a[i].start.x, a[i].notstart.x), +1, i);
        e.emplace_back(std::max(a[i].start.x, a[i].notstart.x), -1, i);
    }
    std::sort(e.begin(), e.end());

    segments.clear();
    where.resize(a.size());
    for (size_t i = 0; i < e.size(); i++) {
        int id = e[i].id;
        if (e[i].tp == 1) {
            auto nxt = segments.lower_bound(a[id]), prv = myprev(nxt);
            if (nxt != segments.end() && simpleintersex (*nxt, a[id])) {
                return {nxt->num, id+1};
            }
            if (prv != segments.end() && simpleintersex (*prv, a[id])) {
                return {prv->num, id+1};
            }
            where[id] = segments.insert(nxt, a[id]);
        } else {
            auto nxt = dupiter(where[id]), prv = myprev(where[id]);
            if (nxt != segments.end() && prv != segments.end() && simpleintersex(*nxt, *prv)) {
                return {prv->num, nxt->num};
            }
            segments.erase(where[id]);
        }
    }
    return {-1, -1};
}
int main() {
    std::ios_base::sync_with_stdio(0);
    int n;
    scanf("%d",&n);
    std::vector<segment> input(n);
    for(int i=0;i<n;i++){
        point p,pp;
        scanf("%lf%lf%lf%lf",&p.x,&p.y,&pp.x,&pp.y);
        input[i] = {p,pp,i+1};
    }
    std::pair<int,int> ans = answer(input);
    if(ans.first == -1) printf("NO\n");
    else printf("YES\n%d %d\n",ans.first, ans.second);
    return 0;
}