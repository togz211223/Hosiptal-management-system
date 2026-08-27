#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

enum Department {
    CARDIOLOGY,
    NEUROLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    EMERGENCY,
    GENERAL
};

enum RoomType {
    GENERAL_WARD,
    ICU,
    PRIVATE_ROOM,
    SEMI_PRIVATE
};

class EmergencyCase {
private:
    int patientId;
    int severity;

public:
    EmergencyCase(int pid, int s);

    int getPatientId() const;
    int getSeverity() const;

    bool operator<(const EmergencyCase& other) const;
};

EmergencyCase::EmergencyCase(int pid, int s) {
    patientId = pid;
    severity = s;
}

int EmergencyCase::getPatientId() const {
    return patientId;
}

int EmergencyCase::getSeverity() const {
    return severity;
}

bool EmergencyCase::operator<(const EmergencyCase& other) const {
    return severity < other.severity;
}

class Patient {
private:
    int id;
    string name;
    int age;
    string contact;

    stack<string> medicalHistory;
    queue<string> testQueue;
    vector<string> prescriptions;

    bool isAdmitted;
    RoomType roomType;

    double bill;

public:
    Patient(int pid, string n, int a, string c) {
        id = pid;
        name = n;
        age = a;
        contact = c;
        isAdmitted = false;
        bill = 0.0;
    }

    void admitPatient(RoomType type) {
        if (isAdmitted) {
            cout << "Patient is already admitted." << endl;
            return;
        }
        isAdmitted = true;
        roomType = type;
        addMedicalRecord("Patient admitted to hospital");

        switch (type) {
            case GENERAL_WARD:
                addBill(500);
                break;
            case ICU:
                addBill(3000);
                break;
            case PRIVATE_ROOM:
                addBill(1500);
                break;
            case SEMI_PRIVATE:
                addBill(1000);
                break;
        }
    }

    void dischargePatient() {
        if (!isAdmitted) {
            cout << "Patient is not currently admitted." << endl;
            return;
        }
        isAdmitted = false;
        addMedicalRecord("Patient discharged from hospital");
    }

    void addMedicalRecord(string record) {
        medicalHistory.push(record);
    }

    void requestTest(string testName) {
        testQueue.push(testName);
        addMedicalRecord("Test requested: " + testName);
    }

    string performTest() {
        if (testQueue.empty()) {
            return "No tests pending";
        }
        string testName = testQueue.front();
        testQueue.pop();
        addMedicalRecord("Test performed: " + testName);
        addBill(300);
        return testName;
    }

    void displayHistory() {
        stack<string> temp = medicalHistory;
        while (!temp.empty()) {
            cout << "- " << temp.top() << endl;
            temp.pop();
        }
    }

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    bool getAdmissionStatus() const {
        return isAdmitted;
    }

    void displayPendingTests() {
        queue<string> temp = testQueue;
        while (!temp.empty()) {
            cout << "- " << temp.front() << endl;
            temp.pop();
        }
    }

    void addPrescription(string medicine) {
        prescriptions.push_back(medicine);
        addMedicalRecord("Prescription added: " + medicine);
        addBill(100);
    }

    void displayPrescriptions() {
        if (prescriptions.empty()) {
            cout << "No prescriptions." << endl;
            return;
        }
        for (const string& md : prescriptions) {
            cout << "- " << md << endl;
        }
    }

    void addBill(double amount) {
        bill += amount;
    }

    double getBill() const {
        return bill;
    }

    void displayBill() const {
        cout << "========== PATIENT BILL ==========" << endl;
        cout << "Patient ID: " << id << endl;
        cout << "Patient Name: " << name << endl;
        cout << "Total Bill: $" << bill << endl;
        cout << "==================================" << endl;
    }

    int getAge() const {
        return age;
    }

    string getContact() const {
        return contact;
    }

    RoomType getRoomType() const {
        return roomType;
    }
};

class Doctor {
private:
    int id;
    string name;
    Department department;

    queue<int> appointmentQueue;

public:
    Doctor(int did, string n, Department d) : id(did), name(n), department(d) {}

    void addAppointment(int patientId) {
        appointmentQueue.push(patientId);
    }

    int seePatient() {
        if (appointmentQueue.empty()) {
            return -1;
        }
        int frontPatient = appointmentQueue.front();
        appointmentQueue.pop();
        return frontPatient;
    }

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    string getDepartment() const {
        switch (department) {
            case CARDIOLOGY: return "Cardiology";
            case NEUROLOGY: return "Neurology";
            case ORTHOPEDICS: return "Orthopedics";
            case PEDIATRICS: return "Pediatrics";
            case EMERGENCY: return "Emergency";
            default: return "General";
        }
    }

    void displayAppointments() const {
        queue<int> tempQueue = appointmentQueue;
        while (!tempQueue.empty()) {
            cout << "- Patient ID: " << tempQueue.front() << endl;
            tempQueue.pop();
        }
    }

    void cancelAppointment(int patientId) {
        if (appointmentQueue.empty()) {
            cout << "No appointments available." << endl;
            return;
        }

        queue<int> tempQueue;
        bool found = false;

        while (!appointmentQueue.empty()) {
            int current = appointmentQueue.front();
            appointmentQueue.pop();

            if (current == patientId && !found) {
                found = true;
            }
            else {
                tempQueue.push(current);
            }
        }

        appointmentQueue = tempQueue;

        if (found) {
            cout << "Appointment cancelled successfully." << endl;
        }
        else {
            cout << "Appointment not found." << endl;
        }
    }

    int getAppointmentCount() const {
        return appointmentQueue.size();
    }
};

class Hospital {
private:
    vector<Patient> patients;
    vector<Doctor> doctors;

    queue<int> emergencyQueue;

    priority_queue<EmergencyCase> priorityEmergencyQueue;

    int patientCounter;
    int doctorCounter;

    int generalRooms;
    int icuRooms;
    int privateRooms;
    int semiPrivateRooms;

public:
    Hospital();

    int registerPatient(
        string name,
        int age,
        string contact
    );

    int addDoctor(
        string name,
        Department dept
    );

    void admitPatient(
        int patientId,
        RoomType type
    );

    void addEmergency(
        int patientId
    );

    int handleEmergency();

    void bookAppointment(
        int doctorId,
        int patientId
    );

    void displayPatientInfo(
        int patientId
    );

    void displayDoctorInfo(
        int doctorId
    );

    Patient* findPatient(
        int patientId
    );

    Doctor* findDoctor(
        int doctorId
    );

    void searchPatientByName(
        string name
    );

    void dischargePatient(
        int patientId
    );

    void requestPatientTest(
        int patientId,
        string testName
    );

    void performPatientTest(
        int patientId
    );

    void displayPatientTests(
        int patientId
    );

    void prescribeMedicine(
        int patientId,
        string medicine
    );

    void displayPrescriptions(
        int patientId
    );

    void displayPatientBill(
        int patientId
    );

    void addPriorityEmergency(
        int patientId,
        int severity
    );

    int handlePriorityEmergency();

    bool isRoomAvailable(
        RoomType type
    );

    void displayRoomStatus();

    void displayAllPatients();

    void displayAllDoctors();

    void displayDoctorAppointments(
        int doctorId
    );

    void cancelAppointment(
        int doctorId,
        int patientId
    );

    void doctorSeePatient(
        int doctorId
    );

    void displayStatistics();
};

Hospital::Hospital() {
    patientCounter = 1;
    doctorCounter = 1;
    generalRooms = 20;
    icuRooms = 5;
    privateRooms = 10;
    semiPrivateRooms = 10;
}

int Hospital::registerPatient(string name, int age, string contact) {
    int id = patientCounter++;
    patients.push_back(Patient(id, name, age, contact));
    return id;
}

int Hospital::addDoctor(string name, Department dept) {
    int id = doctorCounter++;
    doctors.push_back(Doctor(id, name, dept));
    return id;
}

Patient* Hospital::findPatient(int patientId) {
    for (auto& patient : patients) {
        if (patient.getId() == patientId) {
            return &patient;
        }
    }
    return nullptr;
}

Doctor* Hospital::findDoctor(int doctorId) {
    for (auto& doctor : doctors) {
        if (doctor.getId() == doctorId) {
            return &doctor;
        }
    }
    return nullptr;
}

bool Hospital::isRoomAvailable(RoomType type) {
    switch (type) {
    case GENERAL_WARD: return generalRooms > 0;
    case ICU:          return icuRooms > 0;
    case PRIVATE_ROOM: return privateRooms > 0;
    case SEMI_PRIVATE: return semiPrivateRooms > 0;
    default:           return false;
    }
}

void Hospital::displayRoomStatus() {
    cout << "========== ROOM STATUS ==========" << endl;
    cout << "General Ward: " << generalRooms << endl;
    cout << "ICU: " << icuRooms << endl;
    cout << "Private Rooms: " << privateRooms << endl;
    cout << "Semi Private Rooms: " << semiPrivateRooms << endl;
}

void Hospital::admitPatient(int patientId, RoomType type) {
    Patient* pat = findPatient(patientId);
    if (pat == nullptr) {
        cout << "Patient with ID " << patientId << " not found." << endl;
        return;
    }

    if (!isRoomAvailable(type)) {
        cout << "No room available for this room type." << endl;
        return;
    }

    pat->admitPatient(type);
}

void Hospital::dischargePatient(int patientId) {
    Patient* pat = findPatient(patientId);
    if (pat == nullptr) {
        cout << "Patient with ID " << patientId << " not found." << endl;
        return;
    }
    pat->dischargePatient();
    cout << "Patient discharged successfully." << endl;
}

void Hospital::bookAppointment(int doctorId, int patientId) {
    Doctor* doc = findDoctor(doctorId);
    if (doc == nullptr) {
        cout << "Doctor with ID " << doctorId << " not found." << endl;
        return;
    }

    Patient* pat = findPatient(patientId);
    if (pat == nullptr) {
        cout << "Patient with ID " << patientId << " not found." << endl;
        return;
    }

    doc->addAppointment(patientId);
    cout << "Appointment booked for patient " << patientId
        << " with doctor " << doctorId << endl;
}

void Hospital::cancelAppointment(int doctorId, int patientId) {
    Doctor* doc = findDoctor(doctorId);
    if (doc == nullptr) {
        cout << "Doctor with ID " << doctorId << " not found." << endl;
        return;
    }
    doc->cancelAppointment(patientId);
}

void Hospital::doctorSeePatient(int doctorId) {
    Doctor* doc = findDoctor(doctorId);
    if (doc == nullptr) {
        cout << "Doctor with ID " << doctorId << " not found." << endl;
        return;
    }

    int patientId = doc->seePatient();
    if (patientId == -1) {
        cout << "No patients waiting." << endl;
    }
    else {
        // Fixed: Removed the hardcoded "Dr. " prefix to prevent the double "Dr." issue
        cout << doc->getName()
            << " is now seeing patient " << patientId << endl;
    }
}

void Hospital::displayDoctorAppointments(int doctorId) {
    Doctor* doc = findDoctor(doctorId);
    if (doc == nullptr) {
        cout << "Doctor with ID " << doctorId << " not found." << endl;
        return;
    }

    cout << "Appointments for " << doc->getName() << ":" << endl;
    if (doc->getAppointmentCount() == 0) {
        cout << "No appointments." << endl;
    }
    else {
        cout << "Appointment Queue:" << endl;
        doc->displayAppointments();
    }
}

void Hospital::addEmergency(int patientId) {
    emergencyQueue.push(patientId);
}

int Hospital::handleEmergency() {
    if (emergencyQueue.empty()) {
        cout << "No emergencies in queue." << endl;
        return -1;
    }

    int patientId = emergencyQueue.front();
    emergencyQueue.pop();
    cout << "Handled emergency for patient: " << patientId << endl;
    return patientId;
}

void Hospital::addPriorityEmergency(int patientId, int severity) {
    Patient* pat = findPatient(patientId);
    if (pat == nullptr) {
        cout << "Patient with ID " << patientId << " not found." << endl;
        return;
    }

    if (severity < 1 || severity > 5) {
        cout << "Invalid severity level. Must be between 1 and 5." << endl;
        return;
    }

    priorityEmergencyQueue.push(EmergencyCase(patientId, severity));
    cout << "Emergency added with severity " << severity << endl;
}

int Hospital::handlePriorityEmergency() {
    if (priorityEmergencyQueue.empty()) {
        cout << "No priority emergencies." << endl;
        return -1;
    }

    EmergencyCase top = priorityEmergencyQueue.top();
    priorityEmergencyQueue.pop();

    cout << "Handling patient " << top.getPatientId()
        << " with severity " << top.getSeverity() << endl;

    return top.getPatientId();
}

void Hospital::searchPatientByName(string name) {
    bool found = false;
    for (Patient& p : patients) {
        if (p.getName() == name) {
            cout << "Patient Found:" << endl;
            cout << "ID: " << p.getId() << endl;
            cout << "Name: " << p.getName() << endl;
            cout << "Age: " << p.getAge() << endl;
            cout << "Contact: " << p.getContact() << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Patient not found." << endl;
    }
}

void Hospital::displayPatientInfo(int patientId) {
    Patient* pat = findPatient(patientId);
    if (pat == nullptr) {
        cout << "Patient with ID " << patientId << " not found." << endl;
        return;
    }
    cout << "Patient Information:" << endl;
    cout << "ID: " << pat->getId() << endl;
    cout << "Name: " << pat->getName() << endl;
    cout << "Admission Status: " << (pat->getAdmissionStatus() ? "Admitted" : "Not Admitted") << endl;
    cout << "Medical History for " << pat->getName() << " (ID: " << pat->getId() << "):" << endl;
    pat->displayHistory();
}

void Hospital::displayDoctorInfo(int doctorId) {
    Doctor* doc = findDoctor(doctorId);
    if (doc == nullptr) {
        cout << "Doctor with ID " << doctorId << " not found." << endl;
        return;
    }
    cout << "Doctor Information:" << endl;
    cout << "ID: " << doc->getId() << endl;
    cout << "Name: " << doc->getName() << endl;
    cout << "Department: " << doc->getDepartment() << endl;
}

void Hospital::requestPatientTest(int patientId, string testName) {
    Patient* pat = findPatient(patientId);
    if (pat == nullptr) {
        cout << "Patient with ID " << patientId << " not found." << endl;
        return;
    }
    pat->requestTest(testName);
    cout << "Test requested successfully." << endl;
}

void Hospital::performPatientTest(int patientId) {
    Patient* pat = findPatient(patientId);
    if (pat == nullptr) {
        cout << "Patient with ID " << patientId << " not found." << endl;
        return;
    }
    string testName = pat->performTest();
    if (testName != "No tests pending") {
        cout << "Test result/action: " << testName << endl;
    }
    else {
        cout << testName << endl;
    }
}

void Hospital::displayPatientTests(int patientId) {
    Patient* pat = findPatient(patientId);
    if (pat == nullptr) {
        cout << "Patient with ID " << patientId << " not found." << endl;
        return;
    }
    cout << "Pending Tests:" << endl;
    pat->displayPendingTests();
}

void Hospital::prescribeMedicine(int patientId, string medicine) {
    Patient* pat = findPatient(patientId);
    if (pat == nullptr) {
        cout << "Patient with ID " << patientId << " not found." << endl;
        return;
    }
    pat->addPrescription(medicine);
    cout << "Medicine prescribed successfully." << endl;
}

void Hospital::displayPrescriptions(int patientId) {
    Patient* pat = findPatient(patientId);
    if (pat == nullptr) {
        cout << "Patient with ID " << patientId << " not found." << endl;
        return;
    }
    cout << "Prescriptions:" << endl;
    pat->displayPrescriptions();
}

void Hospital::displayPatientBill(int patientId) {
    Patient* pat = findPatient(patientId);
    if (pat == nullptr) {
        cout << "Patient with ID " << patientId << " not found." << endl;
        return;
    }
    pat->displayBill();
}

void Hospital::displayAllPatients() {
    cout << "========== ALL PATIENTS ==========" << endl;
    for (Patient& p : patients) {
        cout << "ID: " << p.getId()
            << " | Name: " << p.getName()
            << " | Age: " << p.getAge()
            << " | Status: " << (p.getAdmissionStatus() ? "Admitted" : "Not Admitted") << endl;
    }
}

void Hospital::displayAllDoctors() {
    cout << "========== ALL DOCTORS ==========" << endl;
    for (Doctor& d : doctors) {
        cout << "ID: " << d.getId()
            << " | Name: " << d.getName()
            << " | Department: " << d.getDepartment()
            << " | Appointments: " << d.getAppointmentCount() << endl;
    }
}

void Hospital::displayStatistics() {
    int admittedCount = 0;
    double totalBills = 0.0;

    for (Patient& p : patients) {
        if (p.getAdmissionStatus()) {
            admittedCount++;
        }
        totalBills += p.getBill();
    }

    cout << "========== HOSPITAL STATISTICS ==========" << endl;
    cout << "Total Patients: " << patients.size() << endl;
    cout << "Total Doctors: " << doctors.size() << endl;
    cout << "Admitted Patients: " << admittedCount << endl;
    cout << "Waiting Emergencies: " << emergencyQueue.size() << endl;
    cout << "Priority Emergencies: " << priorityEmergencyQueue.size() << endl;
    cout << "Total Generated Bills: $" << totalBills << endl;
    cout << "=========================================" << endl;
}

int main() {
    Hospital hospital;

    int p1 = hospital.registerPatient("John Doe", 35, "555-1234");
    int p2 = hospital.registerPatient("Jane Smith", 28, "555-5678");
    int p3 = hospital.registerPatient("Mike Johnson", 45, "555-9012");

    int d1 = hospital.addDoctor("Dr. Smith", CARDIOLOGY);
    int d2 = hospital.addDoctor("Dr. Brown", NEUROLOGY);
    int d3 = hospital.addDoctor("Dr. Lee", PEDIATRICS);

    hospital.admitPatient(p1, PRIVATE_ROOM);
    hospital.admitPatient(p2, ICU);
    hospital.admitPatient(p1, SEMI_PRIVATE);

    hospital.bookAppointment(d1, p1);
    hospital.bookAppointment(d1, p2);
    hospital.bookAppointment(d2, p3);
    hospital.bookAppointment(999, p1);
    hospital.bookAppointment(d1, 999);

    hospital.requestPatientTest(p1, "Blood Test");
    hospital.requestPatientTest(p1, "X-Ray");
    hospital.requestPatientTest(p1, "MRI");
    hospital.displayPatientTests(p1);
    hospital.performPatientTest(p1);
    hospital.displayPatientTests(p1);

    hospital.addEmergency(p3);
    hospital.addEmergency(p1);
    int emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency();

    hospital.dischargePatient(p1);

    hospital.displayPatientInfo(p1);
    hospital.displayPatientInfo(p2);
    hospital.displayPatientInfo(999);

    hospital.displayDoctorInfo(d1);
    hospital.displayDoctorInfo(d2);
    hospital.displayDoctorInfo(999);

    hospital.displayDoctorAppointments(d1);
    hospital.doctorSeePatient(d1);
    hospital.displayDoctorAppointments(d1);

    hospital.searchPatientByName("John Doe");
    hospital.searchPatientByName("Unknown Patient");

    hospital.prescribeMedicine(p1, "Paracetamol");
    hospital.prescribeMedicine(p1, "Antibiotic");
    hospital.displayPrescriptions(p1);

    hospital.displayPatientBill(p1);
    hospital.displayPatientBill(p2);

    hospital.addPriorityEmergency(p1, 2);
    hospital.addPriorityEmergency(p2, 5);
    hospital.addPriorityEmergency(p3, 3);
    hospital.addPriorityEmergency(p1, 4);

    hospital.handlePriorityEmergency();
    hospital.handlePriorityEmergency();
    hospital.handlePriorityEmergency();
    hospital.handlePriorityEmergency();

    hospital.displayRoomStatus();
    hospital.displayAllPatients();
    hospital.displayAllDoctors();

    hospital.cancelAppointment(d1, p2);

    hospital.displayDoctorAppointments(d1);
    hospital.displayDoctorAppointments(d2);

    hospital.displayStatistics();

    Hospital emptyHospital;
    emptyHospital.displayPatientInfo(1);
    emptyHospital.displayDoctorInfo(1);
    emptyHospital.handleEmergency();
    emptyHospital.handlePriorityEmergency();
    emptyHospital.searchPatientByName("John Doe");
    emptyHospital.displayAllPatients();
    emptyHospital.displayAllDoctors();
    emptyHospital.displayStatistics();

    return 0;
}
