#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath> 
#include <vector>
#include <iomanip>
#include<cctype>
#include<fstream>
//my library
using namespace std;

enum Do { ShowClientList = 1, AddNewClient = 2, DeleteClient = 3,
	UpdateClient = 4, FindClient = 5,transcation=6,ManageMenue=7,Logout = 8};

enum uDo {
	ShowUsersList = 1, AddNewUser = 2, DeleteUser = 3,
	UpdateUser = 4, FindUser = 5, MainMenue = 6
};
enum tDo { deposit = 1, withdraw = 2, total_balances = 3, main_menue = 4 };

struct sclient {
	string account_number;
	string pin_code;
	string name;
	string phone;
 long double account_balance;
};

struct suser {
	string username;
	string password;
	int total_premissions;
};


enum enpremission {
	show_client = 1,
	add_new_clientenum = 2,
	delete_client = 4,
	update_client = 8,
	find_client = 16,
	transcations = 32,
	manage_users = 64,
	logout = 8,
};


suser current_user;

const string clients_file_name = "clients.txt";
const string users_file_name = "users.txt";


void print_main_menue_screen();
vector<sclient> data_to_vector_clients(string filename);
bool find_client_by_account_number(string accountnumber, sclient& client);
void go_to_main_menue();
void transcations_menue();
void go_to_manage_users_menue();
void manage_users_menue();
void login();

int read_right_number(string massage) {
	int number = 0;
	cout << massage;
	cin >> number;
	while (cin.fail()) {
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cin >> number;
	}
	return number;
}

sclient read_new_client() {
	sclient client;

	cout << "Enter Account Number? ";
	getline(cin>>ws, client.account_number);
	while (find_client_by_account_number(client.account_number, client)) {
		cout << "This Account Number (" << client.account_number << ") is already exists\n";
		cout << "Enter another : ";
		getline(cin, client.account_number);
	}
	cout << "Enter PinCode? ";
	getline(cin, client.pin_code);
	
	cout << "Enter Name? ";
	getline(cin, client.name);

	cout << "Enter Phone? ";
	getline(cin, client.phone);

	cout << "Enter Account Balance? ";
	cin >> client.account_balance;
	cin.ignore(); // Clears the leftover newline so getline doesn't get skipped later

	return client;
}

vector <string> split_sentence_extra(string sentence, string separator) {
	vector <string> words;
	string temb;
	short pos = 0;

	while ((pos = sentence.find(separator)) != string::npos) {

		temb = sentence.substr(0, pos);

		if (temb != "") {
			words.push_back(temb);
		}

		sentence.erase(0, pos + separator.length());
	}
	if (sentence != "") {
		words.push_back(sentence);
	}
	return words;
}

sclient read_new_client_without_account_number(string account_number) {
	sclient Client;
	Client.account_number = account_number;
	cout << "Enter PinCode? ";
	getline(cin>>ws, Client.pin_code);

	cout << "Enter Name? ";
	getline(cin, Client.name);

	cout << "Enter Phone? ";
	getline(cin, Client.phone);

	cout << "Enter Account Balance? ";
	cin >> Client.account_balance;
	cin.ignore(); // Clears the leftover newline so getline doesn't get skipped later

	return Client;
}

string convert_data_to_line_clients(sclient Client, string Seperator) {
	string stClientRecord = "";

	stClientRecord += Client.account_number + Seperator;
	stClientRecord += Client.pin_code + Seperator;
	stClientRecord += Client.name + Seperator;
	stClientRecord += Client.phone + Seperator;
	stClientRecord += to_string(Client.account_balance);

	return stClientRecord;
}

sclient convert_line_to_record_clients(string line,string sperator) {
	 sclient client;
	 vector<string> words;
	 words=split_sentence_extra(line, sperator);
	 client.account_number = words[0];
	 client.pin_code = words[1];
	 client.name = words[2];
	 client.phone = words[3];
	 client.account_balance = stod(words[4]);
	 
	 return client;

 }

void print_client_info(sclient client) {
	cout << "The following is a client record : \n\n";
	cout << "Account Number : "<< client.account_number<<endl;
	cout << "PinCode : "<< client.pin_code<<endl;
	cout << "Name : "<<client.name<<endl;
	cout << "Phone : "<<client.phone<<endl;
	cout << "Account Balance : " << client.account_balance << endl;
}

void add_line_to_file(string filename, string dataline) {
	fstream myfile;
	myfile.open(filename, ios::out | ios::app);
	if (myfile.is_open()) {
		myfile << dataline<<endl ;
		myfile.close();
	}
}

void add_new_client() {
	sclient client;
	client = read_new_client();
	add_line_to_file(clients_file_name, convert_data_to_line_clients(client, "#//#"));
}

void add_clients() {
	char check='y';
	do {

		system("cls");
		//delete whats in there
		cout << "------------------------------------";
		cout << "\n\tAdd New Clients Screen\n";
		cout << "------------------------------------\n";
		cout << "adding new client\n\n";
		add_new_client();
		cout << "\nclient added successfully!\n";
		cout << "do you want to add more (y for yes / else for no ) : ";
		cin >> check;

	} while (toupper(check) == 'Y');
}

int count_clients(string filename) {
	sclient client;
	fstream file;
	int count = 0;
	file.open(filename, ios::in);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			count++;
		}
		file.close();
	}

	return count;
}

void show_clients(string filename) {
	sclient client;
	fstream file;
	cout << "\t\t\tClient list (" << count_clients(clients_file_name) << ") Client(s).\n";
	cout << "--------------------------------------------------------------------------------\n\n";
	cout << "| Account number   | Pin Code | Client Name          | Phone         | Balance   ";
	cout << "\n\n--------------------------------------------------------------------------------\n";

	file.open(filename, ios::in);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			client = convert_line_to_record_clients(line, "#//#");
			cout << "\n";
			cout << "| " << left << setw(17) << client.account_number;
			cout << "| " << left << setw(9) << client.pin_code;
			cout << "| " << left << setw(21) << client.name;
			cout << "| " << left << setw(14) << client.phone;
			cout << "| " << left << setw(12) << client.account_balance;

		}
		file.close();
	}
	cout << "\n\n--------------------------------------------------------------------------------";

}

string read_account_number() {
	string account_number;
	cout << "Please enter an account number? ";
	cin >> account_number;
	cout << endl;
	return account_number;
}

vector<sclient> data_to_vector_clients(string filename) {
	fstream file;
	vector<sclient> data;
	
	file.open(filename, ios::in);

	if (file.is_open())
	{
		string line;
		sclient client;
		while (getline(file, line))
		{
			client = convert_line_to_record_clients(line,"#//#");
			data.push_back(client);
		}
		file.close();
	}
	return data;
}

void save_vector_to_file(string filename,vector<string> file_contant) 
{
	fstream file;
	file.open(filename, ios::out);
	
	if (file.is_open()) 
	{

		for(string record : file_contant)
		{
			file << record << endl;
		}
		file.close();
	}
}

void delete_client_by_account_number(string filename,string accountnumber) 
{
	vector<sclient> file_contant = data_to_vector_clients(filename);
	vector<string> new_file;
	for(sclient data : file_contant)
	{
		if(data.account_number!=accountnumber)
		{
			new_file.push_back(convert_data_to_line_clients(data,"#//#"));
		}
	}
	save_vector_to_file(filename, new_file);

}

bool print_client_by_account_number(string account_number,sclient client) 
{
if(find_client_by_account_number(account_number,client))
{
	print_client_info(client);
	return 1;
}
else
{
	cout << "Client with Account Number (" << account_number << ") is Not found!";
	return 0;
}
}

void update_client_by_account_number(string filename, string accountnumber) {
	vector<sclient> file_content = data_to_vector_clients(filename);
	vector<string> new_file;
	for (sclient data : file_content)
	{
		if (data.account_number != accountnumber)
		{
			new_file.push_back(convert_data_to_line_clients(data, "#//#"));
		}
		else {
			new_file.push_back(convert_data_to_line_clients(read_new_client_without_account_number(accountnumber),"#//#"));
		}
	}
	save_vector_to_file(filename, new_file);
}

bool find_client_by_account_number(string accountnumber, sclient& client) {
	vector<sclient> vclient = data_to_vector_clients(clients_file_name);

	for (sclient c : vclient)
	{
		if (c.account_number == accountnumber) {
			client = c;
			return 1;
		}
	}
	return 0;
}

void find_client_by_account_number_with_delete_optain(sclient client) {
	cout << "------------------------------------";
	cout << "\n\tDelete Client Screen\n";
	cout << "------------------------------------\n";
	cout << endl;
	string account_number = read_account_number();
	if (print_client_by_account_number(account_number, client)) {

		cout << "\nDo you want to delete this client? y/n : ";
		char ans = 'y';
		cin >> ans;

		if (toupper(ans) == 'Y') {
			delete_client_by_account_number(clients_file_name, account_number);
			cout << "\nClient deleted successfully.\n";
		}
		else {

		}
	}
}

void find_client_by_account_number_with_update_optain(sclient client) {
	cout << "------------------------------------";
	cout << "\n\tUpdate Client Info Screen\n";
	cout << "------------------------------------\n";
	cout << endl;
	string account_number = read_account_number();
	if (print_client_by_account_number(account_number, client)) {

		cout << "\nDo you want to Update this client? y/n : ";
		char ans = 'y';
		cin >> ans;

		if (toupper(ans) == 'Y') {
			cout << endl;
			update_client_by_account_number(clients_file_name, account_number);
			cout << "\nClient updated successfully.\n";
		}
		else {
			cout << "GoodBye.\n";
		}
	}
}

void go_to_transaction_menue() {
	cout << "\npress any key to go back to transaction menue...";
	system("pause>0");
	system("cls");
	transcations_menue();
}

double count_baclances(string filename) {
	sclient client;
	fstream file;
	double count = 0;
	file.open(filename, ios::in);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			client = convert_line_to_record_clients(line, "#//#");
			count += client.account_balance;
		}
		file.close();
	}

	return count;
}

void show_total_balances() {
	sclient client;
	fstream file;
	cout << "\t\t\tClient list (" << count_clients(clients_file_name) << ") Client(s).\n";
	cout << "---------------------------------------------------------\n\n";
	cout << "| Account number   | Client Name          | Balance   ";
	cout << "\n\n---------------------------------------------------------\n";

	file.open(clients_file_name, ios::in);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			client = convert_line_to_record_clients(line, "#//#");
			cout << "\n";
			cout << "| " << left << setw(17) << client.account_number;
			cout << "| " << left << setw(21) << client.name;
			cout << "| " << left << setw(12) << client.account_balance;

		}
		file.close();
	}
	cout << "\n\n---------------------------------------------------------";
	cout << "\n\t\tTotal balances = " << count_baclances(clients_file_name) << endl;
}

void do_deposit(string accountnumber,double deposit) {
	vector<sclient> client_trans = data_to_vector_clients(clients_file_name);
	vector<string>new_file;
	for (sclient data : client_trans) {
		if (data.account_number == accountnumber) {
			data.account_balance += deposit;
			new_file.push_back(convert_data_to_line_clients(data, "#//#"));
		}
		else {
			new_file.push_back(convert_data_to_line_clients(data, "#//#"));
		}
	}
	save_vector_to_file(clients_file_name, new_file);
}

void deposit_screen_trans(sclient client) {
	cout << "------------------------------------";
	cout << "\n\t Deposit Screen\n";
	cout << "------------------------------------\n";
	cout << endl;
	string account_number;
	do {
		account_number = read_account_number();
		if (!(find_client_by_account_number(account_number, client)))
			cout << "Client with (" << account_number << ") is not exist.\n";

	} while (!(find_client_by_account_number(account_number,client)));
	print_client_by_account_number(account_number, client);
		cout << "\nEnter deposit amount : ";
		double deposit=read_right_number("");
		cout << "\nAre you sure you want to prefrom this transcation ? y/n : ";
		char test = 'y';
		cin >> test;
		if (toupper(test) == 'Y') {
			do_deposit(account_number, deposit);
			cout << "\n\nDeposit done successfully\n";
		}

		
	}

void do_withdraw(string accountnumber, double withdraw) {
	vector<sclient> client_trans = data_to_vector_clients(clients_file_name);
	vector<string>new_file;
	for (sclient data : client_trans) {
		if (data.account_number == accountnumber) {
			data.account_balance -= withdraw;
			new_file.push_back(convert_data_to_line_clients(data, "#//#"));
		}
		else {
			new_file.push_back(convert_data_to_line_clients(data, "#//#"));
		}
	}
	save_vector_to_file(clients_file_name, new_file);
}

bool check_vaild_withdraw(string account_number,double withdraw) {
	sclient client;
	fstream file;
	file.open(clients_file_name, ios::in);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			client = convert_line_to_record_clients(line, "#//#");
			if (client.account_number == account_number) {
				if (withdraw > client.account_balance)
					return 0;
			}
		}
		file.close();
	}
	return 1;
}

void withdraw_screen_trans(sclient client) {
	cout << "------------------------------------";
	cout << "\n\t Withdraw Screen\n";
	cout << "------------------------------------\n";
	cout << endl;
	string account_number;
	do {

		account_number = read_account_number();

		if (!(find_client_by_account_number(account_number, client)))
			cout << "Client with (" << account_number << ") is not exist.\n";

	} while (!(find_client_by_account_number(account_number, client)));

	print_client_by_account_number(account_number, client);
	double withdraw;

	do {
		cout << "\nEnter withdraw amount : ";
		withdraw = read_right_number("");
		if (!(check_vaild_withdraw(account_number, withdraw))) {
			cout << "Amount Exceds the balance, you can draw up to : " << client.account_balance<<endl;

		}
	} while (!(check_vaild_withdraw(account_number, withdraw)));

	cout << "\nAre you sure you want to prefrom this transcation ? y/n : ";
	char test = 'y';
	cin >> test;
	if (toupper(test) == 'Y') {
		do_withdraw(account_number, withdraw);
		cout << "\n\Withdraw done successfully\n";
	}


}

void go_to_transcation(int choosen) {
	sclient client;

	switch (choosen)
	{

	case tDo::deposit:
		system("cls");
		deposit_screen_trans(client);
		go_to_transaction_menue();

		break;

	case tDo::withdraw:

		system("cls");
		withdraw_screen_trans(client);
		go_to_transaction_menue();

		break;

	case tDo::total_balances:
		system("cls");
		show_total_balances();
		go_to_transaction_menue();

		break;

	case tDo::main_menue:
		system("cls");
		print_main_menue_screen();
		break;

	default:
		system("cls");
		go_to_transaction_menue();

		break;
	}
}

void transcations_menue() {
	cout << "-------------------------------------------------\n";
	cout << "\t\tTransactions Menue Screen\n";
	cout << "-------------------------------------------------\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "-------------------------------------------------\n";
	cout << "Choose What do you want to do? [1 to 4] : ";
	int choose = read_right_number("");
	go_to_transcation(choose);
}

//Begin with users

suser convert_line_to_record_users(string line, string sperator) {

	suser user;
	vector<string> words;
	words = split_sentence_extra(line, sperator);
	user.username = words[0];
	user.password = words[1];
	user.total_premissions = stoi(words[2]);
	return user;

}

vector<suser> data_to_vector_users(string filename) {

	fstream file;
	vector<suser> data;

	file.open(filename, ios::in);

	if (file.is_open())
	{
		string line;
		suser user;
		while (getline(file, line))
		{
			user = convert_line_to_record_users(line, "#//#");
			data.push_back(user);
		}
		file.close();
	}
	return data;
}

string convert_data_to_line_users(suser user,string Seperator="#//#") {
	string stUsersRecord = "";
	stUsersRecord += user.username + Seperator;
	stUsersRecord += user.password + Seperator;
	stUsersRecord += to_string(user.total_premissions);
	return stUsersRecord;
}

bool find_user_by_username(string username,suser &user) {
	vector<suser> vuser = data_to_vector_users(users_file_name);
	for (suser u : vuser)
	{
		if (u.username == username) {
			user = u;
			return true;
		}
	}
	return false;
}

bool find_user(string username, string password,suser &user) {
	vector<suser> vuser = data_to_vector_users(users_file_name);
	for (suser u : vuser)
	{
		if (u.username == username && u.password == password) {
			user = u;
			current_user = user;
			return 1;
		}
	}return 0;
}

int count_users() {
	fstream file;
	int count = 0;
	file.open(users_file_name, ios::in);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			count++;
		}
		file.close();
	}

	return count;
}

void show_users() {
	system("cls");
	suser user;
	fstream file;
	cout << "\t\tClient list (" << count_users() << ") Client(s).\n";
	cout << "------------------------------------------------------------\n";
	cout << "| User Name        | password          | permissions";
	cout << "\n------------------------------------------------------------\n";
	file.open(users_file_name, ios::in);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			user = convert_line_to_record_users(line, "#//#");
			cout << "| " << left << setw(17) << user.username;
			cout << "| " << left << setw(18) << user.password;
			cout << "| " << left << setw(14) << user.total_premissions;
			cout << endl;
		}
		file.close();
	}
	cout << "\n------------------------------------------------------------";
}

void print_user_info(suser user) {
	cout << "The following is a client record : \n\n";
	cout << "Username : " << user.username << endl;
	cout << "Password : " << user.password << endl;
	cout << "premissions : " << user.total_premissions << endl;
}

bool print_user_by_username(string username,suser user) {
	if (find_user_by_username(username,user))
	{
		print_user_info(user);
		return 1;
	}
	else
	{
		cout << "User with Username (" << username << ") is Not found!";
		return 0;
	}
}

int give_premissions() {
	int premissions = 0;
	char test = 'n';
	cout << "\nDo you want to give access to :\n";

	cout << "show Client List ? y/n : "; cin >> test; if (toupper(test) == 'Y')
		premissions += enpremission::show_client;
	cout << "Add New Client ? y/n : ";cin >> test;if (toupper(test) == 'Y')
		premissions += enpremission::add_new_clientenum;

	cout << "Delete Client ? y/n : ";cin >> test;if (toupper(test) == 'Y')
		premissions += enpremission::delete_client;

	cout << "Update Client Info ? y/n : ";cin >> test;if (toupper(test) == 'Y')
		premissions += enpremission::update_client;

	cout << "Find Client ? y/n : ";cin >> test;if (toupper(test) == 'Y')
		premissions += enpremission::find_client;

	cout << "Transactions ? y/n : ";cin >> test;if (toupper(test) == 'Y')
		premissions += enpremission::transcations;

	cout << "Mangae Users ? y/n : "; cin >> test; if (toupper(test) == 'Y')
		premissions += enpremission::manage_users;
	return premissions;

}

void find_user_screen() {
	suser user;
	system("cls");
	cout << "------------------------------------";
	cout << "\n\tFind User Screen\n";
	cout << "------------------------------------\n";
	cout << endl;
	cout << "Please enter Username : ";
	string username; cin >> username;
	cout << endl;
	print_user_by_username(username,user);
}

suser read_new_user_without_username(string username) {
	suser user;
	user.username = username;
	cout << "Enter Password : ";
	getline(cin >> ws, user.password);

	cout << "\nDo you want to get full access ? (y for yes / else for no ) : ";
	char test; cin >> test;

	if (toupper(test) == 'Y') user.total_premissions = -1;
	else user.total_premissions=give_premissions();

	cin.ignore(); // Clears the leftover newline so getline doesn't get skipped later

	return user;
}

void update_user_by_usernname(string username) {
	vector<suser> file_content = data_to_vector_users(users_file_name);
	vector<string> new_file;
	for (suser data : file_content)
	{
		if (data.username != username)
		{
			new_file.push_back(convert_data_to_line_users(data, "#//#"));
		}
		else {
			new_file.push_back(convert_data_to_line_users(read_new_user_without_username(username), "#//#"));
		}
	}
	save_vector_to_file(users_file_name, new_file);
}

void update_user_screen()
{
	system("cls");
	suser user;
	cout << "------------------------------------";
	cout << "\n\tUpdate User Info Screen\n";
	cout << "------------------------------------\n";
	cout << endl;
	cout << "Please enter Username : ";
	string username; cin >> username;
	cout << endl;
	if (print_user_by_username(username,user)) {

		cout << "\nDo you want to Update this User? y/n : ";
		char ans = 'y';
		cin >> ans;

		if (toupper(ans) == 'Y') {
			cout << endl;
			update_user_by_usernname(username);
			cout << "\cleint updated successfully.\n";
		}

	}
}

void delete_user_by_username(string filename, string username)
{
	vector<suser> file_contant = data_to_vector_users(filename);
	vector<string> new_file;
	for (suser data : file_contant)
	{
		if (data.username != username)
		{
			new_file.push_back(convert_data_to_line_users(data, "#//#"));
		}
	}
	save_vector_to_file(filename, new_file);

}

void delete_user_screen() {
	suser user;
	system("cls");
	cout << "------------------------------------";
	cout << "\n\tDelete User Screen\n";
	cout << "------------------------------------\n";
	cout << endl;
	cout << "Please enter Username : ";
	string username; cin >> username;
	cout << endl;
	if (print_user_by_username(username,user)) {

		cout << "\nDo you want to delete this User? y/n : ";
		char ans = 'y';
		cin >> ans;

		if (toupper(ans) == 'Y') {
			delete_user_by_username(users_file_name, username);
			cout << "\nUser deleted successfully.\n";
		}
	}
}

suser read_new_user() {
	suser user;
	cout << "Enter Username : ";
	getline(cin >> ws, user.username);
	while (find_user_by_username(user.username,user)) {
		cout << "This Account Number (" << user.username << ") is already exists\n";
		cout << "Enter another user name : ";
		getline(cin, user.username);
	}
	cout << "Enter password : ";
	getline(cin, user.password);
	cout << "\nDo you want to get full access ? (y for yes / else for no ) : ";
	char test;cin>> test;
	if (toupper(test) == 'Y') user.total_premissions = -1;
	else user.total_premissions=give_premissions();
	cin.ignore(); // Clears the leftover newline so getline doesn't get skipped later

	return user;
}

void add_new_user() {
	suser user;
	user = read_new_user();
	add_line_to_file(users_file_name, convert_data_to_line_users(user));
}

void add_users() {
		char check = 'y';
		do {

			system("cls");
			//delete whats in there
			cout << "-----------------------------------";
			cout << "\n\tAdd New User Screen\n";
			cout << "-----------------------------------\n";
			cout << "adding new User : \n\n";
			add_new_user();
			cout << "user added successfully!\n";
			cout << "do you want to add more (y for yes / else for no ) ? ";
			cin >> check;
		} while (toupper(check) == 'Y');
	}

void go_to_action_users(int choosen) {

	suser user;

	switch (choosen)
	{

	case uDo::ShowUsersList:
		show_users();
		go_to_manage_users_menue();

		break;

	case uDo::AddNewUser:

		add_users();
		go_to_manage_users_menue();

		break;

	case uDo::DeleteUser:
		delete_user_screen();
		go_to_manage_users_menue();

		break;

	case uDo::UpdateUser:
		update_user_screen();
		go_to_manage_users_menue();

		break;
	case uDo::FindUser:
		find_user_screen();
		go_to_manage_users_menue();

		break;
	case uDo::MainMenue:
		system("cls");
		print_main_menue_screen();
		break;


	default:
		system("cls");
		go_to_manage_users_menue();
		break;
	}
}

void manage_users_menue() {
	cout << "--------------------------------------------------\n";
	cout << "\t\tManage users Menue Screen\n";
	cout << "--------------------------------------------------\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menue.\n";
	cout << "-------------------------------------------------\n";
	cout << "Choose What do you want to do? [1 to 6] : ";
	int choose = read_right_number("");
	go_to_action_users(choose);
}

void go_to_manage_users_menue() {
	cout << "\npress any key to go back to Manage users menue...";
	system("pause>0");
	system("cls");
	manage_users_menue();
}

void go_to_main_menue() {
	cout << "\npress any key to go back to main menue...";
	system("pause>0");
	system("cls");
	print_main_menue_screen();
}

void go_to_action(int choosen) {

	sclient client;

	switch (choosen)
	{

	case Do::ShowClientList :
		system("cls");

		show_clients(clients_file_name);
		go_to_main_menue();

		break;

	case Do::AddNewClient :

		add_clients();
		go_to_main_menue();

		break;

	case Do::DeleteClient :
		system("cls");

		find_client_by_account_number_with_delete_optain(client);
		go_to_main_menue();

		break;

	case Do::UpdateClient :
		system("cls");

		find_client_by_account_number_with_update_optain(client);
		go_to_main_menue();

		break;

	case Do::FindClient:
		system("cls");
		cout << "------------------------------------";
		cout << "\n\tFind Client Screen\n";
		cout << "------------------------------------\n";
		cout << endl;
		print_client_by_account_number(read_account_number(),client);
		go_to_main_menue();
		break;

	case Do::transcation:

		system("cls");
		transcations_menue();
		break;

	case Do::ManageMenue :
		system("cls");
		manage_users_menue();
		go_to_main_menue();
		break;

	case Do::Logout:
		system("cls");
		login();
		break;
	default:
		system("cls");
		go_to_main_menue();
		break;
	}
}

void print_access_denied() {
	system("cls");
	cout << "-----------------------------------------------------------\n";
	cout << "\t\tAccess Denied\n";
	cout << "-----------------------------------------------------------\n";
	cout << "You don't have access to this Please, contact to your admin\n";
	go_to_main_menue();
}

enpremission convert_choose_to_enum_number(int choose) {
	switch (choose) {
	case Do::ShowClientList:
		return enpremission::show_client;
		break;
	case Do::AddNewClient:
		return enpremission::add_new_clientenum;
		break;
	case Do::DeleteClient:
		return enpremission::delete_client;
		break;
	case Do::UpdateClient:
		return enpremission::update_client;
		break;
	case Do::FindClient:
		return enpremission::find_client;
		break;
	case Do::transcation:
		return enpremission::transcations;
		break;
	case Do::ManageMenue:
		return enpremission::manage_users;
		break;
	case Do::Logout:
		return enpremission::logout;
	}
}

int check_access(enpremission premissions,int choosen) {
	if (choosen == 8) {
		return 8;
	}
	if (current_user.total_premissions == -1) {
		return choosen;
	}
	if ((premissions & current_user.total_premissions) == premissions) {
		return choosen;
	}
	else {
		print_access_denied();
	}
}

void print_main_menue_screen() {
	cout << "============================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "============================================\n";
	cout << "\t[1] show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Mangae Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "============================================\n";
	cout << "Choose What do you want to do? [1 to 8] : ";
	int choose = read_right_number("");
	choose = check_access(convert_choose_to_enum_number(choose), choose);
	go_to_action(choose);
}

void login() {
	string username;
	string password;
	suser user;
	cout << "-------------------------------------------\n";
	cout << "\t\tLogin Screen\n";
	cout << "-------------------------------------------\n";
	cout << "Enter Username : ";
	 cin >> username;
	cout << "Enter Password : ";
	 cin >> password;
	while(!find_user(username, password, user)) {
		system("cls");
		cout << "-------------------------------------------\n";
		cout << "\t\tLogin Screen\n";
		cout << "-------------------------------------------\n";
		cout << "Invalid usermae/password!\n";
		cout << "Enter Username : ";
		cin >> username;
		cout << "Enter Password : ";
		cin >> password;
	}
	system("cls");
	print_main_menue_screen();
}

int main()
{
	login();
	return 0;
}
