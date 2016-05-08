#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
struct point {
    int64_t x;
    int64_t y;
};
typedef std::vector<point> polygon_t;
std::string ans[] = {"INSIDE","BORDER","OUTSIDE"};
int64_t oriented_sq(const point& a, const point&b, const point& c){
    return (c.x-a.x) * (b.y-a.y) - (c.y -a.y) * (b.x-a.x);
}
bool left(const point& a, const point&b, const point& c){
    return oriented_sq(a,b,c) < 0;
}
bool right(const point& a, const point&b, const point& c){
    return oriented_sq(a,b,c)>0;
}
int result(const point& a, const point&b, const point& c) {
    int64_t whether = oriented_sq(a,b,c);
    if (whether > 0) return 0;
    else if(whether < 0) return 2;
    else return 1;
}
bool distinct(const point& a, const point&b, const point& c, const point& p) {
    return oriented_sq(b,a,p) >=0 && left(b,c,p);
}

bool is_inside(const point& a, const point&b, const point& c){
    if(oriented_sq(a,b,c)!=0) return false;
    int64_t minx = std::min(a.x, b.x);
    int64_t maxx = std::max(a.x,b.x);
    int64_t miny = std::min(a.y,b.y);
    int64_t maxy = std::max(a.y,b.y);
    return minx <= c.x && c.x <= maxx && miny <= c.y && c.y <= maxy;
}

int solve(const polygon_t &poly, const point& p){
    size_t l = 1, r = poly.size() -1 ;
    if(is_inside(poly.front(),poly[l],p) || is_inside(poly.front(),poly[r],p)){
        return 1;
    }
    if(!distinct(poly[l],poly.front(),poly[r],p)){
        return 2;
    }
    while(l + 1 < r){
        size_t m = (l+r)/2;
        if(distinct(poly[l],poly.front(),poly[m],p)){
            r = m;
        } else {
            l = m;
        }
    }
    return result(poly[l],poly[r],p);
}
int main() {
    auto fin = new std::fstream("inside.in", std::fstream::in);
    auto fout = new std::fstream("inside.out", std::fstream::out | std::fstream::trunc);
    polygon_t polygon;
    int n,k;
    fin->operator>>(n);
    for(int i=0;i<n;i++){
        polygon.push_back({});
        fin->operator>>(polygon.back().x);
        fin->operator>>(polygon.back().y);
    }
    if(left(polygon[0],polygon[1],polygon[2])) std::reverse(polygon.begin(),polygon.end());
    fin->operator>>(k);
    for(int i=0;i<k;i++)
    {
        point tmp;
        fin->operator>>(tmp.x);
        fin->operator>>(tmp.y);
        *fout << ans[solve(polygon,tmp)] << std::endl;
    }
    delete fin;
    delete fout;
    return 0;
}