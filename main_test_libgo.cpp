
#include <memory>
#include <chrono>

#include <libgo/libgo.h>

#include "log/glog.h"

uint64_t getCurMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count();
}

class Point {
public:
    using ptr = std::shared_ptr<Point>;

    Point() {
        //LOG(INFO) << "无参构造";
    }

    Point(int x, int y)
        : m_x(x)
        , m_y(y) {
        //LOG(INFO) << "有参构造";
    }

    Point(const Point& other)
        : m_x(other.m_x)
        , m_y(other.m_y) {
        //LOG(INFO) << "拷贝构造";
    }

    Point& operator=(const Point& other) {
        m_x = other.m_x;
        m_y = other.m_y;
        //LOG(INFO) << "赋值构造";
        return *this;
    }

    Point(Point&& other)
        : m_x(std::exchange(other.m_x, 0))
        , m_y(std::exchange(other.m_y, 0)) {
        // LOG(INFO) << "0移动构造";
    }

    Point& operator=(Point&& other) {
        m_x = std::exchange(other.m_x, 0);
        m_y = std::exchange(other.m_y, 0);
        //LOG(INFO) << "1移动构造";
        return *this;
    }

    int m_x;
    int m_y;
};

int main(int argc, char* argv[]) {
    GLog glog(argv[0]);

    auto thread_num = 8;
    auto total_num = 100000;
    auto pnum = 10;
    auto cnum = 1;
    auto pcount = total_num / pnum;
    auto ccount = total_num / cnum;
    uint64_t sum = 0;

    //co_chan<Point::ptr> points(total_num * 2);
    co_chan<Point> points(total_num * 2);
    co_chan<void> cw(pnum + cnum);

    auto start_ts = getCurMs();
    LOG(WARNING) << "start ts " << start_ts << " thread_num " << thread_num << " total num " << total_num;

    //auto p = std::make_shared<Point>(1, 1);
    //auto p = Point(1, 1);
    for (auto i = 0; i < pnum; ++i) {
        go [&, i] {
            for (auto j = 0; j < pcount; ++j) {
                auto val = i * pcount + j;
                points << std::move(Point(val, val));
            }
            cw << nullptr;
            auto end_ts = getCurMs();
            LOG(WARNING) << "produce index " << i << " end ts " << end_ts << " dur(ms) " << end_ts - start_ts << " size " << points.size();
        };
    }

    // co::Scheduler* sched = co::Scheduler::Create();
    // std::thread t([sched]{ sched->Start(1); });
    // t.detach();

    for (auto i = 0; i < cnum; ++i) {
        //go co_scheduler(sched) [&] {
        go [&, i] {
            for (auto j = 0; j < ccount; ++j) {
                Point p;
                points >> p;
                sum += p.m_x;
            }
            cw << nullptr;
            auto end_ts = getCurMs();
            LOG(WARNING) << "consume index " << i << " end ts " << end_ts << " dur(ms) " << end_ts - start_ts << " sum " << sum << " size " << points.size();
        };
    }

    go [&] {
        for (auto i = 0; i < pnum + cnum; ++i) {
            cw >> nullptr;
        }
        auto end_ts = getCurMs();
        LOG(WARNING) << "produce & consume end ts " << end_ts << " dur(ms) " << end_ts - start_ts << " sum " << sum << " size " << points.size();
    };

    co_sched.Start(thread_num);

    return 0;
}




// package main

// import (
//     "sync"
//     "time"

//     "github.com/astaxie/beego/logs"
// )

// type Point struct {
//     X int
//     Y int
//     Z int
// }

// func NewPoint(x, y, z int) *Point {
//     p := &Point{
//         X: x,
//         Y: y,
//         Z: z,
//     }
//     return p
// }

// func main() {
//     //logs.Info("begin...")

//     points := make(chan Point, 102400)
//     total_num := 1000000
//     pnum := 10
//     cnum := 1
//     pcount := total_num / pnum
//     ccount := total_num / cnum
//     var sum uint64 = 0

//     var wg sync.WaitGroup
//     wg.Add(pnum + cnum)

//     start_ts := time.Now().UnixMilli()
//     logs.Info("start ts ", start_ts, " total num ", total_num)

//     for i := 0; i < pnum; i++ {
//         go func(i, pcount int) {
//             for j := 0; j < pcount; j++ {
//                 v := i*pcount + j
//                 points <- Point{v, v, v}
//             }
//             wg.Done()
//             end_ts := time.Now().UnixMilli()
//             logs.Info("produce index ", i, " end ts ", time.Now().UnixMilli(), " dur ", end_ts-start_ts, " points len ", len(points))
//         }(i, pcount)
//     }

//     for i := 0; i < cnum; i++ {
//         go func(i, ccount int) {
//             for j := 0; j < ccount; j++ {
//                 p := <-points
//                 sum += uint64(p.X)
//             }
//             wg.Done()
//             end_ts := time.Now().UnixMilli()
//             logs.Info("consume index ", i, " end ts ", time.Now().UnixMilli(), " dur ", end_ts-start_ts, " points len ", len(points), " sum ", sum)
//         }(i, ccount)
//     }

//     wg.Wait()
//     end_ts := time.Now().UnixMilli()
//     logs.Info("produce & consume end ts ", time.Now().UnixMilli(), " dur ", end_ts-start_ts, " sum ", sum)
// }
