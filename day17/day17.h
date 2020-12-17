
struct coord {
    int x, y, z;
    
    bool operator==(const coord &o) {
        return (x == o.x) 
            && (y == o.y) 
            && (z == o.z)
        ;
    }

    bool operator<(const coord &o) const{
        return x < o.x 
            || (x == o.x && y < o.y) 
            || (x == o.x && y == o.y && z < o.z);
    }
};

struct hyper_coord {
    int x, y, z, w;

    bool operator==(const hyper_coord &o) {
        return (x == o.x) 
            && (y == o.y) 
            && (z == o.z)
            && (w == o.w)
        ;
    }

    bool operator<(const hyper_coord &o) const{
        return x < o.x 
            || (x == o.x && y < o.y) 
            || (x == o.x && y == o.y && z < o.z)
            || (x == o.x && y == o.y && z == o.z && w < o.w)
        ;
    }
};