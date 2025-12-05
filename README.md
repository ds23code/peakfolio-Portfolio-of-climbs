# **Peakfolio**

**Author:** Dhruv Sharma

## **Project Description**

**Peakfolio** is a comprehensive climbing logbook application that allows users to **add, view, and manage climbing routes, attempts, and overall statistics**. Whether you’re tracking personal progress or analyzing climbing trends, Peakfolio provides a clean, organized, and interactive way to record your climbing adventures.

Key features include:

* Add new climbing routes with difficulty and length.
* Insert, remove, or swap routes in your logbook.
* Record individual attempts with ratings and attempt types (First Go, Success, Fail).
* Filter routes by difficulty.
* Duplicate attempts between climbers for shared experiences.
* Print detailed logs of routes and attempts, including average ratings.

## **Usage**

Run the program and enter commands to interact with your climbing logbook.

* `r [name] [difficulty] [length]` – Append a climbing route.
* `p` – Print all routes.
* `f [difficulty_min] [difficulty_max]` – Filter routes by difficulty.
* `i [name] [difficulty] [length] [route_to_insert_before]` – Insert a route.
* `l [route_1] [route_2] [length_change]` – Change lengths.
* `s [route_1] [route_2]` – Swap routes.
* `a [climber] [attempt_type] [rating] [route]` – Add a climbing attempt.
* `P` – Print all routes and attempts.
* `R [route]` – Remove a route.
* `D [climber]` – Delete attempts by a climber.
* `d [climber_1] [climber_2]` – Duplicate attempts between climbers.
* `?` – Show help.

## **Getting Started**

1. Clone the repository:

```bash
git clone https://github.com/yourusername/peakfolio.git
```

2. Compile the program using a C compiler:

```bash
gcc -o peakfolio peakfolio.c
```

3. Run the executable:

```bash
./peakfolio
```
