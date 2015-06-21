#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;
class worker{
public:
    worker(string n)
    {
        name  = n;
    }
    worker(string name, int _id) : worker(name){
        this->id = _id;
    }
    string GetName(){
        return name;
    }
    int GetId(){
        return this->id;
    }
    void SetName(string n){
        this->name = n;
    }
    void SetId(int _id){
        this->id = _id;
    }
	virtual float salaryPerMonth() = 0;
	virtual void showSalary(){}
//this is overloaded method for writing into output file
	virtual void showSalary(ofstream & out){}

protected:
    int id;
    string name;
};
class hourlyPayedWorker : public  worker
{
public:
    hourlyPayedWorker(string name,int _id) : worker (name, _id){}
    hourlyPayedWorker(string name, int _id, int sal) : hourlyPayedWorker(name, _id)  {
        this->hourSalary = sal;
    }
    float salaryPerMonth()	{
		return hourSalary * 20.8 * 8;
	}
	void showSalary(){
        cout << this->GetName() << " ID: " << this->GetId() << " hourly " << this->salaryPerMonth() << endl;
	}
	void showSalary(ofstream &out){
        out << this->GetName() << " ID: " << this->GetId() << " hourly " << this->salaryPerMonth() << endl;
	}
private:
    int hourSalary;
};
class monthlyPayedWorker : public worker
{
public:
    monthlyPayedWorker(string n, int _id) : worker(n, _id) {}
    monthlyPayedWorker(string n, int _id, int sal) : monthlyPayedWorker(n, _id){
        this->monthSalary = sal;
    }
    float salaryPerMonth()
    {
        return monthSalary;
    }
    void showSalary(){
        cout << this->GetName() << " ID: " << this->GetId() << " monthly " << this->salaryPerMonth() << endl;
	}
	void showSalary(ofstream &out){
        out << this->GetName() << " ID: " << this->GetId() << " monthly " << this->salaryPerMonth() << endl;
	}
private:
    int monthSalary;
} ;
inline bool ifLess(worker *, worker *);
inline void readFromFile(ifstream & in,vector <worker*>&);
inline void writeToFile(ofstream& , vector<worker*>&);
#define firstNumNames (5 <= workers.size()? 5 : workers.size())
#define lastNumIdentifiers (3 <= workers.size()? 3 : workers.size())
int main(void)
{
	vector<worker*> workers;
	ofstream out ;
	ifstream in;
	in.open("input.txt");
	out.open("output.txt");
	cout << "Reading data from input.txt\n";
	//handle reading from file...
	//if any error - just exit the program and close files
    try{
        readFromFile(in, workers);
    }
    catch (exception e){
            cout << "Invalid input file format...\n"
                << "Valid format: <\"name\"> <\"monthly\"|\"hourly\"> <salary>\n"
                << "where:\n\"name\" is worker' name\n"
                << "\"monthly\"|\"hourly\" - type of salary\n"
                << "salary - pay for month or hour respectively\n"
                << "terminating the program...\n";
                workers.clear();
                in.close();
                out.close();
            exit(1);
        }
    sort(workers.begin(), workers.end(), ifLess);
    //this is output needed by task - first 5 names and last 3 id
    cout << "First " << firstNumNames << " names of workers: " << endl;
    for(int i = 0;i < firstNumNames; i++){
        cout << workers[i]->GetName() << endl;
	}
	cout << "Last " << lastNumIdentifiers << " identifiers: " << endl;
	for(int i = workers.size() - 1; i > workers.size() - 1 - lastNumIdentifiers; i--){
        cout << workers[i]->GetId() <<  endl;
	}
	//write info about workers in correct format
	cout << "Writing data to output.txt\n";
	writeToFile(out ,workers);
    workers.clear();
    in.close();
    out.close();
	return 0;
}
//this is condition for sort function
inline bool ifLess(worker * w1, worker * w2)
{
    //sort by salary per month
    if(w1->salaryPerMonth() != w2->salaryPerMonth()){
        return w1->salaryPerMonth() > w2->salaryPerMonth();
    }else{
        //if salaries are equal, then sort by name
        return w1->GetName().compare(w2->GetName()) < 0;
    }
}
inline void readFromFile(ifstream & in, vector <worker*>& workers)
{
    int i = 0;
    while(!in.eof()){
        string workerName, typeOfPay;
        int salary;
        in >> workerName;
        //if loop then exit
        if(workerName == ""){
            continue;
        }
        if (!in.is_open()) cout << "fuck;";
        in >> typeOfPay;
        in >> salary;
        //choose the type of payment - exception by default
        if(typeOfPay == "monthly")
        {
            workers.push_back(new monthlyPayedWorker(workerName, ++i, salary));
        }else if(typeOfPay == "hourly"){
            workers.push_back(new hourlyPayedWorker(workerName, ++i, salary));
        } else throw exception();
    }
}
inline void writeToFile(ofstream& out, vector<worker*>& workers){
    for(int i = 0; i< workers.size(); i++){
        workers[i]->showSalary(out);
    }
}
