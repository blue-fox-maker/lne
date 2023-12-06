#include <algorithm>
#include <array>
#include <list>
#include <memory>
#include <set>
#include <vector>

namespace Temporal
{
    using time_t = size_t;
    using vert_t = size_t;
    struct Interval
    {
        size_t ts, te;
        inline bool contains(const Interval &other) const;
        inline bool contains(const time_t t) const;
        inline Interval combine(const Interval &other) const;
        inline static bool overlaps(const Interval &a, const Interval &b);
        inline bool operator==(const Interval &other) const;
        inline bool operator!=(const Interval &other) const;
    };
    struct Intervals
    {
        std::vector<Interval> intervals;
        inline size_t size() const { return intervals.size(); }
        inline bool empty() const { return intervals.empty(); }
        static Intervals concat(const Intervals &a, const Intervals &b);
        static Intervals combine(const Intervals &a, const Intervals &b);
        static Intervals intersection(const Intervals &a, const Intervals &b);
        inline bool contains(const time_t t) const;
        inline bool contains(const Interval &interval) const;
    };

    inline bool Interval::contains(const Interval &other) const
    {
        return ts <= other.ts && other.te <= te;
    }
    inline bool Interval::contains(const time_t t) const
    {
        return ts <= t && t <= te;
    }
    inline Interval Interval::combine(const Interval &other) const
    {
        return Interval{.ts = std::min(ts, other.ts), .te = std::max(te, other.te)};
    }
    inline bool Interval::operator==(const Interval &other) const
    {
        return ts == other.ts && te == other.te;
    }
    inline bool Interval::operator!=(const Interval &other) const
    {
        return !(*this == other);
    }
    inline static bool overlaps(const Interval &a, const Interval &b)
    {
        return a.ts < b.te && b.ts < a.te;
    }
    Intervals Intervals::concat(const Intervals &a, const Intervals &b)
    {
        Intervals result;
        std::merge(a.intervals.begin(), a.intervals.end(), b.intervals.begin(),
                   b.intervals.end(), std::back_inserter(result.intervals));
        return result;
    }
    Intervals Intervals::combine(const Intervals &a, const Intervals &b)
    {
        auto temp = Intervals::concat(a, b);
        Intervals result;
        for (auto &&interval : temp.intervals)
        {
            if (result.empty() || result.intervals.back().te < interval.ts)
                result.intervals.push_back(interval);
            else
                result.intervals.back().te =
                    std::max(result.intervals.back().te, interval.te);
        }
        return result;
    }
    Intervals Intervals::intersection(const Intervals &a, const Intervals &b)
    {
        Intervals result;
        auto it_a = a.intervals.begin();
        auto it_b = b.intervals.begin();
        while (it_a != a.intervals.end() && it_b != b.intervals.end())
        {
            auto left = std::max(it_a->ts, it_b->ts);
            auto right = std::min(it_a->te, it_b->te);
            if (left <= right)
            {
                result.intervals.push_back({.ts = left, .te = right});
            }
            if (it_a->te < it_b->te)
                ++it_a;
            else
                ++it_b;
        }
        return result;
    }
    inline bool Intervals::contains(const time_t t) const
    {
        return std::any_of(intervals.begin(), intervals.end(), [=](const Interval &interval)
                           { return interval.contains(t); });
    }
    inline bool Intervals::contains(const Interval &interval) const
    {
        return std::any_of(intervals.begin(), intervals.end(), [=](const Interval &interval)
                           { return interval.contains(interval); });
    }
} // namespace Temporal
