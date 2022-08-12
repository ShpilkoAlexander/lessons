#include "test.h"

#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

#include <string>
#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>

namespace test {

namespace transportCatalogue {

void DataFilling(TransportCatalogue& trans_catal) {
    Coordinates coordinates;
    Stop stop;
    double a = 0.2;
    double b = 1.4;
    for (double i = 0.0; i < 5; ++i) {
        stop.stopname = 'A' + i;
        coordinates.lat = a + i;
        coordinates.lng = b + i;

        stop.coordinates = coordinates;
        trans_catal.AddStop(stop);
    }

    trans_catal.AddBus("1", {"B", "C", "A", "A", "C", "B"});

}

void AddEndFindStop() {
    TransportCatalogue trans_catal;
    Stop stop;
    std::vector<std::string> stopnames = {"A", "B", "C"};

    for (const auto& name : stopnames) {
        stop.stopname = name;
        trans_catal.AddStop(std::move(stop));
    }
    assert(trans_catal.FindStop(stopnames[1])->stopname == stopnames[1]);
    assert(trans_catal.FindStop("Empty")->stopname == std::string());

}

void AddEndFindBus() {
    TransportCatalogue trans_catal;
    std::vector<std::string> stopnames = {"A", "B", "C"};
    Stop stop;
    for (const auto& name : stopnames) {
        stop.stopname = name;
        trans_catal.AddStop(std::move(stop));
    }

    Bus bus;
    std::vector<std::string> busnames = {"10", "13", "9"};

    for (const auto& name : busnames) {
        trans_catal.AddBus(std::move(name), {""});
    }

    assert(trans_catal.FindBus(busnames[1])->busname == busnames[1]);
    assert(trans_catal.FindBus("Empty")->busname == std::string());

    std::vector<std::string> buses_stop = {"B", "C", "A", "A", "C", "B"};
    std::vector<std::string> check_buses_stop;
    trans_catal.AddBus("Check Stops", buses_stop);

    for (const auto stop : trans_catal.FindBus("Check Stops")->stops) {
        check_buses_stop.push_back(stop->stopname);
    }

    assert(buses_stop == check_buses_stop);

}

void GetBusInfo() {
    TransportCatalogue trans_catal;
    DataFilling(trans_catal);
    BusInfo bus_info = trans_catal.GetBusInfo("1");
    assert(bus_info.stops_count == 6);
    assert(bus_info.uniq_stops_count == 3);
    //std::cout << "rout len:" << std::fixed << std::setprecision(6) << bus_info.route_len << std::endl;
}
} //transportCatalogue

namespace inputReader {

//void ProcessingRequest() {
//    TransportCatalogue trans_catal;

//    InputReader reader(trans_catal);

//    reader.ProcessingRequest(std::cin);

//    std::cout << "Stops: " << std::endl;
//    for (auto stop : trans_catal.stops_) {
//        std::cout << "Name:" << stop.stopname << " Coordinates: " << stop.coordinates.lat << " " << stop.coordinates.lng << std::endl;
//    }

//    std::cout << std::endl;

//    std::cout << "Buses: " << std::endl;
//    for (auto bus : trans_catal.buses_) {
//        std::cout << "Name:" << bus.busname << " ";
//        for (auto stop : bus.stops) {
//            std::cout << stop->stopname << " < ";
//        }
//        std::cout << std::endl;
//    }

//}

//void DivisionRequest() {
//    std::string type1 = "Stop ",
//                text1 = "Olushka: 12.23123, 32.32313",

//                type2 = "  Bus ",
//                text2 = "Stepanovo: 213, 1231";

//    InputReader reader(0);
//    Request check_stop = reader.DivisionRequest(type1 + text1);
//    Request check_bus = reader.DivisionRequest(type2 + text2);
//    Request check_error = reader.DivisionRequest("sdsds d");

//    assert(check_stop.type_ == Type::STOP && check_stop.text_ == text1);
//    assert(check_bus.type_ == Type::BUS && check_bus.text_ == text2);
//    assert(check_error.type_ == Type::ERROR_TYPE_NAME);
//}

//void ProcessingRequest() {

//    InputReader reader(3);

//    reader.ProcessingRequest(std::cin);

//    for (auto req : reader.GetRequests()) {
//        std::cout << req.type_  << ": " << req.text_<< std::endl;
//    }

//}

//void StopAddingProcessing() {

//    TransportCatalogue trans_cat;

//    Request req(Type::STOP, "fxBeRLlF vAnRrFDDEgA: 38.5794, 34.8782");
//    InputReader reader(trans_cat);
//    reader.StopAddingProcessing(req);
//    auto stop = trans_cat.FindStop("fxBeRLlF vAnRrFDDEgA");

//    assert(stop->stopname == "fxBeRLlF vAnRrFDDEgA");
//    assert(stop->coordinates.lat == 38.5794 && stop->coordinates.lng == 34.8782);

////    std::cout << "Stop: " << stop->stopname << std::endl;
////    std::cout << "Coordinates: " << stop->coordinates.lat << ", " << stop->coordinates.lng << std::endl;

//}

//void BusAddingProcessing() {

//    TransportCatalogue trans_cat;
//    Stop stop;
//    stop.stopname = "A";
//    trans_cat.AddStop(stop);

//    stop.stopname = "B";
//    trans_cat.AddStop(stop);

//    stop.stopname = "C";
//    trans_cat.AddStop(stop);

//    stop.stopname = "D";
//    trans_cat.AddStop(stop);

//    //Request req(Type::BUS, "135: A > B > C > D");
//    Request req(Type::BUS, "137: A - B - C");
//    InputReader reader(trans_cat);
//    reader.BusAddingProcessing(req);
//    const Bus* bus = trans_cat.FindBus("137");

//    std::cout << "Bus name: " << bus->busname << std::endl;
//    std::cout << "Route: ";
//    for (auto stopname : bus->stops) {
//        std::cout << stopname->stopname << " > ";
//    }
//    std::cout << std::endl;


////    std::cout << "Stop: " << stop->stopname << std::endl;
////    std::cout << "Coordinates: " << stop->coordinates.lat << ", " << stop->coordinates.lng << std::endl;

//}


} //inputReader

namespace statReader {



}
} //test


//Stop A: 38.53111, 34.895699
//Stop B: 31.12354, 32.657432
//Stop C: 42.31111, 31.131235
//Bus 12: A > B > C
//Bus 21: A - B - C
