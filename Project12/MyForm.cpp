
#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <string>
#include <set>
#include <sstream>   


#include <msclr/marshal_cppstd.h>
#include "MyForm.h"



using namespace std;

using namespace System;
using namespace msclr::interop;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using msclr::interop::marshal_as;

using namespace System::Globalization;

using System::Globalization::CultureInfo;
using System::Globalization::NumberStyles;





int    __clrcall TryParseIntOrDefault(System::Windows::Forms::TextBox^ tb, int defVal);
double __clrcall TryParseDoubleOrDefault(System::Windows::Forms::TextBox^ tb, double defVal);



static std::map<int, std::set<int>> gBusyBySlot;

static std::vector<int> gAllHalls = { 1,2,3,4,5,6,7,8,9,10 };

static std::map<std::string, std::set<int>> gTakenSeats;

static std::string MakeSeatKey(int slotIdx, int hallNo, const std::string& timeBlock) {
    std::ostringstream oss;
    oss << slotIdx << "#" << hallNo << "#" << timeBlock;
    return oss.str();
}

static inline bool IsHallBusy(int slotIdx, int hallNo) {
    auto it = gBusyBySlot.find(slotIdx);
    return (it != gBusyBySlot.end()) && (it->second.count(hallNo) > 0);
}

static inline void MarkHallBusy(int slotIdx, int hallNo) {
    gBusyBySlot[slotIdx].insert(hallNo);
}

struct ContractDraft {
    int movieCode = -1;
    std::string title;
    std::string genre;
    int age = 0;
    int duration = 0;
    int demand = 0;        // 0..100
    double priceUAH = 0.0;
    std::string signDate;  // "yyyy-MM-dd"
    std::string expiryDate;
};

struct AssignmentResult {
    bool ok = false;
    std::string error;
    std::string slotText;  
    int hallNo = 0;
};


// --- NEW: логічні групи ---
// Ідентифікація користувача
struct AuthData {
    int         code = -1;
    std::string pib;       // textBox1
    std::string position;  // comboBox1 selected item text
};

// Стан навігації/сторінки в UI
struct UiState {
    int lastPage = 1;
};

// Дані сторінки бухгалтерії (Accounter page / case 3)
struct AccounterPageData {
    std::string mb_text;    // MB_text
    std::string t_text;     // T_text
    std::string sal_text;   // Sal_text
    int         emp_cb_index = -1; // Emp_CB selected index
};

// Дані сторінки менеджера (Manager page / case 5)
struct ManagerPageData {
    int seans_cb_index = -1;        // Seans_cb
    int hall_cb_selected_value = -1;// Hall_cb selected (1..10)
};

// Дані сторінки координатора (Coordinator page / case 4/6)
struct CoordinatorPageData {
    std::string mt_text, genre_text, age_text, dur_text, demand_text, price_text;
    std::string start_date, end_date; // "yyyy-MM-dd"
    bool ac_checked = false, ad_checked = false;
};

// --- NEW: згрупована чернетка співробітника ---
struct EmpDraft {
    AuthData             auth;
    UiState              ui;
    AccounterPageData    acc;
    ManagerPageData      mgr;
    CoordinatorPageData  coord;
};

// Global in-memory store: employee code -> draft
static std::map<int, EmpDraft> s_empDrafts;



class Employee {
public:
    // ---- Roles ----
    enum class Role { Accountant, Manager, Cinemacoordinator, Unknown };

    // ---- Registration / Authentication ----
    static bool Register(const std::string& pib,
        const std::string& idStr,
        const std::string& posStr,
        std::string& errorOut,
        Role& roleOut)
    {
        std::string pibCanon = CanonPIB(pib);

        if (!ValidatePIB(pibCanon)) {
            errorOut = "Full name must contain exactly 3 words.";
            roleOut = Role::Unknown;
            return false;
        }
        if (!ValidateIDString(idStr)) {
            errorOut = "ID must be 8 digits.";
            roleOut = Role::Unknown;
            return false;
        }

        Role r = ParseRole(posStr);
        if (r == Role::Unknown) {
            errorOut = "Invalid position.";
            roleOut = Role::Unknown;
            return false;
        }

        int idAsInt = std::stoi(idStr);
        if (IsDuplicate(pibCanon, idAsInt)) {
            errorOut = "Employee with same name or ID already exists.";
            roleOut = Role::Unknown;
            return false;
        }

        Employee e;
        e.PIB = pibCanon;
        e.code = idAsInt;
        e.position = posStr;
        employeeList.push_back(e);

        errorOut.clear();
        roleOut = r;
        return true;
    }

    static bool TryAuthenticate(const std::string& pib,
        const std::string& idStr,
        const std::string& posStr,
        Role& roleOut,
        std::string& errorOut)
    {
        roleOut = Role::Unknown;
        errorOut.clear();
        std::string pibCanon = CanonPIB(pib);

        if (pibCanon.empty() || posStr.empty() || idStr.size() != 8) {
            errorOut = "Please enter full name, 8-digit ID and position.";
            return false;
        }

        int idInt = -1;
        try { idInt = std::stoi(idStr); }
        catch (...) { errorOut = "ID must be 8 digits."; return false; }

        bool pibExists = false, idExists = false;
        bool pairPIBID = false, pairPIBPOS = false, pairIDPOS = false;
        const Employee* full = nullptr;

        for (const auto& e : employeeList) {
            if (e.GetPIB() == pibCanon)  pibExists = true;
            if (e.GetCode() == idInt)    idExists = true;

            const bool matchPIB = (e.GetPIB() == pibCanon);
            const bool matchID = (e.GetCode() == idInt);
            const bool matchPOS = (e.GetPosition() == posStr);

            if (matchPIB && matchID && matchPOS) { full = &e; break; }
            if (matchPIB && matchID && !matchPOS) pairPIBID = true;
            if (matchPIB && matchPOS && !matchID)  pairPIBPOS = true;
            if (matchID && matchPOS && !matchPIB) pairIDPOS = true;
        }

        if (full) {
            roleOut = ParseRole(full->GetPosition());
            return true;
        }

        if (pairPIBID || pairPIBPOS || pairIDPOS || (pibExists ^ idExists)) {
            errorOut = "Credentials partially match existing records (all three must match).";
            return false;
        }

        errorOut = "Employee not found. Please register.";
        return false;
    }

    // ---- Salary management ----
    static bool TryGetSalaryByCode(int employeeCode, double& out) {
        auto it = salaryByCode.find(employeeCode);
        if (it == salaryByCode.end()) return false;
        out = it->second;
        return true;
    }
    static void SetSalary(int code, double amount) { salaryByCode[code] = amount; }
    static const std::map<int, double>& GetSalaries() { return salaryByCode; }

    // ---- Employee list ----
    static const std::vector<Employee>& GetEmployeeList() { return employeeList; }
    static bool HasAny() { return !employeeList.empty(); }
    static bool TryGetCodeByIndex(int idx, int& codeOut) {
        if (idx < 0 || idx >= static_cast<int>(employeeList.size())) return false;
        codeOut = employeeList[idx].GetCode();
        return true;
    }

    // ---- Confirmations ----
    void ConfirmAction() { confirmations++; }
    static bool IncrementConfirmations(int code, int delta = 1) {
        for (auto& e : employeeList) {
            if (e.GetCode() == code) { e.confirmations += delta; return true; }
        }
        return false;
    }
    static bool TryGetConfirmations(int code, int& out) {
        for (const auto& e : employeeList) {
            if (e.GetCode() == code) { out = e.confirmations; return true; }
        }
        return false;
    }
    static int RecommendedBonusByConfirmations(int confirmations) {
        return (confirmations / 5) * 1000; // int → int, ок
    }

    // ---- Instance getters ----
    std::string GetDisplayText() const { return PIB + " | " + position; }
    std::string GetPIB()        const { return PIB; }
    int         GetCode()       const { return code; }
    std::string GetPosition()   const { return position; }

private:
    // ---- Instance data ----
    std::string PIB;
    int         code = -1;
    std::string position;
    int         confirmations = 0;

    // ---- Class storage ----
    static std::vector<Employee> employeeList;
    static std::map<int, double> salaryByCode;

    // ---- Internal helpers (validation/parsing) ----
    static bool ValidatePIB(const std::string& pib) {
        size_t i = 0, n = pib.size();
        int words = 0;
        while (i < n) {
            while (i < n && std::isspace(static_cast<unsigned char>(pib[i]))) ++i;
            if (i < n) {
                ++words;
                while (i < n && !std::isspace(static_cast<unsigned char>(pib[i]))) ++i;
            }
        }
        return words == 3;
    }

    static std::string CanonPIB(const std::string& s) {
        std::string out;
        out.reserve(s.size());
        bool inWord = false;
        for (unsigned char ch : s) {
            if (std::isspace(ch)) {
                if (inWord) { out.push_back(' '); inWord = false; }
            }
            else {
                out.push_back(static_cast<char>(ch));
                inWord = true;
            }
        }
        if (!out.empty() && out.back() == ' ') out.pop_back();
        return out;
    }

    static bool ValidateIDString(const std::string& s) {
        if (s.size() != 8) return false;
        for (char c : s) if (c < '0' || c > '9') return false;
        return true;
    }

    // Парсер ролей із уніфікованими назвами
    static Role ParseRole(const std::string& pos) {
        if (pos == "Accountant")  return Role::Accountant;
        if (pos == "Manager")     return Role::Manager;
        if (pos == "Cinemacoordinator") return Role::Cinemacoordinator;
        return Role::Unknown;
    }

    static bool IsDuplicate(const std::string& pib, int idCode) {
        std::string canon = CanonPIB(pib);
        for (const auto& emp : employeeList) {
            if (emp.PIB == canon)  return true;
            if (emp.code == idCode) return true;
        }
        return false;
    }
};

// static storage definitions
std::vector<Employee> Employee::employeeList;
std::map<int, double> Employee::salaryByCode;

class Budget {
private:
    int    budgetCode;
    double ticketIncome = 0.0; // сумарний дохід (встановлюється через Income)
    double salaryExpenses = 0.0;
    double movieExpenses = 0.0;
    double ticketBudget = 0.0; // акумулятор доходів від проданих квитків (якщо використовуєш)

public:
    explicit Budget(int code) : budgetCode(code) {}

    // Встановити сумарний дохід (раніше ігнорувалося значення)
    void Income(double income) { ticketIncome = income; }

    double GetIncome()       const { return ticketIncome; }
    double GetTicketBudget() const { return ticketBudget; }
    double GetMovieExpenses()const { return movieExpenses; }
    double GetSalaryExpenses()const { return salaryExpenses; }
    int    GetCode()         const { return budgetCode; }

    void AddSalaryExpense(double v) { salaryExpenses += v; if (salaryExpenses < 0) salaryExpenses = 0; }



    // вже є: void Income(double income) { ticketIncome = income; }
    void AddIncome(double v) { ticketIncome += v; }        // + виручка до тоталу
    double GetTotalBudget() const { return ticketIncome + ticketBudget; } // тотал = базовий + квитки

    std::pair<double, double> MovieMinMax(int demandPercent) {
        double minMovieCost = 100000.0;
        double extra = (demandPercent / 5) * 50000.0;
        double maxMovieCost = minMovieCost + extra;
        return { minMovieCost, maxMovieCost };
    }

    std::pair<double, double> PriceMinMax(int demandPercent, double movieCost) const {
        constexpr double kBasePrice = 100.0; // базова ціна
        const double maxPrice = kBasePrice + (demandPercent / 10.0) + (movieCost / 1000.0);
        return { kBasePrice, maxPrice };
    }

    void RecordMovieExpenses(double v) {
        if (v > 0) movieExpenses += v;
        if (movieExpenses < 0) movieExpenses = 0;
    }
    double ClampTicketPrice(int demandPercent, double movieCost, double rawPrice) {
        auto pr = PriceMinMax(demandPercent, movieCost);
        if (rawPrice < pr.first)  rawPrice = pr.first;
        if (rawPrice > pr.second) rawPrice = pr.second;
        return rawPrice;
    }

    // Зручний рядок-репорт (без MessageBox) — можна друкувати в консоль з UI
    std::string ToString() const {
        std::ostringstream oss;
        oss << "Budget Code: " << budgetCode
            << "\nTicket Income: " << ticketIncome << " grn"
            << "\nTicket Budget: " << ticketBudget << " grn"
            << "\nSalary Expenses: " << salaryExpenses << " grn"
            << "\nMovie Expenses: " << movieExpenses << " grn";
        return oss.str();
    }
};

class Hall {
private:
    int hallCode;
    int hallNumber;
    int totalSeats;
    std::vector<bool> seats;

public:
    Hall(int code, int number, std::string /*type*/, int seatsCount)
        : hallCode(code), hallNumber(number), totalSeats(seatsCount), seats(seatsCount, true) {
    }

    // Accessors
    int  GetCode()      const { return hallCode; }
    int  GetNumber()    const { return hallNumber; }
    int  GetTotalSeats()const { return totalSeats; }
    void SetHallNumber(int n) { hallNumber = n; }


    // Призначення залу на часовий слот (працює з глобальними IsHallBusy/MarkHallBusy)
    static bool IsAvailableForSlot(int slotIdx, int hallNo) {
        return !IsHallBusy(slotIdx, hallNo);
    }
    static bool AssignToSlot(int slotIdx, int hallNo, std::string& err) {
        if (!IsAvailableForSlot(slotIdx, hallNo)) {
            err = "This hall is already assigned to the selected seance.";
            return false;
        }
        MarkHallBusy(slotIdx, hallNo);
        return true;
    }
};

class Seans {
private:
    int         code_;        // унікальний код сеансу
    std::string startTime_;   // "HH:MM"
    int         durationMin_; // хвилини

    static std::vector<Seans> seansList;

public:
    // Конструктори
    Seans() : code_(-1), startTime_(""), durationMin_(0) {}
    explicit Seans(std::string time) : code_(-1), startTime_(std::move(time)), durationMin_(0) {}
    Seans(int code, std::string time, int durMin)
        : code_(code), startTime_(std::move(time)), durationMin_(durMin) {
    }

    // Діагностика у консоль (без MessageBox)
    void ShowDebugInfo() const {
        std::cout << "Seans code: " << code_
            << "\nStart: " << startTime_
            << "\nDuration: " << durationMin_ << " min\n";
    }

    // Колекція
    static void AddSeans(const Seans& s) {
        seansList.push_back(s);
        s.ShowDebugInfo();
        std::cout << "Seans added\n";
    }

    static Seans GetSeansByCode(int code) {
        for (const auto& s : seansList)
            if (s.code_ == code) return s;
        throw std::runtime_error("Seans not found.");
    }

    static void PrintSeans() {
        std::cout << "\nSeans list:\n";
        for (const auto& s : seansList)
            std::cout << "  #" << s.code_ << " | " << s.startTime_
            << " | " << s.durationMin_ << " min\n";
    }

    // Accessors
    const std::string& GetStartTime() const { return startTime_; }
    int  GetCode()       const { return code_; }
    int  GetDurationMin()const { return durationMin_; }

    // Рекомендації слотів (як було)
    enum class Slot { Morning, Afternoon, Evening };

    static Slot RecommendSlotByDemand(int demand /*0..100*/) {
        if (demand >= 70) return Slot::Evening;   // 18-22
        if (demand >= 40) return Slot::Afternoon; // 14-17
        return Slot::Morning;                     // 10-13
    }

    static const char* SlotUiText(Slot s) {
        switch (s) {
        case Slot::Morning:   return "10-13";
        case Slot::Afternoon: return "14-17";
        default:              return "18-22";
        }
    }

    static int SlotIndex(Slot s) {
        switch (s) {
        case Slot::Morning:   return 0;
        case Slot::Afternoon: return 1;
        default:              return 2;
        }
    }

    // Зручні хелпери за індексом
    static Slot        SlotFromIndex(int idx) { return (idx == 0) ? Slot::Morning : (idx == 1) ? Slot::Afternoon : Slot::Evening; }
    static const char* SlotUiTextByIndex(int idx) { return SlotUiText(SlotFromIndex(idx)); }
};

// ініціалізація статичного списку
std::vector<Seans> Seans::seansList;

class Movie {
private:
    static int nextCode;      // автоінкремент коду
    int movieCode;
    std::string title;
    std::string genre;
    int ageLimit;
    int duration;             // хвилини
    int demand;               // 0..100
    double priceUAH;          // вартість закупівлі

    std::string releaseDate_; // "yyyy-MM-dd"
    std::string author_;      // режисер/автор

private:
    // Per-movie approvals (no maps needed)
    double approvedTicketPrice_ = -1.0; // -1 => not approved yet
    int    approvedSeansIdx_ = -1;   // 0..2, -1 => not assigned
    int    approvedHallNo_ = -1;   // 1..N, -1 => not assigned

public:
    // Accessors
    bool   HasApprovedTicketPrice() const { return approvedTicketPrice_ > 0.0; }
    double GetApprovedTicketPrice() const { return approvedTicketPrice_; }
    void   SetApprovedTicketPrice(double p) { approvedTicketPrice_ = (p > 0.0 ? p : -1.0); }

    const std::string& GetReleaseDate() const { return releaseDate_; }
    const std::string& GetAuthor() const { return author_; }

    bool   HasManagerAssignment() const { return approvedSeansIdx_ >= 0 && approvedHallNo_ > 0; }
    int    GetApprovedSeansIdx() const { return approvedSeansIdx_; }
    int    GetApprovedHallNo()  const { return approvedHallNo_; }
    void   SetManagerAssignment(int seansIdx, int hallNo) {
        approvedSeansIdx_ = (seansIdx >= 0 ? seansIdx : -1);
        approvedHallNo_ = (hallNo > 0 ? hallNo : -1);
    }

    // ─── Lookup (const / mutable) ─────────────────────────────
    static const Movie& FindMovie(int code) {
        for (const auto& m : movieList)
            if (m.GetCode() == code) return m;
        throw std::runtime_error("Movie not found.");
    }

    static Movie* FindMovieMutable(int code) {
        for (auto& m : movieList)
            if (m.GetCode() == code) return &m;
        return nullptr;
    }

    Movie()
        : movieCode(-1), title(""), genre(""),
        ageLimit(0), duration(0), demand(0), priceUAH(0.0),
        releaseDate_(""), author_("") {
    }  

    Movie(std::string t, std::string g, int age, int dur,
        std::string date, int d, double price, std::string author)
        : movieCode(nextCode++), title(std::move(t)), genre(std::move(g)),
        ageLimit(age), duration(dur), demand(d), priceUAH(price),
        releaseDate_(std::move(date)), author_(std::move(author)) {
    }  

    void SetTitle(const std::string& t) { title = t; }
    void SetGenre(const std::string& g) { genre = g; }
    void SetAgeLimit(int age) { ageLimit = age; }
    void SetDuration(int dur) { duration = dur; }
    void SetDemand(int d) { demand = (d < 0) ? 0 : (d > 100 ? 100 : d); }
    void SetPriceUAH(double p) { priceUAH = (p < 0) ? 0.0 : p; }

    // пакетне застосування правок
    void ApplyEdits(const std::string& t,
        const std::string& g,
        int age_, int dur_,
        int demand_, double price_) {
        SetTitle(t);
        SetGenre(g);
        SetAgeLimit(age_);
        SetDuration(dur_);
        SetDemand(demand_);
        SetPriceUAH(price_);
    }
    void ApplyEdits(const ContractDraft& d) {
        ApplyEdits(d.title, d.genre, d.age, d.duration, d.demand, d.priceUAH);
    }

    // ─── Accessors ────────────────────────────────────────────
    int GetCode() const { return movieCode; }
    const std::string& GetTitle() const { return title; }
    const std::string& GetGenre() const { return genre; }
    int GetAgeLimit() const { return ageLimit; }
    int GetDuration() const { return duration; }
    int GetDemand() const { return demand; }
    double GetPriceUAH() const { return priceUAH; }

    // ─── Storage / listing ───────────────────────────────────
    static void AddMovie(const Movie& m) { movieList.push_back(m); }

    void Print() const {
        std::cout << " " << title << " (" << genre << ", " << ageLimit << "+)\n"
            << "Duration: " << duration
            << "Demand: " << demand
            << "Price: " << priceUAH << " грн\n";
    }

    // глобальне сховище (простий варіант)
    static std::vector<Movie> movieList;
};

// Ініціалізація статиків
int Movie::nextCode = 1;
std::vector<Movie> Movie::movieList;


class Ticket {
private:
    int ticketCode;
    std::string date;
    std::string time;
    double price;

    int hallNo = 0;   // NEW
    int seatNo = 0;   // NEW

    // movieCode → tickets
    static std::map<int, std::vector<Ticket>> movieTickets;

public:
    Ticket(int code, const std::string& d, const std::string& t, double p)
        : ticketCode(code), date(d), time(t), price(p) {
    }

    // NEW: extended ctor with hall/seat
    Ticket(int code, const std::string& d, const std::string& t, double p, int hall, int seat)
        : ticketCode(code), date(d), time(t), price(p), hallNo(hall), seatNo(seat) {
    }

    // Accessors
    double GetPrice() const { return price; }
    void SetPrice(double p) { price = p; }

    int GetCode() const { return ticketCode; }
    const std::string& GetDate() const { return date; }
    const std::string& GetTime() const { return time; }

    // NEW getters
    int GetHall() const { return hallNo; }
    int GetSeat() const { return seatNo; }

    // Storage
    static void SaveTicket(int movieCode, const Ticket& ticket) {
        using System::Windows::Forms::MessageBox;
        using System::String;

        movieTickets[movieCode].push_back(ticket);
        MessageBox::Show(
            String::Format(
                "Ticket saved\nPrice: {0:F2} UAH\nHall #{1}, Seat {2}\n{3} {4}",
                ticket.GetPrice(),
                ticket.GetHall(),
                ticket.GetSeat(),
                gcnew String(ticket.GetDate().c_str()),
                gcnew String(ticket.GetTime().c_str())
            ),
            "Ticket"
        );
    }



    static int CountTicket(int movieCode) {
        return static_cast<int>(movieTickets[movieCode].size());
    }



    // NEW: full helper with hall/seat
    static Ticket IssueNextAndSaveDetailed(int movieCode,
        const std::string& date,
        const std::string& time,
        double price,
        int hallNo,
        int seatNo) {
        int nextTicketCode = CountTicket(movieCode) + 1;
        Ticket t(nextTicketCode, date, time, price, hallNo, seatNo);
        SaveTicket(movieCode, t);
        return t;
    }

    // Debug / Output
    void Print() const {
        std::cout << "Ticket #" << ticketCode
            << " | " << date << " " << time
            << " | Hall " << hallNo << ", Seat " << seatNo
            << " | " << price << " грн\n";
    }
};

// ПІСЛЯ оголошення класу Ticket, у файлі .cpp:
std::map<int, std::vector<Ticket>> Ticket::movieTickets;

void Project12::MyForm::PrefillContractFromMovie(const Movie& m)
{
    // std::string -> System::String^ коректно:
    Mt_text->Text = marshal_as<System::String^>(m.GetTitle());
    Genre_text->Text = marshal_as<System::String^>(m.GetGenre());
    AgeR_text->Text = System::Convert::ToString(m.GetAgeLimit());
    Durm_text->Text = System::Convert::ToString(m.GetDuration());
    Demand_text->Text = System::Convert::ToString(m.GetDemand());
    Price_text->Text = System::Convert::ToString(m.GetPriceUAH());
    // Startd_text / Endd_text координатор заповнює вручну



}

System::Void Project12::MyForm::OnMovieCardClick(System::Object^ sender, System::EventArgs^ e)
{
    auto card = safe_cast<System::Windows::Forms::Panel^>(sender);
    if (card == nullptr) return;

    int code = safe_cast<int>(card->Tag);
    const Movie& m = Movie::FindMovie(code);

    if (!m.HasManagerAssignment())
    {
        MessageBox::Show("This movie is not yet assigned by the manager.", "Not available");
        return;
    }

    // Код фільму збережений у Tag картки
    selectedMovieCode = code;

    if (this->Author_L) {
        this->Author_L->Text = gcnew System::String(m.GetAuthor().c_str());
        this->Author_L->Visible = true; // ← додати
    }
    if (this->RelD_L) {
        this->RelD_L->Text = gcnew System::String(m.GetReleaseDate().c_str());
        this->RelD_L->Visible = true;   // ← додати
    }
    if (this->Auth_LL)   this->Auth_LL->Visible = true;

    AcceprM_B->Visible = true;
    if (this->RelDate_l) this->RelDate_l->Visible = true;
}


class Contract {
private:
    static int nextContractCode;

    int contractCode;
    std::string signDate;     // YYYY-MM-DD
    std::string expiryDate;   // YYYY-MM-DD
    int movieCode;  

    static std::vector<Contract> contractList;


   

public:
    Contract(std::string sign, std::string expiry, int movieCode_, int tickets)
        : contractCode(nextContractCode++),
        signDate(std::move(sign)),
        expiryDate(std::move(expiry)),
        movieCode(movieCode_) {
    }

    static const Contract* FindByCode(int code) {
        for (const auto& c : contractList) {
            if (c.contractCode == code) return &c;
        }
        return nullptr;
    }

    // ЗАМІНІТЬ Exists на суворішу:
    static bool Exists(int movieCode, const std::string& sign, const std::string& exp) {
        for (const auto& c : contractList) {
            if (c.GetSignDate() == sign && c.GetExpiryDate() == exp) {
                if (c.GetMovieCode() == movieCode) return true; // старе правило
                // нове: повний збіг за атрибутами фільму
                const Movie& mNew = Movie::FindMovie(movieCode);
                const Movie& mOld = c.GetMovie();
                bool sameMovie = (mNew.GetTitle() == mOld.GetTitle() &&
                    mNew.GetGenre() == mOld.GetGenre() &&
                    mNew.GetAgeLimit() == mOld.GetAgeLimit() &&
                    mNew.GetDuration() == mOld.GetDuration() &&
                    std::llround(mNew.GetPriceUAH() * 100) == std::llround(mOld.GetPriceUAH() * 100));
                if (sameMovie) return true;
            }
        }
        return false;
    }

    const Movie& GetMovie() const { return Movie::FindMovie(movieCode); }

    static const std::vector<Contract>& GetContractList() { return contractList; }

    int GetCode() const { return contractCode; }
    const std::string& GetSignDate() const { return signDate; }
    const std::string& GetExpiryDate() const { return expiryDate; }
    int GetMovieCode() const { return movieCode; }

    static bool CreateIfNotExists(int movieCode,
        const std::string& sign,
        const std::string& exp,
        Contract*& createdOut)
    {
        if (Exists(movieCode, sign, exp)) {
            createdOut = nullptr;
            return false;
        }
        contractList.emplace_back(sign, exp, movieCode, 0);
        createdOut = &contractList.back();
        return true;
    }

    
};

// one-time definitions (keep them near other statics)
int Contract::nextContractCode = 1;
std::vector<Contract> Contract::contractList;

class Distributor {
private:
    int distributorCode;
    string fullName;
    string phoneNumber;
    bool hasLicense;
    vector<Movie> movieList;
    std::string contactPIB;

public:
    Distributor(int code, string name, string phone, bool license)
        : distributorCode(code), fullName(name), phoneNumber(phone), hasLicense(license) {
        InitializeMovies();
    }

    void InitializeMovies() {
        movieList = {
            Movie("The Mirror", "Art-house", 12, 106, "1975-03-07", 60, 32000.00, "Andrei Tarkovsky"),
            Movie("Stalker", "Sci-Fi", 14, 162, "1979-05-20", 55, 35000.00, "Andrei Tarkovsky"),
            Movie("Solaris", "Sci-Fi", 14, 167, "1972-11-20", 45, 30000.00, "Andrei Tarkovsky"),
            Movie("Ivan's Childhood", "War Drama", 12, 95, "1962-09-10", 30, 25000.00, "Andrei Tarkovsky"),
            Movie("Andrei Rublev", "Historical Drama", 16, 205, "1966-12-16", 35, 40000.00, "Andrei Tarkovsky"),
            Movie("Days of Heaven", "Romantic Drama", 12, 94, "1978-09-13", 50, 28000.00, "Terrence Malick"),
            Movie("The Thin Red Line", "War Drama", 16, 170, "1998-12-25", 40, 33000.00, "Terrence Malick"),
            Movie("The New World", "Historical Drama", 12, 135, "2005-12-25", 35, 31000.00, "Terrence Malick"),
            Movie("Tree of Life", "Philosophical Drama", 10, 139, "2011-05-27", 85, 100000.00, "Terrence Malick"),
            Movie("Wild Strawberries", "Philosophical Drama", 10, 91, "1957-12-26", 70, 27000.00, "Ingmar Bergman")
        };
    }


    vector<Movie> GetMoviesWithinBudget(double maxPrice) const {
        vector<Movie> result;
        for (const auto& m : movieList) {
            if (m.GetPriceUAH() <= maxPrice) {
                result.push_back(m);
            }
        }
        return result;
    }


private:
    Movie FindMovie(int code) const {
        for (const auto& m : movieList) {
            if (m.GetCode() == code) return m;
        }
        throw runtime_error("Movie not found in distributor list.");
    }

};

// --- Forward declarations of types used by role classes (щоб не було циклів)
struct ContractDraft;
struct AssignmentResult;
class Movie;
class Contract;
class Hall;
class Seans;
class Budget;

// --- Standalone role classes (раніше були підкласами Position)

// Coordinator: validation + apply + save
class Cinemacoordinator {
public:
    static bool Validate(const ContractDraft& d, std::string& err) {
        if (d.movieCode < 0) { err = "Movie must be selected."; return false; }
        if (d.title.empty() || d.genre.empty()) { err = "Title/Genre are required."; return false; }
        if (d.age < 0 || d.duration <= 0) { err = "Age/duration invalid."; return false; }
        if (d.demand < 0 || d.demand > 100) { err = "Demand must be 0..100."; return false; }
        if (d.priceUAH < 0.0) { err = "Price must be non-negative."; return false; }
        if (d.signDate.empty() || d.expiryDate.empty()) { err = "Dates required."; return false; }
        return true;
    }

    static bool ApplyMovieEdits(const ContractDraft& d, std::string& err) {
        Movie* m = Movie::FindMovieMutable(d.movieCode);
        if (!m) { err = "Selected movie not found."; return false; }
        m->ApplyEdits(d);
        return true;
    }

    static bool SaveContract(const ContractDraft& d, Contract*& created, std::string& err) {
        if (Contract::Exists(d.movieCode, d.signDate, d.expiryDate)) {
            err = "This contract already exists (same movie and dates).";
            created = nullptr;
            return false;
        }
        bool ok = Contract::CreateIfNotExists(d.movieCode, d.signDate, d.expiryDate, created);
        if (!ok) { err = "Contract create failed."; return false; }
        return true;
    }
};

// Accountant: budget helpers
class Accountant {
public:
    static void RecordExpenses(Budget& b, double movieExpenses) {
        b.RecordMovieExpenses(movieExpenses);
    }

    static double CalcClampedTicketPrice(Budget& b,
        int demandPercent,
        double movieCost,
        double rawPrice) {
        return b.ClampTicketPrice(demandPercent, movieCost, rawPrice);
    }
};

// Manager: assign hall to seance
class Manager {
public:
    static bool AssignHallToSeans(int slotIndex,
        const std::string& hallText,
        AssignmentResult& out)
    {
        if (slotIndex < 0) {
            out = { false, "Please select a seance (time slot).", "", 0 };
            return false;
        }

        int hallNo = 0;
        try { hallNo = std::stoi(hallText); }
        catch (...) {
            out = { false, "Invalid hall number.", "", 0 };
            return false;
        }

        std::string err;
        if (!Hall::AssignToSlot(slotIndex, hallNo, err)) {
            out = { false, err, "", 0 };
            return false;
        }

        out.ok = true;
        out.error.clear();
        out.hallNo = hallNo;
        out.slotText = Seans::SlotUiTextByIndex(slotIndex);
        return true;
    }
};

// --- Slim Position class with role aliases (залишаємо API Position::Role і Parse)
class Position {
private:
    int positionCode;
    std::string positionName;

public:
    enum class Role { Accountant, Manager, Cinemacoordinator, Unknown };

    Position(int code, std::string name)
        : positionCode(code), positionName(std::move(name)) {
    }

    int GetCode() const { return positionCode; }
    const std::string& GetName() const { return positionName; }

    static Role Parse(const std::string& name) {
        if (name == "Accountant")         return Role::Accountant;
        if (name == "Manager")           return Role::Manager;
        if (name == "Cinemacoordinator") return Role::Cinemacoordinator;
        return Role::Unknown;
    }

    // ВАЖЛИВО: зберігаємо старі імена Position::Cinemacoordinator / Accountant / Manager
    // через type-alias'и, щоб не міняти існуючі виклики в коді:
    using Cinemacoordinator = ::Cinemacoordinator;
    using Accountant = ::Accountant;
    using Manager = ::Manager;
};

System::Void Project12::MyForm::MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
    moviesFlow->Controls->Clear();
    moviesFlow->Visible = false;
    budget = new Budget(101);
    budget->Income(250000);

    this->Hall_cb->SelectedIndexChanged += gcnew System::EventHandler(this, &Project12::MyForm::Hall_cb_SelectedIndexChanged);

    this->contracts_cb->SelectedIndexChanged += gcnew System::EventHandler(this, &Project12::MyForm::ApplyContract_ClickShim);


    this->AcceprM_B->Click += gcnew System::EventHandler(this, &Project12::MyForm::OnAcceptMovie);

    ad_cb->AutoCheck = false; // забороняє кліки мишкою змінювати стан
    ad_cb->Enabled = false; 
    ad_cb->TabStop = false;

    this->T_text->Leave += gcnew System::EventHandler(this, &Project12::MyForm::T_text_Leave);
    this->MB_text->Leave += gcnew System::EventHandler(this, &Project12::MyForm::MB_text_Leave);

    this->Demand_text->Leave += gcnew System::EventHandler(this, &Project12::MyForm::T_text_Leave);

    this->Demand_text->TextChanged += gcnew System::EventHandler(this, &Project12::MyForm::T_text_Leave);

    this->Emp_CB->SelectedIndexChanged += gcnew System::EventHandler(this, &Project12::MyForm::Emp_CB_SelectedIndexChanged);

    this->Sal_text->Leave += gcnew System::EventHandler(this, &Project12::MyForm::Sal_text_Leave);

    contracts_cb->DropDownStyle = ComboBoxStyle::DropDownList;
    contracts_cb->DisplayMember = "Value";
    contracts_cb->ValueMember = "Key";
    // (позиції/розміри вже задані дизайнером — лишаємо)

    static bool _applyHandlerWired = false;
    if (!_applyHandlerWired) {
        applyContract_b->Click += gcnew System::EventHandler(this, &Project12::MyForm::ApplyContract_ClickShim);
        _applyHandlerWired = true;
    }


    PopulateContractsCombo();

    contracts_cb->Visible = false;
    applyContract_b->Visible = false;

    // формат DateT
    DateT->Format = DateTimePickerFormat::Custom;
    DateT->CustomFormat = "yyyy-MM-dd";
    DateT->Enabled = false;

    // оновлювати межі при зміні дат
    this->Startd_picker->ValueChanged += gcnew System::EventHandler(this, &Project12::MyForm::StartOrEndChanged_UpdateDateT);
    this->Endd_picker->ValueChanged += gcnew System::EventHandler(this, &Project12::MyForm::StartOrEndChanged_UpdateDateT);
    UpdateDateTBounds();

    if (cpl_cb) {
        cpl_cb->BeginUpdate();
        cpl_cb->Items->Clear();
        for (int seat = 1; seat <= 30; ++seat) {
            cpl_cb->Items->Add(System::Convert::ToString(seat));
        }
        cpl_cb->SelectedIndex = 0; // вибрати перше місце
        cpl_cb->EndUpdate();
    }
}



System::Windows::Forms::Panel^ Project12::MyForm::MakeMovieCard(const Movie& m) {
    using namespace System::Drawing;

    Panel^ card = gcnew Panel();
    card->BorderStyle = BorderStyle::FixedSingle;
    card->BackColor = Color::White;
    card->Size = System::Drawing::Size(300, 320); // трішки вище
    card->Margin = Windows::Forms::Padding(8);

    Label^ title = gcnew Label();
    title->AutoSize = false;
    title->Text = gcnew System::String(m.GetTitle().c_str());
    title->Font = gcnew System::Drawing::Font("Segoe UI", 10, System::Drawing::FontStyle::Bold);
    title->Location = Point(8, 8);
    title->Size = System::Drawing::Size(260, 22);

    Label^ genre = gcnew Label();
    genre->AutoSize = false;
    genre->Text = "Genre: " + gcnew System::String(m.GetGenre().c_str());
    genre->Location = Point(8, 38);
    genre->Size = System::Drawing::Size(260, 25);

    Label^ age = gcnew Label();
    age->AutoSize = false;
    age->Text = "Age: " + System::Convert::ToString(m.GetAgeLimit());
    age->Location = Point(8, 58);
    age->Size = System::Drawing::Size(260, 25);

    Label^ demand = gcnew Label();
    demand->AutoSize = false;
    demand->Text = "Demand: " + System::Convert::ToString(m.GetDemand());
    demand->Location = Point(8, 78);
    demand->Size = System::Drawing::Size(260, 25);

    Label^ duration = gcnew Label();
    duration->AutoSize = false;
    duration->Text = "Duration: " + System::Convert::ToString(m.GetDuration()) + " min";
    duration->Location = Point(8, 98);
    duration->Size = System::Drawing::Size(260, 25);

    // Author
    Label^ author = gcnew Label();
    author->AutoSize = false;
    author->MaximumSize = System::Drawing::Size(260, 0);
    author->AutoEllipsis = true;
    author->Text = "Author: " + gcnew System::String(m.GetAuthor().c_str());
    author->Location = Point(8, 118);             // <-- 118
    author->Size = System::Drawing::Size(260, 25);

    // Release date — інше Y, щоб не перекривалось
    Label^ releaseDate = gcnew Label();
    releaseDate->AutoSize = false;
    releaseDate->Text = "Release date: " + gcnew System::String(m.GetReleaseDate().c_str());
    releaseDate->Location = Point(8, 140);        // <-- 140 (було 118)
    releaseDate->Size = System::Drawing::Size(260, 20);

    Label^ price = gcnew Label();
    price->AutoSize = false;
    price->Text = System::String::Format("Price: {0:F0} UAH", m.GetPriceUAH());
    price->Location = Point(8, 165);
    price->Size = System::Drawing::Size(260, 20);

    Button^ accept = gcnew Button();
    accept->Text = "Accept";
    accept->Location = Point(8, 195);
    accept->Size = System::Drawing::Size(120, 30);
    accept->Tag = m.GetCode();
    accept->Click += gcnew System::EventHandler(this, &MyForm::OnAcceptMovie);

    card->Controls->Add(title);
    card->Controls->Add(genre);
    card->Controls->Add(age);
    card->Controls->Add(demand);
    card->Controls->Add(duration);
    card->Controls->Add(author);
    card->Controls->Add(releaseDate);
    card->Controls->Add(price);
    card->Controls->Add(accept);

    card->Tag = m.GetCode();
    card->Click += gcnew System::EventHandler(this, &Project12::MyForm::OnMovieCardClick);
    return card;
}

System::Void Project12::MyForm::StartOrEndChanged_UpdateDateT(System::Object^ sender, System::EventArgs^ e)
{
    UpdateDateTBounds();
}



void Project12::MyForm::ResetContractUI() {
    // 1) скинути вибір фільму та поля
    selectedMovieCode = -1;
    if (Mt_text)      Mt_text->Clear();
    if (Genre_text)   Genre_text->Clear();
    if (AgeR_text)    AgeR_text->Clear();
    if (Durm_text)    Durm_text->Clear();
    if (Demand_text)  Demand_text->Clear();
    if (Price_text)   Price_text->Clear();
    if (Startd_picker) Startd_picker->Value = DateTime::Today;
    if (Endd_picker)   Endd_picker->Value = DateTime::Today;

    // 2) галочки
    if (ac_cb) ac_cb->Checked = false;
    if (ad_cb) ad_cb->Checked = false; // індикатор: дистриб’ютор ще не підтвердив

    // 3) приховати панель карток і все, що поруч
    if (moviesFlow) moviesFlow->Visible = false;
    if (BM_l)       BM_l->Visible = false;
    if (reqm_b)     reqm_b->Visible = false;

    if (AccC_B) AccC_B->Enabled = true;

    if (MB_text) MB_text->Clear();
}


System::Void Project12::MyForm::OnAcceptMovie(System::Object^ sender, System::EventArgs^ e)
{
    int code = -1;

    if (auto btn = dynamic_cast<System::Windows::Forms::Button^>(sender)) {
        if (btn->Tag != nullptr) code = safe_cast<int>(btn->Tag);
        else if (btn->Parent && btn->Parent->Tag != nullptr) code = safe_cast<int>(btn->Parent->Tag);
    } else if (auto card = dynamic_cast<System::Windows::Forms::Panel^>(sender)) {
        if (card->Tag != nullptr) code = safe_cast<int>(card->Tag);
    }

    if (code == -1) { MessageBox::Show("Movie code not found."); return; }
    selectedMovieCode = code;

    try {
        auto m = Movie::FindMovie(selectedMovieCode);
        PrefillContractFromMovie(m);
        UpdatePreferredSeansAndHall();
    } catch (...) {
        MessageBox::Show("Selected movie not found."); return;
    }

    if (moviesFlow) moviesFlow->Visible = false;
    if (BM_l) BM_l->Visible = false;
    if (reqm_b) reqm_b->Visible = false;
    if (AccC_B) AccC_B->Enabled = true;
    ShowPage(4);
}
System::Void Project12::MyForm::UpdateEmployeeComboBox() {
    Emp_CB->Items->Clear();

    const std::vector<Employee>& list = Employee::GetEmployeeList();
    for (size_t i = 0; i < list.size(); ++i) {
        const Employee& emp = list[i];
        System::String^ item = marshal_as<System::String^>(emp.GetDisplayText());
        Emp_CB->Items->Add(item);
    }
}

System::Void Project12::MyForm::b_Emply_Click(System::Object^ sender, System::EventArgs^ e)
{
    System::String^ posInput = (comboBox1->SelectedItem != nullptr)
        ? comboBox1->SelectedItem->ToString()
        : "";

    std::string pib = msclr::interop::marshal_as<std::string>(textBox1->Text);
    std::string id = msclr::interop::marshal_as<std::string>(textBox2->Text);
    std::string pos = msclr::interop::marshal_as<std::string>(posInput);

    if (pos.empty()) {
        System::Windows::Forms::MessageBox::Show(
            "Please select a position.", "Validation");
        return;
    }
    {
        std::string authErr;
        Employee::Role authRole;

        if (Employee::TryAuthenticate(pib, id, pos, authRole, authErr))
        {
            try { _activeEmployeeCode = std::stoi(id); }
            catch (...) { _activeEmployeeCode = -1; }

            UpdateEmployeeComboBox();
            if (Emp_CB && Emp_CB->Items->Count > 0 && Emp_CB->SelectedIndex < 0)
                Emp_CB->SelectedIndex = 0;

            UpdateEmployMinMaxLabel();

            switch (authRole)
            {
            case Employee::Role::Accountant:
            {
                ShowPage(3);
                double income = budget ? budget->GetIncome() : 0.0;
                Bud_label->Text = System::String::Format("Total budget: {0:F0} grn", income);
                Bud_label->Visible = true;
                UpdateBudgetLabelsFromPercents();
                return;
            }
            case Employee::Role::Manager:
                ShowPage(5);
                return;

            case Employee::Role::Cinemacoordinator:
                ShowPage(6);
                Bud_label->Visible = false;
                BM_l->Visible = false;
                return;

            default:
                break;
            }
        }
        else
        {
            if (authErr.find("partially match") != std::string::npos)
            {
                System::Windows::Forms::MessageBox::Show(
                    msclr::interop::marshal_as<System::String^>(authErr), "Login");
                return;
            }

            if (!authErr.empty() &&
                authErr != "Employee not found. Please register.")
            {
                System::Windows::Forms::MessageBox::Show(
                    msclr::interop::marshal_as<System::String^>(authErr), "Login");
                return;
            }
        }
    }
    {
        std::string err;
        Employee::Role role;
        bool ok = Employee::Register(pib, id, pos, err, role);

        if (!ok) {
            System::Windows::Forms::MessageBox::Show(
                msclr::interop::marshal_as<System::String^>(err),
                "Registration"
            );
            return;
        }

        UpdateEmployeeComboBox();

        try { _activeEmployeeCode = std::stoi(id); }
        catch (...) { _activeEmployeeCode = -1; }

        if (Emp_CB && Emp_CB->Items->Count > 0 && Emp_CB->SelectedIndex < 0)
            Emp_CB->SelectedIndex = 0;

        UpdateEmployMinMaxLabel();

        switch (role) {
        case Employee::Role::Accountant:
        {
            ShowPage(3);
            double income = budget ? budget->GetIncome() : 0.0;
            Bud_label->Text = System::String::Format("Total budget: {0:F0} grn", income);
            Bud_label->Visible = true;
            UpdateBudgetLabelsFromPercents();
            break;
        }
        case Employee::Role::Manager:
            ShowPage(5);
            break;

        case Employee::Role::Cinemacoordinator:
            ShowPage(6);
            Bud_label->Visible = false;
            BM_l->Visible = false;
            break;

        default:
            ShowPage(1);
            break;
        }
    }
}

System::Void Project12::MyForm::reqm_b_Click(System::Object^ sender, System::EventArgs^ e) {
    int demandVal = TryParseIntOrDefault(Demand_text,
        (selectedMovieCode >= 0 ? Movie::FindMovie(selectedMovieCode).GetDemand() : 50));
    auto range = budget->MovieMinMax(demandVal);

    // NEW: якщо ще не ініціалізовано – підтягуємо з бюджету один раз
    if (remainingMovieBudget < 0) {
        remainingMovieBudget = this->budget ? this->budget->GetMovieExpenses() : 0.0;
    }

    if (remainingMovieBudget <= 0.0) {
        MessageBox::Show("Enter the movie budget approved by the accountant first.");
        return;
    }

    ShowMoviesForBudget(remainingMovieBudget);   // тут лейбл BM_l показує саме залишок
}

System::Void Project12::MyForm::ac_cb_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
{
    if (!ad_cb->Checked && ac_cb->Checked)
    {
        ac_cb->Checked = false; 
        MessageBox::Show("Please wait for the distributor's confirmation first.");
    }
}

System::Void Project12::MyForm::OnDistributorReply(System::Object^ sender, System::EventArgs^ e)
{
    auto t = dynamic_cast<System::Windows::Forms::Timer^>(sender);
    if (t) {
        int tagId = 0;
        try { tagId = safe_cast<int>(t->Tag); }
        catch (...) { tagId = -1; }
        if (tagId != _distribReqId) {
            t->Stop();
            t->Tick -= gcnew System::EventHandler(this, &Project12::MyForm::OnDistributorReply);
            delete t;
            return;
        }
    }

    if (_distribTimer) {
        try {
            _distribTimer->Stop();
            _distribTimer->Tick -= gcnew System::EventHandler(this, &Project12::MyForm::OnDistributorReply);
        }
        catch (System::Exception^ ex) {
            System::Diagnostics::Debug::WriteLine(System::String::Format("OnDistributorReply cleanup error: {0}", ex->Message));
        }
        catch (const std::exception& ex) {
            System::Diagnostics::Debug::WriteLine(gcnew System::String(ex.what()));
        }
        catch (...) {
            System::Diagnostics::Debug::WriteLine("OnDistributorReply cleanup: unknown error");
        }
    }

    // Якщо вже підтверджено — вихід
    if (ad_cb->Checked) {
        _awaitingDistrib = false;
        AccC_B->Enabled = true;
        return;
    }

    int movieCode = selectedMovieCode;
    if (movieCode >= 0 && Startd_picker && Endd_picker)
    {
        using msclr::interop::marshal_as;
        std::string sign = marshal_as<std::string>(Startd_picker->Value.ToString("yyyy-MM-dd"));
        std::string exp = marshal_as<std::string>(Endd_picker->Value.ToString("yyyy-MM-dd"));

        
        if (Contract::Exists(movieCode, sign, exp)) {
            _awaitingDistrib = false;
            AccC_B->Enabled = true;
            System::Windows::Forms::MessageBox::Show(
                "Distributor refused: duplicated contract (same dates and movie attributes).",
                "Distributor"
            );
            return;
        }
    }

    // Якщо все ОК — ставимо галочку
    ad_cb->Checked = true;
    AccC_B->Enabled = true;
    _awaitingDistrib = false;
    MessageBox::Show("The distributor has confirmed the contract. Now check the coordinator's box and click Accept again.");
}


bool Project12::MyForm::ValidateContractUI()
{
    cli::array<Control^>^ required = gcnew cli::array<Control^>{
        Mt_text, Genre_text, AgeR_text, Durm_text, Demand_text, Price_text
    };
    for each (Control ^ c in required) {
        if (!c || c->Text->Trim()->Length == 0) {
            MessageBox::Show("Please fill in all contract fields.");
            return false;
        }
    }
    if (Startd_picker->Value > Endd_picker->Value) {
        MessageBox::Show("End date must be after start date.");
        return false;
    }
    if (selectedMovieCode == -1) {
        MessageBox::Show("Please select a movie first.");
        return false;
    }
    return true;
}

ContractDraft Project12::MyForm::BuildContractDraft()
{
    using msclr::interop::marshal_as;
    ContractDraft d;
    d.movieCode = selectedMovieCode;
    try {
        d.title = marshal_as<std::string>(Mt_text->Text);
        d.genre = marshal_as<std::string>(Genre_text->Text);
        d.age = System::Int32::Parse(AgeR_text->Text);
        d.duration = System::Int32::Parse(Durm_text->Text);
        d.demand = TryParseIntOrDefault(Demand_text, 50);
        d.priceUAH = TryParseDoubleOrDefault(Price_text, 0.0);
        d.signDate = marshal_as<std::string>(Startd_picker->Value.ToString("yyyy-MM-dd"));
        d.expiryDate = marshal_as<std::string>(Endd_picker->Value.ToString("yyyy-MM-dd"));
    }
    catch (System::Exception^ ex) {
        MessageBox::Show(System::String::Format("Invalid contract fields: {0}", ex->Message), "Input");
    }
    catch (const std::exception& ex) {
        MessageBox::Show(gcnew System::String(ex.what()), "Input");
    }
    catch (...) {
        MessageBox::Show("Invalid contract fields.", "Input");
    }
    return d;
}

void Project12::MyForm::EnsureDistribTimerCleared()
{
    if (_distribTimer) {
        try {
            _distribTimer->Stop();
            _distribTimer->Tick -= gcnew System::EventHandler(this, &Project12::MyForm::OnDistributorReply);
        }
        catch (System::Exception^ ex) {
            System::Diagnostics::Debug::WriteLine(System::String::Format("EnsureDistribTimerCleared: {0}", ex->Message));
        }
        catch (const std::exception& ex) {
            System::Diagnostics::Debug::WriteLine(gcnew System::String(ex.what()));
        }
        catch (...) {
            System::Diagnostics::Debug::WriteLine("EnsureDistribTimerCleared: unknown error");
        }
        delete _distribTimer;
        _distribTimer = nullptr;
    }
}

void Project12::MyForm::StartDistributorRequest(const ContractDraft& /*d*/)
{
    // повідомлення користувачу + захист від дублювання
    MessageBox::Show("The contract has been sent to the distributor. Waiting for automatic confirmation…");
    if (AccC_B) AccC_B->Enabled = false;

    // чистимо можливий старий таймер
    EnsureDistribTimerCleared();

    // позначаємо активний запит
    _awaitingDistrib = true;
    _distribReqId++;

    // заводимо таймер
    _distribTimer = gcnew System::Windows::Forms::Timer();
    _distribTimer->Interval = 1200; // ms
    _distribTimer->Tag = _distribReqId;
    _distribTimer->Tick += gcnew System::EventHandler(this, &Project12::MyForm::OnDistributorReply);
    _distribTimer->Start();
}

bool Project12::MyForm::SaveCoordinatorContract(const ContractDraft& d, Contract*& created)
{
    using msclr::interop::marshal_as;
    std::string err;
    created = nullptr;

    // дубль-контроль і створення
    if (Contract::Exists(d.movieCode, d.signDate, d.expiryDate)) {
        err = "This contract already exists (same movie and dates).";
        MessageBox::Show(marshal_as<System::String^>(err));
        return false;
    }
    if (!Contract::CreateIfNotExists(d.movieCode, d.signDate, d.expiryDate, created) || !created) {
        MessageBox::Show("Contract create failed.");
        return false;
    }
    MessageBox::Show("Contract saved successfully.");
    return true;
}

void Project12::MyForm::AfterContractSaved(Contract* created, const ContractDraft& d)
{
    // 1) оновити комбобокс контрактів і стани UI
    if (created) {
        PopulateContractsCombo(created->GetCode());
        if (T_text)  T_text->Enabled = false;
        if (T_label) T_label->Enabled = false;
    }
    else {
        PopulateContractsCombo();
    }

    // 2) синхронізувати бюджет
    _signedDemand = d.demand;
    if (remainingMovieBudget < 0 && budget) {
        auto range = budget->MovieMinMax(60);
        remainingMovieBudget = range.second;
    }
    remainingMovieBudget -= d.priceUAH;
    if (remainingMovieBudget < 0) remainingMovieBudget = 0;

    // 3) UI переходи/підказки
    ResetContractUI();
    ShowPage(6);
    UpdatePreferredSeansAndHall();

    if (BM_l) {
        BM_l->Text = System::String::Format("Movie budget: {0:F0} UAH", remainingMovieBudget);
        BM_l->Visible = true;
    }

    UpdateBudgetHints(d);
    UpdateSummaryLabel();

    // 4) підтвердження для працівника
    if (_activeEmployeeCode >= 0) {
        Employee::IncrementConfirmations(_activeEmployeeCode);
        UpdateEmployMinMaxLabel();
    }
}

void Project12::MyForm::UpdateBudgetHints(const ContractDraft& d)
{
    using msclr::interop::marshal_as;

    if (!budget) return;
    try {
        const Movie& m2 = Movie::FindMovie(d.movieCode);
        auto priceRange = budget->PriceMinMax(m2.GetDemand(), m2.GetPriceUAH());
        if (T_label) {
            T_label->Text = System::String::Format(
                "Ticket price range: {0:F0}–{1:F0} UAH",
                priceRange.first, priceRange.second
            );
        }
    }
    catch (System::Exception^ ex) {
        System::Diagnostics::Debug::WriteLine(System::String::Format("UpdateBudgetHints: {0}", ex->Message));
    }
    catch (const std::exception& ex) {
        System::Diagnostics::Debug::WriteLine(gcnew System::String(ex.what()));
    }
    catch (...) {
        System::Diagnostics::Debug::WriteLine("UpdateBudgetHints: unknown error");
    }
}

void Project12::MyForm::UpdateSummaryLabel()
{
    using msclr::interop::marshal_as;

    const auto& all = Contract::GetContractList();
    if (all.empty() || !S_label) return;

    const Contract& last = all.back();
    const Movie& m2 = last.GetMovie();

    System::String^ titleS = marshal_as<System::String^>(m2.GetTitle());
    System::String^ genreS = marshal_as<System::String^>(m2.GetGenre());
    System::String^ signS = marshal_as<System::String^>(last.GetSignDate());
    System::String^ expiryS = marshal_as<System::String^>(last.GetExpiryDate());

    S_label->Text = System::String::Format(
        "Contract saved\n"
        "Movie: {0}\n"
        "Genre: {1}\n"
        "Age Rating: {2}+\n"
        "Duration minutes: {3}\n"
        "Start date: {4}\n"
        "End date: {5}\n"
        "Demand: {6}%\n"
        "Price: {7:F0} UAH",
        titleS, genreS,
        m2.GetAgeLimit(),
        m2.GetDuration(),
        signS, expiryS,
        m2.GetDemand(),
        m2.GetPriceUAH()
    );
    S_label->Visible = true;
}


System::Void Project12::MyForm::AccC_B_Click(System::Object^ sender, System::EventArgs^ e)
{
    using msclr::interop::marshal_as;
    if (!ValidateContractUI()) return;
    ContractDraft d = BuildContractDraft();
    {
        std::string err;
        if (!Cinemacoordinator::Validate(d, err)) {
            MessageBox::Show(marshal_as<System::String^>(err));
            return;
        }
    }
    if (!ad_cb->Checked) {
        if (_awaitingDistrib) {
            MessageBox::Show("The contract has already been sent to the distributor. Please wait for confirmation…");
            return;
        }
        StartDistributorRequest(d);
        return;
    }
    if (!ac_cb->Checked) {
        MessageBox::Show("Please check the coordinator box before saving the contract.");
        return;
    }
    {
        std::string err;
        if (!Cinemacoordinator::ApplyMovieEdits(d, err)) {
            MessageBox::Show(marshal_as<System::String^>(err));
            return;
        }
    }
    Contract* created = nullptr;
    if (!SaveCoordinatorContract(d, created)) return;
    AfterContractSaved(created, d);
}

System::Void Project12::MyForm::ApplyContract_ClickShim(System::Object^ sender, System::EventArgs^ e) {
    ApplySelectedContract();


}

void Project12::MyForm::PopulateContractsCombo() {
    if (!contracts_cb) return;
    contracts_cb->Items->Clear();
    if (!contracts_cb || contracts_cb->SelectedIndex < 0) return;
    if (T_text)  T_text->Enabled = true;
    if (T_label) T_label->Enabled = true;
    if (Seans_cb)    Seans_cb->Enabled = true;
    if (Hall_cb)     Hall_cb->Enabled = true;
    if (PrefHall_L)  PrefHall_L->Enabled = true; 
    if (PrefS_L)     PrefS_L->Enabled = true;

    auto list = Contract::GetContractList();
    for (const auto& c : list) {
        const Movie& m = c.GetMovie();

        System::String^ titleS = msclr::interop::marshal_as<System::String^>(m.GetTitle());
        System::String^ signS = msclr::interop::marshal_as<System::String^>(c.GetSignDate());
        System::String^ expiryS = msclr::interop::marshal_as<System::String^>(c.GetExpiryDate());

        System::String^ display = System::String::Format(
            "{0}. {1} [{2} - {3}]",
            c.GetCode(), titleS, signS, expiryS
        );

        auto kv = System::Collections::Generic::KeyValuePair<int, System::String^>(c.GetCode(), display);
        contracts_cb->Items->Add(kv);
    }

    if (contracts_cb->Items->Count > 0) {
        contracts_cb->SelectedIndex = 0;
    }
}

void Project12::MyForm::ApplySelectedContract() {
    
    if (!contracts_cb || contracts_cb->SelectedIndex < 0) return;

    if (T_text)  T_text->Enabled = true;
    if (T_label) T_label->Enabled = true;

    if (Seans_cb)   Seans_cb->Enabled = true;
    if (Hall_cb)    Hall_cb->Enabled = true;
    if (PrefHall_L) PrefHall_L->Enabled = true;
    if (PrefS_L)    PrefS_L->Enabled = true;

    auto kv = (System::Collections::Generic::KeyValuePair<int, System::String^>)contracts_cb->SelectedItem;
    int contractCode = kv.Key;

    const Contract* c = Contract::FindByCode(contractCode);
    if (!c) {
        System::Windows::Forms::MessageBox::Show("Contract not found.");
        return;
    }

    const Movie& m = c->GetMovie();
    selectedMovieCode = m.GetCode();
    _signedDemand = m.GetDemand(); // тепер GetEffectiveDemand() поверне цей попит

    PrefillContractFromMovie(m);
    try {
        auto signDate = msclr::interop::marshal_as<System::String^>(c->GetSignDate());
        auto expiryDate = msclr::interop::marshal_as<System::String^>(c->GetExpiryDate());

        if (!String::IsNullOrWhiteSpace(signDate))
            Startd_picker->Value = System::DateTime::Parse(signDate);

        if (!String::IsNullOrWhiteSpace(expiryDate))
            Endd_picker->Value = System::DateTime::Parse(expiryDate);
    }
    catch (System::FormatException^) {
    }

    // 3) S_label — зведення
    {
        System::String^ titleS = msclr::interop::marshal_as<System::String^>(m.GetTitle());
        System::String^ genreS = msclr::interop::marshal_as<System::String^>(m.GetGenre());
        System::String^ signS = msclr::interop::marshal_as<System::String^>(c->GetSignDate());
        System::String^ expiryS = msclr::interop::marshal_as<System::String^>(c->GetExpiryDate());

        S_label->Text = System::String::Format(
            "Contract selected\n"
            "Movie: {0}\n"
            "Genre: {1}\n"
            "Age Rating: {2}+\n"
            "Duration minutes: {3}\n"
            "Start date: {4}\n"
            "End date: {5}\n"
            "Demand: {6}%\n"
            "Price: {7:F0} UAH",
            titleS, genreS,
            m.GetAgeLimit(),
            m.GetDuration(),
            signS, expiryS,
            m.GetDemand(),
            m.GetPriceUAH()
        );
        S_label->Visible = true;
    }

    // 4) тексти для бухгалтера (ВАЖЛИВО: посилання на член-класу — через this->budget)
    {
        int demandVal = m.GetDemand();
        double priceUAH = m.GetPriceUAH();

        auto movieRange = this->budget->MovieMinMax(demandVal);
        auto priceRange = this->budget->PriceMinMax(demandVal, priceUAH);

        M_label->Text = System::String::Format("Movie budget range: {0:F0}–{1:F0} UAH", movieRange.first, movieRange.second);
        T_label->Text = System::String::Format("Ticket price range: {0:F0}–{1:F0} UAH", priceRange.first, priceRange.second);

        M_label->BringToFront();            // (опційно, щоб нічого не перекривало)
    }

    // 5) рекомендації сеансу/залів
    UpdatePreferredSeansAndHall();      // використовує _signedDemand
    RebuildHallComboForSelectedSeans(); // оновить список вільних залів для вибраного слоту
}



System::Void Project12::MyForm::ShowMoviesForBudget(double maxBudget)
{
    // 0) Спочатку перемкнути сторінку (щоб HideAllControls не зніс те, що покажемо)
    ShowPage(8);

    // 1) Стрічка бюджету
    BM_l->Text = System::String::Format("Movie budget: {0:F0} UAH", maxBudget);
    BM_l->Visible = true;
    reqm_b->Visible = true;

    // 2) Далі як і було…
    moviesFlow->Controls->Clear();
    moviesFlow->Visible = true;

    Distributor dist(1, "Sample Distributor", "123-456-789", true);
    std::vector<Movie> available = dist.GetMoviesWithinBudget(maxBudget);
    if (available.empty()) { MessageBox::Show("No movies available within the approved budget."); return; }

    for (const auto& m : available) {
        Movie::AddMovie(m);
        auto card = MakeMovieCard(m);
        moviesFlow->Controls->Add(card);
    }
}

System::Void Project12::MyForm::MB_text_Leave(System::Object^ sender, System::EventArgs^ e)
{
    double val = 0.0;
    bool ok = Double::TryParse(MB_text->Text, NumberStyles::Float, CultureInfo::CurrentCulture, val)
        || Double::TryParse(MB_text->Text, NumberStyles::Float, CultureInfo::InvariantCulture, val);
    if (!ok) {
        MB_text->Clear();
        return;
    }

    int demandVal = TryParseIntOrDefault(Demand_text, 50);
    if (selectedMovieCode >= 0) {
        try {
            auto m = Movie::FindMovie(selectedMovieCode);
            demandVal = m.GetDemand();
        }
        catch (System::Exception^ ex) {
            System::Diagnostics::Debug::WriteLine(System::String::Format("MB_text_Leave: {0}", ex->Message));
        }
        catch (const std::exception& ex) {
            System::Diagnostics::Debug::WriteLine(gcnew System::String(ex.what()));
        }
        catch (...) {
            System::Diagnostics::Debug::WriteLine("MB_text_Leave: unknown error");
        }
    }

    auto range = budget->MovieMinMax(demandVal);
    if (val < range.first) { MessageBox::Show("Value is below minimum! Setting to minimum.");  val = range.first; }
    if (val > range.second) { MessageBox::Show("Value is above maximum! Setting to maximum.");  val = range.second; }

    MB_text->Text = val.ToString();

    double total = GetAvailableTotal();
    if (val > total) {
        MessageBox::Show("Requested movie budget exceeds total budget. Clamped to available.");
        val = total;
    }
    MB_text->Text = val.ToString("F0");
}

System::Void Project12::MyForm::Sal_text_Leave(System::Object^, System::EventArgs^) {
    double val = TryParseDoubleOrDefault(Sal_text, 0.0);
    double total = GetAvailableTotal();
    if (val > total) {
        MessageBox::Show("Salary exceeds total budget. Clamped to available.");
        val = total;
    }
    Sal_text->Text = System::String::Format("{0:F0}", val);
}

System::Void Project12::MyForm::T_text_Leave(System::Object^ sender, System::EventArgs^ e)
{
    if (sender == Demand_text) {
        UpdatePreferredSeansAndHall();
        return;
    }

    if (!T_text || !T_text->Text || T_text->Text->Trim()->Length == 0) {
        UpdatePreferredSeansAndHall();
        return;
    }

    double val = 0.0;
    bool ok = Double::TryParse(T_text->Text, NumberStyles::Float, CultureInfo::CurrentCulture, val)
        || Double::TryParse(T_text->Text, NumberStyles::Float, CultureInfo::InvariantCulture, val);
    if (!ok) {
        MessageBox::Show("Invalid number format!");
        UpdatePreferredSeansAndHall();
        return;
    }

    int demandVal = TryParseIntOrDefault(Demand_text, 50);
    double movieCost = TryParseDoubleOrDefault(Price_text, 100000.0);

    bool haveMovie = false;
    if (selectedMovieCode >= 0) {
        try {
            auto m = Movie::FindMovie(selectedMovieCode);
            demandVal = m.GetDemand();
            movieCost = m.GetPriceUAH();
            haveMovie = true;
        }
        catch (System::Exception^ ex) {
            System::Diagnostics::Debug::WriteLine(System::String::Format("T_text_Leave: {0}", ex->Message));
        }
        catch (const std::exception& ex) {
            System::Diagnostics::Debug::WriteLine(gcnew System::String(ex.what()));
        }
        catch (...) {
            System::Diagnostics::Debug::WriteLine("T_text_Leave: unknown error");
        }
    }

    auto range = budget->PriceMinMax(demandVal, movieCost);
    if (val < range.first)  val = range.first;
    if (val > range.second) val = range.second;

    T_text->Text = val.ToString();
    UpdatePreferredSeansAndHall();
}

int Project12::MyForm::GetEffectiveDemand()
{
    // 1) Якщо є реальний попит з підписаного контракту — використовуємо його
    if (_signedDemand >= 0) {
        int d = _signedDemand;
        if (d < 0) d = 0;
        if (d > 100) d = 100;
        return d;
    }

    // 2) Інакше — як і раніше: з текстбоксу або з обраного фільму, з дефолтом 50
    int d = TryParseIntOrDefault(Demand_text, 0);
    if ((d <= 0) && selectedMovieCode >= 0) {
        try { d = Movie::FindMovie(selectedMovieCode).GetDemand(); }
        catch (System::Exception^ ex) {
            System::Diagnostics::Debug::WriteLine(System::String::Format("GetEffectiveDemand: {0}", ex->Message));
        }
        catch (const std::exception& ex) {
            System::Diagnostics::Debug::WriteLine(gcnew System::String(ex.what()));
        }
        catch (...) {
            System::Diagnostics::Debug::WriteLine("GetEffectiveDemand: unknown error");
        }
    }
    if (d <= 0) d = 50;
    if (d < 0) d = 0;
    if (d > 100) d = 100;
    return d;
}



void Project12::MyForm::RebuildHallComboForSelectedSeans()
{
    if (!Seans_cb || !Hall_cb) return;

    int slot = Seans_cb->SelectedIndex; 
    _rebuildingHalls = true;        

    Hall_cb->BeginUpdate();
    Hall_cb->Items->Clear();

    for (int hall : gAllHalls) {
        if (!IsHallBusy(slot, hall)) {
            Hall_cb->Items->Add(System::Convert::ToString(hall));
        }
    }

    Hall_cb->SelectedIndex = (Hall_cb->Items->Count > 0) ? 0 : -1;
    Hall_cb->EndUpdate();

    _rebuildingHalls = false;           
    if (PrefHall_L) {
        PrefHall_L->Text = (Hall_cb->Items->Count == 0)
            ? "No free halls for this seance."
            : "Choose a free hall.";
    }
}

void Project12::MyForm::UpdatePreferredSeansAndHall()
{
    int demand = GetEffectiveDemand();
    double tprice = TryParseDoubleOrDefault(T_text, 0.0);

    
    auto slot = Seans::RecommendSlotByDemand(demand);
    auto slotStr = gcnew System::String(Seans::SlotUiText(slot));
    PrefS_L->Text = "Best seance (by demand " + System::Convert::ToString(demand) + "): " + slotStr;

    if (Seans_cb) {
        Seans_cb->SelectedIndex = Seans::SlotIndex(slot);
    }   
}

System::Void Project12::MyForm::Seans_cb_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
{
    RebuildHallComboForSelectedSeans();
}

System::Void Project12::MyForm::Hall_cb_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
{
    if (_rebuildingHalls) return;                 // не реагуємо на автоселекцію
    if (!Seans_cb || !Hall_cb || Hall_cb->SelectedItem == nullptr) return;

    int slot = Seans_cb->SelectedIndex;
    if (slot < 0) return;

    int hall = 0;
    try { hall = System::Int32::Parse(Hall_cb->SelectedItem->ToString()); }
    catch (...) { return; }

    if (IsHallBusy(slot, hall)) {
        MessageBox::Show("This hall is already assigned to the selected seance.");
        RebuildHallComboForSelectedSeans();
        return;
    }

}

//Accounter

Project12::MyForm::SalaryParse Project12::MyForm::ParseSalaryOnce() {
    SalaryParse sp; sp.ok = false; sp.value = 0.0;
    if (!Sal_text) return sp;

    double val = 0.0;
    bool ok = Double::TryParse(Sal_text->Text, NumberStyles::Float, CultureInfo::CurrentCulture, val)
        || Double::TryParse(Sal_text->Text, NumberStyles::Float, CultureInfo::InvariantCulture, val);
    if (ok) { sp.ok = true; sp.value = val; }
    return sp;
}


void Project12::MyForm::ApproveMovieBudget(System::Text::StringBuilder^ report, bool& anyApproved) {
    // Коментар: перевіряємо наявність бюджету і парсимо MB_text
    if (!this->budget) { report->AppendLine("- Movie budget NOT approved (budget is not initialized)."); return; }

    double mbVal = 0.0;
    bool mbOk = (MB_text && (
        Double::TryParse(MB_text->Text, NumberStyles::Float, CultureInfo::CurrentCulture, mbVal) ||
        Double::TryParse(MB_text->Text, NumberStyles::Float, CultureInfo::InvariantCulture, mbVal)
        ));

    if (!mbOk || mbVal <= 0.0) { report->AppendLine("• Movie budget skipped (empty or invalid)."); return; }

    // Коментар: використовуємо лише доступні кошти
    double available = GetAvailableTotal();
    if (available <= 0.0) {
        report->AppendLine("- Movie budget NOT approved (no money left).");
        return;
    }
    if (mbVal > available) {
        report->AppendLine("• Requested movie budget exceeds available — clamped to available.");
        mbVal = available;
    }

    // Коментар: накопичуємо витрати на фільми і синхронізуємо локальний залишок
    Position::Accountant::RecordExpenses(*this->budget, mbVal);
    if (remainingMovieBudget < 0) remainingMovieBudget = 0;
    remainingMovieBudget += mbVal;

    if (MB_text) MB_text->Clear(); // уникнути повторного додавання на наступний клік

    report->AppendLine("+ Movie budget approved and recorded.");
    anyApproved = true;
}



void Project12::MyForm::ApproveTicketPrice(System::Text::StringBuilder^ report, bool& anyApproved) {
    // Коментар: ціна квитка — потребує контрактів і бюджету
    const auto& contracts = Contract::GetContractList();
    if (contracts.empty()) { report->AppendLine("- Ticket price NOT approved (no contracts yet)."); return; }

    bool hasTicketInput = (T_text && T_text->Text && T_text->Text->Trim()->Length > 0);
    if (!hasTicketInput) { report->AppendLine("• Ticket price skipped (empty)."); return; }
    if (!this->budget) { report->AppendLine("- Ticket price NOT approved (budget is not initialized)."); return; }

    double tRaw = 0.0;
    bool ok = Double::TryParse(T_text->Text, NumberStyles::Float, CultureInfo::CurrentCulture, tRaw)
        || Double::TryParse(T_text->Text, NumberStyles::Float, CultureInfo::InvariantCulture, tRaw);
    if (!ok) { report->AppendLine("• Ticket price skipped (invalid format)."); return; }

    // Коментар: отримуємо ефективні параметри (попит/вартість) з вибраного фільму якщо є
    int demandVal = GetEffectiveDemand();
    double movieCost = TryParseDoubleOrDefault(Price_text, 100000.0);

    if (selectedMovieCode >= 0) {
        try {
            const auto& m = Movie::FindMovie(selectedMovieCode);
            demandVal = m.GetDemand();
            movieCost = m.GetPriceUAH();
        }
        catch (System::Exception^ ex) {
            System::Diagnostics::Debug::WriteLine(System::String::Format("ApproveTicketPrice: {0}", ex->Message));
        }
        catch (const std::exception& ex) {
            System::Diagnostics::Debug::WriteLine(gcnew System::String(ex.what()));
        }
        catch (...) {
            System::Diagnostics::Debug::WriteLine("ApproveTicketPrice: unknown error");
        }
    }

    double tFinal = Position::Accountant::CalcClampedTicketPrice(*this->budget, demandVal, movieCost, tRaw);
    if (T_text) T_text->Text = tFinal.ToString("F0");

    int movieCode = (selectedMovieCode >= 0) ? selectedMovieCode : -1;
    if (movieCode >= 0) {
        if (auto* mm = Movie::FindMovieMutable(movieCode)) {
            mm->SetApprovedTicketPrice(tFinal);
            report->AppendLine("+ Ticket price approved and saved for this movie.");
            anyApproved = true;
        }
        else {
            report->AppendLine("- Movie not found to attach ticket price.");
        }
    }
    else {
        report->AppendLine("- No movie selected to save ticket price.");
    }
}




void Project12::MyForm::ApproveSalary(const SalaryParse& sp, System::Text::StringBuilder^ report, bool& anyApproved) {
    // Коментар: повинні бути працівники та індекс у комбобоксі
    if (!Employee::HasAny() || !Emp_CB || Emp_CB->Items->Count <= 0) {
        report->AppendLine("- Salary NOT approved (no employees in the list).");
        return;
    }

    if (!sp.ok) { report->AppendLine("• Salary skipped (empty or invalid)."); return; }
    if (sp.value < 10000.0) { report->AppendLine("• Salary skipped (below 10000 UAH)."); return; }

    double sal = sp.value;
    double totalBudget = this->budget ? this->budget->GetTotalBudget() : 0.0;
    if (sal > totalBudget) {
        report->AppendLine("• Salary exceeds total budget — clamped to available.");
        sal = totalBudget;
    }

    int idx = (Emp_CB->SelectedIndex >= 0) ? Emp_CB->SelectedIndex : 0;
    int code = -1;
    if (!Employee::TryGetCodeByIndex(idx, code)) {
        report->AppendLine("- Salary NOT approved (failed to resolve employee code).");
        return;
    }

    double prev = 0.0;
    Employee::TryGetSalaryByCode(code, prev);
    Employee::SetSalary(code, sal);

    double delta = sal - prev;
    if (this->budget) this->budget->AddSalaryExpense(delta);

    report->AppendLine("+ Salary approved and stored for the employee.");
    anyApproved = true;
}



// Оновлення лейблу бюджету
void Project12::MyForm::RefreshBudgetLabel() {
    if (!this->budget || !Bud_label) return;
    Bud_label->Text = System::String::Format("Total budget: {0:F0} grn", GetAvailableTotal());
    Bud_label->Visible = true;
}



// Показ підсумку
void Project12::MyForm::ShowApprovalSummary(bool anyApproved, System::Text::StringBuilder^ report) {
    auto title = anyApproved ? "Approval summary" : "Nothing approved";
    System::Windows::Forms::MessageBox::Show(report->ToString(), gcnew System::String(title));
}



System::Void Project12::MyForm::Acc_B_Click(System::Object^ sender, System::EventArgs^ e)
{
    using msclr::interop::marshal_as;
    Acc_B->Enabled = false;

    System::Text::StringBuilder^ report = gcnew System::Text::StringBuilder();
    bool anyApproved = false;

    // 1) Парсимо зарплату один раз (видимо для всіх підблоків)
    SalaryParse sal = ParseSalaryOnce();

    // 2) Окремі підоперації
    ApproveMovieBudget(report, anyApproved);
    ApproveTicketPrice(report, anyApproved);
    ApproveSalary(sal, report, anyApproved);

    // 3) Оновлення UI/бюджетних лейблів
    RefreshBudgetLabel();

    // 4) Підсумкове повідомлення
    ShowApprovalSummary(anyApproved, report);

    // 5) Службове: інкремент підтверджень
    Acc_B->Enabled = true;
    if (anyApproved && _activeEmployeeCode >= 0) {
        Employee::IncrementConfirmations(_activeEmployeeCode);
        UpdateEmployMinMaxLabel();
    }
}



//У вкладці Менеджер - підтверджує зали й сеанси

System::Void Project12::MyForm::Entry_b_Click(System::Object^ sender, System::EventArgs^ e)
{
    using msclr::interop::marshal_as;

    if (!Seans_cb || !Hall_cb) return;

    int slotIndex = Seans_cb->SelectedIndex;
    if (Hall_cb->SelectedItem == nullptr) {
        MessageBox::Show("Please select a hall.");
        return;
    }

    std::string hallText = marshal_as<std::string>(Hall_cb->SelectedItem->ToString());

    AssignmentResult res;
    if (!Position::Manager::AssignHallToSeans(slotIndex, hallText, res)) {
        MessageBox::Show(marshal_as<System::String^>(res.error));
        return;
    }

    MessageBox::Show("Link 'seance - hall' has been saved.");

    if (selectedMovieCode >= 0) {
        if (auto* m = Movie::FindMovieMutable(selectedMovieCode)) {
            m->SetManagerAssignment(slotIndex, res.hallNo);
        }
    }
    RebuildHallComboForSelectedSeans();

    if (_activeEmployeeCode >= 0) {
        Employee::IncrementConfirmations(_activeEmployeeCode);
        UpdateEmployMinMaxLabel();
    }
}



System::Windows::Forms::Panel^ Project12::MyForm::CreateViewerMovieCard(const Movie& m)
{
    using namespace System::Drawing;

    const bool blockedPrice = !(m.HasApprovedTicketPrice() && m.GetApprovedTicketPrice() > 0.0);
    const bool blockedMgr = !m.HasManagerAssignment();
    const bool blocked = blockedPrice || blockedMgr;

    double ticketPrice = 0.0;
    if (!blockedPrice) ticketPrice = m.GetApprovedTicketPrice();

    Panel^ card = gcnew Panel();
    card->BorderStyle = BorderStyle::FixedSingle;
    card->BackColor = Color::White;
    card->Size = System::Drawing::Size(260, 190);
    card->Margin = Windows::Forms::Padding(8);

    int y = 8;

    auto title = gcnew Label();
    title->AutoSize = true;
    title->Font = gcnew System::Drawing::Font("Segoe UI", 10, System::Drawing::FontStyle::Bold);
    title->Text = "Title: " + gcnew System::String(m.GetTitle().c_str());
    title->Location = Point(8, y); y += 22;
    card->Controls->Add(title);

    auto genre = gcnew Label();
    genre->AutoSize = true;
    genre->Text = "Genre: " + gcnew System::String(m.GetGenre().c_str());
    genre->Location = Point(8, y); y += 18;
    card->Controls->Add(genre);

    auto age = gcnew Label();
    age->AutoSize = true;
    age->Text = "Age: " + System::Convert::ToString(m.GetAgeLimit());
    age->Location = Point(8, y); y += 18;
    card->Controls->Add(age);

    auto duration = gcnew Label();
    duration->AutoSize = true;
    duration->Text = "Duration: " + System::Convert::ToString(m.GetDuration()) + " min";
    duration->Location = Point(8, y); y += 18;
    card->Controls->Add(duration);

    auto price = gcnew Label();
    price->AutoSize = true;
    if (blockedPrice) {
        price->Text = "Ticket: — (pending accountant)";
    }
    else {
        price->Text = System::String::Format("Ticket: {0:F0} UAH", ticketPrice);
    }
    price->Location = Point(8, y); y += 18;
    card->Controls->Add(price);

    if (blockedMgr) {
        auto note = gcnew Label();
        note->AutoSize = true;
        note->ForeColor = Color::DarkRed;
        note->Text = "Seance/hall: not assigned yet";
        note->Location = Point(8, y); y += 18;
        card->Controls->Add(note);
    }

    auto accept = gcnew Button();
    accept->Text = "Accept";
    accept->Size = System::Drawing::Size(100, 28);
    accept->Location = Point(8, y);
    accept->Tag = m.GetCode();
    card->Tag = m.GetCode();

    if (blocked) {
        accept->Enabled = false;
        card->Click += gcnew System::EventHandler(this, &Project12::MyForm::OnViewerMovieBlocked);
    }
    else {
        accept->Click += gcnew System::EventHandler(this, &Project12::MyForm::OnAcceptViewerMovie);
        card->Click += gcnew System::EventHandler(this, &Project12::MyForm::OnAcceptViewerMovie);
    }

    card->Controls->Add(accept);
    return card;
}


System::Void Project12::MyForm::OnAcceptViewerMovie(System::Object^ sender, System::EventArgs^ e)
{
    System::Windows::Forms::Control^ src = dynamic_cast<System::Windows::Forms::Control^>(sender);
    if (!src) return;

    System::Object^ tag = src->Tag;
    if (!tag && src->Parent) tag = src->Parent->Tag;
    if (!tag) return;

    try { selectedMovieCode = safe_cast<int>(tag); }
    catch (...) { return; }

    try {
        const auto& m = Movie::FindMovie(selectedMovieCode);
        if (!m.HasManagerAssignment()) {
            MessageBox::Show("Manager must assign a seance and a hall before you can proceed.");
            return;
        }
        if (!(m.HasApprovedTicketPrice() && m.GetApprovedTicketPrice() > 0.0)) {
            MessageBox::Show("The accountant must approve the ticket price first.");
            return;
        }
    }
    catch (...) {
        return;
    }

    UpdateViewerTicketPriceLabel();
    ShowPage(7);
    PopulateViewerChoicesFromManager();
}

System::Void Project12::MyForm::View_B_Click(System::Object^ sender, System::EventArgs^ e)
{
    HideAllControls();

    Contr_LM->Visible = true;
    Contr_LM->Text = "Welcome viewer!";

    moviesFlow->Visible = true;

    PopulateViewerCatalog();
}

void Project12::MyForm::PopulateViewerCatalog()
{
    if (!moviesFlow) return;

    moviesFlow->FlowDirection = FlowDirection::LeftToRight;
    moviesFlow->WrapContents = true;
    moviesFlow->AutoScroll = true;
    moviesFlow->Controls->Clear();

    const auto& contracts = Contract::GetContractList();
    if (contracts.empty()) {
        System::Windows::Forms::MessageBox::Show("No approved contracts yet.");
        return;
    }

    std::set<int> seenMovieCodes;

    for (const auto& c : contracts) {
        const Movie& m = c.GetMovie();
        if (!seenMovieCodes.insert(m.GetCode()).second) continue; 

        auto card = CreateViewerMovieCard(m);
        moviesFlow->Controls->Add(card);
    }

    if (moviesFlow->Controls->Count == 0) {
        System::Windows::Forms::MessageBox::Show("No movies available for viewers.");
    }
}

void Project12::MyForm::UpdateViewerTicketPriceLabel()
{
    using System::Diagnostics::Debug;
    using System::Windows::Forms::MessageBox;

    double price = 0.0;

    // 1) Спроба взяти затверджену ціну квитка з фільму
    if (selectedMovieCode >= 0) {
        try {
            const auto& m = Movie::FindMovie(selectedMovieCode);
            if (m.HasApprovedTicketPrice()) {
                price = m.GetApprovedTicketPrice();
            }
        }
        catch (const std::exception& ex) {
            Debug::WriteLine(gcnew System::String(ex.what()));
            MessageBox::Show("Failed to load movie data. Using estimated ticket price.", "Movie error");
        }
        catch (System::Exception^ ex) {
            Debug::WriteLine(System::String::Format("UpdateViewerTicketPriceLabel: {0}", ex->Message));
            MessageBox::Show("Internal error while reading the movie. Using estimated ticket price.", "Error");
        }
        catch (...) {
            Debug::WriteLine("UpdateViewerTicketPriceLabel: unknown error");
            MessageBox::Show("Unknown error while reading the movie. Using estimated ticket price.", "Error");
        }
    }

    // 2) Якщо ціни немає — рахуємо оціночну через бюджет (з говорящим кетчем всередині)
    if (price <= 0.0) {
        // ефективний попит (твоя функція)
        int demand = GetEffectiveDemand();

        double movieCost = 100000.0; // дефолтна собівартість, якщо не вдалось прочитати
        if (selectedMovieCode >= 0) {
            try {
                movieCost = Movie::FindMovie(selectedMovieCode).GetPriceUAH();
            }
            catch (const std::exception& ex) {
                Debug::WriteLine(gcnew System::String(ex.what()));
                MessageBox::Show("Could not read the movie cost. Falling back to a default estimate.", "Warning");
            }
            catch (System::Exception^ ex) {
                Debug::WriteLine(System::String::Format("Read movie cost failed: {0}", ex->Message));
                MessageBox::Show("Could not read the movie cost. Falling back to a default estimate.", "Warning");
            }
            catch (...) {
                Debug::WriteLine("Read movie cost: unknown error");
                MessageBox::Show("Could not read the movie cost. Falling back to a default estimate.", "Warning");
            }
        }

        // об’єкт бюджету (використовуємо наявний або тимчасовий)
        Budget* b = this->budget ? this->budget : new Budget(101);

        try {
            auto pr = b->PriceMinMax(demand, movieCost);
            price = (pr.first + pr.second) / 2.0; // середина діапазону як оціночна
        }
        catch (const std::exception& ex) {
            Debug::WriteLine(gcnew System::String(ex.what()));
            MessageBox::Show("Failed to calculate ticket price range. Using a basic fallback.", "Budget");
            price = 100.0; // базовий фолбек
        }
        catch (System::Exception^ ex) {
            Debug::WriteLine(System::String::Format("PriceMinMax failed: {0}", ex->Message));
            MessageBox::Show("Failed to calculate ticket price range. Using a basic fallback.", "Budget");
            price = 100.0;
        }
        catch (...) {
            Debug::WriteLine("PriceMinMax: unknown error");
            MessageBox::Show("Failed to calculate ticket price range. Using a basic fallback.", "Budget");
            price = 100.0;
        }

        // видаляємо тимчасовий бюджет (лише якщо створили локально)
        if (!this->budget) {
            delete b;
        }
    }

    // 3) Оновлення лейбла UI
    if (Mt_l) {
        Mt_l->Text = System::String::Format("Ticket price: {0:F0} UAH", price);
        Mt_l->Visible = true;
    }
}

static std::pair<int, int> GetSlotRangeMinutes(int slotIdx) {
    switch (slotIdx) {
    case 0: return { 10 * 60, 13 * 60 };
    case 1: return { 14 * 60, 17 * 60 };
    default: return { 18 * 60, 22 * 60 };
    }
}
static System::String^ HHMM(int minutes) {
    int h = minutes / 60, m = minutes % 60;
    return System::String::Format("{0:D2}:{1:D2}", h, m);
}

static System::Collections::Generic::List<System::String^>^
BuildTimeBlocksForSlot(int slotIdx, int durationMin) {
    using ListStr = System::Collections::Generic::List<System::String^>;
    auto list = gcnew ListStr();
    if (durationMin <= 0) return list;

    auto range = GetSlotRangeMinutes(slotIdx);
    int start = range.first, end = range.second;

    for (int t = start; t + durationMin <= end; t += durationMin) {
        auto s1 = HHMM(t);
        auto s2 = HHMM(t + durationMin);
        list->Add(System::String::Format("{0}-{1}", s1, s2));
    }
    return list;
}

void Project12::MyForm::PopulateViewerChoicesFromManager() {
    using System::Windows::Forms::MessageBox;
    using System::Diagnostics::Debug;
    using System::Collections::Generic::List;

    // 0) Basic input validation
    if (selectedMovieCode < 0 || !cs_cb || !ch_cb) {
        MessageBox::Show("Please select a movie (or initialize the lists).", "Data not ready");
        return;
    }

    int seansIdx = 0;
    int hallNo = 0;
    int durMin = 120; // default in case something goes wrong

    // 1) Safe movie data read (talkative catches)
    try {
        const auto& m = Movie::FindMovie(selectedMovieCode);
        durMin = m.GetDuration();
        if (m.HasManagerAssignment()) {
            seansIdx = m.GetApprovedSeansIdx();
            hallNo = m.GetApprovedHallNo();
        }
    }
    catch (const std::exception& ex) {
        Debug::WriteLine(gcnew System::String(ex.what()));
        MessageBox::Show("Failed to load movie data: " + gcnew System::String(ex.what()), "Movie error");
        return;
    }
    catch (System::Exception^ ex) {
        Debug::WriteLine(System::String::Format("PopulateViewerChoicesFromManager: {0}", ex->Message));
        MessageBox::Show("Internal error while loading the movie: " + ex->Message, "Error");
        return;
    }
    catch (...) {
        Debug::WriteLine("PopulateViewerChoicesFromManager: unknown error on movie load");
        MessageBox::Show("Unknown error while loading movie data.", "Error");
        return;
    }

    // 2) Build time blocks (returns List<String^>)
    List<System::String^>^ blocks = nullptr;
    try {
        blocks = BuildTimeBlocksForSlot(seansIdx, durMin);
    }
    catch (const std::exception& ex) {
        Debug::WriteLine(gcnew System::String(ex.what()));
        MessageBox::Show("Error while building time blocks: " + gcnew System::String(ex.what()), "Error");
        return;
    }
    catch (System::Exception^ ex) {
        Debug::WriteLine(System::String::Format("BuildTimeBlocksForSlot failed: {0}", ex->Message));
        MessageBox::Show("Failed to generate time slots: " + ex->Message, "Error");
        return;
    }
    catch (...) {
        Debug::WriteLine("BuildTimeBlocksForSlot: unknown error");
        MessageBox::Show("An unknown error occurred while generating time slots.", "Error");
        return;
    }

    cs_cb->BeginUpdate();
    cs_cb->Items->Clear();
    if (blocks != nullptr && blocks->Count > 0) {
        for each (System::String ^ s in blocks) {
            cs_cb->Items->Add(s);
        }
        cs_cb->SelectedIndex = 0;
    }
    else {
        // Nothing to show — inform the user
        MessageBox::Show("No available time blocks for the selected showtime.", "No data");
        cs_cb->SelectedIndex = -1;
    }
    cs_cb->EndUpdate();

    // 3) If the manager assigned a hall — try to preselect it in the list
    int parseFailures = 0;
    if (hallNo > 0) {
        int idx = -1;
        for (int i = 0; i < ch_cb->Items->Count; ++i) {
            try {
                if (System::Int32::Parse(ch_cb->Items[i]->ToString()) == hallNo) { idx = i; break; }
            }
            catch (System::FormatException^) { ++parseFailures; }
            catch (System::OverflowException^) { ++parseFailures; }
            catch (System::Exception^ ex) {
                ++parseFailures;
                Debug::WriteLine(System::String::Format("Hall parse error: {0}", ex->Message));
            }
            catch (...) {
                ++parseFailures;
                Debug::WriteLine("Hall parse error: unknown");
            }
        }
        if (parseFailures > 0) {
            MessageBox::Show(System::String::Format(
                "Some hall list items are corrupted or have an invalid format (cases: {0}).", parseFailures),
                "Warning");
        }
        if (idx >= 0) ch_cb->SelectedIndex = idx;
    }

    // 4) Rebuild the hall list with occupancy diagnostics
    ch_cb->BeginUpdate();
    ch_cb->Items->Clear();

    if (hallNo > 0) {
        // Manager pinned a hall — show only that one
        ch_cb->Items->Add(System::Convert::ToString(hallNo));
        ch_cb->SelectedIndex = 0;
    }
    else {
        int added = 0;
        for (int h : gAllHalls) {
            try {
                if (!IsHallBusy(seansIdx, h)) {
                    ch_cb->Items->Add(System::Convert::ToString(h));
                    ++added;
                }
            }
            catch (const std::exception& ex) {
                Debug::WriteLine(gcnew System::String(ex.what()));
            }
            catch (System::Exception^ ex) {
                Debug::WriteLine(System::String::Format("IsHallBusy failed: {0}", ex->Message));
            }
            catch (...) {
                Debug::WriteLine("IsHallBusy: unknown error");
            }
        }
        if (added == 0) {
            MessageBox::Show("No free halls for the selected time slot.", "Schedule is busy");
            ch_cb->SelectedIndex = -1;
        }
        else {
            ch_cb->SelectedIndex = 0;
        }
    }

    ch_cb->EndUpdate();
}

void Project12::MyForm::UpdateDateTBounds()
{
    if (!DateT || !Startd_picker || !Endd_picker) return;

    auto minD = Startd_picker->Value.Date;
    auto maxD = Endd_picker->Value.Date;

    if (minD > maxD) {
        auto tmp = minD;
        minD = maxD;
        maxD = tmp;
    }


    DateT->MinDate = minD;
    DateT->MaxDate = maxD;

    if (DateT->Value < minD) DateT->Value = minD;
    if (DateT->Value > maxD) DateT->Value = maxD;

    DateT->Enabled = true;
}

double Project12::MyForm::GetAvailableTotal() {
    if (!this->budget) return 0.0;
    double out = this->budget->GetTotalBudget()
        - this->budget->GetSalaryExpenses()
        - this->budget->GetMovieExpenses();
    return (out < 0.0) ? 0.0 : out;
}

void Project12::MyForm::RebuildSeatComboForCurrentSelection() {
    if (!cpl_cb || !cs_cb || !ch_cb) return;
    if (cs_cb->SelectedItem == nullptr || ch_cb->SelectedItem == nullptr) return;

    int slotIdx = -1;
    if (selectedMovieCode >= 0) {
        try {
            const auto& m = Movie::FindMovie(selectedMovieCode);
            slotIdx = m.GetApprovedSeansIdx();
        }
        catch (System::Exception^ ex) {
            System::Diagnostics::Debug::WriteLine(System::String::Format("RebuildSeatComboForCurrentSelection: {0}", ex->Message));
        }
        catch (const std::exception& ex) {
            System::Diagnostics::Debug::WriteLine(gcnew System::String(ex.what()));
        }
        catch (...) {
            System::Diagnostics::Debug::WriteLine("RebuildSeatComboForCurrentSelection: unknown error");
        }
    }
    if (slotIdx < 0) slotIdx = cs_cb->SelectedIndex;

    int hallNo = 0;
    try { hallNo = System::Int32::Parse(ch_cb->SelectedItem->ToString()); }
    catch (...) { return; }

    std::string block = msclr::interop::marshal_as<std::string>(cs_cb->SelectedItem->ToString());
    std::string key = MakeSeatKey(slotIdx, hallNo, block);

    cpl_cb->BeginUpdate();
    cpl_cb->Items->Clear();

    for (int seat = 1; seat <= 30; ++seat) {
        if (gTakenSeats[key].count(seat) == 0) {
            cpl_cb->Items->Add(System::Convert::ToString(seat));
        }
    }

    cpl_cb->SelectedIndex = (cpl_cb->Items->Count > 0) ? 0 : -1;
    cpl_cb->EndUpdate();
}

System::Void Project12::MyForm::cs_cb_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
{
    RebuildSeatComboForCurrentSelection();
}

System::Void Project12::MyForm::OnViewerMovieBlocked(System::Object^, System::EventArgs^)
{
    System::Windows::Forms::MessageBox::Show(
        "This movie is not available yet:\n"
        "• The accountant hasn't approved the ticket price, and/or\n"
        "• The manager hasn't assigned a seance and a hall.",
        "Not available"
    );
}

int __clrcall TryParseIntOrDefault(System::Windows::Forms::TextBox^ tb, int defVal) {
    using namespace System;
    using namespace System::Globalization;
    if (!tb || String::IsNullOrWhiteSpace(tb->Text)) return defVal;

    int v = defVal;
    bool ok = Int32::TryParse(tb->Text, NumberStyles::Integer, CultureInfo::CurrentCulture, v)
        || Int32::TryParse(tb->Text, NumberStyles::Integer, CultureInfo::InvariantCulture, v);
    return ok ? v : defVal;
}

double __clrcall TryParseDoubleOrDefault(System::Windows::Forms::TextBox^ tb, double defVal) {
    using namespace System;
    using namespace System::Globalization;
    if (!tb || String::IsNullOrWhiteSpace(tb->Text)) return defVal;

    double v = defVal;
    bool ok = Double::TryParse(tb->Text, NumberStyles::Float | NumberStyles::AllowThousands, CultureInfo::CurrentCulture, v)
        || Double::TryParse(tb->Text, NumberStyles::Float | NumberStyles::AllowThousands, CultureInfo::InvariantCulture, v);
    return ok ? v : defVal;
}

bool Project12::MyForm::ValidateViewerControls() {
    if (!cs_cb || !ch_cb || !cpl_cb || !DateT) return false;

    if (selectedMovieCode < 0) {
        System::Windows::Forms::MessageBox::Show("Please choose a movie first.");
        return false;
    }
    if (cs_cb->SelectedItem == nullptr) {
        System::Windows::Forms::MessageBox::Show("Please choose a time block.");
        return false;
    }
    if (ch_cb->SelectedItem == nullptr) {
        System::Windows::Forms::MessageBox::Show("Please choose a hall.");
        return false;
    }
    if (cpl_cb->SelectedItem == nullptr) {
        System::Windows::Forms::MessageBox::Show("Please choose a seat.");
        return false;
    }
    return true;
}

bool Project12::MyForm::ResolveSlotIndex(int& slotIdx) {
    slotIdx = -1;
    try {
        const auto& m = Movie::FindMovie(selectedMovieCode); // повертає const Movie&
        if (!m.HasManagerAssignment()) {
            System::Windows::Forms::MessageBox::Show("Manager must assign a seance and a hall before buying.");
            return false;
        }
        slotIdx = m.GetApprovedSeansIdx();
        return true;
    }
    catch (...) {
        System::Windows::Forms::MessageBox::Show("Movie not found.");
        return false;
    }
}

bool Project12::MyForm::ParseHallAndSeat(int& hallNo, int& seatNo) {
    hallNo = 0; seatNo = 0;
    try { hallNo = System::Int32::Parse(ch_cb->SelectedItem->ToString()); }
    catch (...) { System::Windows::Forms::MessageBox::Show("Invalid hall."); return false; }

    try { seatNo = System::Int32::Parse(cpl_cb->SelectedItem->ToString()); }
    catch (...) { System::Windows::Forms::MessageBox::Show("Invalid seat."); return false; }

    return true;
}

void Project12::MyForm::ReadTimeAndDate(std::string& block, std::string& dateStr) {
    using msclr::interop::marshal_as;
    block = marshal_as<std::string>(cs_cb->SelectedItem->ToString());           // "10:00-11:40"
    dateStr = marshal_as<std::string>(DateT->Value.ToString("yyyy-MM-dd"));       // "yyyy-MM-dd"
}

bool Project12::MyForm::CheckAndLockSeat(int slotIdx, int hallNo, int seatNo, const std::string& block) {
    std::string seatKey = MakeSeatKey(slotIdx, hallNo, block);
    if (gTakenSeats[seatKey].count(seatNo) != 0) {
        System::Windows::Forms::MessageBox::Show("This seat is already taken.");
        RebuildSeatComboForCurrentSelection();
        return false;
    }
    gTakenSeats[seatKey].insert(seatNo);
    return true;
}

double Project12::MyForm::ComputeTicketPriceOrDefault() {
    double price = TryParseDoubleOrDefault(T_text, 0.0);
    if (price > 0.0) return price;

    // fall back: середина між min/max
    int demandVal = GetEffectiveDemand();
    double movieCost = 100000.0;
    try { movieCost = Movie::FindMovie(selectedMovieCode).GetPriceUAH(); }
    catch (System::Exception^ ex) {
        System::Diagnostics::Debug::WriteLine(System::String::Format("ComputeTicketPriceOrDefault: {0}", ex->Message));
    }
    catch (const std::exception& ex) {
        System::Diagnostics::Debug::WriteLine(gcnew System::String(ex.what()));
    }
    catch (...) {
        System::Diagnostics::Debug::WriteLine("ComputeTicketPriceOrDefault: unknown error");
    }

    // уникаємо new/delete: користуємось існуючим бюджетом або тимчасовим стековим
    if (this->budget) {
        auto pr = this->budget->PriceMinMax(demandVal, movieCost);
        return (pr.first + pr.second) / 2.0;
    }
    else {
        Budget temp(101);
        auto pr = temp.PriceMinMax(demandVal, movieCost);
        return (pr.first + pr.second) / 2.0;
    }
}

void Project12::MyForm::ApplyBudgetAfterSale(double price) {
    if (!this->budget) return;
    this->budget->AddIncome(price);
    if (Bud_label) {
        Bud_label->Text = System::String::Format("Total budget: {0} grn", this->budget->GetTotalBudget());
    }
}

void Project12::MyForm::RemoveSelectedSeatFromCombo() {
    if (!cpl_cb) return;
    int removeIdx = cpl_cb->SelectedIndex;
    if (removeIdx >= 0 && removeIdx < cpl_cb->Items->Count) {
        cpl_cb->Items->RemoveAt(removeIdx);
        cpl_cb->SelectedIndex = (cpl_cb->Items->Count > 0) ? 0 : -1;
    }
}

void Project12::MyForm::ShowTicketSuccess(const std::string& dateStr, const std::string& block, int hallNo, int seatNo, double price) {
    using System::Windows::Forms::MessageBox;
    MessageBox::Show(
        "Seat booked successfully.\n"
        "Date " + this->DateT->Value.ToString("yyyy-MM-dd") +
        "\nTime " + gcnew System::String(block.c_str()) +
        "\nHall #" + System::Convert::ToString(hallNo) +
        ", Seat " + System::Convert::ToString(seatNo) +
        "\nPrice " + System::String::Format("{0:F0} UAH", price),
        "Ticket"
    );
}

System::Void Project12::MyForm::Buy_Click(System::Object^ sender, System::EventArgs^ e)
{
    using msclr::interop::marshal_as;
    if (!ValidateViewerControls()) return;
    int slotIdx = -1;
    if (!ResolveSlotIndex(slotIdx)) return;
    int hallNo = 0, seatNo = 0;
    if (!ParseHallAndSeat(hallNo, seatNo)) return;
    std::string block, dateStr;
    ReadTimeAndDate(block, dateStr);
    if (!CheckAndLockSeat(slotIdx, hallNo, seatNo, block)) return;
    double price = ComputeTicketPriceOrDefault();
    Ticket issued = Ticket::IssueNextAndSaveDetailed(
        selectedMovieCode, dateStr, block, price, hallNo, seatNo
    );
    ApplyBudgetAfterSale(price);
    RemoveSelectedSeatFromCombo();
    ShowTicketSuccess(dateStr, block, hallNo, seatNo, price);
}

System::Void Project12::MyForm::Emp_CB_SelectedIndexChanged(System::Object^, System::EventArgs^) {
    UpdateEmployMinMaxLabel();
}

void Project12::MyForm::UpdateEmployMinMaxLabel() {
    if (!EmployMinMax || !Emp_CB) return;

    int idx = Emp_CB->SelectedIndex;
    int code = -1;
    if (idx >= 0 && Employee::TryGetCodeByIndex(idx, code)) {
        int conf = 0;
        if (Employee::TryGetConfirmations(code, conf)) {
            int bonus = Employee::RecommendedBonusByConfirmations(conf);
            double avail = GetAvailableTotal();
            EmployMinMax->Text = System::String::Format(
                "Recommended bonus: {0} UAH (successful actions: {1})",
                bonus, conf
            );
            return;
        }
    }
    EmployMinMax->Text = "Recomm bonus: —";

    UpdateBudgetLabelsFromPercents();
}

void Project12::MyForm::UpdateBudgetLabelsFromPercents() {
    UpdateBudgetLabelsFromPercents(0.80, 0.20);
}

System::Void Project12::MyForm::UpdateBudgetLabelsFromPercents(double movieShare, double salaryShare)
{
    double total = GetAvailableTotal();
    if (total < 0) total = 0;

    const double movieBudget = total * movieShare;
    const double salaryPool = total * salaryShare;

    if (M_label) {
        M_label->Text = System::String::Format(
            "Movie budget ({0:P0} of total): {1:F0} UAH",
            movieShare, movieBudget
        );
        M_label->Visible = true;
    }

    int bonus = 0, conf = 0;
    if (Emp_CB) {
        int idx = Emp_CB->SelectedIndex, code = -1;
        if (idx >= 0 && Employee::TryGetCodeByIndex(idx, code) &&
            Employee::TryGetConfirmations(code, conf))
            bonus = Employee::RecommendedBonusByConfirmations(conf);
    }

    if (EmployMinMax) {
        EmployMinMax->Text = System::String::Format(
            "Salary pool ({0:P0} of total): {1:F0} UAH — Recommended bonus: {2} UAH{3}",
            salaryShare, salaryPool, bonus,
            (conf > 0 ? System::String::Format(" (successful actions: {0})", conf) : "")
        );
        EmployMinMax->Visible = true;
    }
}

void Project12::MyForm::PopulateContractsCombo(int selectCode) {
    if (!contracts_cb) return;
    contracts_cb->Items->Clear();

    auto list = Contract::GetContractList();
    int toSelect = -1, i = 0;
    for (const auto& c : list) {
        const Movie& m = c.GetMovie();
        System::String^ display = System::String::Format(
            "{0}. {1} [{2} - {3}]",
            c.GetCode(),
            msclr::interop::marshal_as<System::String^>(m.GetTitle()),
            msclr::interop::marshal_as<System::String^>(c.GetSignDate()),
            msclr::interop::marshal_as<System::String^>(c.GetExpiryDate())
        );
        auto kv = System::Collections::Generic::KeyValuePair<int, System::String^>(c.GetCode(), display);
        contracts_cb->Items->Add(kv);
        if (c.GetCode() == selectCode) toSelect = i;
        ++i;
    }

    if (contracts_cb->Items->Count > 0)
        contracts_cb->SelectedIndex = (toSelect >= 0 ? toSelect : 0);
}


[STAThreadAttribute]
int main(cli::array<System::String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    Project12::MyForm^ form = gcnew Project12::MyForm();
    Application::Run(form);

    return 0;
}

