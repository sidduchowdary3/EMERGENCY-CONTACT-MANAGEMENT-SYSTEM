#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

bool check_cellnumber(long long int CellNumber)
{
    string numStr = to_string(CellNumber);
    int length = numStr.length();
    if (length != 10 || (length == 11 && numStr[0] != '0'))
    {
        cout << "Invalid number format. Please enter a 10-digit number." << endl;
        return false;
    }
    return true;
}

bool number_exists(const map<string, long long int>& Contacts, long long int CellNumber)
{
    for (const auto& pair : Contacts)
    {
        if (pair.second == CellNumber)
        {
            return true;
        }
    }
    return false;
}

bool name_exists(const map<string, long long int>& Contacts, const string& Name)
{
    return Contacts.find(Name) != Contacts.end();
}

void creating_contacts(map<string, long long int>& Contacts, map<string, long long int>& emergencyContacts)
{
    string Name;
    long long int CellNumber;
    int choice = 1;
    while (choice)
    {
        cout << "Enter the contact number: ";
        cin >> CellNumber;
        // Check if the number is valid
        while (!check_cellnumber(CellNumber))
        {
            cout << "Please enter a valid 10-digit number: ";
            cin >> CellNumber;
        }
        // Check if the number already exists
        if (number_exists(Contacts, CellNumber))
        {
            cout << "Skipping this contact. Please enter a new number." << endl;
            continue; // Skip to the next iteration
        }
        cin.ignore(); // Ignore newline character after reading choice
        // Read the name and check if it already exists
        cout << "Enter the Name: ";
        getline(cin, Name);
        while (name_exists(Contacts, Name))
        {
            cout << "Name already exists. Please enter a new name: ";
            getline(cin, Name);
        }
        // Map the string to the integer
        char ch;
        cout << "Do you want to save it to Emergency Contacts ? (y/n): ";
        cin >> ch;
        if (ch == 'y' || ch == 'Y')
        {
            emergencyContacts[Name] = CellNumber;
        }
        Contacts[Name] = CellNumber;
        cout << "Enter 1 to add more contacts: ";
        cin >> choice;
        cin.ignore(); // Ignore newline character after reading choice
    }
}

int main()
{
    map<string, long long int> Contacts, emergencyContacts;
    creating_contacts(Contacts, emergencyContacts);
    // Display the mapped values in sorted order by name
    cout << "----------@CONTACTS@----------" << endl;
    cout << "NAME" << " -> " << "NUMBER" << endl;
    vector<pair<string, long long int>> sortedContacts(Contacts.begin(), Contacts.end());
    sort(sortedContacts.begin(), sortedContacts.end());
    for (const auto& pair : sortedContacts)
    {
        cout << pair.first << " -> " << pair.second << endl;
    }
    // Prompt the user to enter answered call numbers
    cout << "ANSWERED CALLS (enter 0 to stop): " << endl;
    vector<long long int> answeredCalls;
    long long int answeredNumber;
    string Name;
    while (true)
    {
        cin >> answeredNumber;
        if (answeredNumber == 0)
        {
            break;
        }
        if (!check_cellnumber(answeredNumber))
        {
            continue;
        }
        if (!number_exists(Contacts, answeredNumber))
        {
            char choice;
            cout << "Unknown number. Do you want to save it? (y/n): ";
            cin >> choice;
            if (choice == 'y' || choice == 'Y')
            {
                cin.ignore(); // Ignore newline character after reading choice
                cout << "Enter the Name: ";
                getline(cin, Name);
                while (name_exists(Contacts, Name))
                {
                    cout << "Name already exists. Please enter a new name: ";
                    getline(cin, Name);
                }
                char ch;
                cout << "Do you want to save it to Emergency Contacts ? (y/n): ";
                cin >> ch;
                if (ch == 'y' || ch == 'Y')
                {
                    emergencyContacts[Name] = answeredNumber;
                }
                Contacts[Name] = answeredNumber;
            }
        }
        answeredCalls.push_back(answeredNumber);
    }
    // Count the number of times each contact number was dialed
    map<long long int, int> dialedCounts;
    for (long long int number : answeredCalls)
    {
        dialedCounts[number]++;
    }
    // Create a max heap of the most dialed numbers
    auto CompareCounts = [](const pair<long long int, int>& a, const pair<long long int, int>& b)
    {
        return a.second < b.second;
    };
    priority_queue<pair<long long int, int>, vector<pair<long long int, int>>, decltype(CompareCounts)> maxHeap(CompareCounts);
    for (const auto& count : dialedCounts)
    {
        maxHeap.push(count);
    }
    // Creating emergency contacts based on the top three most dialed numbers
    int n = 3;
    while (n > 0 && !maxHeap.empty())
    {
        auto topDialed = maxHeap.top();
        maxHeap.pop();
        for (const auto& pair : Contacts)
        {
            if (pair.second == topDialed.first && emergencyContacts.find(pair.first) == emergencyContacts.end())
            {
                emergencyContacts[pair.first] = pair.second;
                n--;
                break;
            }
        }
    }
    // Display the emergency contacts
    cout << "----------@EMERGENCY CONTACTS@----------" << endl;
    cout << "NAME" << " -> " << "NUMBER" << endl;
    for (const auto& pair : emergencyContacts)
    {
        cout << pair.first << " -> " << pair.second << endl;
    }
    return 0;
}
