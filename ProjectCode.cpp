#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Event{
    protected:
        int eventID;
        string title;
        string date;
        string venue;

    public:
        Event(int id, string t, string d, string v){
            eventID = id;
            title = t;
            date = d;
            venue = v;
        }

        virtual ~Event(){}

        virtual void displayDetails()= 0;
        virtual string serialize()= 0;

        int getID(){
            return eventID;
        }

        string getDate() {
            return date;
        }

        
        int getDateNumeric(){
            if (date.length() < 10)
                return 0;

            int day = stoi(date.substr(0, 2));
            int month = stoi(date.substr(3, 2));
            int year = stoi(date.substr(6, 4));

            return year * 10000 + month * 100 + day;
        }
};

class Hackathon : public Event{
    private:
        int prizePool;
        int maxTeamSize;

    public:
        Hackathon(int id, string t, string d, string v,int prize, int teamSize)
            :Event(id, t, d, v)
            {
                prizePool=prize;
                maxTeamSize=teamSize;
            }

        void displayDetails() override {
            cout << "\n[HACKATHON] ID: " << eventID << "\n"
                 << "Title: " << title << "\n"
                 << "Date: " << date << "\n"
                 << "Venue: " << venue << "\n"
                 << "Prize Pool: $" << prizePool << "\n"
                 << "Max Team Size: " << maxTeamSize << " members\n";
                 
        }

        string serialize() override {
            ostringstream ss;
            ss << "H," << eventID << "," << title << ","
               << date << "," << venue << ","
               << prizePool << "," << maxTeamSize;

            return ss.str();
        }
};

class ClubEvent : public Event{
    private:
        string clubName;
        double entryFee;

    public:
        ClubEvent(int id, string t, string d, string v,
                  string club, double fee)
            : Event(id, t, d, v),
              clubName(club), entryFee(fee){}

        void displayDetails() override {
            cout << "\n[CLUB EVENT] ID: " << eventID << "\n"
                 << "Title: " << title << "\n"
                 << "Date: " << date << "\n"
                 << "Venue: " << venue << "\n"
                 << "Organizing Club: " << clubName << "\n"
                 << "Entry Fee: $" << entryFee << "\n";
                 
        }

        string serialize() override {
            ostringstream ss;
            ss << "C," << eventID << "," << title << ","
               << date << "," << venue << ","
               << clubName << "," << entryFee;

            return ss.str();
        }
};


class EventManager {
private:
    Event* events[100];
    int eventCount;

    void saveEventToFile(Event* e){
        ofstream outFile("events.txt", ios::app);
        if (outFile.is_open()) {
            outFile << e->serialize() << "\n";
            outFile.close();
        }
    }

    void sortEventsByDate() {
        for (int i = 1; i < eventCount; i++) {
            Event* key = events[i];
            int j = i - 1;
            while (j >= 0 &&events[j]->getDateNumeric() > key->getDateNumeric()) {
                events[j + 1] = events[j];
                j--;
            }
            events[j + 1] = key;
        }
    }

    int binarySearchEventIndexByID(int id) {
        Event* idSortedEvents[100];

        for (int i = 0; i < eventCount; i++) {
            idSortedEvents[i] = events[i];
        }

        // Insertion-sort 
        for (int i= 1; i < eventCount; i++) {
            Event* key = idSortedEvents[i];
            int j = i - 1;

            while (j >= 0 &&
                   idSortedEvents[j]->getID() > key->getID()) {
                idSortedEvents[j + 1] = idSortedEvents[j];
                j--;
            }

            idSortedEvents[j + 1] = key;
        }

        int low = 0;
        int high = eventCount - 1;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            int midID = idSortedEvents[mid]->getID();

            if (midID == id) {
                for (int i = 0; i < eventCount; i++) {
                    if (events[i] == idSortedEvents[mid]) {
                        return i;
                    }
                }
                return -1;
            }
            else if (midID < id) {
                low = mid + 1;
            }
            else {
                high = mid - 1;
            }
        }

        return -1;
    }

public:
    EventManager(){
        eventCount = 0;
        loadEventsFromFile();
    }

    // Add a new event.
    void addEvent(Event* newEvent, bool saveToFile = true) {
        if (eventCount < 100) {
            events[eventCount++] = newEvent;
            sortEventsByDate();

            if (saveToFile) {
                saveEventToFile(newEvent);
                cout << "\n[Success] Event added and saved to file successfully!\n";
            }
        }
        else {
            cout << "\n[Error] Event storage is full!\n";
        }
    }

    void loadEventsFromFile(){}

    void displayAllEvents(){
        if (eventCount == 0) {
            cout<<"\nNo upcoming events available \n";
            return;
        }
        cout<< "\n--- UPCOMING EVENTS ----\n";

        for (int i = 0; i < eventCount; i++) {
            events[i]->displayDetails();
        }
        cout << "\n";
    }

    
    // BINARY SEARCH: SEARCH EVENT BY ID
    void searchEvent(int id) {
        int index=binarySearchEventIndexByID(id);

        if (index != -1) {
            cout << "\n[Found]";
            events[index]->displayDetails();
        }
        else {
            cout << "\n[Error] Event with ID "
                 << id << " not found.\n";
        }
    }

};




bool authenticateAdmin(){
    string password;
    cout<<"\nEnter Admin Password: ";
    cin>>password;

    if(password == "admin123") {
        cout << "\nWelcome Admin!\n";
        return true;
    }

    cout << "\nIncorrect Password.\n";
    return false;
}

bool authenticateStudent(string& loggedInID){
    int choice;
    cout << "\n--- STUDENT LOGIN PORTAL ---"
         << "\n1. Login"
         << "\n2. Register New Account"
         << "\nEnter your choice: ";

    cin >> choice;

    string studentID, password;

    cout << "Enter Student ID: ";
    cin >> studentID;

    cout << "Enter Password: ";
    cin >> password;

    if (choice == 2) {
    }
    else if (choice == 1) {
    }
    else {
        cout << "\nInvalid choice.\n";
    }

    return false;
}

void registerStudentToFile(string studentID, int eventID){}

void viewRegisteredEvents(string studentID, EventManager& manager) {}


void adminMenu(EventManager& manager) {
    int choice;

    do {
        cout << "\n--- ADMIN PANEL ---"
             << "\n1. Add Hackathon"
             << "\n2. Add Club Event"
             << "\n3. View All Events"
             << "\n4. Logout"
             << "\nEnter your choice: ";

        cin >> choice;

        if (choice == 1 || choice == 2) {
            int id;
            string title, date, venue;

            cout<<"Enter Event ID: ";
            cin >> id;

            cout<<"Enter Title: ";
            getline(cin, title);

            cout<<"Enter Date (DD/MM/YYYY): ";
            getline(cin, date);

            cout<<"Enter Venue: ";
            getline(cin, venue);

            if (choice == 1) {
                int prize, teamSize;
                cout<<"Enter Prize Pool : ";
                cin>>prize;

                cout<<"Enter Max Team Size: ";
                cin>>teamSize;
            }
            else {
                string club;
                double fee;

                cout << "Enter Club Name: ";
                cin >> club;

                cout << "Enter Entry Fee ($): ";
                cin >> fee;

            }
        }
        else if (choice == 3) {
            manager.displayAllEvents();
        }
        else if (choice != 4) {
            cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 4);
}


void studentMenu(EventManager& manager, string studentID) {
    int choice;

    do {
       cout << "\n--- STUDENT PORTAL (Logged in as: "
     << studentID << ") ---"
     << "\n1. Browse Upcoming Events"
     << "\n2. Search Event by ID"
     << "\n3. Register for Event"
     << "\n4. My Registered Events"
     << "\n5. Logout"
     << "\nEnter your choice: ";

    cin >> choice;

    switch (choice) {
    case 1:
        manager.displayAllEvents();
        break;

    case 2: 
        int id;
        cout << "Enter Event ID to search: ";
        cin >> id;
        manager.searchEvent(id);
        break;
    
    case 3: 
        int Newid;
        cout << "Enter Event ID you want to register for: ";
        cin >> Newid;

        registerStudentToFile(studentID, id);
        break;
    
    case 4:
        viewRegisteredEvents(studentID, manager);
        break;

    case 5:
        cout << "\nLogging out...\n";
        break;

    default:
        cout << "Invalid choice. Try again.\n";
    }
    } while (choice != 5);
}


int main() {
    EventManager manager;
    int mainChoice;
        cout << "\n========================================="
             << "\n  CAMPUS EVENT & HACKATHON PORTAL"
             << "\n========================================="
             << "\n1. Admin Login"
             << "\n2. Student Login / Portal"
             << "\n3. Exit"
             << "\nEnter your choice: ";

        cin >> mainChoice;

        switch(mainChoice) {
            case 1:
                break;

            case 2: {
                break;
            }

            case 3:
                break;

            default:
                cout << "Invalid choice. Please select 1, 2, or 3.\n";
        }

    

    return 0;
}