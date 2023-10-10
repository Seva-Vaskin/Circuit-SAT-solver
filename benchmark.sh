#!/bin/bash
set -e
set -o pipefail

if [ "$#" -ne 2 ]; then
  echo "Usage: benchmarks.sh solver_name time_limit"
  exit 1
fi

cd "$(dirname "$0")"

solver_name="$1"
time_limit="$2"
results="./data/benchmarks/$solver_name-$(date +'%m%d%Y-%H%M%S')"
tests="./data/benchs_for_test"
build_dir="cmake-build-release"
app_name="CircuitSATSolver"

mkdir -p "$(dirname "$results")"

echo "Configuring cmake..."
cmake -DCMAKE_BUILD_TYPE=Release -S . -B "$build_dir" >/dev/null || exit 1
echo "Building the application..."
cmake --build "$build_dir" --target "$app_name" >/dev/null || exit 1

echo "Testing information:" | tee -a "$results"
echo " * Solver name: $solver_name" | tee -a "$results"
echo " * Time limit: $time_limit" | tee -a "$results"
echo "Environment:" | tee -a "$results"
echo " * Commit: $(git rev-parse HEAD)" | tee -a "$results"
echo " * CPU:    $(lscpu | grep 'Model name' | awk '{for (i=3; i<=NF; i++) printf "%s ", $i; print ""}')" | tee -a "$results"
echo " * RAM:    $(free -h | grep "Mem" | awk '{print $2}')" | tee -a "$results"
echo "===========================================" >>"$results"
for file in "$tests"/*.bench; do
    echo "Testing $(basename $file)"
    "./$build_dir/$app_name" "$solver_name" "$file" "$time_limit" | tee -a "$results"
done
