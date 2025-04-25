create table if not exists Airports (
    AirportID varchar(10) not null unique primary key,
    Name varchar(100) not null,
    Location varchar(100) not null,
    PassengerTerminals integer not null default 1,
    CargoTerminals integer not null default 0,
    Capacity integer not null default 50,
    Runways integer not null default 1
);

create table if not exists Slots (
    SlotID varchar(10) not null unique primary key,
    AirportID varchar(10) foreign key references Airports(AirportID),
    AirlineID varchar(10) not null,
    StartTime float not null,
    EndTime float not null,
    TurnaruondTime integer
);