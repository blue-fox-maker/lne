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
        inline bool contains(const Interval &other) const
        {
            return ts <= other.ts && other.te <= te;
        }
        inline bool contains(const time_t t) const { return ts <= t && t <= te; }
        inline Interval combine(const Interval &other) const
        {
            return Interval{.ts = std::min(ts, other.ts), .te = std::max(te, other.te)};
        }
        inline static bool overlaps(const Interval &a, const Interval &b)
        {
            return a.ts < b.te && b.ts < a.te;
        }
    };
    struct Intervals
    {
        std::vector<Interval> intervals;
        inline size_t size() const { return intervals.size(); }
        static Intervals concat(const Intervals &a, const Intervals &b)
        {
            Intervals res;
            std::merge(a.intervals.begin(), a.intervals.end(), b.intervals.begin(),
                       b.intervals.end(), std::back_inserter(res.intervals));
            return res;
        }
        static Intervals combine(const Intervals &a, const Intervals &b)
        {
            auto temp = Intervals::concat(a, b);
            Intervals res;
            for (auto &&interval : temp.intervals)
            {
                if (res.size() == 0 || res.intervals.back().te < interval.ts)
                    res.intervals.push_back(interval);
                else
                    res.intervals.back().te =
                        std::max(res.intervals.back().te, interval.te);
            }
            return res;
        }
        static Intervals intersection(const Intervals &a, const Intervals &b)
        {
            Intervals res;
            auto ptr_a = a.intervals.begin();
            auto ptr_b = b.intervals.begin();
            while (ptr_a != a.intervals.end() && ptr_b != b.intervals.end())
            {
                auto left = std::max(ptr_a->ts, ptr_b->ts);
                auto right = std::min(ptr_a->te, ptr_b->te);
                if (left <= right)
                {
                    res.intervals.push_back({.ts = left, .te = right});
                }
                if (ptr_a->te < ptr_b->te)
                    ++ptr_a;
                else
                    ++ptr_b;
            }
        }
        inline bool contains(const time_t t) const
        {
          return std::any_of(intervals.begin(),intervals.end(),[=](const Interval &interval){
                return interval.contains(t);
  });
        }
        inline bool contains(const Interval &interval) const
        {
                        return std::any_of(intervals.begin(),intervals.end(),[=](const Interval &interval){
                return interval.contains(interval);
	});
        }
    };

} // namespace Temporal
