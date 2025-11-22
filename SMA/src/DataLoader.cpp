#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

// --- Structure to hold one row of price data ---
struct PriceRow {
    std::string date;
    double open;
    double high;
    double low;
    double close;
    int volume;
};

// --- Function to load CSV file into a vector of PriceRow ---
std::vector<PriceRow> loadCSV(const std::string& filepath) {
    std::vector<PriceRow> data;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error: cannot open file: " << filepath << "\n";
        return data; // Return empty vector if file cannot be opened
    }

    std::string line;
    
    // --- Skip header ---
    if (!std::getline(file, line)) {
        std::cerr << "Error: empty file or cannot read header.\n";
        return data;
    }

    // --- Read each line of data ---
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string item;
        PriceRow row;

        // Parse CSV columns
        try {
            std::getline(ss, row.date, ',');
            std::getline(ss, item, ','); row.open  = std::stod(item);
            std::getline(ss, item, ','); row.high  = std::stod(item);
            std::getline(ss, item, ','); row.low   = std::stod(item);
            std::getline(ss, item, ','); row.close = std::stod(item);
            std::getline(ss, item, ','); row.volume = std::stoi(item);
        } catch (const std::exception& e) {
            std::cerr << "Skipping invalid row: " << line << " (" << e.what() << ")\n";
            continue; // Skip row if conversion fails
        }

        data.push_back(row);
    }

    return data;
}

int main() {
    std::string filepath = "data/data.csv"; // Path to your CSV file
    std::vector<PriceRow> prices = loadCSV(filepath);

    if (prices.empty()) {
        std::cout << "No data loaded.\n";
        return 0;
    }

    std::cout << "Loaded rows: " << prices.size() << "\n";
    std::cout << "First row date: " << prices[0].date << "\n";

    return 0;
}
