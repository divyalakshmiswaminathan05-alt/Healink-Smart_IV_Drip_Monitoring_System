#include "HX711.h"

#define DT_PIN 5
#define SCK_PIN 4

HX711 scale;

// Known calibration weight (not needed anymore but kept)
float known_weight = 100.0;  

// INSERTED YOUR CALIBRATION FACTOR HERE
float calibration_factor = 6277.970215;

// Calibration already done → skip auto-calibration
bool calibrated = true;

// ---------- MEDIAN FILTER (20 samples) ----------
long getMedian(long *arr, int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      if (arr[j] < arr[i]) {
        long t = arr[i];
        arr[i] = arr[j];
        arr[j] = t;
      }
    }
  }
  return arr[n / 2];
}

// ---------- MOVING AVERAGE (15 samples) ----------
float movingAverage(float val) {
  static float buf[15];
  static int idx = 0;
  static bool filled = false;

  buf[idx] = val;
  idx = (idx + 1) % 15;

  if (idx == 0) filled = true;

  float sum = 0;
  int limit = filled ? 15 : idx;
  for (int i = 0; i < limit; i++) sum += buf[i];

  return sum / limit;
}

// ---------- CONDITIONS ----------
const float STABLE_TOLERANCE = 0.5;      // g
const int STABLE_REQUIRED = 10;          // consecutive readings
const float DRIP_EMPTY_THRESHOLD = 5.0;  // g, threshold for empty drip
const float REVERSE_FLOW_DELTA = 1.5;    // g, sudden increase = reverse flow

float previousWeight = 0;
int stableCount = 0;
bool reverseFlowActive = false;
bool dripEmpty = false;

void setup() {
  Serial.begin(115200);
  scale.begin(DT_PIN, SCK_PIN);

  Serial.println("\n--------------------------------------");
  Serial.println(" SMART IV DRIP – HIGH PRECISION SCALE ");
  Serial.println("  Using Fixed Calibration Factor      ");
  Serial.println("--------------------------------------\n");

  Serial.print("Calibration Factor Loaded: ");
  Serial.println(calibration_factor, 6);
}

void loop() {

  // -------- TAKE 20 RAW HX711 READINGS --------
  long raw[20];
  for (int i = 0; i < 20; i++) {
    while (!scale.is_ready()) delay(2);
    raw[i] = scale.read();
    delay(80);
  }

  long median_val = getMedian(raw, 20);

  static long last_val = median_val;
  if (abs(median_val - last_val) < 5) median_val = last_val;
  last_val = median_val;

  // -------- CALCULATE WEIGHT (grams) --------
  float weight = median_val / calibration_factor;
  weight = movingAverage(weight);  // smooth output

  // -------- PRINT RESULT --------
  Serial.print("Weight (g): ");
  Serial.println(weight, 2);

  // -------- CONDITION 1: DRIP EMPTY --------
  if (weight <= DRIP_EMPTY_THRESHOLD && !dripEmpty) {
    Serial.println("⚠ DRIP FINISHED — Weight Below Threshold!");
    dripEmpty = true;
  } else if (weight > DRIP_EMPTY_THRESHOLD) {
    dripEmpty = false;
  }

  // -------- CONDITION 2: REVERSE FLOW --------
  if (weight > previousWeight + REVERSE_FLOW_DELTA) {
    if (!reverseFlowActive) Serial.println("❗ REVERSE FLOW DETECTED — Weight Increasing!");
    reverseFlowActive = true;
  } else {
    reverseFlowActive = false;
  }

  // -------- CONDITION 3: STABILITY CHECK --------
  if (abs(weight - previousWeight) <= STABLE_TOLERANCE) {
    stableCount++;
  } else {
    stableCount = 0;
  }

  if (stableCount >= STABLE_REQUIRED) {
    Serial.println("✔ Weight Stable for 10 Consecutive Readings");
    stableCount = 0;
  }

  previousWeight = weight;
  delay(200);
}
