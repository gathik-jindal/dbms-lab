# Air Travel - Route management

## Use cases

### Add Airports / Routes

- **Actors**: Airport Authorities, Airlines
- **Description**: This allows for the addition of new airports and routes to the system, enabling better connectivity and expansion of air travel networks.
- Detailed Description: Airports and routes are added based on demand, geographical importance, and regulatory approvals to ensure efficient air travel management.

### Airline slot allocation

- **Actors**: Government, Airlines
- **Description**: This allows for the airlines to reserve slots at airports to use for arrivals and departure.
- Detailed Description: A slot is very important to airlines, since the more slots they have the more they can profit from that airport. A slot is given to an airline based on a lot of factors, such as aircraft type, turnaround time and parking duration.

### Airline route allocation

- **Actors**: Government of countries, Airlines
- **Description**: This allows for countries to give a route that involves more than one country to an airline. This is important because airlines generate international brand value through international flights.
- Detailed Description: This type of agreement requires some form of regulation in terms of which airline can fly a certain number of flights in a week.

## Tables

```bash
├── Airports
│   └── Slots ( Reserved by )
├── Routes
│   └── Reserved-by
├── Airlines
├── Treaties ( Agreements between countries )
├── Countries ( Is this needed? )
```

### Tables Description

#### Countries

| Field            | Description                                           |
| ---------------- | ----------------------------------------------------- |
| CountryID        | Unique country identifier                             |
| Name             | Official name of the country                          |
| ICAOCode         | ICAO country code                                     |
| AirspaceCapacity | Optional: flight volume capacity of national airspace |

#### Airports

| Field              | Description                                        |
| ------------------ | -------------------------------------------------- |
| AirportID          | Unique identifier for the airport                  |
| Name               | Full name of the airport                           |
| Location           | City and country of the airport                    |
| PassengerTerminals | Number of terminals for passengers                 |
| CargoTerminals     | Number of terminals for cargo                      |
| Capacity           | Max passengers the airport can handle per terminal |
| Runways            | Number of runways the airport has                  |

#### Airlines

| Field                | Description                                            |
| -------------------- | ------------------------------------------------------ |
| AirlineID            | Unique airline identifier                              |
| Name                 | Official name of the airline                           |
| CountryID            | Foreign key to the airline’s country of origin         |
| FleetSize            | Number of aircraft in operation                        |
| InternationalCarrier | Whether the airline has international rights (boolean) |

#### Slots

| Field           | Description                                            |
| --------------- | ------------------------------------------------------ |
| SlotID          | Unique slot identifier                                 |
| AirportID       | Foreign key referencing Airports                       |
| AirlineID       | Foreign key referencing Airlines                       |
| StartTime       | Start time of slot                                     |
| EndTime         | End time of slot                                       |
| TurnaroundTime  | Time needed to prep the aircraft for next flight       |
| ParkingDuration | Duration the aircraft stays at the airport             |
| Season          | Season for which the slot is valid (e.g., Summer 2025) |

#### Treaties

| Field                 | Description                                            |
| --------------------- | ------------------------------------------------------ |
| TreatyID              | Unique treaty identifier                               |
| CountryAID            | First country involved                                 |
| CountryBID            | Second country involved                                |
| AllowedFlightsPerWeek | Max flights allowed between the two countries per week |
| ValidFrom             | Start date of the treaty                               |
| ValidUntil            | End/review date of the treaty                          |

#### Routes

| Field         | Description                                        |
| ------------- | -------------------------------------------------- |
| RouteID       | Unique route identifier                            |
| FromAirportID | Departure airport (foreign key to Airports)        |
| ToAirportID   | Destination airport (foreign key)                  |
| Distance      | Distance between airports (in km or miles)         |
| TreatyID      | If international, links to a Treaty (or else NULL) |

#### TreatyAirline

| Field     | Description                   |
| --------- | ----------------------------- |
| TreatyID  | Foreign key to Treaties table |
| AirlineID | Foreign key to Airlines table |

## UML Code

```bash
@startuml
entity "Airports" as Airports {
  * ID : int
  * Name : string
  * Location : string
  * PassengerTerminals : int
  * CargoTerminals : int
  * Capacity : int
  * Runways : int
}

entity "Slots" as Slots {
  * ID : int
  * AirportID : int
  * AirlineID : int
  * TimeWindow : string
  * TurnaroundTime : int
  * ParkingDuration : int
  * Season : string
}

entity "Routes" as Routes {
  * ID : int
  * FromAirportID : int
  * ToAirportID : int
  * Distance : float
  * TreatyID : int
}

entity "Airlines" as Airlines {
  * ID : int
  * Name : string
  * CountryID : int
  * FleetSize : int
  * InternationalCarrier : boolean
}

entity "Treaties" as Treaties {
  * ID : int
  * CountryAID : int
  * CountryBID : int
  * AllowedFlightsPerWeek : int
  * ValidFrom : date
  * ValidUntil : date
  * Notes : string
}

entity "TreatyAirline" as TreatyAirline {
  * TreatyID : int
  * AirlineID : int
}

entity "Countries" as Countries {
  * ID : int
  * Name : string
  * ICAOCode : string
  * AirspaceCapacity : int
}

Airports -- Slots : has
Airlines -- Slots : uses
Routes -- Airports : from
Routes -- Airports : to
Routes -- Treaties : governed by
Airlines -- TreatyAirline : registered
Treaties -- TreatyAirline : includes
Airlines -- Countries : based in
Treaties -- Countries : CountryA
Treaties -- Countries : CountryB

@enduml
```
