#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <set>

struct Share
{
    int64_t y;
    int64_t x;
};


int64_t f(uint32_t x, std::vector<int64_t> const & coefs)
{
    int64_t y = 0;
    for (auto i = 0; i < coefs.size(); i++)
        y += coefs[i] * (int64_t)std::pow((double)x, (double)i);

    return y;
}

std::vector<Share> split(int64_t secret, uint32_t n, uint32_t t)
{
    std::vector<Share> shares;

    std::vector<int64_t> coefs;
    coefs.push_back(secret);

    for (auto i = 1; i < t; i++)
        coefs.push_back((std::rand() % 1000) - 500);

    for (auto i = 0; i < n; i++)
        shares.push_back({ f(i + 1, coefs), i + 1 });

    return shares;
}

int64_t recover(int n, std::vector<Share> const & shares)
{
    auto l = [shares](int64_t i) -> double {
        double res = 1.;
        for (auto j = 0; j < shares.size(); j++)
            if (i != shares[j].x)
                res *= (double)shares[j].x / ((double)shares[j].x - (double)i);
        return res;
    };

    double res = 0;

    for (auto i = 0; i < shares.size(); i++)
        res += shares[i].y * l(shares[i].x);

    return (uint64_t)res;
}

std::vector<Share> choose(int t, std::vector<Share> const & shares)
{
    std::vector<Share> choose;
    std::set<uint32_t> chIdx;

    for (auto i = 0; i < t; i++) {
        uint32_t idx;
        while(chIdx.find((idx = rand() % t)) != chIdx.end());
        choose.push_back(shares[idx]);
        chIdx.insert(idx);
    }
    return choose;
}

auto main(int argc, char **argv) -> int
{
    std::srand(unsigned(std::time(0)));

    auto n = atoi(argv[1]);
    auto t = atoi(argv[2]);
    auto secret = atoi(argv[3]);
    auto tt = atoi(argv[4]);

    std::cout << "n = " << n << "\n"
            << "t = " << t << "\n"
            << "secret = " << secret << "\n";

    auto shares = split(secret, n, t);

    std::cout << "shares = \'";
    for (auto share : shares)
        std::cout << "(" << share.x << "; " << share.y << ") ";
    std::cout << "\'\n";

    auto chs = choose(tt, shares);
    std::cout << "choosed shares = \'";
    for (auto share : chs)
        std::cout << "(" << share.x << "; " << share.y << ") ";
    std::cout << "\'\n";

    auto recs = recover(n, chs);

    std::cout << "rec. secret = " << recs << "\n";

    return 0;
}
