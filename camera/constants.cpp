#include "constants.h"

// Paths
const std::string DATASET_PATH = "../camera/data/dataset/";
const std::string TESTCASES_PATH = "../camera/data/testcases/";
const std::string EXPECTED_RESULTS_FILENAME = "results_expected.txt";
const std::string EXPECTED_RESULTS_PATH = TESTCASES_PATH + EXPECTED_RESULTS_FILENAME;
const std::string PREDICTED_RESULTS_PATH = TESTCASES_PATH + "results_predicted.txt";
const std::string WRONG_CASES_PATH = "../camera/data/wrong/";
const std::string SINGLE_DATA_PATH = "../camera/data/data_single/";
const std::string USER_PROFILE_PATH = "../camera/data/testcases/002/";
const std::string USER_FONT_PATH = "../camera/data/ttf/";

// Make it true to generate test iterations from IAM data set.
const bool GENERATE_TEST_ITERATIONS = false;

// Make it true once to generate the preprocessed images to save time.
const bool GENERATE_PRE_PROCESSED_DATA = false;

// Debugging flags.
const bool DEBUG_PARAGRAPH_SEGMENTATION = false;
const bool DEBUG_LINE_SEGMENTATION = false;
const bool DEBUG_THINNING_ALGORITHM = false;
const bool DEBUG_SAVE_WRONG_TESTCASES = false;
