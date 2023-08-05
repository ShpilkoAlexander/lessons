#include "request_handler.h"

/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */

// MapRenderer понадобится в следующей части итогового проекта
RequestHandler::RequestHandler(const TransportCatalogue& db /*, const renderer::MapRenderer& renderer*/)
    : db_(db) {
}

// Возвращает информацию о маршруте (запрос Bus)
std::optional<BusInfo> RequestHandler::GetBusStat(const std::string_view& bus_name) const {
    return db_.GetBusInfo(bus_name);
}

// Возвращает маршруты, проходящие через
const std::set<std::string_view> RequestHandler::GetBusesByStop(const std::string_view& stop_name) const {

    return db_.GetStopInfo(stop_name).buses;
}

// Этот метод будет нужен в следующей части итогового проекта
//svg::Document RenderMap() const;
