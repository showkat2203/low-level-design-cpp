#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <ctime>

using namespace std;

// Forward declarations
class Pizza;
class Order;
class Payment;
class Employee;
class Customer;
class Delivery;

// Enumerations
enum class PizzaSize {
    Small,
    Medium,
    Large
};

enum class PaymentStatus {
    Unpaid,
    Completed,
    Failed
};

enum class OrderStatus {
    Placed,
    Preparing,
    ReadyForDelivery,
    Completed,
    Canceled
};

enum class AccountStatus {
    Active,
    Closed,
    Blacklisted
};

// Address class
class Address {
private:
    string streetAddress;
    string city;
    string state;
    string zipCode;

public:
    Address(string streetAddress, string city, string state, string zipCode)
        : streetAddress(streetAddress), city(city), state(state), zipCode(zipCode) {}
};

// Person class
class Person {
protected:
    string name;
    string phone;
    Address address;

public:
    Person(string name, string phone, Address address)
        : name(name), phone(phone), address(address) {}
    virtual ~Person() = default;
};

// Customer class
class Customer : public Person {
public:
    Customer(string name, string phone, Address address)
        : Person(name, phone, address) {}

    void placeOrder(Order& order);
    void makePayment(Payment& payment);
};

// Employee class
class Employee : public Person {
protected:
    int employeeID;
    string hireDate;

public:
    Employee(string name, string phone, Address address, int employeeID, string hireDate)
        : Person(name, phone, address), employeeID(employeeID), hireDate(hireDate) {}
    virtual void performDuty() = 0;
};

// Specialized Employee Classes
class Chef : public Employee {
public:
    Chef(string name, string phone, Address address, int employeeID, string hireDate)
        : Employee(name, phone, address, employeeID, hireDate) {}

    void performDuty() override {
        cout << "Chef is preparing pizzas." << endl;
    }

    void preparePizza(const Pizza& pizza);
};

class Cashier : public Employee {
public:
    Cashier(string name, string phone, Address address, int employeeID, string hireDate)
        : Employee(name, phone, address, employeeID, hireDate) {}

    void performDuty() override {
        cout << "Cashier is processing payments." << endl;
    }

    void processPayment(Payment& payment);
};

class DeliveryDriver : public Employee {
public:
    DeliveryDriver(string name, string phone, Address address, int employeeID, string hireDate)
        : Employee(name, phone, address, employeeID, hireDate) {}

    void performDuty() override {
        cout << "Delivery driver is delivering orders." << endl;
    }

    void deliverOrder(const Delivery& delivery);
};

// Pizza class using Builder Pattern
class Pizza {
private:
    string name;
    PizzaSize size;
    double price;
    vector<string> toppings;
    string crustType;

public:
    Pizza(string name, PizzaSize size, double price, vector<string> toppings, string crustType)
        : name(name), size(size), price(price), toppings(toppings), crustType(crustType) {}

    double getPrice() const { return price; }
    string getName() const { return name; }
    void printDetails() const {
        cout << "Pizza: " << name << ", Size: " << (size == PizzaSize::Small ? "Small" : size == PizzaSize::Medium ? "Medium" : "Large")
             << ", Crust: " << crustType << ", Toppings: ";
        for (const auto& topping : toppings) {
            cout << topping << " ";
        }
        cout << endl;
    }

    // Builder class
    class Builder {
    private:
        string name;
        PizzaSize size;
        double price;
        vector<string> toppings;
        string crustType;

    public:
        Builder(string name, PizzaSize size, double price)
            : name(name), size(size), price(price), crustType("Regular") {}

        Builder& addTopping(const string& topping) {
            toppings.push_back(topping);
            return *this;
        }

        Builder& setCrustType(const string& crust) {
            crustType = crust;
            return *this;
        }

        Pizza build() {
            return Pizza(name, size, price, toppings, crustType);
        }
    };
};

// Order class
class Order {
private:
    int orderID;
    OrderStatus status;
    vector<Pizza> pizzas;
    Customer customer;
    double totalPrice;

public:
    Order(int orderID, Customer customer)
        : orderID(orderID), status(OrderStatus::Placed), customer(customer), totalPrice(0.0) {}

    void addPizza(const Pizza& pizza) {
        pizzas.push_back(pizza);
        totalPrice += pizza.getPrice();
    }

    void setStatus(OrderStatus newStatus) { status = newStatus; }
    OrderStatus getStatus() const { return status; }
    double getTotalPrice() const { return totalPrice; }
    void printOrderDetails() const {
        cout << "Order ID: " << orderID << ", Status: " << (status == OrderStatus::Placed ? "Placed" : status == OrderStatus::Preparing ? "Preparing" : status == OrderStatus::ReadyForDelivery ? "Ready for Delivery" : "Completed") << endl;
        for (const auto& pizza : pizzas) {
            pizza.printDetails();
        }
        cout << "Total Price: $" << totalPrice << endl;
    }
};

// Payment class
class Payment {
protected:
    int paymentID;
    time_t paymentDate;
    double amount;
    PaymentStatus status;

public:
    Payment(int paymentID, double amount)
        : paymentID(paymentID), paymentDate(time(nullptr)), amount(amount), status(PaymentStatus::Unpaid) {}
    virtual ~Payment() = default;
    virtual void processPayment() = 0;
};

// Cash Payment class
class CashPayment : public Payment {
public:
    CashPayment(int paymentID, double amount)
        : Payment(paymentID, amount) {}

    void processPayment() override {
        cout << "Processing cash payment of $" << amount << endl;
        status = PaymentStatus::Completed;
    }
};

// Credit Card Payment class
class CreditCardPayment : public Payment {
private:
    string cardNumber;
    string cardHolderName;

public:
    CreditCardPayment(int paymentID, double amount, string cardNumber, string cardHolderName)
        : Payment(paymentID, amount), cardNumber(cardNumber), cardHolderName(cardHolderName) {}

    void processPayment() override {
        cout << "Processing credit card payment of $" << amount << " for card holder: " << cardHolderName << endl;
        status = PaymentStatus::Completed;
    }
};

// Delivery class
class Delivery {
private:
    int deliveryID;
    Order order;
    string deliveryTime;

public:
    Delivery(int deliveryID, Order order, string deliveryTime)
        : deliveryID(deliveryID), order(order), deliveryTime(deliveryTime) {}

    void startDelivery() {
        cout << "Starting delivery for Order ID: " << deliveryID << endl;
    }
};

// Method Implementations
void Customer::placeOrder(Order& order) {
    cout << "Order placed for customer: " << name << endl;
}

void Customer::makePayment(Payment& payment) {
    payment.processPayment();
}

void Chef::preparePizza(const Pizza& pizza) {
    cout << "Preparing " << pizza.getName() << endl;
}

void Cashier::processPayment(Payment& payment) {
    payment.processPayment();
}

void DeliveryDriver::deliverOrder(const Delivery& delivery) {
    cout << "Delivering order..." << endl;
    delivery.startDelivery();
}

// Main Function
int main() {
    Address address("123 Pizza St", "PizzaCity", "PZ", "12345");
    Customer customer("John Doe", "555-1234", address);
    Order order(1, customer);

    // Building and adding pizzas to the order using Builder Pattern
    Pizza pizza1 = Pizza::Builder("Margherita", PizzaSize::Medium, 12.99)
                        .addTopping("Cheese")
                        .addTopping("Tomato")
                        .setCrustType("Thin Crust")
                        .build();
    Pizza pizza2 = Pizza::Builder("Pepperoni", PizzaSize::Large, 15.99)
                        .addTopping("Pepperoni")
                        .addTopping("Cheese")
                        .build();
    order.addPizza(pizza1);
    order.addPizza(pizza2);

    // Customer places the order
    customer.placeOrder(order);

    // Print order details
    order.printOrderDetails();

    // Chef prepares the pizzas
    Chef chef("Mario Rossi", "555-5678", address, 101, "2022-01-01");
    chef.preparePizza(pizza1);
    chef.preparePizza(pizza2);

    // Cashier processes payment
    Cashier cashier("Jane Smith", "555-7890", address, 102, "2022-01-01");
    CashPayment cashPayment(1, order.getTotalPrice());
    customer.makePayment(cashPayment);

    // Delivery Driver delivers the order
    Delivery delivery(1, order, "2024-10-17 18:00");
    DeliveryDriver driver("Luigi Verde", "555-3456", address, 103, "2022-01-01");
    driver.deliverOrder(delivery);

    return 0;
}
