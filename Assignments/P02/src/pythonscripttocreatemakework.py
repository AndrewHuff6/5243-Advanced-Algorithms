#!/usr/bin/env python3

import subprocess

workloads = ["A", "B", "C", "D"]
sizes = [1000, 5000, 10000, 20000]

for w in workloads:
    for n in sizes:
        outfile = f"./work_files/workload_{w}_{n}.json"

        cmd = [
            "./wg",
            "--workload", w,
            "--size", str(n),
            "--save", outfile
        ]

        print("Running:", " ".join(cmd))
        subprocess.run(cmd, check=True)
