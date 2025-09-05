#include <iostream>
#include <string>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

using namespace std;

sql::Connection* connectDB() {
    sql::Driver* driver = get_driver_instance();
    sql::Connection* con = driver->connect("tcp://127.0.0.1:3306", "root", "Hello@2511");
    con->setSchema("cpp_pharmacymanagementsystem");
    cout << "Connected to database successfully!\n";
    return con;
}

void addMedicine(sql::Connection* conn) {
    string name, company, arrival, expiry;
    float price;
    int quantity;

    cout << "Enter Medicine Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Company: ";
    getline(cin, company);
    cout << "Enter Arrival Date (YYYY-MM-DD): ";
    cin >> arrival;
    cout << "Enter Expiry Date (YYYY-MM-DD): ";
    cin >> expiry;
    cout << "Enter Price: ";
    cin >> price;
    cout << "Enter Quantity: ";
    cin >> quantity;

    try {
        unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "INSERT INTO medicine_tb (name, company, arrival_date, expire_date, price, quantity) "
                "VALUES (?, ?, ?, ?, ?, ?)"
            )
        );

        pstmt->setString(1, name);
        pstmt->setString(2, company);
        pstmt->setString(3, arrival);
        pstmt->setString(4, expiry);
        pstmt->setDouble(5, price);
        pstmt->setInt(6, quantity);

        pstmt->executeUpdate();
        cout << "Medicine added successfully!" << endl;

    }
    catch (sql::SQLException& e) {
        cerr << "Error: " << e.what() << endl;
        cerr << "MySQL Error Code: " << e.getErrorCode() << endl;
        cerr << "SQL State: " << e.getSQLState() << endl;
    }
}


void viewMedicines(sql::Connection* con) {
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM medicine_tb");

    cout << "\n=== Medicine List ===\n";
    while (res->next()) {
        cout << "ID: " << res->getInt("id")
            << " | Name: " << res->getString("name")
            << " | Company: " << res->getString("company")
            << " | Arrival: " << res->getString("arrival_date")
            << " | Expire: " << res->getString("expire_date")
            << " | Price: " << res->getDouble("price")
            << " | Qty: " << res->getInt("quantity") << endl;
    }
    delete res;
    delete stmt;
}

void updateMedicine(sql::Connection* con) {
    int id, qty;
    double price;

    cout << "Enter Medicine ID to update: ";
    cin >> id;
    cout << "Enter new Price: ";
    cin >> price;
    cout << "Enter new Quantity: ";
    cin >> qty;

    sql::PreparedStatement* pstmt = con->prepareStatement(
        "UPDATE medicine_tb SET price=?, quantity=? WHERE id=?"
    );
    pstmt->setDouble(1, price);
    pstmt->setInt(2, qty);
    pstmt->setInt(3, id);

    int rows = pstmt->executeUpdate();
    if (rows > 0)
        cout << "Medicine updated successfully!\n";
    else
        cout << "Medicine ID not found.\n";

    delete pstmt;
}

void deleteMedicine(sql::Connection* con) {
    int id;
    cout << "Enter Medicine ID to delete: ";
    cin >> id;

    sql::PreparedStatement* pstmt = con->prepareStatement("DELETE FROM medicine_tb WHERE id=?");
    pstmt->setInt(1, id);

    int rows = pstmt->executeUpdate();
    if (rows > 0)
        cout << "Medicine deleted successfully!\n";
    else
        cout << "Medicine ID not found.\n";

    delete pstmt;
}

void searchMedicine(sql::Connection* con) {
    string keyword;
    cout << "Enter medicine name or company to search: ";
    cin >> ws; getline(cin, keyword);

    sql::PreparedStatement* pstmt = con->prepareStatement(
        "SELECT * FROM medicine_tb WHERE name LIKE ? OR company LIKE ?"
    );
    string likeStr = "%" + keyword + "%";
    pstmt->setString(1, likeStr);
    pstmt->setString(2, likeStr);

    sql::ResultSet* res = pstmt->executeQuery();

    cout << "\n=== Search Results ===\n";
    bool found = false;
    while (res->next()) {
        cout << "ID: " << res->getInt("id")
            << " | Name: " << res->getString("name")
            << " | Company: " << res->getString("company")
            << " | Arrival: " << res->getString("arrival_date")
            << " | Expire: " << res->getString("expire_date")
            << " | Price: " << res->getDouble("price")
            << " | Qty: " << res->getInt("quantity") << endl;
        found = true;
    }

    if (!found)
        cout << "No matching medicines found.\n";

    delete res;
    delete pstmt;
}

int main() {
    try {
        sql::Connection* con = connectDB();
        int choice;

        do {
            cout << "\n=== Pharmacy Management System ===" << endl;
            cout << "1. Add Medicine" << endl;
            cout << "2. View Medicines" << endl;
            cout << "3. Update Medicine" << endl;
            cout << "4. Delete Medicine" << endl;
            cout << "5. Search Medicine" << endl;
            cout << "6. Exit" << endl;
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
            case 1: addMedicine(con); break;
            case 2: viewMedicines(con); break;
            case 3: updateMedicine(con); break;
            case 4: deleteMedicine(con); break;
            case 5: searchMedicine(con); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n"; break;
            }
        } while (choice != 6);

        delete con;
    }
    catch (sql::SQLException& e) {
        cout << "Error: " << e.what() << endl;
        cout << "MySQL Error Code: " << e.getErrorCode() << endl;
        cout << "SQL State: " << e.getSQLState() << endl;
    }

    return 0;
}
