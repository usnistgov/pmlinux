//import java.util.Random;
import java.io.*;

public class pmtest {

    native String[] init();
    native String[] yes();
    native String[] no();
    
    public static void main(String[] args) {
	System.loadLibrary("pm");
	
	pmtest test = new pmtest();
	String[] decision_info = test.init();
	
	int x = 1;
	while(x>0) {
	    
	    int ans = get_decision(decision_info[0], decision_info[1], decision_info[2], decision_info[3]);
	    
	    if (ans == 1) {
		decision_info = test.yes();
	    }
	    else {    
		decision_info = test.no();
	    }
	   
	}
    }

    public static int get_decision(String process_id, String user_id, String pathname, String system_call) {
	
	try {
	    BufferedReader reader = new BufferedReader(new FileReader("db.txt"));
	    String line;
	    while ((line = reader.readLine()) != null && !line.contains(system_call)){
	    }
	    if (line != null){
		while((line = reader.readLine()) != null && !pathname.contains(line)){
		}
		if (line != null) {
		    line = reader.readLine();
		    if (line.contains(user_id)) {
			reader.close();
			return 1;
		    }
		}
	    }

	    else {
		reader.close();
		return 0;
	    }
	}
	catch (Exception e)
	    {
		System.err.format("Exception occurred trying to read db.txt");
      		return 0;
	    }
	return 0;
    }
}
