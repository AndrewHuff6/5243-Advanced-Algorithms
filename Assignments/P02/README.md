# FINAL REPORT

This project was definitely fun in seeing which operations did best given certain workloads. Through this project, 
I learned that the stats were pretty accurate with the "common tradeoffs" section of the readme provided. However, 
I've noticed that my workLoads files were all about 8 times greater than N, with some of them being even larger. 

The line that surprised me the most was line 30 in the results.csv file. The binary search tree with 20,000 elements had over one trillion comparisons in total. This could be a valid total due to each systematic comparison depending on both the size of the tree as well as the value being inserted. For the most part, it seemed that the sorted array did the worst in each category. This makes sense, especially when keeping track of the relink shifts. The results in the results.csv are structured as follows:

structure,workload,size,comparisons,structural_ops,inserts,deletes,lookups,resize_events,shifts_relinks

# Command Timeline

In order to run this project, these were the commands I used to compile and execute.
1. g++ -std=c++20 -Iinclude workload_generator.cpp -o workload_generator
2. chmod +x generate_workoads.sh
3. ./generate_workloads.sh
4. g++ -std=c++20 -Iinclude main.cpp -o experiment
5. ./experiment

# Notes

If you look closely, you will notice in step 2 that I misspelt "workloads" and spelt it as "workoads". Just wanted to point out the typo as it corresponds to the created executable in the source directory. I have also inserted the common tradeoffs table here as reference.

| Structure      | Insert | Lookup | Delete | Hidden Cost         |
| -------------- | ------ | ------ | ------ | ------------------- |
| Unsorted Array | Fast   | Slow   | Medium | None                |
| Sorted Array   | Slow   | Fast   | Slow   | Shifting            |
| Linked List    | Medium | Slow   | Fast   | Traversal           |
| BST            | Medium | Medium | Medium | Tree shape          |
| Hash Table     | Fast   | Fast   | Fast   | Resize / collisions |
