// main.cpp
// derived from claude to help with the runJobFile methods in each DS class
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// include all .hpp files
#include "bst.hpp"
#include "hashTable.hpp"
#include "linkedList.hpp"
#include "sortedArraySet.hpp"

using namespace std;

// -------------------------------------------------------------
// CSV writer
// -------------------------------------------------------------
// Appends one row of results to the CSV file.
// Creates the file with a header row if it doesn't exist yet.
void writeCSV(const string &filename,
              const string &dsName,
              const string &workload,
              int           size,
              const Counters &c)
{
    // Check if file already exists (to decide whether to write header)
    bool needsHeader = false;
    {
        ifstream check(filename);
        needsHeader = !check.good();
    }

    ofstream f(filename, ios::app);   // open in append mode
    if (!f) {
        cerr << "Error: could not open " << filename << "\n";
        return;
    }

    if (needsHeader) {
        f << "structure,workload,size,"
          << "comparisons,structural_ops,inserts,deletes,"
          << "lookups,resize_events,shifts_relinks\n";
    }

    f << dsName     << ","
      << workload   << ","
      << size       << ","
      << c.comparisons    << ","
      << c.structural_ops << ","
      << c.inserts        << ","
      << c.deletes        << ","
      << c.lookups        << ","
      << c.resize_events  << ","
      << c.shifts_relinks << "\n";
}

// -------------------------------------------------------------
// Run one data structure against one workload file
// -------------------------------------------------------------
template <typename DS>
void runOne(DS &ds, const string &workloadFile,
            const string &workload, int size,
            const string &csvFile)
{
    ds.reset();             // clear data + counters before each run
    ds.runJobFile(workloadFile);

    Counters c = ds.getCounters();

    // Print a quick summary to the terminal
    cout << left
         << setw(16) << ds.name()
         << setw(12) << workload
         << setw(8)  << size
         << "  cmp="    << setw(10) << c.comparisons
         << "  ins="    << setw(8)  << c.inserts
         << "  lkp="    << setw(8)  << c.lookups
         << "  del="    << setw(8)  << c.deletes
         << "  resize=" << setw(4)  << c.resize_events
         << "  shifts=" << c.shifts_relinks
         << "\n";

    writeCSV(csvFile, ds.name(), workload, size, c);
}

// -------------------------------------------------------------
// main
// -------------------------------------------------------------
int main() {

    const string CSV_FILE      = "results.csv";
    const string WORKLOAD_DIR  = "workLoads/";   // folder where .json files live

    // All workload types and sizes
    vector<string> workloadTypes = {"A", "B", "C", "D"};
    vector<int>    sizes         = {1000, 5000, 10000, 20000};

    // Instantiate each data structure once
    Bst            bst;
    HashTable      ht;
    LinkedList     ll;
    SortedArraySet sa;

    cout << left
         << setw(16) << "Structure"
         << setw(12) << "Workload"
         << setw(8)  << "Size"
         << "  Stats\n";
    cout << string(80, '-') << "\n";

    for (const auto &wType : workloadTypes) {
        for (int n : sizes) {

            // Build filename e.g. workLoads/workLoad_A_1000.json
            string fname = WORKLOAD_DIR
                         + "workLoad_" + wType + "_" + to_string(n) + ".json";

            cout << "\n[" << wType << " / n=" << n << "]  file: " << fname << "\n";

            runOne(bst, fname, wType, n, CSV_FILE);
            runOne(ht,  fname, wType, n, CSV_FILE);
            runOne(ll,  fname, wType, n, CSV_FILE);
            runOne(sa,  fname, wType, n, CSV_FILE);
        }
    }

    cout << "\nDone! Results saved to: " << CSV_FILE << "\n";
    return 0;
}
