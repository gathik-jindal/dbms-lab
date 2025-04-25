import java.sql.*;

public class Dependent {
    static final String JDBC_DRIVER = "com.mysql.cj.jdbc.Driver";
    static final String DB_URL = "jdbc:mysql://localhost:3306/companydb";

    static final String USER = "root";
    static final String PASSWORD = "mysql";

    public static void main(String[] args) {
        Connection conn = null;
        PreparedStatement pstmt = null;
        ResultSet rs = null;

        try {
            Class.forName(JDBC_DRIVER);
            conn = DriverManager.getConnection(DB_URL, USER, PASSWORD);

            // SQL: Join employee and dependent tables
            String sql = "SELECT e.ssn, e.fname, e.lname, d.dependent_name, d.relationship " +
                    "FROM employee e " +
                    "JOIN dependent d ON e.ssn = d.essn " +
                    "ORDER BY e.ssn";

            pstmt = conn.prepareStatement(sql);
            rs = pstmt.executeQuery();

            String lastSSN = "";
            int depCount = 1;

            System.out.println("* * * *");
            while (rs.next()) {
                String ssn = rs.getString("ssn");
                String fname = rs.getString("fname");
                String lname = rs.getString("lname");
                String depName = rs.getString("dependent_name");
                String relationship = rs.getString("relationship");

                // New employee header
                if (!ssn.equals(lastSSN)) {
                    if (!lastSSN.equals("")) {
                        System.out.println("* * * *");
                    }
                    System.out.println("Employee full name: " + fname + " " + lname);
                    depCount = 1;
                    lastSSN = ssn;
                }

                System.out.println("Dependent #" + depCount + " : " + depName + " (" + relationship + ")");
                depCount++;
            }

            if (!lastSSN.equals("")) {
                System.out.println("* * * *");
            }

        } catch (Exception e) {
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
    }
}