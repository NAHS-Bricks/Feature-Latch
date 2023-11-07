#include <nahs-Bricks-Feature-Latch.h>
#include <nahs-Bricks-Lib-SerHelp.h>

NahsBricksFeatureLatch::NahsBricksFeatureLatch() {
}

/*
Returns name of feature
*/
String NahsBricksFeatureLatch::getName() {
    return "latch";
}

/*
Returns version of feature
*/
uint16_t NahsBricksFeatureLatch::getVersion() {
    return version;
}

/*
Configures FSmem und RTCmem variables (prepares feature to be fully operational)
*/
void NahsBricksFeatureLatch::begin() {
    // Enableing interrupt function on latch if brick just started up
    if(!RTCmem.isValid()) {
        _latch->setInterrupt(NahsBricksLibCoIC_Latch::INT_CTL::LOW_SIG);
    }
}

/*
Starts background processes like fetching data from other components
*/
void NahsBricksFeatureLatch::start() {
    // Start the Conversion with Latch as data preparation may take some time
    _latch->conversionBegin();
}

/*
Adds data to outgoing json, that is send to BrickServer
*/
void NahsBricksFeatureLatch::deliver(JsonDocument* out_json) {
    // wait for data preparation to complete and deliver data
    JsonArray l_array = out_json->createNestedArray("l");
    while(!_latch->readyToFetchStates()) delay(1);
    _latch->fetchLatchStates();
    for(uint8_t i = 0; i < _latchPinsCount; ++i) {
        l_array.add(_latch->latchState(_latchPins[i]));
    }
}

/*
Processes feedback coming from BrickServer
*/
void NahsBricksFeatureLatch::feedback(JsonDocument* in_json) {
    // check if new latch-triggers are delivered
    if (in_json->containsKey("t")) {
        _latch->clearAllTriggers();
        uint8_t i = 0;
        for (JsonArray latch : in_json->operator[]("t").as<JsonArray>()) {
            for (JsonVariant trigger : latch) {
                _latch->setTrigger(_latchPins[i], static_cast<NahsBricksLibCoIC_Latch::TRIGGER_t>(trigger.as<uint8_t>()));
            }
            ++i;
        }
    }
}

/*
Finalizes feature (closes stuff)
*/
void NahsBricksFeatureLatch::end() {
    // Telling Latch we are done with this session
    _latch->conversionEnd();
}

/*
Prints the features RTCdata in a formatted way to Serial (used for brickSetup)
*/
void NahsBricksFeatureLatch::printRTCdata() {
}

/*
Prints the features FSdata in a formatted way to Serial (used for brickSetup)
*/
void NahsBricksFeatureLatch::printFSdata() {
}

/*
BrickSetup hands over to this function, when features-submenu is selected
*/
void NahsBricksFeatureLatch::brickSetupHandover() {
    _latch->setInterrupt(NahsBricksLibCoIC_Latch::INT_CTL::DISABLE);
    Serial.println("Disabled Interrupt");
    _printMenu();
    while (true) {
        Serial.println();
        Serial.print("Select: ");
        uint8_t input = SerHelp.readLine().toInt();
        switch(input) {
            case 1:
                _readLatchStates();
                break;
            case 9:
                Serial.println("Enabled Interrupt");
                Serial.flush();
                _latch->setInterrupt(NahsBricksLibCoIC_Latch::INT_CTL::LOW_SIG);
                Serial.println("Returning to MainMenu!");
                return;
                break;
            default:
                Serial.println("Invalid input!");
                _printMenu();
                break;
        }
    }
}

/*
Brick-Specific helper to assign the Latch (or LatchExpander) to be used by feature
*/
void NahsBricksFeatureLatch::assignLatch(NahsBricksLibCoIC_Latch &latch) {
    _latch = &latch;
}

/*
Brick-Specific helper to assign the LatchPins (or LatchExpanderPins) to be used by feature
*/
void NahsBricksFeatureLatch::assignLatchPin(uint8_t pinIndex) {
    if (_latchPinsCount < 8 && pinIndex < _latch->latchCount()) {
        _latchPins[_latchPinsCount] = pinIndex;
        ++_latchPinsCount;
    }
}

/*
Helper to print Feature submenu during BrickSetup
*/
void NahsBricksFeatureLatch::_printMenu() {
    Serial.println("1) Read Latch-States");
    Serial.println("9) Return to MainMenu");
}

/*
BrickSetup function to read and print Latch-States
*/
void NahsBricksFeatureLatch::_readLatchStates() {
    Serial.println("Latch States:");
    while(true) {
        _latch->conversionBegin();
        while(_latch->readyToFetchStates() == false) delay(1);
        _latch->fetchLatchStates();
        for(uint8_t i = 0; i < _latchPinsCount; ++i) {
            Serial.print(_latch->latchState(_latchPins[i]));
        }
        Serial.println();
        _latch->conversionEnd();
        delay(50);
        if(_latch->queueLength() == 0) return;
    }
}


//------------------------------------------
// globally predefined variable
#if !defined(NO_GLOBAL_INSTANCES)
NahsBricksFeatureLatch FeatureLatch;
#endif
