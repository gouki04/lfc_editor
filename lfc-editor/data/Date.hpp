#pragma once
#include <ctime>
#include <cmath>

namespace lfc
{
    struct DiffDate
    {
        double total_seconds;

        int TotalYears()
        {
            if (IsValid()) {
                return (int)floor(total_seconds / (24.0 * 60 * 60) / 365.0);
            }
            else {
                return -1;
            }
        }

        bool IsValid()
        {
            return total_seconds >= 0;
        }

        DiffDate()
        {
            total_seconds = -1;
        }

        DiffDate(double sec)
        {
            total_seconds = sec;
        }
    };

    struct Date
    {
        int year;
        int month;
        int day;

        Date()
        {
            year = -1;
            month = -1;
            day = -1;
        }

        Date(int y, int m, int d)
        {
            year = y;
            month = m;
            day = d;
        }

        void Set(int y, int m, int d)
        {
            year = y;
            month = m;
            day = d;
        }

        bool IsValid()
        {
            return year >= 1900 || month > 0 || day > 0;
        }

        static Date Now()
        {
            auto now = std::time(nullptr);
            auto tm = std::gmtime(&now);
            return Date((int)(tm->tm_year + 1900), (int)(tm->tm_mon + 1), (int)(tm->tm_mday));
        }

        struct tm ToTM()
        {
            struct tm ret;
            ret.tm_year = year - 1900;
            ret.tm_mon = month - 1;
            ret.tm_mday = day;
            ret.tm_hour = 0;
            ret.tm_min = 0;
            ret.tm_sec = 0;
            ret.tm_isdst = -1;

            return ret;
        }

        time_t ToTime_t()
        {
            auto time_m = ToTM();
            return mktime(&time_m);
        }

        DiffDate operator-(Date end)
        {
            if (IsValid() && end.IsValid()) {
                auto diff = difftime(ToTime_t(), end.ToTime_t());
                return DiffDate(diff);
            }
            else {
                return DiffDate();
            }
        }

        bool operator==(const Date &other)
        {
            return year == other.year && month == other.month && day == other.day;
        }

        bool operator!=(const Date &other)
        {
            return !(*this == other);
        }

        bool operator<(const Date &other)
        {
            if (year == other.year) {
                if (month == other.month) {
                    return day < other.day;
                }

                return month < other.month;
            }

            return year < other.year;
        }

        bool operator<=(const Date &other)
        {
            if (year == other.year) {
                if (month == other.month) {
                    return day <= other.day;
                }

                return month < other.month;
            }

            return year < other.year;
        }

        bool operator>(const Date &other)
        {
            return !(*this <= other);
        }

        bool operator>=(const Date &other)
        {
            return !(*this < other);
        }

        template<class Archive>
        void serialize(Archive &archive)
        {
            archive(CEREAL_NVP(year), CEREAL_NVP(month), CEREAL_NVP(day));
        }
    };

    struct DateRange
    {
        std::string name;
        Date begin;
        Date end;

        bool Contains(Date date) 
        {
            if (!begin.IsValid()) {
                if (!end.IsValid()) {
                    return true;
                }
                else {
                    return date <= end;
                }
            }
            else {
                if (!end.IsValid()) {
                    return date >= begin;
                }
                else {
                    return date >= begin && date <= end;
                }
            }
        }
    };
}