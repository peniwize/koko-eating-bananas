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
#include <numeric>
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

    8 hours
    k = 5 (banana's eaten / hour)
    0: 3,6,7,11
    1: 0,6,7,11
    2: 0,1,7,11
    3: 0,1,2,11
    4: 0,1,2, 6
    5: 0,0,2, 6
    6: 0,0,0, 6
    7: 0,0,0, 1
    8: 0,0,0, 0
    NOTE: This produces the same result as k = 4!

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
    1:  7,11,23,4,20
    2:  7, 0,23,4,20
    3:  7, 0, 0,4,20
    4:  7, 0, 0,0,20
    5:  7, 0, 0,0, 0
    6:  0, 0, 0,0, 0
*/
class Solution {
public:
    int minEatingSpeed_bruteForce(vector<int>& piles, int h) {
        int bananasEatenPerHour = 1;

        while (true) {
            auto bananaPiles = piles;
            auto remainingHours = h;
            size_t bananaPileIdx = 0;
            size_t emptyBananaPileCnt = 0;
            for (; 0 < remainingHours; --remainingHours) {
                // Advance index to next uneaten banana pile.
                while (0 == bananaPiles[bananaPileIdx]) {
                    ++emptyBananaPileCnt;

                    // Increment banana pile index and track empty piles.
                    bananaPileIdx = (bananaPileIdx + 1) % bananaPiles.size();
                    if (0 == bananaPileIdx) {
                        auto const allBananaPilesEaten = emptyBananaPileCnt == bananaPiles.size();
                        if (allBananaPilesEaten) { break; }
                        emptyBananaPileCnt = 0;
                    }
                }
                auto const allBananaPilesEaten = emptyBananaPileCnt == bananaPiles.size();
                if (allBananaPilesEaten) { break; }
                
                // Reduce banana pile size by 'n' bananaPiles.
                bananaPiles[bananaPileIdx] = (std::max)(0, bananaPiles[bananaPileIdx] - bananasEatenPerHour);

                // Increment banana pile index and track empty piles.
                bananaPileIdx = (bananaPileIdx + 1) % bananaPiles.size();
                if (0 == bananaPileIdx) {
                    auto const allBananaPilesEaten = emptyBananaPileCnt == bananaPiles.size();
                    if (allBananaPilesEaten) { break; }
                    emptyBananaPileCnt = 0;
                }
            }

            // Detect when all banana piles have been eaten
            // (all array elements are zero and did not stop eating early).
            auto const finishedEarly = 0 < remainingHours;
            if (!finishedEarly) {
                size_t bananaPileIdx = 0;
                for (; bananaPiles.size() > bananaPileIdx; ++bananaPileIdx) {
                    auto const thereAreUneatenBananas = 0 != bananaPiles[bananaPileIdx];
                    if (thereAreUneatenBananas) { break; }
                }
                auto const allBananasEaten = bananaPiles.size() == bananaPileIdx;
                if (allBananasEaten) { break; }
            }

            ++bananasEatenPerHour;
        }

        return bananasEatenPerHour;
    }

    /*!
        Time = O(log2(max(piles))*len(piles))
        Space = O(1)
    */
    int minEatingSpeed_binarySearch(vector<int> const& piles, int const h) {
        int maxConsumptionRate = 0;

        // Determine the maximum number of bananas that can be eaten per hour.
        auto const maxElement = std::max_element(piles.begin(), piles.end());
        if (piles.end() != maxElement) {
            auto const largestBananaPileSize = *maxElement;

            // Binary search to find the max banana consumption rate (k).
            int minBananaCnt = 1; // Binary search min value.
            int maxBananaCnt = largestBananaPileSize; // Binary search max value.
            while (minBananaCnt <= maxBananaCnt) {
                int const bananaCnt = minBananaCnt + (maxBananaCnt - minBananaCnt + 1) / 2; // Binary search mid point.

                // Determine if all bananas can be consumed in 'h' hours,
                // Convert banana pile counts to how long is required to eat each pile.
                int timeToEatAllPiles = 0;
                for (auto const& pile : piles) {
                    timeToEatAllPiles += pile / bananaCnt + (0 == pile % bananaCnt ? 0 : 1);
                }
                auto const ateTooSlowly = timeToEatAllPiles > h;
                if (ateTooSlowly) {
                    // Ate too slowly; ran out of time before finishing.
                    minBananaCnt = bananaCnt + 1; // Continue searching right half of remaining range.
                } else {
                    // Ate either too fast or one of the right rates (there may be several).
                    maxConsumptionRate = bananaCnt; // Most recent rate that consumed all bananas.
                    maxBananaCnt = bananaCnt - 1; // Continue searching left half of remaining range.
                }
            }
        }

        return maxConsumptionRate;
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
    os << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count()
       << '.' << (std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count() % 1000)
       << " ms";
    return os;
}

TEST_CASE("Case 0")
{
    cerr << "Case 0" << '\n';
    { // New scope.
        auto const start = std::chrono::steady_clock::now();
        vector<int> piles{2};
        CHECK(2 == Solution{}.minEatingSpeed(piles, 1));
        cerr << "Elapsed time: " << elapsed_time_t{start} << '\n';
    }
    cerr << '\n';
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

TEST_CASE("Case 4")
{
    cerr << "Case 4" << '\n';
    { // New scope.
        auto const start = std::chrono::steady_clock::now();
        vector<int> piles{312884470};
        CHECK(2 == Solution{}.minEatingSpeed(piles, 312884469));
        cerr << "Elapsed time: " << elapsed_time_t{start} << '\n';
    }
    cerr << '\n';
}

/*
    End of "main.cpp"
*/
