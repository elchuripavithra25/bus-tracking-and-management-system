#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>
#include <string>
using namespace std;

class Person {
protected:
    string name;
    string id;

public:
    Person(string n = "", string i = "")
        : name(n), id(i) {}

    string getName() const { return name; }
    string getID() const { return id; }

    virtual void displayRole() const = 0;
    virtual ~Person() {}
};

class Driver : public Person {
public:
    Driver(string n = "", string i = "")
        : Person(n, i) {}

    void displayRole() const override {
        cout << "Driver Name: " << name
             << "\nDriver ID: " << id << endl;
    }
};

class Conductor : public Person {
public:
    Conductor(string n = "", string i = "")
        : Person(n, i) {}

    void displayRole() const override {
        cout << "Conductor Name: " << name
             << "\nConductor ID: " << id << endl;
    }
};

class Bus {
private:
    int busNumber;
    string regNumber, routeID, departureTime;
    Driver driver;
    Conductor conductor;
    string location;
    double position;
    string status, allocationDate;

public:
    Bus(int b, string reg, string route, string time,
        Driver d, Conductor c, string loc,
        double pos, string stat, string date)
        : busNumber(b), regNumber(reg), routeID(route),
          departureTime(time), driver(d), conductor(c),
          location(loc), position(pos), status(stat),
          allocationDate(date) {}

    int getBusNumber() const { return busNumber; }
    string getRegNumber() const { return regNumber; }
    string getRouteID() const { return routeID; }
    string getDepartureTime() const { return departureTime; }
    string getLocation() const { return location; }
    double getPosition() const { return position; }
    string getStatus() const { return status; }
    string getAllocationDate() const { return allocationDate; }

    string getDriverID() const { return driver.getID(); }
    string getConductorID() const { return conductor.getID(); }
    string getDriverName() const { return driver.getName(); }
    string getConductorName() const { return conductor.getName(); }

    void setRoute(string route, string time, string date) {
        routeID = route;
        departureTime = time;
        allocationDate = date;
    }

    void setLocation(string loc, double pos) {
        location = loc;
        position = pos;
    }

    void setStatus(string stat) {
        status = stat;
    }

    void setStaff(string dn, string di, string cn, string ci) {
        driver = Driver(dn, di);
        conductor = Conductor(cn, ci);
    }

    void display() const {
        cout << "\nBus Number: " << busNumber
             << "\nRegistration Number: " << regNumber
             << "\nRoute ID: " << routeID
             << "\nDeparture Time: " << departureTime
             << "\nDaily Allocation Date: " << allocationDate
             << "\nLocation: " << location
             << "\nPosition: " << position << " KM"
             << "\nStatus: " << status << endl;

        cout << "\n--- Staff Details ---\n";
        const Person* p = &driver;
        p->displayRole();

        p = &conductor;
        p->displayRole();
    }
};

class DepotSystem {
private:
    vector<Bus> buses;

    int findBus(const string& reg) const {
        for (size_t i = 0; i < buses.size(); i++) {
            if (buses[i].getRegNumber() == reg)
                return static_cast<int>(i);
        }
        return -1;
    }

    void saveData() const {
        ofstream file("buses.txt");

        if (!file) {
            cout << "Error saving bus records!\n";
            return;
        }

        for (const Bus& b : buses) {
            file << b.getBusNumber() << "|"
                 << b.getRegNumber() << "|"
                 << b.getRouteID() << "|"
                 << b.getDepartureTime() << "|"
                 << b.getDriverID() << "|"
                 << b.getDriverName() << "|"
                 << b.getConductorID() << "|"
                 << b.getConductorName() << "|"
                 << b.getLocation() << "|"
                 << b.getPosition() << "|"
                 << b.getStatus() << "|"
                 << b.getAllocationDate() << '\n';
        }
    }

public:
    void loadData() {
        ifstream file("buses.txt");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string f[12];

            for (int i = 0; i < 12; i++) {
                if (!getline(ss, f[i], '|'))
                    f[i] = "";
            }

            try {
                if (f[0].empty() || f[1].empty() ||
                    f[9].empty() || f[10].empty())
                    continue;

                if (findBus(f[1]) != -1)
                    continue;

                buses.push_back(Bus(
                    stoi(f[0]), f[1], f[2], f[3],
                    Driver(f[5], f[4]),
                    Conductor(f[7], f[6]),
                    f[8], stod(f[9]), f[10],
                    f[11].empty() ? "Not Set" : f[11]
                ));
            }
            catch (const exception&) {
                cout << "Skipping invalid record in buses.txt\n";
            }
        }
    }

    void addBus() {
        int number;
        double pos;
        string reg, route, time, dn, di, cn, ci;
        string loc, status, date;

        cout << "\nEnter Bus Number: ";
        if (!(cin >> number)) {
            cout << "Invalid bus number!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Registration Number: ";
        getline(cin, reg);

        if (findBus(reg) != -1) {
            cout << "Bus already exists!\n";
            return;
        }

        cout << "Enter Route ID: ";
        getline(cin, route);

        cout << "Enter Departure Time: ";
        getline(cin, time);

        cout << "Enter Daily Allocation Date (DD-MM-YYYY): ";
        getline(cin, date);

        cout << "Enter Driver ID: ";
        getline(cin, di);

        cout << "Enter Driver Name: ";
        getline(cin, dn);

        cout << "Enter Conductor ID: ";
        getline(cin, ci);

        cout << "Enter Conductor Name: ";
        getline(cin, cn);

        cout << "Enter Current Location: ";
        getline(cin, loc);

        cout << "Enter Position in KM: ";
        if (!(cin >> pos) || !isfinite(pos) || pos < 0) {
            cout << "Invalid position!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Status (Running/Stopped): ";
        getline(cin, status);

        if (status != "Running" && status != "Stopped") {
            cout << "Status must be Running or Stopped!\n";
            return;
        }

        buses.push_back(Bus(number, reg, route, time,
            Driver(dn, di), Conductor(cn, ci),
            loc, pos, status, date));

        saveData();
        cout << "\nBus Added Successfully!\n";
    }

    void displayAll() const {
        if (buses.empty()) {
            cout << "\nNo Bus Records Found!\n";
            return;
        }

        for (const Bus& b : buses) {
            cout << "\n========== BUS DETAILS ==========\n";
            b.display();
        }
    }

    void searchBus() const {
        string reg;
        cout << "\nEnter Registration Number: ";
        getline(cin >> ws, reg);

        int index = findBus(reg);
        if (index != -1)
            buses[index].display();
        else
            cout << "Bus Not Found!\n";
    }

    void assignRoute() {
        string reg, route, time, date;

        cout << "\nEnter Registration Number: ";
        getline(cin >> ws, reg);

        int index = findBus(reg);
        if (index == -1) {
            cout << "Bus Not Found!\n";
            return;
        }

        cout << "Enter New Route ID: ";
        getline(cin, route);

        cout << "Enter Departure Time: ";
        getline(cin, time);

        cout << "Enter New Allocation Date (DD-MM-YYYY): ";
        getline(cin, date);

        buses[index].setRoute(route, time, date);
        saveData();
        cout << "Route Assigned Successfully!\n";
    }

    void updateLocation() {
        string reg, loc;
        double pos;

        cout << "\nEnter Registration Number: ";
        getline(cin >> ws, reg);

        int index = findBus(reg);
        if (index == -1) {
            cout << "Bus Not Found!\n";
            return;
        }

        cout << "Enter New Location: ";
        getline(cin, loc);

        cout << "Enter Position in KM: ";
        if (!(cin >> pos) || !isfinite(pos) || pos < 0) {
            cout << "Invalid position!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        buses[index].setLocation(loc, pos);
        saveData();
        cout << "Location Updated Successfully!\n";
    }

    void trackBus() const {
        string reg;

        cout << "\nEnter Registration Number: ";
        getline(cin >> ws, reg);

        int index = findBus(reg);
        if (index != -1) {
            cout << "\n========== BUS TRACKING ==========\n";
            buses[index].display();
        } else {
            cout << "Bus Not Found!\n";
        }
    }

    void routeDetails() const {
        string route;
        bool found = false;

        cout << "\nEnter Route ID: ";
        getline(cin >> ws, route);

        for (const Bus& b : buses) {
            if (b.getRouteID() == route) {
                b.display();
                found = true;
            }
        }

        if (!found)
            cout << "No Buses Found on This Route!\n";
    }

    // Corrected nearby bus alert function
    void checkAlerts() const {
        double limit;

        cout << "\nEnter Distance Limit in KM: ";

        if (!(cin >> limit) || !isfinite(limit) || limit <= 0) {
            cout << "Enter a valid positive distance!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        bool alert = false;

        for (size_t i = 0; i < buses.size(); i++) {
            for (size_t j = i + 1; j < buses.size(); j++) {

                // Only compare buses on the same route
                // when both buses are running.
                if (buses[i].getRouteID() == buses[j].getRouteID() &&
                    buses[i].getStatus() == "Running" &&
                    buses[j].getStatus() == "Running") {

                    double distance = fabs(
                        buses[i].getPosition() -
                        buses[j].getPosition()
                    );

                    if (distance < limit) {
                        cout << "\n!!! NEARBY BUS ALERT !!!\n";

                        cout << "Bus "
                             << buses[i].getBusNumber()
                             << " and Bus "
                             << buses[j].getBusNumber()
                             << " are very close!\n";

                        cout << "Route: "
                             << buses[i].getRouteID() << '\n';

                        cout << "Distance: "
                             << distance << " KM\n";

                        alert = true;
                    }
                }
            }
        }

        if (!alert)
            cout << "\nNo Nearby Bus Alerts!\n";
    }

    void updateStaff() {
        string reg, dn, di, cn, ci;

        cout << "\nEnter Registration Number: ";
        getline(cin >> ws, reg);

        int index = findBus(reg);
        if (index == -1) {
            cout << "Bus Not Found!\n";
            return;
        }

        cout << "Enter New Driver ID: ";
        getline(cin, di);

        cout << "Enter New Driver Name: ";
        getline(cin, dn);

        cout << "Enter New Conductor ID: ";
        getline(cin, ci);

        cout << "Enter New Conductor Name: ";
        getline(cin, cn);

        buses[index].setStaff(dn, di, cn, ci);
        saveData();
        cout << "Staff Details Updated!\n";
    }

    void updateStatus() {
        string reg, status;

        cout << "\nEnter Registration Number: ";
        getline(cin >> ws, reg);

        int index = findBus(reg);
        if (index == -1) {
            cout << "Bus Not Found!\n";
            return;
        }

        cout << "Enter New Status (Running/Stopped): ";
        getline(cin, status);

        if (status != "Running" && status != "Stopped") {
            cout << "Status must be Running or Stopped!\n";
            return;
        }

        buses[index].setStatus(status);
        saveData();
        cout << "Status Updated!\n";
    }

    void deleteBus() {
        string reg;

        cout << "\nEnter Registration Number: ";
        getline(cin >> ws, reg);

        int index = findBus(reg);
        if (index != -1) {
            buses.erase(buses.begin() + index);
            saveData();
            cout << "Bus Deleted Successfully!\n";
        } else {
            cout << "Bus Not Found!\n";
        }
    }
};

int main() {
    DepotSystem depot;
    depot.loadData();

    int choice = 0;

    do {
        cout << "\n\n======================================";
        cout << "\n  BUS TRACKING AND MANAGEMENT SYSTEM";
        cout << "\n======================================";

        cout << "\n1. Add Bus Details";
        cout << "\n2. View All Buses";
        cout << "\n3. Search Bus";
        cout << "\n4. Assign Route";
        cout << "\n5. Update Bus Location";
        cout << "\n6. Track Bus";
        cout << "\n7. View Route Details";
        cout << "\n8. Check Nearby Bus Alerts";
        cout << "\n9. Update Driver/Conductor Details";
        cout << "\n10. Update Bus Status";
        cout << "\n11. Delete Bus";
        cout << "\n12. Exit";
        cout << "\n\nEnter Your Choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Enter a number from 1 to 12.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Remove the menu input's leftover newline.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: depot.addBus(); break;
            case 2: depot.displayAll(); break;
            case 3: depot.searchBus(); break;
            case 4: depot.assignRoute(); break;
            case 5: depot.updateLocation(); break;
            case 6: depot.trackBus(); break;
            case 7: depot.routeDetails(); break;
            case 8: depot.checkAlerts(); break;
            case 9: depot.updateStaff(); break;
            case 10: depot.updateStatus(); break;
            case 11: depot.deleteBus(); break;
            case 12: cout << "\nThank You!\n"; break;
            default: cout << "\nInvalid Choice!\n";
        }

    } while (choice != 12);

    return 0;
}


