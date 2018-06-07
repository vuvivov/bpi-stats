# bpi-stats

## Project Overview
This is a command line c++ application that processes bpi price data and displays statistics.<br />
The folder "src" contains a library of the core features.<br />
The folder "app" contains the main application and a sample data file.<br />
The folder "unitTests" contains the unit tests for the library and a sample data file.<br />
The file "documentation.pdf" contains detailed documentation of the source code.
## Build
### Linux/ MacOS
#### Requirements
- C++17
- Boost (>=1.67.0)
- CMake (>=3.10)
#### Instructions
Make sure BOOST_ROOT is set correctly. Under project folder:
```
mkdir build
cd build
cmake ..
make
```
### Windows
#### Requirements
- Visual Studio(>=2017)
#### Instructions
- Install VC++2017 and NuGet Package Manager for Visual Studio if haven't.
- Create a console application from source code.
- Install Boost using NuGet Package Manager.
## Run Application
### Option 1: Input data from file
Provide a file path as argument (e.g. "bpistats prices.json"). The file needs to contain data in the format of https://api.coindesk.com/v1/bpi/historical/close.json?start=2018-01-01&end=2018-01-20
### Option 2: Fetch data from web
Input two arguments representing start and end date(e.g. "bpistats 2018-01-04 2018-01-08")
### Demo
```
C:\Users\Shangwei\source\repos\bpistats\Debug>bpistats prices.json

<Highest price and date(s)>
17135.836300
2018-01-06

<Lowest price and date(s)>
11141.248800
2018-01-17

<The number of data points in the time span>
20

<The average price for the time span>
13975.165275

<The median price for the time span>
14000.750000

<The standard deviation of the prices for the time span>
1732.221456

C:\Users\Shangwei\source\repos\bpistats\Debug>bpistats 2018-01-01 2018-01-05

<Highest price and date(s)>
16937.173800
2018-01-05

<Lowest price and date(s)>
13412.440000
2018-01-01

<The number of data points in the time span>
5

<The average price for the time span>
15076.049540

<The median price for the time span>
15134.651300

<The standard deviation of the prices for the time span>
1127.253733

```
