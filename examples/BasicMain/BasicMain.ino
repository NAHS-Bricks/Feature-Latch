#include <Arduino.h>
#include <nahs-Bricks-OS.h>
// include all features of brick
#include <nahs-Bricks-Feature-Latch.h>

void setup() {
  // Now register all the features under All
  // Note: the order of registration is the same as the features are handled internally by FeatureAll
  FeatureAll.registerFeature(&FeatureLatch);

  // Set Brick-Specific stuff
  BricksOS.setSetupPin(D5);
  FeatureAll.setBrickType(2);

  // Set Brick-Specific (feature related) stuff
  // For this Feature you have to start the Wire connection, connect the Latch (or LatchExpander)
  //   on it's address and assign the one to be used to the Feature
  Wire.begin();
  Latch.begin(45);
  FeatureLatch.assignLatch(Latch);
  FeatureLatch.assignLatchPin(0);
  FeatureLatch.assignLatchPin(1);

  // Finally hand over to BrickOS
  BricksOS.handover();
}

void loop() {
  // Not used on Bricks
}