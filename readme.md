# archive stdin by time format

## build

```bash
g++ -std=c++14 -O2 -pthread archive.cpp -o archive
```

## run

```bash
cmd | archive output_prefix timefmt
```

timefmt is the same as in strptime