#pragma once
#include <Arduino.h>
#include <RmTypes.hpp>

class RmPackageValidator
{
public:
    // RmPackageValidator();
    static RmPackage Validate(String pkg, bool sendEvent = true);
    static RmPackage IdValidation(String pkg);
    static RmPackage StateValidation(String pkg);
    static RmPackage GpsValidation(String pkg);
};

// extern RmPackageValidator *rmPackageValidator;