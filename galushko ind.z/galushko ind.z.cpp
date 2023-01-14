#include <fstream>
#include <ctime>

#define CLIENTS_FILE "clients.txt"
#define SERVICES_FILE "services.txt"
#define USEDSERVICES_FILE "usedservices.txt"

using namespace std;

struct DateTime {
	int day = 0;
	int month = 0;
	int year = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;
};
struct Client {
	char fullname[300];
	unsigned long long int number;
	DateTime begin;
	DateTime end;
	int duty;
	int credit;
};

struct Service {
	char name[300];
	int code;
	double price;
	char interval[100];
};

struct UsedService {
	unsigned long long int number;
	int code;
	DateTime date;
	int count;
};

Client clients[50];
int clients_count = 0;

Service services[50];
int services_count = 0;

UsedService usedservices[50];
int usedservices_count = 0;


void load();
void report();

int main() {
	load();
	report();
	return 0;
}


void readDate(std::ifstream& s, DateTime& d, char sep) {

	char buffer[20];
	s.getline(buffer, 20, '.');
	d.day = atoi(buffer);

	s.getline(buffer, 20, '.');
	d.month = atoi(buffer);

	s.getline(buffer, 20, sep);
	d.year = atoi(buffer);
}

void load() {
	ifstream fin;
	fin.open(CLIENTS_FILE);
	char buffer[20];


	for (clients_count = 0; fin.good(); clients_count++) {

		fin.getline(clients[clients_count].fullname, 300, ',');

		fin.getline(buffer, 20, ',');
		clients[clients_count].number = atoll(buffer);

		readDate(fin, clients[clients_count].begin, ',');

		readDate(fin, clients[clients_count].end, ',');

		fin.getline(buffer, 20, ',');
		clients[clients_count].duty = atoi(buffer);

		fin.getline(buffer, 20);
		clients[clients_count].credit = atoi(buffer);
	}
	fin.close();
	fin.open(SERVICES_FILE);


	for (services_count = 0; fin.good(); services_count++) {

		fin.getline(services[services_count].name, 300, ',');

		fin.getline(buffer, 20, ',');
		services[services_count].code = atoi(buffer);

		fin.getline(buffer, 20, ',');
		services[services_count].price = atof(buffer);

		fin.getline(services[services_count].interval, 100);
	}
	fin.close();
	fin.open(USEDSERVICES_FILE);

	for (usedservices_count = 0; fin.good(); usedservices_count++) {

		fin.getline(buffer, 20, ',');
		usedservices[usedservices_count].number = atoll(buffer);
		fin.getline(buffer, 20, ',');
		usedservices[usedservices_count].code = atoi(buffer);
		fin.getline(buffer, 20, '.');
		usedservices[usedservices_count].date.day = atoi(buffer);
		fin.getline(buffer, 20, '.');
		usedservices[usedservices_count].date.month = atoi(buffer);
		fin.getline(buffer, 20, ' ');
		usedservices[usedservices_count].date.year = atoi(buffer);
		fin.getline(buffer, 20, ':');
		usedservices[usedservices_count].date.hour = atoi(buffer);
		fin.getline(buffer, 20, ':');
		usedservices[usedservices_count].date.minute = atoi(buffer);
		fin.getline(buffer, 20, ',');
		usedservices[usedservices_count].date.second = atoi(buffer);
		fin.getline(buffer, 20);
		usedservices[usedservices_count].count = atoi(buffer);
	}
	fin.close();
}

void report() {
	ifstream fin;
	fin.open("Param.txt");
	char buffer[300];

	fin.getline(buffer, 300);
	fin.close();
	Client client;
	for (int i = 0; i < clients_count; i++)
		if (strcmp(buffer, clients[i].fullname) == 0)
		{
			client = clients[i]; break;
		}

	int start_m, end_m;
	tm t;
	time_t ts = time(0);
	localtime_s(&t, &ts);
	start_m = t.tm_mon / 3 * 3 + 1;
	end_m = start_m + 2;

	ofstream fout;
	fout.open("Report.txt");
	int servs[50];
	for (int i = 0; i < services_count; i++) servs[i] = 0;
	for (int i = 0; i < usedservices_count; i++)
	{
		if (usedservices[i].number == client.number)
		{
			if (usedservices[i].date.month >= start_m && usedservices[i].date.month <= end_m && usedservices[i].date.year == t.tm_year + 1900)
			{
				for (int j = 0; j < services_count; j++)
				{
					if (services[j].code == usedservices[i].code)
					{
						servs[j] += usedservices[i].count * services[j].price;
					}
				}
			}
		}

	}
	for (int i = 0; i < services_count; i++)
	{
		if (servs[i] != 0)
		{
			fout << services[i].name << ": " << servs[i] << endl;
		}
	}
}