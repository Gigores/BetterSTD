#include "btrstd/datetime.h"

#ifdef _WIN32
#   include "windows.h"
#else
#   include "time.h"
#endif

btr_time_point_s BTR_Time_now(void)
{
#ifdef _WIN32
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;

    long long ns = (long long)uli.QuadPart * 100LL;
    const unsigned long long EPOCH_DIFF = 11644473600000000000ULL;
    return (btr_time_point_s){
        .nanoseconds = ns - EPOCH_DIFF
    };
#else
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    return (btr_time_point_s) {
        .nanoseconds = (long long)ts.tv_sec * 1000000000ULL + ts.tv_nsec,
    };
#endif
}
btr_time_point_s BTR_Time_monotonic(void)
{
#ifdef _WIN32
    static LARGE_INTEGER freq;
    static int init = 0;
    if (!init) {
        QueryPerformanceFrequency(&freq);
        init = 1;
    }
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    long long ns = (now.QuadPart * 1000000000LL) / freq.QuadPart;
    return (btr_time_point_s){
        .nanoseconds = ns
    };
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (btr_time_point_s) {
        .nanoseconds = (long long)ts.tv_sec * 1000000000ULL + ts.tv_nsec,
    };
#endif
}

btr_duration_s BTR_Hour(double hours) {
    return (btr_duration_s) {
        .nanoseconds = (unsigned long long)(hours * 1000000000.0 * 60.0 * 60.0),
    };
}
btr_duration_s BTR_Min(double minutes) {
    return (btr_duration_s) {
        .nanoseconds = (unsigned long long)(minutes * 1000000000.0 * 60.0),
    };
}
btr_duration_s BTR_Sec(double seconds) {
    return (btr_duration_s) {
        .nanoseconds = (unsigned long long)(seconds * 1000000000.0),
    };
}
btr_duration_s BTR_Milsec(double milliseconds) {
    return (btr_duration_s) {
        .nanoseconds = (unsigned long long)(milliseconds * 1000000.0),
    };
}
btr_duration_s BTR_Micsec(double microseconds) {
    return (btr_duration_s) {
        .nanoseconds = (unsigned long long)(microseconds * 1000.0),
    };
}
btr_duration_s BTR_Nansec(unsigned long long nanoseconds) {
    return (btr_duration_s) {
        .nanoseconds = nanoseconds,
    };
}

double BTR_Duration_asSec(btr_duration_s this) {
    return this.nanoseconds / 1000000000.0;
}
double BTR_Duration_asMilsec(btr_duration_s this) {
    return this.nanoseconds / 1000000.0;
}
double BTR_Duration_asMicsec(btr_duration_s this) {
    return this.nanoseconds / 1000.0;
}
unsigned long long BTR_Duration_asNansec(btr_duration_s this) {
    return this.nanoseconds;
}

btr_duration_s BTR_Duration_add(btr_duration_s a, btr_duration_s b) {
    return (btr_duration_s) {
        .nanoseconds = a.nanoseconds + b.nanoseconds,
    };
}
btr_duration_s BTR_Duration_sub(btr_duration_s a, btr_duration_s b) {
    return (btr_duration_s) {
        .nanoseconds = a.nanoseconds - b.nanoseconds,
    };
}
btr_duration_s BTR_Duration_mul(btr_duration_s a, btr_duration_s b) {
    return (btr_duration_s) {
        .nanoseconds = a.nanoseconds * b.nanoseconds,
    };
}
btr_duration_s BTR_Duration_div(btr_duration_s a, btr_duration_s b) {
    return (btr_duration_s) {
        .nanoseconds = a.nanoseconds / b.nanoseconds,
    };
}

btr_duration_s BTR_Time_diff(btr_time_point_s a, btr_time_point_s b) {
    if (a.nanoseconds > b.nanoseconds)
        return (btr_duration_s) {
            .nanoseconds = a.nanoseconds - b.nanoseconds,
        };
    else
        return (btr_duration_s) {
            .nanoseconds = b.nanoseconds - a.nanoseconds,
        };
}
btr_duration_s BTR_Time_since(btr_time_point_s start) {
    return BTR_Time_diff(start, BTR_Time_now());
}

void BTR_sleep(btr_duration_s time)
{
#ifdef _WIN32
    Sleep((DWORD)(time.nanoseconds / 1000000ULL));
#else
    struct timespec req;
    struct timespec rem;
    req.tv_sec  = time.nanoseconds / 1000000000ULL;
    req.tv_nsec = time.nanoseconds % 1000000000ULL;
    while (nanosleep(&req, &rem) == -1)
        req = rem;
#endif
}

btr_timer_s BTR_Timer_start(void) {
    return (btr_timer_s) {
        .started = BTR_Time_monotonic(),
    };
}
void BTR_Timer_reset(btr_timer_s *this) {
    this->started = BTR_Time_monotonic();
}
btr_duration_s BTR_Timer_elapsed(const btr_timer_s *this) {
    return BTR_Time_diff(this->started, BTR_Time_monotonic());
}

btr_datetime_s BTR_DateTime_now(void) {
    return BTR_DateTime_fromTimestamp(BTR_Time_now());
}
btr_datetime_s BTR_DateTime_localNow(void) {
    return BTR_DateTime_fromLocalTimestamp(BTR_Time_now());
}
btr_datetime_s BTR_DateTime_fromTimestamp(btr_time_point_s point)
{
    time_t sec = (time_t)(point.nanoseconds / 1000000000LL);

    struct tm tm;

#ifdef _WIN32
    gmtime_s(&tm, &sec);
#else
    gmtime_r(&sec, &tm);
#endif

    return (btr_datetime_s){
        .year       = tm.tm_year + 1900,
        .month      = tm.tm_mon + 1,
        .day        = tm.tm_mday,

        .hour       = tm.tm_hour,
        .minute     = tm.tm_min,
        .second     = tm.tm_sec,

        .nanosecond = (unsigned)(point.nanoseconds % 1000000000LL),
    };
}
btr_datetime_s BTR_DateTime_fromTimestampLocal(btr_time_point_s point)
{
    time_t sec = (time_t)(point.nanoseconds / 1000000000LL);

    struct tm tm;

#ifdef _WIN32
    localtime_s(&tm, &sec);
#else
    localtime_r(&sec, &tm);
#endif

    long long ns = point.nanoseconds % 1000000000LL;
    if (ns < 0) {
        ns += 1000000000LL;
    }

    return (btr_datetime_s) {
        .year       = (unsigned)(tm.tm_year + 1900),
        .month      = (unsigned)(tm.tm_mon + 1),
        .day        = (unsigned)tm.tm_mday,

        .hour       = (unsigned)tm.tm_hour,
        .minute     = (unsigned)tm.tm_min,
        .second     = (unsigned)tm.tm_sec,

        .nanosecond = (unsigned)ns
    };
}
btr_time_point_s BTR_DateTime_toTimestamp(const btr_datetime_s *date)
{
    struct tm tm = {
        .tm_year = (int)date->year - 1900,
        .tm_mon  = (int)date->month - 1,
        .tm_mday = (int)date->day,

        .tm_hour = (int)date->hour,
        .tm_min  = (int)date->minute,
        .tm_sec  = (int)date->second,

        .tm_isdst = 0
    };

#ifdef _WIN32
    time_t sec = _mkgmtime(&tm);
#else
    time_t sec = timegm(&tm);
#endif

    return (btr_time_point_s){
        .nanoseconds =
            (long long)sec * 1000000000LL +
            date->nanosecond
    };
}
