#include <iostream>

#include "transport_catalogue.h"
#include "json_reader.h"

#include <iostream>

using namespace std;

int main()
{
    TransportCatalogue db;
    JsonReader proccessing_json(db);

    //Чтение Json
    proccessing_json.ReadJson(std::cin);

    //Загрузка данных в транспортный каталог
    proccessing_json.LoadData();

    //Обработка запросов
    proccessing_json.ProcessingRequest();

    //Вывод JSON-массива ответов
    proccessing_json.PrintResponseArray(std::cout);

}
    /*
     * Примерная структура программы:
     *
     * Считать JSON из stdin
     * Построить на его основе JSON базу данных транспортного справочника
     * Выполнить запросы к справочнику, находящиеся в массиве "stat_requests", построив JSON-массив
     * с ответами.
     * Вывести в stdout ответы в виде JSON
     */
