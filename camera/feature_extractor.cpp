#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <numeric>

// #include "../utils/utils.cpp"

class FeatureExtractor {
public:
    FeatureExtractor(const cv::Mat& gray_lines, const cv::Mat& bin_lines)
        : gray_lines(gray_lines), bin_lines(bin_lines) {}

    std::vector<double> extract() {
        // Extract LBP histogram features

        std::vector<float> lbpFeatures = lbp_histogram();

        // Extend the features vector with LBP histogram features
        this->features.insert(this->features.end(), lbpFeatures.begin(), lbpFeatures.end());

        return this->features;
    }

private:
    cv::Mat gray_lines;
    cv::Mat bin_lines;
    std::vector<double> features;

    std::vector<float> lbp_histogram() {
        // Set up the histogram
        std::vector<float> hist(256, 0.0f);

        //cout << "this->bin_lines.size(): " << this->bin_lines.size() << endl;

        //for (size_t i = 0; i < this->bin_lines.size(); ++i) {
        // Assuming that get_lbp_histogram is a static member function of FeatureExtractor
        // You may need to adjust this based on your actual implementation
        hist = get_lbp_histogram(this->gray_lines, this->bin_lines, hist, true);
        //}

        // Calculate the mean of hist
        float mean = std::accumulate(hist.begin(), hist.end(), 0.0f) / hist.size();
        //cout << "\nhist lbp_histogram 1: "<< hist.size() << endl;

        // Normalize each element of hist by dividing it by the mean
        for (float& value : hist) {
            value /= mean;
            //cout << value << " ";
        }

        return hist;
    }


    static std::vector<float> get_lbp_histogram(cv::Mat& img, const cv::Mat& mask, std::vector<float> hist, bool acc) {
        // Get image dimensions
        int height = img.rows;
        int width = img.cols;

        // Initialize LBP image
        cv::Mat lbp = cv::Mat(height, width, CV_8U, cv::Scalar(0));

        // Directions
        int v = 3;
        int dx[] = {0, v, v, v, 0, -v, -v, -v};
        int dy[] = {v, v, 0, -v, -v, -v, 0, v};

        cv::Mat view_shf;
        cv::Mat view_img;
        cv::Mat view_lbp;
        cv::Mat res;
        // Loop over the 8 neighbors
        for (int i = 0; i < 8; ++i) {
            view_shf = shift(img, cv::Vec2i(dy[i], dx[i]));
            view_img = shift(img, cv::Vec2i(-dy[i], -dx[i]));
            view_lbp = shift(lbp, cv::Vec2i(-dy[i], -dx[i]));

            res = view_img >= view_shf;
            view_lbp |= res & (1 << i);
        }

        // Convert lbp matrix to 8-bit unsigned integer type
        cv::Mat lbp_8u;
        lbp.convertTo(lbp_8u, CV_8U);


        // Calculate LBP histogram of only black pixels
        const int hist_size = hist.size();
        float range[] = { 0, 256 };
        const float* hist_range[] = {range};
        bool uniform = true;
        const int channels[] = { 0 };
        //cout << "calculating hist: " << hist_size << endl;
        cv::Mat histMat;
        cv::calcHist(&lbp_8u, 1, channels, mask, histMat, 1, &hist_size, hist_range, uniform, acc);
        histMat.reshape(1, 256).copyTo(hist);

        return hist;
    }

    static cv::Mat shift(const cv::Mat& img, const cv::Vec2i& shift) {
        int r = shift[0], c = shift[1];

        if (r >= 0) {
            return (c >= 0) ? img.rowRange(r, img.rows).colRange(c, img.cols) : img.rowRange(r, img.rows).colRange(0, c + img.cols);
        } else {
            return (c >= 0) ? img.rowRange(0, r + img.rows).colRange(c, img.cols) : img.rowRange(0, r + img.rows).colRange(0, c + img.cols);
        }
    }


};
