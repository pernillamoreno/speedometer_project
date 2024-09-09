#ifndef SETTING_H
#define SETTING_H

#include <string>
#include <map>
#include <climits>

namespace Setting
{
    struct setting_t
    {
        const char *name;
        int min, max, start, length;

        constexpr setting_t() : name(nullptr), min(0), max(0), start(0), length(0) {}
        constexpr setting_t(const char *_name, int _min = 0, int _max = 0, int _start = 0, int _length = 0) : name(_name), min(_min), max(_max), start(_start), length(_length) {}
    };

    template <size_t N>
    struct SettingsSize
    {
        static constexpr size_t length(const setting_t *settings)
        {
            return settings[0].length + SettingsSize<N - 1>::length(settings + 1);
        }

        static constexpr size_t size(const setting_t *settings)
        {
            return (length(settings) % 8 == 0) ? (length(settings) / 8) : (length(settings) / 8 + 1);
        }
    };

    template <>
    struct SettingsSize<1>
    {
        static constexpr size_t length(const setting_t *settings)
        {
            return settings[0].length;
        }
    };

    class Signal
    {
        static constexpr setting_t settings[]{
            {"speed", 0, 240, 0, 8},
            {"temperature", -60, 60, 8, 7},
            {"battery_level", 0, 100, 15, 7},
            {"left_indicator", 0, 1, 22, 1},
            {"right_indicator", 0, 1, 23, 1},
        };

        std::map<std::string, setting_t> signalMap;


        Signal()
        {
            for (const auto &s : settings)
            {
                signalMap[s.name] = s;
            }
        }
        Signal(const Signal &) = delete;
        Signal &operator=(const Signal &) = delete;
        Signal(Signal &&) = delete;
        Signal &operator=(Signal &&) = delete;

    public:
        static constexpr size_t BUFSIZE{SettingsSize<sizeof(settings) / sizeof(settings[0])>::size(settings)};

        setting_t &operator[](const std::string &name)
        {
            return signalMap[name];
        }

        static Signal &getInstance()
        {
            static Signal instance;
            return instance;
        }
    };

    constexpr uint8_t INTERVAL = 50;

    namespace TCP
    {
        constexpr int PORT = 12345;
        constexpr char SERVER[]{"127.0.0.1"};
    }

    namespace UART
    {
        constexpr int BAUDRATE = 115200;
        constexpr char CLIENT_PORT[]{"/dev/ttyUSB1"};
        constexpr char SERVER_PORT[]{"/dev/ttyUSB0"};
    }
};
#endif // SETTING_H
