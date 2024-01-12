#include "utils.h"

void displayImage(const string& name, const Mat& img, bool wait) {
    namedWindow(name, WINDOW_NORMAL);
    resizeWindow(name, img.cols / 3, img.rows / 3);

    if (wait) {
        waitKey(0);
        destroyAllWindows();
    }
}

void copyFile(const fs::path& src, const fs::path& dst) {
    fs::create_directories(dst.parent_path());
    fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
}

vector<vector<string>> chunk(const vector<string>& l, int n) {
    vector<vector<string>> chunks;
    for (size_t i = 0; i < l.size(); i += n) {
        chunks.push_back(vector<string>(l.begin() + i, l.begin() + min(i + n, l.size())));
    }
    return chunks;
}

pair<int, int> calculateAccuracy() {
    if (!fs::exists("./data/testcases/results_expected.txt")) {
        cout << ">> Please add '" << "./data/testcases/results_expected.txt" << "' with the expected results of all the test images" << endl;
        return {-1, 0};
    }

    ifstream predictedFile("./data/testcases/results_predicted.txt");
    ifstream expectedFile("./data/testcases/results_expected.txt");

    string predictedLine, expectedLine;
    int count = 0, total = 0;

    while (getline(predictedFile, predictedLine) && getline(expectedFile, expectedLine)) {
        if (predictedLine == expectedLine) {
            count++;
        }
        total++;
    }

    return {count, total};
}

void listTestDirectoryContent(const fs::path& directory, const fs::path& output_file) {
    ofstream file(output_file);

    function<void(const fs::path&, int)> rec = [&](const fs::path& path, int depth) {
        for (const auto& entry : fs::directory_iterator(path)) {
            string indent(depth * 4, ' ');

            if (entry.is_directory()) {
                file << indent << entry.path().filename() << "/\n";
                cout << indent << entry.path().filename() << "/\n";
                rec(entry.path(), depth + 1);
            } else {
                file << indent << entry.path().filename() << "\n";
                cout << indent << entry.path().filename() << "\n";
            }
        }
    };

    rec(directory, 0);

    file.close();
}

void printWrongTestcases() {
    if (!fs::exists("./data/testcases/results_expected.txt")) {
        cout << "Please add '" << "./data/testcases/results_expected.txt" << "' with the expected results of all the test images" << endl;
        return;
    }

    ifstream predictedFile("./data/testcases/results_predicted.txt");
    ifstream expectedFile("./data/testcases/results_expected.txt");

    string predictedLine, expectedLine;
    int i = 0;

    while (getline(predictedFile, predictedLine) && getline(expectedFile, expectedLine)) {
        if (predictedLine != expectedLine) {
            cout << "Wrong classification in iteration: " << setw(3) << setfill('0') << i
                 << " (output: " << predictedLine << ", expected: " << expectedLine << ")" << endl;
        }
        i++;
    }
}

void saveWrongTestcases() {
    if (!fs::exists("./data/testcases/results_expected.txt")) {
        cout << "Please add '" << "./data/testcases/results_expected.txt" << "' with the expected results of all the test images" << endl;
        return;
    }

    ifstream predictedFile("./data/testcases/results_predicted.txt");
    ifstream expectedFile("./data/testcases/results_expected.txt");

    string predictedLine, expectedLine;
    int i = 0, k = 0;

    if (!fs::exists("./data/wrong/")) {
        fs::create_directories("./data/wrong/");
    }

    ofstream outFile("./data/wrong/output.txt", ios::app);

    while (getline(predictedFile, predictedLine) && getline(expectedFile, expectedLine)) {
        if (predictedLine != expectedLine) {
            fs::path srcPath = "./data/testcases/" + to_string(i) + "/";
            fs::path dstPath = "./data/wrong/" + to_string(k) + "/";
            fs::create_directories(dstPath);
            fs::copy(srcPath, dstPath, fs::copy_options::recursive);

            outFile << expectedLine << '\n';
            k++;
        }
        i++;
    }

    outFile.close();
}
