#ifndef __CORE_BASE_TIMER_MANAGER_H__
#define __CORE_BASE_TIMER_MANAGER_H__

#include <libgo/libgo.h>

typedef std::function<void(void)> TimerCB;

namespace Eayew {

class TimerManager {
public:
    using ptr = std::shared_ptr<TimerManager>;

    TimerManager(std::shared_ptr<co::CoTimer> timer) : m_timer(timer) {}

    void addIntervalTimer(int interval, TimerCB cb, bool forever = true);
    void addMinuteTimer(int second, TimerCB cb, bool forever = true);
    void addHourlyTimer(int minute, int second, TimerCB cb, bool forever = true);
    void addDailyTimer(int hour, int minute, int second, TimerCB cb, bool forever = true);

private:
    unsigned long long getCurrentMillisecs();

    void intervalTimer(int interval, TimerCB cb);
    void minuteTimer(int second, TimerCB cb);
    void hourlyTimer(int minute, int second, TimerCB cb);
    void dailyTimer(int hour, int minute, int second, TimerCB cb);

private:
    std::shared_ptr<co::CoTimer> m_timer;

};

}


#endif