#include "json_reader.h"

#include "transport_catalogue.h"

JsonReader::JsonReader(TransportCatalogue db)
    : db_(db) {
}

//Загрузка данных в транспортный каталог
void JsonReader::LoadData() {
    for (const auto& request : base_requests_) {
        if (request.AsMap().at("type").AsString() == "Stop") {
            StopAdding(request.AsMap());
        }
    }
    for (const auto& request : base_requests_) {
        if (request.AsMap().at("type").AsString() == "Bus") {
                    BusAdding(request.AsMap());
        }
    }

    DistanceAdding();
}


//Чтение Json и определение base_requests_ и stat_requests_
void JsonReader::ReadJson(std::istream& input) {
    json::Document read_data = json::Load(input);

    base_requests_ = read_data.GetRoot().AsMap().at("base_requests").AsArray();
    stat_requests_ = read_data.GetRoot().AsMap().at("stat_requests").AsArray();
}

//Обработка запроса на добавления остановки
void JsonReader::StopAdding(const json::Dict& stop_info) {
    Stop stop;

    stop.stopname = stop_info.at("name").AsString();

    stop.coordinates.lat = stop_info.at("latitude").AsDouble();
    stop.coordinates.lng = stop_info.at("longitude").AsDouble();

    StopsWithDistances stops_with_distance;
    stops_with_distance.stop = db_.AddStop(std::move(stop));
    stops_with_distance.distances = stop_info.at("road_distances").AsMap();

    stops_with_distance_.push_back(std::move(stops_with_distance));
}

//Обработка запроса на добавления остановки
void JsonReader::BusAdding(const json::Dict& bus_info) {
    std::vector<std::string> stopnames;

    for (auto stopname : bus_info.at("stops").AsArray()) {
        stopnames.push_back(stopname.AsString());
    }

    if (!bus_info.at("is_roundtrip").AsBool()) {
        std::vector<std::string> reverse_name;
        std::for_each(std::next(stopnames.rbegin()), stopnames.rend(),
                      [&reverse_name] (auto& name) {reverse_name.push_back(name);}
        );
        stopnames.insert(stopnames.end(), reverse_name.begin(), reverse_name.end());
    }

    db_.AddBus(bus_info.at("name").AsString(), stopnames);
}

void JsonReader::DistanceAdding() {
   for (const auto& stop_distances : stops_with_distance_) {
       if (stop_distances.distances.empty()) {
           continue;
       }
       StopsDistancesAdding(stop_distances.stop, stop_distances.distances);
   }
}

void JsonReader::StopsDistancesAdding(const Stop* stop, const json::Dict& distances) {

    for (const auto& [stopname, distance] : distances) {
        db_.SetDistancesToStops(PairStops{stop, db_.FindStop(stopname)}, distance.AsInt());
    }
}

//Обработка запроса об остановке
void JsonReader::ProccessingStopRequest(const json::Dict& stop_request) {
    StopInfo stop_info = db_.GetStopInfo(stop_request.at("name").AsString());
    json::Dict response;
    if (stop_info.is_found) {
        json::Array node_buses;

        for (auto bus : stop_info.buses) {
            node_buses.push_back(json::Node(std::string(bus)));
        }
        response["buses"] = node_buses;
    }
    else {
        response["error_message"] = "not found";
    }
    response["request_id"] = stop_request.at("id").AsInt();
    response_array_.push_back(json::Node{response});
}

//Обработка запроса о маршруте
void JsonReader::ProccessingBusRequest(const json::Dict& bus_request) {
    BusInfo bus_info = db_.GetBusInfo(bus_request.at("name").AsString());
    json::Dict response;

    if (bus_info.is_found) {
        response["curvature"] = json::Node{bus_info.curvature};
        response["route_length"] = json::Node{bus_info.route_len};
        response["stop_count"] = json::Node{int(bus_info.stops_count)};
        response["unique_stop_count"] = json::Node{int(bus_info.uniq_stops_count)};
    }
    else {
        response["error_message"] = "not found";
    }
    response["request_id"] = bus_request.at("id").AsInt();
    response_array_.push_back(json::Node{response});
}


//Обработка запросов
void JsonReader::ProcessingRequest() {
    for (const auto& request : stat_requests_) {
        if (request.AsMap().at("type").AsString() == "Stop") {
            ProccessingStopRequest(request.AsMap());
        }
        else if (request.AsMap().at("type").AsString() == "Bus") {
            ProccessingBusRequest(request.AsMap());
        }
        else {
            throw std::runtime_error("Proccessing requests error");
        }
    }
}

//Вывод JSON-массива ответов
void JsonReader::PrintResponseArray(std::ostream& output) {
    json::PrintNode(response_array_, output);

}
