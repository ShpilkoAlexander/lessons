#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

ostream& operator<<(ostream& output, vector<string> vec){
    for(const auto& el : vec){
        output << " "s << el;
    }
    output << endl;
    return output;
}

istream& operator>>(istream& input, Query& q) {
    // Реализуйте эту функцию
    string type;
    input >> type;
    if(type == "NEW_BUS"s){
        q.type = QueryType::NewBus;
        q.stops = {};
        q.bus = "";
        input >> q.bus;
        string stop;
        int count_stops = 0;
        input >> count_stops;
        for(int i = 0; i < count_stops; ++i){
            input >> stop;
            q.stops.push_back(stop);
        }

    }
    else if (type == "BUSES_FOR_STOP"s) {
        q.stop = "";
        q.type = QueryType::BusesForStop;
        input >> q.stop;
    }
    else if (type == "STOPS_FOR_BUS"s) {
        q.bus = "";
        q.type = QueryType::StopsForBus;
        input >> q.bus;
    }
    else if (type == "ALL_BUSES"s) {
        q.type = QueryType::AllBuses;
    }
    return input;
}

struct BusesForStopResponse {
    // Наполните полями эту структуру
    vector<string> buses;
};

ostream& operator<<(ostream& output, const BusesForStopResponse& r) {
    // Реализуйте эту функцию
    if (r.buses.empty()){
        output << "No stop"s << endl;
    }
    else {
        bool isFirst = true;
        for(string bus : r.buses){
            if(isFirst){
                output << bus;
                isFirst = false;
                continue;
            }
            output << " " << bus;
        }
        output << endl;
    }
    return output;
}

struct StopsForBusResponse {
    // Наполните полями эту структуру
    map<string, vector<string>> stops_for_bus;
    vector <string> stops;
};

ostream& operator<<(ostream& output, const StopsForBusResponse& r) {
    // Реализуйте эту функцию
    if(r.stops_for_bus.empty()){
        output << "No bus"s << endl;
    }
    else{
        for(const auto& stop: r.stops){
            output << "Stop "s << stop << ":"s;
            if(r.stops_for_bus.at(stop).empty()){
                output << " no interchange"s << endl;
                continue;
            }
            output << r.stops_for_bus.at(stop);
        }
    }
    return output;
}

struct AllBusesResponse {
    // Наполните полями эту структуру
    map<string, vector<string>> all_buses;
};

ostream& operator<<(ostream& output, const AllBusesResponse& r) {
    // Реализуйте эту функцию
    if(r.all_buses.empty()){
        output << "No buses"s << endl;
    }
    else {
        for(const auto& [bus, stops] : r.all_buses){
            output << "Bus "s << bus <<":"s << stops;
        }
    }
    return output;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        stops_to_buses[bus] = stops;
        for(const auto& stop : stops){
            buses_to_stops[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse buses;
        if(buses_to_stops.count(stop)){
            buses.buses = buses_to_stops.at(stop);
        }
        return buses;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        // Реализуйте этот метод
        StopsForBusResponse stops;
        if (stops_to_buses.count(bus)){
            stops.stops = stops_to_buses.at(bus);
            for(const auto& stop : stops_to_buses.at(bus)){
                for(const auto& bus_stop : buses_to_stops.at(stop)){
                    stops.stops_for_bus[stop];
                    if(bus_stop != bus){
                        stops.stops_for_bus[stop].push_back(bus_stop);
                    }
                }
            }
        }
        return stops;

    }

    AllBusesResponse GetAllBuses() const {
        // Реализуйте этот метод
        AllBusesResponse all_buses;
        all_buses.all_buses = stops_to_buses;
        return all_buses;
    }
private:
    map<string, vector<string>> stops_to_buses, buses_to_stops;
};



void TestStopsForBusOutput() {
    StopsForBusResponse Stops;
    ostringstream output;

    output << Stops;
    assert(output.str() == "No bus\n"s);
    output.str(""s);

    Stops.stops_for_bus["Vnukovo"s];
    output << Stops;
    assert(output.str() == "Stop Vnukovo: no interchange\n"s);
    output.str(""s);

    Stops.stops_for_bus["Vnukovo"s] = {"1"s, "99"s, "101"s};
    output << Stops;
    assert(output.str() == "Stop Vnukovo: 1 99 101\n"s);

}

void TestAllBusesOutput() {
    AllBusesResponse Buses;
    ostringstream output;

    output << Buses;
    assert(output.str() == "No buses\n");
    output.str("") ;

    Buses.all_buses["272"] = {"Vnukovo", "Kol", "Dver"};
    output << Buses;
    assert(output.str() == "Bus 272: Vnukovo Kol Dver\n");
    output.str("");
}

void TestQueryInput() {
    istringstream input;
    Query q;

    input.str("NEW_BUS 32 3 Tolstopaltsevo Marushkino Vnukovo"s);
    input >> q;
    assert(q.type == QueryType::NewBus);
    assert(q.bus == "32"s);
    vector<string> stops = {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s};
    assert(q.stops == stops);
    input.clear();

    input.str("BUSES_FOR_STOP Vnukovo"s);
    input >> q;
    assert(q.type == QueryType::BusesForStop);
    assert(q.stop == "Vnukovo"s);
    input.clear();

    input.str("STOPS_FOR_BUS 87"s);
    input >> q;
    assert(q.type == QueryType::StopsForBus);
    assert(q.bus == "87"s);
    input.clear();

    input.str("ALL_BUSES"s);
    input >> q;
    assert(q.type == QueryType::AllBuses);
    input.clear();


}

void TestBusManager(){
    BusManager manager;
    string vnukovo = "Vnukovo"s, strela = "Strela"s, rog = "rog"s;
    vector<string> stops = {vnukovo, strela, rog};

    manager.AddBus("34"s, stops);
//    assert(manager.stops_to_buses["34"s] == stops);
//    assert(manager.buses_to_stops[vnukovo][0] == "34"s);

    ostringstream output;
    output << manager.GetBusesForStop(vnukovo);
    assert(output.str() == "34 \n");


}

void Test() {
    TestQueryInput();
    TestStopsForBusOutput();
    TestAllBusesOutput();
    TestBusManager();
}


// Не меняя тела функции main, реализуйте функции и классы выше

int main() {


    //  Test();


    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop);
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus);
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses();
                break;
        }
    }
}
