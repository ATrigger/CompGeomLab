#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
struct point {
    int64_t x=0;
    int64_t y=0;
    point() {}
    point(int64_t x, int64_t y) : x(x),y(y){}
};
typedef std::vector<point> polygon_t;
typedef std::pair<point,point> segment_t;
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
int64_t oriented_sq(const point& a, const point&b, const point& c){
    return sgn((c.x-a.x) * (b.y-a.y) - (c.y -a.y) * (b.x-a.x));
}
bool left(const point& a, const point&b, const point& c){
    return oriented_sq(a,b,c) < 0;
}
bool right(const point& a, const point&b, const point& c){
    return oriented_sq(a,b,c)>0;
}
bool distinct(const point& a, const point&b, const point& c, const point& p) {
    return oriented_sq(b,a,p) >=0 && left(b,c,p);
}

bool bounding_box(const segment_t& seg, const point& c){
    int64_t minx = std::min(seg.first.x, seg.second.x);
    int64_t maxx = std::max(seg.first.x,seg.second.x);
    int64_t miny = std::min(seg.first.y,seg.second.y);
    int64_t maxy = std::max(seg.first.y,seg.second.y);
    return minx <= c.x && c.x <= maxx && miny <= c.y && c.y <= maxy;
}
bool really_inside(const segment_t &seg, const point&c){
    return oriented_sq(seg.first,seg.second,c) == 0 && bounding_box(seg,c);
}
bool intersex(const segment_t &seg, const segment_t &secseg){
    auto first = oriented_sq(seg.first,seg.second,secseg.first);
    auto second = oriented_sq(seg.first,seg.second,secseg.second);
    auto  third = oriented_sq(secseg.first,secseg.second,seg.first);
    auto  fourth = oriented_sq(secseg.first,secseg.second,seg.second);

    if(first != second) return third != fourth;
    else if(first == 0){
        if(seg.first.x == seg.second.x && seg.first.y == seg.second.y && third != 0) return false;
        else return bounding_box(seg,secseg.first) || bounding_box(seg,secseg.second) || bounding_box(secseg,seg.first) || bounding_box(secseg,seg.second);
    }
    else return false;
}
int main() {
    auto fin = new std::fstream("point.in", std::fstream::in);
    auto fout = new std::fstream("point.out", std::fstream::out | std::fstream::trunc);
    polygon_t polygon;
    int n;
    point starto;
    std::string ans[] = {"NO","YES"};
    fin->operator>>(n);
    fin->operator>>(starto.x);
    fin->operator>>(starto.y);
    segment_t ray {starto,{10000239,starto.y+1}};
    for(int i=0;i<n;i++){
        polygon.push_back({});
        fin->operator>>(polygon.back().x);
        fin->operator>>(polygon.back().y);
    }
    int sex = 0;
    bool flag = false;
    for(int i=0;i<n;i++){
        segment_t tmp = {polygon[i],polygon[(i+1)%n]};
        if(really_inside(tmp,starto)) {
            flag = true;
            break;
        }
        sex += intersex(tmp,ray);
    }
    *fout << ans[(flag || sex %2 ==1)] << std::endl;
    delete fin;
    delete fout;
    return 0;
}