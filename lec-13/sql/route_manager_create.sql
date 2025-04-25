create table if not exists Countries (
    CountryID varchar(10) not null unique primary key,
    Name varchar(100) not null,
    ICAOCode varchar(3) not null unique,
    AirspaceCapacity integer not null default 1000
);

create table if not exists Airports (
    AirportID varchar(10) not null unique primary key,
    Name varchar(100) not null,
    Location varchar(100) not null,
    PassengerTerminals integer not null default 1,
    CargoTerminals integer not null default 0,
    Capacity integer not null default 50,
    Runways integer not null default 1
);

create table if not exists Airlines (
    AirlineID varchar(10) not null unique primary key,
    Name varchar(100) not null,
    CountryID varchar(10) foreign key references Countries(CountryID) on delete cascade,
    FleetSize integer not null default 0,
    InternationalCarrier boolean not null default false
);

create table if not exists Slots (
    SlotID varchar(10) not null unique primary key,
    AirportID varchar(10) not null foreign key references Airports(AirportID) on delete cascade,
    AirlineID varchar(10) not null foreign key references Airlines(AirlineID) on delete cascade,
    StartTime float not null,
    EndTime float not null,
    TurnaruondTime integer,
    ParkingDuration integer not null default 0,
    Season varchar(50) not null
);

create table if not exists Treaties (
    TreatyID varchar(10) not null unique primary key,
    CountryAID varchar(10) not null foreign key references Countries(CountryID) on delete cascade,
    CountryBID varchar(10) not null foreign key references Countries(CountryID) on delete cascade,
    AllowedFlightsPerWeek integer not null default 0,
    ValidFrom date not null,
    ValidUntil date not null
);

create table if not exists Routes (
    RouteID varchar(10) not null unique primary key,
    FromAirportID varchar(10) not null foreign key references Airports(AirportID) on delete cascade,
    ToAirportID varchar(10) not null foreign key references Airports(AirportID) on delete cascade,
    Distance integer not null,
    TreatyID varchar(10) foreign key references Treaties(TreatyID) on delete cascade
);

create table if not exists TreatyAirline (
    TraetyID varchar(10) not null foreign key references Treaties(TreatyID) on delete cascade,
    AirlineID varchar(10) not null foreign key references Airlines(AirlineID) on delete cascade
);