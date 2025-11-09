#include <iostream>
#include <vector>
#include <string>
using namespace std;

// --- INGREDIENT (for stock) ---
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
                cout << "Ingredient quantity increased!\n";
                return;
            }
        }
        ingredients.push_back(Ingredient(n, q));
        cout << "Ingredient added!\n";
    }
    bool useIngredient(const string& n, int q) {
        for (auto& ing : ingredients) {
            if (ing.getName() == n) return ing.remove(q);
        }
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
};

// --- ORDER MANAGER ---
class OrderManager {
private:
    vector<Order> orders;
public:
    void createOrder(const string& username, const string& dish) {
        orders.push_back(Order(username, dish));
        cout << "✅ Your order has been accepted!\n";
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

// --- ADMIN ---
class Admin {
private:
    vector<string> menuDishes;
public:
    Admin() {
        menuDishes.push_back("Pizza");
        menuDishes.push_back("Sandwich");
        menuDishes.push_back("Salad");
    }

    void adminPanel(const UserManager& userManager, Admin& admin, OrderManager& orderManager, Stock& stock) {
        int adminChoice;
        do {
            cout << "\n--- ADMIN PANEL ---\n";
            cout << "1. All users\n2. View menu\n3. All orders\n4. View ingredient stock\n5. Add ingredient\n0. Exit\nChoice: ";
            cin >> adminChoice;
            if (adminChoice == 1) admin.showAllUsers(userManager.getAll());
            else if (adminChoice == 2) admin.showMenu();
            else if (adminChoice == 3) orderManager.showAllOrders();
            else if (adminChoice == 4) stock.showAll();
            else if (adminChoice == 5) {
                string name; int count;
                cout << "Ingredient name: "; cin >> name;
                cout << "Quantity: "; cin >> count;
                stock.addIngredient(name, count);
            }
        } while (adminChoice != 0);
    }

    void addDish(const string& dish) { menuDishes.push_back(dish); }
    void showMenu() const {
        cout << "\nMENU:\n";
        for (size_t i = 0; i < menuDishes.size(); i++)
            cout << i + 1 << ". " << menuDishes[i] << endl;
    }
    string getMenuByIndex(int idx) const {
        if (idx >= 0 && idx < (int)menuDishes.size())
            return menuDishes[idx];
        return "";
    }
    int getMenuSize() const { return (int)menuDishes.size(); }
    void showAllUsers(const vector<User>& users) const {
        cout << "\nALL USERS:\n";
        for (const auto& u : users)
            cout << u.getUsername() << " (" << u.getEmail() << ")\n";
        if (users.empty()) cout << "(No users)\n";
    }
};

// --- USER MANAGER ---
class UserManager {
private:
    vector<User> users;
    string activeUser;
    bool activeUserIsAdmin = false;
public:
    UserManager() {
        users.push_back(User("admin", "admin", "admin@restaurant.com", true));
    }
    void signUp() {
        string u, p, e;
        cout << "Username: "; cin >> u;
        cout << "Password: "; cin >> p;
        cout << "Email: "; cin >> e;

        for (const auto& user : users) {
            if (user.getUsername() == u) {
                cout << "Username already exists!\n";
                return;
            }
        }
        users.push_back(User(u, p, e, false));
        cout << "Registration complete!\n";
    }
    bool signIn() {
        string u, p;
        cout << "Username: "; cin >> u;
        cout << "Password: "; cin >> p;
        for (const auto& user : users)
            if (user.getUsername() == u && user.getPassword() == p) {
                activeUser = u;
                activeUserIsAdmin = user.isAdmin();
                cout << "Logged in!\n";
                return true;
            }
        cout << "Username or password is incorrect!\n";
        return false;
    }
    string getActive() const { return activeUser; }
    bool isActiveAdmin() const { return activeUserIsAdmin; }
    const vector<User>& getAll() const { return users; }
};

int main() {
    UserManager userManager;
    OrderManager orderManager;
    Admin admin;
    Stock stock;

    stock.addIngredient("Pizza", 10);
    stock.addIngredient("Sandwich", 5);
    stock.addIngredient("Salad", 7);

    int choice;
    while (true) {
        cout << "\n1. Sign up\n2. Login\n0. Exit\nChoice: ";
        cin >> choice;
        if (choice == 1)
            userManager.signUp();
        else if (choice == 2) {
            if (userManager.signIn()) {
                if (userManager.isActiveAdmin()) {
                    adminPanel(userManager, admin, orderManager, stock);
                }
                else {
                    int uchoice;
                    while (true) {
                        cout << "\n1. View menu\n2. Place order\n3. My orders\n0. Exit\nChoice: ";
                        cin >> uchoice;
                        if (uchoice == 1) admin.showMenu();
                        else if (uchoice == 2) {
                            admin.showMenu();
                            int dish;
                            cout << "Choose dish (number): "; cin >> dish;
                            string chosen = admin.getMenuByIndex(dish - 1);

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