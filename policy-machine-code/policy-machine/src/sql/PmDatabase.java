package sql;

import java.sql.Connection;
import java.sql.DriverManager;


public class PmDatabase {

    public PmDatabase() {
        }

    public static Connection getConnection() throws Exception {
    	Connection conn = null;
	//Edit the connection to a database
 	   try{
  	     Class.forName("com.mysql.jdbc.Driver");
 		 conn = DriverManager.getConnection("jdbc:mysql://localhost/policydb","root","password");
 	   } catch (Exception e) {
 		  System.out.println("SQLException: " + e.getMessage());
 	   }
 	   return conn;
    }
}
