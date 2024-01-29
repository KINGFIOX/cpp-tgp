#include <iostream>
#include <map>

using namespace std;

template <typename T>
class E {
    template <typename U>
    using str_map_t = map<string, U>;

public:
    str_map_t<int> m;
};

int main(void)
{
    E<float> e;
    e.m.insert({ "first", 1 });
    e.m.insert({ "second", 2 });
    for (auto v : e.m) {
        cout << v.first << " " << v.second << endl;
    }
}
