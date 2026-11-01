#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 500

typedef struct {
    int id;
    char type[20];
    char category[30];
    float amount;
    char date[20];
    char note[100];
} Transaction;

Transaction transactions[MAX];
int count = 0;

/* ---------- DESIGN FUNCTIONS ---------- */

void line() {
    printf("============================================================\n");
}

void header(char title[]) {
    printf("\n");
    line();
    printf("                 SMARTSPEND v3.0\n");
    printf("              PERSONAL FINANCE MANAGER\n");
    line();
    printf("  %s\n", title);
    line();
}

/* ---------- FILE HANDLING ---------- */

void saveData() {
    FILE *fp = fopen("transactions.dat", "wb");

    if (fp == NULL) {
        printf("\nUnable to save data!\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, fp);
    fwrite(transactions, sizeof(Transaction), count, fp);

    fclose(fp);
}

void loadData() {
    FILE *fp = fopen("transactions.dat", "rb");

    if (fp == NULL)
        return;

    if (fread(&count, sizeof(int), 1, fp) != 1 ||
        count < 0 || count > MAX) {
        count = 0;
        fclose(fp);
        return;
    }

    if (fread(transactions, sizeof(Transaction), count, fp) != count)
        count = 0;

    fclose(fp);
}

/* ---------- ADD TRANSACTION ---------- */

void addTransaction() {
    Transaction t;
    int choice;

    if (count >= MAX) {
        printf("\nTransaction limit reached!\n");
        return;
    }

    header("ADD NEW TRANSACTION");

    printf("\n  1. Income");
    printf("\n  2. Expense");

    printf("\n\n  Select Type: ");

    if (scanf("%d", &choice) != 1) {
        printf("\n  Invalid input!\n");
        while (getchar() != '\n');
        return;
    }

    if (choice == 1)
        strcpy(t.type, "Income");
    else if (choice == 2)
        strcpy(t.type, "Expense");
    else {
        printf("\n  Invalid choice!\n");
        return;
    }

    printf("\n  Category       : ");
    scanf(" %29[^\n]", t.category);

    printf("  Amount (Rs.)   : ");

    if (scanf("%f", &t.amount) != 1 || t.amount <= 0) {
        printf("\n  Invalid amount!\n");
        while (getchar() != '\n');
        return;
    }

    printf("  Date           : ");
    scanf(" %19s", t.date);

    printf("  Note           : ");
    scanf(" %99[^\n]", t.note);

    t.id = count + 1;

    transactions[count] = t;
    count++;

    saveData();

    printf("\n");
    line();
    printf("       TRANSACTION ADDED SUCCESSFULLY!\n");
    line();
}

/* ---------- VIEW TRANSACTIONS ---------- */

void viewTransactions() {
    int i;

    header("TRANSACTION HISTORY");

    if (count == 0) {
        printf("\n  No transactions available.\n");
        return;
    }

    printf("\n");
    printf("------------------------------------------------------------\n");
    printf("%-5s %-10s %-15s %-10s %-12s\n",
           "ID", "TYPE", "CATEGORY", "AMOUNT", "DATE");
    printf("------------------------------------------------------------\n");

    for (i = 0; i < count; i++) {
        printf("%-5d %-10s %-15.15s Rs.%-7.2f %-12s\n",
               transactions[i].id,
               transactions[i].type,
               transactions[i].category,
               transactions[i].amount,
               transactions[i].date);

        printf("      Note: %s\n", transactions[i].note);
    }

    printf("------------------------------------------------------------\n");
    printf("  Total Transactions: %d\n", count);
}

/* ---------- FINANCIAL DASHBOARD ---------- */

void showSummary() {
    float income = 0, expense = 0, balance;
    int i;

    for (i = 0; i < count; i++) {
        if (strcmp(transactions[i].type, "Income") == 0)
            income += transactions[i].amount;
        else
            expense += transactions[i].amount;
    }

    balance = income - expense;

    header("FINANCIAL DASHBOARD");

    printf("\n");
    printf("  +-------------------+-------------------+\n");
    printf("  |   TOTAL INCOME    |  TOTAL EXPENSES   |\n");
    printf("  +-------------------+-------------------+\n");
    printf("  | Rs. %-13.2f | Rs. %-13.2f |\n",
           income, expense);
    printf("  +-------------------+-------------------+\n");

    printf("\n");
    printf("  +---------------------------------------+\n");
    printf("  |            AVAILABLE BALANCE          |\n");
    printf("  +---------------------------------------+\n");
    printf("  |              Rs. %-17.2f|\n", balance);
    printf("  +---------------------------------------+\n");

    printf("\n  Total Transactions: %d\n", count);

    if (balance < 0)
        printf("  Status: Expenses exceed income.\n");
    else
        printf("  Status: Balance is non-negative.\n");
}

/* ---------- SEARCH TRANSACTION ---------- */

void searchTransaction() {
    int id, i;

    header("SEARCH TRANSACTION");

    printf("\n  Enter Transaction ID: ");

    if (scanf("%d", &id) != 1) {
        printf("\n  Invalid input!\n");
        while (getchar() != '\n');
        return;
    }

    for (i = 0; i < count; i++) {
        if (transactions[i].id == id) {

            printf("\n");
            line();
            printf("             TRANSACTION FOUND\n");
            line();

            printf("  ID       : %d\n", transactions[i].id);
            printf("  Type     : %s\n", transactions[i].type);
            printf("  Category : %s\n", transactions[i].category);
            printf("  Amount   : Rs. %.2f\n", transactions[i].amount);
            printf("  Date     : %s\n", transactions[i].date);
            printf("  Note     : %s\n", transactions[i].note);

            line();
            return;
        }
    }

    printf("\n  Transaction not found!\n");
}

/* ---------- EDIT TRANSACTION ---------- */

void editTransaction() {
    int id, i, choice;

    header("EDIT TRANSACTION");

    printf("\n  Enter Transaction ID: ");

    if (scanf("%d", &id) != 1) {
        printf("\n  Invalid input!\n");
        while (getchar() != '\n');
        return;
    }

    for (i = 0; i < count; i++) {
        if (transactions[i].id == id) {

            printf("\n  1. Income");
            printf("\n  2. Expense");
            printf("\n  Select New Type: ");

            if (scanf("%d", &choice) != 1) {
                printf("\n  Invalid input!\n");
                while (getchar() != '\n');
                return;
            }

            if (choice == 1)
                strcpy(transactions[i].type, "Income");
            else if (choice == 2)
                strcpy(transactions[i].type, "Expense");
            else {
                printf("\n  Invalid choice!\n");
                return;
            }

            printf("\n  New Category: ");
            scanf(" %29[^\n]", transactions[i].category);

            printf("  New Amount: ");

            if (scanf("%f", &transactions[i].amount) != 1 ||
                transactions[i].amount <= 0) {
                printf("\n  Invalid amount!\n");
                while (getchar() != '\n');
                return;
            }

            printf("  New Date: ");
            scanf(" %19s", transactions[i].date);

            printf("  New Note: ");
            scanf(" %99[^\n]", transactions[i].note);

            saveData();

            printf("\n  Transaction updated successfully!\n");
            return;
        }
    }

    printf("\n  Transaction not found!\n");
}

/* ---------- DELETE TRANSACTION ---------- */

void deleteTransaction() {
    int id, i, j;

    header("DELETE TRANSACTION");

    printf("\n  Enter Transaction ID: ");

    if (scanf("%d", &id) != 1) {
        printf("\n  Invalid input!\n");
        while (getchar() != '\n');
        return;
    }

    for (i = 0; i < count; i++) {
        if (transactions[i].id == id) {

            for (j = i; j < count - 1; j++)
                transactions[j] = transactions[j + 1];

            count--;

            for (j = 0; j < count; j++)
                transactions[j].id = j + 1;

            saveData();

            printf("\n  Transaction deleted successfully!\n");
            return;
        }
    }

    printf("\n  Transaction not found!\n");
}

/* ---------- MAIN MENU ---------- */

int main() {
    int choice;

    loadData();

    do {
        header("YOUR PERSONAL FINANCE DASHBOARD");

        printf("\n  1.  Add Transaction");
        printf("\n  2.  View Transactions");
        printf("\n  3.  Financial Dashboard");
        printf("\n  4.  Search Transaction");
        printf("\n  5.  Edit Transaction");
        printf("\n  6.  Delete Transaction");
        printf("\n  7.  Exit");

        printf("\n\n");
        line();
        printf("  Enter Your Choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\n  Invalid input! Try again.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {

            case 1:
                addTransaction();
                break;

            case 2:
                viewTransactions();
                break;

            case 3:
                showSummary();
                break;

            case 4:
                searchTransaction();
                break;

            case 5:
                editTransaction();
                break;

            case 6:
                deleteTransaction();
                break;

            case 7:
                saveData();
                header("SESSION COMPLETED");
                printf("\n  Thank you for using SmartSpend!\n");
                printf("  Manage your money. Manage your future.\n\n");
                break;

            default:
                printf("\n  Invalid choice! Please try again.\n");
        }

    } while (choice != 7);

    return 0;
}
