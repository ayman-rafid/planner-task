#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <stdexcept>
#include <string>
#include <utility>

class Planner {
private:
    std::string material_;
    std::string wing_type_;
    std::string takeoff_;
    double weight_{0.0};
    std::string manufacturer_;
    std::string model_;

    static std::string trim(const std::string& text) {
        std::size_t start = 0;
        while (start < text.size() && std::isspace(static_cast<unsigned char>(text[start]))) {
            ++start;
        }

        std::size_t end = text.size();
        while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1]))) {
            --end;
        }

        return text.substr(start, end - start);
    }

    static std::string toLower(std::string text) {
        for (char& ch : text) {
            ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }
        return text;
    }

    static std::string normalizeTakeoff(const std::string& takeoff) {
        const std::string cleaned = toLower(trim(takeoff));

        if (cleaned == "ground" || cleaned == "from the ground") {
            return "from the ground";
        }

        if (cleaned == "hill" || cleaned == "from a hill") {
            return "from a hill";
        }

        throw std::invalid_argument("Takeoff must be 'from the ground' or 'from a hill'.");
    }

public:
    Planner() = default;

    Planner(std::string material,
            std::string wing_type,
            std::string takeoff,
            double weight,
            std::string manufacturer,
            std::string model) {
        setMaterial(material);
        setWingType(wing_type);
        setTakeoff(takeoff);
        setWeight(weight);
        setManufacturer(manufacturer);
        setModel(model);
    }

    Planner(const Planner&) = default;
    Planner& operator=(const Planner&) = default;
    Planner(Planner&&) noexcept = default;
    Planner& operator=(Planner&&) noexcept = default;
    ~Planner() = default;

    const std::string& getMaterial() const noexcept { return material_; }
    const std::string& getWingType() const noexcept { return wing_type_; }
    const std::string& getTakeoff() const noexcept { return takeoff_; }
    double getWeight() const noexcept { return weight_; }
    const std::string& getManufacturer() const noexcept { return manufacturer_; }
    const std::string& getModel() const noexcept { return model_; }

    void setMaterial(const std::string& material) {
        const std::string cleaned = trim(material);
        if (cleaned.empty()) {
            throw std::invalid_argument("Material cannot be empty.");
        }
        material_ = cleaned;
    }

    void setWingType(const std::string& wing_type) {
        const std::string cleaned = trim(wing_type);
        if (cleaned.empty()) {
            throw std::invalid_argument("Wing type cannot be empty.");
        }
        wing_type_ = cleaned;
    }

    void setTakeoff(const std::string& takeoff) {
        takeoff_ = normalizeTakeoff(takeoff);
    }

    void setWeight(double weight) {
        if (weight < 0.0) {
            throw std::invalid_argument("Weight cannot be negative.");
        }
        weight_ = weight;
    }

    void setManufacturer(const std::string& manufacturer) {
        const std::string cleaned = trim(manufacturer);
        if (cleaned.empty()) {
            throw std::invalid_argument("Manufacturer cannot be empty.");
        }
        manufacturer_ = cleaned;
    }

    void setModel(const std::string& model) {
        const std::string cleaned = trim(model);
        if (cleaned.empty()) {
            throw std::invalid_argument("Model cannot be empty.");
        }
        model_ = cleaned;
    }
};

std::string readNonEmptyLine(const std::string& prompt) {
    std::string value;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);

        bool has_non_space = false;
        for (char ch : value) {
            if (!std::isspace(static_cast<unsigned char>(ch))) {
                has_non_space = true;
                break;
            }
        }

        if (has_non_space) {
            return value;
        }

        std::cout << "Input cannot be empty. Try again.\n";
    }
}

double readWeight(const std::string& prompt) {
    double weight{};

    while (true) {
        std::cout << prompt;

        if (std::cin >> weight && weight >= 0.0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return weight;
        }

        std::cout << "Invalid weight. Enter a non-negative number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string readTakeoff(const std::string& prompt) {
    while (true) {
        const std::string input = readNonEmptyLine(prompt);

        try {
            Planner temp;
            temp.setTakeoff(input);
            return temp.getTakeoff();
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid takeoff. Enter 'from the ground' or 'from a hill'.\n";
        }
    }
}

Planner inputPlanner(int index) {
    std::cout << "\nEnter data for planner #" << index << ":\n";

    const std::string material = readNonEmptyLine("Material: ");
    const std::string wing_type = readNonEmptyLine("Wing type: ");
    const std::string takeoff = readTakeoff("Takeoff (from the ground or from a hill): ");
    const double weight = readWeight("Weight: ");
    const std::string manufacturer = readNonEmptyLine("Manufacturer: ");
    const std::string model = readNonEmptyLine("Model: ");

    return Planner(material, wing_type, takeoff, weight, manufacturer, model);
}

void printTable(const std::list<Planner>& planners) {
    std::cout << "\nSorted planners by weight (descending):\n\n";

    std::cout << std::left
              << std::setw(15) << "Material"
              << std::setw(18) << "Wing Type"
              << std::setw(20) << "Takeoff"
              << std::setw(12) << "Weight"
              << std::setw(18) << "Manufacturer"
              << std::setw(18) << "Model"
              << '\n';

    std::cout << std::string(101, '-') << '\n';

    for (const auto& planner : planners) {
        std::cout << std::left
                  << std::setw(15) << planner.getMaterial()
                  << std::setw(18) << planner.getWingType()
                  << std::setw(20) << planner.getTakeoff()
                  << std::setw(12) << std::fixed << std::setprecision(2) << planner.getWeight()
                  << std::setw(18) << planner.getManufacturer()
                  << std::setw(18) << planner.getModel()
                  << '\n';
    }
}

int main() {
    std::list<Planner> planners;

    for (int i = 1; i <= 3; ++i) {
        planners.emplace_back(inputPlanner(i));
    }

    planners.sort([](const Planner& a, const Planner& b) {
        return a.getWeight() > b.getWeight();
    });

    printTable(planners);

    planners.clear();

    std::cout << "\nAll data has been removed from the list.\n";
    return 0;
}