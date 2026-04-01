#include <algorithm>
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

public:
    Planner() = default;

    Planner(std::string material,
            std::string wing_type,
            std::string takeoff,
            double weight,
            std::string manufacturer,
            std::string model)
        : material_(std::move(material)),
          wing_type_(std::move(wing_type)),
          takeoff_(std::move(takeoff)),
          weight_(weight),
          manufacturer_(std::move(manufacturer)),
          model_(std::move(model)) {}

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

    void setMaterial(const std::string& material) { material_ = material; }
    void setWingType(const std::string& wing_type) { wing_type_ = wing_type; }
    void setTakeoff(const std::string& takeoff) { takeoff_ = takeoff; }

    void setWeight(double weight) {
        if (weight < 0.0) {
            throw std::invalid_argument("Weight cannot be negative.");
        }
        weight_ = weight;
    }

    void setManufacturer(const std::string& manufacturer) { manufacturer_ = manufacturer; }
    void setModel(const std::string& model) { model_ = model; }
};

std::string readNonEmptyLine(const std::string& prompt) {
    std::string value;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);

        if (!value.empty()) {
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

Planner inputPlanner(int index) {
    std::cout << "\nEnter data for planner #" << index << ":\n";

    const std::string material = readNonEmptyLine("Material: ");
    const std::string wing_type = readNonEmptyLine("Wing type: ");
    const std::string takeoff = readNonEmptyLine("Takeoff (from the ground or from a hill): ");
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
              << std::setw(28) << "Takeoff"
              << std::setw(12) << "Weight"
              << std::setw(18) << "Manufacturer"
              << std::setw(18) << "Model"
              << '\n';

    std::cout << std::string(109, '-') << '\n';

    for (const auto& planner : planners) {
        std::cout << std::left
                  << std::setw(15) << planner.getMaterial()
                  << std::setw(18) << planner.getWingType()
                  << std::setw(28) << planner.getTakeoff()
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