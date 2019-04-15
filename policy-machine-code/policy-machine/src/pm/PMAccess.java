package pm;


import java.sql.Connection;
import java.sql.SQLException;

import sql.MySQL_Functions;
import sql.MySQL_Parameters;
import sql.ParamType;
import sql.PmDatabase;

public class PMAccess {
    native String[] init();
    native String[] no();
    native String[] yes();
    native String get_username(String uid);
    
    public PmDatabase pmDatabase = new PmDatabase();
    public static Connection conn = null;
    static long s = System.currentTimeMillis();
    static long end = System.currentTimeMillis();
    
    public static void main(String[] args) throws Exception {
		    
	System.loadLibrary("pm");
	PMAccess policy_machine = new PMAccess(); 

	String user = null;
	String object = null;
	//user = args[0];
	//object = args[1];
	String username;
	String allowed_ops;
	String filename;
	boolean ans;
	//conn = PmDatabase.getConnection();
			
	String[] decision_info = policy_machine.init();
	System.out.println("Testing");
	int x = 0;
	while (x < 1) {
	    conn = PmDatabase.getConnection();
	    username = policy_machine.get_username(decision_info[1]);
	    System.out.println(username);
	    filename = get_filename(decision_info[2]);
	    allowed_ops = getAllowedOperations(username, filename);
	    ans = get_answer(allowed_ops, decision_info[3]);
	    System.out.println(ans);
	    if (ans == true) {
		System.out.println("yes");
		decision_info = policy_machine.yes();
	    }
	    if (ans == false) {
		System.out.println("no");
	       decision_info = policy_machine.no();
	    }
	 }
	 
    }
		
    public static String getAllowedOperations(String user, String object) throws SQLException  {

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
    }

    public static boolean get_answer(String allowed_operations, String system_call) {
	System.out.println(allowed_operations + " " + system_call);
	return allowed_operations.contains(system_call);
    }

    public static String get_filename(String full_path)
    {
	int index = full_path.lastIndexOf("/");
	String short_path = full_path.substring(index+1);
	return short_path;
    }

}
