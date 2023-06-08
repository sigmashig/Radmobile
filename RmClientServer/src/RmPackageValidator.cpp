#include "RmPackageValidator.hpp"
#include "RmCommands.hpp"
#include "RmTypes.hpp"
#include <esp_event.h>
#include "RmId.hpp"
#include "RmGPS.hpp"

ESP_EVENT_DECLARE_BASE(RMPROTOCOL_EVENT);

RmPackage RmPackageValidator::Validate(String pkg, bool sendEvent)
{
    // try to detect command
    RmPackage package;
    // try to detect state
    package = StateValidation(pkg);
    if (package.pkgType == PackageType::PKG_STATE)
    {
        // command detected
        // send event
        if (sendEvent)
        {
            esp_event_post(RMPROTOCOL_EVENT, RMEVENT_STATE_RECEIVED, &(package.content.state), sizeof(CommandState), portMAX_DELAY);
        }
        return package;
    }
    // try to detect id
    package = IdValidation(pkg);
    if (package.pkgType == PackageType::PKG_ID)
    {
        if (sendEvent)
        {

            esp_event_post(RMPROTOCOL_EVENT, RMEVENT_ID_RECEIVED, &(package.content.id), sizeof(IdConfig), portMAX_DELAY);
        }
        return package;
    }
    // try to detect gps
    package = GpsValidation(pkg);
    if (package.pkgType == PackageType::PKG_GPS)
    {
        if (sendEvent)
        {
            esp_event_post(RMPROTOCOL_EVENT, RMEVENT_GPS_RECEIVED, &(package.content.gps), sizeof(GpsPosition), portMAX_DELAY);
        }
        return package;
    }
    package.pkgType = PackageType::PKG_INVALID;
    return package;
}

RmPackage RmPackageValidator::IdValidation(String pkg)
{
    // (X#AA,01,aA)
    RmPackage package;
    IdConfig id = RmId::StringToId(pkg);
    if (id.isValid)
    {
        package.pkgType = PackageType::PKG_ID;
        package.content.id = id;
    }
    else
    {
        package.pkgType = PackageType::PKG_INVALID;
    }

    return package;
}

RmPackage RmPackageValidator::StateValidation(String pkg)
{
    CommandState state = RmCommands::StringToState(pkg);
    RmPackage package;
    package.pkgType = PackageType::PKG_INVALID;
    if (state.isValid)
    {
        // send event
        package.pkgType = PackageType::PKG_STATE;
        package.content.state = state;
    }
    return package;
}

RmPackage RmPackageValidator::GpsValidation(String pkg)
{
    GpsPosition gps = RmGPS::StringAsGps(pkg);
    RmPackage package;
    package.pkgType = PackageType::PKG_INVALID;
    if (gps.isValid)
    {
        // send event
        package.pkgType = PackageType::PKG_GPS;
        package.content.gps = gps;
    }
    return package;
}
