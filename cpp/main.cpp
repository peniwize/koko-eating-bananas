/*!
    \file "main.cpp"

    Author: Matt Ervin <matt@impsoftware.org>
    Formatting: 4 spaces/tab (spaces only; no tabs), 120 columns.
    Doc-tool: Doxygen (http://www.doxygen.com/)

    https://leetcode.com/problems/koko-eating-bananas/
*/

//!\sa https://github.com/doctest/doctest/blob/master/doc/markdown/main.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <algorithm>
#include <cassert>
#include <chrono>
#include <doctest/doctest.h> //!\sa https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#include <iterator>
#include <queue>
#include <vector>
#include <set>
#include <span>

using namespace std;

// [----------------(120 columns)---------------> Module Code Delimiter <---------------(120 columns)----------------]

/*!
    Koko loves to eat bananas. There are n piles of bananas, 
    the ith pile has piles[i] bananas. The guards have gone and 
    will come back in h hours.

    Koko can decide her bananas-per-hour eating speed of k.
    Each hour, she chooses some pile of bananas and eats k bananas from that pile.
    If the pile has less than k bananas, she eats all of them instead and will not 
    eat any more bananas during this hour.

    Koko likes to eat slowly but still wants to finish eating all the bananas 
    before the guards return.

    Return the minimum integer k such that she can eat all the bananas within h hours.

    Example #1:
    8 hours
    k = 4 (banana's eaten / hour)
    0: 3,6,7,11
    1: 0,6,7,11
    2: 0,2,7,11
    3: 0,2,3,11
    4: 0,2,3,7
    5: 0,0,3,7
    6: 0,0,0,7
    7: 0,0,0,3
    8: 0,0,0,0


    Example #2:
    5 hours
    k = 30 (banana's eaten / hour)
    0: 30,11,23,4,20
    1:  0,11,23,4,20
    2:  0, 0,23,4,20
    3:  0, 0, 0,0,20
    4:  0, 0, 0,0, 0

    Example #3:
    6 hours
    k = 23  (max / hr = max value = 30)
        Iterate through all values in [1, 30] to find optimal value.
        Use binary search rather than iterating _every_ value: Try: 30, 15, ...
        Optimal = maximum value of k when all array elements become zero.
        Time = O(n*log2(max(piles[*])))
    0: 30,11,23,4,20
    1: 
*/
class Solution {
public:
    int minEatingSpeed_bruteForce(vector<int>& piles, int h) {
        int k = 1;

        while (true) {
            auto ary = piles;
            auto hours = h;
            size_t idx = 0;
            size_t zero_cnt = 0;
            for (; 0 < hours; --hours) {
                while (0 == ary[idx]) {
                    ++zero_cnt;
                    idx = (idx + 1) % ary.size();
                    if (0 == idx) {
                        if (zero_cnt == ary.size()) {
                            break;
                        }
                        zero_cnt = 0;
                    }
                }
                if (zero_cnt == ary.size()) {
                    break;
                }
                ary[idx] = (std::max)(0, ary[idx] - k);
                idx = (idx + 1) % ary.size();
                if (0 == idx) {
                    if (zero_cnt == ary.size()) {
                        break;
                    }
                    zero_cnt = 0;
                }
            }

            auto const early = 0 < hours;
            if (!early) {
                size_t idx = 0;
                for (; ary.size() > idx; ++idx) {
                    if (0 != ary[idx]) {
                        break;
                    }
                }
                auto const zero = ary.size() == idx;
                if (zero) {
                    break;
                }
            }

            ++k;
        }

        return k;
    }

    int minEatingSpeed_binarySearch(vector<int>& piles, int h) {

//
//!\todo TODO: >>> Under Construction <<<
//
return 0;

    }

    int minEatingSpeed(vector<int>& piles, int h) {
        //return minEatingSpeed_bruteForce(piles, h);
        return minEatingSpeed_binarySearch(piles, h);
    }
};
// [----------------(120 columns)---------------> Module Code Delimiter <---------------(120 columns)----------------]

struct elapsed_time_t
{
    std::chrono::steady_clock::time_point start{};
    std::chrono::steady_clock::time_point end{};
    
    elapsed_time_t(
        std::chrono::steady_clock::time_point start
        , std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now()
    ) : start{std::move(start)}, end{std::move(end)} {}
};

std::ostream&
operator<<(std::ostream& os, elapsed_time_t const& et)
{
    auto const elapsed_time = et.end - et.start;
    os << std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count()
       << '.' << (std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count() % 1000)
       << " ms";
    return os;
}

TEST_CASE("Case 1")
{
    cerr << "Case 1" << '\n';
    { // New scope.
        auto const start = std::chrono::steady_clock::now();
        vector<int> piles{3,6,7,11};
        CHECK(4 == Solution{}.minEatingSpeed(piles, 8));
        cerr << "Elapsed time: " << elapsed_time_t{start} << '\n';
    }
    cerr << '\n';
}

TEST_CASE("Case 2")
{
    cerr << "Case 2" << '\n';
    { // New scope.
        auto const start = std::chrono::steady_clock::now();
        vector<int> piles{30,11,23,4,20};
        CHECK(30 == Solution{}.minEatingSpeed(piles, 5));
        cerr << "Elapsed time: " << elapsed_time_t{start} << '\n';
    }
    cerr << '\n';
}

TEST_CASE("Case 3")
{
    cerr << "Case 3" << '\n';
    { // New scope.
        auto const start = std::chrono::steady_clock::now();
        vector<int> piles{30,11,23,4,20};
        CHECK(23 == Solution{}.minEatingSpeed(piles, 6));
        cerr << "Elapsed time: " << elapsed_time_t{start} << '\n';
    }
    cerr << '\n';
}

/*
    End of "main.cpp"
*/
