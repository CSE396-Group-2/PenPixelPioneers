#include "textsimulation.h"
#include <QMessageBox>

TextSimulation::TextSimulation() {
    std::string filepath = "../camera/data/data_single/text_to_write.jpeg";

    image = cv::imread(filepath, cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cout << "Could not read the image" << std::endl;
    }
    else {
        cv::threshold(image, image, 100, 255, cv::THRESH_BINARY);

        std::string outputFilePath = "../camera/data/data_single/thresholded_image.jpeg";
        cv::imwrite(outputFilePath, image);

        std::cout << "Thresholded image saved to: " << outputFilePath << std::endl;
    }
}

std::vector<cv::Point> TextSimulation::findShortestPath(const std::vector<cv::Point>& availableNodes,
                                                        const cv::Point start,
                                                        const cv::Point end) {
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    std::unordered_set<cv::Point, PointHash> visited;
    std::queue<std::vector<cv::Point>> q;
    q.push({start});

    while (!q.empty()) {
        std::vector<cv::Point> currentPath = q.front();
        q.pop();

        cv::Point currentPoint = currentPath.back();

        // Check if the current point is the destination
        if (currentPoint == end) {
            return currentPath; // Return the path
        }

        for (int i = 0; i < 8; ++i) {
            cv::Point adjPoint = currentPoint + cv::Point(dx[i], dy[i]);

            // Check if the adjacent point is not visited and available
            if (visited.find(adjPoint) == visited.end() &&
                std::find(availableNodes.begin(), availableNodes.end(), adjPoint) != availableNodes.end()) {
                visited.insert(adjPoint);
                std::vector<cv::Point> newPath = currentPath;
                newPath.push_back(adjPoint);
                q.push(newPath);
            }
        }
    }

    // Return an empty path if no path is found
    return std::vector<cv::Point>();
}

std::vector<cv::Point> TextSimulation::reorderIsland(const std::vector<cv::Point>& island) {
    std::vector<cv::Point> reorderedIsland;
    if (island.empty()) {
        return reorderedIsland;
    }

    std::vector<cv::Point> unvisitedNodes = island;

    // Start from the first node in the island
    cv::Point current = island.front();
    reorderedIsland.push_back(current);
    unvisitedNodes.erase(unvisitedNodes.begin()); // Remove the first node (visited)

    bool exploring = true;
    while (exploring) {
        // Find the closest unvisited node

        if(!unvisitedNodes.empty()) {
            double minDistance = std::numeric_limits<double>::max();
            int closestNodeIndex = -1;
            for (size_t i = 0; i < unvisitedNodes.size(); ++i) {
                cv::Point unvisitedNode =  unvisitedNodes[i];

                double dist = cv::norm(current - unvisitedNode);
                if (dist < minDistance) {
                    minDistance = dist;
                    closestNodeIndex = static_cast<int>(i);
                }
            }


            cv::Point nextNode = unvisitedNodes[closestNodeIndex];

            bool isAdj = (std::abs(current.x - nextNode.x) <= 1) && (std::abs(current.y - nextNode.y) <= 1);

            // Check if the closest node is adjacent to the current node in a 3x3 grid
            if (!isAdj) {
                std::vector<cv::Point> pathToPoint = findShortestPath(island, current, nextNode);

                for (size_t i = 1; i < pathToPoint.size() - 1; ++i) {
                    reorderedIsland.push_back(pathToPoint[i]);
                }
            }

            current = nextNode;
            reorderedIsland.push_back(current);
            unvisitedNodes.erase(unvisitedNodes.begin() + closestNodeIndex);


        }
        else {
            exploring = false; // Stop if all nodes are visited
        }
    }

    return reorderedIsland;
}


//main function
void TextSimulation::processImage() {
    std::vector<std::vector<cv::Point>> islands;
    pixelCoordinates.clear();

    cv::Mat labels;
    int nLabels = cv::connectedComponents(image, labels, 8, CV_32S);

    islands.clear();
    islands.resize(nLabels - 1); // Skip background label 0

    // Assign pixels to respective islands based on label
    for (int i = 0; i < labels.rows; ++i) {
        for (int j = 0; j < labels.cols; ++j) {
            int label = labels.at<int>(i, j);
            if (label > 0) {
                islands[label - 1].push_back(cv::Point(j, i));
            }
        }
    }

    // Sort islands based on their x-coordinates
    std::sort(islands.begin(), islands.end(), [](const std::vector<cv::Point>& island1, const std::vector<cv::Point>& island2) {
        cv::Rect boundingRect1 = cv::boundingRect(island1);
        cv::Rect boundingRect2 = cv::boundingRect(island2);
        return boundingRect1.x < boundingRect2.x;
    });

    // Simplify paths within each island
    for (size_t i = 0; i < islands.size(); ++i) {
        islands[i] = reorderIsland(islands[i]);
    }

    // Clear and store island coordinates into pixelCoordinates
    for (size_t i = 0; i < islands.size(); ++i) {
        const std::vector<cv::Point>& island = islands[i];

        // Add "-1,-1" after the first coordinate of each island (except the first island)
        if (i > 0) {
            pixelCoordinates.push_back(island.front());
            pixelCoordinates.push_back(cv::Point(-1, -1));
            // Add the rest of the island coordinates
            pixelCoordinates.insert(pixelCoordinates.end(), island.begin() + 1, island.end());
        } else {
            // Add the coordinates of the first island
            pixelCoordinates.insert(pixelCoordinates.end(), island.begin(), island.end());
        }

        // Add "-2,-2" after the last coordinate of each island
        pixelCoordinates.push_back(cv::Point(-2, -2));
    }

    // reset pen position
    pixelCoordinates.push_back(cv::Point(pixelCoordinates.front().x, pixelCoordinates.front().y + 50));

    // Save coordinates in the specified format to a text file
    std::string filePath = "../camera/data/data_single/coordinates.json";
    std::ofstream outputFile(filePath);

    if (outputFile.is_open()) {
        outputFile << "[\n";

        for (size_t i = 0; i < pixelCoordinates.size(); ++i) {
            const cv::Point &point = pixelCoordinates[i];
            outputFile << "\"" << point.x << "," << point.y << "\",\n";
        }
        outputFile << "]";

        outputFile.close();
        std::cout << "Coordinates saved to " << filePath << std::endl;
    } else {
        std::cerr << "Unable to open file: " << filePath << std::endl;
    }

}

void TextSimulation::sendSerialData(QString serialPort, bool& stopSignal) {

    int total = pixelCoordinates.size();

#ifdef _WIN32
    const char * serialPortValue = serialPort.toStdString().c_str();
    HANDLE serial = CreateFileA(serialPortValue, GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);

    if (serial == INVALID_HANDLE_VALUE) {
        emit updateStatus(QString::fromStdString("Can't open serial port '" + serialPort.toStdString() + "'."));
        return;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(serial, &dcbSerialParams)) {
        std::cerr << "Error getting serial port state!" << std::endl;
        CloseHandle(serial);
        return;
    }

    dcbSerialParams.BaudRate = CBR_9600; // Set your Arduino baud rate
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(serial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port state!" << std::endl;
        CloseHandle(serial);
        return;
    }
#else
    // "/dev/ttyUSB0"
    const char * serialPortValue = ("/dev/" + serialPort.toStdString()).c_str();
    serial = open(serialPortValue, O_RDWR | O_NOCTTY | O_NDELAY);
    std::cout << "serial port: " << serialPortValue << std::endl;

    if (serial == -1) {
        std::cerr << "Error opening serial port!" << std::endl;
        return;
    }

    struct termios tty;
    if (tcgetattr(serial, &tty) != 0) {
        std::cerr << "Error getting serial port attributes!" << std::endl;
        close(serial);
        return;
    }

    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 5;

    if (tcsetattr(serial, TCSANOW, &tty) != 0) {
        std::cerr << "Error setting serial port attributes!" << std::endl;
        close(serial);
        return;
    }
#endif

    std::string dataToSend;
    cv::Point qElement;
    cv::Point lastElement;
    std::string messageString;
    int finished;
    double percentage;
#ifdef _WIN32
    DWORD bytesWritten;
#endif
    bool liftUpFlag = false;
    double distance;
    int sleepTime;
    auto startTime = std::chrono::steady_clock::now();
    std::stringstream elapsedTimeStream;

    while (!pixelCoordinates.empty() && !stopSignal) {
        finished = total - pixelCoordinates.size();
        percentage = static_cast<double>(finished) / total * 100;

        std::stringstream percentageValue;
        percentageValue << std::fixed << std::setprecision(2) << percentage;

        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

        // Calculate minutes and seconds
        int minutes = elapsedTime / 60;
        int seconds = elapsedTime % 60;

        elapsedTimeStream.str("");
        elapsedTimeStream << minutes << "." << std::setfill('0') << std::setw(2) << seconds;

        messageString = "Pencil Movements: " + percentageValue.str() + "% - Time Elapsed: " + elapsedTimeStream.str();
        emit updateStatus(QString::fromStdString(messageString));

        qElement = pixelCoordinates.front();  // Get the first element
        pixelCoordinates.erase(pixelCoordinates.begin());   // Remove the first element

        dataToSend = std::to_string(qElement.x) + "," + std::to_string(qElement.y) + "\n";

#ifdef _WIN32
        if (!WriteFile(serial, dataToSend.c_str(), dataToSend.length(), &bytesWritten, nullptr)) {
            std::cerr << "Error writing to serial port!" << std::endl;
            CloseHandle(serial);
            return;
        }
#else
        ssize_t bytesWritten = write(serial, dataToSend.c_str(), dataToSend.length());
        if (bytesWritten < 0) {
            std::cerr << "Error writing to serial port!" << std::endl;
            close(serial);
            return;
        }
        //tcflush(serial, TCIOFLUSH);
#endif

        if(liftUpFlag){
            liftUpFlag = false;
            distance = cv::norm(qElement - lastElement);
            sleepTime = static_cast<int>(((distance / 6) * 1000) + 500);
            std::cout << "Distance: " << distance << " Time: " << sleepTime << " Current: x: " << qElement.x << " y: " << qElement.y << " Prev: x: " << lastElement.x << " y: " << lastElement.y << std::endl;
#ifdef _WIN32
            Sleep(sleepTime);
#else
            usleep(sleepTime*1000);
#endif
        }
        else if(qElement.x == -1){
#ifdef _WIN32
            Sleep(500);
#else
            usleep(500000);
#endif
        }
        else if(qElement.x == -2){
            liftUpFlag = true;
#ifdef _WIN32
            Sleep(500);
#else
            usleep(500000);
#endif
        }
        else {
            if(lastElement.x == qElement.x || lastElement.y == qElement.y){
#ifdef _WIN32
                Sleep(150);
#else
                usleep(150000);
#endif
            }
            else {
#ifdef _WIN32
                Sleep(200);
#else
                usleep(200000);
#endif
            }


            lastElement = qElement;
        }
    }

    dataToSend = "reset\n";

#ifdef _WIN32
    if (!WriteFile(serial, dataToSend.c_str(), dataToSend.length(), &bytesWritten, nullptr)) {
        std::cerr << "Error writing to serial port!" << std::endl;
        CloseHandle(serial);
        return;
    }
#else
    ssize_t bytesWritten = write(serial, dataToSend.c_str(), dataToSend.length());
    if (bytesWritten < 0) {
        std::cerr << "Error writing to serial port!" << std::endl;
        close(serial);
        return;
    }
    //tcflush(serial, TCIOFLUSH);
#endif

    messageString = "Finished sending pencil movements in: " + elapsedTimeStream.str();
    emit updateStatus(QString::fromStdString(messageString));

#ifdef _WIN32
    CloseHandle(serial);
#else
    close(serial);
#endif

}
