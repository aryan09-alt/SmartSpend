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

void saveData() {
    FILE *fp = fopen("transactions.dat", "wb");

    if (fp == NULL) {
        printf("\nError saving data!\n");
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

void addTransaction() {
    if (count >= MAX) {
        printf("\nTransaction limit reached!\n");
        return;
    }

    Transaction t;
    int choice;

    printf("\n1. Income\n2. Expense\n");
    printf("Choose type: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }

    if (choice == 1)
        strcpy(t.type, "Income");
    else if (choice == 2)
        strcpy(t.type, "Expense");
    else {
        printf("Invalid choice!\n");
        return;
    }

    printf("Enter category: ");
    scanf(" %29[^\n]", t.category);

    printf("Enter amount: ");

    if (scanf("%f", &t.amount) != 1 || t.amount <= 0) {
        printf("Invalid amount!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter date (DD-MM-YYYY): ");
    scanf(" %19s", t.date);

    printf("Enter note: ");
    scanf(" %99[^\n]", t.note);

    t.id = count + 1;

    transactions[count] = t;
    count++;

    saveData();

    printf("\nTransaction added successfully!\n");
}

void viewTransactions() {
    int i;

    if (count == 0) {
        printf("\nNo transactions found!\n");
        return;
    }

    printf("\n========== TRANSACTION HISTORY ==========\n");

    for (i = 0; i < count; i++) {
        printf("\nID: %d\n", transactions[i].id);
        printf("Type: %s\n", transactions[i].type);
        printf("Category: %s\n", transactions[i].category);
        printf("Amount: Rs. %.2f\n", transactions[i].amount);
        printf("Date: %s\n", transactions[i].date);
        printf("Note: %s\n", transactions[i].note);
        printf("----------------------------------------\n");
    }
}

void showSummary() {
    float income = 0, expense = 0;
    int i;

    for (i = 0; i < count; i++) {
        if (strcmp(transactions[i].type, "Income") == 0)
            income += transactions[i].amount;
        else
            expense += transactions[i].amount;
    }

    printf("\n========== FINANCIAL SUMMARY ==========\n");
    printf("Total Income   : Rs. %.2f\n", income);
    printf("Total Expenses : Rs. %.2f\n", expense);
    printf("Balance        : Rs. %.2f\n", income - expense);
}

void searchTransaction() {
    int id, i, found = 0;

    printf("\nEnter Transaction ID: ");

    if (scanf("%d", &id) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }

    for (i = 0; i < count; i++) {
        if (transactions[i].id == id) {
            printf("\nTransaction Found!\n");
            printf("Type: %s\n", transactions[i].type);
            printf("Category: %s\n", transactions[i].category);
            printf("Amount: Rs. %.2f\n", transactions[i].amount);
            printf("Date: %s\n", transactions[i].date);
            printf("Note: %s\n", transactions[i].note);

            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nTransaction not found!\n");
}

void editTransaction() {
    int id, i, choice;

    printf("\nEnter Transaction ID to edit: ");

    if (scanf("%d", &id) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }

    for (i = 0; i < count; i++) {
        if (transactions[i].id == id) {

            printf("\n1. Income\n2. Expense\n");
            printf("Choose new type: ");

            if (scanf("%d", &choice) != 1) {
                printf("Invalid input!\n");
                while (getchar() != '\n');
                return;
            }

            if (choice == 1)
                strcpy(transactions[i].type, "Income");
            else if (choice == 2)
                strcpy(transactions[i].type, "Expense");
            else {
                printf("Invalid choice!\n");
                return;
            }

            printf("Enter new category: ");
            scanf(" %29[^\n]", transactions[i].category);

            printf("Enter new amount: ");

            if (scanf("%f", &transactions[i].amount) != 1 ||
                transactions[i].amount <= 0) {
                printf("Invalid amount!\n");
                while (getchar() != '\n');
                return;
            }

            printf("Enter new date: ");
            scanf(" %19s", transactions[i].date);

            printf("Enter new note: ");
            scanf(" %99[^\n]", transactions[i].note);

            saveData();

            printf("\nTransaction updated successfully!\n");
            return;
        }
    }

    printf("\nTransaction not found!\n");
}

void deleteTransaction() {
    int id, i, j;

    printf("\nEnter Transaction ID to delete: ");

    if (scanf("%d", &id) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }

    for (i = 0; i < count; i++) {
        if (transactions[i].id == id) {

            for (j = i; j < count - 1; j++) {
                transactions[j] = transactions[j + 1];
            }

            count--;

            for (j = 0; j < count; j++) {
                transactions[j].id = j + 1;
            }

            saveData();

            printf("\nTransaction deleted successfully!\n");
            return;
        }
    }

    printf("\nTransaction not found!\n");
}

int main() {
    int choice;

    loadData();

    do {
        printf("\n");
        printf("========================================\n");
        printf("           SMARTSPEND v2.0              \n");
        printf("       PERSONAL EXPENSE TRACKER         \n");
        printf("========================================\n");

        printf("\n1. Add Transaction");
        printf("\n2. View Transactions");
        printf("\n3. Financial Summary");
        printf("\n4. Search Transaction");
        printf("\n5. Edit Transaction");
        printf("\n6. Delete Transaction");
        printf("\n7. Exit");

        printf("\n\nEnter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input!\n");
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
                printf("\nThank you for using SmartSpend!\n");
                break;

            default:
                printf("\nInvalid choice! Try again.\n");
        }

    } while (choice != 7);

    return 0;
}