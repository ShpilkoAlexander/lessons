#pragma once

#include "transport_catalogue.h"
#include "input_reader.h"

namespace test {

namespace transportCatalogue {

void DataFilling(TransportCatalogue& trans_catal);

void AddEndFindStop();

void AddEndFindBus();

void GetBusInfo();

}

namespace inputReader {

void DivisionRequest();

void StopAddingProcessing();

void BusAddingProcessing();

void ProcessingRequest();

} //inputReader

namespace statReader {

//void DataOutput() {

} //statReader

}

