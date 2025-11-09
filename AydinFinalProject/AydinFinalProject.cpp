#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// --- INGREDIENT ---
class Ingredient {
private:
    string name;
    int quantity;
public:
    Ingredient() {}
    Ingredient(const string& n, int q) : name(n), quantity(q) {}

    void setName(const string& n) { name = n; }
    void setQuantity(int q) { quantity = q; }
    string getName() const { return name; }
    int getQuantity() const { return quantity; }

    void add(int q) { quantity += q; }
    bool remove(int q) {
        if (q > quantity) return false;
        quantity -= q;
        return true;
    }
    // txt string
    string toText() const { return name + " " + to_string(quantity); }
    static Ingredient fromText(const string& line) {
        Ingredient ing;
        size_t sp = line.find(' ');
        if (sp != string::npos) {
            ing.name = line.substr(0, sp);
            ing.quantity = stoi(line.substr(sp + 1));
        }
        return ing;
    }
};

// --- STOCK ---
class Stock {
private:
    vector<Ingredient> ingredients;
public:
    void addIngredient(const string& n, int q) {
        for (auto& ing : ingredients) {
            if (ing.getName() == n) {
                ing.add(q);
                saveToFile();
                cout << "Ingredient quantity increased!\n";
                return;
            }
        }
        ingredients.push_back(Ingredient(n, q));
        saveToFile();
        cout << "Ingredient added!\n";
    }
    bool useIngredient(const string& n, int q) {
        for (auto& ing : ingredients)
            if (ing.getName() == n)
                if (ing.remove(q)) { saveToFile(); return true; }
                else return false;
        return false;
    }
    void showAll() const {
        cout << "\n--- INGREDIENT STOCK ---\n";
        for (const auto& ing : ingredients)
            cout << ing.getName() << ": " << ing.getQuantity() << endl;
        if (ingredients.empty()) cout << "(Nothing found)\n";
    }
    bool hasEnough(const string& n, int q) const {
        for (const auto& ing : ingredients)
            if (ing.getName() == n && ing.getQuantity() >= q) return true;
        return false;
    }
    void loadFromFile(const string& file = "ingredients.txt") {
        ingredients.clear();
        ifstream fin(file);
        string line;
        while (getline(fin, line)) {
            if (!line.empty())
                ingredients.push_back(Ingredient::fromText(line));
        }
    }
    void saveToFile(const string& file = "ingredients.txt") const {
        ofstream fout(file);
        for (const auto& ing : ingredients)
            fout << ing.toText() << endl;
    }
};

// --- USER ---
class User {
private:
    string username, password, email;
    bool admin;
public:
    User() : admin(false) {}
    User(const string& u, const string& p, const string& e, bool isAdmin = false)
        : username(u), password(p), email(e), admin(isAdmin) {
    }

    void setUsername(const string& u) { username = u; }
    void setPassword(const string& p) { password = p; }
    void setEmail(const string& e) { email = e; }
    void setAdmin(bool val) { admin = val; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getEmail()    const { return email; }
    bool isAdmin() const { return admin; }
    string toText() const { return username + " " + password + " " + email + " " + (admin ? "1" : "0"); }
    static User fromText(const string& line) {
        User u;
        vector<string> field;
        size_t pos = 0, prev = 0;
        for (int i = 0; i < 3; ++i) {
            pos = line.find(' ', prev);
            field.push_back(line.substr(prev, pos - prev));
            prev = pos + 1;
        }
        field.push_back(line.substr(prev));
        u.username = field[0]; u.password = field[1]; u.email = field[2]; u.admin = (field[3] == "1");
        return u;
    }
};

// --- USER MANAGER ---
class UserManager {
private:
    vector<User> users;
    string activeUser;
    bool activeUserIsAdmin = false;
public:
    UserManager() { loadFromFile(); }
    void loadFromFile(const string& file = "users.txt") {
        users.clear();
        ifstream fin(file);
        string line;
        while (getline(fin, line)) {
            if (!line.empty())
                users.push_back(User::fromText(line));
        }
    }
    void saveToFile(const string& file = "users.txt") const {
        ofstream fout(file);
        for (const auto& u : users)
            fout << u.toText() << endl;
    }
    void signUp() {
        string u, p, e; bool admin = false;
        cout << "Username: "; cin >> u;
        cout << "Password: "; cin >> p;
        cout << "Email: "; cin >> e;
        if (u == "admin") admin = true;
        for (const auto& user : users) {
            if (user.getUsername() == u) {
                cout << "Username already exists!\n";
                return;
            }
        }
        users.push_back(User(u, p, e, admin));
        saveToFile();
        cout << "Registration complete!\n";
    }
    bool signIn() {
        string u, p;
        cout << "Username: "; cin >> u;
        cout << "Password: "; cin >> p;
        for (const auto& user : users) {
            if (user.getUsername() == u && user.getPassword() == p) {
                activeUser = u;
                activeUserIsAdmin = user.isAdmin();
                cout << "Logged in!\n";
                return true;
            }
        }
        cout << "Username or password is incorrect!\n";
        return false;
    }
    string getActive() const { return activeUser; }
    bool isActiveAdmin() const { return activeUserIsAdmin; }
    const vector<User>& getAll() const { return users; }
    void showAll() const {
        cout << "\nALL USERS:\n";
        for (const auto& u : users)
            cout << u.getUsername() << " (" << u.getEmail() << ")\n";
        if (users.empty()) cout << "(No users)\n";
    }
};

// --- ORDER ---
class Order {
private:
    string username;
    string dish;
    string status;
public:
    Order() {}
    Order(const string& u, const string& d, const string& s = "Preparing") : username(u), dish(d), status(s) {}

    void setUsername(const string& u) { username = u; }
    void setDish(const string& d) { dish = d; }
    void setStatus(const string& s) { status = s; }
    string getUsername() const { return username; }
    string getDish()     const { return dish; }
    string getStatus()   const { return status; }
    string toText() const { return username + " " + dish + " " + status; }
    static Order fromText(const string& line) {
        Order o;
        size_t p1 = line.find(' ');
        size_t p2 = line.find(' ', p1 + 1);
        o.username = line.substr(0, p1);
        o.dish = line.substr(p1 + 1, p2 - p1 - 1);
        o.status = line.substr(p2 + 1);
        return o;
    }
};

// --- ORDER MANAGER ---
class OrderManager {
private:
    vector<Order> orders;
public:
    OrderManager() { loadFromFile(); }
    void loadFromFile(const string& file = "orders.txt") {
        orders.clear();
        ifstream fin(file);
        string line;
        while (getline(fin, line)) {
            if (!line.empty())
                orders.push_back(Order::fromText(line));
        }
    }
    void saveToFile(const string& file = "orders.txt") const {
        ofstream fout(file);
        for (const auto& o : orders)
            fout << o.toText() << endl;
    }
    void createOrder(const string& username, const string& dish) {
        orders.push_back(Order(username, dish));
        saveToFile();
        cout << "Your order has been accepted!\n";
    }
    void showMyOrders(const string& username) const {
        cout << "\nYour orders:\n";
        int cnt = 0;
        for (const auto& o : orders)
            if (o.getUsername() == username) {
                cout << "- " << o.getDish() << " | Status: " << o.getStatus() << endl;
                cnt++;
            }
        if (!cnt) cout << "(No orders)\n";
    }
    void showAllOrders() const {
        cout << "\nAll orders:\n";
        int i = 0;
        for (const auto& o : orders)
            cout << ++i << ") " << o.getUsername() << " => " << o.getDish() << " (" << o.getStatus() << ")\n";
        if (orders.empty())
            cout << "(No orders found)\n";
    }
};

// --- MENU (dishes) ---
class Menu {
private:
    vector<string> dishes;
public:
    Menu() { loadFromFile(); }
    void loadFromFile(const string& file = "menu.txt") {
        dishes.clear();
        ifstream fin(file);
        string line;
        while (getline(fin, line)) {
            if (!line.empty())
                dishes.push_back(line);
        }
    }
    void saveToFile(const string& file = "menu.txt") const {
        ofstream fout(file);
        for (const auto& d : dishes)
            fout << d << endl;
    }
    void showMenu() const {
        cout << "\nMENU:\n";
        for (size_t i = 0; i < dishes.size(); i++)
            cout << i + 1 << ". " << dishes[i] << endl;
        if (dishes.empty()) cout << "(No dishes)\n";
    }
    void addDish(const string& dish) {
        dishes.push_back(dish);
        saveToFile();
    }
    string getMenuByIndex(int idx) const {
        if (idx >= 0 && idx < (int)dishes.size())
            return dishes[idx];
        return "";
    }
    int getMenuSize() const { return (int)dishes.size(); }
};

// --- ADMIN ---
class Admin {
public:
    void adminPanel(UserManager& userManager, OrderManager& orderManager, Stock& stock, Menu& menu) {
        int adminChoice;
        do {
            cout << "\n--- ADMIN PANEL ---\n";
            cout << "1. Show all users\n2. Show menu\n3. Show all orders\n4. Show ingredient stock\n5. Add ingredient\n6. Add dish to menu\n0. Exit\nChoice: ";
            cin >> adminChoice;
            if (adminChoice == 1) userManager.showAll();
            else if (adminChoice == 2) menu.showMenu();
            else if (adminChoice == 3) orderManager.showAllOrders();
            else if (adminChoice == 4) stock.showAll();
            else if (adminChoice == 5) {
                string name; int count;
                cout << "Ingredient name: "; cin >> name;
                cout << "Quantity: "; cin >> count;
                stock.addIngredient(name, count);
            }
            else if (adminChoice == 6) {
                string dish;
                cout << "Dish name: ";
                cin.ignore();
                getline(cin, dish);
                menu.addDish(dish);
                cout << "Dish added!\n";
            }
        } while (adminChoice != 0);
    }
};

// --- MAIN ---
int main() {
    UserManager userManager;
    OrderManager orderManager;
    Stock stock; stock.loadFromFile();
    Menu menu;

    if (menu.getMenuSize() == 0) { menu.addDish("Pizza"); menu.addDish("Sandwich"); menu.addDish("Salad"); }
    if (!stock.hasEnough("Pizza", 1)) { stock.addIngredient("Pizza", 10); }
    if (!stock.hasEnough("Sandwich", 1)) { stock.addIngredient("Sandwich", 5); }
    if (!stock.hasEnough("Salad", 1)) { stock.addIngredient("Salad", 7); }

    Admin admin;
    int choice;
    while (true) {
        cout << "\n1. Sign up\n2. Login\n0. Exit\nChoice: ";
        cin >> choice;
        if (choice == 1)
            userManager.signUp();
        else if (choice == 2) {
            if (userManager.signIn()) {
                if (userManager.isActiveAdmin()) {
                    admin.adminPanel(userManager, orderManager, stock, menu);
                }
                else {
                    int uchoice;
                    while (true) {
                        cout << "\n1. View menu\n2. Place order\n3. My orders\n0. Exit\nChoice: ";
                        cin >> uchoice;
                        if (uchoice == 1) menu.showMenu();
                        else if (uchoice == 2) {
                            menu.showMenu();
                            int dish;
                            cout << "Choose dish (number): "; cin >> dish;
                            string chosen = menu.getMenuByIndex(dish - 1);

                            if (chosen != "" && stock.hasEnough(chosen, 1)) {
                                stock.useIngredient(chosen, 1);
                                orderManager.createOrder(userManager.getActive(), chosen);
                            }
                            else {
                                cout << "Not enough in stock or invalid selection!\n";
                            }
                        }
                        else if (uchoice == 3)
                            orderManager.showMyOrders(userManager.getActive());
                        else if (uchoice == 0) break;
                    }
                }
            }
        }
        else if (choice == 0) {
            cout << "Program finished!\n";
            break;
        }
        else cout << "Invalid choice!\n";
    }
    return 0;
}