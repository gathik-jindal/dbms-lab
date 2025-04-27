import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.sql.*;
import java.util.Scanner;

public class Main {
    static final String SQL_CREATE_PATH = "/home/krishna/Projects/dbms-lab/lec-13/sql/route_manager_create.sql";
    static final String SQL_INSERT_PATH = "/home/krishna/Projects/dbms-lab/lec-13/sql/route_manager_insert.sql";

    static final String JDBC_DRIVER = "com.mysql.cj.jdbc.Driver";
    static final String DB_URL = "jdbc:mysql://localhost:3306/route_manager";

    static final String USER = "root";
    static final String PASSWORD = "mysql";

    static public void main(String args[]) {

        // variables that we'll use throughtout the program
        // for communicating with mysql
        Connection conn = null;
        PreparedStatement pstmt = null;
        ResultSet rs = null;
        Statement stmt = null;

        // Initialization by creating the tables from the SQL script

        try {
            Class.forName(JDBC_DRIVER);
            conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
            stmt = conn.createStatement();

            // SQL: Create tables using the script (source)
            String createSQL = Files.readString(Paths.get(SQL_CREATE_PATH));
            for (String sql : createSQL.split(";")) {
                sql = sql.trim();
                if (!sql.isEmpty()) {
                    stmt.execute(sql + ";");
                }
            }
            System.out.println("Tables created successfully!");

            // Insert data
            String insertSQL = Files.readString(Paths.get(SQL_INSERT_PATH));
            for (String sql : insertSQL.split(";")) {
                sql = sql.trim();
                if (!sql.isEmpty()) {
                    stmt.execute(sql + ";");
                }
            }
            System.out.println("Data inserted successfully!");

        } catch (SQLException e) {
            System.out.println("SQL Exception: " + e.getMessage());
        } catch (ClassNotFoundException e) {
            System.out.println("Class Not Found Exception: " + e.getMessage());
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            // Clean-up
            try {
                if (rs != null)
                    rs.close();
                if (pstmt != null)
                    pstmt.close();
                if (conn != null)
                    conn.close();
            } catch (SQLException se) {
                se.printStackTrace();
            }
        }

        System.out.println("Welcome to the Route Manager!");
        System.out.println("Type '?' for help.");
        System.out.println("Type 'quit' to exit the program.");

        // Main loop for the program
        Boolean running = true;
        while (running) {

            System.out.print("Command: ");
            Scanner scanner = new Scanner(System.in);
            String command = scanner.nextLine();

            switch (command) {
                case "?":
                    // Displays the help menu
                    System.out.println(
                            "Route Manager Help Menu\n"
                                    + "? - Displays this help menu\n"
                                    + "quit - Quits the program\n"
                                    + "routes - Displays the routes between two cities\n"
                                    + "add route - Adds a new route\n"
                                    + "delete route - Deletes a route\n"
                                    + "update route - Updates a route\n"
                                    + "list routes - Displays the list of all routes\n"
                                    + "countries - Displays the list of all countries\n"
                                    + "airports - Displays the list of all airports\n"
                                    + "airlines - Displays the list of all airlines\n"
                                    + "treaties - Displays the list of all treaties\n"
                                    + "slots - Displays the list of all slots\n"
                                    + "add slot - Adds a new slot\n" // check if the time is not intersecting with
                                                                     // anther time
                                    + "delete slot - Deletes a slot\n"
                                    + "update slot - Updates a slot");
                    break;
                case "quit":
                    // Quits the program
                    System.out.println("Quitting the program...");
                    try {
                        if (conn != null)
                            conn.close();
                    } catch (SQLException e) {
                        System.out.println("SQL Exception: " + e.getMessage());
                    }
                    // System.exit(0);
                    scanner.close();
                    running = false;
                    break;
                case "routes":
                    // Displays the routes between two cities
                    System.out.print("Enter the source AirportID: ");
                    String sourceAirport = scanner.nextLine();
                    System.out.print("Enter the destination AirportID: ");
                    String destAirport = scanner.nextLine();
                    try {
                        conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
                        pstmt = conn
                                .prepareStatement("SELECT * FROM Routes WHERE fromAirportID = ? AND toAirportID = ?");
                        pstmt.setString(1, sourceAirport);
                        pstmt.setString(2, destAirport);
                        rs = pstmt.executeQuery();
                        while (rs.next()) {
                            System.out.println("Route: " + rs.getString("RouteID"));
                            System.out.println(rs.getString("fromAirportID") + " -> " + rs.getString("toAirportID"));
                            System.out.println("Distance: " + rs.getString("distance"));
                            System.out.println("TreatyID: " + rs.getString("TreatyID"));
                        }
                    } catch (SQLException e) {
                        // System.out.println("SQL Exception: " + e.getMessage());
                        System.err.println("Error: Unable to fetch routes. Did you enter the correct AirportID?");
                    }
                    break;

                case "add route":
                    // Adds a new route
                    System.out.print("Enter the RouteID: ");
                    String routeID = scanner.nextLine();
                    System.out.print("Enter the from AirportID: ");
                    String fromAirportID = scanner.nextLine();
                    System.out.print("Enter the to AirportID: ");
                    String toAirportID = scanner.nextLine();
                    System.out.print("Enter the distance: ");
                    String distance = scanner.nextLine();
                    System.out.print("Enter the TreatyID: ");
                    String treatyID = scanner.nextLine();

                    try {
                        conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
                        pstmt = conn.prepareStatement(
                                "INSERT INTO Routes (RouteID, FromAirportID, ToAirportID, Distance, TreatyID) VALUES (?, ?, ?, ?, ?)");
                        pstmt.setString(1, routeID);
                        pstmt.setString(2, fromAirportID);
                        pstmt.setString(3, toAirportID);
                        pstmt.setString(4, distance);
                        pstmt.setString(5, treatyID);
                        pstmt.executeUpdate();
                        System.out.println("Route added successfully!");
                    } catch (SQLException e) {
                        System.out.println("SQL Exception: " + e.getMessage());
                    } finally {
                        try {
                            if (pstmt != null)
                                pstmt.close();
                            if (conn != null)
                                conn.close();
                        } catch (SQLException se) {
                            se.printStackTrace();
                        }
                    }
                    break;

                case "delete route":
                    // Deletes a route
                    System.out.print("Enter the RouteID: ");
                    String deleteRouteID = scanner.nextLine();

                    try {
                        conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
                        pstmt = conn.prepareStatement("DELETE FROM Routes WHERE RouteID = ?");
                        pstmt.setString(1, deleteRouteID);
                        pstmt.executeUpdate();
                        System.out.println("Route deleted successfully!");
                    } catch (SQLException e) {
                        System.out.println("SQL Exception: " + e.getMessage());
                        System.err.println("Error: Unable to delete route. Did you enter the correct RouteID?");
                    } finally {
                        try {
                            if (pstmt != null)
                                pstmt.close();
                            if (conn != null)
                                conn.close();
                        } catch (SQLException se) {
                            se.printStackTrace();
                        }
                    }
                    break;

                case "update route":
                    // Updates a route
                    System.out.print("Enter the RouteID: ");
                    String updateRouteID = scanner.nextLine();
                    System.out.print("Enter the new from AirportID: ");
                    String newFromAirportID = scanner.nextLine();
                    System.out.print("Enter the new to AirportID: ");
                    String newToAirportID = scanner.nextLine();
                    System.out.print("Enter the new distance: ");
                    String newDistance = scanner.nextLine();
                    System.out.print("Enter the new TreatyID: ");
                    String newTreatyID = scanner.nextLine();

                    try {
                        conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
                        pstmt = conn.prepareStatement(
                                "UPDATE Routes SET FromAirportID = ?, ToAirportID = ?, Distance = ?, TreatyID = ? WHERE RouteID = ?");
                        pstmt.setString(1, newFromAirportID);
                        pstmt.setString(2, newToAirportID);
                        pstmt.setString(3, newDistance);
                        pstmt.setString(4, newTreatyID);
                        pstmt.setString(5, updateRouteID);
                        pstmt.executeUpdate();
                        System.out.println("Route updated successfully!");
                    } catch (SQLException e) {
                        System.out.println("SQL Exception: " + e.getMessage());
                        System.err.println("Error: Unable to update route. Did you enter the correct RouteID?");
                    } finally {
                        try {
                            if (pstmt != null)
                                pstmt.close();
                            if (conn != null)
                                conn.close();
                        } catch (SQLException se) {
                            se.printStackTrace();
                        }
                    }
                    break;

                case "list routes":
                    // Displays the list of all routes
                    try {
                        conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
                        pstmt = conn.prepareStatement("SELECT * FROM Routes");
                        rs = pstmt.executeQuery();
                        System.out.println("------------------------------");
                        while (rs.next()) {
                            System.out.println("Route: " + rs.getString("RouteID"));
                            System.out.println(rs.getString("FromAirportID") + " -> " + rs.getString("ToAirportID"));
                            System.out.println("Distance: " + rs.getString("Distance"));
                            System.out.println("TreatyID: " + rs.getString("TreatyID"));
                            System.out.println("------------------------------");
                        }
                    } catch (SQLException e) {
                        System.out.println("SQL Exception: " + e.getMessage());
                    } finally {
                        try {
                            if (rs != null)
                                rs.close();
                            if (pstmt != null)
                                pstmt.close();
                            if (conn != null)
                                conn.close();
                        } catch (SQLException se) {
                            se.printStackTrace();
                        }
                    }
                    break;

                case "countries":
                    // Displays the list of all countries
                    try {
                        conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
                        pstmt = conn.prepareStatement("SELECT * FROM Countries");
                        rs = pstmt.executeQuery();
                        System.out.println("------------------------------");
                        while (rs.next()) {
                            System.out.println("Country: " + rs.getString("CountryID"));
                            System.out.println("Name: " + rs.getString("Name"));
                            System.out.println("ICAOCode: " + rs.getString("ICAOCode"));
                            System.out.println("AirspaceCapacity: " + rs.getString("AirspaceCapacity"));
                            System.out.println("------------------------------");
                        }
                    } catch (SQLException e) {
                        System.out.println("SQL Exception: " + e.getMessage());
                    } finally {
                        try {
                            if (rs != null)
                                rs.close();
                            if (pstmt != null)
                                pstmt.close();
                            if (conn != null)
                                conn.close();
                        } catch (SQLException se) {
                            se.printStackTrace();
                        }
                    }
                    break;

                case "airports":
                    // Displays the list of all airports
                    try {
                        conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
                        pstmt = conn.prepareStatement("SELECT * FROM Airports");
                        rs = pstmt.executeQuery();
                        System.out.println("------------------------------");
                        while (rs.next()) {
                            System.out.println("Airport: " + rs.getString("AirportID"));
                            System.out.println("Name: " + rs.getString("Name"));
                            System.out.println("CountryID: " + rs.getString("CountryID"));
                            System.out.println("Location: " + rs.getString("Location"));
                            System.out.println("PassengerTerminals: " + rs.getString("PassengerTerminals"));
                            System.out.println("CargoTerminals: " + rs.getString("CargoTerminals"));
                            System.out.println("Capacity: " + rs.getString("Capacity"));
                            System.out.println("Runways: " + rs.getString("Runways"));
                            System.out.println("-------------------------------");
                        }
                    } catch (SQLException e) {
                        System.out.println("SQL Exception: " + e.getMessage());
                    } finally {
                        try {
                            if (rs != null)
                                rs.close();
                            if (pstmt != null)
                                pstmt.close();
                            if (conn != null)
                                conn.close();
                        } catch (SQLException se) {
                            se.printStackTrace();
                        }
                    }
                    break;

                case "airlines":
                    // Displays the list of all airlines
                    try {
                        conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
                        pstmt = conn.prepareStatement("SELECT * FROM Airlines");
                        rs = pstmt.executeQuery();
                        System.out.println("------------------------------");
                        while (rs.next()) {
                            System.out.println("Airline: " + rs.getString("AirlineID"));
                            System.out.println("Name: " + rs.getString("Name"));
                            System.out.println("CountryID: " + rs.getString("CountryID"));
                            System.out.println("FleetSize: " + rs.getString("FleetSize"));
                            System.out.println("InternationalCarrier: " + rs.getString("InternationalCarrier"));
                            System.out.println("-------------------------------");
                        }
                    } catch (SQLException e) {
                        System.out.println("SQL Exception: " + e.getMessage());
                    } finally {
                        try {
                            if (rs != null)
                                rs.close();
                            if (pstmt != null)
                                pstmt.close();
                            if (conn != null)
                                conn.close();
                        } catch (SQLException se) {
                            se.printStackTrace();
                        }
                    }
                    break;

                case "treaties":
                    // Displays the list of all treaties
                    try {
                        conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
                        pstmt = conn.prepareStatement("SELECT * FROM Treaties");
                        rs = pstmt.executeQuery();
                        System.out.println("------------------------------");
                        while (rs.next()) {
                            System.out.println("Treaty: " + rs.getString("TreatyID"));
                            System.out.println("CountryAID: " + rs.getString("CountryAID"));
                            System.out.println("CountryBID: " + rs.getString("CountryBID"));
                            System.out.println("AllowedFlightsPerWeek: " + rs.getString("AllowedFlightsPerWeek"));
                            System.out.println("ValidFrom: " + rs.getString("ValidFrom"));
                            System.out.println("ValidUntil: " + rs.getString("ValidUntil"));
                            System.out.println("-------------------------------");
                        }
                    } catch (SQLException e) {
                        System.out.println("SQL Exception: " + e.getMessage());
                    } finally {
                        try {
                            if (rs != null)
                                rs.close();
                            if (pstmt != null)
                                pstmt.close();
                            if (conn != null)
                                conn.close();
                        } catch (SQLException se) {
                            se.printStackTrace();
                        }
                    }
                    break;

                case "slots":
                    // Displays the list of all slots
                    try {
                        conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
                        pstmt = conn.prepareStatement("SELECT * FROM Slots");
                        rs = pstmt.executeQuery();
                        System.out.println("------------------------------");
                        while (rs.next()) {
                            System.out.println("Slot: " + rs.getString("SlotID"));
                            System.out.println("AirlineID: " + rs.getString("AirlineID"));
                            System.out.println("AirportID: " + rs.getString("AirportID"));
                            System.out.println("StartTime: " + rs.getString("StartTime"));
                            System.out.println("EndTime: " + rs.getString("EndTime"));
                            System.out.println("TurnaroundTime: " + rs.getString("TurnaroundTime"));
                            System.out.println("ParkingDuration: " + rs.getString("ParkingDuration"));
                            System.out.println("Season: " + rs.getString("Season"));
                            System.out.println("-------------------------------");
                        }
                    } catch (SQLException e) {
                        System.out.println("SQL Exception: " + e.getMessage());
                    } finally {
                        try {
                            if (rs != null)
                                rs.close();
                            if (pstmt != null)
                                pstmt.close();
                            if (conn != null)
                                conn.close();
                        } catch (SQLException se) {
                            se.printStackTrace();
                        }
                    }
                    break;

                case "add slot":
                    // Adds a new slot
                    System.out.print("Enter the SlotID: ");
                    String slotID = scanner.nextLine();
                    System.out.print("Enter the AirlineID: ");
                    String airlineID = scanner.nextLine();
                    System.out.print("Enter the AirportID: ");
                    String airportID = scanner.nextLine();
                    System.out.print("Enter the StartTime: ");
                    String startTime = scanner.nextLine();
                    System.out.print("Enter the EndTime: ");
                    String endTime = scanner.nextLine();
                    System.out.print("Enter the TurnaroundTime: ");
                    String turnaroundTime = scanner.nextLine();
                    System.out.print("Enter the ParkingDuration: ");
                    String parkingDuration = scanner.nextLine();
                    System.out.print("Enter the Season: ");
                    String season = scanner.nextLine();

                    // Check if the slot already exists
                    try {
                        conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
                        pstmt = conn.prepareStatement(
                                "SELECT * FROM Slots WHERE AirportID = ? AND Season = ? AND ((StartTime <= ? AND EndTime >= ?) OR (StartTime <= ? AND EndTime >= ?))");
                        pstmt.setString(1, airportID);
                        pstmt.setString(2, season);
                        pstmt.setString(3, startTime);
                        pstmt.setString(4, startTime);
                        pstmt.setString(5, endTime);
                        pstmt.setString(6, endTime);
                        rs = pstmt.executeQuery();
                        if (rs.next()) {
                            System.out.println("Slot already exists for that airport at that season.");
                            break;
                        }
                    } catch (SQLException e) {
                        System.out.println("SQL Exception: " + e.getMessage());
                    } finally {
                        try {
                            if (rs != null)
                                rs.close();
                            if (pstmt != null)
                                pstmt.close();
                            if (conn != null)
                                conn.close();
                        } catch (SQLException se) {
                            se.printStackTrace();
                        }
                    }

                    try {
                        conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
                        pstmt = conn.prepareStatement(
                                "INSERT INTO Slots (SlotID, AirlineID, AirportID, StartTime, EndTime, TurnaroundTime, ParkingDuration, Season) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
                        pstmt.setString(1, slotID);
                        pstmt.setString(2, airlineID);
                        pstmt.setString(3, airportID);
                        pstmt.setString(4, startTime);
                        pstmt.setString(5, endTime);
                        pstmt.setString(6, turnaroundTime);
                        pstmt.setString(7, parkingDuration);
                        pstmt.setString(8, season);
                        pstmt.executeUpdate();
                        System.out.println("Slot added successfully!");
                    } catch (SQLException e) {
                        System.out.println("SQL Exception: " + e.getMessage());
                    } finally {
                        try {
                            if (pstmt != null)
                                pstmt.close();
                            if (conn != null)
                                conn.close();
                        } catch (SQLException se) {
                            se.printStackTrace();
                        }
                    }
                    break;

                case "delete slot":
                    // Deletes a slot
                    System.out.print("Enter the SlotID: ");
                    String deleteSlotID = scanner.nextLine();

                    try {
                        conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
                        pstmt = conn.prepareStatement("DELETE FROM Slots WHERE SlotID = ?");
                        pstmt.setString(1, deleteSlotID);
                        pstmt.executeUpdate();
                        System.out.println("Slot deleted successfully!");
                    } catch (SQLException e) {
                        System.out.println("SQL Exception: " + e.getMessage());
                        System.err.println("Error: Unable to delete slot. Did you enter the correct SlotID?");
                    } finally {
                        try {
                            if (pstmt != null)
                                pstmt.close();
                            if (conn != null)
                                conn.close();
                        } catch (SQLException se) {
                            se.printStackTrace();
                        }
                    }
                    break;

                case "update slot":
                    // Updates a slot
                    System.out.print("Enter the SlotID: ");
                    String updateSlotID = scanner.nextLine();
                    System.out.print("Enter the new AirlineID: ");
                    String newAirlineID = scanner.nextLine();
                    System.out.print("Enter the new AirportID: ");
                    String newAirportID = scanner.nextLine();
                    System.out.print("Enter the new StartTime: ");
                    String newStartTime = scanner.nextLine();
                    System.out.print("Enter the new EndTime: ");
                    String newEndTime = scanner.nextLine();
                    System.out.print("Enter the new TurnaroundTime: ");
                    String newTurnaroundTime = scanner.nextLine();
                    System.out.print("Enter the new ParkingDuration: ");
                    String newParkingDuration = scanner.nextLine();
                    System.out.print("Enter the new Season: ");
                    String newSeason = scanner.nextLine();

                    try {
                        conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
                        pstmt = conn.prepareStatement(
                                "UPDATE Slots SET AirlineID = ?, AirportID = ?, StartTime = ?, EndTime = ?, TurnaroundTime = ?, ParkingDuration = ?, Season = ? WHERE SlotID = ?");
                        pstmt.setString(1, newAirlineID);
                        pstmt.setString(2, newAirportID);
                        pstmt.setString(3, newStartTime);
                        pstmt.setString(4, newEndTime);
                        pstmt.setString(5, newTurnaroundTime);
                        pstmt.setString(6, newParkingDuration);
                        pstmt.setString(7, newSeason);
                        pstmt.setString(8, updateSlotID);
                        pstmt.executeUpdate();
                        System.out.println("Slot updated successfully!");
                    } catch (SQLException e) {
                        System.out.println("SQL Exception: " + e.getMessage());
                        System.err.println("Error: Unable to update slot. Did you enter the correct SlotID?");
                    } finally {
                        try {
                            if (pstmt != null)
                                pstmt.close();
                            if (conn != null)
                                conn.close();
                        } catch (SQLException se) {
                            se.printStackTrace();
                        }
                    }
                    break;

                default:
                    // Invalid command
                    System.out.println("Invalid command. Type '?' for help.");
                    break;
            }
        }
    }
}