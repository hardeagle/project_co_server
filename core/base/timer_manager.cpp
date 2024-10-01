#include "timer_manager.h"

#include <sys/time.h>    
#include <unistd.h>

namespace Eayew {

static const int COEF = 1000;
static const int MINUTE = 60;
static const int HOUR = 60 * 60;
static const int LAG = 8 * 60 * 60;
static const int DAY = 24 * 60 * 60;

void TimerManager::addIntervalTimer(int interval, TimerCB cb, bool forever) {
    if (!forever) {
        m_task.run_in([this, cb]() {
            m_sched->go([this, cb]() {
                cb();
            });
        }, interval);
        return;
    }
    m_task.run_every([this, cb]() {
        m_sched->go([this, cb]() {
            cb();
        });
    }, interval);
}

// void TimerManager::addMinuteTimer(int second, TimerCB cb, bool forever) {
//     int seconds = second * COEF;
//     unsigned long long now = getCurrentMillisecs();
//     int interval = 0;
//     int remain = now % (MINUTE * COEF);
//     if (seconds > remain) {
//         interval = seconds - remain;
//     } else {
//         interval = seconds + MINUTE * COEF - remain;
//     }

//     if (!forever) {
//         m_timer->ExpireAt(std::chrono::milliseconds(interval), cb);
//         return;
//     }

//     m_timer->ExpireAt(std::chrono::milliseconds(interval), std::bind(&TimerManager::minuteTimer, this, second, cb));
// }

// void TimerManager::addHourlyTimer(int minute, int second, TimerCB cb, bool forever) {
//     int seconds = (minute * MINUTE + second) * COEF;
//     unsigned long long now = getCurrentMillisecs();
//     int interval = 0;
//     int remain = now % (HOUR * COEF);
//     if (seconds > remain) {
//         interval = seconds - remain;
//     } else {
//         interval = seconds + HOUR * COEF - remain;
//     }

//     if (!forever) {
//         m_timer->ExpireAt(std::chrono::milliseconds(interval), cb);
//         return;
//     }

//     m_timer->ExpireAt(std::chrono::milliseconds(interval), std::bind(&TimerManager::hourlyTimer, this, minute, second, cb));
// }

void TimerManager::addDailyTimer(int hour, int minute, int second, TimerCB cb, bool forever) {
    if (!forever) {
        m_task.run_at([this, cb]() {
            m_sched->go([this, cb]() {
                cb();
            });
        }, hour, minute, second);
        return;
    }
    m_task.run_daily([this, cb]() {
        m_sched->go([this, cb]() {
            cb();
        });
    }, hour, minute, second);
}

// unsigned long long TimerManager::getCurrentMillisecs() {
//     timeval tv;    
//     ::gettimeofday(&tv, 0);
//     unsigned long long ret = tv.tv_sec;
//     return ret * 1000 + tv.tv_usec / 1000;
// }

// void TimerManager::intervalTimer(int interval, TimerCB cb) {
//     if (cb) {
//         cb();
//     }
//     m_timer->ExpireAt(std::chrono::milliseconds(interval), std::bind(&TimerManager::intervalTimer, this, interval, cb));
// }


// void TimerManager::minuteTimer(int second, TimerCB cb) {
//     if (cb) {
//         cb();
//     }

//     int seconds = second * COEF;
//     unsigned long long now = getCurrentMillisecs();
//     int interval = 0;
//     int remain = now % (MINUTE * COEF);
//     if (seconds > remain) {
//         interval = seconds - remain;
//     } else {
//         interval = seconds + MINUTE * COEF - remain;
//     }

//     m_timer->ExpireAt(std::chrono::milliseconds(interval), std::bind(&TimerManager::minuteTimer, this, second, cb));
// }

// void TimerManager::hourlyTimer(int minute, int second, TimerCB cb) {
//     if (cb) {
//         cb();
//     }

//     int seconds = (minute * MINUTE + second) * COEF;
//     unsigned long long now = getCurrentMillisecs();
//     int interval = 0;
//     int remain = now % (HOUR * COEF);
//     if (seconds > remain) {
//         interval = seconds - remain;
//     } else {
//         interval = seconds + HOUR * COEF - remain;
//     }

//     m_timer->ExpireAt(std::chrono::milliseconds(interval), std::bind(&TimerManager::hourlyTimer, this, minute, second, cb));
// }

// void TimerManager::dailyTimer(int hour, int minute, int second, TimerCB cb) {
//     if (cb) {
//         cb();
//     }

//     int seconds = (hour * HOUR +  minute * MINUTE + second) * COEF;
//     unsigned long long now = getCurrentMillisecs();
//     int interval = 0;
//     int remain = now % (DAY * COEF) + LAG * COEF;
//     if (seconds > remain) {
//         interval = seconds - remain;
//     } else {
//         interval = seconds + DAY * COEF - remain;
//     }

//     m_timer->ExpireAt(std::chrono::milliseconds(interval), std::bind(&TimerManager::dailyTimer, this, hour, minute, second, cb));
// }

}