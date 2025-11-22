#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <numeric> // for std::accumulate

using namespace std;


// --- Structure to hold one row of price data ---
struct PriceRow {
    string date;
    double open;
    double high;
    double low;
    double close;
    int volume;
};

// --- Function to load CSV file into a vector of PriceRow ---
vector<PriceRow> loadCSV(const string& filepath) {
    vector<PriceRow> data;
    ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error: cannot open file: " << filepath << "\n";
        return data;
    }

    string line;
    
    // --- Skip header ---
    if (!getline(file, line)) {
        cerr << "Error: empty file or cannot read header.\n";
        return data;
    }

    // --- Read each line of data ---
    while (getline(file, line)) {
        stringstream ss(line);
        string item;
        PriceRow row;

        try {
            getline(ss, row.date, ',');
            getline(ss, item, ','); row.open  = std::stod(item);
            getline(ss, item, ','); row.high  = std::stod(item);
            getline(ss, item, ','); row.low   = std::stod(item);
            getline(ss, item, ','); row.close = std::stod(item);
            getline(ss, item, ','); row.volume = std::stoi(item);
        } catch (const std::exception& e) {
            cerr << "Skipping invalid row: " << line << " (" << e.what() << ")\n";
            continue;
        }

        data.push_back(row);
    }

    return data;
}

// --- NEW: Calculate Simple Moving Average ---
// SMA is the average of last 'period' closing prices
double calculateSMA(const vector<PriceRow>& data, int period, int currentIndex) {
    // We can't calculate SMA if we don't have enough historical data
    if (currentIndex < period - 1) {
        return 0.0; // Or you could return a special value to indicate insufficient data
    }
    
    double sum = 0.0;
    // Sum up the last 'period' closing prices
    for (int i = currentIndex - period + 1; i <= currentIndex; i++) {
        sum += data[i].close;
    }
    
    return sum / period;
}

// --- Let's test SMA with a simple example first ---
void testSMAWithSampleData() {
    cout << "=== Testing SMA with Sample Data ===\n";
    
    // Create some sample price data
    vector<PriceRow> sampleData = {
        {"2025-01-01", 100, 105, 99, 101, 1000},
        {"2025-01-02", 101, 106, 100, 103, 1100},
        {"2025-01-03", 103, 108, 102, 105, 1200},
        {"2025-01-04", 105, 110, 104, 107, 1300},
        {"2025-01-05", 107, 112, 106, 109, 1400}
    };
    
    int period = 3; // 3-day moving average
    
    for (int i = 0; i < sampleData.size(); i++) {
        double sma = calculateSMA(sampleData, period, i);
        cout << "Day " << (i+1) << " (" << sampleData[i].date << "): "
             << "Close = " << sampleData[i].close;
        
        if (sma > 0) {
            cout << ", SMA(" << period << ") = " << sma;
        } else {
            cout << ", SMA: Not enough data";
        }
        cout << "\n";
    }
}

int main() {
    // First, let's understand SMA with sample data
    testSMAWithSampleData();
    
    cout << "\n=== Now with Real Data ===\n";
    
    // Then try with real data
    string filepath = "data/data.csv";
    vector<PriceRow> prices = loadCSV(filepath);

    if (prices.empty()) {
        cout << "No data loaded.\n";
        return 0;
    }

    cout << "Loaded rows: " << prices.size() << "\n";
    
    // Calculate SMA for the last few days to see it working
    int period = 5; // 5-day moving average
    int daysToShow = 10;
    
    cout << "\nLast " << daysToShow << " days with SMA(" << period << "):\n";
    
    int startIndex = prices.size() - daysToShow;
    if (startIndex < 0) startIndex = 0;
    
    for (int i = startIndex; i < prices.size(); i++) {
        double sma = calculateSMA(prices, period, i);
        cout << prices[i].date << ": Close = " << prices[i].close;
        
        if (sma > 0) {
            cout << ", SMA = " << sma;
        } else {
            cout << ", SMA: Not enough data";
        }
        cout << "\n";
    }

    return 0;
}