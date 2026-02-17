# PCH Benchmark Notes

Date: 2026-02-15

Measured with:

```bash
python3 /Volumes/LCS.Data/CP-Problems/CodeForces/Algorithms/scripts/benchmark_pch.py \
  --round-dir /Volumes/LCS.Data/CP-Problems/CodeForces/Rounds/Round_1047_Div_3 \
  --target problem_A \
  --compiler gcc \
  --algorithms-dir /Volumes/LCS.Data/CP-Problems/CodeForces/Algorithms \
  --json-out /Volumes/LCS.Data/CP-Problems/CodeForces/Algorithms/cmake/PCHBenchmarkResult.json
```

## Result Summary

- Incremental build gain with PCH ON: `+1.561s` (PCH ON faster)
- Cold build penalty with PCH ON: `+2.426s`
- First build after reconfigure/new target penalty with PCH ON: `+3.183s`

Decision:

- Keep PCH default enabled for Debug iteration speed (`recommendation: keep_default_pch`).

Operational note (for rapid `cppnew` bursts):

- Temporarily disable PCH during bulk problem creation, then re-enable:

```bash
cppconf Debug gcc pch=off
# create many problems / first-pass builds
cppconf Debug gcc pch=on
```
