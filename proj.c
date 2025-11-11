#include <stdio.h>
#include <string.h>

#define MAX_BINS 100
#define MAX_TYPE_LEN 20
#define MAX_LOC_LEN 40

/* Bitwise Flags for bin status */
#define FLAG_ACTIVE 0x01      /* 1 => Active */
#define FLAG_FULL 0x02        /* 1 => Full */
#define FLAG_NEEDS_CLEAN 0x04 /* 1 => Needs Cleaning */

/* Structure definition */
typedef struct {
    int id;
    char type[MAX_TYPE_LEN];
    char location[MAX_LOC_LEN];
    int capacity;       /* total capacity (liters) */
    int fillLevel;      /* current fill percentage */
    int flags;          /* bitwise flags for status */
} Bin;

/* Global array and counter */
Bin bins[MAX_BINS];
int binCount = 0;

/* Secondary zone (for merge demo) */
Bin zoneBins[MAX_BINS];
int zoneCount = 0;

/* Function declarations */
void addBin();
void displayAllBins();
void displayBin(int idx);
void updateBin();
void deleteBin();
void searchByType();
void searchByLocation();
void lowCapacityAlert();
void sortByFillLevel();
void sortByType();
void toggleBinStatus();
void mergeZones();
void populateSampleBins();
void populateZoneSample();
int findBinIndexById(int id);
void swapBins(Bin *a, Bin *b);
void bitwiseDemo();
void swapWithoutThirdVariableDemo();
int readInt(const char prompt[], int min, int max);
void readString(const char prompt[], char dest[], int maxlen);
void mainMenu();

/* -------------------------------------------------- */
/* Utility Functions */
/* -------------------------------------------------- */

/* Safe integer input with bounds */
int readInt(const char prompt[], int min, int max) {
    int val, rc;
    while (1) {
        printf("%s", prompt);
        rc = scanf("%d", &val);
        if (rc != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Enter an integer.\n");
            continue;
        }
        while (getchar() != '\n');
        if (val < min) val = min;
        if (val > max) val = max;
        return val;
    }
}

/* Read simple string (no spaces) safely */
void readString(const char prompt[], char dest[], int maxlen) {
    printf("%s", prompt);
    if (maxlen > 1) {
        char fmt[10];
        sprintf(fmt, "%%%ds", maxlen - 1);
        scanf(fmt, dest);
    } else {
        scanf("%s", dest);
    }
    while (getchar() != '\n');
}

/* Swap two Bin records */
void swapBins(Bin *a, Bin *b) {
    Bin temp = *a;
    *a = *b;
    *b = temp;
}

/* Find index of bin by ID */
int findBinIndexById(int id) {
    for (int i = 0; i < binCount; i++) {
        if (bins[i].id == id)
            return i;
    }
    return -1;
}

/* -------------------------------------------------- */
/* Core Functionalities */
/* -------------------------------------------------- */

/* Populate sample bins for testing */
void populateSampleBins() {
    if (binCount > 0) {
        printf("Bins already exist. Skipping.\n");
        return;
    }
    binCount = 4;

    bins[0].id = 101;
    strcpy(bins[0].type, "Plastic");
    strcpy(bins[0].location, "Sector17");
    bins[0].capacity = 100;
    bins[0].fillLevel = 95;
    bins[0].flags = FLAG_ACTIVE | FLAG_FULL;

    bins[1].id = 102;
    strcpy(bins[1].type, "Organic");
    strcpy(bins[1].location, "Sector22");
    bins[1].capacity = 150;
    bins[1].fillLevel = 70;
    bins[1].flags = FLAG_ACTIVE;

    bins[2].id = 103;
    strcpy(bins[2].type, "Glass");
    strcpy(bins[2].location, "Sector21");
    bins[2].capacity = 120;
    bins[2].fillLevel = 90;
    bins[2].flags = FLAG_ACTIVE | FLAG_FULL;

    bins[3].id = 104;
    strcpy(bins[3].type, "Paper");
    strcpy(bins[3].location, "Sector19");
    bins[3].capacity = 80;
    bins[3].fillLevel = 40;
    bins[3].flags = FLAG_ACTIVE;

    printf("Sample bin data added successfully.\n");
}

/* Add a new bin */
void addBin() {
    if (binCount >= MAX_BINS) {
        printf("Cannot add more bins.\n");
        return;
    }
    Bin b;
    b.id = readInt("Enter Bin ID: ", 1, 10000);
    readString("Enter Bin Type (Plastic/Organic/Glass/Paper): ", b.type, MAX_TYPE_LEN);
    readString("Enter Location: ", b.location, MAX_LOC_LEN);
    b.capacity = readInt("Enter Capacity (liters): ", 1, 10000);
    b.fillLevel = readInt("Enter Current Fill Percentage: ", 0, 100);
    b.flags = FLAG_ACTIVE;
    if (b.fillLevel >= 90) b.flags |= FLAG_FULL;

    bins[binCount++] = b;
    printf("Bin added successfully. Total bins: %d\n", binCount);
}

/* Display all bins */
void displayAllBins() {
    if (binCount == 0) {
        printf("No bins available.\n");
        return;
    }
    printf("\nNo  ID   Type        Location        Cap  Fill%%  Active Full  Clean\n");
    for (int i = 0; i < binCount; i++) {
        Bin *b = &bins[i];
        printf("%-3d %-4d %-10s %-15s %-4d %-6d %-6c %-5c %-5c\n",
               i + 1, b->id, b->type, b->location, b->capacity, b->fillLevel,
               (b->flags & FLAG_ACTIVE) ? 'Y' : 'N',
               (b->flags & FLAG_FULL) ? 'Y' : 'N',
               (b->flags & FLAG_NEEDS_CLEAN) ? 'Y' : 'N');
    }
}

/* Display one bin */
void displayBin(int idx) {
    if (idx < 0 || idx >= binCount) return;
    Bin *b = &bins[idx];
    printf("\n--- Bin Details ---\n");
    printf("ID: %d\n", b->id);
    printf("Type: %s\n", b->type);
    printf("Location: %s\n", b->location);
    printf("Capacity: %d\n", b->capacity);
    printf("Fill Level: %d%%\n", b->fillLevel);
    printf("Status: ");
    if (b->flags & FLAG_ACTIVE) printf("Active ");
    else printf("Inactive ");
    if (b->flags & FLAG_FULL) printf("| Full ");
    if (b->flags & FLAG_NEEDS_CLEAN) printf("| Needs Cleaning");
    printf("\n");
}

/* Update a bin */
void updateBin() {
    if (binCount == 0) {
        printf("No bins to update.\n");
        return;
    }
    int id = readInt("Enter Bin ID to update: ", 1, 10000);
    int idx = findBinIndexById(id);
    if (idx == -1) {
        printf("Bin ID not found.\n");
        return;
    }
    Bin *b = &bins[idx];
    printf("Updating Bin ID %d (%s)\n", b->id, b->type);
    printf("1. Update Fill Level\n2. Update Location\n3. Toggle Clean Flag\n4. Back\n");
    int ch = readInt("Enter choice: ", 1, 4);
    if (ch == 1) {
        b->fillLevel = readInt("Enter new fill percentage: ", 0, 100);
        if (b->fillLevel >= 90) b->flags |= FLAG_FULL;
        else b->flags &= ~FLAG_FULL;
    } else if (ch == 2) {
        readString("Enter new location: ", b->location, MAX_LOC_LEN);
    } else if (ch == 3) {
        b->flags ^= FLAG_NEEDS_CLEAN;
        printf("Toggled cleaning status.\n");
    }
    printf("Update successful.\n");
}

/* Delete bin by ID */
void deleteBin() {
    if (binCount == 0) {
        printf("No bins to delete.\n");
        return;
    }
    int id = readInt("Enter Bin ID to delete: ", 1, 10000);
    int idx = findBinIndexById(id);
    if (idx == -1) {
        printf("Bin not found.\n");
        return;
    }
    for (int i = idx; i < binCount - 1; i++) bins[i] = bins[i + 1];
    binCount--;
    printf("Bin deleted successfully.\n");
}

/* Search by Type */
void searchByType() {
    if (binCount == 0) {
        printf("Database empty.\n");
        return;
    }
    char q[MAX_TYPE_LEN];
    readString("Enter bin type to search: ", q, MAX_TYPE_LEN);
    int found = 0;
    for (int i = 0; i < binCount; i++) {
        if (strcmp(bins[i].type, q) == 0) {
            displayBin(i);
            found = 1;
        }
    }
    if (!found) printf("No bins of type '%s' found.\n", q);
}

/* Search by Location */
void searchByLocation() {
    if (binCount == 0) {
        printf("Database empty.\n");
        return;
    }
    char loc[MAX_LOC_LEN];
    readString("Enter location to search: ", loc, MAX_LOC_LEN);
    int found = 0;
    for (int i = 0; i < binCount; i++) {
        if (strcmp(bins[i].location, loc) == 0) {
            displayBin(i);
            found = 1;
        }
    }
    if (!found) printf("No bins found in location '%s'.\n", loc);
}

/* Low capacity / full alert */
void lowCapacityAlert() {
    if (binCount == 0) {
        printf("No bins to check.\n");
        return;
    }
    int threshold = readInt("Enter fill percentage alert threshold (e.g. 80): ", 0, 100);
    int found = 0;
    for (int i = 0; i < binCount; i++) {
        if (bins[i].fillLevel >= threshold) {
            printf("ALERT: Bin ID %d (%s) is %.0f%% full at %s\n",
                   bins[i].id, bins[i].type,
                   (float)bins[i].fillLevel, bins[i].location);
            found = 1;
        }
    }
    if (!found) printf("All bins are below threshold.\n");
}

/* Sort bins by fill level descending */
void sortByFillLevel() {
    for (int i = 0; i < binCount - 1; i++) {
        for (int j = i + 1; j < binCount; j++) {
            if (bins[j].fillLevel > bins[i].fillLevel)
                swapBins(&bins[i], &bins[j]);
        }
    }
    printf("Bins sorted by fill level (descending).\n");
}

/* Sort by type alphabetically */
void sortByType() {
    for (int i = 0; i < binCount - 1; i++) {
        for (int j = i + 1; j < binCount; j++) {
            if (strcmp(bins[i].type, bins[j].type) > 0)
                swapBins(&bins[i], &bins[j]);
        }
    }
    printf("Bins sorted by type (A-Z).\n");
}

/* Toggle bin active/inactive (bitwise demo) */
void toggleBinStatus() {
    if (binCount == 0) {
        printf("No bins to toggle.\n");
        return;
    }
    int id = readInt("Enter Bin ID to toggle active status: ", 1, 10000);
    int idx = findBinIndexById(id);
    if (idx == -1) {
        printf("Bin not found.\n");
        return;
    }
    bins[idx].flags ^= FLAG_ACTIVE;
    printf("Bin %d now %s.\n", id, (bins[idx].flags & FLAG_ACTIVE) ? "Active" : "Inactive");
}

/* Populate secondary zone bins */
void populateZoneSample() {
    zoneCount = 3;

    zoneBins[0].id = 201;
    strcpy(zoneBins[0].type, "Organic");
    strcpy(zoneBins[0].location, "Sector25");
    zoneBins[0].capacity = 100;
    zoneBins[0].fillLevel = 80;
    zoneBins[0].flags = FLAG_ACTIVE;

    zoneBins[1].id = 202;
    strcpy(zoneBins[1].type, "Plastic");
    strcpy(zoneBins[1].location, "Sector26");
    zoneBins[1].capacity = 120;
    zoneBins[1].fillLevel = 60;
    zoneBins[1].flags = FLAG_ACTIVE;

    zoneBins[2].id = 203;
    strcpy(zoneBins[2].type, "Metal");
    strcpy(zoneBins[2].location, "Sector27");
    zoneBins[2].capacity = 90;
    zoneBins[2].fillLevel = 30;
    zoneBins[2].flags = FLAG_ACTIVE;

    printf("Zone sample data added.\n");
}

/* Merge zone bins into main bins (excluding duplicates by type) */
void mergeZones() {
    if (zoneCount == 0) {
        printf("No zone bins to merge.\n");
        return;
    }
    for (int i = 0; i < zoneCount; i++) {
        int dup = 0;
        for (int j = 0; j < binCount; j++) {
            if (strcmp(zoneBins[i].type, bins[j].type) == 0) {
                dup = 1;
                break;
            }
        }
        if (!dup && binCount < MAX_BINS) {
            bins[binCount++] = zoneBins[i];
            printf("Merged bin '%s' from zone.\n", zoneBins[i].type);
        } else if (dup)
            printf("Skipped duplicate bin '%s'.\n", zoneBins[i].type);
    }
    printf("Zone merge completed.\n");
}

/* Bitwise operation demo (AND, OR, XOR) */
void bitwiseDemo() {
    int x = readInt("Enter first integer: ", -1000000, 1000000);
    int y = readInt("Enter second integer: ", -1000000, 1000000);
    printf("AND=%d OR=%d XOR=%d\n", x & y, x | y, x ^ y);
}

/* Swap without third variable demo */
void swapWithoutThirdVariableDemo() {
    int a = readInt("Enter a: ", -1000, 1000);
    int b = readInt("Enter b: ", -1000, 1000);
    printf("Before: a=%d b=%d\n", a, b);
    a = a + b;
    b = a - b;
    a = a - b;
    printf("After:  a=%d b=%d\n", a, b);
}

/* -------------------------------------------------- */
/* MAIN MENU */
/* -------------------------------------------------- */
void mainMenu() {
    int ch;
    printf("\nSMART WASTE SEGREGATION & BIN TRACKER SYSTEM\n");
    do {
        printf("\n------------------- MENU --------------------\n");
        printf("0. Populate Sample Bins\n");
        printf("1. Add Bin\n");
        printf("2. Display All Bins\n");
        printf("3. Update Bin\n");
        printf("4. Delete Bin\n");
        printf("5. Search by Type\n");
        printf("6. Search by Location\n");
        printf("7. Low Capacity Alert\n");
        printf("8. Sort by Fill Level\n");
        printf("9. Sort by Type\n");
        printf("10. Toggle Active Status (Bitwise)\n");
        printf("11. Add Zone Sample Data\n");
        printf("12. Merge Zone into Main\n");
        printf("13. Swap Without Third Variable Demo\n");
        printf("14. Bitwise AND/OR/XOR Demo\n");
        printf("15. Exit\n");
        ch = readInt("Enter your choice: ", 0, 15);
        switch (ch) {
            case 0: populateSampleBins(); break;
            case 1: addBin(); break;
            case 2: displayAllBins(); break;
            case 3: updateBin(); break;
            case 4: deleteBin(); break;
            case 5: searchByType(); break;
            case 6: searchByLocation(); break;
            case 7: lowCapacityAlert(); break;
            case 8: sortByFillLevel(); break;
            case 9: sortByType(); break;
            case 10: toggleBinStatus(); break;
            case 11: populateZoneSample(); break;
            case 12: mergeZones(); break;
            case 13: swapWithoutThirdVariableDemo(); break;
            case 14: bitwiseDemo(); break;
            case 15: printf("Exiting program...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (ch != 15);
}
