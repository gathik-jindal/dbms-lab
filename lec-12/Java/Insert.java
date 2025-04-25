import java.sql.*;
import java.util.Scanner;

public class Insert {

    static final String JDBC_DRIVER = "com.mysql.cj.jdbc.Driver";
    static final String DB_URL = "jdbc:mysql://localhost:3306/companydb";

    static final String USER = "root";
    static final String PASSWORD = "mysql";

    public static void main(String[] args) {
        Connection conn = null;
        Statement stmt = null;

        Scanner scanner = new Scanner(System.in);

        System.out.println("Enter the following employee details one by one");

        System.out.print("SSN: ");
        String ssn = scanner.next();
        System.out.print("fname: ");
        String fname = scanner.next();
        System.out.print("minit: ");
        String minit = scanner.next();
        System.out.print("lname: ");
        String lname = scanner.next();
        System.out.print("bdate (YYYY-MM-DD): ");
        String bdate = scanner.next();
        System.out.print("address: ");
        String address = scanner.next();
        System.out.print("sex: ");
        String sex = scanner.next();
        System.out.print("salary: ");
        Double salary = scanner.nextDouble();
        System.out.print("super_ssn: ");
        String super_ssn = scanner.next();
        System.out.print("dno: ");
        Integer dno = scanner.nextInt();
        // assuming only one project for now
        System.out.print("pno: ");
        Integer pno = scanner.nextInt();
        System.out.print("number of hours: ");
        Double hours = scanner.nextDouble();

        try {
            Class.forName(JDBC_DRIVER);
            conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);
            conn.setAutoCommit(false);
            stmt = conn.createStatement();
            System.out.println("Connected to Database! AutoCommit is off\nCreating statement...");

            // Execute the insert statement
            System.out.println("Inserting records into the table...");

            PreparedStatement ps = conn.prepareStatement(
                    "INSERT INTO employee (ssn, fname, minit, lname, bdate, address, sex, salary, super_ssn, dno) " +
                            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            ps.setString(1, ssn);
            ps.setString(2, fname);
            ps.setString(3, minit);
            ps.setString(4, lname);
            ps.setDate(5, java.sql.Date.valueOf(bdate)); // bdate = "1965-09-01"
            ps.setString(6, address);
            ps.setString(7, sex);
            ps.setDouble(8, salary);
            ps.setString(9, super_ssn);
            ps.setInt(10, dno);

            PreparedStatement psWorksOn = conn.prepareStatement(
                    "INSERT INTO works_on (essn, pno, hours) VALUES (?, ?, ?)");
            psWorksOn.setString(1, ssn);
            psWorksOn.setInt(2, pno);
            psWorksOn.setDouble(3, hours);

            System.out.println("Employee: " + ps.toString());
            System.out.println("Works On: " + psWorksOn.toString());

            ps.executeUpdate();
            psWorksOn.executeUpdate();
            System.out.println("Inserted records into the table...");

            // Commit the transaction
            System.out.println("Committing the transaction based on the hours > 0 condition...");
            if (hours > 0) {
                conn.commit();
            } else {
                System.out.println("Rolling back the transaction as hours <= 0...");
                conn.rollback();
            }
            System.out.println("Transaction committed/rolled back successfully!");

        } catch (SQLException se) {
            // Handle errors for JDBC
            try {
                if (conn != null) {
                    conn.rollback();
                    System.out.println("Failed and rolled back!");
                }
            } catch (SQLException rollbackEx) {
                rollbackEx.printStackTrace();
            }
            se.printStackTrace();
        } catch (Exception e) { // Handle errors for Class.forName
            e.printStackTrace();
        } finally { // finally block used to close resources regardless of whether an exception was
                    // thrown or not
            try {
                if (stmt != null)
                    stmt.close();
            } catch (SQLException se2) {
            }
            try {
                if (conn != null)
                    conn.close();
            } catch (SQLException se) {
                se.printStackTrace();
            }
        }
    }
}