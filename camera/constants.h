#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// Paths
extern const std::string DATASET_PATH;
extern const std::string TESTCASES_PATH;
extern const std::string EXPECTED_RESULTS_FILENAME;
extern const std::string EXPECTED_RESULTS_PATH;
extern const std::string PREDICTED_RESULTS_PATH;
extern const std::string WRONG_CASES_PATH;
extern const std::string SINGLE_DATA_PATH;
extern const std::string USER_PROFILE_PATH;
extern const std::string USER_FONT_PATH;

// Make it true to generate test iterations from IAM data set.
extern const bool GENERATE_TEST_ITERATIONS;

// Make it true once to generate the preprocessed images to save time.
extern const bool GENERATE_PRE_PROCESSED_DATA;

// Debugging flags.
extern const bool DEBUG_PARAGRAPH_SEGMENTATION;
extern const bool DEBUG_LINE_SEGMENTATION;
extern const bool DEBUG_THINNING_ALGORITHM;
extern const bool DEBUG_SAVE_WRONG_TESTCASES;

#endif // CONSTANTS_H
