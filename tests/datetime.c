#include "btrstd/datetime.h"
#include "stdbool.h"
#include "stdio.h"
#include "assert.h"

// test duration constructors and converters
static void test1(void)
{
    printf("> test1\n");

    assert(BTR_Duration_asNansec(BTR_Nansec(42)) == 42);
    // 1 sec = 1e9 ns
    assert(BTR_Duration_asNansec(BTR_Sec(1)) == 1000000000ULL);
    // 1 ms = 1e6 ns
    assert(BTR_Duration_asNansec(BTR_Milsec(1)) == 1000000ULL);
    // 1 us = 1000 ns
    assert(BTR_Duration_asNansec(BTR_Micsec(1)) == 1000ULL);
    // 1 min = 60 sec
    assert(BTR_Duration_asNansec(BTR_Min(1)) == 60000000000ULL);
    // 1 hour = 3600 sec
    assert(BTR_Duration_asNansec(BTR_Hour(1)) == 3600000000000ULL);

    // round-trip converters
    assert(BTR_Duration_asSec(BTR_Sec(2.5)) == 2.5);
    assert(BTR_Duration_asMilsec(BTR_Milsec(500)) == 500.0);
    assert(BTR_Duration_asMicsec(BTR_Micsec(250)) == 250.0);
}

// test duration arithmetic
static void test2(void)
{
    printf("> test2\n");

    btr_duration_s a = BTR_Sec(10);
    btr_duration_s b = BTR_Sec(3);

    btr_duration_s sum = BTR_Duration_add(a, b);
    assert(BTR_Duration_asSec(sum) == 13.0);

    btr_duration_s diff = BTR_Duration_sub(a, b);
    assert(BTR_Duration_asSec(diff) == 7.0);

    btr_duration_s mul = BTR_Duration_mul(BTR_Sec(3), BTR_Sec(2));
    assert(BTR_Duration_asNansec(mul) == 3000000000ULL * 2000000000ULL);

    btr_duration_s div = BTR_Duration_div(a, BTR_Sec(2));
    assert(BTR_Duration_asNansec(div) == 5);
}

// test Time_now returns a reasonable value (after Unix epoch)
static void test3(void)
{
    printf("> test3\n");

    btr_time_point_s now = BTR_Time_now();
    // Must be positive (after 1970 epoch)
    assert(now.nanoseconds > 0);
    // Must be after year 2000 (~946684800 seconds = ~946684800000000000 ns)
    assert(now.nanoseconds > 946684800000000000ULL);
}

// test Time_monotonic returns increasing values
static void test4(void)
{
    printf("> test4\n");

    btr_time_point_s a = BTR_Time_monotonic();
    btr_time_point_s b = BTR_Time_monotonic();
    assert(b.nanoseconds >= a.nanoseconds);

    // small delay should show increase
    BTR_sleep(BTR_Milsec(10));
    btr_time_point_s c = BTR_Time_monotonic();
    assert(c.nanoseconds > a.nanoseconds);
}

// test Time_diff
static void test5(void)
{
    printf("> test5\n");

    btr_time_point_s t0 = { .nanoseconds = 1000 };
    btr_time_point_s t1 = { .nanoseconds = 5000 };

    btr_duration_s diff = BTR_Time_diff(t0, t1);
    assert(diff.nanoseconds == 4000);

    // commutative
    diff = BTR_Time_diff(t1, t0);
    assert(diff.nanoseconds == 4000);
}

// test Time_since
static void test6(void)
{
    printf("> test6\n");

    btr_time_point_s start = BTR_Time_now();
    BTR_sleep(BTR_Milsec(10));
    btr_duration_s elapsed = BTR_Time_since(start);
    assert(elapsed.nanoseconds >= 10000000ULL); // at least 10ms
}

// test Timer
static void test7(void)
{
    printf("> test7\n");

    btr_timer_s timer = BTR_Timer_start();
    BTR_sleep(BTR_Milsec(10));
    btr_duration_s elapsed = BTR_Timer_elapsed(&timer);
    assert(elapsed.nanoseconds >= 10000000ULL); // at least 10ms

    // reset and check
    BTR_Timer_reset(&timer);
    btr_duration_s afterReset = BTR_Timer_elapsed(&timer);
    assert(afterReset.nanoseconds < 100000000ULL); // less than 100ms
}

// test sleep duration is approximately correct
static void test8(void)
{
    printf("> test8\n");

    btr_time_point_s before = BTR_Time_monotonic();
    BTR_sleep(BTR_Milsec(50));
    btr_time_point_s after = BTR_Time_monotonic();

    btr_duration_s actual = BTR_Time_diff(before, after);
    // should be at least 50ms, with generous tolerance
    assert(actual.nanoseconds >= 45000000ULL);
    // should complete within 1 second
    assert(actual.nanoseconds < 1000000000ULL);
}

// test DateTime_fromTimestamp at epoch
static void test9(void)
{
    printf("> test9\n");

    btr_time_point_s epoch = { .nanoseconds = 0 };
    btr_datetime_s dt = BTR_DateTime_fromTimestamp(epoch);

    assert(dt.year == 1970);
    assert(dt.month == 1);
    assert(dt.day == 1);
    assert(dt.hour == 0);
    assert(dt.minute == 0);
    assert(dt.second == 0);
    assert(dt.nanosecond == 0);
}

// test DateTime roundtrip: timestamp -> datetime -> timestamp
static void test10(void)
{
    printf("> test10\n");

    btr_time_point_s original = { .nanoseconds = 1000000000ULL }; // 1 sec after epoch
    btr_datetime_s dt = BTR_DateTime_fromTimestamp(original);

    assert(dt.year == 1970);
    assert(dt.month == 1);
    assert(dt.day == 1);
    assert(dt.hour == 0);
    assert(dt.minute == 0);
    assert(dt.second == 1);

    btr_time_point_s back = BTR_DateTime_toTimestamp(&dt);
    assert(back.nanoseconds == original.nanoseconds);
}

// test DateTime roundtrip with sub-second precision
static void test11(void)
{
    printf("> test11\n");

    btr_time_point_s original = { .nanoseconds = 123456789ULL };
    btr_datetime_s dt = BTR_DateTime_fromTimestamp(original);

    assert(dt.year == 1970);
    assert(dt.month == 1);
    assert(dt.day == 1);
    assert(dt.hour == 0);
    assert(dt.minute == 0);
    assert(dt.second == 0);
    assert(dt.nanosecond == 123456789);

    btr_time_point_s back = BTR_DateTime_toTimestamp(&dt);
    assert(back.nanoseconds == original.nanoseconds);
}

// test DateTime roundtrip with a modern date
static void test12(void)
{
    printf("> test12\n");

    btr_datetime_s dt = {
        .year = 2025, .month = 6, .day = 15,
        .hour = 10, .minute = 30, .second = 45,
        .nanosecond = 500000000
    };

    btr_time_point_s ts = BTR_DateTime_toTimestamp(&dt);
    btr_datetime_s back = BTR_DateTime_fromTimestamp(ts);

    assert(back.year == dt.year);
    assert(back.month == dt.month);
    assert(back.day == dt.day);
    assert(back.hour == dt.hour);
    assert(back.minute == dt.minute);
    assert(back.second == dt.second);
    assert(back.nanosecond == dt.nanosecond);
}

// test DateTime_now returns a sensible value
static void test13(void)
{
    printf("> test13\n");

    btr_datetime_s now = BTR_DateTime_now();
    assert(now.year >= 2026);
    assert(now.month >= 1 && now.month <= 12);
    assert(now.day >= 1 && now.day <= 31);
    assert(now.hour <= 23);
    assert(now.minute <= 59);
    assert(now.second <= 60);
}

int main(void)
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    test10();
    test11();
    test12();
    test13();
    printf("SUCCESS\n");
    return 0;
}
