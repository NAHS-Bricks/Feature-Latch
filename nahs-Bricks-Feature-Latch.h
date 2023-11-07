#ifndef NAHS_BRICKS_FEATURE_LATCH_H
#define NAHS_BRICKS_FEATURE_LATCH_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <nahs-Bricks-Lib-CoIC.h>
#include <nahs-Bricks-Feature-BaseClass.h>
#include <nahs-Bricks-Lib-RTCmem.h>

class NahsBricksFeatureLatch : public NahsBricksFeatureBaseClass {
    private:  // Variables
        static const uint16_t version = 1;
        NahsBricksLibCoIC_Latch *_latch;
        uint8_t _latchPins[8];
        uint8_t _latchPinsCount = 0;

    public: // BaseClass implementations
        NahsBricksFeatureLatch();
        String getName();
        uint16_t getVersion();
        void begin();
        void start();
        void deliver(JsonDocument* out_json);
        void feedback(JsonDocument* in_json);
        void end();
        void printRTCdata();
        void printFSdata();
        void brickSetupHandover();

    public:  // Brick-Specific setter
        void assignLatch(NahsBricksLibCoIC_Latch &latch);
        void assignLatchPin(uint8_t pinIndex);

    private:  // internal Helpers

    private:  // BrickSetup Helpers
        void _printMenu();
        void _readLatchStates();
};

#if !defined(NO_GLOBAL_INSTANCES)
extern NahsBricksFeatureLatch FeatureLatch;
#endif

#endif // NAHS_BRICKS_FEATURE_LATCH_H
