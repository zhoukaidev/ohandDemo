#pragma once

#include <ArduinoJson.h>

StaticJsonDocument<200> generate_device_ping_msg(int runcount)
{
    StaticJsonDocument<200> doc;
    doc["interval"] = 20;
    doc["ssh"] = "";
    doc["jupyter"] = "";
    doc["cpu"] = 100;
    doc["memory"] = 100;
    doc["disk"] = 100;
    doc["macAddress"] = "012345";
    doc["ip"] = "012345";
    doc["running"] = 200;
    return doc;
}
