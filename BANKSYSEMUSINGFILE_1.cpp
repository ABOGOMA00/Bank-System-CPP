#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

struct sClient
{
    string AcountNumber;
    string PinCode;
    string Phone;
    string Name;
    double AcountBalance = 0;
    bool markofdelet = false;
};

const string NameFile = "Clients.txt";

vector<string> SplitFunciton(string S1, string Delm)
{
    vector<string> vString;
    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delm)) != string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delm.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1);
    }
    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient client;
    vector<string> vclient = SplitFunciton(Line, Seperator);

    client.AcountNumber = vclient[0];
    client.PinCode = vclient[1];
    client.Name = vclient[2];
    client.Phone = vclient[3];
    client.AcountBalance = stod(vclient[4]);

    return client;
}

vector<sClient> LoadDataFromFile(string NameFile)
{
    vector<sClient> vClients;
    fstream MyFile(NameFile, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

void PrintClientRecord(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AcountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AcountBalance << endl;
}

void PrintAllClientsData(const vector<sClient>& vClients)
{
    cout << "\n\t\tClient List (" << vClients.size() << ") Client(s)\n";
    cout << "========================================================================================================================\n";
    cout << "|" << left << setw(15) << "Acount Num"
        << "|" << left << setw(10) << "Pin Code"
        << "|" << left << setw(40) << "Client Name"
        << "|" << left << setw(12) << "Phone"
        << "|" << left << setw(12) << "Balance"
        << endl;
    cout << "========================================================================================================================\n";

    for (const sClient& C : vClients)
        PrintClientRecord(C);

    cout << "========================================================================================================================\n";
}

void ShowAllClient_1()
{
    PrintAllClientsData(LoadDataFromFile(NameFile));
}

bool IsAccountExist(const vector<sClient>& Vclient, const string& accountNumber)
{
    for (const sClient& C : Vclient)
    {
        if (C.AcountNumber == accountNumber)
            return true;
    }
    return false;
}

sClient NewClient(const vector<sClient>& Vclient)
{
    sClient client;

    do
    {
        cout << "Enter your Account number: ";
        getline(cin >> ws, client.AcountNumber);

        if (IsAccountExist(Vclient, client.AcountNumber))
            cout << "Client number [" << client.AcountNumber << "] already exists. Please enter another one.\n";
        else
            break;

    } while (true);

    cout << "Enter your Pin Code: ";
    getline(cin, client.PinCode);

    cout << "Enter your Name: ";
    getline(cin, client.Name);

    cout << "Enter your Phone: ";
    getline(cin, client.Phone);

    cout << "Enter Account Balance: ";
    cin >> client.AcountBalance;
    cin.ignore();

    return client;
}

string ConvertRecordToLine(sClient record, string seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += record.AcountNumber + seperator;
    stClientRecord += record.PinCode + seperator;
    stClientRecord += record.Name + seperator;
    stClientRecord += record.Phone + seperator;
    stClientRecord += to_string(record.AcountBalance);
    return stClientRecord;
}

void UploadDataToFile(string Line, string NameFile)
{
    fstream myfile(NameFile, ios::app | ios::out);
    if (myfile.is_open())
    {
        myfile << Line << endl;
        myfile.close();
    }
}

void AddNewClient()
{
    vector<sClient> vClients = LoadDataFromFile(NameFile);
    sClient Client = NewClient(vClients);
    UploadDataToFile(ConvertRecordToLine(Client), NameFile);
}

void AddClients_2()
{
    char AddMore = 'Y';
    do
    {
        system("cls");
        cout << "Adding New Client:\n\n";
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

string ReadAccountNum()
{
    string AccountNum;
    cout << "Enter your Account NUM: ";
    cin >> AccountNum;
    return AccountNum;
}

bool findclientByAccountNum(string AccountNumber, const vector<sClient>& vClients, sClient& Client)
{
    for (const sClient& C : vClients)
    {
        if (C.AcountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

void printoneclient(sClient client)
{
    cout << "AcountNUM = " << client.AcountNumber << endl;
    cout << "pinCode = " << client.PinCode << endl;
    cout << "Name = " << client.Name << endl;
    cout << "phone = " << client.Phone << endl;
    cout << "AcountBalance = " << client.AcountBalance << endl;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AcountNumber == AccountNumber)
        {
            C.markofdelet = true;
            return true;
        }
    }
    return false;
}

vector<sClient> SaveClientsDataToFile(string FileName, const vector<sClient>& vClients)
{
    fstream MyFile(FileName, ios::out);
    string DataLine;

    if (MyFile.is_open())
    {
        for (const sClient& C : vClients)
        {
            if (!C.markofdelet)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    else
        cout << "Error: Could not open the file!" << endl;

    return vClients;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (findclientByAccountNum(AccountNumber, vClients, Client))
    {
        printoneclient(Client);
        cout << "\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(NameFile, vClients);
            cout << "\nClient Deleted Successfully\n";
            return true;
        }
    }
    else
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    return false;
}

sClient ChangeInfoClient(string AcountNum)
{
    sClient Client;
    string AcountB;

    Client.AcountNumber = AcountNum;

    cout << "\nEnter Pin Code: ";
    getline(cin >> ws, Client.PinCode);
    cout << "Enter Name: ";
    getline(cin, Client.Name);
    cout << "Enter Phone Num: ";
    getline(cin, Client.Phone);
    cout << "Enter Acount Balance: ";
    getline(cin, AcountB);
    Client.AcountBalance = stod(AcountB);

    return Client;
}

bool UpdatedlientUsingAcountNum(string AcountNUM, vector<sClient>& vClients)
{
    sClient client;
    char Answer = 'n';

    if (findclientByAccountNum(AcountNUM, vClients, client))
    {
        printoneclient(client);
        cout << "Do You want Update This [Y][N]: ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& S : vClients)
            {
                if (S.AcountNumber == AcountNUM)
                {
                    S = ChangeInfoClient(AcountNUM);
                    break;
                }
            }
            SaveClientsDataToFile(NameFile, vClients);
            cout << "\nClient Updated Successfully\n";
            return true;
        }
    }
    else
        cout << "\nClient with Account Number (" << AcountNUM << ") is Not Found!";
    return false;
}

void FindClient(string AccountNumber, const vector<sClient>& vClients, sClient& Client)
{
    cout << "-------------------------------------\n";
    cout << "\t\tFindClient\n";
    cout << "-------------------------------------\n";
    if (findclientByAccountNum(AccountNumber, vClients, Client))
        printoneclient(Client);
    else
        cout << "This Account Is NOT Found :[" << AccountNumber << "]\n";
}

int ShowMAunMenueScreen()
{
    int num;
    cout << "===========================================\n";
    cout << "\t\tMain Menu\n";
    cout << "===========================================\n";
    cout << "\t[1] Show ALL client Data\n";
    cout << "\t[2] Add New Client\n";
    cout << "\t[3] Delete Client\n";
    cout << "\t[4] Update Client Info\n";
    cout << "\t[5] Find Client\n";
    cout << "\t[6] Exit\n";
    cout << "===========================================\n";
    cout << "Choose [1 to 6]: ";
    cin >> num;
    cin.ignore();
    return num;
}

void RunTheprogram(string NameFile)
{
    sClient client;
    vector<sClient> vclint;
    int choice;

    do
    {
        vclint = LoadDataFromFile(NameFile);
        choice = ShowMAunMenueScreen();

        system("cls");

        switch (choice)
        {
        case 1: ShowAllClient_1(); break;
        case 2: AddClients_2(); break;
        case 3: DeleteClientByAccountNumber(ReadAccountNum(), vclint); break;
        case 4: UpdatedlientUsingAcountNum(ReadAccountNum(), vclint); break;
        case 5: FindClient(ReadAccountNum(), vclint, client); break;
        case 6: cout << "Exiting...\n"; break;
        default: cout << "Invalid choice!\n"; break;
        }

        system("pause");
        system("cls");
    } while (choice != 6);
}

int main()
{
    RunTheprogram(NameFile);
    return 0;
}
