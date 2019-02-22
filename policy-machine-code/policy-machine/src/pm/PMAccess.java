package pm;


import java.sql.Connection;
import java.sql.SQLException;

/*import sql.MySQL_Functions;
import sql.MySQL_Parameters;
import sql.ParamType;
import sql.PmDatabase;*/

public class PMAccess {
    //public PmDatabase pmDatabase = new PmDatabase();
    public static Connection conn = null;
        static long s = System.currentTimeMillis();
        static long end = System.currentTimeMillis();
		public static void main(String[] args) throws Exception {
		    //Add struct here for decision info
		    System.out.println("Start");
		    String user = null;
			String object = null;
			user = args[0];
			object = args[1];
			//conn = PmDatabase.getConnection();
			System.out.println("DB connected");
			//Add sys_read call here to set up decision info struct
			//Add sys_setxattr call here to let kernel know we're up and running
			//Add code here to change the user and object passed to getAllowedOperations
			//Add loop here to infinitely process answers
			//System.out.println(getAllowedOperations(user,object));
	        System.out.println("TIME to process (Milliseconds):  " + (double)(end-s)/1000);

		}
		
    /*public static String getAllowedOperations(String user, String object) throws SQLException  {
	    	String ALLOWED_OPERATIONS = MySQL_Functions.GET_ALLOWED_OPERATOION_FOR_USER;
	    	MySQL_Parameters params = new MySQL_Parameters();
	    	params.setOutParamType(ParamType.STRING);
	        params.addParam(ParamType.STRING, user==null?null:user);
	        params.addParam(ParamType.STRING, object==null?null:object);
	        Object returned = null;
			try {
				returned = MySQL_Functions.executeFunction(ALLOWED_OPERATIONS, params);
			} catch (Exception e) {
				throw new SQLException(e);
			} finally {
			    conn.close();
			}
			
			if (returned == null ) {
				return "No Access";
			} else if (returned.equals("0")) {
				return "User " + user + " and/or " + "File " + object + " does not exist. Check the names and try again.";
			} else return returned.toString();
			}*/

}
