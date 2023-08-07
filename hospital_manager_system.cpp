#include<iostream>
#include<vector>
using namespace std;

// forward declaration
class HospitalManager;
class Report;

class Patient{
private:
    int id;
    string name, problem, bloodGroup, mobile;
    int age, doctorId;
    bool isMale, feePaid;

public:
    Patient() { }
    Patient(int id, string name, int age, string problem, string bloodGroup,
                string mobile, bool isMale, int doctorID){
        this->id = id;
        this->name = name;
        this->age = age;
        this->problem = problem;
        this->bloodGroup = bloodGroup;
        this->mobile = mobile;
        this->isMale = isMale;
        this->doctorId = doctorID;
        feePaid = false;
    }
    void display(){
        cout << "Patient [ID : " << id << "]";
        cout << "\n  - Name : \t" << name;
        cout << "\n  - Age : \t" << age;
        cout << "\n  - Gender : \t" << (isMale ? 'M' : 'F');
        cout << "\n  - Blood Group : " << bloodGroup;
        cout << "\n  - Contact : \t" << mobile;
        cout << "\n  - Problem : \t" << problem << endl;
    }
    int getID(){
        return id;
    }
    string getName(){
        return name;
    }
    void clearFee(){
        feePaid = true;
    }
    bool isPaid(){
        return feePaid;
    }
    int getDoctorId(){
        return doctorId;
    }

    friend void dischargePatient(HospitalManager &admin, int id);
};

class Doctor{
private:
    int id, visitFee;
    string name, speciality;

public:
    Doctor() { }

    Doctor(int id, string name, string speciality, int visitFee){
        this->id = id;
        this->name = name;
        this->speciality = speciality;
        this->visitFee = visitFee;
    } 
    void display(){
        cout << "Name      : " << name << " \t| Doctor ID : " << id;
        cout << "\nSpeciality: " << speciality <<  "   \t| Fee : " << visitFee << " tk" << endl;
    }

    void shortDisplay(){
        cout << name << " [Doctor ID - " << id << " | Fee : " << visitFee << " tk]\n";
    }

    int getFee(){
        return visitFee;
    }
    int getID(){
        return id;
    }
    string getName(){
        return name;
    }
    string getSpeciality(){
        return speciality;
    }
};

const int maxPatient = 100;
int lastPatientId = 1000;
int lastDoctorId = 100;

class HospitalManager{
private:
    vector<Doctor> doctors;
    vector<Patient> patients;

public:
    HospitalManager(){ }

    void addNewDoctor(string name, string speciality, int visitFee){
        doctors.push_back(Doctor(++lastDoctorId, name, speciality, visitFee));
        cout << "\n[+] - Added New doctor with ID : " << lastDoctorId << "\n";
    }

    void removeDoctor(int id){
        int indx = findDoctor(id);
        if(indx == -1) 
            cout << "\n[!] - Invalid Doctor ID\n";
        else{
            cout << "\n[!] - Doctor [" << doctors[indx].getName() << "] Removed\n";
            doctors.erase(doctors.begin()+indx);
        }
    }

    int findDoctor(int id){
        for(int i = 0; i<doctors.size(); i++){
            if(doctors[i].getID() == id)
                return i;
        }
        return -1;
    }

    void showAllDoctors(){
        cout << "\n\t\tAll Doctors (" << doctors.size() << ")\n\n";
        for(Doctor d: doctors){
            d.display();
            cout << endl;
        }
    }
    
    bool admitNewPatient(string name, int age, string problem, string bloodGroup,
                string mobile, bool isMale){
        if(doctors.size() == maxPatient){
            cout << "\n[!] - Patient Capacity is full. Can not admit the patient.\n";
            return false;
        }
        for(Doctor d: doctors){
            if(d.getSpeciality() == problem){
                patients.push_back(
                    Patient(++lastPatientId, name, age, problem, bloodGroup, mobile, isMale, d.getID())
                );
                cout << "\n[+] - Patient Admitted [ID - " << lastPatientId << "]\n";
                cout << "[+] - Patient will be checked up by ";
                d.shortDisplay();
                return true;
            }
        }
        cout << "\n[!] - Could Not admit the Patient!";
        cout << "\n[!] - No Doctor is available for this Problem!" << "\n";
        return false;
    }
    
    void showAllPatients(){
        cout << "\n\t\tAll Patients (" << patients.size() << ")\n\n";
        for(Patient p: patients){
            p.display();
            int docId = findDoctor(p.getDoctorId());
            cout << "  - Medical Fee : " << (p.isPaid() ? "PAID" : "NOT PAID") << "\n"; 
            if(docId == -1)
                cout << "\nNo Doctor is Checking this patient\n";
            else{
                cout << "Checking by ";
                doctors[docId].shortDisplay();
                cout << "\n";
            }
        }
        
    }

    int findPatient(int id){
        for(int i = 0; i<patients.size(); i++){
            if(patients[i].getID() == id)
                return i;
        }
        return -1;
    }

    void searchPatient(int id){
        int idx = findPatient(id);
        if(idx == -1) cout << "\n[!] - No patient found with ID : " << id << "\n";
        else{
            cout << "\n";
            patients[idx].display();
            cout << "  - Medical Fee : " << (patients[idx].isPaid() ? "PAID" : "NOT PAID") << "\n"; 
            int docId = findDoctor(patients[idx].getDoctorId());
            if(docId == -1)
                cout << "\nNo Doctor is Checking this patient\n";
            else{
                cout << "\nThe Patient is getting checked by =>\n";
                doctors[docId].display();
            }
        }
    }

    void searchPatient(string name){
        for(int i = 0; i<patients.size(); i++){
            if(patients[i].getName() == name){
                cout << "\n";
                patients[i].display();
                cout << "  - Medical Fee : " << (patients[i].isPaid() ? "PAID" : "NOT PAID") << "\n"; 
                int docId = findDoctor(patients[i].getDoctorId());
                if(docId == -1)
                    cout << "\nNo Doctor is Checking this patient\n";
                else{
                    cout << "\nThe Patient is getting checked by =>\n";
                    doctors[docId].display();
                }
                return;
            }
        }
        cout << "\n[!] - No patient found with Name : " << name << "\n";
    }

    void payMedicalFee(int id){
        int idx = findPatient(id);
        if(idx == -1) cout << "\n[!] - No patient found with ID : " << id << "\n";
        else{
            cout << "\n[+] - Patient name : " << patients[idx].getName();
            int docId = findDoctor(patients[idx].getDoctorId());
            if(docId != -1)
                cout << "\n[+] - Fee Amount : " << doctors[docId].getFee() << " tk\n";
            
            if(patients[idx].isPaid())
                cout << "\n[+] - Fee Already Paid\n";
            else{
                patients[idx].clearFee();
                cout << "\n[+] - Fee Paid Successfully\n";
            }
        }
    }

    int getPatientCount(){
        return patients.size();
    }

    int getDoctorCount(){
        return doctors.size();
    }

    friend void dischargePatient(HospitalManager &, int);
};

class ReportItem{
    string problem;
    int patientCount;
    float avgAge;
public:
    ReportItem(string p, int c, float a){
        problem = p;
        patientCount = c;
        avgAge = a;
    }
    friend class Report;
    friend void displayReport(Report &, HospitalManager &);
};

class Report{
private:
    vector<ReportItem> problems;

public:
    Report() { }

    void addProblem(string problem, int age){
        for(int i = 0; i<problems.size(); i++){
            if(problems[i].problem == problem){
                problems[i].avgAge = 
                    (problems[i].avgAge * problems[i].patientCount + age) 
                    / ++problems[i].patientCount;
                return;
            }
        }
        problems.push_back(ReportItem(problem, 1, age));
    }

    friend void displayReport(Report &s, HospitalManager &admin);
};

void displayReport(Report &s, HospitalManager &admin){
    cout << "\nTotal Patient admitted till now : " << lastPatientId-1000;
    cout << "\nCurrent Patient count           : " << admin.getPatientCount();
    cout << "\nCurrent Doctor count            : " << admin.getDoctorCount() << "\n\n";
    for(auto p: s.problems){
        cout << "Problem Type : " << p.problem << "\t Patient Count : " << p.patientCount << "\t Average Age : " << p.avgAge << "\n";
    }
    cout << "\n";
}

void dischargePatient(HospitalManager &admin, int id){
    int indx = admin.findPatient(id);
    if(indx == -1){ 
        cout << "\n[!] - Invalid Patient ID\n";
        return;
    }
    Patient p = admin.patients[indx];
    cout << "\n[+] - Patient name : " << p.getName();
    int docId = admin.findDoctor(p.getDoctorId());
    if(docId != -1)
        cout << "\n[+] - Fee Amount : " << admin.doctors[docId].getFee() << " tk";
    cout << "\n[+] - Fee Status : " << (p.feePaid ? "PAID" : "NOT PAID") << "\n"; 

    if(!p.feePaid){
        cout << "\n[!] - Fee needs not be paid before discharge!\n";
    }else{
        admin.patients.erase(admin.patients.begin()+indx);
        cout << "\n[+] - Patient Discharged\n";
    }
}

int main(){
    HospitalManager admin;
    Report report;
    int command, id, age, fee;
    string name, problem, speciality, blood, mobile;
    char gender;
    
    while(true){
        system("cls");

        cout << "\n\t\tHospital Management System\n";
        cout << "\t\t--------------------------\n\n";
        cout << " \t[1] - Appoint new Doctor \n";
        cout << " \t[2] - Show all Doctors \n";
        cout << " \t[3] - Remove a Doctor \n";
        cout << " \t[4] - Admit a Patient \n";
        cout << " \t[5] - Show all Patient \n";
        cout << " \t[6] - Search a Patient \n";
        cout << " \t[7] - Pay medical fee \n";
        cout << " \t[8] - Discharge a Patient \n";
        cout << " \t[9] - Report \n";
        cout << " \t[0] - QUIT \n";
    
        cout << "\n[+] - Choose a Action : ";
        cin >> command;
        
        system("cls");

        switch (command){
        case 1:
            cout << "\n\t Appoint A New Doctor\n\n";
            cout << "Enter Name : ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Speciality : ";
            cin >> speciality;
            cout << "Enter Visiting Fee : ";
            cin >> fee;
            admin.addNewDoctor(name, speciality, fee);
            break;

        case 2:
            admin.showAllDoctors();
            break;

        case 3:
            cout << "\n\t Remove Doctor \n\nEnter Doctor ID : ";
            cin >> id;
            admin.removeDoctor(id);
            break;

        case 4:
            cout << "\n\t Admit A Patient\n\n";
            cout << "Enter Name : ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Age : ";
            cin >> age;
            cout << "Enter Gender (M/F) : ";
            cin >> gender;
            cout << "Enter Mobile No. : ";
            cin >> mobile;
            cout << "Enter Blood Group : ";
            cin >> blood;
            cout << "Enter Problem : ";
            cin >> problem;
            if(admin.admitNewPatient(name, age, problem, blood, mobile, gender == 'M')){
                report.addProblem(problem, age);
            }
            break;

        case 5:
            admin.showAllPatients();
            break;

        case 6:
            cout << "\n\t Search Patient \n\n";
            cout << "1. Search by ID\n";
            cout << "2. Search by Name\n";
            cout << "\nEnter Command : ";
            cin >> command;
            if(command == 1){
                cout << "Enter Patient ID : ";
                cin >> id;
                admin.searchPatient(id);
            }else{
                cout << "Enter Patient Full name : ";
                cin.ignore();
                getline(cin, name);
                admin.searchPatient(name);
            }
            break;

        case 7:
            cout << "\n\t Pay Medical Fee \n\nEnter Patient ID : ";
            cin >> id;
            admin.payMedicalFee(id);
            break;

        case 8:
            cout << "\n\t Discharge a Patient \n\nEnter Patient ID : ";
            cin >> id;
            dischargePatient(admin, id);
            break;
        
        case 9:
            cout << "\n\t--- Report ---\n\n";
            displayReport(report, admin);
            break;;

        case 0:
            cout << "\n\t--- Thank your for using ---\n\n";
            return 0;

        default:
            cout << "Invalid Command\n";
        }

        cout << "Press any key...\n";
        cin.ignore();
        getchar();
    }
}