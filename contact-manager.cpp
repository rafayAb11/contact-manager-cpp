#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include<limits>
using namespace std;

struct contactInfo {
    string name;
    string number;
    string email;
};

const string CONTACT_FILE = "contact.txt";

bool isValidEmail(const string &email) { 
    size_t atPos = email.find('@');
    size_t dotPos = email.rfind(".com");
    return atPos != string::npos && dotPos != string::npos && atPos < dotPos;
}

bool isValidNumber(const string &num) {
    int digits = 0;
    for (char ch : num) {
        if (isdigit((unsigned char)ch)) digits++;
        else if (ch=='+' || ch=='-' || ch==' ' || ch=='(' || ch==')') continue;
        else return false; 
    }
    return digits >= 6;
}

vector<contactInfo> readAllContacts() {
    vector<contactInfo> list;
    ifstream in(CONTACT_FILE);
    contactInfo t;
    while (in >> t.name >> t.number >> t.email) {
        list.push_back(t);
    }
    return list;
}

bool writeAllContacts(const vector<contactInfo> &list) {
    ofstream tmp("temp.txt");
    if (!tmp) return false;
    for (const auto &c : list) {
        tmp << c.name << " " << c.number << " " << c.email << "\n";
    }
    tmp.close();
    remove(CONTACT_FILE.c_str());
    if (rename("temp.txt", CONTACT_FILE.c_str()) != 0) {
        remove("temp.txt");
        return false;
    }
    return true;
}

void displayContact(const contactInfo &c) {
    cout << left << setw(20) << c.name
         << setw(15) << c.number
         << setw(30) << c.email << endl;
}

bool findContact(const string &query, contactInfo &result) {
    vector<contactInfo> all = readAllContacts();
    for (auto &e : all) {
        if (e.name == query || e.number == query || e.email == query) {
            result = e;
            return true;
        }
    }
    return false;
}

void createContact() {
    contactInfo c;
    cout << "Enter Name: ";
    getline(cin, c.name);

    do {
        cout << "Enter Number: ";
        cin >> c.number;
        if (!isValidNumber(c.number)) cout << "Invalid number!\n";
    } while (!isValidNumber(c.number));

    do {
        cout << "Enter Email: ";
        cin >> c.email;
        if (!isValidEmail(c.email)) cout << "Invalid email!\n";
    } while (!isValidEmail(c.email));

    ofstream file(CONTACT_FILE, ios::app);
    file << c.name << " " << c.number << " " << c.email << "\n";
    cout << "Contact Added!\n";
}

void searchContact() {
    string query;
    cout << "Enter name, number or email to search: ";
    getline(cin, query);
    contactInfo found;
    if (findContact(query, found)) {
        cout << "Contact Found:\n";
        displayContact(found);
    } else {
        cout << "No contact found.\n";
    }
}

void updateContact() {
    string query;
    cout << "Enter name, number or email to update: ";
    getline(cin, query);
    contactInfo found;
    if (!findContact(query, found)) {
        cout << "No contact found.\n";
        return;
    }

    cout << "Enter new Name: ";
    getline(cin,found.name);

    do {
        cout << "Enter new Number: ";
        cin >> found.number;
        if (!isValidNumber(found.number)) cout << "Invalid number!\n";
    } while (!isValidNumber(found.number));

    do {
        cout << "Enter new Email: ";
        cin >> found.email;
        if (!isValidEmail(found.email)) cout << "Invalid email!\n";
    } while (!isValidEmail(found.email));

    vector<contactInfo> all = readAllContacts();
    bool changed = false;
    for (auto &entry : all) {
        if (entry.name == query || entry.number == query || entry.email == query) {
            entry = found;
            changed = true;
        }
    }
    if (changed) {
        if (!writeAllContacts(all)) {
            cout << "Error saving updated contacts!\n";
        } else {
            cout << "Contact Updated!\n";
        }
    } else {
        cout << "Cannot Find Contact!\n";
    }
}

void deleteContact() {
    string query;
    cout << "Enter name, number or email to delete: ";
    getline(cin, query);
    contactInfo found;
    if (!findContact(query, found)) {
        cout << "No contact found.\n";
        return;
    }

    vector<contactInfo> all = readAllContacts();
    size_t origSize = all.size();
    all.erase(
        remove_if(all.begin(), all.end(), [&](const contactInfo &e){
            return e.name == query || e.number == query || e.email == query;
        }),
        all.end()
    );

    if (all.size() == origSize) {
        cout << "Cannot Find Contact!\n";
    } else {
        if (!writeAllContacts(all)) {
            cout << "Error deleting contact!\n";
        } else {
            cout << "Contact Deleted!\n";
        }
    }
}

void displayContacts() {
    vector<contactInfo> all = readAllContacts();
    cout << left << setw(20) << "Name" << setw(15) << "Number" << setw(30) << "Email" << endl;
    cout << string(65, '-') << endl;
    for (auto &c : all) {
        displayContact(c);
    }
}

int main() {

    { ofstream ofs(CONTACT_FILE, ios::app); }

    int choice;
    do {
        cout << "\n----- Contact Management System -----\n";
        cout << "1. Create Contact\n";
        cout << "2. Search Contact\n";
        cout << "3. Update Contact\n";
        cout << "4. Delete Contact\n";
        cout << "5. Display All Contacts\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: createContact(); break;
            case 2: searchContact(); break;
            case 3: updateContact(); break;
            case 4: deleteContact(); break;
            case 5: displayContacts(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n"; break;
        }
    } while (choice != 0);
    return 0;
}
