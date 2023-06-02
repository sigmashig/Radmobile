#include "RmVehicle.hpp"
#include "SigmaLoger.hpp"

ESP_EVENT_DECLARE_BASE(RMPROTOCOL_EVENT);

RmVehicle::RmVehicle()
{
    esp_event_handler_instance_register(RMPROTOCOL_EVENT, ESP_EVENT_ANY_ID, stateReceived, NULL, NULL);
}

void RmVehicle::stateReceived(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{

    switch (event_id)
    {
    case RMEVENT_STATE_RECEIVED:
    {
        Log->Debug(F("RmVehicle::stateReceived"));
        CommandState *state = (CommandState *)event_data;
        rmVehicle->ApplyState(*state);
        break;
    }
    case RMEVENT_PID_CORRECTION_LEFT:
    {
        int *x = (int *)event_data;
        Log->Printf("RmVehicle PID correction left: %f\n", *x).Debug();
        rmVehicle->ApplyCorrection(DIRECTION_LEFT, *x);
        break;
    }
    case RMEVENT_PID_CORRECTION_RIGHT:
    {
        int *x = (int *)event_data;
        Log->Printf("RmVehicle PID correction right: %f\n", *x).Debug();
        rmVehicle->ApplyCorrection(DIRECTION_RIGHT, *x);
        break;
    }
    case RMEVENT_PID_CORRECTION_PITCH:
    case RMEVENT_PID_CORRECTION_ROLL:
        Log->Debug(F("RmVehicle::correction PITCH/ROLL"));
        break;
    }
}

//--------------------------------

RmVehicle *rmVehicle = NULL;