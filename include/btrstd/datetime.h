#pragma once

typedef struct BTR_Duration {
    unsigned long long nanoseconds;
} btr_duration_s;

typedef struct BTR_TimePoint {
    unsigned long long nanoseconds;
} btr_time_point_s;

// Returns a time point which is represented by the amount of
// nanoseconds since the Epoch, 1970-01-01 00:00:00 +0000 UTC.
btr_time_point_s BTR_Time_now(void);
// Returns a time point which represents the amount of
// time passed from some unspecified point in the past.
btr_time_point_s BTR_Time_monotonic(void);

// BTR_Duration constructors
btr_duration_s BTR_Hour(double);
btr_duration_s BTR_Min(double);
btr_duration_s BTR_Sec(double);
btr_duration_s BTR_Milsec(double);
btr_duration_s BTR_Micsec(double);
btr_duration_s BTR_Nansec(unsigned long long);

// BTR_Duration converters
double BTR_Duration_asSec(btr_duration_s);
double BTR_Duration_asMilsec(btr_duration_s);
double BTR_Duration_asMicsec(btr_duration_s);
unsigned long long BTR_Duration_asNansec(btr_duration_s);

btr_duration_s BTR_Duration_add(btr_duration_s, btr_duration_s);
btr_duration_s BTR_Duration_sub(btr_duration_s, btr_duration_s);
btr_duration_s BTR_Duration_mul(btr_duration_s, btr_duration_s);
btr_duration_s BTR_Duration_div(btr_duration_s, btr_duration_s);

// Returns the difference between two time points.
btr_duration_s BTR_Time_diff(btr_time_point_s a, btr_time_point_s b);
// Returns the elapsed duration since the given time point.
btr_duration_s BTR_Time_since(btr_time_point_s start);

// Suspends the execution of the calling thread for
// the given amount of time.
void BTR_sleep(btr_duration_s);

typedef struct BTR_Timer {
    btr_time_point_s started;
} btr_timer_s;

// Creates and starts a new timer.
btr_timer_s BTR_Timer_start(void);
// Resets the timer to zero.
void BTR_Timer_reset(btr_timer_s *);
// Returns the duration since the timer was started.
btr_duration_s BTR_Timer_elapsed(const btr_timer_s *);

typedef struct BTR_DateTime {
    unsigned int year;
    unsigned int month;   // 1-12
    unsigned int day;     // 1-31

    unsigned int hour;    // 0-23
    unsigned int minute;  // 0-59
    unsigned int second;  // 0-60 (leap second)
    unsigned int nanosecond;
} btr_datetime_s;

// Returns the current UTC datetime
btr_datetime_s BTR_DateTime_now(void);
// Returns the current local datetime
btr_datetime_s BTR_DateTime_localNow(void);
// Returns the datetime created from a time point
btr_datetime_s BTR_DateTime_fromTimestamp(btr_time_point_s);
// Returns the local datetime created from a time point
btr_datetime_s BTR_DateTime_fromLocalTimestamp(btr_time_point_s);
// Returns a time point created from a datetime.
btr_time_point_s BTR_DateTime_toTimestamp(const btr_datetime_s *);
