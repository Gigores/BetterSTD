#pragma once

typedef struct BTR_Duration {
    long long nanoseconds;
} btr_duration_s;

typedef struct BTR_TimePoint {
    long long nanoseconds;
} btr_time_point_s;

typedef struct BTR_Timer {
    btr_time_point_s started;
} btr_timer_s;

typedef struct BTR_DateTime {
    int year;
    int month;   // 1-12
    int day;     // 1-31

    int hour;    // 0-23
    int minute;  // 0-59
    int second;  // 0-60 (leap second)
    int nanosecond;
} btr_datetime_s;

btr_time_point_s BTR_Time_now(void);  // !!!
btr_time_point_s BTR_Time_monotonic(void);  // !!!

btr_duration_s BTR_Seconds(double);  // !!!
btr_duration_s BTR_Milliseconds(double);  // !!!
btr_duration_s BTR_Microseconds(double);
btr_duration_s BTR_Nanoseconds(long long);

double BTR_Duration_asSeconds(btr_duration_s);  // !!!
double BTR_Duration_asMilliseconds(btr_duration_s);
double BTR_Duration_asMicroseconds(btr_duration_s);
long long BTR_Duration_asNanoseconds(btr_duration_s);

btr_duration_s BTR_Duration_add(btr_duration_s, btr_duration_s);
btr_duration_s BTR_Duration_sub(btr_duration_s, btr_duration_s);
btr_duration_s BTR_Duration_mul(btr_duration_s, btr_duration_s);
btr_duration_s BTR_Duration_div(btr_duration_s, btr_duration_s);

btr_duration_s BTR_Time_diff(btr_time_point_s a, btr_time_point_s b);  // !!!
btr_duration_s BTR_Time_since(btr_time_point_s start);

void BTR_sleep(btr_duration_s);  // !!!

btr_timer_s BTR_Timer_start(void);  // !!!
void BTR_Timer_reset(btr_timer_s *);
btr_duration_s BTR_Timer_elapsed(const btr_timer_s *);  // !!!

btr_datetime_s BTR_DateTime_now(void);
btr_datetime_s BTR_DateTime_fromTimestamp(btr_time_point_s);
btr_time_point_s BTR_DateTime_toTimestamp(const btr_datetime_s *);
