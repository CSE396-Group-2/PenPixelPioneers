#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
// #include "../utils/utils.cpp"
// #include "../utils/constants.cpp"

using namespace std;

class LineSegmentor {
public:
    LineSegmentor(const cv::Mat& gray_img, const cv::Mat& bin_img)
        : gray_img(gray_img), bin_img(bin_img) {

        /*cv::imshow("bin_img", bin_img);
        cv::waitKey(0);*/

        // Store references to the page images.
        this->gray_img = gray_img;
        this->bin_img = bin_img;

        // Get horizontal histogram.
        this->hor_hist = std::vector<int>(this->bin_img.rows); // Replace cv::Mat with std::vector

        for (int i = 0; i < this->bin_img.rows; ++i) {
            this->hor_hist[i] = cv::sum(this->bin_img.row(i))[0] / 255.0; // Replace hor_hist.at<float>(i, 0) with hor_hist[i]
        }

        cout << "this->bin_img.rows: " << this->bin_img.rows << endl;

        double min, max;
        cv::minMaxLoc(this->hor_hist, &min, &max);

        // Get line density thresholds.
        this->threshold_high = static_cast<int>(max / 3);
        //cout << "threshold_high " << this->threshold_high << endl;

        this->threshold_low = 25;

        // Calculate peaks and valleys of the page.
        this->detect_peaks();
        this->avg_peaks_dist = static_cast<int>((this->peaks.back() - this->peaks[0]) / this->peaks.size());
        //cout << "avg_peaks_dist: " << this->avg_peaks_dist << endl;
        this->detect_valleys();
        //cout << "detect valleys: " << this->valleys.size() << endl;

        // Detect missing peaks and valleys in a second iteration.
        this->detect_missing_peaks_valleys();

        // Detect line boundaries.
        this->detect_line_boundaries();
    }

    std::pair<std::vector<cv::Mat>, std::vector<cv::Mat>> segment() {
        // Illustrate line segmentation.
        // this->display_segmentation(); // Assuming display_segmentation is commented out or already translated.

        // Initialize lines lists.
        std::vector<cv::Mat> gray_lines, bin_lines;


        // Loop on every line boundary.
        for (const auto& boundary : this->lines_boundaries) {
            // Crop gray line.
            cv::Mat g_line = this->gray_img(cv::Rect(boundary[0], boundary[1], boundary[2] - boundary[0] + 1, boundary[3] - boundary[1] + 1));
            gray_lines.push_back(g_line);

            // Crop binary line.
            cv::Mat b_line = this->bin_img(cv::Rect(boundary[0], boundary[1], boundary[2] - boundary[0] + 1, boundary[3] - boundary[1] + 1));
            bin_lines.push_back(b_line);
        }
        // Return list of separated lines.
        return std::make_pair(gray_lines, bin_lines);
    }


private:
    cv::Mat gray_img;
    cv::Mat bin_img;
    std::vector<int> hor_hist;
    int threshold_high;
    int threshold_low;
    std::vector<int> peaks;
    std::vector<int> valleys;
    std::vector<std::vector<int>> lines_boundaries;
    int avg_peaks_dist;

    void detect_peaks() {
        this->peaks.clear();
        int i = 0;
        //cout << "this->hor_hist.rows len: " << this->hor_hist.size() << endl;
        while (i < this->hor_hist.size()) {
            if (this->hor_hist.at(i) < this->threshold_high) {
                i++;
                continue;
            }

            int peak_idx = i;

            while (i < this->hor_hist.size() && this->is_probable_peak(i)) {
                if (this->hor_hist.at(i) > this->hor_hist.at(peak_idx)) {
                    peak_idx = i;
                }
                i++;
            }
            //cout << "peak_idx: " << peak_idx << endl;

            this->peaks.push_back(peak_idx);
        }
    }

    void detect_valleys() {
        this->valleys.clear();
        this->valleys.push_back(0);
        int i = 1;

        while (i < this->peaks.size()) {
            int u = this->peaks[i - 1];
            int d = this->peaks[i];
            i++;

            int expected_valley = d - this->avg_peaks_dist / 2;
            int valley_idx = u;

            while (u < d) {
                int dist1 = std::abs(u - expected_valley);
                int dist2 = std::abs(valley_idx - expected_valley);

                bool cond1 = this->hor_hist.at(u) < this->hor_hist.at(valley_idx);
                bool cond2 = this->hor_hist.at(u) == this->hor_hist.at(valley_idx) && dist1 < dist2;

                if (cond1 || cond2) {
                    valley_idx = u;
                }

                u++;
            }

            this->valleys.push_back(valley_idx);
        }

        this->valleys.push_back(this->hor_hist.size() - 1);
    }

    void detect_missing_peaks_valleys() {
        int i = 1;
        bool found = false;

        while (i < this->valleys.size()) {
            int up = this->valleys[i - 1];
            int down = this->valleys[i];
            int dis = down - up;
            i++;

            if (dis < 1.5 * this->avg_peaks_dist) {
                continue;
            }

            int u = up + this->avg_peaks_dist;
            int d = std::min(down, u + this->avg_peaks_dist);

            while ((d - u) * 2 > this->avg_peaks_dist) {
                if (this->is_probable_valley(u) && this->is_probable_valley(d)) {
                    int peak = this->get_peak_in_range(u, d);

                    if (this->hor_hist.at(peak) > this->threshold_low) {
                        this->peaks.push_back(this->get_peak_in_range(u, d));
                        found = true;
                    }
                }

                u = u + this->avg_peaks_dist;
                d = std::min(down, u + this->avg_peaks_dist);
            }
        }

        if (found) {
            std::sort(this->peaks.begin(), this->peaks.end());
            this->detect_valleys();
        }
    }

    void detect_line_boundaries() {
        this->lines_boundaries.clear();
        int height = this->bin_img.rows;
        int width = this->bin_img.cols;

        int i = 1;
        while (i < this->valleys.size()) {
            int u = this->valleys[i - 1];
            int d = this->valleys[i];
            int l = 0;
            int r = width - 1;
            i++;

            while (u < d && this->hor_hist.at(u) == 0) {
                u++;
            }
            while (d > u && this->hor_hist.at(d) == 0) {
                d--;
            }

            std::vector<int> ver_hist;
            cv::reduce(this->bin_img(cv::Rect(0, u, width, d - u + 1)), ver_hist, 0, cv::REDUCE_SUM, CV_32S);

            while (l < r && ver_hist[l] == 0) {
                l++;
            }

            while (r > l && ver_hist[r] == 0) {
                r--;
            }

            this->lines_boundaries.push_back({l, u, r, d});
        }
    }

    int calc_average_line_slope() {
        int avg_slope = 0;
        int i = 1;

        while (i < this->valleys.size()) {
            int u = this->valleys[i - 1];
            int d = this->valleys[i];
            avg_slope += this->calc_range_slope(u, d);
            i++;
        }

        return static_cast<int>(avg_slope / (this->valleys.size() - 1));
    }

    int calc_range_slope(int up, int down) {
        int max_der = -1e9;
        int min_der = 1e9;

        while (up < down) {
            up++;
            int val = this->hor_hist.at(up) - this->hor_hist.at(up - 1);
            max_der = std::max(max_der, val);
            min_der = std::min(min_der, val);
        }

        return max_der - min_der;
    }

    int get_peak_in_range(int up, int down) {
        int peak_idx = up;

        while (up < down) {
            if (this->hor_hist.at(up) > this->hor_hist.at(peak_idx)) {
                peak_idx = up;
            }
            up++;
        }

        return peak_idx;
    }

    bool is_probable_peak(int row) {
        int width = 15;

        for (int i = -width; i <= width; i++) {
            if (row + i < 0 || row + i >= this->hor_hist.size()) {
                continue;
            }
            if (this->hor_hist.at(row + i) >= this->threshold_high) {
                return true;
            }
        }

        return false;
    }

    bool is_probable_valley(int row) {
        int width = 30;
        int count = 0;

        for (int i = -width; i <= width; i++) {
            if (row + i < 0 || row + i >= this->hor_hist.size()) {
                return true;
            }
            if (this->hor_hist.at(row + i) <= this->threshold_low) {
                count++;
            }
        }

        return count * 2 >= width;
    }
};
