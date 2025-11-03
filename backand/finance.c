#include <stdio.h>  // Yeh header include kiya gaya hai standard input/output functions jaise printf aur scanf ka istemal karne ke liye.
#include <stdlib.h>  // Yeh header include kiya gaya hai general utilities ke liye, yahan zyada istemal nahi hota lekin C programs mein standard hai.
#include <string.h>  // Yeh header include kiya gaya hai string manipulation functions jaise strcpy aur strcmp ka istemal karne ke liye.

// Define the maximum number of transactions, the filename, and the password
#define MAX_TRANSACTIONS 100  // Yeh macro maximum number of transactions ko define karta hai jo array mein store ho sakti hain.
#define FILENAME "transactions.txt"  // Yeh macro file ka naam define karta hai jahan transactions save aur load hote hain.
#define ADMIN_PASSWORD "meerab26"  // Yeh macro admin password ko define karta hai jo login ke liye zaroori hai.

// 1. Structure Definition for Transaction
typedef struct {  // Yeh keyword ek naya data type define karta hai jo related data ko group karta hai.
    char description[50];  // Yeh array transaction ki description store karta hai, limited to 49 characters plus null terminator.
    char type[10];  // Yeh array transaction ka type store karta hai, ya to "Income" ya "Expense".
    float amount;  // Yeh variable transaction ki monetary amount store karta hai.
} Transaction;  // Yeh struct definition ko close karta hai aur usko 'Transaction' naam deta hai.

// Global array and counter to store transactions
Transaction transactions[MAX_TRANSACTIONS];  // Yeh global array Transaction structs ka hai jo MAX_TRANSACTIONS tak entries hold karta hai.
int transactionCount = 0;  // Yeh global variable current number of transactions ko track karta hai jo store hain.

// --- Function Prototypes ---
void displayMenu();  // Yeh prototype displayMenu function ko declare karta hai, jo main menu options dikhata hai.
void loadTransactions();  // Yeh prototype loadTransactions function ko declare karta hai, jo file se transactions padhta hai.
void saveTransactions();  // Yeh prototype saveTransactions function ko declare karta hai, jo file mein transactions likhta hai.
void addTransaction();  // Yeh prototype addTransaction function ko declare karta hai, jo naya transaction add karne deta hai.
void viewTransactions();  // Yeh prototype viewTransactions function ko declare karta hai, jo saare transactions dikhata hai.
void calculateAndDisplayStats();  // Yeh prototype calculateAndDisplayStats function ko declare karta hai, jo financial stats calculate aur dikhata hai.
int login();  // Yeh prototype login function ko declare karta hai, jo password authentication handle karta hai.

// --- Main Function ---
int main() {  // Yeh program ka entry point hai jahan execution shuru hota hai.
    int choice;  // Yeh variable user ki menu choice store karta hai.
    
    printf("==========================================\n");  // Yeh program title header ko console par print karta hai.
    printf("   Simple Finance Tracker (C Language)   \n");  // Yeh program ka subtitle print karta hai.
    printf("==========================================\n");  // Yeh header ki closing line print karta hai.

    // --- NEW: Run the login check ---
    if (!login()) {  // Yeh login function ko call karta hai aur check karta hai ke woh false (0) return karta hai, matlab login fail hua.
        printf("\nAuthentication Failed. Exiting program.\n");  // Yeh failure message print karta hai agar login unsuccessful tha.
        return 0;  // Yeh program ko exit karta hai status code 0 ke saath agar authentication fail ho.
    }
    // ---------------------------------
    
    // Load existing transactions only after successful login
    loadTransactions();  // Yeh function ko call karta hai jo previously saved transactions ko file se load karta hai.

    // Main program loop
    do {  // Yeh do-while loop shuru karta hai jo tab tak continue karta hai jab tak user exit na kare.
        displayMenu();  // Yeh function ko call karta hai jo main menu options dikhata hai.
        printf("Enter your choice (1-4): ");  // Yeh user ko unki menu choice enter karne ke liye prompt karta hai.
        
        // Input validation for integer
        if (scanf("%d", &choice) != 1) {  // Yeh input se integer padhne ki koshish karta hai aur check karta hai ke woh succeed hua ya nahi.
            printf("\nInvalid input. Please enter a number.\n");  // Yeh error message print karta hai agar input valid integer nahi tha.
            while (getchar() != '\n');  // Yeh input buffer mein remaining characters ko clear karta hai.
            choice = 0;  // Yeh choice variable ko 0 par reset karta hai taake invalid execution na ho.
            continue;  // Yeh loop ke baaki part ko skip karta hai aur next iteration shuru karta hai.
        }
        
        while (getchar() != '\n');  // Yeh integer padhne ke baad input buffer ko clear karta hai.

        switch (choice) {  // Yeh switch statement shuru karta hai jo 'choice' ki value ke base par different menu choices handle karta hai.
            case 1:  // Yeh case execute hota hai agar user ne 1 choose kiya (Add Transaction).
                addTransaction();  // Yeh function ko call karta hai jo naya transaction add karta hai.
                break;  // Yeh switch statement ko exit karta hai case execute hone ke baad.
            case 2:  // Yeh case execute hota hai agar user ne 2 choose kiya (View Transactions).
                viewTransactions();  // Yeh function ko call karta hai jo saare transactions dikhata hai.
                break;  // Yeh switch statement ko exit karta hai case execute hone ke baad.
            case 3:  // Yeh case execute hota hai agar user ne 3 choose kiya (View Stats).
                calculateAndDisplayStats();  // Yeh function ko call karta hai jo financial statistics calculate aur dikhata hai.
                break;  // Yeh switch statement ko exit karta hai case execute hone ke baad.
            case 4:  // Yeh case execute hota hai agar user ne 4 choose kiya (Exit).
                saveTransactions();  // Yeh function ko call karta hai jo saare transactions ko file mein save karta hai exit se pehle.
                printf("\nExiting Finance Tracker. Data saved successfully. Goodbye!\n");  // Yeh exit message print karta hai.
                break;  // Yeh switch statement ko exit karta hai case execute hone ke baad.
            default:  // Yeh case execute hota hai agar choice 1-4 nahi hai.
                printf("\nInvalid choice. Please select from 1 to 4.\n");  // Yeh invalid choices ke liye error message print karta hai.
        }
    } while (choice != 4);  // Yeh condition loop ko tab tak run karti hai jab tak choice 4 (exit) nahi hota.

    return 0;  // Yeh 0 return karta hai successful program termination ka indicate karne ke liye.
}

// -----------------------------------
// --- NEW LOGIN FUNCTION DEFINITION ---
// -----------------------------------
/**
 * @brief Handles the admin password check.
 * @return 1 on success, 0 on failure.
 */
int login() {  // Yeh login function define karta hai jo admin password check karta hai.
    char inputPassword[50];  // Yeh array user ke dwara entered password ko store karta hai.
    int attempts = 3;  // Yeh variable remaining login attempts ko track karta hai.
    
    printf("\n--- Admin Login Required ---\n");  // Yeh login prompt header print karta hai.

    while (attempts > 0) {  // Yeh loop tab tak continue karta hai jab tak attempts bache hain.
        printf("Enter Admin Password (Attempts left: %d): ", attempts);  // Yeh user ko password enter karne ke liye prompt karta hai aur remaining attempts dikhata hai.
        
        // Read the password input
        fgets(inputPassword, sizeof(inputPassword), stdin);  // Yeh user se input ki line padhta hai aur inputPassword array mein store karta hai.
        
        // Remove trailing newline character
        inputPassword[strcspn(inputPassword, "\n")] = 0;  // Yeh newline character ko find aur remove karta hai jo fgets ne add kiya hai.

        // Compare the input with the defined password
        if (strcmp(inputPassword, ADMIN_PASSWORD) == 0) {  // Yeh entered password ko predefined admin password se compare karta hai.
            printf("\nLogin successful! Welcome.\n");  // Yeh success message print karta hai agar passwords match hote hain.
            return 1;  // Yeh 1 return karta hai successful login ka indicate karne ke liye.
        } else {  // Yeh execute hota hai agar passwords match nahi hote.
            attempts--;  // Yeh attempts counter ko decrement karta hai.
            printf("Invalid password. Please try again.\n");  // Yeh error message print karta hai aur retry ke liye prompt karta hai.
        }
    }
    
    return 0;  // Yeh 0 return karta hai agar saare attempts khatam ho gaye, matlab login failure.
}
// -----------------------------------

// --- Other Function Definitions (Same as before) ---

/**
 * @brief Displays the main menu options.
 */
void displayMenu() {  // Yeh displayMenu function define karta hai jo menu options dikhata hai.
    printf("\n--- Main Menu ---\n");  // Yeh menu header print karta hai.
    printf("1. Add New Transaction\n");  // Yeh option 1 print karta hai.
    printf("2. View All Transactions\n");  // Yeh option 2 print karta hai.
    printf("3. View Financial Summary (Stats)\n");  // Yeh option 3 print karta hai.
    printf("4. Exit and Save Data\n");  // Yeh option 4 print karta hai.
    printf("-----------------\n");  // Yeh separator line print karta hai.
}

/**
 * @brief Loads transactions from the file (transactions.txt).
 */
void loadTransactions() {  // Yeh loadTransactions function define karta hai jo file se data padhta hai.
    FILE *file = fopen(FILENAME, "r");  // Yeh file ko read mode mein open karta hai aur file pointer ko 'file' mein assign karta hai.
    
    if (file == NULL) {  // Yeh check karta hai ke file open nahi ho saki (e.g., exist nahi karta).
        printf("Note: Data file not found. Starting with a fresh slate.\n");  // Yeh note print karta hai agar file missing hai.
        return;  // Yeh function ko early exit karta hai agar file nahi mili.
    }
    
    // Read transactions from the file line by line until EOF
    while (transactionCount < MAX_TRANSACTIONS &&  // Yeh loop tab tak continue karta hai jab tak slots bache hain aur data successfully padha jaye.
           fscanf(file, "%49[^\t]\t%9[^\t]\t%f\n",  // Yeh file se formatted data padhta hai: description, type, aur amount.
                  transactions[transactionCount].description,  // Yeh description ko current transaction slot mein store karta hai.
                  transactions[transactionCount].type,  // Yeh type ko current transaction slot mein store karta hai.
                  &transactions[transactionCount].amount) == 3) {  // Yeh amount store karta hai aur check karta hai ke teeno fields padhe gaye.
        transactionCount++;  // Yeh transaction count ko increment karta hai successfully padhne ke baad.
    }
    
    fclose(file);  // Yeh file ko close karta hai padhne ke baad.
    printf("Loaded %d previous transactions.\n", transactionCount);  // Yeh loaded transactions ki number print karta hai.
}

/**
 * @brief Saves all current transactions to the file (transactions.txt).
 */
void saveTransactions() {  // Yeh saveTransactions function define karta hai jo file mein data likhta hai.
    FILE *file = fopen(FILENAME, "w");  // Yeh file ko write mode mein open karta hai (existing content overwrite karta hai) aur pointer ko 'file' mein assign karta hai.
    
    if (file == NULL) {  // Yeh check karta hai ke file writing ke liye open nahi ho saki.
        printf("Error: Could not open file for saving!\n");  // Yeh error message print karta hai agar file open nahi ho saki.
        return;  // Yeh function ko exit karta hai agar saving fail ho.
    }
    
    // Write each transaction to the file, separating fields with a tab (\t)
    for (int i = 0; i < transactionCount; i++) {  // Yeh loop saare stored transactions ke through iterate karta hai.
        fprintf(file, "%s\t%s\t%.2f\n",  // Yeh description, type, aur amount ko file mein tabs aur newline ke saath likhta hai.
                transactions[i].description,  // Yeh i-th transaction ki description likhta hai.
                transactions[i].type,  // Yeh i-th transaction ka type likhta hai.
                transactions[i].amount);  // Yeh i-th transaction ka amount likhta hai.
    }
    
    fclose(file);  // Yeh file ko close karta hai writing ke baad.
}

/**
 * @brief Handles user input to add a new transaction (Income or Expense).
 */
void addTransaction() {  // Yeh addTransaction function define karta hai jo naya entry add karta hai.
    if (transactionCount >= MAX_TRANSACTIONS) {  // Yeh check karta hai ke maximum number of transactions reach ho gaye hain.
        printf("\nError: Maximum transactions limit reached.\n");  // Yeh error print karta hai agar limit hit ho.
        return;  // Yeh function ko exit karta hai bina aur add kiye.
    }

    Transaction newTransaction;  // Yeh naya Transaction struct declare karta hai input ke liye.
    int typeChoice;  // Yeh variable user ki choice ko store karta hai transaction type ke liye.
    float amount;  // Yeh variable temporarily amount ko hold karta hai jo enter kiya gaya.

    printf("\n--- Add New Transaction ---\n");  // Yeh add transaction header print karta hai.
    
    // Get Transaction Type
    printf("Select Type:\n");  // Yeh type selection prompt print karta hai.
    printf("  1. Income\n");  // Yeh option 1 print karta hai Income ke liye.
    printf("  2. Expense\n");  // Yeh option 2 print karta hai Expense ke liye.
    printf("Enter type (1 or 2): ");  // Yeh type input ke liye prompt karta hai.
    
    if (scanf("%d", &typeChoice) != 1 || (typeChoice != 1 && typeChoice != 2)) {  // Yeh type choice padhta hai aur validate karta hai.
        printf("Invalid type choice.\n");  // Yeh error print karta hai agar choice invalid hai.
        while (getchar() != '\n');  // Yeh input buffer ko clear karta hai.
        return;  // Yeh function ko exit karta hai invalid input par.
    }
    while (getchar() != '\n');  // Yeh padhne ke baad buffer ko clear karta hai.

    // Set type string
    if (typeChoice == 1) {  // Yeh check karta hai ke user ne Income choose kiya.
        strcpy(newTransaction.type, "Income");  // Yeh "Income" ko type field mein copy karta hai.
    } else {  // Yeh execute hota hai agar Expense choose kiya gaya.
        strcpy(newTransaction.type, "Expense");  // Yeh "Expense" ko type field mein copy karta hai.
    }

    // Get Description
    printf("Enter Description (max 49 chars): ");  // Yeh description ke liye prompt karta hai.
    fgets(newTransaction.description, sizeof(newTransaction.description), stdin);  // Yeh description input padhta hai.
    // Remove newline character added by fgets
    newTransaction.description[strcspn(newTransaction.description, "\n")] = 0;  // Yeh description se trailing newline ko remove karta hai.

    // Get Amount
    printf("Enter Amount (Rs): ");  // Yeh amount ke liye prompt karta hai.
    if (scanf("%f", &amount) != 1 || amount <= 0) {  // Yeh amount padhta hai aur validate karta hai (positive hona chahiye).
        printf("Invalid amount. Must be a positive number.\n");  // Yeh invalid amount ke liye error print karta hai.
        while (getchar() != '\n');  // Yeh buffer ko clear karta hai.
        return;  // Yeh invalid amount par exit karta hai.
    }
    newTransaction.amount = amount;  // Yeh valid amount ko transaction mein assign karta hai.

    // Add to array
    transactions[transactionCount] = newTransaction;  // Yeh naya transaction ko global array mein copy karta hai.
    transactionCount++;  // Yeh transaction count ko increment karta hai.

    printf("\nTransaction added successfully: %s - Rs %.2f (%s)\n",  // Yeh success message print karta hai details ke saath.
           newTransaction.description, newTransaction.amount, newTransaction.type);
}

/**
 * @brief Displays all recorded transactions in a formatted list.
 */
void viewTransactions() {  // Yeh viewTransactions function define karta hai jo saare entries dikhata hai.
    if (transactionCount == 0) {  // Yeh check karta hai ke koi transactions nahi hain.
        printf("\nNo transactions recorded yet.\n");  // Yeh message print karta hai agar list empty hai.
        return;  // Yeh function ko exit karta hai.
    }

    printf("\n--- All Transactions ---\n");  // Yeh view header print karta hai.
    printf("%-5s | %-10s | %-30s | %s\n", "No.", "Type", "Description", "Amount (Rs)");  // Yeh table header print karta hai.
    printf("----------------------------------------------------------------\n");  // Yeh separator line print karta hai.
    
    for (int i = 0; i < transactionCount; i++) {  // Yeh loop saare transactions ke through iterate karta hai.
        printf("%-5d | %-10s | %-30s | %.2f\n",  // Yeh har transaction ko formatted columns mein print karta hai.
               i + 1,  // Yeh transaction number print karta hai (1-based).
               transactions[i].type,  // Yeh type print karta hai.
               transactions[i].description,  // Yeh description print karta hai.
               transactions[i].amount);  // Yeh amount print karta hai.
    }
    printf("----------------------------------------------------------------\n");  // Yeh closing separator print karta hai.
}

/**
 * @brief Calculates and displays the total income, total expense, and current balance.
 */
void calculateAndDisplayStats() {  // Yeh calculateAndDisplayStats function define karta hai jo financial summary dikhata hai.
    float totalIncome = 0.0;  // Yeh variable total income ko accumulate karta hai.
    float totalExpense = 0.0;  // Yeh variable total expense ko accumulate karta hai.
    
    if (transactionCount == 0) {  // Yeh check karta hai ke koi transactions nahi hain.
        printf("\nNo transactions recorded to calculate stats.\n");  // Yeh message print karta hai agar koi data nahi hai.
        return;  // Yeh function ko exit karta hai.
    }

    // Calculate totals
    for (int i = 0; i < transactionCount; i++) {  // Yeh loop saare transactions ke through iterate karta hai.
        if (strcmp(transactions[i].type, "Income") == 0) {  // Yeh check karta hai ke transaction Income hai.
            totalIncome += transactions[i].amount;  // Yeh amount ko total income mein add karta hai.
        } else {  // Yeh Expense transactions ke liye execute hota hai.
            totalExpense += transactions[i].amount;  // Yeh amount ko total expense mein add karta hai.
        }
    }

    float balance = totalIncome - totalExpense;  // Yeh net balance calculate karta hai.

    printf("\n--- Financial Summary ---\n");  // Yeh summary header print karta hai.
    printf("Total Income:    Rs %.2f\n", totalIncome);  // Yeh total income print karta hai.
    printf("Total Expense:   Rs %.2f\n", totalExpense);  // Yeh total expense print karta hai.
    printf("---------------------------\n");  // Yeh separator print karta hai.
    printf("Current Balance: Rs %.2f\n", balance);  // Yeh balance print karta hai.
    printf("---------------------------\n");  // Yeh closing separator print karta hai.
}
