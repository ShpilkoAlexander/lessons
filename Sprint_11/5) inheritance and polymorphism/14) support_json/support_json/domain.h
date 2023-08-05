#pragma once

#include "geo.h"

#include <string>
#include <vector>
#include <set>

struct Stop {
    std::string stopname;
    geo::Coordinates coordinates;
};

struct Bus {
    std::string busname;
    std::vector<const Stop*> stops;

    bool operator==(const Bus& other) const {
        return busname == other.busname;
    }
    bool operator!=(const Bus& other) const {
        return !(*this == other);
    }
};

struct BusInfo {
    std::string busname;
    size_t stops_count;
    size_t uniq_stops_count;
    double route_len;
    double curvature;
    bool is_found = true;
};

struct StopInfo {
    std::string stopname;
    std::set<std::string_view> buses;
    bool is_found = true;
};

enum Type{
    STOP,
    BUS,
    ERROR_TYPE_NAME
};

struct Request {
    Type type_;
    std::string text_;

    Request(Type type, std::string text) :
        type_(type), text_(text){

    }
};


using PairStops = std::pair<const Stop*, const Stop*>;

struct PairStopsHasher {
    size_t operator() (const PairStops& pair_stop) const {
        size_t h_first = stop_hasher_(pair_stop.first);
        size_t h_second = stop_hasher_(pair_stop.second);

        return h_first + h_second * 37;
    }
private:
    std::hash<const Stop*> stop_hasher_;
};
